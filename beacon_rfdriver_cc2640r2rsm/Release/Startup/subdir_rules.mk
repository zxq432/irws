################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Startup/board.obj: C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/target/board.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O4 --opt_for_speed=0 --include_path="C:/ti/workspace_v7/beacon_rfdriver_cc2640r2rsm" --include_path="C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/target" --include_path="C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 --define=RF_SINGLEMODE --define=POWER_SAVING --define=ADV_NC_OPTIMIZED --define=EVERY_OTHER --define=CC2640R2_RSM --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL --c99 --relaxed_ansi --plain_char=unsigned --diag_suppress=48 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --embedded_constants=on --unaligned_access=on --enum_type=packed --wchar_t=16 --abi=eabi --common=on --fp_reassoc=off --sat_reassoc=off --preproc_with_compile --preproc_dependency="Startup/board.d_raw" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Startup/ccfg.obj: ../Startup/ccfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O4 --opt_for_speed=0 --include_path="C:/ti/workspace_v7/beacon_rfdriver_cc2640r2rsm" --include_path="C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/target" --include_path="C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 --define=RF_SINGLEMODE --define=POWER_SAVING --define=ADV_NC_OPTIMIZED --define=EVERY_OTHER --define=CC2640R2_RSM --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL --c99 --relaxed_ansi --plain_char=unsigned --diag_suppress=48 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --embedded_constants=on --unaligned_access=on --enum_type=packed --wchar_t=16 --abi=eabi --common=on --fp_reassoc=off --sat_reassoc=off --preproc_with_compile --preproc_dependency="Startup/ccfg.d_raw" --obj_directory="Startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


