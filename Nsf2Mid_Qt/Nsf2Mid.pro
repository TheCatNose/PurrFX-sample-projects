QT     += core widgets gui
CONFIG += c++11

SOURCES += \
    ../Nsf2Mid/CMidiFile.cpp \
main.cpp \
    cmainwidget.cpp

HEADERS += \
    ../Nsf2Mid/CMidiFile.h \
cmainwidget.h

FORMS += \
cmainwidget.ui

# Common
INCLUDEPATH += ../common/Qt/
HEADERS += ../common/Qt/DString.h

# PurrFX
INCLUDEPATH += ../
SOURCES += $$files(../PurrFX/*.cpp, true)
HEADERS += $$files(../PurrFX/*.h, true)
SOURCES -= $$files(../PurrFX/prj/*.cpp, true)
HEADERS -= $$files(../PurrFX/prj/*.h, true)
