#ifndef NODEITEM_H
#define NODEITEM_H

#include <hivex.h>
#include <hiveitem.h>
#include <QList>
#include <QDebug>

class NodeItem : public HiveItem
{
public:
    NodeItem(HiveItem *parent, int index, QString name,
             hive_h *hive, hive_node_h node);
    NodeItem(HiveItem *parent, int index,
             hive_h *hive, hive_node_h node);
    virtual ~NodeItem();

    virtual HiveItem *childItem(int index) override;

    virtual int childCount() const override;
    virtual bool isNode() const override;

private:
    void loadChildren();

    static QString getNodeName(hive_h *hive, hive_node_h node);

    hive_h *m_hive;
    hive_node_h m_node;
    bool m_haveLoaded;
    QList<NodeItem *> m_children;

    QMutex m_childrenMutex;

    friend QDebug operator<<(QDebug dbg, const NodeItem &node);
};

#endif // NODEITEM_H
