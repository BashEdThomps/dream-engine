#include "MaterialShaderTableDelegate.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <QLineEdit>
#include <QComboBox>
#include <DreamCore.h>

MaterialShaderTableDelegate::MaterialShaderTableDelegate
(QObject* parent)
    : QItemDelegate (parent)
{
    auto log = spdlog::get("MaterialShaderTableDelegate");
    if (log==nullptr)
    {
       log=spdlog::stdout_color_mt("MaterialShaderTableDelegate");
    }
    log->debug("Constructing");
}

MaterialShaderTableDelegate::~MaterialShaderTableDelegate()
{
    auto log = spdlog::get("MaterialShaderTableDelegate");
    log->debug("Destructing");
}

QWidget*
MaterialShaderTableDelegate::createEditor
(QWidget* parent, const QStyleOptionViewItem& option, const  QModelIndex& index)
const
{
    Q_UNUSED(option)

    auto log = spdlog::get("MaterialShaderTableDelegate");

    QWidget* widget = nullptr;
    QString name;
    QComboBox* combo = nullptr;

    switch (index.column())
    {
        case 0:
           name = index.data().toString();
           log->debug("Creating QLineEdit Delegate for {}",name.toStdString());
           widget = new QLineEdit(name, parent);
           break;
        case 1:
            log->debug("Creating QComboBox Delegate for {}",name.toStdString());
            widget = new QComboBox(parent);
            combo = dynamic_cast<QComboBox*>(widget);
            for (auto shaderDef : mShaderDefinitions)
            {
                combo->addItem(
                    QString::fromStdString(shaderDef->getName()),
                    QString::fromStdString(shaderDef->getUuid())
                );
            }
            break;
    }
    return widget;
}

void
MaterialShaderTableDelegate::setShaderDefinitions
( vector<ShaderDefinition*>& shaderDefinitions)
{
    mShaderDefinitions = shaderDefinitions;
}

void MaterialShaderTableDelegate::setMaterialList(vector<string> materialList)
{
   mMaterialList = materialList;
}
