#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hivemodel.h"
#include "hiveitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_Hive_triggered();
    void onNodeActivated(HiveItem *item);

private:
    void initConnections();
    void initIcons();
    void initShortcuts();

    Ui::MainWindow *ui;
    HiveModel *m_hiveModel;
};

#endif // MAINWINDOW_H
