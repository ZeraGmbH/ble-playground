#include "efentoenvironmentsensor.h"
#include <QDate>

static constexpr qint16 ManufId = 0x026C;

static constexpr unsigned char measurementPeriodBaseZera = 15;
static constexpr unsigned char sensorTypeTemperatur = 1;
static constexpr unsigned char sensorTypeHumidity = 2;
static constexpr unsigned char sensorTypeAirPressure = 3;

static constexpr unsigned long tempMaxRawValue = 80000;
static constexpr unsigned long humidityMaxRawValue = 200;
static constexpr unsigned int airPressMaxRawValue = 40000;

static constexpr unsigned char frameTypeAdvertisement = 3;
static constexpr unsigned char frameTypeScanResponse = 4;

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


EfentoEnvironmentSensor::EfentoEnvironmentSensor(QBluetoothAddress address) :
    m_address(address)
{
}

void EfentoEnvironmentSensor::decode(const QBluetoothDeviceInfo &info)
{
    if (info.address() == m_address) {
        QByteArray manufData = info.manufacturerData(ManufId);
        if(isAdvertisementFrame(manufData)) {
            decodeAdvertiseValues(manufData);
        }
        else if(isScanResponseFrame(manufData)) {
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

float EfentoEnvironmentSensor::celsiusToFahrenheit(float tempCelsius)
{
    return tempCelsius * 1.8 + 32.0;
}

bool EfentoEnvironmentSensor::isAdvertisementFrame(const QByteArray &manufData)
{
    return manufData[0] == frameTypeAdvertisement;
}

bool EfentoEnvironmentSensor::isScanResponseFrame(const QByteArray &manufData)
{
    return manufData[0] == frameTypeScanResponse;
}

void EfentoEnvironmentSensor::handleInvalid(const QByteArray &manufData)
{
    qWarning("Efento: Unknown manufactoring data: %i", manufData[0]);
}

void EfentoEnvironmentSensor::decodeAdvertiseValues(const QByteArray &manufData)
{
    unsigned int oldWarningFlags = m_warningFlags;
    m_warningFlags = 0x00;
    unsigned char hlpB;
    m_firmwareVersion[0] = manufData[7] & 0xF8;
    m_firmwareVersion[0] >>= 3;
    m_firmwareVersion[1] = manufData[8];
    m_firmwareVersion[1] >>= 5;
    hlpB = manufData[7] & 0x07;
    hlpB <<= 3;
    m_firmwareVersion[1] |= hlpB;
    m_firmwareVersion[2] = manufData[8] & 0x1F;
    if (!(manufData[9] & 0x01)) {
        qInfo("Sensor battery low detect");
        m_warningFlags |= warningLowBattery;
    }
    if (manufData[9] & 0x08) {
        qInfo("Encryption enabled");
        m_warningFlags |= warningEncryptionEnabled;
    }
    unsigned int measurementPeriodBase = manufData[14];
    measurementPeriodBase <<= 8;
    measurementPeriodBase += manufData[15];
    if (measurementPeriodBase != measurementPeriodBaseZera)
        m_warningFlags |= warningMeasuremPeriBaseFalse;
    if ((manufData[16] != 0x00) || (manufData[17] != 0x01))
        m_warningFlags |= warningMeasuremPeriFactFalse;
    unsigned long calibrationDay;
    calibrationDay = manufData[18];
    calibrationDay <<= 8;
    calibrationDay += manufData[19];
    if (calibrationDay == 0x00)
        m_warningFlags |= warningNoCalibDateSet;
    QDate calibDate(1970, 1, 1);
    calibDate = calibDate.addDays(calibrationDay);
    m_lastCalibration = calibDate.toString("dd.MM.yyyy");

    if(oldWarningFlags != m_warningFlags)
        emit sigNewWarnings();
}

void EfentoEnvironmentSensor::decodeMeasureValues(const QByteArray &manufData)
{
    unsigned long oldErrorFlags = m_errorFlags;
    bool valueChanged = false;
    m_errorFlags = 0x00;
    if (manufData[1] == sensorTypeTemperatur) {
        m_temperaturInC = -9999.99; // todo better solution???
        m_temperaturInF = -9999.99;
        unsigned long temperatureRaw = manufData[2];
        temperatureRaw <<= 8;
        temperatureRaw += manufData[3];
        temperatureRaw <<= 8;
        temperatureRaw += manufData[4];
        if (temperatureRaw > tempMaxRawValue)
            m_errorFlags |= errorTempExceedRange;
        else {
            float newTemp = zigzagConvert(temperatureRaw, 10.0);
            if(m_temperaturInC != newTemp) {
                m_temperaturInC = newTemp;
                m_temperaturInF = celsiusToFahrenheit(m_temperaturInC);
                valueChanged = true;
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot1;

    if (manufData[5] == sensorTypeHumidity) {
        unsigned long humidityRaw = manufData[6];
        humidityRaw <<= 8;
        humidityRaw += manufData[7];
        humidityRaw <<= 8;
        humidityRaw += manufData[8];
        if (humidityRaw > humidityMaxRawValue)
            m_errorFlags |= errorHumidExceedRange;
        else {
            float newHumidity = zigzagConvert(humidityRaw, 1.0);
            if(newHumidity != m_humidity) {
                m_humidity = newHumidity;
                if (m_humidity < 0)
                    m_errorFlags |= errorHumidValueNegtive;
                valueChanged = true;
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot2;

    if (manufData[9] == sensorTypeAirPressure) {
        unsigned long airPressunreRaw = manufData[10];
        airPressunreRaw <<= 8;
        airPressunreRaw += manufData[11];
        airPressunreRaw <<= 8;
        airPressunreRaw += manufData[12];
        if (airPressunreRaw > airPressMaxRawValue)
            m_errorFlags |= errorAirPressExceedRange;
        else {
            float newAirPressure = zigzagConvert(airPressunreRaw, 10.0);
            if(newAirPressure != m_airPressure) {
                m_airPressure = newAirPressure;
                if (airPressunreRaw < 0)
                    m_errorFlags |= errorAirPressValueNegtive;
                valueChanged = true;
            }
        }
    }
    else
        m_errorFlags |= errorTypeSlot3;

    if(valueChanged)
        emit sigNewValues();
    if(oldErrorFlags != m_errorFlags)
        emit sigNewErrors();
}

float EfentoEnvironmentSensor::zigzagConvert(unsigned long valueRaw, float divisor)
{
    bool isNegative = false;
    float val;
    if (valueRaw & 0x01)      // value is negativ
        isNegative = true;
    valueRaw >>= 1;
    val = valueRaw;
    if (isNegative)
        val *= -1;
    val /= divisor;         // todo protect against 0 ????
    return val;
}
