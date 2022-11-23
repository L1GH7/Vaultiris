# -*-makefile-*-
#
# Copyright (C) 2015 by Enrico Joerns <e.joerns@pengutronix.de>
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_RAUC) += rauc

#
# Paths and names
#
RAUC_VERSION	:= 1.5
RAUC_MD5	:= 83d86821473102682cd1ad2a4a9a7a4c
RAUC		:= rauc-$(RAUC_VERSION)
RAUC_SUFFIX	:= tar.xz
RAUC_URL	:= https://github.com/rauc/rauc/releases/download/v$(RAUC_VERSION)/$(RAUC).$(RAUC_SUFFIX)
RAUC_SOURCE	:= $(SRCDIR)/$(RAUC).$(RAUC_SUFFIX)
RAUC_DIR	:= $(BUILDDIR)/$(RAUC)
RAUC_LICENSE	:= LGPL-2.1-only

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#RAUC_CONF_ENV	:= $(CROSS_ENV)

#
# autoconf
#
RAUC_CONF_TOOL	:= autoconf
RAUC_CONF_OPT	:= \
	$(CROSS_AUTOCONF_USR) \
	--enable-debug=info \
	$(GLOBAL_LARGE_FILE_OPTION) \
	--disable-code-coverage \
	--disable-valgrind \
	--$(call ptx/endis,PTXCONF_RAUC_SERVICE)-service \
	--$(call ptx/endis,PTXCONF_RAUC_NETWORK)-network \
	--$(call ptx/endis,PTXCONF_RAUC_JSON)-json \
	--with-systemdunitdir=/usr/lib/systemd/system \
	--with-dbuspolicydir=/usr/share/dbus-1/system.d \
	--with-dbussystemservicedir=/usr/share/dbus-1/system-services

$(STATEDIR)/rauc.prepare:
	@$(call targetinfo)
	@test ! -e "$(call ptx/in-platformconfigdir, config/rauc/rauc.key.pem)" || \
		ptxd_bailout "Please use the signing provider infrastructure desribed in:" \
			"https://www.ptxdist.org/doc/dev_code_signing.html"
	@$(call world/prepare, RAUC)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/rauc.targetinstall:
	@$(call targetinfo)

	@$(call install_init, rauc)
	@$(call install_fixup, rauc,PRIORITY,optional)
	@$(call install_fixup, rauc,SECTION,base)
	@$(call install_fixup, rauc,AUTHOR,"Enrico Joerns <e.joerns@pengutronix.de>")
	@$(call install_fixup, rauc,DESCRIPTION,missing)

	@$(call install_copy, rauc, 0, 0, 0755, -, /usr/bin/rauc)

ifdef PTXCONF_RAUC_CONFIGURATION
	@$(call install_alternative, rauc, 0, 0, 0644, /etc/rauc/system.conf)
	@$(call install_replace, rauc, /etc/rauc/system.conf, \
		@RAUC_BUNDLE_COMPATIBLE@, \
		"$(call remove_quotes,$(PTXCONF_RAUC_COMPATIBLE))")
	@$(call install_copy, rauc, 0, 0, 0644, $(shell cs_get_ca update), \
		/etc/rauc/ca.cert.pem)
endif

ifdef PTXCONF_RAUC_SERVICE
	@$(call install_copy, rauc, 0, 0, 0644, -, \
		/usr/share/dbus-1/system-services/de.pengutronix.rauc.service)
	@$(call install_copy, rauc, 0, 0, 0644, -, \
		/usr/share/dbus-1/system.d/de.pengutronix.rauc.conf)
endif

ifdef PTXCONF_INITMETHOD_SYSTEMD
	@$(call install_alternative, rauc, 0, 0, 0644, \
		/usr/lib/systemd/system/rauc.service)

	@$(call install_alternative, rauc, 0, 0, 0644, \
		/usr/lib/systemd/system/rauc-mark-good.service)
	@$(call install_link, rauc, ../rauc-mark-good.service, \
		/usr/lib/systemd/system/multi-user.target.wants/rauc-mark-good.service)
else
	@$(call install_copy, rauc, 0, 0, 0755, -, \
		/usr/libexec/rauc-service.sh)
endif

	@$(call install_finish, rauc)

	@$(call touch)

# vim: syntax=make
