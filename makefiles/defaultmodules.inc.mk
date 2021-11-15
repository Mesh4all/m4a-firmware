DEFAULT_MODULE +=

# Include potentially added default modules by the board
-include $(BOARDDIR)/Makefile.default

# Include potentially added default modules by the CPU
-include $(RIOTCPU)/$(CPU)/Makefile.default
