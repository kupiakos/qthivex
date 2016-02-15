#ifndef HIVEITEM_H
#define HIVEITEM_H

#include <QMutex>
#include <QString>
#include <QVariant>

class HiveItem
{
public:
    HiveItem(HiveItem *parent, int index, QString name);
    virtual ~HiveItem();

    virtual HiveItem *parentItem();
    virtual HiveItem *childItem(int index);

    virtual int index() const;
    virtual QString name() const;
    virtual QVariant data() const;

    virtual int childCount() const;
    virtual bool hasChildren() const;
    virtual bool isNode() const = 0;

protected:
    void updateIndex(int index);

private:
    HiveItem *m_parentItem;
    int m_index;
    QString m_name;
};

#endif // HIVEITEM_H
