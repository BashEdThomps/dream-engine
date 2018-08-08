#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T02:54:45
#
#-------------------------------------------------

QMAKE_CXXFLAGS_WARN_ON = -Wno-ignored-qualifiers # Not available in GCC? -Wno-address-of-packed-member

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets


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
    Controller/MainController.cpp \
    \
    Model/DreamProjectModel.cpp \
    Model/ProjectDirectoryModel.cpp \
    Model/PreferencesModel.cpp \
    \
    View/QOpenGLWindowComponent.cpp \
    View/GLView/Grid.cpp \
    View/GLView/SelectionHighlighter.cpp \
    View/GLView/GLDrawable.cpp \
    View/GLView/RelationshipTree.cpp \
    \
    Test/TestProjectDirModel.cpp \
    \
    Model/TreeModels/GenericTreeItem.cpp \
    \
    Model/TreeModels/Scenegraph/ScenegraphTreeModel.cpp \
    Model/TreeModels/Scenegraph/ScenegraphTreeItem.cpp \
    \
    Model/TreeModels/AssetDefinition/AssetDefinitionTreeItem.cpp \
    Model/TreeModels/AssetDefinition/AssetDefinitionTreeModel.cpp \
    \
    Model/TreeModels/Properties/AbstractPropertiesItem.cpp \
    \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesModel.cpp \
    \
    Model/TreeModels/Properties/Scene/ScenePropertiesModel.cpp \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesModel.cpp \
    Model/TreeModels/Properties/Project/ProjectPropertiesModel.cpp \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesItem.cpp \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesItem.cpp \
    Model/TreeModels/Properties/Scene/ScenePropertiesItem.cpp \
    Model/TreeModels/Properties/Project/ProjectPropertiesItem.cpp \
    Model/TemplatesModel.cpp \
    View/PreferencesWidget.cpp \
    Controller/PreferencesDialogController.cpp \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesTreeDelegate.cpp \
    Model/TreeModels/Properties/Scene/ScenePropertiesTreeDelegate.cpp \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesTreeDelegate.cpp \
    Model/TreeModels/Properties/Project/ProjectPropertiesTreeDelegate.cpp \
    Model/TreeModels/Properties/AbstractPropertiesModel.cpp \
    Model/MacOSOpenModel.cpp \
    Model/TreeModels/TreeModelToolButton.cpp \
    Controller/ScriptEditorController.cpp \
    View/SyntaxHighlighters/GLSLHighlighter.cpp \
    View/SyntaxHighlighters/LuaHighlighter.cpp \
    Controller/ScriptEditorTabController.cpp \
    Controller/MainWindowController.cpp \
    View/CodeEditorWidget.cpp \
    Model/MaterialShaderTableModel.cpp \
    Controller/MaterialShaderTableController.cpp



HEADERS  += \
    \
    Controller/MainController.h \
    \
    Model/DreamProjectModel.h \
    Model/ProjectDirectoryModel.h \
    Model/PreferencesModel.h \
    \
    View/QOpenGLWindowComponent.h \
    View/GLView/Grid.h \
    View/GLView/SelectionHighlighter.h \
    View/GLView/GLDrawable.h \
    View/GLView/RelationshipTree.h \
    \
    Test/TestProjectDirModel.h \
    Test/catch.hpp \
    \
    Model/TreeModels/GenericTreeItem.h \
    \
    Model/TreeModels/Scenegraph/ScenegraphTreeModel.h \
    Model/TreeModels/Scenegraph/ScenegraphTreeItem.h \
    \
    Model/TreeModels/AssetDefinition/AssetDefinitionTreeItem.h \
    Model/TreeModels/AssetDefinition/AssetDefinitionTreeModel.h \
    \
    Model/TreeModels/Properties/AbstractPropertiesItem.h \
    Model/TreeModels/Properties/AbstractPropertiesModel.h \
    \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesModel.h \
    \
    Model/TreeModels/Properties/Scene/ScenePropertiesModel.h \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesModel.h \
    Model/TreeModels/Properties/Project/ProjectPropertiesModel.h \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesItem.h \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesItem.h \
    Model/TreeModels/Properties/Scene/ScenePropertiesItem.h \
    Model/TreeModels/Properties/Project/ProjectPropertiesItem.h \
    Model/TemplatesModel.h \
    View/PreferencesWidget.h \
    Controller/PreferencesDialogController.h \
    Model/TreeModels/Properties/AssetDefinition/AssetDefinitionPropertiesTreeDelegate.h \
    Model/TreeModels/Properties/Scene/ScenePropertiesTreeDelegate.h \
    Model/TreeModels/Properties/SceneObject/SceneObjectPropertiesTreeDelegate.h \
    Model/TreeModels/Properties/Project/ProjectPropertiesTreeDelegate.h \
    Model/MacOSOpenModel.h \
    Model/TreeModels/TreeModelToolButton.h \
    Controller/ScriptEditorController.h \
    View/SyntaxHighlighters/GLSLHighlighter.h \
    View/SyntaxHighlighters/LuaHighlighter.h \
    Controller/ScriptEditorTabController.h \
    Controller/MainWindowController.h \
    View/CodeEditorWidget.h \
    Model/MaterialShaderTableModel.h \
    Controller/MaterialShaderTableController.h



FORMS += View/MainWindow.ui \
    View/PreferencesWidget.ui \
    View/ScriptEditorForm.ui \
    View/ScriptEditorTabForm.ui \
    View/MaterialShaderTableForm.ui

RESOURCES += \
    Resources/Resources.qrc

ICON = Resources/icons/DreamTool.icns

macx: LIBS += \
    -framework OpenGL \
    -L/opt/octronic/builds/Dream/DreamCore \
    -L/opt/octronic/builds/Dream/Luabind \
    -L/opt/octronic/builds/Dream/SOIL \
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
    -L/opt/octronic/builds/Dream/SOIL \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lDreamCore \
    -lfreetype \
    -lGLEW \
    -lAssimp

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

RCC_DIR = Resources

DEPENDPATH += \
    $$PWD/../DreamCore/include
