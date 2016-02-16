#include "hivevaluesmodel.h"
#include <QVariant>
#include <QFont>

enum ValueColumn
{
    Name = 0,
    Type = 1,
    Data = 2
};

HiveValuesModel::HiveValuesModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int HiveValuesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_values.length();
}

int HiveValuesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QModelIndex HiveValuesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || column < 0 || column > columnCount(parent))
        return QModelIndex{};

    ValueItem *item = m_values.value(row, nullptr);
    if (!item)
        return QModelIndex{};

    return createIndex(row, column, item);
}

QVariant HiveValuesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ValueItem *item = static_cast<ValueItem *>(index.internalPointer());
    if (!item)
        return QVariant{};

    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case ValueColumn::Name:
        {
            QString name = item->name();
            // Default Value
            if (name.isEmpty())
            {
                name = "(Default)";
            }
            return name;
        }
        case ValueColumn::Type:
            return item->valueTypeString();
        case ValueColumn::Data:
            return item->data();
        }
        break;

    case Qt::FontRole:
        if (index.column() == ValueColumn::Name && item->name().isEmpty())
        {
            QFont font{};
            font.setItalic(true);
            return font;
        }
        break;
    }

    return QVariant{};
}

QVariant HiveValuesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant{};
    }
    switch (section)
    {
    case ValueColumn::Name:
        return "Name";
    case ValueColumn::Type:
        return "Type";
    case ValueColumn::Data:
        return "Data";
    default:
        return QVariant{};
    }
}

void HiveValuesModel::loadNode(HiveItem *item)
{
    beginResetModel();
    // The HiveItem given is responsible for deleting ValueItems.
    m_values.clear();
    if (item)
    {
        for (const auto &val : item->getValues())
        {
            auto valItem = dynamic_cast<ValueItem *>(val);
            if (valItem)
                m_values.append(valItem);
        }
    }
    endResetModel();
}
