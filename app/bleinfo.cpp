#include "bleinfo.h"
#include <QFile>
#include <QTextStream>

BleInfo::BleInfo()
{
    m_macAddress = "282C02000000";  // or with ":" possible
    m_path = "/opt/zera/conf/bleSensorMAC";
}

bool BleInfo::readBleSensorMacAddr()
{
    QFile file(m_path);

    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            m_macAddress = stream.readLine();
            file.close();
            return true;
        }
    }
    return false;
}


QString BleInfo::getSensorMacAddr()
{
    return m_macAddress;
}
