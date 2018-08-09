#pragma onceo

#include <QItemDelegate>
#include <vector>
#include <string>

namespace Dream
{
    class ShaderDefinition;
}

using Dream::ShaderDefinition;
using std::vector;
using std::string;

class MaterialShaderTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MaterialShaderTableDelegate(QObject* parent  = nullptr);
    ~MaterialShaderTableDelegate() override;
    QWidget*createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setShaderDefinitions(const vector<ShaderDefinition*>& shaderDefinitions);

    void setMaterialList(vector<string> materialList);
protected:
    vector<ShaderDefinition*> mShaderDefinitions;
    vector<string> mMaterialList;
};

