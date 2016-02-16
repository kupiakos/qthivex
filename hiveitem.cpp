#include "hiveitem.h"

HiveItem::HiveItem(HiveItem *parent, int index, QString name) :
    m_parentItem(parent), m_index(index), m_name(name)
{
}

HiveItem::~HiveItem()
{
}

HiveItem *HiveItem::parentItem()
{
    return m_parentItem;
}

HiveItem *HiveItem::childItem(int index)
{
    return nullptr;
}

int HiveItem::index() const
{
    return m_index;
}

QString HiveItem::name() const
{
    return m_name;
}

QVariant HiveItem::data() const
{
    return QVariant{};
}

int HiveItem::childCount() const
{
    return 0;
}

bool HiveItem::hasChildren() const
{
    return childCount() > 0;
}

QList<HiveItem *> HiveItem::getValues()
{
    return QList<HiveItem *>{};
}

void HiveItem::updateIndex(int index)
{
    m_index = index;
}
