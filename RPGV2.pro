#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T16:23:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia qml concurrent

DEFINES *= QT_USE_QSTRINGBUILDER
TARGET = RPGV2
TEMPLATE = app


SOURCES += main.cpp\
	Mainw.cpp \
    RPG/com/RpgWidget.cpp \
    RPG/utils/Utils.cpp \
    RPG/com/RpgDialog.cpp \
    RPG/core/RpgMapBlock.cpp \
    RPG/core/RpgAutoTileBase.cpp \
    RPG/Global.cpp \
    RPG/com/RpgScene.cpp \
    RPG/core/RpgTileSetBase.cpp \
    RPG/com/RpgBanner.cpp \
    RPG/com/RpgChoice.cpp \
    ExternalFont.cpp \
    RPG/com/RpgScript.cpp \
    RPG/com/RpgVar.cpp \
    RPG/com/RpgState.cpp \
    RPG/core/RpgDialogBase.cpp \
    RPG/com/RpgMusic.cpp \
    RPG/com/RpgSound.cpp \
    RPG/com/RpgItem.cpp \
    RPG/core/RpgObject.cpp \
    RPG/script/RpgBannerHelper.cpp \
    RPG/script/RpgChoiceHelper.cpp \
    RPG/script/RpgDialogHelper.cpp \
    RPG/script/RpgMusicHelper.cpp \
    RPG/script/RpgSoundHelper.cpp \
    RPG/script/RpgSceneHelper.cpp \
    RPG/script/RpgUtilsHelper.cpp \
    RPG/script/RpgVarHelper.cpp \
    RPG/com/RpgLyric.cpp \
    RPG/script/RpgLyricHelper.cpp \
    RPG/core/RpgFileManager.cpp \
    RPG/exception/RpgException.cpp \
    RPG/exception/RpgFileNotFoundException.cpp \
    RPG/exception/RpgFileCannotReadException.cpp \
    RPG/exception/RpgNullPointerException.cpp \
    RPG/script/RpgItemHelper.cpp \
    RPG/exception/RpgKeyNotFoundException.cpp

HEADERS  += Mainw.h \
    RPG/About.h \
    RPG/Global.h \
    RPG/com/RpgWidget.h \
    RPG/utils/Utils.h \
    RPG/com/RpgDialog.h \
    RPG/core/RpgMapBlock.h \
    RPG/core/RpgAutoTileBase.h \
    RPG/com/RpgScene.h \
    RPG/core/RpgTileSetBase.h \
    RPG/com/RpgBanner.h \
    RPG/com/RpgChoice.h \
    ExternalFont.h \
    RPG/com/RpgScript.h \
    RPG/com/RpgVar.h \
    RPG/com/RpgState.h \
    RPG/core/RpgDialogBase.h \
    RPG/com/RpgMusic.h \
    RPG/com/RpgSound.h \
    RPG/com/RpgItem.h \
    RPG/core/RpgObject.h \
    RPG/script/RpgBannerHelper.h \
    RPG/script/RpgChoiceHelper.h \
    RPG/script/RpgDialogHelper.h \
    RPG/script/RpgMusicHelper.h \
    RPG/script/RpgSoundHelper.h \
    RPG/script/RpgSceneHelper.h \
    RPG/script/RpgUtilsHelper.h \
    RPG/script/RpgVarHelper.h \
    RPG/com/RpgLyric.h \
    RPG/script/RpgLyricHelper.h \
    RPG/core/RpgFileManager.h \
    RPG/exception/RpgException.h \
    RPG/exception/RpgFileNotFoundException.h \
    RPG/exception/RpgFileCannotReadException.h \
    RPG/exception/RpgNullPointerException.h \
    RPG/script/RpgItemHelper.h \
    RPG/exception/RpgKeyNotFoundException.h

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

unix{
	DESTDIR = $$PWD/bin
	OBJECTS_DIR = $$PWD/tmp
	MOC_DIR = $$PWD/tmp
	RCC_DIR = $$PWD/tmp
	UI_DIR = $$PWD/tmp
}

DISTFILES += \
    data/fonts/A-OTF-FolkPro-Bold.otf \
    data/fonts/A-OTF-FolkPro-Medium.otf \
    data/fonts/cinecaption227.ttf \
    data/fonts/msyh.ttc \
	data/fonts/msyhl.ttc \
    data/fonts/msyhbd.ttc \
    data/images/tilesets/016-ForestTown02.png \
    data/images/autotiles/test.png \
    data/images/drawing/001.png \
    data/images/drawing/01.png \
    data/images/drawing/002.png \
    data/images/drawing/02.png \
    data/images/drawing/03.png \
    data/images/panoramas/001-Sky01.jpg \
    data/images/panoramas/002-Sky02.jpg \
    data/images/panoramas/003-StarlitSky01.jpg \
    data/images/panoramas/004-CloudySky01.jpg \
    data/images/panoramas/005-Sunset01.jpg \
    data/images/panoramas/006-Mountains01.jpg \
    data/images/panoramas/007-Ocean01.jpg \
    data/images/skin/redAlpha.png \
    data/images/skin/blueAlpha.png \
    data/fonts/A-OTF-FolkPro-Bold.otf \
    data/fonts/A-OTF-FolkPro-Medium.otf \
    data/fonts/cinecaption227.ttf \
    data/fonts/msyh.ttc \
    data/fonts/msyhbd.ttc \
    data/fonts/msyhl.ttc \
    data/sounds/OP_月光.mp3 \
    data/sounds/title.mp3 \
    data/sounds/月光（オルゴール）.mp3 \
    data/sounds/雨の日に.mp3 \
    data/sounds/title.ogg \
    data/images/background/タイトル画面_背景.jpg \
    data/images/panoramas/001-Sky01.jpg \
    data/images/panoramas/002-Sky02.jpg \
    data/images/panoramas/003-StarlitSky01.jpg \
    data/images/panoramas/004-CloudySky01.jpg \
    data/images/panoramas/005-Sunset01.jpg \
    data/images/panoramas/006-Mountains01.jpg \
    data/images/panoramas/007-Ocean01.jpg \
    data/images/autotiles/autotileDemo.png \
    data/images/autotiles/test.png \
    data/images/background/mix/タイトル_キャラ01.png \
    data/images/background/mix/タイトルロゴ.png \
    data/images/background/title.png \
    data/images/background/title1.png \
    data/images/background/title2.png \
    data/images/demo/scene.png \
    data/images/demo/title.png \
    data/images/drawing/001.png \
    data/images/drawing/002.png \
    data/images/drawing/01.png \
    data/images/drawing/02.png \
    data/images/drawing/03.png \
    data/images/fonts/ascii.png \
    data/images/skin/blackAlpha.png \
    data/images/skin/blue - 副本.png \
    data/images/skin/blue - 副本1.png \
    data/images/skin/blue.png \
    data/images/skin/blueAlpha.png \
    data/images/skin/infor.png \
    data/images/skin/redAlpha.png \
    data/images/tilesets/016-ForestTown02.png \
    data/images/background/title2.psd \
    data/images/skin/blueAlpha.psd \
    data/se/banned.wav \
    data/se/select.wav \
    data/fonts/A-OTF-FolkPro-Bold.otf \
    data/fonts/A-OTF-FolkPro-Medium.otf \
    data/fonts/cinecaption227.ttf \
    data/fonts/msyh.ttc \
    data/fonts/msyhbd.ttc \
    data/fonts/msyhl.ttc \
    data/sounds/OP_月光.mp3 \
    data/sounds/title.mp3 \
    data/sounds/月光（オルゴール）.mp3 \
    data/sounds/雨の日に.mp3 \
    data/se/メニュー画面.ogg \
    data/sounds/title.ogg \
    data/se/banned.wav \
    data/se/select.wav \
    data/se/メニュー画面.wav \
    data/images/background/タイトル画面_背景.jpg \
    data/images/panoramas/001-Sky01.jpg \
    data/images/panoramas/002-Sky02.jpg \
    data/images/panoramas/003-StarlitSky01.jpg \
    data/images/panoramas/004-CloudySky01.jpg \
    data/images/panoramas/005-Sunset01.jpg \
    data/images/panoramas/006-Mountains01.jpg \
    data/images/panoramas/007-Ocean01.jpg \
    data/images/autotiles/autotileDemo.png \
    data/images/autotiles/test.png \
    data/images/background/mix/タイトル_キャラ01.png \
    data/images/background/mix/タイトルロゴ.png \
    data/images/background/title.png \
    data/images/background/title1.png \
    data/images/background/title2.png \
    data/images/demo/scene.png \
    data/images/demo/title.png \
    data/images/drawing/001.png \
    data/images/drawing/002.png \
    data/images/drawing/01.png \
    data/images/drawing/02.png \
    data/images/drawing/03.png \
    data/images/fonts/ascii.png \
    data/images/skin/blackAlpha.png \
    data/images/skin/blue - 副本.png \
    data/images/skin/blue - 副本1.png \
    data/images/skin/blue.png \
    data/images/skin/blueAlpha.png \
    data/images/skin/infor.png \
    data/images/skin/redAlpha.png \
    data/images/tilesets/016-ForestTown02.png \
    data/images/background/title2.psd \
    data/images/skin/blueAlpha.psd \
    data/scripts/main.js \
    data/scripts/title.js \
    data/scripts/initialize.json \
    data/fonts/A-OTF-FolkPro-Bold.otf \
    data/fonts/A-OTF-FolkPro-Medium.otf \
    data/fonts/cinecaption227.ttf \
    data/fonts/msyh.ttc \
    data/fonts/msyhbd.ttc \
    data/fonts/msyhl.ttc \
    data/sounds/OP_月光.mp3 \
    data/sounds/title.mp3 \
    data/sounds/月光（オルゴール）.mp3 \
    data/sounds/雨の日に.mp3 \
    data/se/メニュー画面.ogg \
    data/sounds/title.ogg \
    data/se/banned.wav \
    data/se/select.wav \
    data/se/メニュー画面.wav \
    data/images/background/タイトル画面_背景.jpg \
    data/images/panoramas/001-Sky01.jpg \
    data/images/panoramas/002-Sky02.jpg \
    data/images/panoramas/003-StarlitSky01.jpg \
    data/images/panoramas/004-CloudySky01.jpg \
    data/images/panoramas/005-Sunset01.jpg \
    data/images/panoramas/006-Mountains01.jpg \
    data/images/panoramas/007-Ocean01.jpg \
    data/images/autotiles/autotileDemo.png \
    data/images/autotiles/test.png \
    data/images/background/mix/タイトル_キャラ01.png \
    data/images/background/mix/タイトルロゴ.png \
    data/images/background/title.png \
    data/images/background/title1.png \
    data/images/background/title2.png \
    data/images/demo/scene.png \
    data/images/demo/title.png \
    data/images/drawing/001.png \
    data/images/drawing/002.png \
    data/images/drawing/01.png \
    data/images/drawing/02.png \
    data/images/drawing/03.png \
    data/images/fonts/ascii.png \
    data/images/skin/blackAlpha.png \
    data/images/skin/blue - 副本.png \
    data/images/skin/blue - 副本1.png \
    data/images/skin/blue.png \
    data/images/skin/blueAlpha.png \
    data/images/skin/infor.png \
    data/images/skin/redAlpha.png \
    data/images/tilesets/016-ForestTown02.png \
    data/images/background/title2.psd \
    data/images/skin/blueAlpha.psd \
    data/lyrics/カンタレラ~grace edition~.lrc.bak \
    data/lyrics/p1.lrc \
    data/lyrics/p2.lrc \
	data/lyrics/Turnin\'.lrc \
    data/lyrics/カンタレラ~grace edition~.lrc \
    data/music/nero - カンタレラ~grace edition~.mp3 \
    data/music/OP_月光.mp3 \
    data/music/title.mp3 \
    data/music/Young+Rising+Sons-Turnin’.mp3 \
    data/music/インドア系ならトラックメイカ.mp3 \
    data/music/雨の日に.mp3 \
    data/music/月光（オルゴール）.mp3 \
    data/music/title.ogg \
    data/music/Young+Rising+Sons-Turnin’.ogg \
    data/scripts/main.js \
    data/scripts/title.js \
    data/scripts/initialize.json \
    data/images/background/タイトル画面_背景.jpg \
    data/images/background/lyrictest.png \
    data/images/background/title.png \
    data/images/background/title1.png \
    data/images/background/title2.png \
    data/images/background/lyrictest.psd \
    data/images/background/title2.psd
