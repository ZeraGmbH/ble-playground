#include <QDebug>
#include "efentosensor.h"
#include <cstring>
#include <iostream>

EfentoSensor::EfentoSensor(const char* manufactureData)
{
    std::memcpy(m_manufactureData, manufactureData, sizeof (m_manufactureData));

    m_softwareVerMaj = 0;
    m_softwareVerMin = 0;
    m_battLevelOK = false;
    m_encryptionEnable = false;
    //m_storageErrorActive = false;
}

EfentoSensor::~EfentoSensor()
{
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
            unsigned long temperatureRaw = m_temperatureRaw;
            if (temperatureRaw & 0x01)      // convert from ZIGZAG to float
                isNegative = true;
            temperatureRaw >>= 1;
            m_temperaturInC = temperatureRaw;
            if (isNegative)
                m_temperaturInC *= -1;
            m_temperaturInC /= 10.0;
            m_temperaturInF = m_temperaturInC;
            m_temperaturInF *= 1.8;
            m_temperaturInF += 32.0;
        }
    }
    else
        m_errorFlags |= m_errorTypeSlot1;

    if (m_manufactureData[5] == m_sensorTypeHumidity)
    {
        m_humidityRaw = m_manufactureData[6];
        m_humidityRaw <<= 8;
        m_humidityRaw += m_manufactureData[7];
        m_humidityRaw <<= 8;
        m_humidityRaw += m_manufactureData[8];
        if (m_humidityRaw > m_HumidityMaxRawValue)
            m_errorFlags |= m_errorHumidExceedRange;
        else
        {
            unsigned long humidityRaw = m_humidityRaw;
            if (m_humidityRaw & 0x01)      // convert from ZIGZAG to unsigned char
            {
                m_errorFlags |= m_errorHumidValueNegtive;
                m_humidity = m_HumidityNotValid;
            }
            else
            {
                m_humidityRaw >>= 1;
                m_humidity = (unsigned char) m_humidityRaw;
            }
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
            unsigned int airPressureRaw = m_airPressunreRaw;
            if (airPressureRaw & 0x01)      // convert from ZIGZAG to float
            {
                m_errorFlags |= m_errorAirPressValueNegtive;
                m_airPressure = m_AirPressNotValid;
            }
            else
            {
                airPressureRaw >>= 1;
                m_airPressure = airPressureRaw;
                m_airPressure /= 10;
            }
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

unsigned char EfentoSensor::getHumidity()
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

