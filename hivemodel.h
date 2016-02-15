#ifndef HIVEMODEL_H
#define HIVEMODEL_H

#include <QAbstractItemModel>
#include "hiveitem.h"

class HiveModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    static HiveModel *loadHive(char *hiveLocation);
    virtual ~HiveModel();

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool hasChildren(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    explicit HiveModel(HiveItem *root, QObject *parent = 0);

    HiveItem *m_root;
};

#endif // HIVEMODEL_H
