/*
 * MainWindow.cpp
 *
 * Created: 13 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "MainWindowController.h"
#include "ui_MainWindow.h"

#include <QWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QMenu>

#include <vector>
#include <algorithm>
#include <memory>

#include "View/QOpenGLWindowComponent.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "../Model/ProjectDirectoryModel.h"
#include "PathEditorFormController.h"
#include "AudioToolsFormController.h"
#include "MaterialEditorFormController.h"

using std::pair;
using std::begin;
using std::find;
using std::end;
using std::make_shared;

const vector<int> MainWindowController::mKeysPassedToWindow =
{
    Qt::Key_W,
    Qt::Key_A,
    Qt::Key_S,
    Qt::Key_D,
    Qt::Key_Up,
    Qt::Key_Down,
    Qt::Key_Left,
    Qt::Key_Right,
    Qt::Key_Shift,
    Qt::Key_Alt
};

MainWindowController::MainWindowController
(QWidget *parent)
    : QMainWindow(parent),
      LuaPrintListener(),
      mUi(new Ui::MainWindow),
      mProjectDefinitionHandle(nullptr),
      mProjectDirectoryModelHandle(nullptr),
      mTemplatesModelHandle(nullptr),
      mWindowComponentHandle(nullptr),
      mPathEditorFormControllerHandle(nullptr)
{
    auto log = spdlog::get("MainWindowController");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("MainWindowController");
    }

    log->debug("Constructing");

    mUi->setupUi(this);
    setupUiFeatures();

    setupGL(parent);
    setupMenu_Asset_NewDefinition();
    setupMenu_Debug();
    setActionsEnabled_Scene_Playback(false);
    setActionsEnabled_Scene_Modification(false);
    setActionEnabled_File_Save(false);
    LuaComponent::AddPrintListener(this);
    //mUi->leftRightSplitter->setStretchFactor(1,4);
    //mUi->previewDebugSplitter->setStretchFactor(1,1);
}

void MainWindowController::setupMenu_Debug()
{
    connect(mUi->actionLogLevelTrace,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelDebug,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelInfo,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelWarn,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelError,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelCritical,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
    connect(mUi->actionLogLevelOff,SIGNAL(triggered(bool)),this,SLOT(onMenu_Debug_LogLevelChanged(bool))) ;
}

void MainWindowController::setupUiFeatures()
{
    mUi->scenegraphPropertiesSplitter->setStretchFactor(3,1);

    mVolumeSlider.setOrientation(Qt::Horizontal);
    mVolumeSlider.setMinimum(0);
    mVolumeSlider.setMaximum(100);
    mVolumeSlider.setSingleStep(1);
    mVolumeSlider.setMinimumWidth(150);
    mVolumeSlider.setMaximumWidth(150);
    mVolumeSlider.setValue(100);
    connect(&mVolumeSlider,SIGNAL(valueChanged(int)),this,SLOT(onMainVolumeChanged(int)));
    QIcon* volIcon = new QIcon(":svg/volume-up-solid.svg");
    QLabel* volLabel = new QLabel();
    volLabel->setPixmap(volIcon->pixmap(QSize(16,16)));
    volLabel->setFixedSize(QSize(16,16));
    mUi->statusBar->addPermanentWidget(volLabel,1);
    mUi->statusBar->addPermanentWidget(&mVolumeSlider,1);
    setupEditorTabCloseButtonSignal();

    auto browserTitle = new QLabel("Project Browser");
    browserTitle->setMinimumHeight(24);
    mUi->projectBrowserDockWidget->setTitleBarWidget(browserTitle);

    auto luaDebugTitle = new QLabel("Lua Log");
    luaDebugTitle->setMinimumHeight(24);
    mUi->luaDebugDockWidget->setTitleBarWidget(luaDebugTitle);

    mUi->scenegraphTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
        mUi->scenegraphTreeView,
        SIGNAL(customContextMenuRequested(const QPoint &)),
        this,
        SLOT(onScenegraphContextMenuRequested(const QPoint &))
    );
    mUi->assetDefinitionTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
        mUi->assetDefinitionTreeView,
        SIGNAL(customContextMenuRequested(const QPoint &)),
        this,
        SLOT(onAsseetDefinitionContextMenuRequested(const QPoint &))
    );
    connect(
        mUi->assetDefinitionTreeView,
        SIGNAL(activated(const QModelIndex&)),
        this,
        SLOT(onAssetDefinitionTreeViewActivated(const QModelIndex&))
    );
    connect(
        mUi->scenegraphTreeView,
        SIGNAL(activated(const QModelIndex&)),
        this,
        SLOT(onScenegraphTreeViewActivated(const QModelIndex&))
    );
    connect(
        mUi->propertiesTreeView,
        SIGNAL(activated(const QModelIndex&)),
        this,
        SLOT(onPropertiesTreeViewActivated(const QModelIndex&))
    );

    connect
    (
        mUi->actionLuaLog,
        SIGNAL(triggered(bool)),
        this,
        SLOT(onActionLuaLogTriggered(bool))
    );

    connect
    (
        mUi->actionScripting,
        SIGNAL(triggered(bool)),
        this,
        SLOT(onActionScriptingTriggered(bool))
    );
}

void
MainWindowController::onScenegraphContextMenuRequested
(const QPoint &point)
{
    auto log = spdlog::get("MainWindowController");
    log->debug("SceneGraph Context Menu Requested {},{}",point.x(),point.y());
    QModelIndex index = mUi->scenegraphTreeView->indexAt(point);
    if (index.isValid())
    {
        auto item = static_cast<ScenegraphTreeItem*>(index.internalPointer());
        auto contextMenu = createScenegraphTreeContextMenu(item);
        contextMenu->exec(mUi->scenegraphTreeView->mapToGlobal(point));
    }
}

void
MainWindowController::onAsseetDefinitionContextMenuRequested
(const QPoint &point)
{

    auto log = spdlog::get("MainWindowController");
    log->debug("Asset Definition Context Menu Requested {},{}",point.x(),point.y());
    QModelIndex index = mUi->assetDefinitionTreeView->indexAt(point);
    if (index.isValid())
    {
        auto item = static_cast<AssetDefinitionTreeItem*>(index.internalPointer());
        auto contextMenu = createAssetDefinitionTreeContextMenu(item);
        contextMenu->exec(mUi->assetDefinitionTreeView->mapToGlobal(point));
    }
}

void
MainWindowController::onCreateAssetDefinitionAction
()
{
    CreateAssetDefinitionAction* caa = dynamic_cast<CreateAssetDefinitionAction*>(sender());
    auto log = spdlog::get("MainWindowController");
    log->debug("Create Asset Definition of type {}", caa->getType().toStdString());
    emit notifyCreateNewAssetDefinition(caa->getType());
}

shared_ptr<QMenu>
MainWindowController::createAssetDefinitionTreeContextMenu
(AssetDefinitionTreeItem* item)
{
    IAssetDefinition* assetDef;
    auto menu = make_shared<QMenu>();
    QAction* nameAction = nullptr;
    CreateAssetDefinitionAction* createAction = nullptr;
    DeleteAssetDefinitionAction* deleteAction = nullptr;
    switch (item->getType())
    {
        case ASSET_DEFINITION:
            assetDef = dynamic_cast<IAssetDefinition*>(item->getAssetDefinition());
            nameAction = new QAction(QString::fromStdString(assetDef->getName()),menu.get());
            nameAction->setEnabled(false);
            menu->addAction(nameAction);
            menu->addSeparator();
            deleteAction = new DeleteAssetDefinitionAction("Delete",menu.get());
            deleteAction->mItemHandle = assetDef;
            menu->addAction(deleteAction);
            break;
        case ASSET_TREE_NODE:
            createAction = new CreateAssetDefinitionAction
            (
                item->getTitle(),
                QString("New %1 Asset").arg(item->getTitle()),
                menu.get()
            );
            menu->addAction(createAction);
            break;
    }

    if (createAction != nullptr)
    {
        connect(
            createAction,SIGNAL(triggered()),
            this,SLOT(onCreateAssetDefinitionAction())
        );
    }

    if (deleteAction != nullptr)
    {
        connect(deleteAction,SIGNAL(triggered()),
                this, SLOT(onAssetDefinitionMenuDeleteTriggered()));
    }
    return menu;
}

shared_ptr<QMenu>
MainWindowController::createScenegraphTreeContextMenu
(ScenegraphTreeItem* item)
{
    ProjectDefinition* projectDef = nullptr;
    SceneDefinition* sceneDef = nullptr;
    SceneObjectDefinition* sceneObjectDef = nullptr;

    ScenegraphMenuAction* titleAction = nullptr;
    ScenegraphMenuAction* deleteAction = nullptr;
    ScenegraphMenuAction* addChildAction = nullptr;
    ScenegraphMenuAction* duplicateAction = nullptr;
    QMenu*   addAssetMenu = nullptr;

    auto menu = make_shared<QMenu>();
    switch (item->getType())
    {
        case SCENEGRAPH_PROJECT:
            projectDef = dynamic_cast<ProjectDefinition*>(item->getItem());

            titleAction = new ScenegraphMenuAction(item, QString::fromStdString(projectDef->getName()),menu.get());
            titleAction->setEnabled(false);
            menu->addAction(titleAction);
            addChildAction = new ScenegraphMenuAction(item,"Add Scene",menu.get());
            menu->addSeparator();
            menu->addAction(addChildAction);

            connect(addChildAction,SIGNAL(triggered()), this, SLOT(onScenegraphMenuAddNewSceneTriggered()));

            break;
        case SCENEGRAPH_SCENE:
            sceneDef = dynamic_cast<SceneDefinition*>(item->getItem());

            titleAction = new ScenegraphMenuAction(item,QString::fromStdString(sceneDef->getName()),menu.get());
            titleAction->setEnabled(false);
            deleteAction = new ScenegraphMenuAction(item, "Delete Scene",menu.get());
            menu->addAction(titleAction);
            menu->addSeparator();
            menu->addAction(deleteAction);

            connect(deleteAction,SIGNAL(triggered()), this, SLOT(onScenegraphMenuDeleteSceneTriggered()));

            break;
        case SCENEGRAPH_SCENE_OBJECT:
            sceneObjectDef = dynamic_cast<SceneObjectDefinition*>(item->getItem());

            titleAction = new ScenegraphMenuAction(item, QString::fromStdString(sceneObjectDef->getName()),menu.get());
            titleAction->setEnabled(false);
            deleteAction = new ScenegraphMenuAction(item, "Delete Scene Object",menu.get());
            addChildAction = new ScenegraphMenuAction(item, "Add Child",menu.get());
            duplicateAction = new ScenegraphMenuAction(item, "Duplicate",menu.get());

            menu->addAction(titleAction);
            menu->addSeparator();
            menu->addAction(addChildAction);
            menu->addAction(duplicateAction);
            addAssetMenu = menu->addMenu("Add Asset");
            createAssetsMenu(addAssetMenu,item);
            menu->addAction(deleteAction);

            connect(duplicateAction,SIGNAL(triggered()), this, SLOT(onScenegraphMenuDuplicateSceneObjectTriggered()));
            connect(addChildAction,SIGNAL(triggered()), this, SLOT(onScenegraphMenuAddSceneObjectTriggered()));
            connect(deleteAction,SIGNAL(triggered()), this, SLOT(onScenegraphMenuDeleteSceneObjectTriggered()));

            break;
        case SCENEGRAPH_TREE_NODE :
            break;
    }


    return menu;
}


void
MainWindowController::onScenegraphMenuAddNewSceneTriggered
()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Scenegraph Menu: Add New Scene");
    auto action = dynamic_cast<ScenegraphMenuAction*>(sender());

    // Only triggered by project item
    auto projectDefHandle = dynamic_cast<ProjectDefinition*>(action->getItemHandle()->getItem());
    auto newScene = projectDefHandle->createNewSceneDefinition();
    log->debug("Created new scene {} for project {}", newScene->getNameAndUuidString(), projectDefHandle->getNameAndUuidString());
    emit notifyScenegraphTreeDataChanged();
}

void
MainWindowController::onScenegraphMenuDeleteSceneTriggered
()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Scenegraph Menu: Delete Scene");
    auto action = dynamic_cast<ScenegraphMenuAction*>(sender());

    // Only triggered by project item
    auto sceneDefHandle = dynamic_cast<SceneDefinition*>(action->getItemHandle()->getItem());
    auto parentProjectHandle = sceneDefHandle->getProjectDefinition();
    parentProjectHandle->removeSceneDefinition(sceneDefHandle);
    log->debug("Deleted scene from project {}",parentProjectHandle->getNameAndUuidString());
    emit notifyScenegraphTreeDataChanged();
}

void
MainWindowController::onScenegraphMenuAddSceneObjectTriggered
()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Scenegraph Menu: Add Scene Object");
    auto action = dynamic_cast<ScenegraphMenuAction*>(sender());

    switch (action->getItemHandle()->getType())
    {
        case SCENEGRAPH_TREE_NODE:
        case SCENEGRAPH_PROJECT:
        case SCENEGRAPH_SCENE:
            break;
        case SCENEGRAPH_SCENE_OBJECT:
            auto sceneObjectDefinitionHandle = dynamic_cast<SceneObjectDefinition*>(
                action->getItemHandle()->getItem()
            );
            auto newSceneObject = sceneObjectDefinitionHandle->createNewChildSceneObjectDefinition();
            log->debug(
                "Created scene object {} for {}",
                newSceneObject->getNameAndUuidString(),
                sceneObjectDefinitionHandle->getNameAndUuidString()
            );
            break;
    }
    emit notifyScenegraphTreeDataChanged();
}

void MainWindowController::onScenegraphMenuDuplicateSceneObjectTriggered()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Scenegraph Menu: Duplicate Scene Object");

    auto action = dynamic_cast<ScenegraphMenuAction*>(sender());

    switch (action->getItemHandle()->getType())
    {
        case SCENEGRAPH_TREE_NODE:
        case SCENEGRAPH_PROJECT:
        case SCENEGRAPH_SCENE:
            break;
        case SCENEGRAPH_SCENE_OBJECT:
            auto sceneObjectDefinitionHandle = dynamic_cast<SceneObjectDefinition*>
            (
                action->getItemHandle()->getItem()
            );
            auto duplicateSO = sceneObjectDefinitionHandle->duplicate();
            log->debug(
                "Duplicated scene object {} to {}",
                sceneObjectDefinitionHandle->getNameAndUuidString(),
                duplicateSO->getNameAndUuidString()
            );
            break;
    }
    emit notifyScenegraphTreeDataChanged();
}

void
MainWindowController::onScenegraphMenuDeleteSceneObjectTriggered
()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Scenegraph Menu: Delete Scene Object");

    auto action = dynamic_cast<ScenegraphMenuAction*>(sender());
    auto sceneObjectDefinitionHandle = dynamic_cast<SceneObjectDefinition*>(
        action->getItemHandle()->getItem()
    );
    auto parentSceneObject = sceneObjectDefinitionHandle->getParentSceneObject();

    if (parentSceneObject != nullptr)
    {
        parentSceneObject->removeChildSceneObjectDefinition(sceneObjectDefinitionHandle);
        log->debug(
            "Deleted scene object from {}",
            parentSceneObject->getNameAndUuidString()
        );
        emit notifyScenegraphTreeDataChanged();
    }
    else
    {
       QMessageBox::warning(this,"Error","Cannot delete Root SceneObject");
    }
}

void
MainWindowController::onAssetDefinitionMenuDeleteTriggered
()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Delete AssetDefinition");

    auto action = dynamic_cast<DeleteAssetDefinitionAction*>(sender());
    auto assetDefinitionHandle = action->mItemHandle;

    if (assetDefinitionHandle != nullptr)
    {
        QMessageBox confirmDelete(this);
        confirmDelete.setWindowTitle("Are you sure");
        confirmDelete.setIcon(QMessageBox::Question);
        confirmDelete.addButton("Cancel",QMessageBox::ButtonRole::RejectRole);
        auto accept = confirmDelete.addButton("Delete",QMessageBox::ButtonRole::AcceptRole);
        confirmDelete.setText
        (
            QString("Are you sure you want to delete %1?")
            .arg(QString::fromStdString(assetDefinitionHandle->getName()))
        );
        confirmDelete.setInformativeText("This will remove it's data from the project and cannot be undone.");
        confirmDelete.exec();

        if (confirmDelete.clickedButton() == accept)
        {
            log->debug(
                "Deleting Asset Definition scene object from {}",
                assetDefinitionHandle->getNameAndUuidString()
            );

            if (mProjectDirectoryModelHandle != nullptr)
            {
                mProjectDirectoryModelHandle->deleteAssetDataDirectory(assetDefinitionHandle);
            }
            else
            {
                log->error("Unable to delete asset directory. Project Dir Model is nullptr");
            }

            mProjectDefinitionHandle->removeAssetDefinition(assetDefinitionHandle);
            emit notifyAssetDefinitionTreeDataChanged();
        }
    }
    else
    {
       QMessageBox::warning(this,"Error","Cannot delete Root SceneObject");
    }

}

void MainWindowController::onAddAssetToSceneObjectTriggered()
{
    auto log = spdlog::get("MainWindowController");
    auto action = dynamic_cast<AddAssetToSceneObjectAction*>(sender());
    IAssetDefinition* adHandle = action->getAssetDefinitionHandle();
    ScenegraphTreeItem* treeItemHandle = action->getItemHandle();
    if (treeItemHandle->getType() == SCENEGRAPH_SCENE_OBJECT)
    {
        auto sceneObjHandle = dynamic_cast<SceneObjectDefinition*>(treeItemHandle->getItem());
        log->debug("Adding asset {} to object {}",adHandle->getNameAndUuidString(),sceneObjHandle->getNameAndUuidString());
        sceneObjHandle->addAssetDefinitionToLoadQueue(adHandle);
    }
    emit notifyScenegraphTreeDataChanged();
    emit notifyPropertiesTreeDataChanged();
}

void
MainWindowController::onScenegraphTreeViewActivated
(const QModelIndex& index)
{
    auto log = spdlog::get("MainWindowController");
    if (index.isValid())
    {
        auto item = static_cast<ScenegraphTreeItem*>(index.internalPointer());
        log->debug("Valid index in current tree {}",item->getTitle().toStdString());
    }
    else
    {
        log->debug("No valid index in current tree");
    }
}

void
MainWindowController::onPropertiesTreeViewActivated
(const QModelIndex& index)
{
    auto log = spdlog::get("MainWindowController");
    if (index.isValid())
    {
        auto item = static_cast<ScenegraphTreeItem*>(index.internalPointer());
        log->debug("Valid index in current tree {}",item->getTitle().toStdString());
    }
    else
    {
        log->debug("No valid index in current tree");
    }
}

void
MainWindowController::onAssetDefinitionTreeViewActivated
(const QModelIndex& index)
{
    auto log = spdlog::get("MainWindowController");
    if (index.isValid())
    {
        auto item = static_cast<ScenegraphTreeItem*>(index.internalPointer());
        log->debug("Valid index in current tree {}",item->getTitle().toStdString());
    }
    else
    {
        log->debug("No valid index in current tree");
    }
}

void
MainWindowController::onMenu_Debug_LogLevelChanged
(bool)
{
    auto log = spdlog::get("MainWindowController");
    auto action = dynamic_cast<QAction*>(sender());
    QString level = action->text();
    cout << "Setting log level to " << level.toStdString() << endl;
    showStatusBarMessage(QString("Log Level: %1").arg(level));
    spdlog::set_level(spdlog::level::from_str(level.toLower().toStdString()));
}

void
MainWindowController::onActionLuaLogTriggered
(bool checked)
{
    mUi->luaDebugDockWidget->toggleViewAction()->triggered(checked);
}

void
MainWindowController::onActionScriptingTriggered
(bool checked)
{
    emit notifyActionScriptingTriggered(checked);
}

void MainWindowController::setPathEditorFormControllerHandle(PathEditorFormController* pathEditorFormControllerHandle)
{
    mPathEditorFormControllerHandle = pathEditorFormControllerHandle;
}

void MainWindowController::setTemplatesModelHandle(TemplatesModel* templatesModelHandle)
{
    mTemplatesModelHandle = templatesModelHandle;
}


void
MainWindowController::createAssetsMenu
(QMenu* menu,ScenegraphTreeItem* item)
{
    if (mProjectDefinitionHandle != nullptr)
    {
        auto assetsMap = mProjectDefinitionHandle->getAssetDefinitionsMap();
        auto beginAssetsMap = begin(assetsMap);
        auto endAssetsMap = end(assetsMap);
        auto currentAssetMapPair = beginAssetsMap;

        while (currentAssetMapPair != endAssetsMap)
        {
            AssetType currentType = currentAssetMapPair->first;
            string currentTypeString = Constants::getAssetTypeStringFromTypeEnum(currentType);
            QString currentTypeReadable = QString::fromStdString(Constants::getAssetTypeReadableNameFromString(currentTypeString));

            QMenu* currentMenu = menu->addMenu(currentTypeReadable);

            vector<IAssetDefinition*> currentVector = currentAssetMapPair->second;

            for (IAssetDefinition* currentDefinition : currentVector )
            {
                QAction* nextAssetAction = new AddAssetToSceneObjectAction(
                    item,
                    currentDefinition,
                    QString::fromStdString(currentDefinition->getName()),
                    currentMenu
                );
                connect(nextAssetAction,SIGNAL(triggered()),this,SLOT(onAddAssetToSceneObjectTriggered()));
                currentMenu->addAction(nextAssetAction);
            }
            currentAssetMapPair++;
        }
    }
}

void
MainWindowController::setActionsEnabled_Scene_Modification
(bool enabled)
{
    mUi->actionSceneMenuNewScene->setEnabled(enabled);
    mUi->actionSceneNewSceneObject->setEnabled(enabled);
    mUi->actionSceneRemoveScene->setEnabled(enabled);

    mUi->actionAssetMenuAddAssetToSelectedSceneObject->setEnabled(enabled);

    mMenu_Asset_NewDefinition->setEnabled(enabled);
    setActionEnabled_Debug_DumpProjectDefinitionJson(enabled);
}

void
MainWindowController::setActionsEnabled_Scene_Playback
(bool enabled)
{
    mUi->actionPlay->setEnabled(enabled);
    mUi->actionStop->setEnabled(enabled);
    mUi->actionReload->setEnabled(enabled);
}

void
MainWindowController::setActionEnabled_File_Open
(bool enabled)
{
    mUi->actionOpen->setEnabled(enabled);
}

QAction*
MainWindowController::getAction_Asset_NewDefinition
(AssetType type)
{
    return mActionMap_Asset_NewDefinition.at(type);
}

QAction*
MainWindowController::getAction_Scene_NewScene
()
{
    return mUi->actionSceneMenuNewScene;
}

QAction*
MainWindowController::getAction_Scene_NewSceneObject
()
{
    return mUi->actionSceneNewSceneObject;
}

QAction*
MainWindowController::getAction_ControlScene
()
{
    return mUi->actionControl;
}

QAction*
MainWindowController::getAction_Asset_AddToSelectedSceneObject
()
{
    return mUi->actionAssetMenuAddAssetToSelectedSceneObject;
}

QAction*
MainWindowController::getAction_Debug_DumpProjectDefinitionJson
()
{
    return mUi->actionDebug_DumpProjectDefinitionJson;
}

void
MainWindowController::setActionEnabled_Debug_DumpProjectDefinitionJson
(bool enabled)
{

    mUi->actionDebug_DumpProjectDefinitionJson->setEnabled(enabled);
}

void
MainWindowController::setActionEnabled_File_Save
(bool enabled)
{
    mUi->actionSave->setEnabled(enabled);
}

void
MainWindowController::setActionEnabled_File_New
(bool enabled)
{
    mUi->actionNew->setEnabled(enabled);
}

void
MainWindowController::setupMenu_Asset_NewDefinition
()
{
    mMenu_Asset_NewDefinition.reset(new QMenu());
    mMenu_Asset_NewDefinition->setTitle("New Definition");

    for (pair<AssetType,string> typePair : Constants::DREAM_ASSET_TYPES_MAP)
    {
        QAction *typeAction = new QAction
                (
                    QString::fromStdString
                    (
                        Constants::getAssetTypeReadableNameFromString(typePair.second)
                        )
                    );
        mMenu_Asset_NewDefinition->addAction(typeAction);
        mActionMap_Asset_NewDefinition.insert(pair<AssetType,QAction*>(typePair.first, typeAction));
    }

    mUi->menuAsset->addMenu(mMenu_Asset_NewDefinition.get());
}

void
MainWindowController::setupGL
(QWidget*)
{
    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile );
    glFormat.setSamples(4);
    mWindowComponentHandle = mUi->openGLWidget;
    mWindowComponentHandle->setFormat(glFormat);
}

MainWindowController::~MainWindowController
()
{
    delete mUi;
}

void
MainWindowController::onPrint
(string str)
{
   mUi->debugOutputTextEdit->appendPlainText(QString::fromStdString(str));
}

QAction*
MainWindowController::getAction_File_New
()
{
    return mUi->actionNew;
}

QAction*
MainWindowController::getAction_File_Open
()
{
    return mUi->actionOpen;
}

QAction*
MainWindowController::getAction_File_CloseProject
()
{
    return mUi->actionCloseProject;
}

QAction*
MainWindowController::getAction_View_TogglePhysicsDebug
()
{
    return mUi->actionTogglePhysicsDebug;
}


QAction*
MainWindowController::getAction_File_Save
()
{
    return mUi->actionSave;
}

QAction*
MainWindowController::getAction_Scene_Play
()
{
    return mUi->actionPlay;
}

QAction*
MainWindowController::getAction_Scene_Stop
()
{
    return mUi->actionStop;
}

void
MainWindowController::onInvalidProjectDirectory
(QString directory)
{
    QMessageBox::critical(
                this,
                tr("Invalid Project Directory"),
                tr("\"%1\"\n\nDoes not contain a vaild Dream project.").arg(directory)
                );
}

void
MainWindowController::onNoSceneSelected
()
{
    QMessageBox::critical(
                this,
                tr("No Scene Scelected"),
                tr("Please select a scene from the Scenegraph tree before running")
                );
    showStatusBarMessage(QString("No Scene Selected to Run!"));
}

QTreeView*
MainWindowController::getScenegraphTreeView
()
{
    return mUi->scenegraphTreeView;
}

QTreeView*
MainWindowController::getPropertiesTreeView
()
{
    return mUi->propertiesTreeView;
}

QTreeView*
MainWindowController::getAssetDefinitionTreeView
()
{
    return mUi->assetDefinitionTreeView;
}

QAction*
MainWindowController::getAction_Preferences
()
{
    return mUi->actionPreferences;
}

void
MainWindowController::showStatusBarMessage
(QString msg)
{
    mUi->statusBar->showMessage(msg);
}

QAction*
MainWindowController::getAction_Scene_Reload
()
{
    return mUi->actionReload;
}

QAction*
MainWindowController::getAction_View_ToggleGrid
()
{
    return mUi->actionToggleGrid;
}

QAction*
MainWindowController::getAction_View_ToggleDebug
()
{
    return mUi->actionToggleDebug;
}

QOpenGLWindowComponent*
MainWindowController::getWindowComponent
()
{
    return mWindowComponentHandle;
}

void MainWindowController::setProjectDirectoryModel(ProjectDirectoryModel* dirModel)
{
   mProjectDirectoryModelHandle = dirModel;
}

QDockWidget*
MainWindowController::addRightDockWidget
(QWidget* widget)
{
    mRightDockWidget.setWidget(widget);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, &mRightDockWidget);
    //connect(&mRightDockWidget, SIGNAL(closeEvent(QCloseEvent*)), this, SLOT(onRightDockWidgetClosed(QCloseEvent*)));
    mRightDockWidget.show();
    return &mRightDockWidget;
}

void
MainWindowController::onSceneStopped
(SceneDefinition* scene)
{
    if (scene)
    {
        showStatusBarMessage(
                    QString("Stopped Scene: %1")
                    .arg(QString::fromStdString(scene->getNameAndUuidString())
                         )
                    );
    }
    else
    {
        showStatusBarMessage(QString("No Scene Running"));
    }
}

void
MainWindowController::onProjectDefinitionChanged
(ProjectDefinition* def)
{
    mProjectDefinitionHandle = def;
}

void MainWindowController::onScenegraphTreeExpandRequested()
{
    auto log = spdlog::get("MainWindowController");
    log->trace("onScenegraphTreeExpandRequested()");
}

void
MainWindowController::keyPressEvent
(QKeyEvent* ke)
{
    if (shouldPassKey(ke->key()))
    {
        if (mPathEditorFormControllerHandle)
        {
            mPathEditorFormControllerHandle->keyPressEvent(ke);
        }
        else
        {
            mWindowComponentHandle->keyPressEvent(ke);
        }
    }
    else
    {
        QMainWindow::keyPressEvent(ke);
    }
}

void
MainWindowController::keyReleaseEvent
(QKeyEvent* ke)
{
    if (shouldPassKey(ke->key()))
    {
        if (mPathEditorFormControllerHandle)
        {
            mPathEditorFormControllerHandle->keyReleaseEvent(ke);
        }
        else
        {
            mWindowComponentHandle->keyReleaseEvent(ke);
        }
    }
    else
    {
        QMainWindow::keyReleaseEvent(ke);
    }
}

void
MainWindowController::onRightTabWidgetClosed
(QCloseEvent*)
{
    auto log = spdlog::get("MainWindowController");
    log->critical("Right tab widget closed");
    emit notifyRightTabWidgetClosed();
    mPathEditorFormControllerHandle = nullptr;
}

void MainWindowController::onNewTextureCreated()
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Echoing newTextureCreated()");
    emit notifyNewTextureCreated();
}

bool
MainWindowController::shouldPassKey
(int key)
{
    return std::find(
        begin(mKeysPassedToWindow),
        end(mKeysPassedToWindow),
        key
    ) != end(mKeysPassedToWindow);
}


CreateAssetDefinitionAction::CreateAssetDefinitionAction
(QString type, QObject* parent)
    : QAction(parent),
      mType(type) {}

CreateAssetDefinitionAction::CreateAssetDefinitionAction
(QString type,const  QString& text, QObject* parent)
    : QAction(text,parent),
      mType(type)
{}

CreateAssetDefinitionAction::CreateAssetDefinitionAction
(QString type, const QIcon& icon, const QString& text, QObject* parent)
    : QAction(icon,text,parent),
      mType(type) {}

QString
CreateAssetDefinitionAction::getType
()
{
    return mType;
}

ScenegraphMenuAction::ScenegraphMenuAction
(ScenegraphTreeItem *itemHandle, QObject* parent)
    : QAction(parent), mItemHandle(itemHandle) {}

ScenegraphMenuAction::ScenegraphMenuAction
(ScenegraphTreeItem *itemHandle,const  QString& text, QObject* parent)
    : QAction(text,parent), mItemHandle(itemHandle) {}

ScenegraphMenuAction::ScenegraphMenuAction
(ScenegraphTreeItem *itemHandle, const QIcon& icon, const QString& text, QObject* parent)
    : QAction(icon,text,parent), mItemHandle(itemHandle) {}

ScenegraphTreeItem*
ScenegraphMenuAction::getItemHandle
()
{
   return mItemHandle;
}

AddAssetToSceneObjectAction::AddAssetToSceneObjectAction
(ScenegraphTreeItem* itemHandle, IAssetDefinition* adHandle, QObject* parent)
    : QAction(parent),
      mItemHandle(itemHandle),
      mAssetDefinitionHandle(adHandle)
{

}

AddAssetToSceneObjectAction::AddAssetToSceneObjectAction
(ScenegraphTreeItem* itemHandle, IAssetDefinition* adHandle, const QString& text, QObject* parent)
    : QAction(text,parent),
      mItemHandle(itemHandle),
      mAssetDefinitionHandle(adHandle)

{

}

AddAssetToSceneObjectAction::AddAssetToSceneObjectAction
(ScenegraphTreeItem* itemHandle, IAssetDefinition* adHandle, const QIcon& icon, const QString& text, QObject* parent)
: QAction(icon,text,parent),
      mItemHandle(itemHandle),
      mAssetDefinitionHandle(adHandle)

{

}

ScenegraphTreeItem*
AddAssetToSceneObjectAction::getItemHandle
()
{
    return mItemHandle;
}

IAssetDefinition*
AddAssetToSceneObjectAction::getAssetDefinitionHandle
()
{
    return mAssetDefinitionHandle;
}


void MainWindowController::onMainVolumeChanged(int vol)
{
   auto log = spdlog::get("MainWindowController");
   log->debug("Volume changed to {}",vol);
   emit notifyMainVolumeChanged(vol);
}

DeleteAssetDefinitionAction::DeleteAssetDefinitionAction
(QObject* parent)
    : QAction(parent)
{

}

DeleteAssetDefinitionAction::DeleteAssetDefinitionAction
(const QString& text, QObject* parent)
    : QAction(text,parent)
{

}

DeleteAssetDefinitionAction::DeleteAssetDefinitionAction
(const QIcon& icon, const QString& text, QObject* parent)
    : QAction(icon,text,parent)
{

}

int
MainWindowController::isEditorTabOpen
(IAssetDefinition* definition)
{
    for (auto tab : mEditorTabForms)
    {
       if (tab->getDefinitionHandle() == definition)
       {
           return mUi->rightTabWidget->indexOf(tab.get())+1;
       }
    }
    return -1;
}

void
MainWindowController::onEditorTabCloseRequested
(int index)
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Requesting closure of tab {}",index);

    int editorIndex = index-1;
    if (editorIndex < 0)
    {
        return;
    }

    auto tabWidget = mEditorTabForms.at(editorIndex);

    auto type = tabWidget->getDefinitionHandle()->getType();
    if (type == Constants::ASSET_TYPE_SCRIPT || type == Constants::ASSET_TYPE_SHADER)
    {
        auto tab = dynamic_pointer_cast<TextEditorTabController>(tabWidget);
        if (tab->hasTextChanged())
        {
            int response = QMessageBox::question
            (
                this,
                "Save before closing?",
                "File contents has changed. Do you want to save before closing?"
            );

            if (response == QMessageBox::Yes)
            {
                tab->onSaveButtonClicked(true);
            }
        }
    }
    else if (type == Constants::ASSET_TYPE_AUDIO)
    {
        // Audio stuff here
    }

    mUi->rightTabWidget->removeTab(index);
    mEditorTabForms.erase(mEditorTabForms.begin() + editorIndex);
}

void
MainWindowController::openScriptEditor
(ScriptDefinition* scriptDefinitionHandle)
{
    auto log = spdlog::get("MainWindowController");

    int index = isEditorTabOpen(scriptDefinitionHandle);

    if (index > -1)
    {
        mUi->rightTabWidget->setCurrentIndex(index);
        return;
    }

    if (mProjectDirectoryModelHandle != nullptr)
    {
        //clearExistingTabs();
        auto form = make_shared<TextEditorTabController>
        (
            QString::fromStdString(scriptDefinitionHandle->getFormat()),
            scriptDefinitionHandle,
            mTemplatesModelHandle,
            mProjectDirectoryModelHandle
        );

        connect(
            form.get(),
            SIGNAL(msgToStatusBar(QString)),
            this,
            SLOT(showStatusBarMessage(QString))
        );

        QByteArray data = mProjectDirectoryModelHandle->readScriptData(scriptDefinitionHandle);
        form->useLuaHighlighter();
        form->setPlainText(QString(data));
        form->setDefinitionHandle(scriptDefinitionHandle);
        mEditorTabForms.push_back(form);

        int index = mUi->rightTabWidget->addTab
        (
            form.get(),
            QString::fromStdString(scriptDefinitionHandle->getName())
            .append(" (%1)")
            .arg(QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA))
        );

        mUi->rightTabWidget->setCurrentIndex(index);
    }
}



void
MainWindowController::openShaderEditor
(ShaderDefinition* shaderDefinitionHandle)
{
    auto log = spdlog::get("MainWindowController");
    int index = isEditorTabOpen(shaderDefinitionHandle);

    if (index > -1)
    {
        mUi->rightTabWidget->setCurrentIndex(index);
        return;
    }

    if (mProjectDirectoryModelHandle != nullptr)
    {
        //clearExistingTabs();
        ShaderFileTuple data = mProjectDirectoryModelHandle->readShaderData(shaderDefinitionHandle);

        auto vertexForm = make_shared<TextEditorTabController>(
            QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME),
            shaderDefinitionHandle,
            mTemplatesModelHandle,
            mProjectDirectoryModelHandle
        );

        connect(
            vertexForm.get(),
            SIGNAL(msgToStatusBar(QString)),
            this,
            SLOT(showStatusBarMessage(QString))
        );

        vertexForm->setPlainText(data.vertexShader);
        vertexForm->useGLSLHighlighter();

        mEditorTabForms.push_back(vertexForm);

        auto fragmentForm = make_shared<TextEditorTabController>(
            QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME),
            shaderDefinitionHandle,
            mTemplatesModelHandle,
            mProjectDirectoryModelHandle
        );

        connect(
            fragmentForm.get(),
            SIGNAL(msgToStatusBar(QString)),
            this,
            SLOT(showStatusBarMessage(QString))
        );

        fragmentForm->setPlainText(data.fragmentShader);
        fragmentForm->useGLSLHighlighter();

        mEditorTabForms.push_back(fragmentForm);

        int vertIndex = mUi->rightTabWidget->addTab(
                    vertexForm.get(),
                    QString::fromStdString(shaderDefinitionHandle->getName())
                    .append(" (%1)")
                    .arg(QString::fromStdString(Constants::SHADER_VERTEX))
                    );

        int fragIndex = mUi->rightTabWidget->addTab(
                    fragmentForm.get(),
                    QString::fromStdString(shaderDefinitionHandle->getName())
                    .append(" (%1)")
                    .arg(QString::fromStdString(Constants::SHADER_FRAGMENT))
                    );

        mUi->rightTabWidget->setCurrentIndex(vertIndex);
    }
}

void
MainWindowController::openMaterialEditor
(MaterialDefinition* adHandle, vector<ShaderDefinition*> shaders, vector<TextureDefinition*> textures)
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Opening Material editor tab for  {}",adHandle->getNameAndUuidString());
    int index = isEditorTabOpen(adHandle);

    if (index > -1)
    {
        log->debug("All ready open in tab {}",index);
        mUi->rightTabWidget->setCurrentIndex(index);
        return;
    }

    auto form = make_shared<MaterialEditorFormController>(adHandle,mProjectDirectoryModelHandle, mUi->rightTabWidget);
    connect(form.get(),SIGNAL(notifyNewTextureCreated()),this,SLOT(onNewTextureCreated()));
    form->populateShaderComboBox(shaders);
    form->populateTexturesMenu(textures);
    form->readDefinitionIntoUi();
    index = mUi->rightTabWidget->addTab
    (
        form.get(),
        QString::fromStdString(adHandle->getName())
    );

    log->debug("Created new tab at {}",index);

    mEditorTabForms.push_back(form);
    mUi->rightTabWidget->setCurrentIndex(index);

}

void
MainWindowController::openAudioEventEditor
(AudioDefinition* adHandle)
{
    auto log = spdlog::get("MainWindowController");
    log->debug("Opening audio editor tab for  {}",adHandle->getNameAndUuidString());
    int index = isEditorTabOpen(adHandle);

    if (index > -1)
    {
        log->debug("All ready open in tab {}",index);
        mUi->rightTabWidget->setCurrentIndex(index);
        return;
    }

    auto form = make_shared<AudioToolsFormController>(adHandle,mProjectDirectoryModelHandle, mUi->rightTabWidget);
    index = mUi->rightTabWidget->addTab
    (
        form.get(),
        QString::fromStdString(adHandle->getName())
    );

    log->debug("Created new tab at {}",index);

    mEditorTabForms.push_back(form);
    mUi->rightTabWidget->setCurrentIndex(index);
}

void
MainWindowController::setupEditorTabCloseButtonSignal
()
{
    connect(
        mUi->rightTabWidget,SIGNAL(tabCloseRequested(int)),
        this, SLOT(onEditorTabCloseRequested(int))
    );
}

void
MainWindowController::closeAllEditors
()
{
    for (auto editor : mEditorTabForms)
    {
        auto index = mUi->tabWidget->indexOf(editor.get());
        mUi->tabWidget->removeTab(index);
    }
    mEditorTabForms.clear();
}

void
MainWindowController::closeRightWidget
()
{
    mRightDockWidget.close();
}

void
MainWindowController::closeProject
()
{
    mWindowComponentHandle->clearRuntime();
    getScenegraphTreeView()->setModel(nullptr);
    getAssetDefinitionTreeView()->setModel(nullptr);
    getPropertiesTreeView()->setModel(nullptr);
    closeAllEditors();
    closeRightWidget();
}
