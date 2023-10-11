#ifndef EFENTOSENSOR_H
#define EFENTOSENSOR_H

class EfentoSensor {
public:
    EfentoSensor(const char* manufactureData);
    ~EfentoSensor();

    unsigned long   getMeasurementCounter();
    bool            checkMsgValid();
    unsigned char   getSensorType(unsigned char slot);
    signed int      getTemperatur(unsigned char slot);
    unsigned char   getHumidity(unsigned char slot);
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




 private:

    unsigned char  m_manufactureData[26];
    bool           m_battLevelOK;
    bool           m_encryptionEnable;
    bool           m_storageErrorActive;

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

    // Global Errors
    static constexpr unsigned long m_errorDataVersion = 0x0001;
    static constexpr unsigned long m_errorReserveBits = 0x0002;
    static constexpr unsigned long m_errorReserveToZero = 0x0004;
    static constexpr unsigned long m_errorTypeSlot1 = 0x0008;
    static constexpr unsigned long m_errorTypeSlot2 = 0x0010;
    static constexpr unsigned long m_errorTypeSlot3 = 0x0020;
    static constexpr unsigned long m_errorTempEfOutOfRange = 0x0040;  // Efento-ERR: 0xFFFD
    static constexpr unsigned long m_errorTempEfSensorError = 0x0080; // Efento-ERR: 0xFED4 ... 0xFFFE
    static constexpr unsigned long m_errorTempOutOfRange = 0x0100;
    static constexpr unsigned long m_errorTempUnvalidSlot = 0x0200;
    static constexpr unsigned long m_errorHumidUnvalidSlot = 0x0400;
    static constexpr unsigned long m_errorHumidEfFuturUse = 0x0800; // Efento-ERR: 0x65 ... 0xFC
    static constexpr unsigned long m_errorHumidEfOutOfRange = 0x1000;  // Efento-ERR: 0xFD
    static constexpr unsigned long m_errorHumidEfSensorError = 0x2000; // Efento-ERR: 0xFE
    static constexpr unsigned long m_errorHumidEfNoMeasurment = 0x4000; // Efento-ERR: 0xFF




    // Efento Errors
    static constexpr unsigned int m_errEfentoTempOutOfRange = 0xFFFD;
    static constexpr unsigned int m_errEfentoTempSensorErrorMin = 0xFED4;
    static constexpr unsigned int m_errEfentoTempSensorErrorMax = 0xFFFE;
    static constexpr unsigned int m_errEfentoHumidityOutOfRange = 0xFD;
    static constexpr unsigned int m_errEfentoHumiditySensorError = 0xFE;
    static constexpr unsigned int m_errEfentoHumidityNoMeasurement = 0xFF;
    static constexpr unsigned int m_errHumidityFutureUseMin = 0x65;
    static constexpr unsigned int m_errHumidityFutureUseMax = 0xFC;


};


#endif // EFENTOSENSOR_H
