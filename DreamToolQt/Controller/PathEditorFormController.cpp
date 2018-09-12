#include "PathEditorFormController.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <DreamCore.h>
#include <QMessageBox>

PathEditorFormController::PathEditorFormController
(QWidget *parent)
    : QWidget(parent),
      mPathDefinitionHandle(nullptr)
{
    auto log = spdlog::get("PathEditorFormController");
    if (log==nullptr)
    {

        log = spdlog::stdout_color_mt("PathEditorFormController");
    }
    log->trace("Constructing");
    mUi.setupUi(this);
    setWindowTitle("Path Editor");
    connect(mUi.addButton,SIGNAL(clicked(bool)),this,SLOT(onAddButtonClicked(bool)));
    connect(mUi.removeButton,SIGNAL(clicked(bool)),this,SLOT(onRemoveButtonClicked(bool)));
    connect(&mTableModel,SIGNAL(changed()),this,SLOT(onTableChanged()));
    connect(mUi.pathVisibleButton,SIGNAL(clicked(bool)),this,SLOT(onPathVisibleButtonClicked(bool)));
    connect(mUi.tangentButton,SIGNAL(clicked(bool)),this,SLOT(onTangentVisibleButtonClicked(bool)));
    connect(mUi.tangentIndexSlider,SIGNAL(valueChanged(int)),this,SLOT(onTangentIndexChanged(int)));
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

PathEditorFormController::~PathEditorFormController
()
{
    auto log = spdlog::get("PathEditorFormController");
    log->trace("Destructing");
}

void PathEditorFormController::setProjectPath(QString projectPath)
{
    mProjectPath = projectPath;
}

void
PathEditorFormController::setPathDefinition
(shared_ptr<PathDefinition> def)
{
    mPathDefinitionHandle = def;
    populate();
}

void
PathEditorFormController::onAddButtonClicked
(bool)
{
    auto log = spdlog::get("PathEditorFormController");
    log->trace("Add Button Clicked");
    mTableModel.insertRows(mTableModel.rowCount(QModelIndex()),1,QModelIndex());
    emit notifyTableChanged();
}

void
PathEditorFormController::onRemoveButtonClicked
(bool)
{
    auto log = spdlog::get("PathEditorFormController");
    log->trace("Remove Button Clicked");
    auto selected = mUi.tableView->currentIndex();
    mTableModel.removeRows(selected.row(),1,QModelIndex());
    emit notifyTableChanged();
}

void PathEditorFormController::onTableChanged()
{

    auto log = spdlog::get("PathEditorFormController");
    log->critical("Table Changed");
    emit notifyTableChanged();
}

void
PathEditorFormController::onTableSelectionChanged
(const QItemSelection& sel, const QItemSelection& desel)
{
    Q_UNUSED(desel)
    auto log = spdlog::get("PathEditorFormController");
    if (sel.count() == 0)
    {
        return;
    }
    auto indexes = sel.first().indexes();
    if (indexes.count() == 0)
    {
        return;
    }

    int row = indexes.first().row();
    log->critical("Selected row {}",row);
    emit notifySelectedRowChanged(row);
}

void PathEditorFormController::onPathVisibleButtonClicked(bool visible)
{
    emit notifyPathVisibilityChanged(visible);
}

void PathEditorFormController::onTangentVisibleButtonClicked(bool visible)
{
   emit notifyTangentVisibilityChanged(visible);
}

void PathEditorFormController::onTangentIndexChanged(int val)
{
   emit notifyTangentIndexChanged(val);
}

void
PathEditorFormController::populate
()
{
    auto log = spdlog::get("PathEditorFormController");
    log->info("Populating");
    if (mPathDefinitionHandle != nullptr)
    {
        log->info("PathDefinition is present");
        mTableModel.setPathDefinition(mPathDefinitionHandle);
        mUi.tableView->setModel(&mTableModel);
        connect(
        mUi.tableView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(onTableSelectionChanged(const QItemSelection &, const QItemSelection &))
        );
        mUi.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else
    {
        log->error("Cannot populate PathEditor table, model definition is null");
    }
}

void
PathEditorFormController::getAllUpInYourFace
()
{
    show();
    activateWindow();
    raise();
    setFocus();
}

void PathEditorFormController::onNumberOfTangentsChanged(int num)
{
   mUi.tangentIndexSlider->setMinimum(0);
   mUi.tangentIndexSlider->setMaximum(num);
   mUi.tangentIndexSlider->setTickInterval(1);
}

void
PathEditorFormController::closeEvent
(QCloseEvent* event)
{
    emit notifyCloseEvent();
    QWidget::closeEvent(event);
}
