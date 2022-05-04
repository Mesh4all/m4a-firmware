.PHONY: m4agen-installed generate-%

-include makefiles/color.inc.mk

m4agen-installed:
	@command -v m4agen > /dev/null 2>&1 || \
	{ $(COLOR_ECHO) \
	"$(COLOR_RED)'m4agen' command is not available \
	please consider installing it from \
	https://pypi.python.org/pypi/m4agen$(COLOR_RESET)"; \
	bash ./dist/tools/utils/m4agen_installer.sh; }

GENERATORS = board example module test
$(addprefix generate-,$(GENERATORS)): generate-%

generate-%: m4agen-installed
	@RIOTBASE=$(CURDIR) m4agen $* -i
