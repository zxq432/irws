## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/rom_pem3.oem3

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/rom_pem3.xdl
	$(SED) 's"^\"\(package/cfg/rom_pem3cfg.cmd\)\"$""\"C:/ti/workspace_v7/beacon_rfdriver_cc2640r2rsm/.config/xconfig_rom/\1\""' package/cfg/rom_pem3.xdl > $@
	-$(SETDATE) -r:max package/cfg/rom_pem3.h compiler.opt compiler.opt.defs
