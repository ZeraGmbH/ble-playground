#ifndef BLEINFO_H
#define BLEINFO_H
#include <QObject>  //???
#include <QString>

class BleInfo
{
public:
    BleInfo();

    bool    readBleSensorMacAddr();
    QString getSensorMacAddr();
    QString m_macAddress;

private:
    QString m_path;
};

#endif // BLEINFO_H
