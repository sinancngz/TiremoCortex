################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug.c \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_cmd.c \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_hardfault.c \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_log.c \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_retarget.c \
C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_serial.c 

OBJS += \
./Debug/Source/debug.o \
./Debug/Source/debug_cmd.o \
./Debug/Source/debug_hardfault.o \
./Debug/Source/debug_log.o \
./Debug/Source/debug_retarget.o \
./Debug/Source/debug_serial.o 

C_DEPS += \
./Debug/Source/debug.d \
./Debug/Source/debug_cmd.d \
./Debug/Source/debug_hardfault.d \
./Debug/Source/debug_log.d \
./Debug/Source/debug_retarget.d \
./Debug/Source/debug_serial.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/Source/debug.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Debug/Source/debug_cmd.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_cmd.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Debug/Source/debug_hardfault.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_hardfault.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Debug/Source/debug_log.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_log.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Debug/Source/debug_retarget.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_retarget.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Debug/Source/debug_serial.o: C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Platform/Library/ABOV/Debug/Source/debug_serial.c Debug/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -DEXTRN_SUBFAMILY_A34G43x -DEXTRN_ABOV_MODULE_CONFIG -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Core/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Framework/CMSIS/Device/ABOV/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../ProductConfig/Config" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/HAL/HPL/Include" -I"C:/TiremoCortex/ADC_SingleMode_Interrupt/Generation/AUDK32_A34xxxx-1.0.12/Example/Build/Eclipse/TmplUserApp/Workspace/tmpl_userapp/../../../../../../Platform/Library/ABOV/Debug/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


