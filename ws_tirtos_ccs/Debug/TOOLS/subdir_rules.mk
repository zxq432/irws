################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1939792865:
	@$(MAKE) -Onone -f TOOLS/subdir_rules.mk build-1939792865-inproc

build-1939792865-inproc: ../TOOLS/rom.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_03_33_core/xs" --xdcpath="I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source;I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/kernel/tirtos/packages;I:/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640R2F -r debug -c "I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me -O4 --opt_for_speed=0 --include_path=\"C:/ti/workspace_v7/beacon_rfdriver_cc2640r2rsm\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/target\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/hal/src/target/_common\" --include_path=\"I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2\" --include_path=\"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include\" --define=DeviceFamily_CC26X0R2 --define=BOARD_DISPLAY_USE_LCD=0 --define=BOARD_DISPLAY_USE_UART=1 --define=BOARD_DISPLAY_USE_UART_ANSI=1 --define=CC2640R2_RSM --define=CC26XX --define=CC26XX_R2 --define=ICALL_EVENTS --define=POWER_SAVING --define=RF_SINGLEMODE --define=xUSE_ICALL --define=ADV_NC_OPTIMIZED --define=EVERY_OTHER --define=xCC2650_LAUNCHXL --define=xCC26XXWARE --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --relaxed_ansi --plain_char=unsigned --diag_suppress=48 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --embedded_constants=on --unaligned_access=on --enum_type=packed --wchar_t=16 --abi=eabi --common=on --fp_reassoc=off --sat_reassoc=off  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-1939792865 ../TOOLS/rom.cfg
configPkg/compiler.opt: build-1939792865
configPkg/: build-1939792865


