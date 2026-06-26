/**
 *******************************************************************************
 * @file        mp23abs1_sensor.c
 * @author      ABOV Adaptation
 * @version     v1.0.0
 * @date        2024
 * @brief       MP23ABS1 analog MEMS microphone sensor driver for ABOV MCU
 *******************************************************************************
 * @attention
 *
 * This driver provides functionality for MP23ABS1 analog microphone on ABOV:
 * - Initialization and configuration using ABOV HAL
 * - Audio recording with timer-based sampling
 * - Software high-pass filtering for DC removal
 * - Multiple sampling rate support (8kHz - 48kHz)
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "mp23abs1_sensor.h"
#include "hal_adc.h"
#include "hal_pcu.h"
#include <string.h>

/* Private defines -----------------------------------------------------------*/
#define HP_FILTER_COEFF         0xF8    /* High-pass filter coefficient (less aggressive) */
#define HP_FILTER_SHIFT         256     /* High-pass filter shift value */
#define ADC_RESOLUTION          4096    /* 12-bit ADC resolution */
#define ADC_OFFSET              1705    /* ADC center offset (measured from microphone) */
#define SIGNAL_GAIN             8       /* Additional gain multiplier (1-16) */
#define ENABLE_RAW_ADC_DEBUG    0       /* 1 = verbose raw UART spam during recording */

/* Private variables ---------------------------------------------------------*/
static MP23ABS1_Context_t MP23ABS1_Context = {0};
static volatile bool bSampleReady = false;
static uint8_t s_debugFirstSample = 1;

/* Private function prototypes -----------------------------------------------*/
static int32_t MP23ABS1_GPIO_Init(void);
static void MP23ABS1_ProcessSample(uint16_t un16AdcValue);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Initialize MP23ABS1 analog microphone
 * @param  pInit: Pointer to initialization structure
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_Init(MP23ABS1_Init_t *pInit)
{
    /* Check parameters */
    if (pInit == NULL)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    if (pInit->SampleRate == 0 || pInit->Volume > 100 || 
        pInit->pBuffer == NULL || pInit->BufferSize == 0)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    /* Check if already initialized */
    if (MP23ABS1_Context.State != MP23ABS1_STATE_RESET)
    {
        return MP23ABS1_BUSY;
    }
    
    /* Validate sample rate - accept any positive value for flexibility */
    if (pInit->SampleRate == 0 || pInit->SampleRate > 48000)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    /* Reset context */
    memset(&MP23ABS1_Context, 0, sizeof(MP23ABS1_Context_t));
    
    /* Store configuration */
    MP23ABS1_Context.SampleRate = pInit->SampleRate;
    MP23ABS1_Context.Volume = pInit->Volume;
    MP23ABS1_Context.pBuffer = pInit->pBuffer;
    MP23ABS1_Context.BufferSize = pInit->BufferSize;
    MP23ABS1_Context.WriteIndex = 0;
    MP23ABS1_Context.SampleCount = 0;
    
    /* Initialize GPIO for PA0 (ADC is already initialized in user_adc.c) */
    if (MP23ABS1_GPIO_Init() != MP23ABS1_OK)
    {
        MP23ABS1_Context.State = MP23ABS1_STATE_ERROR;
        return MP23ABS1_ERROR;
    }
    
    /* Initialize high-pass filter */
    MP23ABS1_Context.HPFilter.Z = 0;
    MP23ABS1_Context.HPFilter.oldOut = 0;
    MP23ABS1_Context.HPFilter.oldIn = 0;
    
    /* Set state to ready */
    MP23ABS1_Context.State = MP23ABS1_STATE_READY;
    
    return MP23ABS1_OK;
}

/**
 * @brief  De-initialize MP23ABS1 sensor
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_DeInit(void)
{
    /* Stop recording if active */
    if (MP23ABS1_Context.State == MP23ABS1_STATE_RECORDING)
    {
        MP23ABS1_StopRecording();
    }
    
    /* Stop ADC */
    HAL_ADC_Stop(MP23ABS1_ADC_ID);
    /* Note: ADC is shared, don't de-initialize it */
    
    /* Reset context */
    memset(&MP23ABS1_Context, 0, sizeof(MP23ABS1_Context_t));
    MP23ABS1_Context.State = MP23ABS1_STATE_RESET;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Start audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_StartRecording(void)
{
    /* Check state */
    if (MP23ABS1_Context.State != MP23ABS1_STATE_READY && 
        MP23ABS1_Context.State != MP23ABS1_STATE_PAUSED)
    {
        return MP23ABS1_ERROR;
    }
    
    /* Reset buffer index if starting fresh */
    if (MP23ABS1_Context.State == MP23ABS1_STATE_READY)
    {
        MP23ABS1_Context.WriteIndex = 0;
        MP23ABS1_Context.SampleCount = 0;
        s_debugFirstSample = 1;
    }
    
    /* Update state */
    MP23ABS1_Context.State = MP23ABS1_STATE_RECORDING;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Stop audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_StopRecording(void)
{
    /* Check state */
    if (MP23ABS1_Context.State != MP23ABS1_STATE_RECORDING &&
        MP23ABS1_Context.State != MP23ABS1_STATE_PAUSED)
    {
        return MP23ABS1_ERROR;
    }
    
    /* Stop ADC continuous mode */
    HAL_ADC_Stop(MP23ABS1_ADC_ID);
    
    /* Update state */
    MP23ABS1_Context.State = MP23ABS1_STATE_READY;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Pause audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_PauseRecording(void)
{
    /* Check state */
    if (MP23ABS1_Context.State != MP23ABS1_STATE_RECORDING)
    {
        return MP23ABS1_ERROR;
    }
    
    /* Update state */
    MP23ABS1_Context.State = MP23ABS1_STATE_PAUSED;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Resume audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_ResumeRecording(void)
{
    /* Check state */
    if (MP23ABS1_Context.State != MP23ABS1_STATE_PAUSED)
    {
        return MP23ABS1_ERROR;
    }
    
    /* Update state */
    MP23ABS1_Context.State = MP23ABS1_STATE_RECORDING;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Set sampling frequency
 * @param  SampleRate: New sampling rate (any value from 100 Hz to 48 kHz)
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_SetSampleRate(uint32_t SampleRate)
{
    /* Check if recording */
    if (MP23ABS1_Context.State == MP23ABS1_STATE_RECORDING)
    {
        return MP23ABS1_BUSY;
    }
    
    /* Validate sample rate - accept flexible range */
    if (SampleRate == 0 || SampleRate > 48000)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    MP23ABS1_Context.SampleRate = SampleRate;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Get current sampling frequency
 * @param  pSampleRate: Pointer to store current sample rate
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_GetSampleRate(uint32_t *pSampleRate)
{
    if (pSampleRate == NULL)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    *pSampleRate = MP23ABS1_Context.SampleRate;
    return MP23ABS1_OK;
}

/**
 * @brief  Set volume level
 * @param  Volume: Volume level (0-100)
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_SetVolume(uint32_t Volume)
{
    if (Volume > 100)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    MP23ABS1_Context.Volume = Volume;
    return MP23ABS1_OK;
}

/**
 * @brief  Get current volume level
 * @param  pVolume: Pointer to store current volume
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_GetVolume(uint32_t *pVolume)
{
    if (pVolume == NULL)
    {
        return MP23ABS1_INVALID_PARAM;
    }
    
    *pVolume = MP23ABS1_Context.Volume;
    return MP23ABS1_OK;
}

/**
 * @brief  Get current sensor state
 * @retval Current state
 */
MP23ABS1_State_t MP23ABS1_GetState(void)
{
    return MP23ABS1_Context.State;
}

/**
 * @brief  Get number of samples recorded
 * @retval Number of samples in buffer
 */
uint32_t MP23ABS1_GetSampleCount(void)
{
    return MP23ABS1_Context.SampleCount;
}

/**
 * @brief  Timer interrupt handler - Must be called from Timer ISR
 * @note   Call this function at the desired sampling rate
 * @retval None
 */
extern uint8_t SeqAdc_ReadMicSample(uint16_t *pun16MicRaw);

void MP23ABS1_TimerHandler(void)
{
    uint16_t un16AdcValue;

    if (MP23ABS1_Context.State != MP23ABS1_STATE_RECORDING)
        return;

    /* SEQ mode: one Start runs ch0 (mic) then ch23 (VCORE) — see prv_user_code.c */
    if (SeqAdc_ReadMicSample(&un16AdcValue) == 0U)
        MP23ABS1_ProcessSample(un16AdcValue);
}

/**
 * @brief  Apply high-pass filter to remove DC offset
 * @param  InputSample: Input audio sample (16-bit ADC value)
 * @param  pFilter: Pointer to HP filter state
 * @retval Filtered output sample (16-bit)
 */
int16_t MP23ABS1_ApplyHPFilter(int32_t InputSample, MP23ABS1_HP_Filter_t *pFilter)
{
    int32_t output;
    
    /* Convert ADC value to signed and apply volume gain */
    int32_t signedSample = (InputSample - ADC_OFFSET);
    
    /* Apply volume gain */
    pFilter->Z = (signedSample * (int32_t)MP23ABS1_Context.Volume) / 100;
    
    /* Apply additional signal gain */
    pFilter->Z = pFilter->Z * SIGNAL_GAIN;
    
    /* High-pass filter: y[n] = alpha * (y[n-1] + x[n] - x[n-1]) */
    pFilter->oldOut = (HP_FILTER_COEFF * (pFilter->oldOut + pFilter->Z - pFilter->oldIn)) / HP_FILTER_SHIFT;
    pFilter->oldIn = pFilter->Z;
    
    /* Saturate to 16-bit range */
    output = MP23ABS1_SATURATE(pFilter->oldOut, -32760, 32760);
    
    return (int16_t)output;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initialize GPIO for analog microphone (PA0)
 * @note   ADC is already initialized in user_adc.c (ADC_ID_1)
 * @retval Status
 */
static int32_t MP23ABS1_GPIO_Init(void)
{
    HAL_ERR_e eErr;

    /* Configure PA0 as Analog Input */
    eErr = HAL_PCU_SetInOutMode(PCU_ID_A, PCU_PIN_ID_0, PCU_INOUT_ANG_INPUT);
    if (eErr != HAL_ERR_OK) return MP23ABS1_ERROR;
    
    eErr = HAL_PCU_SetAltMode(PCU_ID_A, PCU_PIN_ID_0, PCU_ALT_7);
    if (eErr != HAL_ERR_OK) return MP23ABS1_ERROR;
    
    return MP23ABS1_OK;
}

/**
 * @brief  Process single ADC sample
 * @param  un16AdcValue: Raw ADC value
 * @retval None
 */
static void MP23ABS1_ProcessSample(uint16_t un16AdcValue)
{
    int16_t filteredSample;
    
    /* Debug: Print raw ADC values continuously (comma-separated, no trailing comma) */
    #if ENABLE_RAW_ADC_DEBUG
    extern void DebugFramework_PutDec32(uint32_t val);
    extern void DebugFramework_Puts(const char *str);
    extern void DebugFramework_PutsLine(const char *str);
    if (!s_debugFirstSample) {
        DebugFramework_Puts(",");
    }
    if (s_debugFirstSample) {
        DebugFramework_Puts("Microphone ADC Value:");
    }
    s_debugFirstSample = 0;
    DebugFramework_PutDec32(un16AdcValue);
    #endif

    /* Apply high-pass filter to remove DC offset */
    filteredSample = MP23ABS1_ApplyHPFilter((int32_t)un16AdcValue, &MP23ABS1_Context.HPFilter);
    
    /* Store in buffer */
    MP23ABS1_Context.pBuffer[MP23ABS1_Context.WriteIndex] = (uint16_t)(filteredSample + 32768);
    
    /* Update write index (circular buffer) */
    MP23ABS1_Context.WriteIndex++;
    if (MP23ABS1_Context.WriteIndex >= MP23ABS1_Context.BufferSize)
    {
        MP23ABS1_Context.WriteIndex = 0;
        
        /* Notify upper layer that buffer is full */
        MP23ABS1_BufferReady_Callback(MP23ABS1_Context.pBuffer, MP23ABS1_Context.BufferSize);
    }
    
    /* Update sample count */
    MP23ABS1_Context.SampleCount++;
}

/* Weak callback implementations ---------------------------------------------*/

/**
 * @brief  Buffer ready callback (weak implementation)
 * @note   Override this function in your application
 * @param  pBuffer: Pointer to buffer with audio data
 * @param  Size: Number of samples
 */
__attribute__((weak)) void MP23ABS1_BufferReady_Callback(uint16_t *pBuffer, uint32_t Size)
{
    /* Prevent unused parameter warning */
    (void)pBuffer;
    (void)Size;
    
    /* User should implement this function to process audio data */
}

/**
 * @brief  Error callback (weak implementation)
 * @note   Override this function in your application
 */
__attribute__((weak)) void MP23ABS1_Error_Callback(void)
{
    /* User should implement this function to handle errors */
}
