#ifndef HIVEVIEW_H
#define HIVEVIEW_H

#include <QTreeView>
#include "hiveitem.h"

class HiveView : public QTreeView
{
    Q_OBJECT
public:
    explicit HiveView(QWidget *parent = 0);

signals:
    void activeHiveItemChanged(HiveItem *item);

protected slots:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // HIVEVIEW_H
