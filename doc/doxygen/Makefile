.PHONY: doc
doc: html

.PHONY: html
html: src/css/custom.css
	( cat m4a.doxyfile ; echo "GENERATE_HTML = yes" ) | doxygen -


clean:
	-@rm -rf latex html doxygen_objdb_*.tmp doxygen_entrydb_*.tmp