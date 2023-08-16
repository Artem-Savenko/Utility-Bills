# APP INFO
VERSION = 1.1.0
QMAKE_TARGET_DESCRIPTION = "Program for managing utility bills"
#    //////////////////////////////////////////////////////

QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    Model.cpp \
    Common.cpp \
    CmboxModel.cpp \
    BillTypesModel.cpp \
    ModelSummary.cpp \
    bill.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS += appIcon.ico

HEADERS += Model.h \
    Common.h \
    CmboxModel.h \
    BillTypesModel.h \
    ModelSummary.h \
    bill.h
