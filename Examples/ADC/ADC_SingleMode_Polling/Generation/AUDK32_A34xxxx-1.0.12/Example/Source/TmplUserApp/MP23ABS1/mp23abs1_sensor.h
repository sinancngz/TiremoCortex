/**
 *******************************************************************************
 * @file        mp23abs1_sensor.h
 * @author      ABOV Adaptation
 * @version     v1.0.0
 * @date        2024
 * @brief       MP23ABS1 analog MEMS microphone sensor driver for ABOV MCU
 *******************************************************************************
 * @attention
 *
 * MP23ABS1 is a high performance MEMS audio sensor single ended analog 
 * bottom-port microphone from STMicroelectronics.
 * 
 * This driver uses:
 * - ADC (Analog-to-Digital Converter) to acquire analog signal from PA0
 * - Timer interrupt for sampling rate control
 * - Software high-pass filter for DC offset removal
 * - Circular buffer for audio data storage
 *
 *******************************************************************************
 */

#ifndef __MP23ABS1_SENSOR_H
#define __MP23ABS1_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "hal_adc.h"

/* Exported defines ----------------------------------------------------------*/

/**
 * @brief MP23ABS1 Sampling Frequencies
 */
#define MP23ABS1_FREQ_8KHZ          8000U
#define MP23ABS1_FREQ_16KHZ         16000U
#define MP23ABS1_FREQ_32KHZ         32000U
#define MP23ABS1_FREQ_48KHZ         48000U

/**
 * @brief Default Configuration
 */
#define MP23ABS1_DEFAULT_FREQ       MP23ABS1_FREQ_16KHZ
#define MP23ABS1_BITS_PER_SAMPLE    16U
#define MP23ABS1_CHANNELS           1U
#define MP23ABS1_DEFAULT_VOLUME     64U

/**
 * @brief ADC Configuration for MP23ABS1 on ABOV
 * Using ADC_ID_1 Channel 0 (PA0)
 */
#define MP23ABS1_ADC_ID             ADC_ID_0
#define MP23ABS1_ADC_CHANNEL        0

/**
 * @brief Buffer size calculation macro
 * Calculates samples per millisecond based on frequency
 */
#define MP23ABS1_SAMPLES_PER_MS(freq)   ((freq) / 1000U)

/**
 * @brief Status return codes
 */
#define MP23ABS1_OK                 0
#define MP23ABS1_ERROR             -1
#define MP23ABS1_INVALID_PARAM     -2
#define MP23ABS1_NOT_INITIALIZED   -3
#define MP23ABS1_BUSY              -4

/* Exported types ------------------------------------------------------------*/

/**
 * @brief MP23ABS1 State enumeration
 */
typedef enum
{
    MP23ABS1_STATE_RESET = 0,       /**< Peripheral not initialized */
    MP23ABS1_STATE_READY,           /**< Peripheral initialized and ready */
    MP23ABS1_STATE_RECORDING,       /**< Recording in progress */
    MP23ABS1_STATE_PAUSED,          /**< Recording paused */
    MP23ABS1_STATE_ERROR            /**< Error state */
} MP23ABS1_State_t;

/**
 * @brief High-Pass Filter state structure
 * Used to remove DC offset from audio signal
 */
typedef struct
{
    int32_t Z;                      /**< Current input sample */
    int32_t oldOut;                 /**< Previous output sample */
    int32_t oldIn;                  /**< Previous input sample */
} MP23ABS1_HP_Filter_t;

/**
 * @brief MP23ABS1 Initialization structure
 */
typedef struct
{
    uint32_t SampleRate;            /**< Audio sampling frequency (100Hz - 48kHz) */
    uint32_t Volume;                /**< Volume level (0-100) */
    uint16_t *pBuffer;              /**< Pointer to audio buffer */
    uint32_t BufferSize;            /**< Buffer size in samples */
} MP23ABS1_Init_t;

/**
 * @brief MP23ABS1 Context structure
 * Contains all runtime information for the sensor
 */
typedef struct
{
    MP23ABS1_State_t State;         /**< Current sensor state */
    uint32_t SampleRate;            /**< Current sampling rate */
    uint32_t Volume;                /**< Current volume (0-100) */
    uint16_t *pBuffer;              /**< Audio data buffer pointer */
    uint32_t BufferSize;            /**< Buffer size in samples */
    MP23ABS1_HP_Filter_t HPFilter;  /**< High-pass filter state */
    
    /* Internal buffer management */
    uint32_t WriteIndex;            /**< Current write position in buffer */
    uint32_t SampleCount;           /**< Total samples recorded */
    
    /* Timing */
    uint32_t TimerInterval;         /**< Timer interval for sampling */
    
} MP23ABS1_Context_t;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Initialize MP23ABS1 analog microphone
 * @param  pInit: Pointer to initialization structure
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_Init(MP23ABS1_Init_t *pInit);

/**
 * @brief  De-initialize MP23ABS1 sensor
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_DeInit(void);

/**
 * @brief  Start audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_StartRecording(void);

/**
 * @brief  Stop audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_StopRecording(void);

/**
 * @brief  Pause audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_PauseRecording(void);

/**
 * @brief  Resume audio recording
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_ResumeRecording(void);

/**
 * @brief  Set sampling frequency
 * @param  SampleRate: New sampling rate (100 Hz to 48 kHz)
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_SetSampleRate(uint32_t SampleRate);

/**
 * @brief  Get current sampling frequency
 * @param  pSampleRate: Pointer to store current sample rate
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_GetSampleRate(uint32_t *pSampleRate);

/**
 * @brief  Set volume level
 * @param  Volume: Volume level (0-100)
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_SetVolume(uint32_t Volume);

/**
 * @brief  Get current volume level
 * @param  pVolume: Pointer to store current volume
 * @retval Status: MP23ABS1_OK or error code
 */
int32_t MP23ABS1_GetVolume(uint32_t *pVolume);

/**
 * @brief  Get current sensor state
 * @retval Current state
 */
MP23ABS1_State_t MP23ABS1_GetState(void);

/**
 * @brief  Timer interrupt handler - Must be called from Timer ISR
 * @note   This function handles periodic ADC sampling
 * @retval None
 */
void MP23ABS1_TimerHandler(void);

/**
 * @brief  Apply high-pass filter to remove DC offset
 * @param  InputSample: Input audio sample (16-bit ADC value)
 * @param  pFilter: Pointer to HP filter state
 * @retval Filtered output sample (16-bit)
 */
int16_t MP23ABS1_ApplyHPFilter(int32_t InputSample, MP23ABS1_HP_Filter_t *pFilter);

/**
 * @brief  Get number of samples recorded
 * @retval Number of samples in buffer
 */
uint32_t MP23ABS1_GetSampleCount(void);

/* Callback functions (weak) -------------------------------------------------*/

/**
 * @brief  Buffer ready callback
 * @note   This function should be implemented by user application
 * @param  pBuffer: Pointer to buffer with audio data
 * @param  Size: Number of samples available
 */
void MP23ABS1_BufferReady_Callback(uint16_t *pBuffer, uint32_t Size);

/**
 * @brief  Error callback
 * @note   This function should be implemented by user application
 */
void MP23ABS1_Error_Callback(void);

/* Helper macros -------------------------------------------------------------*/

/**
 * @brief Saturate value between min and max
 */
#define MP23ABS1_SATURATE(val, min, max)  (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

/**
 * @brief Convert 12-bit ADC value to signed 16-bit audio sample
 */
#define MP23ABS1_ADC_TO_AUDIO(adc)  ((int16_t)(((int32_t)(adc) - 2048) << 4))

#ifdef __cplusplus
}
#endif

#endif /* __MP23ABS1_SENSOR_H */
