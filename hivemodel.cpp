#include "hivemodel.h"
#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <hivex.h>
#include <errno.h>
#include "nodeitem.h"

HiveModel::HiveModel(HiveItem *root, QObject *parent)
    : QAbstractItemModel(parent), m_root(root)
{
}

HiveModel *HiveModel::loadHive(char *hiveLocation)
{
    hive_h *hive = hivex_open(hiveLocation, 0);
    if (!hive)
    {
        qCritical() << "Error code" << errno
                    << "while loading hive from location" << hiveLocation;
        return nullptr;
    }

    hive_node_h root = hivex_root(hive);
    if (!root)
    {
        qCritical() << "Error code" << errno
                    << "while finding root hive" << hiveLocation;
    }

    HiveItem *rootItem = static_cast<HiveItem *>(
                new NodeItem(nullptr, 0, "<ROOT>", hive, root));

    return new HiveModel(rootItem);
}

HiveModel::~HiveModel()
{
    if (m_root)
        delete m_root;
}

QModelIndex HiveModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_root)
        return QModelIndex{};

    HiveItem *parentItem;
    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<HiveItem *>(parent.internalPointer());

    if (!parentItem)
    {
        qWarning() << "Cannot index into invalid hive item";
        return QModelIndex{};
    }

    HiveItem *childItem = parentItem->childItem(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex{};
}

QModelIndex HiveModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || !m_root)
        return QModelIndex{};

    HiveItem *childItem = static_cast<HiveItem *>(index.internalPointer());
    if (!childItem)
    {
        qWarning() << "Cannot get parent of invalid hive item";
        return QModelIndex{};
    }

    HiveItem *parentItem = childItem->parentItem();

    if (!parentItem || parentItem == m_root)
        return QModelIndex{};

    return createIndex(parentItem->index(), 0, parentItem);
}

bool HiveModel::hasChildren(const QModelIndex &parent) const
{
    if (!m_root)
        return false;

    HiveItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<HiveItem *>(parent.internalPointer());

    if (!parentItem)
    {
        qWarning() << "Cannot check children of invalid hive item";
        return false;
    }

    bool hasChildren = parentItem->hasChildren();
    // qDebug() << "HiveItem" << parentItem->name() << "has children:" << hasChildren;

    return hasChildren;
}

int HiveModel::rowCount(const QModelIndex &parent) const
{
    HiveItem *parentItem;
    if (parent.column() > 0 || !m_root)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<HiveItem *>(parent.internalPointer());

    if (!parentItem)
    {
        qWarning() << "Cannot get child count of invalid hive item";
        return 0;
    }

    int count = parentItem->childCount();
    // qDebug() << "HiveItem" << parentItem->name() << "has" << count << "children";

    return count;
}

int HiveModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant HiveModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_root)
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::DecorationRole)
        return QVariant{};

    HiveItem *item = static_cast<HiveItem *>(index.internalPointer());
    if (!item)
    {
        qWarning() << "Could not load data for item";
        return QVariant{};
    }

    switch (role)
    {
    case Qt::DisplayRole:
        return item->name();
    case Qt::DecorationRole:
        if (item->isNode())
            return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        return QVariant{};
    }
    return QVariant{};
}

Qt::ItemFlags HiveModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || !m_root)
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant HiveModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    if (!m_root)
        return QVariant{};
//    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//        return QString{"Name"};
    return QVariant{};
}
