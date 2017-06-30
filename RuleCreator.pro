#-------------------------------------------------
#
# Проект RuleCreator создан 2017-02-21T23:49:20
#
#-------------------------------------------------

QT       += core gui

PROJECT = RuleCreator

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$PROJECT
CONFIG += link_pkgconfig
CONFIG += c++11

TEMPLATE = app

VERSION = $$system(bash $$PWD/scripts/git-version)
VERSION_REVISION = $$system(git describe --always)
VERSION_DATE = $$system(git show -s --format="%ct")
# макрос экспорта:
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_REVISION=\\\"$$VERSION_REVISION\\\"
DEFINES += APP_DATE=\\\"$$VERSION_DATE\\\"

SOURCES += \
  $$PWD/src/main.cpp\
  $$PWD/src/rulecreator.cpp \
  $$PWD/src/highlighter.cpp \
  $$PWD/src/tabbar.cpp \
  $$PWD/src/tabwidget.cpp \
  $$PWD/src/linenumberarea.cpp \
  $$PWD/src/codetextedit.cpp \
  $$PWD/src/rulewidget.cpp \
  $$PWD/src/regexpwidget.cpp \
  $$PWD/src/messagemodel.cpp \
  $$PWD/src/msglistview.cpp \
  $$PWD/src/menu.cpp \
  $$PWD/src/msgeditwidget.cpp

HEADERS  += \
  $$PWD/src/rulecreator.h \
  $$PWD/src/highlighter.h \
  $$PWD/src/tabbar.h \
  $$PWD/src/tabwidget.h \
  $$PWD/src/linenumberarea.h \
  $$PWD/src/codetextedit.h \
  $$PWD/src/rulewidget.h \
  $$PWD/src/regexpwidget.h \
  $$PWD/src/messagemodel.h \
  $$PWD/src/msglistview.h \
  $$PWD/src/menu.h \
  $$PWD/src/msgeditwidget.h


DESTDIR = $$PWD/build/bin/
MOC_DIR = $$PWD/build/bin/
OBJECTS_DIR = $$PWD/build/bin/

isEmpty(PREFIX) {
    PREFIX=$$PWD
}
CONFIG_PATH = $$PREFIX/etc/$$PROJECT/
RESOURCES_PATH = $$PREFIX/share/$$PROJECT/

DEFINES += CONFIG_PATH=\\\"$$CONFIG_PATH\\\"
DEFINES += RESOURCES_PATH=\\\"$$RESOURCES_PATH\\\"

PKG_CONFIG_LIBDIR  = $$PREFIX/share/pkgconfig
PKG_CONFIG_LIBDIR += $$system(pkg-config --variable pc_path pkg-config)

PKGCONFIG += TLogger TRules Common TDB

# installs
target.path     = $$PREFIX/bin

# config
config.path = $$PREFIX/etc/$$PROJECT
config.files += $$PWD/config/settings.conf

## style
styles.path = $$RESOURCES_PATH/style
styles.files += $$PWD/share/styles/dark.qss
styles.files += $$PWD/share/styles/fluent.qss

INSTALLS += target config styles

