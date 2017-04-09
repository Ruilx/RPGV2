#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T16:23:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = RPGV2
TEMPLATE = app


SOURCES += main.cpp\
        Mainw.cpp \
    RPG/com/RpgWidget.cpp \
    RPG/utils/Utils.cpp \
    RPG/com/RpgDialog.cpp \
    RPG/core/RpgDialogBase.cpp \
    RPG/core/RpgMapBlock.cpp \
    RPG/core/RpgAutoTileBase.cpp \
    RPG/Global.cpp \
    RPG/com/RpgScene.cpp \
    RPG/core/RpgTileSetBase.cpp

HEADERS  += Mainw.h \
    RPG/About.h \
    RPG/Global.h \
    RPG/com/RpgWidget.h \
    RPG/utils/Utils.h \
    RPG/com/RpgDialog.h \
    RPG/core/RpgDialogBase.h \
    RPG/core/RpgMapBlock.h \
    RPG/core/RpgAutoTileBase.h \
    RPG/com/RpgScene.h \
    RPG/core/RpgTileSetBase.h


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
