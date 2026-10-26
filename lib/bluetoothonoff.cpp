#include "bluetoothonoff.h"
#include <QCoreApplication>

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
