#ifndef VALUEITEM_H
#define VALUEITEM_H

#include <hivex.h>
#include "hiveitem.h"

class ValueItem : public HiveItem
{
public:
    ValueItem(HiveItem *parent, int index, hive_h *hive, hive_value_h value);

    virtual int valueType();
    virtual QString valueTypeString();
    virtual QByteArray rawData();
    virtual bool isNode() const override;
    virtual QVariant data();

private:
    void loadData();

    static QString getValueName(hive_h *hive, hive_value_h value);

    hive_h *m_hive;
    hive_value_h m_valueHandle;
    hive_type m_valueType;
    QByteArray m_rawData;
    QVariant m_data;

    friend QDebug operator<<(QDebug dbg, const ValueItem &value);
};

#endif // VALUEITEM_H
