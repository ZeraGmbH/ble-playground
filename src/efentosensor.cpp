#include "efentosensor.h"
#include <cstring>
#include <iostream>

EfentoSensor::EfentoSensor(const char* manufactureData)
{
    std::memcpy(m_manufactureData, manufactureData, sizeof (m_manufactureData));

    m_softwareVerMaj = 0;       // AHÖ  sinnig????
    m_softwareVerMin = 0;
    m_battLevelOK = false;
    m_encryptionEnable = false;
    m_storageErrorActive = false;

}

EfentoSensor::~EfentoSensor()
{
}

unsigned long EfentoSensor::getMeasurementCounter()
{
    unsigned long measureCntAct = m_manufactureData[4];
    measureCntAct *= 256;
    measureCntAct += m_manufactureData[5];
    measureCntAct *= 256;
    measureCntAct += m_manufactureData[6];
    measureCntAct *= 256;
    measureCntAct += m_manufactureData[7];
    return measureCntAct;
}

bool EfentoSensor::checkMsgValid()
{
    m_errorFlags = 0;
    if (m_manufactureData[0] != m_dataVersion)          // byte must be 0x02
        m_errorFlags |= m_errorDataVersion;

    m_softwareVerMaj = m_manufactureData[1];
    m_softwareVerMin = m_manufactureData[2];

    if (m_manufactureData[1] & 0x01)
        m_battLevelOK = true;
    if (m_manufactureData[1] & 0x02)
        m_encryptionEnable = true;
    if (m_manufactureData[1] & 0x04)
        m_storageErrorActive = true;

    if ((m_manufactureData[1] & 0xF0) > 0)              // higher nibble must be 0x00
        m_errorFlags |= m_errorReserveBits;

    if (m_manufactureData[10] != 0x00)                  // reserved and must be 0x00
        m_errorFlags |= m_errorReserveToZero;

    if (m_manufactureData[11] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
        m_errorFlags |= m_errorTypeSlot1;
    else
        m_sensortypeSlot1 = m_manufactureData[11];

    if (m_manufactureData[12] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
        m_errorFlags |= m_errorTypeSlot2;
    else
        m_sensortypeSlot2 = m_manufactureData[12];

    if (m_manufactureData[13] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
        m_errorFlags |= m_errorTypeSlot3;
    else
        m_sensortypeSlot3 = m_manufactureData[13];

    if (m_errorFlags > 0)
        return false;
    return true;
}

unsigned char EfentoSensor::getSensorType(unsigned char slot)
{
    unsigned char ret;
    switch (slot)
    {
    case m_sensorSlot1:
        ret = m_manufactureData[11];
        break;
    case m_sensorSlot2:
        ret = m_manufactureData[12];
        break;
    case m_sensorSlot3:
        ret = m_manufactureData[13];
        break;
    default:
        ret = m_sensorTypeNone;
    }
    return ret;
}

signed int EfentoSensor::getTemperatur(unsigned char slot)  // returns temp in 1/1000 °C e.g. -5°C = -500
{
     signed int temperature = -9999;

     unsigned int temperatureRaw;
     switch (slot) {
          case m_sensorSlot1:
               temperatureRaw = m_manufactureData[14];
               temperatureRaw <<= 8;
               temperatureRaw += m_manufactureData[15];
               break;
          case m_sensorSlot2:
               temperatureRaw = m_manufactureData[16];
               temperatureRaw <<= 8;
               temperatureRaw += m_manufactureData[17];
               break;
          case m_sensorSlot3:
               temperatureRaw = m_manufactureData[18];
               temperatureRaw <<= 8;
               temperatureRaw += m_manufactureData[19];
               break;
          default:
               m_errorFlags |= m_errorTempUnvalidSlot;
     }

     if (temperatureRaw <= m_TemperatureValueMax)
     {
          temperature = temperatureRaw;
          temperature -= m_temperatureOffeset;
     }
     else
     {
          if (temperatureRaw == m_errEfentoTempOutOfRange)
               m_errorFlags |= m_errorTempEfOutOfRange;
          else if ((temperatureRaw >= m_errEfentoTempSensorErrorMin) && (temperatureRaw <= m_errEfentoTempSensorErrorMax))
               m_errorFlags |= m_errorTempEfSensorError;
          else
               m_errorFlags |= m_errorTempOutOfRange;
     }
     return temperature;
}



unsigned char EfentoSensor::getHumidity(unsigned char slot)
{
    unsigned char humidity = 99;

    switch (slot) {
         case m_sensorSlot1:
              humidity = m_manufactureData[15];
              break;
         case m_sensorSlot2:
              humidity = m_manufactureData[17];
              break;
         case m_sensorSlot3:
              humidity = m_manufactureData[19];
              break;
         default:
              m_errorFlags |= m_errorHumidUnvalidSlot;
    }

    if ((humidity >= m_HumidityFutureUseMin) && (humidity <= m_HumidityFutureUseMax))
         m_errorFlags |= m_errorHumidEfFuturUse;

    else if (humidity == m_errEfentoHumidityOutOfRange)
          m_errorFlags |= m_errorHumidEfOutOfRange;

     else if (humidity == m_errEfentoHumiditySensorError)
          m_errorFlags |= m_errorHumidEfSensorError;

     else if (humidity == m_errEfentoHumidityNoMeasurement)
          m_errorFlags |= m_errorHumidEfNoMeasurment;

     return humidity;
}













bool EfentoSensor::isErrorAtive()
{
    if (m_errorFlags > 0)
        return true;
    return false;
}


