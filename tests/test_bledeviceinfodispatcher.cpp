#include "test_bledeviceinfodispatcher.h"
#include "bledeviceinfodispatcher.h"
#include "bluetoothdeviceinfodecodermock.h"
#include <QTest>

QTEST_MAIN(test_bledeviceinfodispatcher)

void test_bledeviceinfodispatcher::addRemoveIdentityCheck()
{
    BluetoothDeviceInfoDecoderMockPtr bleMockFoo = std::make_unique<BluetoothDeviceInfoDecoderMock>();
    bleMockFoo->setName("foo");
    BluetoothDeviceInfoDecoderMockPtr bleMockBar = std::make_unique<BluetoothDeviceInfoDecoderMock>();
    bleMockBar->setName("bar");

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId fooId = dispatcher.addBleDecoder(std::move(bleMockFoo));
    BleDispatcherId barId = dispatcher.addBleDecoder(std::move(bleMockBar));

    BluetoothDeviceInfoDecoderPtr bleMockFooDel = dispatcher.removeBleDecoder(fooId);
    BluetoothDeviceInfoDecoderPtr bleMockBarDel = dispatcher.removeBleDecoder(barId);

    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMockFooDel.get())->getName(), "foo");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMockBarDel.get())->getName(), "bar");
}

void test_bledeviceinfodispatcher::addOneCheckForNotification()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock = std::make_unique<BluetoothDeviceInfoDecoderMock>();
    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(std::move(bleMock));

    QBluetoothDeviceInfo bleInfo;
    dispatcher.onDeviceDiscovered(bleInfo);
    dispatcher.onDeviceDiscovered(bleInfo);

    BluetoothDeviceInfoDecoderPtr bleMockDel = dispatcher.removeBleDecoder(id);
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMockDel.get())->getDecodeCount(), 2);
}

void test_bledeviceinfodispatcher::addZeroPointer()
{
    BluetoothDeviceInfoDecoderMockPtr bleMockZero;
    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(std::move(bleMockZero));

    QBluetoothDeviceInfo bleInfo;
    dispatcher.onDeviceDiscovered(bleInfo);

    BluetoothDeviceInfoDecoderPtr decoder = dispatcher.removeBleDecoder(id);
    QCOMPARE(decoder, nullptr);
}

void test_bledeviceinfodispatcher::removeOnEmptyDispatcher()
{
    BleDeviceInfoDispatcher dispatcher;
    BluetoothDeviceInfoDecoderPtr decoder = dispatcher.removeBleDecoder(BleDispatcherId());
    QBluetoothDeviceInfo bleInfo;
    dispatcher.onDeviceDiscovered(bleInfo);
    QCOMPARE(decoder, nullptr);
}

void test_bledeviceinfodispatcher::removeWithInvalidId()
{
    BluetoothDeviceInfoDecoderMockPtr bleMockZero;
    BleDeviceInfoDispatcher dispatcher;
    dispatcher.addBleDecoder(std::move(bleMockZero));

    BleDispatcherId id;
    BluetoothDeviceInfoDecoderPtr decoder = dispatcher.removeBleDecoder(id);
    QCOMPARE(decoder, nullptr);
}

void test_bledeviceinfodispatcher::addTwoRemoveOneCheckForNotification()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock1 = std::make_unique<BluetoothDeviceInfoDecoderMock>();
    BluetoothDeviceInfoDecoderMockPtr bleMock2 = std::make_unique<BluetoothDeviceInfoDecoderMock>();

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(std::move(bleMock1));
    dispatcher.addBleDecoder(std::move(bleMock2));

    BluetoothDeviceInfoDecoderPtr bleMock1Del = dispatcher.removeBleDecoder(id);

    QBluetoothDeviceInfo bleInfo;
    dispatcher.onDeviceDiscovered(bleInfo);
    dispatcher.onDeviceDiscovered(bleInfo);
}

