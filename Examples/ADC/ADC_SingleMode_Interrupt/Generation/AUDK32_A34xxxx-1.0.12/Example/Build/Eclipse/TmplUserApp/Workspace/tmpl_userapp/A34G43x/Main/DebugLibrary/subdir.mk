################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Source/TmplUserApp/DebugLibrary/debug_framework.c 

OBJS += \
./Main/DebugLibrary/debug_framework.o 

C_DEPS += \
./Main/DebugLibrary/debug_framework.d 


# Each subdirectory must supply rules for building sources it contributes
Main/DebugLibrary/debug_framework.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Source/TmplUserApp/DebugLibrary/debug_framework.c Main/DebugLibrary/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


