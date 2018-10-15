#pragma onceo

#include <QItemDelegate>
#include <vector>
#include <string>
#include <memory>

namespace Dream
{
    class MaterialDefinition;
}

using Dream::MaterialDefinition;
using std::shared_ptr;
using std::vector;
using std::string;

class ModelMaterialTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ModelMaterialTableDelegate(QObject* parent  = nullptr);
    ~ModelMaterialTableDelegate() override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const  override;
    void setMaterialDefinitions(vector<MaterialDefinition*>& shaderDefinitions);
    void setMaterialList(vector<string> materialList);
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

protected:
    vector<MaterialDefinition*> mShaderDefinitions;
    vector<string> mMaterialList;
};

