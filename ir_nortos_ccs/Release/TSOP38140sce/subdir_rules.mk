################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
TSOP38140sce/scif_framework.obj: ../TSOP38140sce/scif_framework.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O2 --include_path="C:/ti/workspace_v7/ir_nortos_ccs" --include_path="C:/ti/workspace_v7/ir_nortos_ccs/HX711sce" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/inc" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common/cc26xx" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos/posix" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 --define=POWER_SAVING --define=sensor=23 --define=ADV_NC_OPTIMIZED -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="TSOP38140sce/scif_framework.d_raw" --obj_directory="TSOP38140sce" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

TSOP38140sce/scif_osal_none.obj: ../TSOP38140sce/scif_osal_none.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O2 --include_path="C:/ti/workspace_v7/ir_nortos_ccs" --include_path="C:/ti/workspace_v7/ir_nortos_ccs/HX711sce" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/inc" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common/cc26xx" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos/posix" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 --define=POWER_SAVING --define=sensor=23 --define=ADV_NC_OPTIMIZED -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="TSOP38140sce/scif_osal_none.d_raw" --obj_directory="TSOP38140sce" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

TSOP38140sce/scif_tsop38140.obj: ../TSOP38140sce/scif_tsop38140.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O2 --include_path="C:/ti/workspace_v7/ir_nortos_ccs" --include_path="C:/ti/workspace_v7/ir_nortos_ccs/HX711sce" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/inc" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common/cc26xx" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos" --include_path="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/nortos/posix" --include_path="I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=DeviceFamily_CC26X0R2 --define=POWER_SAVING --define=sensor=23 --define=ADV_NC_OPTIMIZED -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="TSOP38140sce/scif_tsop38140.d_raw" --obj_directory="TSOP38140sce" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


