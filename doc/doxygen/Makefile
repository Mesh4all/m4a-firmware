REPO_BASE=$(shell git rev-parse --show-toplevel)
# Generate list of quoted absolute include paths. Evaluated in riot.doxyfile.
export STRIP_FROM_INC_PATH_LIST=$(shell \
    git ls-tree -dr --full-tree --name-only HEAD core drivers sys |\
    grep '/include$$' |\
    sed 's/.*/\"$(subst /,\/,$(REPO_BASE))\/\0\"/')

# use lessc (http://lesscss.org/#using-less) for compiling CSS
LESSC ?= $(shell command -v lessc 2>/dev/null)

.PHONY: doc
doc: html

# by marking html as phony we force make to re-run Doxygen even if the directory exists.
.PHONY: html
html: src/css/m4a.css
	( cat m4a.doxyfile ; echo "GENERATE_HTML = yes" ) | doxygen -

clean:
	-@rm -rf latex html doxygen_objdb_*.tmp doxygen_entrydb_*.tmp