################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
nortos/startup/startup_cc13xx_cc26xx_ccs.obj: ../nortos/startup/startup_cc13xx_cc26xx_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O2 --include_path="C:/ti/workspace_v7/beacon_rfdriver_cc2640r2rsm_nortos_ccs" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos/posix" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="nortos/startup/startup_cc13xx_cc26xx_ccs.d_raw" --obj_directory="nortos/startup" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


