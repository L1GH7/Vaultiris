TARGET = GSSCred-move-uaf

ARCH       ?= x86_64
SDK        ?= macosx
SIGNING_ID ?= -

SYSROOT  := $(shell xcrun --sdk $(SDK) --show-sdk-path)
ifeq ($(SYSROOT),)
$(error Could not find SDK "$(SDK)")
endif
CLANG    := $(shell xcrun --sdk $(SDK) --find clang)
CC       := $(CLANG) -isysroot $(SYSROOT) -arch $(ARCH)
CODESIGN := codesign

CFLAGS = -O2 -Wall -Werror -Wpedantic -Wno-gnu

FRAMEWORKS = -framework Foundation

SOURCES = $(TARGET).c

HEADERS =

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(FRAMEWORKS) $(DEFINES) -o $@ $(SOURCES)
	$(CODESIGN) -s '$(SIGNING_ID)' $@

clean:
	rm -f -- $(TARGET)
