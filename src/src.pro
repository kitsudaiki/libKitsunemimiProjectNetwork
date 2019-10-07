QT       -= qt core gui

TARGET = KitsuneProjectCommon
TEMPLATE = lib
CONFIG += c++14
VERSION = 0.1.0

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

LIBS += -L../../libKitsuneNetwork/src -lKitsuneNetwork
LIBS += -L../../libKitsuneNetwork/src/debug -lKitsuneNetwork
LIBS += -L../../libKitsuneNetwork/src/release -lKitsuneNetwork
INCLUDEPATH += ../../libKitsuneNetwork/include/libKitsuneNetwork

LIBS += -L../../libKitsunePersistence/src -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/debug -lKitsunePersistence
LIBS += -L../../libKitsunePersistence/src/release -lKitsunePersistence
INCLUDEPATH += ../../libKitsunePersistence/include/libKitsunePersistence

LIBS +=  -lssl -lcrypt

INCLUDEPATH += $$PWD \
               $$PWD/../include/libKitsuneProjectCommon

HEADERS += \
    ../include/libKitsuneProjectCommon/network_session/session.h \
    ../include/libKitsuneProjectCommon/network_session/session_handler.h \
    network_session/timer_thread.h \
    network_session/callbacks.h \
    network_session/messages/message_processing.h \
    network_session/messages/message_creation.h \
    network_session/messages/session_init_processing.h \
    network_session/messages/session_end_processing.h \
    network_session/messages/message_definitions.h

SOURCES += \
    network_session/timer_thread.cpp \
    network_session/session_handler.cpp \
    network_session/session.cpp
