#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T02:54:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DreamToolQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    View/MainWindow.cpp \
    Controller/MainController.cpp \
    Model/DreamModel.cpp \
    Model/QTDreamAudioComponent.cpp \
    Model/QTDreamWindowComponent.cpp \
    Model/ProjectTreeItem.cpp \
    Model/ProjectTreeModel.cpp \
    Model/Properties/PropertiesModel.cpp \
    Model/Properties/ScenePropertiesModel.cpp \
    Model/Properties/SceneObjectPropertiesModel.cpp \
    Model/Properties/ProjectPropertiesModel.cpp \
    Model/Properties/AssetDefinitionPropertiesModel.cpp \
    Model/Properties/PropertiesItem.cpp \
    Model/Properties/AssetDefinitionTypeComboDelegate.cpp \
    Model/QTDreamAudioInstance.cpp \
    Model/ProjectDirModel.cpp

HEADERS  += View/MainWindow.h \
    Controller/MainController.h \
    Model/DreamModel.h \
    Model/QTDreamAudioComponent.h \
    Model/QTDreamWindowComponent.h \
    Model/ProjectTreeItem.h \
    Model/ProjectTreeModel.h \
    Model/Properties/PropertiesModel.h \
    Model/Properties/AssetDefinitionPropertiesModel.h \
    Model/Properties/ProjectPropertiesModel.h \
    Model/Properties/SceneObjectPropertiesModel.h \
    Model/Properties/ScenePropertiesModel.h \
    Model/Properties/PropertiesItem.h \
    Model/Properties/AssetDefinitionTypeComboDelegate.h \
    Model/QTDreamAudioInstance.h \
    Model/ProjectDirModel.h

FORMS    += View/MainWindow.ui

RESOURCES += \
    Resources/Resources.qrc

ICON = Resources/icons/DreamTool.icns

macx: LIBS += \
    -L$$PWD/../../build-dream-Desktop_Qt_5_8_0_clang_64bit2-Debug/DreamCore/ \
    -L$$PWD/../../build-dream-Desktop_Qt_5_8_0_clang_64bit2-Debug/Luabind/ \
    -L/usr/local/lib \
    -L/opt/local/lib \
    -L/usr/lib \
    -lDreamCore \
    -llua.5.3 \
    -lLuabind \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lSOIL \
    -lFreeType

LIBS += \
    -L$$PWD/../../build-dream-Desktop_Qt_5_8_0_GCC_64bit-Debug/DreamCore \
    -L$$PWD/../../build-dream-Desktop_Qt_5_8_0_GCC_64bit-Debug/Luabind \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lDreamCore
    -lFreeType

INCLUDEPATH += \
    $$PWD/../DreamCore/include \
    /usr/local/include \
    /usr/include/bullet \
    /usr/include/lua5.3 \
    /usr/local/include/lua5.3 \
    /usr/local/include/bullet \
    /usr/include/freetype2 \
    /usr/local/include/freetype2 \
    /opt/local/include

DEPENDPATH += \
    $$PWD/../DreamCore/include
