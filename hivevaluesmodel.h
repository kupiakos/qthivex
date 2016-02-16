#ifndef HIVEVALUESMODEL_H
#define HIVEVALUESMODEL_H

#include <QAbstractTableModel>
#include <QFont>
#include "valueitem.h"

class HiveValuesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit HiveValuesModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    virtual void loadNode(HiveItem *item);

private:
    QFont m_italicFont;
    QList<ValueItem *> m_values;
};

#endif // HIVEVALUESMODEL_H
