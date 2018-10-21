#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T02:54:45
#
#-------------------------------------------------

QMAKE_CXXFLAGS_WARN_ON = -Wno-ignored-qualifiers # Not available in GCC? -Wno-address-of-packed-member

QT += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Set your required SDK version here
#QMAKE_MAC_SDK = macosx10.14

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
    View/SyntaxHighlighters/GLSLHighlighter.cpp \
    View/SyntaxHighlighters/LuaHighlighter.cpp \
    Controller/MainWindowController.cpp \
    View/CodeEditorWidget.cpp \
    View/GLView/PathPointViewer.cpp \
    Model/PathEditorTableModel.cpp \
    Controller/PathEditorFormController.cpp \
    View/WindowInputState.cpp \
    Controller/ApplicationController.cpp \
    View/WaveformWidget.cpp \
    Controller/AudioToolsFormController.cpp \
    Controller/AbstractEditorWidget.cpp \
    Controller/PCMPlayer.cpp \
    Model/AudioMarkersTableModel.cpp \
    Controller/WavFile.cpp \
    View/ADSRWidget.cpp \
    Controller/MaterialEditorFormController.cpp \
    Controller/TextEditorTabController.cpp \
    Model/MaterialParametersTableModel.cpp \
    View/MaterialPreviewGL.cpp \
    Controller/ModelMaterialFormController.cpp \
    Model/ModelMaterialTableDelegate.cpp \
    Model/ModelMaterialTableModel.cpp

HEADERS  += \
    \
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
    View/SyntaxHighlighters/GLSLHighlighter.h \
    View/SyntaxHighlighters/LuaHighlighter.h \
    Controller/MainWindowController.h \
    View/CodeEditorWidget.h \
    View/GLView/PathPointViewer.h \
    Controller/PathEditorFormController.h \
    Model/PathEditorTableModel.h \
    View/WindowInputState.h \
    Controller/ApplicationController.h \
    Controller/UIActions.h \
    View/WaveformWidget.h \
    Controller/AudioToolsFormController.h \
    Controller/AbstractEditorWidget.h \
    Controller/PCMPlayer.h \
    Model/AudioMarkersTableModel.h \
    Controller/WavFile.h \
    View/ADSRWidget.h \
    Controller/MaterialEditorFormController.h \
    Controller/TextEditorTabController.h \
    Model/MaterialParametersTableModel.h \
    View/MaterialPreviewGL.h \
    Controller/ModelMaterialFormController.h \
    Model/ModelMaterialTableModel.h \
    Model/ModelMaterialTableDelegate.h

FORMS += View/MainWindow.ui \
    View/PreferencesWidget.ui \
    View/PathEditorTableForm.ui \
    View/AudioToolsForm.ui \
    View/MaterialEditorForm.ui \
    View/TextEditorTabForm.ui \
    View/ModelMaterialTableForm.ui

RESOURCES += \
    Resources/Resources.qrc \
    qdarkstyle/style.qrc

ICON = Resources/icons/Ag.icns

macx: LIBS += \
    -framework OpenGL \
    -framework OpenAL \
    -L/opt/octronic/build/Dream/DreamCore \
    -L/usr/local/lib \
    -L/opt/local/lib \
    -L/usr/lib \
    -lDreamCore \
    -llua.5.3 \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lSOIL \
    -lFreeType \
    -logg

LIBS += \
    -L/opt/octronic/build/Dream/DreamCore \
    -L/usr/lib/x86_64-linux-gnu \
    -L/usr/local/lib \
    -L/usr/lib \
    -lBulletCollision -lLinearMath -lBulletDynamics \
    -lDreamCore \
    -lfreetype \
    -lassimp \
    -ltinyspline \
    -ltinysplinecpp \
    -logg

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
