################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
utils/%.obj: ../utils/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.2.0.295" --include_path="C:/Users/steph/workspace_v10/drum_pad/midi" --include_path="C:/Users/steph/workspace_v10/drum_pad/usb_midi" --include_path="C:/Users/steph/workspace_v10/drum_pad" --include_path="C:/Users/steph/workspace_v10/drum_pad/utils" --include_path="C:/Users/steph/workspace_v10/drum_pad/FreeRTOS/include" --include_path="C:/Users/steph/workspace_v10/drum_pad/FreeRTOS/portable/CCS/ARM_CM4F" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="utils/$(basename $(<F)).d_raw" --obj_directory="utils" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/ustdlib.obj: C:/ti/TivaWare_C_Series-2.2.0.295/utils/ustdlib.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.2.0.295" --include_path="C:/Users/steph/workspace_v10/drum_pad/midi" --include_path="C:/Users/steph/workspace_v10/drum_pad/usb_midi" --include_path="C:/Users/steph/workspace_v10/drum_pad" --include_path="C:/Users/steph/workspace_v10/drum_pad/utils" --include_path="C:/Users/steph/workspace_v10/drum_pad/FreeRTOS/include" --include_path="C:/Users/steph/workspace_v10/drum_pad/FreeRTOS/portable/CCS/ARM_CM4F" --include_path="C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="utils/$(basename $(<F)).d_raw" --obj_directory="utils" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


