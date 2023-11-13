#ifndef BLUETOOTHONOFF_H
#define BLUETOOTHONOFF_H

#include <QObject>
#include <QBluetoothLocalDevice>

class BluetoothOnOff : public QObject
{
    Q_OBJECT
public:
    BluetoothOnOff();
    void switchOn(QBluetoothLocalDevice::HostMode hostMode = QBluetoothLocalDevice::HostConnectable);
    void switchOff();
signals:
    void sigOnOff(bool on);

private slots:
    void onHostModeChanged(QBluetoothLocalDevice::HostMode state);
private:
    QBluetoothLocalDevice m_bluetoothDevice;
    bool m_on;
};

#endif // BLUETOOTHONOFF_H
