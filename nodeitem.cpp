#include "nodeitem.h"

#include <QMutexLocker>
#include <algorithm>
#include <stdlib.h>
#include "valueitem.h"

NodeItem::NodeItem(HiveItem *parent, int index, QString name,
                   hive_h *hive, hive_node_h node) :
    HiveItem(parent, index, name),
    m_hive(hive), m_nodeHandle(node),
    m_loadedChildren(false), m_loadedValues(false)
{
    qDebug() << "Created" << *this;
}

NodeItem::NodeItem(HiveItem *parent, int index,
                   hive_h *hive, hive_node_h node) :
    NodeItem(parent, index, getNodeName(hive, node), hive, node)
{
}

NodeItem::~NodeItem()
{
    QMutexLocker locker{&m_childrenMutex};
    qDeleteAll(m_children);
    qDeleteAll(m_values);
    if (m_nodeHandle == hivex_root(m_hive))
    {
        qDebug() << "Closing Hive" << m_hive;
        hivex_close(m_hive);
    }
}

HiveItem *NodeItem::childItem(int index)
{
    loadChildren();
    QMutexLocker locker{&m_childrenMutex};
    HiveItem *child = static_cast<HiveItem *>(m_children.value(index, nullptr));
    if (!child)
    {
        qWarning() << "Could not load child of index" << index << "for" << *this;
    }
    return child;
}

int NodeItem::childCount() const
{
    size_t numChildren = hivex_node_nr_children(m_hive, m_nodeHandle);
    // qDebug() << *this << "has" << numChildren << "children";
    return numChildren;
}

bool NodeItem::isNode() const
{
    return true;
}

QList<HiveItem *> NodeItem::getValues()
{
    loadValues();
    QMutexLocker locker{&m_valuesMutex};
    return m_values;
}

void NodeItem::loadChildren()
{
    QMutexLocker locker{&m_childrenMutex};
    if (m_loadedChildren)
    {
        return;
    }

    qDebug() << "Loading children of" << *this;

    qDebug() << "Deleting current children";
    qDeleteAll(m_children);

    hive_node_h *nodes = hivex_node_children(m_hive, m_nodeHandle);
    if (!nodes)
    {
        qWarning() << "Could not load child nodes of" << *this;
        return;
    }

    qDebug() << "Loading child nodes";

    int childIndex = 0;
    for (hive_node_h *childNode = nodes; *childNode; ++childNode)
    {
        qDebug() << "Loading child node of index" << childIndex
                 << "hive " << m_hive << "node" << *childNode;
        m_children.append(new NodeItem{static_cast<HiveItem *>(this),
                                       childIndex, m_hive, *childNode});
        ++childIndex;
    }
    free(nodes);
    nodes = nullptr;

    qDebug() << "Sorting child nodes";
    std::stable_sort(m_children.begin(), m_children.end(),
                [](NodeItem * const &a, NodeItem * const &b) {
        return (a && b) ? QString::compare(a->name(), b->name(), Qt::CaseInsensitive) < 0 : false;
    });

    qDebug() << "Updating child indexes";
    childIndex = 0;
    for (const auto &child : m_children)
        child->updateIndex(childIndex++);

    m_loadedChildren = true;
}

void NodeItem::loadValues()
{
    QMutexLocker locker{&m_childrenMutex};
    if (m_loadedValues)
    {
        return;
    }

    qDebug() << "Loading values of" << *this;
    qDebug() << "Deleting current values";
    qDeleteAll(m_values);

    hive_value_h *values = hivex_node_values(m_hive, m_nodeHandle);
    if (!values)
    {
        qWarning() << "Could not load values of" << *this;
        return;
    }

    int index = 0;
    for (hive_value_h *value = values; *value; ++value)
    {
        qDebug() << "Loading value of index" << index
                 << "hive" << m_hive << "value" << *value;
        m_values.append(static_cast<HiveItem *>(
                            new ValueItem(this, index, m_hive, *value)));
        ++index;
    }
    free(values);
    values = nullptr;

    qDebug() << "Sorting values";
    std::stable_sort(m_values.begin(), m_values.end(),
                [](HiveItem * const &a, HiveItem * const &b) {
        return (a && b) ? QString::compare(a->name(), b->name(), Qt::CaseInsensitive) < 0 : false;
    });

    qDebug() << "Updating value indexes";
    index = 0;
    for (const auto &value : m_values)
        value->updateIndex(index++);

    m_loadedValues = true;
}

QString NodeItem::getNodeName(hive_h *hive, hive_node_h node)
{
    char *name = hivex_node_name(hive, node);
    if (name)
    {
        QString qName = QString::fromUtf8(name, hivex_node_name_len(hive, node));
        qDebug() << "Name of node" << node << "in hive" << hive << "is" << qName;
        free(name);
        return qName;
    }
    qWarning() << "Could not load node name of hive" << hive << "node" << node;
    return QString{};
}

QDebug operator<<(QDebug dbg, const NodeItem &node)
{
    QDebugStateSaver saver{dbg};
    Q_UNUSED(saver);
    dbg.nospace() << "NodeItem(name: " << node.name()
                            << ", hive: " << node.m_hive
                            << ", node: " << node.m_nodeHandle << ")";
    return dbg;
}
