#include "valueitem.h"
#include <QDebug>

ValueItem::ValueItem(HiveItem *parent, int index,
                     hive_h *hive, hive_value_h value) :
    HiveItem(parent, index, getValueName(hive, value)),
    m_hive(hive), m_valueHandle(value)
{
    loadData();
}

int ValueItem::valueType()
{
    return m_valueType;
}

QString ValueItem::valueTypeString()
{
    switch (m_valueType)
    {
    case hive_t_REG_NONE:
        return "REG_NONE";
    case hive_t_REG_SZ:
        return "REG_SZ";
    case hive_t_REG_EXPAND_SZ:
        return "REG_EXPAND_SZ";
    case hive_t_REG_BINARY:
        return "REG_BINARY";
    case hive_t_REG_DWORD:
        return "REG_DWORD";
    case hive_t_REG_DWORD_BIG_ENDIAN:
        return "REG_DWORD_BIG_ENDIAN";
    case hive_t_REG_LINK:
        return "REG_LINK";
    case hive_t_REG_MULTI_SZ:
        return "REG_MULTI_SZ";
    case hive_t_REG_RESOURCE_LIST:
        return "REG_RESOURCE_LIST";
    case hive_t_REG_FULL_RESOURCE_DESCRIPTOR:
        return "REG_FULL_RESOURCE_DESCRIPTOR";
    case hive_t_REG_RESOURCE_REQUIREMENTS_LIST:
        return "REG_RESOURCE_REQUIREMENTS_LIST";
    case hive_t_REG_QWORD:
        return "REG_QWORD";
    default:
        return "UNKNOWN";
    }
}

QByteArray ValueItem::rawData()
{
    return m_rawData;
}

bool ValueItem::isNode() const
{
    return false;
}

QVariant ValueItem::data()
{
    return m_data;
}

void ValueItem::loadData()
{
    size_t len;
    hive_type valueType;
    char *raw = hivex_value_value(m_hive, m_valueHandle, &valueType, &len);
    if (!raw)
    {
        qCritical() << "Could not load hive data";
        return;
    }
    m_valueType = valueType;
    m_rawData = QByteArray{raw, static_cast<int>(len)};

    switch (valueType)
    {
    case hive_t_REG_SZ:
    case hive_t_REG_EXPAND_SZ:
    case hive_t_REG_LINK:
    {
        char *str = hivex_value_string(m_hive, m_valueHandle);
        if (!str)
        {
            m_data = QString{"<ERROR>"};
            break;
        }
        m_data = QString::fromUtf8(str);
        free(str);
        break;
    }
    case hive_t_REG_MULTI_SZ:
    {
        char **strings = hivex_value_multiple_strings(m_hive, m_valueHandle);
        QStringList strList;
        if (!strings)
        {
            strList << "<ERROR>";
            m_data = strList;
            break;
        }
        for (char **str = strings; *str; str++)
        {
            strList << QString::fromUtf8(*str);
            free(*str);
        }
        m_data = strList;
        free(strings);
        break;
    }
    case hive_t_REG_DWORD:
    case hive_t_REG_DWORD_BIG_ENDIAN:
        m_data = static_cast<qint32>(hivex_value_dword(m_hive, m_valueHandle));
        break;
    case hive_t_REG_QWORD:
        m_data = static_cast<qint64>(hivex_value_qword(m_hive, m_valueHandle));
        break;
    default:
        m_data = m_rawData;
        break;
    }
}

QString ValueItem::getValueName(hive_h *hive, hive_value_h value)
{
    char *name = hivex_value_key(hive, value);
    if (!name)
    {
        qCritical() << "Could not load value name of hive"
                    << hive << "value" << value;
        return "<ERROR>";
    }
    QString qName = QString::fromUtf8(name, hivex_value_key_len(hive, value));
    qDebug() << "Name of value" << value << "in hive" << hive << "is" << qName;
    free(name);
    return qName;
}

QDebug operator<<(QDebug dbg, const ValueItem &value)
{
    QDebugStateSaver saver{dbg};
    Q_UNUSED(saver);
    dbg.nospace() << "ValueItem(name: " << value.name()
                  << ", hive: " << value.m_hive
                  << ", handle: " << value.m_valueHandle
                  << ", type: " << value.m_valueType << ")";
    return dbg;
}
