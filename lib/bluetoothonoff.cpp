#include "bluetoothonoff.h"
#include <QCoreApplication>

// This class
// * controls Bluetooth on/off state
// * since there are rare cases on which kernel needs three attempts to upload
//   firmware which makes device avaliable after zera-classes' ble-module is
//   loaded
//   => If no device found it starts polling for Bluetooth device and gives up
//   after exceeding WaitToStartTimeoutMs
//
// We found no way yet to get tests close to target conditions (in all our
// build environments) so unfortunately manual testing is necessary.
//
// For the wait/poll there are 3 possible conditions
// 1. Bluetooth comes up early: This is the 99% case on target and just needs
//    checks in logs for 'Local Bluetooth device is up.'
// 2. Bluetooth comes up never or after timeout. This can be provoked by
//    systemctl disable bluetooth.service & reboot & check logs for
//    'Local Bluetooth device did not come up!'. This message should be spawned
//    ~WaitToStartTimeoutMs after ble-module was started.
// 3. Bluetooth comes up while polling - monkey work - sigh:
//    * compile kernel with CONFIG_BT_HCIUART_LL=m
//    * compile ble-playground with WaitToStartTimeoutMs = 600*1000 to have time
//    * on target:
//      * su
//      * echo "blacklist hci_uart" > /etc/modprobe.d/99-no-bt-early.conf
//      * reboot
//      * su
//      * modprobe hci_uart
//      * checks in logs for 'Local Bluetooth device is up.' it should be much lather than
//        ble-module start

BluetoothOnOff::BluetoothOnOff()
{
    if (!checkLocalBluetoothLoaded())
        startWaitForLocalDevice();
}

void BluetoothOnOff::switchOn()
{
    if (checkLocalBluetoothLoaded())
        m_bluetoothLocalDevice->powerOn();
    else
        setPendingRequest(ON);
}

void BluetoothOnOff::switchOff()
{
    if (checkLocalBluetoothLoaded())
        m_bluetoothLocalDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    else
        setPendingRequest(OFF);
}

bool BluetoothOnOff::isOn()
{
    return getCurrentState() == ON;
}

BluetoothOnOff::OnState BluetoothOnOff::getCurrentState()
{
    if (checkLocalBluetoothLoaded())
        return decodeHostMode(m_bluetoothLocalDevice->hostMode());
    return OFF;
}

bool BluetoothOnOff::checkLocalBluetoothLoaded()
{
    if (m_bluetoothLocalDevice)
        return true;
    const QList<QBluetoothHostInfo> devices = QBluetoothLocalDevice::allDevices();
    for (const QBluetoothHostInfo& device : devices)
        if (tryCreateNewLocalDevice(device.address()))
            return true;
    return false;
}

void BluetoothOnOff::startWaitForLocalDevice()
{
    qInfo("Local Bluetooth device is not yet up - starting poll...");
    connect(m_waitForLocalBluetoothReadyPollTimer.get(), &TimerTemplateQt::sigExpired,
            this, &BluetoothOnOff::onPollForStarted);
    m_waitForLocalBluetoothReadyPollTimer->start();
    m_pollElapsedTimer->start();
}

void BluetoothOnOff::onPollForStarted()
{
    if (checkLocalBluetoothLoaded()) {
        m_waitForLocalBluetoothReadyPollTimer->stop();
        handlePendingRequest();
    }
    else if (m_pollElapsedTimer->getElapsedMs() > WaitToStartTimeoutMs)
        giveUpWaitingForDevice();
}

void BluetoothOnOff::onHostModeChanged(QBluetoothLocalDevice::HostMode mode)
{
    OnState newState = decodeHostMode(mode);
    emit sigOnOff(newState == ON);
}

bool BluetoothOnOff::tryCreateNewLocalDevice(const QBluetoothAddress &blueAddr)
{
    m_bluetoothLocalDevice = std::make_unique<QBluetoothLocalDevice>(blueAddr);
    if (m_bluetoothLocalDevice->isValid()) {
        qInfo("Local Bluetooth device is up.");
        connect(m_bluetoothLocalDevice.get(), &QBluetoothLocalDevice::hostModeStateChanged,
                this, &BluetoothOnOff::onHostModeChanged);
        return true;
    };
    m_bluetoothLocalDevice.reset();
    return false;
}

void BluetoothOnOff::setPendingRequest(OnState requestedState)
{
    m_bOnOffRequestPending = true;
    m_requiredState = requestedState;
}

void BluetoothOnOff::handlePendingRequest()
{
    if (m_bOnOffRequestPending) {
        m_bOnOffRequestPending = false;
        if (m_requiredState == ON)
            switchOn();
        else
            switchOff();
    }
}

void BluetoothOnOff::giveUpWaitingForDevice()
{
    m_waitForLocalBluetoothReadyPollTimer->stop();
    const QString appName = QCoreApplication::applicationName().toLower();
    const QString errMsg = "Local Bluetooth device did not come up!";
    // * Production MUST support bluetooth
    //   => critical (we have test_errors_in_journal_final in autobuilder-dut-testsuite)
    // * Tests
    //   * can miss bluetooth (OE/CI/host without bluetooth)
    //   * test_modman_regression_all_sessions_scpi fails (which is good...)
    //   => warning
    if (appName.contains("test"))
        qWarning("%s", qPrintable(errMsg));
    else
        qCritical("%s", qPrintable(errMsg));
}

BluetoothOnOff::OnState BluetoothOnOff::decodeHostMode(QBluetoothLocalDevice::HostMode mode)
{
    return mode == QBluetoothLocalDevice::HostPoweredOff ? OFF : ON;
}
