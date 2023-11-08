#include "efentoenvironmentsensor.h"
#include "temperatureconverter.h"
#include <QDate>

static constexpr qint16 ManufId = 0x026C;

static constexpr quint8 measurementPeriodBaseZera = 15;
static constexpr quint8 sensorTypeTemperatur = 1;
static constexpr quint8 sensorTypeHumidity = 2;
static constexpr quint8 sensorTypeAirPressure = 3;

static constexpr quint32 tempMaxRawValue = 80000;
static constexpr quint32 humidityMaxRawValue = 200;
static constexpr quint16 airPressMaxRawValue = 40000;

static constexpr quint8 frameTypeAdvertisement = 3;
static constexpr quint8 frameTypeScanResponse = 4;

static constexpr quint8 frameSizeAdvertisment = 20;
static constexpr quint8 frameSizeScanResponse = 13;


EfentoEnvironmentSensor::EfentoEnvironmentSensor()
{
}

void EfentoEnvironmentSensor::setBluetoothAddress(QBluetoothAddress address)
{
    m_address = address;
}

void EfentoEnvironmentSensor::decode(const QBluetoothDeviceInfo &info)
{
    if (info.address() == m_address) {
        QByteArray manufData = info.manufacturerData(ManufId);
        if(isValidAdvertismentFrame(manufData)) {
            decodeAdvertiseValues(manufData);
        }
        else if(isValidScanResponseFrame(manufData)) {
            decodeMeasureValues(manufData);
        }
        else
            handleInvalid(manufData);
    }
}

bool EfentoEnvironmentSensor::isConnected()
{
    return m_isConnected;
}

float EfentoEnvironmentSensor::getTemperaturInC()
{
    return m_temperaturInC;
}

float EfentoEnvironmentSensor::getTemperaturInF()
{
    return m_temperaturInF;
}

float EfentoEnvironmentSensor::getHumidity()
{
    return m_humidity;
}

float EfentoEnvironmentSensor::getAirPressure()
{
    return m_airPressure;
}

quint32 EfentoEnvironmentSensor::getErrorFlags()
{
    return m_errorFlags;
}

quint16 EfentoEnvironmentSensor::getWarningFlags()
{
    return m_warningFlags;
}

bool EfentoEnvironmentSensor::isValidAdvertismentFrame(const QByteArray &manufData)
{
    if (manufData.length() >= frameSizeAdvertisment) {
        return manufData.at(0) == frameTypeAdvertisement;
    }
    return false;
}

bool EfentoEnvironmentSensor::isValidScanResponseFrame(const QByteArray &manufData)
{
    if (manufData.length() >= frameSizeScanResponse) {
        return manufData.at(0) == frameTypeScanResponse;
    }
    return false;
}

void EfentoEnvironmentSensor::handleInvalid(const QByteArray &manufData)
{
    qWarning("Efento: Unknown manufactoring data: %i", manufData.at(0));
}

void EfentoEnvironmentSensor::decodeAdvertiseValues(const QByteArray &manufData)
{
    quint16 oldWarningFlags = m_warningFlags;
    m_warningFlags = 0x00;
    quint8 hlpB;
    m_firmwareVersion[0] = manufData.at(7) & 0xF8;
    m_firmwareVersion[0] >>= 3;
    m_firmwareVersion[1] = manufData.at(8);
    m_firmwareVersion[1] >>= 5;
    hlpB = manufData.at(7) & 0x07;
    hlpB <<= 3;
    m_firmwareVersion[1] |= hlpB;
    m_firmwareVersion[2] = manufData.at(8) & 0x1F;
    if (!(manufData.at(9) & 0x01)) {
        qInfo("Sensor battery low detect");
        m_warningFlags |= warningLowBattery;
    }
    if (manufData.at(9) & 0x08) {
        qInfo("Encryption enabled");
        m_warningFlags |= warningEncryptionEnabled;
    }
    quint16 measurementPeriodBase = manufData.at(14);
    measurementPeriodBase <<= 8;
    measurementPeriodBase += manufData.at(15);
    if (measurementPeriodBase != measurementPeriodBaseZera)
        m_warningFlags |= warningMeasuremPeriBaseFalse;
    if ((manufData.at(16) != 0x00) || (manufData.at(17) != 0x01))
        m_warningFlags |= warningMeasuremPeriFactFalse;
    quint32 calibrationDay;
    calibrationDay = manufData.at(18);
    calibrationDay <<= 8;
    calibrationDay += manufData.at(19);
    if (calibrationDay == 0x00)
        m_warningFlags |= warningNoCalibDateSet;
    QDate calibDate(1970, 1, 1);
    calibDate = calibDate.addDays(calibrationDay);
    m_lastCalibration = calibDate.toString("dd.MM.yyyy");

    if(oldWarningFlags != m_warningFlags)
        emit sigNewWarnings();
    qInfo("Received ASdvertise-Values..");
}

void EfentoEnvironmentSensor::decodeMeasureValues(const QByteArray &manufData)
{
    quint32 oldErrorFlags = m_errorFlags;
    bool valueChanged = false;
    m_errorFlags = 0x00;
    qInfo("Received Measure-Values..");
    decodeTemperature(manufData, valueChanged);
    decodeHumidity(manufData, valueChanged);
    decodeAirPressure(manufData, valueChanged);
    if(valueChanged) {
        qInfo("   -> %2.2f°C  %2.2f°F  %2.0f%% RH  %2.2f hPa", m_temperaturInC, m_temperaturInF, m_humidity, m_airPressure);
        emit sigNewValues();
    }
    if(oldErrorFlags != m_errorFlags)
        emit sigNewErrors();
}

void EfentoEnvironmentSensor::decodeTemperature(const QByteArray &manufData, bool &valueChanged)
{
    if (manufData.at(1) == sensorTypeTemperatur) {
        quint32 temperatureRaw = (quint8)manufData.at(2);
        temperatureRaw <<= 8;
        temperatureRaw += (quint8)manufData.at(3);
        temperatureRaw <<= 8;
        temperatureRaw += (quint8)manufData.at(4);
        if (temperatureRaw > tempMaxRawValue)
            m_errorFlags |= errorTempExceedRange;
        else {
            float newTemp = zigzagConvert(temperatureRaw, 10.0);
            if(m_temperaturInC != newTemp) {
                m_temperaturInC = newTemp;
                m_temperaturInF = TemperatureConverter::celsiusToFahrenheit(m_temperaturInC);
                valueChanged = true;
                qInfo("   -> NEW temperature");
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot1;
}

void EfentoEnvironmentSensor::decodeHumidity(const QByteArray &manufData, bool &valueChanged)
{
    if (manufData.at(5) == sensorTypeHumidity) {
        quint32 humidityRaw = (quint8)manufData.at(6);
        humidityRaw <<= 8;
        humidityRaw += (quint8)manufData.at(7);
        humidityRaw <<= 8;
        humidityRaw += (quint8)manufData.at(8);
        if (humidityRaw > humidityMaxRawValue)
            m_errorFlags |= errorHumidExceedRange;
        else {
            float newHumidity = zigzagConvert(humidityRaw, 1.0);
            if(newHumidity != m_humidity) {
                m_humidity = newHumidity;
                if (m_humidity < 0)
                    m_errorFlags |= errorHumidValueNegtive;
                valueChanged = true;
                qInfo("   -> NEW humidity");
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot2;
}

void EfentoEnvironmentSensor::decodeAirPressure(const QByteArray &manufData, bool &valueChanged)
{
    if (manufData.at(9) == sensorTypeAirPressure) {
        quint32 airPressunreRaw = (quint8)manufData.at(10);
        airPressunreRaw <<= 8;
        airPressunreRaw += (quint8)manufData.at(11);
        airPressunreRaw <<= 8;
        airPressunreRaw += (quint8)manufData.at(12);
        if (airPressunreRaw > airPressMaxRawValue)
            m_errorFlags |= errorAirPressExceedRange;
        else {
            float newAirPressure = zigzagConvert(airPressunreRaw, 10.0);
            if(newAirPressure != m_airPressure) {
                m_airPressure = newAirPressure;
                if (newAirPressure < 0)
                    m_errorFlags |= errorAirPressValueNegtive;
                valueChanged = true;
                qInfo("   -> NEW Air-pressure");
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot3;
}

float EfentoEnvironmentSensor::zigzagConvert(quint32 zigzagVal, float divisor)
{
    qint32 valRaw = zigzagVal;
    bool isNegative = (valRaw & 0x01);
    if (isNegative)
        valRaw++;
    valRaw >>= 1;
    if (isNegative)
        valRaw *= -1;

    float val = valRaw;
    val /= divisor;
    return val;
}

void EfentoEnvironmentSensor::resetErrorFlags()
{
    m_errorFlags = 0;
}
