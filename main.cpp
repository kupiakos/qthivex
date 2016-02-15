#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include <QDebug>
#include "hivemodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.showNormal();
    qDebug() << a.topLevelWidgets();
    return a.exec();
}
