#ifndef EFENTOSENSOR_H
#define EFENTOSENSOR_H

class EfentoSensor {
public:
    EfentoSensor(const char* manufactureData);
    ~EfentoSensor();

    unsigned char   checkMsgType();
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

    unsigned char   m_softwareVerMaj;
    unsigned char   m_softwareVerMin;
    unsigned long   m_errorFlags;
    unsigned int    m_warningFlags;


    /*
    bool           getBatteryLevelOK();
    bool           getEncryptionEnabled();
    bool           getStorageErrrorOccured();
    void           clearGlobalError();
    unsigned int   getGlobalError();
    */
    // constants FW 6.x.y
    static constexpr unsigned long m_TempMaxRawValue = 80000;
    static constexpr unsigned long m_HumidityMaxRawValue = 200;
    static constexpr unsigned int m_AirPressMaxRawValue = 40000;
    static constexpr unsigned char m_HumidityNotValid = 0xFF;
    static constexpr unsigned int m_AirPressNotValid = 0xFFFF;

    static constexpr unsigned char m_frameTypeNone = 0;
    static constexpr unsigned char m_frameTypeAdvertisement = 1;
    static constexpr unsigned char m_frameTypeMeasurement = 2;

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


    static constexpr unsigned long m_warningLowBattery = 1<<0;
    static constexpr unsigned long m_warningMeasuremPeriBaseFalse = 1<<1;
    static constexpr unsigned long m_warningMeasuremPeriFactFalse = 1<<2;
    static constexpr unsigned long m_warningNoCalibDateSet = 1<<3;

    //FW5
    //static constexpr unsigned long m_errorTempEfOutOfRange = 1<<6;          // Efento-ERR: 0xFFFD
    //static constexpr unsigned long m_errorTempEfSensorError = 1<<7;         // Efento-ERR: 0xFED4 ... 0xFFFE
    //static constexpr unsigned long m_errorTempOutOfRange = 1<<8;
    //static constexpr unsigned long m_errorTempUnvalidSlot = 1<<9;
    //static constexpr unsigned long m_errorHumidUnvalidSlot = 1<<10;
    //static constexpr unsigned long m_errorHumidEfFuturUse = 1<<11;          // Efento-ERR: 0x65 ... 0xFC
    //static constexpr unsigned long m_errorHumidEfOutOfRange = 1<<12;        // Efento-ERR: 0xFD
    //static constexpr unsigned long m_errorHumidEfSensorError = 1<<13;       // Efento-ERR: 0xFE
    //static constexpr unsigned long m_errorHumidEfNoMeasurment = 1<<14;      // Efento-ERR: 0xFF
    //static constexpr unsigned long m_errorAirPressUnvalidSlot = 1<<15;
    //static constexpr unsigned long m_errorAirPressEfFuturUse = 1<<16;       // Efento-ERR: 0xFF00 .. 0xFFFC
    //static constexpr unsigned long m_errorAirPressEfOutOfRange = 1<<17;     // Efento-ERR: 0xFFFD
    //static constexpr unsigned long m_errorAirPressEfSensorFail = 1<<18;     // Efento-ERR: 0xFFFE
    //static constexpr unsigned long m_errorAirPressEfNoMeasurment = 1<<19;   // Efento-ERR: 0xFFFF
    //static constexpr unsigned long m_errorMeasurementPeriod = 1<<20;
    //static constexpr unsigned long m_warningLowBattery = 1<<0;
    //static constexpr unsigned long m_warningStorageError = 1<<1;

private:

    unsigned char   m_manufactureData[26];

    unsigned long   m_temperatureRaw;
    unsigned long   m_humidityRaw;
    unsigned long   m_airPressunreRaw;
    unsigned char   m_firmwareVersion[3];

    bool            m_battLevelOK;
    bool            m_encryptionEnable;
    unsigned long   m_measurementTs;
    unsigned int    m_measurementPeriodBase;

    //bool           m_storageErrorActive;
    float           m_temperaturInC;
    float           m_temperaturInF;
    unsigned char   m_humidity;
    float           m_airPressure;

    unsigned int   m_calibrationDate;
    unsigned int   m_crc16;


    // constants
    static constexpr unsigned char m_dataVersion = 0x02;
    //static constexpr unsigned char m_MeasurePeriodUnit = 0x80;      // seconds
    static constexpr unsigned char m_measurementPeriodBaseZera = 15;    // FW6
    static constexpr unsigned char m_ReserveToZero = 0;
    static constexpr unsigned char m_HumidityValueMax = 0x64;
    static constexpr unsigned int m_TemperatureValueMax = 0x7530;
    static constexpr unsigned int m_temperatureOffeset = 15000;
    static constexpr unsigned char m_HumidityFutureUseMin = 0x65;
    static constexpr unsigned char m_HumidityFutureUseMax = 0xFC;
    static constexpr unsigned long m_airPrssureValueMax = 0xFEFF;

// Efento Errors FW5
//    static constexpr unsigned long m_errorTypeSlot2 = 1<<4;
//    static constexpr unsigned long m_errorTypeSlot3 = 1<<5;
//    static constexpr unsigned int m_errEfentoTempOutOfRange = 0xFFFD;
//    static constexpr unsigned int m_errEfentoTempSensorErrorMin = 0xFED4;
//    static constexpr unsigned int m_errEfentoTempSensorErrorMax = 0xFFFE;
//    static constexpr unsigned int m_errEfentoHumidityOutOfRange = 0xFD;
//    static constexpr unsigned int m_errEfentoHumiditySensorError = 0xFE;
//    static constexpr unsigned int m_errEfentoHumidityNoMeasurement = 0xFF;
//    static constexpr unsigned int m_errEfentoHumidityFutureUseMin = 0x65;
//    static constexpr unsigned int m_errEfentoHumidityFutureUseMax = 0xFC;
//    static constexpr unsigned int m_errEfentoAirPressureFutureUseMin = 0xFF00;
//    static constexpr unsigned int m_errEfentoAirPressureFutureUseMax = 0xFFFC;
//    static constexpr unsigned int m_errEfentoAirPressureOutOfRange = 0xFFFD;
//    static constexpr unsigned int m_errEfentoAirPressureSensorFail = 0xFFFE;
//    static constexpr unsigned int m_errEfentoAirPressureNoMeasurement = 0xFFFF;

};


#endif // EFENTOSENSOR_H
