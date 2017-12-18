################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./Application/main_tirtos.obj" \
"./Application/smartrf_settings.obj" \
$(GEN_CMDS__FLAG) \
-l"I:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/devices/cc26x0r2/driverlib/bin/ccs/driverlib.lib" \
-l"C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/rom/ble_rom_releases/cc26xx_r2/Final_Release/common_r2.symbols" \
-l"C:/ti/simplelink_cc2640r2_sdk_1_50_00_58/source/ti/blestack/common/cc26xx/ccs/cc26xx_app.cmd" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include Application/subdir_vars.mk
-include Application/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ir_tirtos_ccs.out \

EXE_OUTPUTS__QUOTED += \
"ir_tirtos_ccs.out" \

BIN_OUTPUTS += \
ir_tirtos_ccs.hex \

BIN_OUTPUTS__QUOTED += \
"ir_tirtos_ccs.hex" \


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ir_tirtos_ccs.out"

# Tool invocations
ir_tirtos_ccs.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me -O4 --opt_for_speed=0 --define=DeviceFamily_CC26X0R2 --define=BOARD_DISPLAY_USE_LCD=0 --define=BOARD_DISPLAY_USE_UART=1 --define=BOARD_DISPLAY_USE_UART_ANSI=1 --define=CC2640R2_RSM --define=CC26XX --define=CC26XX_R2 --define=ICALL_EVENTS --define=POWER_SAVING --define=RF_SINGLEMODE --define=xUSE_ICALL --define=ADV_NC_OPTIMIZED --define=EVERY_OTHER --define=xCC2650_LAUNCHXL --define=xCC26XXWARE --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --relaxed_ansi --plain_char=unsigned --diag_suppress=48 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --embedded_constants=on --unaligned_access=on --enum_type=packed --wchar_t=16 --abi=eabi --common=on --fp_reassoc=off --sat_reassoc=off -z -m"ir_tirtos_ccs.map" --heap_size=0 --stack_size=256 -i"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/lib" -i"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --reread_libs --define=CC26X0ROM=2 --diag_suppress=10247-D --diag_suppress=16002-D --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ir_tirtos_ccs_linkInfo.xml" --rom_model --unused_section_elimination=on -o "ir_tirtos_ccs.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

ir_tirtos_ccs.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armhex" --memwidth=8 --romwidth=8 --intel -o "ir_tirtos_ccs.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "Application\main_tirtos.obj" "Application\smartrf_settings.obj" 
	-$(RM) "Application\main_tirtos.d" "Application\smartrf_settings.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-I:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armhex -order MS --memwidth=8 --romwidth=8 --intel -o ir_tirtos_ccs.hex ir_tirtos_ccs.out
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

