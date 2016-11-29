#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T16:29:03
#
#-------------------------------------------------
CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
} else {
    BUILD_FLAG = release
}

PROJECT_ROOT_PATH = $${PWD}/
LIBS_PATH = $${PROJECT_ROOT_PATH}/lib.$${OS_SUFFIX}/
INC_PATH = $${PROJECT_ROOT_PATH}/src/includes/
IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/
BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}/

QT       += core network sql
QT       -= gui

TARGET = InternetRadioPlayer
CONFIG   += console
CONFIG   -= app_bundle

BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}/
RCC_DIR = $${BUILD_PATH}/rcc/
UI_DIR = $${BUILD_PATH}/ui/
MOC_DIR = $${BUILD_PATH}/moc/
OBJECTS_DIR = $${BUILD_PATH}/obj/
DESTDIR = $${BIN_PATH}/

LIBS += -L$${LIBS_PATH}/ -lbass
INCLUDEPATH += $${INC_PATH}/
INCLUDEPATH += $${IMPORT_PATH}/

TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/RadioClient.cpp \
    src/RadioPlayer.cpp \
    src/RadioPlayerStream.cpp \
    src/RadioServer.cpp \
    src/MusicStreamsBase.cpp \

HEADERS += \
    lib/bass.h \
    $${INC_PATH}/RadioClient.h \
    $${INC_PATH}/RadioPlayer.h \
    $${INC_PATH}/RadioPlayerStream.h \
    $${INC_PATH}/RadioServer.h \
    $${INC_PATH}/MusicStreamsBase.h \
    $${INC_PATH}/DataStructures.h
