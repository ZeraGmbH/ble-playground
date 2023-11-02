#ifndef EFENTOENVIRONMENTSENSOR_H
#define EFENTOENVIRONMENTSENSOR_H

#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>

class EfentoEnvironmentSensor : public QObject
{
    Q_OBJECT
public:
    EfentoEnvironmentSensor(QBluetoothAddress address);
    void decode(const QBluetoothDeviceInfo &info);

    bool isConnected();
    float getTemperaturInC();
    float getTemperaturInF();
    float getHumidity();
    float getAirPressure();

signals:
    void sigChangeConnectState();
    void sigNewValues();
    void sigNewWarnings();
    void sigNewErrors();

private:
    bool isAdvertisementFrame(const QByteArray &manufData);
    bool isScanResponseFrame(const QByteArray &manufData);
    void handleInvalid(const QByteArray &manufData);
    void decodeAdvertiseValues(const QByteArray &manufData);
    void decodeMeasureValues(const QByteArray &manufData);
    float zigzagConvert(unsigned long valueRaw, float divisor);

    QBluetoothAddress m_address;
    unsigned char m_softwareVerMaj;
    unsigned char m_softwareVerMin;
    unsigned long m_errorFlags;
    unsigned int m_warningFlags;
    unsigned char m_firmwareVersion[3];
    //bool m_battLevelOK;
    bool m_encryptionEnable;
    unsigned long m_measurementTs;
    unsigned int m_measurementPeriodBase;
    float m_temperaturInC;
    float m_temperaturInF;
    float m_humidity;
    float m_airPressure;
    unsigned int m_calibrationDate;
    bool m_isConnected;
};

#endif // EFENTOENVIRONMENTSENSOR_H