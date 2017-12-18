# invoke SourceDir generated makefile for rom.pem3
rom.pem3: .libraries,rom.pem3
.libraries,rom.pem3: package/cfg/rom_pem3.xdl
	$(MAKE) -f C:\ti\workspace_v7\beacon_rfdriver_cc2640r2rsm\TOOLS/src/makefile.libs

clean::
	$(MAKE) -f C:\ti\workspace_v7\beacon_rfdriver_cc2640r2rsm\TOOLS/src/makefile.libs clean

