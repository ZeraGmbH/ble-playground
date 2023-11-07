#ifndef EFENTOENVIRONMENTSENSOR_H
#define EFENTOENVIRONMENTSENSOR_H

#include "bluetoothdeviceinfodecoder.h"
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>

class EfentoEnvironmentSensor : public BluetoothDeviceInfoDecoder
{
    Q_OBJECT
public:
    EfentoEnvironmentSensor();
    void setBluetoothAddress(QBluetoothAddress address);
    void decode(const QBluetoothDeviceInfo &info) override;

    static constexpr unsigned long errorTempUnvalidSlot = 1<<0;
    static constexpr unsigned long errorTempExceedRange = 1<<1;
    static constexpr unsigned long errorTypeSlot1 = 1<<2;
    static constexpr unsigned long errorTypeSlot2 = 1<<3;
    static constexpr unsigned long errorTypeSlot3 = 1<<4;
    static constexpr unsigned long errorHumidExceedRange = 1<<5;
    static constexpr unsigned long errorAirPressExceedRange = 1<<6;
    static constexpr unsigned long errorHumidValueNegtive = 1<<7;
    static constexpr unsigned long errorAirPressValueNegtive = 1<<8;
    static constexpr unsigned long errorFrameTypeNotValid = 1<<9;

    static constexpr unsigned long warningLowBattery = 1<<0;
    static constexpr unsigned long warningEncryptionEnabled = 1<<1;
    static constexpr unsigned long warningMeasuremPeriBaseFalse = 1<<2;
    static constexpr unsigned long warningMeasuremPeriFactFalse = 1<<3;
    static constexpr unsigned long warningNoCalibDateSet = 1<<4;

    bool isConnected();
    float getTemperaturInC();
    float getTemperaturInF();
    float getHumidity();
    float getAirPressure();
    unsigned long getErrorFlags();
    unsigned int getWarningFlags();
signals:
    void sigChangeConnectState();
    void sigNewValues();
    void sigNewWarnings();
    void sigNewErrors();

protected:
    void decodeTemperature(const QByteArray &manufData, bool &valueChanged);
    void decodeHumidity(const QByteArray &manufData, bool &valueChanged);
    void decodeAirPressure(const QByteArray &manufData, bool &valueChanged);
    static float zigzagConvert(unsigned long zigzagVal, float divisor);

private:
    bool isAdvertisementFrame(const QByteArray &manufData);
    bool isScanResponseFrame(const QByteArray &manufData);
    void handleInvalid(const QByteArray &manufData);
    void decodeAdvertiseValues(const QByteArray &manufData);
    void decodeMeasureValues(const QByteArray &manufData);

    QBluetoothAddress m_address;
    unsigned long m_errorFlags;
    unsigned int m_warningFlags;
    unsigned char m_firmwareVersion[3];
    float m_temperaturInC;
    float m_temperaturInF;
    float m_humidity;
    float m_airPressure;
    QString m_lastCalibration;
    bool m_isConnected;
};

#endif // EFENTOENVIRONMENTSENSOR_H
