# XSL stylesheets downloaded from http://docbook.sourceforge.net/release/xsl/current/html/
# Makefile from BitBake/OpenEmbedded manuals

topdir = .
manual = $(topdir)/usermanual.xml
# types = pdf txt rtf ps xhtml html man tex texi dvi
# types = pdf txt
types = $(docbooktotypes)
docbooktotypes = pdf txt
# htmlcssfile =
# htmlcss =
cleanfiles = $(foreach i,$(types),$(topdir)/$(i))

ifdef DEBUG
define command
	$(1)
endef
else
define command
	@echo $(2) $(3) $(4)
	@$(1)
endef
endif

all: $(types)

lint: $(manual) FORCE
	$(call command,xmllint --xinclude --postvalid --noout $(manual),XMLLINT $(manual))

$(types): lint FORCE


$(docbooktotypes): $(manual)
	$(call command,docbook2$@ $(manual),DOCBOOK2 $@ $(manual))

clean:
	rm -rf $(cleanfiles)

$(foreach i,$(types) $(foreach type,$(htmltypes),$(type)-nochunks),clean-$(i)):
	rm -rf $(patsubst clean-%,%,$@)

FORCE:
