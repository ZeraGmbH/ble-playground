#ifndef EFENTOSENSOR_H
#define EFENTOSENSOR_H

class EfentoSensor {
public:
    EfentoSensor(const char* manufactureData);
    ~EfentoSensor();

    unsigned char   checkFrameType();
    void            decodeMeasureValues();
    void            decodeAdvertiseValues();
    unsigned long   getLastMeasureTS();

    unsigned char   getSensorType(unsigned char slot);
    float           getTemperaturInC();
    float           getTemperaturInF();
    unsigned char   getHumidity();
    float           getAirPressure();
    bool            isErrorActive();
    unsigned long   getActError();
    unsigned int    getActWarning();
    unsigned char   getFrameType();

    // constants FW 6.x.y
    static constexpr unsigned long m_TempMaxRawValue = 80000;
    static constexpr unsigned long m_HumidityMaxRawValue = 200;
    static constexpr unsigned int m_AirPressMaxRawValue = 40000;
    static constexpr unsigned char m_HumidityNotValid = 0xFF;
    static constexpr unsigned int m_AirPressNotValid = 0xFFFF;

    static constexpr unsigned char m_frameTypeNone = 0;
    static constexpr unsigned char m_frameTypeAdvertisement = 3;
    static constexpr unsigned char m_frameTypeScanResponse = 4;

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

private:

    unsigned char   m_manufactureData[26];

    unsigned char   m_softwareVerMaj;
    unsigned char   m_softwareVerMin;
    unsigned long   m_errorFlags;
    unsigned int    m_warningFlags;


    unsigned long   m_temperatureRaw;
    unsigned long   m_humidityRaw;
    unsigned long   m_airPressunreRaw;
    unsigned char   m_firmwareVersion[3];
    bool            m_battLevelOK;
    bool            m_encryptionEnable;
    unsigned long   m_measurementTs;
    unsigned int    m_measurementPeriodBase;
    float           m_temperaturInC;
    float           m_temperaturInF;
    unsigned char   m_humidity;
    float           m_airPressure;
    unsigned int    m_calibrationDate;
    unsigned int    m_crc16;

    // constants
    static constexpr unsigned char m_measurementPeriodBaseZera = 15;    // FW6
};


#endif // EFENTOSENSOR_H
