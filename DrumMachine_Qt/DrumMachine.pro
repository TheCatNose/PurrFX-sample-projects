QT       += core widgets gui multimedia

CONFIG += c++11

SOURCES += \
    ../DrumMachine/CSequencePlayer.cpp \
    CSamplesListModel.cpp \
main.cpp \
    cmainwidget.cpp

HEADERS += \
    ../DrumMachine/CSequencePlayer.h \
    CSamplesListModel.h \
cmainwidget.h

FORMS += \
cmainwidget.ui

# Common
INCLUDEPATH += ../common/Qt/
SOURCES += $$files(../common/Qt/*.cpp, false)
HEADERS += $$files(../common/Qt/*.h, false)

# PurrFX
INCLUDEPATH += ../PurrFX/
SOURCES += $$files(../PurrFX/*.cpp, true)
HEADERS += $$files(../PurrFX/*.h, true)
SOURCES -= $$files(../PurrFX/prj/*.cpp, true)
HEADERS -= $$files(../PurrFX/prj/*.h, true)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
