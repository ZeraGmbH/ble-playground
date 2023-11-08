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

    static constexpr quint8 frameTypeAdvertisement = 3;
    static constexpr quint8 frameTypeScanResponse = 4;

    static constexpr quint32 errorTempUnvalidSlot = 1<<0;
    static constexpr quint32 errorTempExceedRange = 1<<1;
    static constexpr quint32 errorTypeSlot1 = 1<<2;
    static constexpr quint32 errorTypeSlot2 = 1<<3;
    static constexpr quint32 errorTypeSlot3 = 1<<4;
    static constexpr quint32 errorHumidExceedRange = 1<<5;
    static constexpr quint32 errorAirPressExceedRange = 1<<6;
    static constexpr quint32 errorHumidValueNegtive = 1<<7;
    static constexpr quint32 errorAirPressValueNegtive = 1<<8;
    static constexpr quint32 errorFrameTypeNotValid = 1<<9;

    static constexpr quint32 warningLowBattery = 1<<0;
    static constexpr quint32 warningEncryptionEnabled = 1<<1;
    static constexpr quint32 warningMeasuremPeriBaseFalse = 1<<2;
    static constexpr quint32 warningMeasuremPeriFactFalse = 1<<3;
    static constexpr quint32 warningNoCalibDateSet = 1<<4;

    bool isConnected();
    float getTemperaturInC();
    float getTemperaturInF();
    float getHumidity();
    float getAirPressure();
    quint32 getErrorFlags();
    quint16 getWarningFlags();
signals:
    void sigChangeConnectState();
    void sigNewValues();
    void sigNewWarnings();
    void sigNewErrors();

protected:
    bool isValidAdvertismentFrame(const QByteArray &manufData);
    bool isValidScanResponseFrame(const QByteArray &manufData);
    void decodeTemperature(const QByteArray &manufData, bool &valueChanged);
    void decodeHumidity(const QByteArray &manufData, bool &valueChanged);
    void decodeAirPressure(const QByteArray &manufData, bool &valueChanged);
    static float zigzagConvert(quint32 zigzagVal, float divisor);
    void resetErrorFlags();

private:
    void handleInvalid(const QByteArray &manufData);
    void decodeAdvertiseValues(const QByteArray &manufData);
    void decodeMeasureValues(const QByteArray &manufData);

    QBluetoothAddress m_address;
    quint32 m_errorFlags;
    quint16 m_warningFlags;
    quint8 m_firmwareVersion[3];
    float m_temperaturInC;
    float m_temperaturInF;
    float m_humidity;
    float m_airPressure;
    QString m_lastCalibration;
    bool m_isConnected;
};

#endif // EFENTOENVIRONMENTSENSOR_H
