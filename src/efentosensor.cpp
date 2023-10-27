#include <QDebug>
#include "efentosensor.h"
#include <cstring>
#include <iostream>

// constants FW 6.x.y
static constexpr unsigned long m_TempMaxRawValue = 80000;
static constexpr unsigned long m_HumidityMaxRawValue = 200;
static constexpr unsigned int m_AirPressMaxRawValue = 40000;

static constexpr unsigned char m_sensorTypeTemperatur = 1;
static constexpr unsigned char m_sensorTypeHumidity = 2;
static constexpr unsigned char m_sensorTypeAirPressure = 3;
static constexpr unsigned char m_sensorTypeDiffPressure = 4;

// Global Errors and Warnings  FW 6.x.y
static constexpr unsigned long m_errorTempUnvalidSlot = 1<<0;
static constexpr unsigned long m_errorTempExceedRange = 1<<1;
static constexpr unsigned long m_errorTypeSlot1 = 1<<2;
static constexpr unsigned long m_errorTypeSlot2 = 1<<3;
static constexpr unsigned long m_errorTypeSlot3 = 1<<4;
static constexpr unsigned long m_errorHumidExceedRange = 1<<5;
static constexpr unsigned long m_errorAirPressExceedRange = 1<<6;
static constexpr unsigned long m_errorHumidValueNegtive = 1<<7;
static constexpr unsigned long m_errorAirPressValueNegtive = 1<<8;
static constexpr unsigned long m_errorFrameTypeNotValid = 1<<9;
static constexpr unsigned long m_warningLowBattery = 1<<0;
static constexpr unsigned long m_warningMeasuremPeriBaseFalse = 1<<1;
static constexpr unsigned long m_warningMeasuremPeriFactFalse = 1<<2;
static constexpr unsigned long m_warningNoCalibDateSet = 1<<3;


EfentoSensor::EfentoSensor(const char* manufactureData)
{
    std::memcpy(m_manufactureData, manufactureData, sizeof (m_manufactureData));

    m_softwareVerMaj = 0;
    m_softwareVerMin = 0;
    m_battLevelOK = false;
    m_encryptionEnable = false;
}

EfentoSensor::~EfentoSensor()
{
}

float EfentoSensor::zigzagConvert(unsigned long valueRaw, float divisor)
{
    bool isNegative = false;
    float val;
    if (valueRaw & 0x01)      // value is negativ
        isNegative = true;
    valueRaw >>= 1;
    val = valueRaw;
    if (isNegative)
        val *= -1;
    val /= divisor;
    return val;
}

unsigned char EfentoSensor::getFrameType()
{
    return m_manufactureData[0];
}

unsigned char EfentoSensor::checkFrameType()
{
    if (m_manufactureData[0] == m_frameTypeScanResponse)
        return m_frameTypeScanResponse;
    else if (m_manufactureData[0] == m_frameTypeAdvertisement)
        return m_frameTypeAdvertisement;

    m_errorFlags |= m_errorFrameTypeNotValid;
    return m_frameTypeNone;
}

void EfentoSensor::decodeMeasureValues()
{
    m_errorFlags = 0x00;
    bool isNegative = false;
    if (m_manufactureData[1] == m_sensorTypeTemperatur)
    {
        m_temperaturInC = -9999.99;
        m_temperaturInF = -9999.99;
        m_temperatureRaw = m_manufactureData[2];
        m_temperatureRaw <<= 8;
        m_temperatureRaw += m_manufactureData[3];
        m_temperatureRaw <<= 8;
        m_temperatureRaw += m_manufactureData[4];
        if (m_temperatureRaw > m_TempMaxRawValue)
            m_errorFlags |= m_errorTempExceedRange;
        else
        {
            m_temperaturInC = zigzagConvert(m_temperatureRaw, 10.0);
            m_temperaturInF = m_temperaturInC;
            m_temperaturInF *= 1.8;
            m_temperaturInF += 32.0;
        }
    }
    else
        m_errorFlags |= m_errorTypeSlot1;

    if (m_manufactureData[5] == m_sensorTypeHumidity)
    {
        unsigned long humidityRaw = m_manufactureData[6];
        humidityRaw <<= 8;
        humidityRaw += m_manufactureData[7];
        humidityRaw <<= 8;
        humidityRaw += m_manufactureData[8];
        if (humidityRaw > m_HumidityMaxRawValue)
            m_errorFlags |= m_errorHumidExceedRange;
        else
        {
            m_humidity = zigzagConvert(humidityRaw, 1.0);
            if (m_humidity < 0)
                m_errorFlags |= m_errorHumidValueNegtive;
        }
    }
    else
        m_errorFlags |= m_errorTypeSlot2;

    if (m_manufactureData[9] == m_sensorTypeAirPressure)
    {
        m_airPressunreRaw = m_manufactureData[10];
        m_airPressunreRaw <<= 8;
        m_airPressunreRaw += m_manufactureData[11];
        m_airPressunreRaw <<= 8;
        m_airPressunreRaw += m_manufactureData[12];
        if (m_airPressunreRaw > m_AirPressMaxRawValue)
            m_errorFlags |= m_errorAirPressExceedRange;
        else
        {
            m_airPressure = zigzagConvert(m_airPressunreRaw, 10.0);
            if (m_airPressunreRaw < 0)
                m_errorFlags |= m_errorAirPressValueNegtive;
        }
    }
    else
        m_errorFlags |= m_errorTypeSlot3;
}

void EfentoSensor::decodeAdvertiseValues()
{
    m_warningFlags = 0x00;
    unsigned char hlpB;
    m_firmwareVersion[0] = m_manufactureData[7] & 0xF8;
    m_firmwareVersion[0] >>= 3;
    m_firmwareVersion[1] = m_manufactureData[8];
    m_firmwareVersion[1] >>= 5;
    hlpB = m_manufactureData[7] & 0x07;
    hlpB <<= 3;
    m_firmwareVersion[1] |= hlpB;
    m_firmwareVersion[2] = m_manufactureData[8] & 0x1F;
    if (m_manufactureData[9] & 0x01)
        m_battLevelOK = true;
    else
    {
        m_battLevelOK = false;
        qInfo("Sensor battery low detect");
        m_warningFlags |= m_warningLowBattery;

    }
    if (m_manufactureData[9] & 0x04)
        m_encryptionEnable = true;
    else
        m_encryptionEnable = false;

    m_measurementTs = m_manufactureData[10];
    m_measurementTs <<= 8;
    m_measurementTs += m_manufactureData[11];
    m_measurementTs <<= 8;
    m_measurementTs += m_manufactureData[12];
    m_measurementTs <<= 8;
    m_measurementTs += m_manufactureData[13];

    m_measurementPeriodBase = m_manufactureData[14];
    m_measurementPeriodBase <<= 8;
    m_measurementPeriodBase += m_manufactureData[15];
    if (m_measurementPeriodBase != m_measurementPeriodBaseZera)
        m_warningFlags |= m_warningMeasuremPeriBaseFalse;
    if ((m_manufactureData[16] != 0x00) || (m_manufactureData[17] != 0x01))
        m_warningFlags |= m_warningMeasuremPeriFactFalse;

    m_calibrationDate = m_manufactureData[18];
    m_calibrationDate <<= 8;
    m_calibrationDate += m_manufactureData[19];
    if (m_calibrationDate == 0x00)
        m_warningFlags |= m_warningNoCalibDateSet;

    //std::cout << "FW-Mj: " << std::to_string(m_firmwareVersion[0]) << "  FW-Mi: " << std::to_string(m_firmwareVersion[1]) << "  FW-LTS: " << std::to_string(m_firmwareVersion[2]) << std::endl;
    //std::cout << "Measure TS: " << std::to_string(m_measurementTs) << std::endl;
}


float EfentoSensor::getTemperaturInC()
{
    return m_temperaturInC;
}

float EfentoSensor::getTemperaturInF()
{
    return m_temperaturInF;
}

float EfentoSensor::getHumidity()
{
    return m_humidity;
}

float EfentoSensor::getAirPressure()
{
    return m_airPressure;
}

unsigned long EfentoSensor::getLastMeasureTS()
{
    return m_measurementTs;
}

bool EfentoSensor::isErrorActive()
{
    if (m_errorFlags > 0)
        return true;
    return false;
}

unsigned long EfentoSensor::getActError()
{
    return m_errorFlags;
}

unsigned int EfentoSensor::getActWarning()
{
    return m_warningFlags;
}

