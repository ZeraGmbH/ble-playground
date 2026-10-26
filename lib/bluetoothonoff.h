#ifndef BLUETOOTHONOFF_H
#define BLUETOOTHONOFF_H

#include <timerfactoryqt.h>
#include <QObject>
#include <QBluetoothLocalDevice>


class BluetoothOnOff : public QObject
{
    Q_OBJECT
public:
    BluetoothOnOff();

    static constexpr int WaitToStartPollTimeMs = 500;
    static constexpr int WaitToStartTimeoutMs = 30*1000;

    void switchOn();
    void switchOff();
    bool isOn();
signals:
    void sigOnOff(bool on);

private slots:
    void onHostModeChanged(QBluetoothLocalDevice::HostMode mode);
    void onPollForStarted();
private:
    enum OnState {
        OFF,
        ON
    };
    bool checkLocalBluetoothLoaded();
    void startWaitForLocalDevice();
    bool tryCreateNewLocalDevice(const QBluetoothAddress &blueAddr);
    OnState decodeHostMode(QBluetoothLocalDevice::HostMode mode);
    OnState getCurrentState();
    void setPendingRequest(OnState requestedState);
    void handlePendingRequest();
    void giveUpWaitingForDevice();

    std::unique_ptr<QBluetoothLocalDevice> m_bluetoothLocalDevice;

    TimerTemplateQtPtr m_waitForLocalBluetoothReadyPollTimer = TimerFactoryQt::createPeriodic(WaitToStartPollTimeMs);
    TimerElapsedTemplateQtPtr m_pollElapsedTimer = TimerFactoryQt::createElapsed();

    bool m_bOnOffRequestPending = false;
    OnState m_requiredState = OFF;
};

#endif // BLUETOOTHONOFF_H
