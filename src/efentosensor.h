#ifndef EFENTOSENSOR_H
#define EFENTOSENSOR_H

class EfentoSensor {
public:
    EfentoSensor(const char* manufactureData);
    ~EfentoSensor();

    unsigned long   getMeasurementCounter();
    bool            checkMsgValid();
    unsigned char   getSensorType(unsigned char slot);
    float           getTemperaturInC(unsigned char slot);
    float           getTemperaturInF();
    unsigned char   getHumidity(unsigned char slot);
    float           getAirPressure(unsigned char slot);
    bool            isErrorAtive();

    unsigned char  m_softwareVerMaj;
    unsigned char  m_softwareVerMin;
    unsigned long  m_errorFlags;


    /*
    bool           getBatteryLevelOK();
    bool           getEncryptionEnabled();
    bool           getStorageErrrorOccured();
    unsigned char  getSensorType(unsigned char slot);
    signed int     getTemperatur(unsigned char slot);  // returns temp in 1/1000 °C e.g. -5°C = -500
    unsigned char  getHumidity(unsigned char slot);
    void           clearGlobalError();
    unsigned int   getGlobalError();
    */
    // constants
    static constexpr unsigned char m_sensorSlotNone = 0;
    static constexpr unsigned char m_sensorSlot1 = 1;
    static constexpr unsigned char m_sensorSlot2 = 2;
    static constexpr unsigned char m_sensorSlot3 = 3;
    static constexpr unsigned char m_sensorTypeNone = 0;
    static constexpr unsigned char m_sensorTypeTemperatur = 1;
    static constexpr unsigned char m_sensorTypeHumidity = 2;
    static constexpr unsigned char m_sensorTypeAirPressure = 3;
    static constexpr unsigned char m_sensorTypeDiffPressure = 4;


    // Global Errors
    static constexpr unsigned long m_errorDataVersion = 1<<0;
    static constexpr unsigned long m_errorReserveBits = 1<<1;
    static constexpr unsigned long m_errorReserveToZero = 1<<2;
    static constexpr unsigned long m_errorTypeSlot1 = 1<<3;
    static constexpr unsigned long m_errorTypeSlot2 = 1<<4;
    static constexpr unsigned long m_errorTypeSlot3 = 1<<5;
    static constexpr unsigned long m_errorTempEfOutOfRange = 1<<6;          // Efento-ERR: 0xFFFD
    static constexpr unsigned long m_errorTempEfSensorError = 1<<7;         // Efento-ERR: 0xFED4 ... 0xFFFE
    static constexpr unsigned long m_errorTempOutOfRange = 1<<8;
    static constexpr unsigned long m_errorTempUnvalidSlot = 1<<9;
    static constexpr unsigned long m_errorHumidUnvalidSlot = 1<<10;
    static constexpr unsigned long m_errorHumidEfFuturUse = 1<<11;          // Efento-ERR: 0x65 ... 0xFC
    static constexpr unsigned long m_errorHumidEfOutOfRange = 1<<12;        // Efento-ERR: 0xFD
    static constexpr unsigned long m_errorHumidEfSensorError = 1<<13;       // Efento-ERR: 0xFE
    static constexpr unsigned long m_errorHumidEfNoMeasurment = 1<<14;      // Efento-ERR: 0xFF
    static constexpr unsigned long m_errorAirPressUnvalidSlot = 1<<15;
    static constexpr unsigned long m_errorAirPressEfFuturUse = 1<<16;       // Efento-ERR: 0xFF00 .. 0xFFFC
    static constexpr unsigned long m_errorAirPressEfOutOfRange = 1<<17;     // Efento-ERR: 0xFFFD
    static constexpr unsigned long m_errorAirPressEfSensorFail = 1<<18;     // Efento-ERR: 0xFFFE
    static constexpr unsigned long m_errorAirPressEfNoMeasurment = 1<<19;   // Efento-ERR: 0xFFFF

private:

    unsigned char  m_manufactureData[26];
    bool           m_battLevelOK;
    bool           m_encryptionEnable;
    bool           m_storageErrorActive;
    float          m_temperaturInC;
    float          m_temperaturInF;
    float          m_airPressure;

    unsigned int   m_efentoErrorState;
    unsigned char  m_sensorType;
    unsigned char  m_sensorStatus;
    bool           m_sensorBinaryFlag;
    unsigned long  m_measurementCount;
    unsigned int   m_measurePeriod;
    bool           m_measurePeriod_inSec;
    unsigned char  m_sensortypeSlot1;
    unsigned char  m_sensortypeSlot2;
    unsigned char  m_sensortypeSlot3;
    unsigned int   m_measurementSlot1;
    unsigned int   m_measurementSlot2;
    unsigned int   m_measurementSlot3;
    unsigned int   m_calibrationDate;
    unsigned int   m_crc16;
    unsigned char  m_humidity;

    // constants
    static constexpr unsigned char m_dataVersion = 0x02;

    static constexpr unsigned char m_HumidityValueMax = 0x64;
    static constexpr unsigned int m_TemperatureValueMax = 0x7530;
    static constexpr unsigned int m_temperatureOffeset = 15000;
    static constexpr unsigned char m_HumidityFutureUseMin = 0x65;
    static constexpr unsigned char m_HumidityFutureUseMax = 0xFC;
    static constexpr unsigned long m_airPrssureValueMax = 0xFEFF;

    // Efento Errors
    static constexpr unsigned int m_errEfentoTempOutOfRange = 0xFFFD;
    static constexpr unsigned int m_errEfentoTempSensorErrorMin = 0xFED4;
    static constexpr unsigned int m_errEfentoTempSensorErrorMax = 0xFFFE;
    static constexpr unsigned int m_errEfentoHumidityOutOfRange = 0xFD;
    static constexpr unsigned int m_errEfentoHumiditySensorError = 0xFE;
    static constexpr unsigned int m_errEfentoHumidityNoMeasurement = 0xFF;
    static constexpr unsigned int m_errEfentoHumidityFutureUseMin = 0x65;
    static constexpr unsigned int m_errEfentoHumidityFutureUseMax = 0xFC;
    static constexpr unsigned int m_errEfentoAirPressureFutureUseMin = 0xFF00;
    static constexpr unsigned int m_errEfentoAirPressureFutureUseMax = 0xFFFC;
    static constexpr unsigned int m_errEfentoAirPressureOutOfRange = 0xFFFD;
    static constexpr unsigned int m_errEfentoAirPressureSensorFail = 0xFFFE;
    static constexpr unsigned int m_errEfentoAirPressureNoMeasurement = 0xFFFF;



};


#endif // EFENTOSENSOR_H
