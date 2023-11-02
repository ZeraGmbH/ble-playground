#ifndef EFENTOSENSOR_H
#define EFENTOSENSOR_H

class EfentoSensor
{
public:
    EfentoSensor(const char* manufactureData);
    ~EfentoSensor();
    float getTemperaturInC();
    float getTemperaturInF();
    float getHumidity();
    float getAirPressure();

    unsigned char checkFrameType();

    void decodeMeasureValues();
    void decodeAdvertiseValues();

    unsigned long getLastMeasureTS();
    unsigned char getSensorType(unsigned char slot);
    bool isErrorActive();
    unsigned long getActError();
    unsigned int getActWarning();
    unsigned char getFrameType();

    static constexpr unsigned char m_frameTypeAdvertisement = 3;
    static constexpr unsigned char m_frameTypeScanResponse = 4;

private:
    float zigzagConvert(unsigned long valueRaw, float divisor);
    static constexpr unsigned char m_frameTypeNone = 0;

    unsigned char m_manufactureData[26];
    unsigned char m_softwareVerMaj;
    unsigned char m_softwareVerMin;
    unsigned long m_errorFlags;
    unsigned int m_warningFlags;
    unsigned char m_firmwareVersion[3];
    bool m_battLevelOK;
    bool m_encryptionEnable;
    unsigned long m_measurementTs;
    unsigned int m_measurementPeriodBase;
    float m_temperaturInC;
    float m_temperaturInF;
    float m_humidity;
    float m_airPressure;
    unsigned int m_calibrationDate;
};


#endif // EFENTOSENSOR_H