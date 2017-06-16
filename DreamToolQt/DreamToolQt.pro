#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T02:54:45
#
#-------------------------------------------------

QMAKE_CXXFLAGS_WARN_ON = -Wno-ignored-qualifiers # Not available in GCC? -Wno-address-of-packed-member

QT += core gui opengl

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


SOURCES += \
    main.cpp \
    \
    View/MainWindow.cpp \
    View/QOpenGLWindowComponent.cpp \
    Controller/MainController.cpp \
    \
    Model/DreamProjectModel.cpp \
    Model/ProjectDirModel.cpp \
    \
    Model/TreeModels/AssetDefinitionTreeItem.cpp \
    Model/TreeModels/AssetDefinitionTreeModel.cpp \
    Model/TreeModels/GenericTreeItem.cpp \
    Model/TreeModels/ProjectTreeItem.cpp \
    Model/TreeModels/ProjectTreeModel.cpp \
    \
    Model/TreeModels/Properties/PropertiesModel.cpp \
    Model/TreeModels/Properties/ScenePropertiesModel.cpp \
    Model/TreeModels/Properties/SceneObjectPropertiesModel.cpp \
    Model/TreeModels/Properties/ProjectPropertiesModel.cpp \
    Model/TreeModels/Properties/AssetDefinitionPropertiesModel.cpp \
    Model/TreeModels/Properties/PropertiesItem.cpp \
    Model/TreeModels/Properties/AssetDefinitionTypeComboDelegate.cpp \
    Model/TreeModels/Properties/AssetDefinitionFormatComboDelegate.cpp \
    Model/TreeModels/Properties/Shader/ShaderTemplateComboDelegate.cpp \
    Model/TreeModels/Properties/Script/ScriptTemplateComboDelegate.cpp \
    View/GLView/Grid.cpp \
    View/GLView/SelectionHighlighter.cpp \
    View/GLView/GLDrawable.cpp \
    View/GLView/RelationshipTree.cpp \
    Test/TestProjectDirModel.cpp \
    Model/PreferencesModel.cpp


HEADERS  += \
    View/MainWindow.h \
    View/QOpenGLWindowComponent.h \
    \
    Controller/MainController.h \
    \
    Model/DreamProjectModel.h \
    Model/ProjectDirModel.h \
    \
    Model/TreeModels/AssetDefinitionTreeItem.h \
    Model/TreeModels/AssetDefinitionTreeModel.h \
    Model/TreeModels/GenericTreeItem.h \
    Model/TreeModels/ProjectTreeItem.h \
    \
    Model/TreeModels/ProjectTreeModel.h \
    Model/TreeModels/Properties/PropertiesModel.h \
    Model/TreeModels/Properties/AssetDefinitionPropertiesModel.h \
    Model/TreeModels/Properties/ProjectPropertiesModel.h \
    Model/TreeModels/Properties/SceneObjectPropertiesModel.h \
    Model/TreeModels/Properties/ScenePropertiesModel.h \
    Model/TreeModels/Properties/PropertiesItem.h \
    Model/TreeModels/Properties/AssetDefinitionTypeComboDelegate.h \
    Model/TreeModels/Properties/AssetDefinitionFormatComboDelegate.h \
    Model/TreeModels/Properties/Shader/ShaderTemplateComboDelegate.h \
    Model/TreeModels/Properties/Script/ScriptTemplateComboDelegate.h \
    View/GLView/Grid.h \
    View/GLView/SelectionHighlighter.h \
    View/GLView/GLDrawable.h \
    View/GLView/RelationshipTree.h \
    Test/TestProjectDirModel.h \
    Test/catch.hpp \
    Model/PreferencesModel.h


FORMS    += View/MainWindow.ui

RESOURCES += \
    Resources/Resources.qrc

ICON = Resources/icons/DreamTool.icns

macx: LIBS += \
    -framework OpenGL \
    -L/opt/octronic/builds/Dream/DreamCore \
    -L/opt/octronic/builds/Dream/Luabind \
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
    -L/opt/octronic/builds/Dream/DreamCore \
    -L/opt/octronic/builds/Dream/Luabind \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lDreamCore \
    -lfreetype \
    -lGLEW

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
