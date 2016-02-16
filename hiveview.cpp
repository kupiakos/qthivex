#include "hiveview.h"

HiveView::HiveView(QWidget *parent) : QTreeView(parent)
{

}

void HiveView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTreeView::currentChanged(current, previous);
    if (current.isValid())
        emit activeHiveItemChanged(static_cast<HiveItem *>(current.internalPointer()));
    else
        emit activeHiveItemChanged(nullptr);
}

