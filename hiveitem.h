#ifndef HIVEITEM_H
#define HIVEITEM_H

#include <QMutex>
#include <QString>
#include <QList>
#include <QVariant>
#include <QPair>

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

    // Display string & whether "special" or not
    virtual QPair<QString, bool> dataDisplay() const;

    virtual int childCount() const;
    virtual bool hasChildren() const;
    virtual bool isNode() const = 0;
    virtual QList<HiveItem *> getValues();
    void updateIndex(int index);

private:
    HiveItem *m_parentItem;
    int m_index;
    QString m_name;
};

#endif // HIVEITEM_H
