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
    //m_storageErrorActive = false;

}

EfentoSensor::~EfentoSensor()
{
}


unsigned char EfentoSensor::getFrameType()   // FW 6.x.y
{
    return m_manufactureData[0];
}



unsigned char EfentoSensor::checkMsgType()   // FW 6.x.y
{
    if (m_manufactureData[0] == 0x04)          // byte must be 0x04
        return m_frameTypeMeasurement;
    else if (m_manufactureData[0] == 0x03)
        return m_frameTypeAdvertisement;

    return m_frameTypeNone;
}

void EfentoSensor::decodeMeasureValues()    // FW6
{
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
            m_temperaturInC /= 10;
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


void EfentoSensor::decodeAdvertiseValues()  // for FW 6.x.y
{
    unsigned char hlpB;
    m_firmwareVersion[0] = m_manufactureData[7] & 0xF8;     // todo check if correct
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

    m_calibrationDate = m_manufactureData[16];
    m_calibrationDate <<= 8;
    m_calibrationDate += m_manufactureData[17];
    if (m_calibrationDate == 0x00)
        m_warningFlags |= m_warningNoCalibDateSet;

    //std::cout << "FW-Mj: " << std::to_string(m_firmwareVersion[0]) << "  FW-Mi: " << std::to_string(m_firmwareVersion[1]) << "  FW-LTS: " << std::to_string(m_firmwareVersion[2]) << std::endl;
    //std::cout << "Measure TS: " << std::to_string(m_measurementTs) << std::endl;


    //std::cout "Measure TS: " << std::to_string(m_measurementTs) << std::endl;
}



float EfentoSensor::getTemperaturInC()    // for FW 6.x.y
{
    return m_temperaturInC;
}

float EfentoSensor::getTemperaturInF()    // for FW 6.x.y
{
    return m_temperaturInF;
}

unsigned char EfentoSensor::getHumidity()
{
    return m_humidity;
}


float EfentoSensor::getAirPressure()    // for FW 6.x.y
{
    return m_airPressure;
}


unsigned long EfentoSensor::getLastMeasureTS()
{
    return m_measurementTs;
}





//bool EfentoSensor::checkMsgValid()  // FW 5.x
//{
//    m_errorFlags = 0;
//    m_warningFlags = 0;

//    if (m_manufactureData[0] != m_dataVersion)          // byte must be 0x02
//        m_errorFlags |= m_errorDataVersion;
//    m_softwareVerMaj = m_manufactureData[1];
//    m_softwareVerMin = m_manufactureData[2];

//    if (m_manufactureData[1] & 0x01)
//    {
//        m_battLevelOK = true;
//        m_warningFlags |= m_warningLowBattery;
//    }
//    if (m_manufactureData[1] & 0x02)
//        m_encryptionEnable = true;
//    if (m_manufactureData[1] & 0x04)
//    {
//        m_storageErrorActive = true;
//        m_warningFlags |= m_warningStorageError;
//    }

//    if ((m_manufactureData[1] & 0xF0) > 0)              // higher nibble must be 0x00
//        m_errorFlags |= m_errorReserveBits;

//    if ((m_manufactureData[8] != m_MeasurePeriodUnit) || (m_manufactureData[9] != m_MeasurePeriodValue))
//        m_errorFlags |= m_errorMeasurementPeriod;

//    if (m_manufactureData[10] != m_ReserveToZero)       // reserved and must be 0x00
//        m_errorFlags |= m_errorReserveToZero;

//    if (m_manufactureData[11] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
//        m_errorFlags |= m_errorTypeSlot1;
//    else
//        m_sensortypeSlot1 = m_manufactureData[11];

//    if (m_manufactureData[12] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
//        m_errorFlags |= m_errorTypeSlot2;
//    else
//        m_sensortypeSlot2 = m_manufactureData[12];

//    if (m_manufactureData[13] > m_sensorTypeDiffPressure) // only a few sensortypes or non are valid
//        m_errorFlags |= m_errorTypeSlot3;
//    else
//        m_sensortypeSlot3 = m_manufactureData[13];

//    if (m_errorFlags > 0)
//        return false;

//    return true;
//}

//unsigned char EfentoSensor::getSensorType(unsigned char slot)
//{
//    unsigned char ret;
//    switch (slot)
//    {
//    case m_sensorSlot1:
//        ret = m_manufactureData[11];
//        break;
//    case m_sensorSlot2:
//        ret = m_manufactureData[12];
//        break;
//    case m_sensorSlot3:
//        ret = m_manufactureData[13];
//        break;
//    default:
//        ret = m_sensorTypeNone;
//    }
//    return ret;
//}

//unsigned long EfentoSensor::getMeasurementCounter()
//{
//    unsigned long measureCntAct = m_manufactureData[4];
//    measureCntAct *= 256;
//    measureCntAct += m_manufactureData[5];
//    measureCntAct *= 256;
//    measureCntAct += m_manufactureData[6];
//    measureCntAct *= 256;
//    measureCntAct += m_manufactureData[7];
//    return measureCntAct;
//}




//float EfentoSensor::getTemperaturInC(unsigned char slot)   for FW 5.x
//{
//     m_temperaturInC = -9999.99;

//     unsigned int temperatureRaw;
//     switch (slot) {
//          case m_sensorSlot1:
//               temperatureRaw = m_manufactureData[14];
//               temperatureRaw <<= 8;
//               temperatureRaw += m_manufactureData[15];
//               break;
//          case m_sensorSlot2:
//               temperatureRaw = m_manufactureData[16];
//               temperatureRaw <<= 8;
//               temperatureRaw += m_manufactureData[17];
//               break;
//          case m_sensorSlot3:
//               temperatureRaw = m_manufactureData[18];
//               temperatureRaw <<= 8;
//               temperatureRaw += m_manufactureData[19];
//               break;
//          default:
//               m_errorFlags |= m_errorTempUnvalidSlot;
//     }

//     if (temperatureRaw <= m_TemperatureValueMax)
//     {
//          m_temperaturInC = temperatureRaw;
//          m_temperaturInC -= m_temperatureOffeset;
//          m_temperaturInC /= 100;
//     }
//     else
//     {
//          if (temperatureRaw == m_errEfentoTempOutOfRange)
//               m_errorFlags |= m_errorTempEfOutOfRange;
//          else if ((temperatureRaw >= m_errEfentoTempSensorErrorMin) && (temperatureRaw <= m_errEfentoTempSensorErrorMax))
//               m_errorFlags |= m_errorTempEfSensorError;
//          else
//               m_errorFlags |= m_errorTempOutOfRange;
//     }

//     return m_temperaturInC;
//}




//float EfentoSensor::getTemperaturInF()
//{
//    if (m_temperaturInC == -9999.99)
//        m_temperaturInF = -9999.99;
//    {
//        m_temperaturInF = m_temperaturInC;
//        m_temperaturInF *= 1.8;
//        m_temperaturInF += 32.0;
//    }
//    return m_temperaturInF;
//}


//unsigned char EfentoSensor::getHumidity(unsigned char slot)
//{
//    unsigned char humidity = 99;

//    switch (slot) {
//         case m_sensorSlot1:
//              humidity = m_manufactureData[15];
//              break;
//         case m_sensorSlot2:
//              humidity = m_manufactureData[17];
//              break;
//         case m_sensorSlot3:
//              humidity = m_manufactureData[19];
//              break;
//         default:
//              m_errorFlags |= m_errorHumidUnvalidSlot;
//    }

//    if ((humidity >= m_errEfentoHumidityFutureUseMin) && (humidity <= m_errEfentoHumidityFutureUseMax))
//         m_errorFlags |= m_errorHumidEfFuturUse;

//    else if (humidity == m_errEfentoHumidityOutOfRange)
//          m_errorFlags |= m_errorHumidEfOutOfRange;

//     else if (humidity == m_errEfentoHumiditySensorError)
//          m_errorFlags |= m_errorHumidEfSensorError;

//     else if (humidity == m_errEfentoHumidityNoMeasurement)
//          m_errorFlags |= m_errorHumidEfNoMeasurment;

//     return humidity;
//}


//float EfentoSensor::getAirPressure(unsigned char slot)
//{
//    unsigned long airPressureRaw;

//    m_airPressure = -9999.99;

//    unsigned int temperatureRaw;
//    switch (slot) {
//         case m_sensorSlot1:
//              airPressureRaw = m_manufactureData[14];
//              airPressureRaw <<= 8;
//              airPressureRaw += m_manufactureData[15];
//              break;
//         case m_sensorSlot2:
//              airPressureRaw = m_manufactureData[16];
//              airPressureRaw <<= 8;
//              airPressureRaw += m_manufactureData[17];
//              break;
//         case m_sensorSlot3:
//              airPressureRaw = m_manufactureData[18];
//              airPressureRaw <<= 8;
//              airPressureRaw += m_manufactureData[19];
//              break;
//         default:
//              m_errorFlags |= m_errorAirPressUnvalidSlot;
//    }

//    if (airPressureRaw <= m_airPrssureValueMax)
//    {
//        m_airPressure = airPressureRaw;
//        m_airPressure /= 10;
//    }
//    else
//    {
//        if ((airPressureRaw >= m_errEfentoAirPressureFutureUseMin) && (airPressureRaw <= m_errEfentoAirPressureFutureUseMax))
//             m_errorFlags |= m_errorAirPressEfFuturUse;

//         else if (airPressureRaw == m_errEfentoAirPressureOutOfRange)
//              m_errorFlags |= m_errorAirPressEfOutOfRange;

//         else if (airPressureRaw == m_errEfentoAirPressureSensorFail)
//              m_errorFlags |= m_errorAirPressEfSensorFail;

//         else if (airPressureRaw == m_errEfentoAirPressureNoMeasurement)
//              m_errorFlags |= m_errorAirPressEfNoMeasurment;
//    }
//    return m_temperaturInC;
//}

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

