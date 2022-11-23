include theos/makefiles/common.mk

TWEAK_NAME = CVE20144377
CVE20144377_FILES = Tweak.mm
CVE20144377_FRAMEWORKS = CoreFoundation

include $(THEOS_MAKE_PATH)/tweak.mk

after-install::
	install.exec "killall -9 MobileSafari"
