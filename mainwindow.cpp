#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include "hivemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_hiveModel(nullptr)
{
    ui->setupUi(this);
    initIcons();
    initShortcuts();
}

void MainWindow::initIcons()
{
    ui->actionOpen_Hive->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
}

void MainWindow::initShortcuts()
{
    ui->actionOpen_Hive->setShortcut(QKeySequence::Open);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_hiveModel)
        delete m_hiveModel;
}

void MainWindow::on_actionOpen_Hive_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Hive"), QDir::homePath());
    if (!fileName.isEmpty())
    {
        HiveModel *model = HiveModel::loadHive(fileName.toUtf8().data());
        if (!model)
        {
            QMessageBox::critical(this, tr("Hive load error"), tr("Could not load hive file"));
        }
        else
        {
            if (m_hiveModel)
            {
                ui->nodeView->setModel(nullptr);
                delete m_hiveModel;
            }
            m_hiveModel = model;
            ui->nodeView->setModel(m_hiveModel);
        }
    }
}
