#ifndef BLUETOOTHDEVICEINFODECODERMOCK_H
#define BLUETOOTHDEVICEINFODECODERMOCK_H

#include <bluetoothdeviceinfodecoder.h>

class BluetoothDeviceInfoDecoderMock : public BluetoothDeviceInfoDecoder
{
    Q_OBJECT
public:
    BluetoothDeviceInfoDecoderMock();
    void setName(QString name);
    QString getName();
    void decode(const QBluetoothDeviceInfo &info) override;
    int getDecodeCount();
private:
    QString m_name;
    int m_decodeCount = 0;
};

typedef std::unique_ptr<BluetoothDeviceInfoDecoderMock> BluetoothDeviceInfoDecoderMockPtr;

#endif // BLUETOOTHDEVICEINFODECODERMOCK_H
