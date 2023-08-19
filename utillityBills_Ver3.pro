# APP INFO
VERSION = 1.1.0
QMAKE_TARGET_DESCRIPTION = "Program for managing utility bills"
#    //////////////////////////////////////////////////////

QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/main.cpp \
        src/Model.cpp \
        src/Common.cpp \
        src/CmboxModel.cpp \
        src/BillTypesModel.cpp \
        src/ModelSummary.cpp \
        src/bill.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS += img/appIcon.ico

HEADERS += src/Model.h \
           src/Common.h \
           src/CmboxModel.h \
           src/BillTypesModel.h \
           src/ModelSummary.h \
           src/bill.h
