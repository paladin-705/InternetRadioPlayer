CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
} else {
    BUILD_FLAG = release
}

PROJECT_ROOT_PATH = $${PWD}/
SRC_PATH = $${PROJECT_ROOT_PATH}/src/
INC_PATH = $${PROJECT_ROOT_PATH}/src/includes/
IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/
RES_PATH = $${PROJECT_ROOT_PATH}/res/
UI_PATH = $${PROJECT_ROOT_PATH}/ui/

BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}/
BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}/

RCC_DIR = $${BUILD_PATH}/rcc/
UI_DIR = $${BUILD_PATH}/ui/
MOC_DIR = $${BUILD_PATH}/moc/
OBJECTS_DIR = $${BUILD_PATH}/obj/
DESTDIR = $${BIN_PATH}/

#######################################################
QT += qml quick network

TARGET = InternetRadioPlayerController

CONFIG += c++11

INCLUDEPATH += $${INC_PATH}/

SOURCES += \
    $${SRC_PATH}/main.cpp \
    $${SRC_PATH}/MusicPlayerController.cpp \
    $${SRC_PATH}/SocketClient.cpp \
    $${SRC_PATH}/ListViewStreamModel.cpp

HEADERS += \
    $${INC_PATH}/MusicPlayerController.h \
    $${INC_PATH}/DataStructures.h \
    $${INC_PATH}/SocketClient.h \
    $${INC_PATH}/ListViewStreamModel.h

RESOURCES += $${RES_PATH}/qml.qrc

QML_IMPORT_PATH =$${RES_PATH}/

QML_DESIGNER_IMPORT_PATH = $${RES_PATH}/

DEFINES += QT_DEPRECATED_WARNINGS

#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES +=


