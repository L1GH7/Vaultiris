TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/aesblock_decryptor.cpp \
    src/aeskey_decoder.cpp \
    src/b64ops.cpp
HEADERS += \
    src/aesblock_decryptor.h \
    src/aeskey_decoder.h \
    src/b64ops.h \
    src/cracking_options.h

DISTFILES += \
    PoC1.cred \
    PoC2.cred \
    PoC3.cred \
    PoC4.cred \
    README.md

QMAKE_INCDIR += src
INCLUDEPATH += src

LIBS += -lcrypto
