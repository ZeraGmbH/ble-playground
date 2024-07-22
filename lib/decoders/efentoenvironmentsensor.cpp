#include "efentoenvironmentsensor.h"
#include "temperatureconverter.h"
#include <QDate>
#include <timerfactoryqt.h>

static constexpr qint16 ManufId = 0x026C;

static constexpr quint8 measurementPeriodBaseZera = 15;

static constexpr quint8 posSensorTypeTemperatur = 1;
static constexpr quint8 sensorTypeTemperatur = 1;
static constexpr quint8 posSensorTypeHumidity = 5;
static constexpr quint8 sensorTypeHumidity = 2;
static constexpr quint8 posSensorTypeAirPressure = 9;
static constexpr quint8 sensorTypeAirPressure = 3;
static constexpr quint8 parameterDataBytes = 3;

static constexpr quint32 tempMaxRawValue = 80000;
static constexpr quint32 humidityMaxRawValue = 200;
static constexpr quint16 airPressMaxRawValue = 40000;

static constexpr quint8 ending_bytes_to_ignore = 2;
static constexpr quint8 frameSizeAdvertisment = 20 - ending_bytes_to_ignore;
static constexpr quint8 frameSizeScanResponse = 11 - ending_bytes_to_ignore;


void EfentoEnvironmentSensor::setBluetoothAddress(QBluetoothAddress validAddress)
{
    m_address = validAddress;

    m_timeoutTimer = TimerFactoryQt::createSingleShot(30000);
    connect(m_timeoutTimer.get(), &TimerTemplateQt::sigExpired, this, &EfentoEnvironmentSensor::onTimeout);
}

void EfentoEnvironmentSensor::onTimeout()
{
    EfentoEnvironmentSensor::resetMeasureValues();
    m_warningFlags |= warningSensorLost;
    qInfo("BLE Sensor lost");
    m_timeoutTimer->start();
    emit sigNewValues();
}

void EfentoEnvironmentSensor::decode(const QBluetoothDeviceInfo &info)
{
    if (info.address() == m_address) {
        QByteArray manufData = info.manufacturerData(ManufId);
        // Last 2 bytes are CRC
        manufData.remove(manufData.size()-ending_bytes_to_ignore, ending_bytes_to_ignore);
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

double EfentoEnvironmentSensor::getTemperaturInC()
{
    if(qIsNaN(m_temperaturInC))
        return (m_temperaturInC);
    double d_temperaturInC = (double)m_temperaturInC;
    d_temperaturInC *= 10.0;
    d_temperaturInC = qRound(d_temperaturInC);
    d_temperaturInC /= 10.0;
    return d_temperaturInC;
}

double EfentoEnvironmentSensor::getTemperaturInF()
{
    if (qIsNaN(m_temperaturInF))
        return(m_temperaturInF);
    double d_temperaturInF = (double)m_temperaturInF;
    d_temperaturInF *= 100.0;
    d_temperaturInF = qRound(d_temperaturInF);
    d_temperaturInF /= 100.0;
    return d_temperaturInF;
}

double EfentoEnvironmentSensor::getHumidity()
{
    if (qIsNaN(m_humidity))
        return m_humidity;
    double d_humidity = (double)m_humidity;
    d_humidity *= 10.0;
    d_humidity = qRound(d_humidity);
    d_humidity /= 10.0;
    return d_humidity;
}

double EfentoEnvironmentSensor::getAirPressure()
{
    if (qIsNaN(m_airPressure))
        return m_airPressure;
    double d_airPressure = (double)m_airPressure;
    d_airPressure *= 10.0;
    d_airPressure = qRound(d_airPressure);
    d_airPressure /= 10.0;
    return d_airPressure;
}

quint32 EfentoEnvironmentSensor::getErrorFlags()
{
    return m_errorFlags;
}

quint32 EfentoEnvironmentSensor::getWarningFlags()
{
    return m_warningFlags;
}

bool EfentoEnvironmentSensor::isValidAdvertismentFrame(const QByteArray &manufData)
{
    if (manufData.length() >= frameSizeAdvertisment)
        return manufData.at(0) == frameTypeAdvertisement;
    return false;
}

bool EfentoEnvironmentSensor::isValidScanResponseFrame(const QByteArray &manufData)
{
    if (manufData.length() >= frameSizeScanResponse)
        return manufData.at(0) == frameTypeScanResponse;
    return false;
}

void EfentoEnvironmentSensor::handleInvalid(const QByteArray &manufData)
{
    qWarning("Efento: Unknown manufactoring data: %i", manufData.at(0));
}

void EfentoEnvironmentSensor::decodeAdvertiseValues(const QByteArray &manufData)
{
    m_warningFlags &= warningSensorLost;      // reset each bit except SensorLost-bit
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
        qInfo("BLE Sensor battery low detect");
        m_warningFlags |= warningLowBattery;
    }
    if (manufData.at(9) & 0x08) {
        qInfo("BLE Sensor Encryption enabled");
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
    if (m_lastCalibration.isEmpty()) {
        m_lastCalibration = calibDate.toString("dd.MM.yyyy");
        qInfo("BLE Sensor last Calibration: %s", qPrintable(m_lastCalibration));
    }
    m_timeoutTimer->start();
    emit sigNewWarnings();
}

void EfentoEnvironmentSensor::decodeMeasureValues(const QByteArray &manufData)
{
    m_errorFlags = 0x00;
    decodeTemperature(manufData);
    decodeHumidity(manufData);
    decodeAirPressure(manufData);
    if (!m_errorFlags) {
        m_isConnected = true;
        m_warningFlags &= ~warningSensorLost;
        m_timeoutTimer->start();
        emit sigNewValues();
    }
    else
        qInfo("BLE Sensor error %d in decodeMeasureValues", m_errorFlags);
    emit sigNewErrors();
}

void EfentoEnvironmentSensor::decodeTemperature(const QByteArray &manufData)
{
    if (isParameterPresent(manufData, posSensorTypeTemperatur, sensorTypeTemperatur)) {
        quint32 temperatureRaw = (quint8)manufData.at(2);
        temperatureRaw <<= 8;
        temperatureRaw += (quint8)manufData.at(3);
        temperatureRaw <<= 8;
        temperatureRaw += (quint8)manufData.at(4);
        if (temperatureRaw > tempMaxRawValue)
            m_errorFlags |= errorTempExceedRange;
        else {
            m_temperaturInC = zigzagConvert(temperatureRaw, 10.0);
            m_temperaturInF = TemperatureConverter::celsiusToFahrenheit(m_temperaturInC);
        }
    }
    else
        m_errorFlags |= errorTypeSlot1;
}

void EfentoEnvironmentSensor::decodeHumidity(const QByteArray &manufData)
{
    if (isParameterPresent(manufData, posSensorTypeHumidity, sensorTypeHumidity)) {
        quint32 humidityRaw = (quint8)manufData.at(6);
        humidityRaw <<= 8;
        humidityRaw += (quint8)manufData.at(7);
        humidityRaw <<= 8;
        humidityRaw += (quint8)manufData.at(8);
        if (humidityRaw > humidityMaxRawValue)
            m_errorFlags |= errorHumidExceedRange;
        else {
            m_humidity = zigzagConvert(humidityRaw, 1.0);
            if (m_humidity < 0)
                m_errorFlags |= errorHumidValueNegtive;
        }
    }
    else
        m_errorFlags |= errorTypeSlot2;
}

void EfentoEnvironmentSensor::decodeAirPressure(const QByteArray &manufData)
{
    if (isParameterPresent(manufData, posSensorTypeAirPressure, sensorTypeAirPressure)) {
        quint32 airPressunreRaw = (quint8)manufData.at(10);
        airPressunreRaw <<= 8;
        airPressunreRaw += (quint8)manufData.at(11);
        airPressunreRaw <<= 8;
        airPressunreRaw += (quint8)manufData.at(12);
        if (airPressunreRaw > airPressMaxRawValue) {
            m_airPressure = qQNaN();
            qInfo("BLE Sensor decodeAirPressure -> errorAirPressExceedRange");
        }
        else {
            m_airPressure = zigzagConvert(airPressunreRaw, 10.0);
            if (m_airPressure < 0) {
                m_airPressure = qQNaN();
                qInfo("BLE Sensor decodeAirPressure -> errorAirPressValueNegtive");
            }
        }
    }
    else
        m_airPressure = qQNaN();
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

void EfentoEnvironmentSensor::resetMeasureValues()
{
    m_temperaturInC = qQNaN();
    m_temperaturInF = qQNaN();
    m_humidity = qQNaN();
    m_airPressure = qQNaN();
}

bool EfentoEnvironmentSensor::isParameterPresent(const QByteArray &manufData, quint8 manufDataIndex, quint8 parameterID)
{
    return (manufData.count() > manufDataIndex + parameterDataBytes) && (manufData.at(manufDataIndex) == parameterID);
}
