#pragma once

#include <DreamCore.h>
#include "../Model/TreeModels/Scenegraph/ScenegraphTreeItem.h"
#include <QAction>
#include <QString>

using Dream::IAssetDefinition;

class AddAssetToSceneObjectAction : public QAction
{
public:
    AddAssetToSceneObjectAction
    (ScenegraphTreeItem* itemHandle, IAssetDefinition* adHadle, QObject *parent = nullptr);

    AddAssetToSceneObjectAction
    (ScenegraphTreeItem* itemHandle, IAssetDefinition* adHadle, const QString &text, QObject *parent = nullptr);

    AddAssetToSceneObjectAction
    (ScenegraphTreeItem* itemHandle, IAssetDefinition* adHadle, const QIcon &icon, const QString &text, QObject *parent = nullptr);

    ScenegraphTreeItem* getItemHandle() ;
    IAssetDefinition* getAssetDefinitionHandle() ;

private:
    ScenegraphTreeItem* mItemHandle;
    IAssetDefinition* mAssetDefinitionHandle;

};

class ScenegraphMenuAction : public QAction
{
public:
    ScenegraphMenuAction(ScenegraphTreeItem* itemHandle, QObject *parent = nullptr);
    ScenegraphMenuAction(ScenegraphTreeItem* itemHandle, const QString &text, QObject *parent = nullptr);
    ScenegraphMenuAction(ScenegraphTreeItem* itemHandle, const QIcon &icon, const QString &text, QObject *parent = nullptr);

    ScenegraphTreeItem* getItemHandle() ;
private:
    ScenegraphTreeItem* mItemHandle;

};

class CreateAssetDefinitionAction : public QAction
{
public:
    CreateAssetDefinitionAction(QString type, QObject *parent = nullptr);
    CreateAssetDefinitionAction(QString type, const QString &text, QObject *parent = nullptr);
    CreateAssetDefinitionAction(QString type, const QIcon &icon, const QString &text, QObject *parent = nullptr);

    QString getType() ;

private:
    QString mType;
};

class DeleteAssetDefinitionAction : public QAction
{
public:
    DeleteAssetDefinitionAction
    (QObject *parent = nullptr);

    DeleteAssetDefinitionAction
    (const QString &text, QObject *parent = nullptr);

    DeleteAssetDefinitionAction
    (const QIcon &icon,const QString &text, QObject *parent = nullptr);

    IAssetDefinition* mItemHandle;
};
