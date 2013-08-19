# This file is part of MXE.
# See index.html for further information.

PKG             := jsoncpp
$(PKG)_IGNORE   :=
$(PKG)_CHECKSUM := a14eb501c44e610b8aaa2962bd1cc1775ed4fde2
$(PKG)_SUBDIR   := $(PKG)-src-$($(PKG)_VERSION)
$(PKG)_FILE     := $(PKG)-src-$($(PKG)_VERSION).tar.gz
$(PKG)_URL      := http://$(SOURCEFORGE_MIRROR)/project/$(PKG)/$(PKG)/$($(PKG)_VERSION)/$($(PKG)_FILE)
$(PKG)_DEPS     := build-gcc

define $(PKG)_UPDATE
    $(WGET) -q -O- 'http://sourceforge.net/projects/jsoncpp/files/jsoncpp/' | \
    $(SED) -n 's,.*/\([0-9][^"]*\)/".*,\1,p' | \
    head -1
endef

define $(PKG)_BUILD
    cd '$(1)' && scons \
	platform=mingw-cross \
        MINGW_CROSS_PREFIX='$(MXE_TOOL_PREFIX)' \
        PREFIX='$(HOST_PREFIX)'
    $(INSTALL) -vd '$(HOST_PREFIX)/lib'
    $(INSTALL) -vd '$(HOST_PREFIX)/include/json'
    cd '$(1)' && $(INSTALL) -v 'libs/mingw-cross/libjson_mingw-cross_libmt.so' '$(HOST_PREFIX)/lib/jsoncpp.dll'
    cd '$(1)' && $(INSTALL) -v -m644 include/json/*.h '$(HOST_PREFIX)/include/json'
endef
