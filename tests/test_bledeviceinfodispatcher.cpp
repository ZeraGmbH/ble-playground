#include "test_bledeviceinfodispatcher.h"
#include "bledeviceinfodispatcher.h"
#include "bluetoothdeviceinfodecodermock.h"
#include <QTest>

QTEST_MAIN(test_bledeviceinfodispatcher)

void test_bledeviceinfodispatcher::addRemoveIdentityCheck()
{
    BluetoothDeviceInfoDecoderMockPtr bleMockFoo = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMockFoo->setName("foo");
    BluetoothDeviceInfoDecoderMockPtr bleMockBar = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMockBar->setName("bar");

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId fooId = dispatcher.addBleDecoder(bleMockFoo);
    BleDispatcherId barId = dispatcher.addBleDecoder(bleMockBar);

    BluetoothDeviceInfoDecoderPtr bleMockFooDel = dispatcher.removeBleDecoder(fooId);
    BluetoothDeviceInfoDecoderPtr bleMockBarDel = dispatcher.removeBleDecoder(barId);

    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMockFooDel.get())->getName(), "foo");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMockBarDel.get())->getName(), "bar");
}

void test_bledeviceinfodispatcher::addOneCheckForNotification()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(bleMock);

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
    BleDispatcherId id = dispatcher.addBleDecoder(bleMockZero);

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
    dispatcher.addBleDecoder(bleMockZero);

    BleDispatcherId id;
    BluetoothDeviceInfoDecoderPtr decoder = dispatcher.removeBleDecoder(id);
    QCOMPARE(decoder, nullptr);
}

void test_bledeviceinfodispatcher::addTwoRemoveOneCheckForNotification()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock1 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock1->setName("1");
    BluetoothDeviceInfoDecoderMockPtr bleMock2 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock2->setName("2");

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(bleMock1);
    dispatcher.addBleDecoder(bleMock2);

    BluetoothDeviceInfoDecoderPtr bleMock1Del = dispatcher.removeBleDecoder(id);

    QBluetoothDeviceInfo bleInfo;
    dispatcher.onDeviceDiscovered(bleInfo);
    dispatcher.onDeviceDiscovered(bleInfo);

    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock1Del.get())->getName(), "1");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock1.get())->getDecodeCount(), 0);
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock2.get())->getDecodeCount(), 2);
}

void test_bledeviceinfodispatcher::addTwoRemoveOneTwice()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock1 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock1->setName("1");
    BluetoothDeviceInfoDecoderMockPtr bleMock2 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock2->setName("2");

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id = dispatcher.addBleDecoder(bleMock1);
    dispatcher.addBleDecoder(bleMock2);

    BluetoothDeviceInfoDecoderPtr bleMock1Del = dispatcher.removeBleDecoder(id);
    BluetoothDeviceInfoDecoderPtr bleMock1DelRepeat = dispatcher.removeBleDecoder(id);

    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock1Del.get())->getName(), "1");
    QCOMPARE(bleMock1DelRepeat, nullptr);
}

void test_bledeviceinfodispatcher::addTwoAndFind()
{
    BluetoothDeviceInfoDecoderMockPtr bleMock1 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock1->setName("1");
    BluetoothDeviceInfoDecoderMockPtr bleMock2 = std::make_shared<BluetoothDeviceInfoDecoderMock>();
    bleMock2->setName("2");

    BleDeviceInfoDispatcher dispatcher;
    BleDispatcherId id1 = dispatcher.addBleDecoder(bleMock1);
    BleDispatcherId id2 = dispatcher.addBleDecoder(bleMock2);

    BluetoothDeviceInfoDecoderPtr bleMock1Found = dispatcher.findBleDecoder(id1);
    BluetoothDeviceInfoDecoderPtr bleMock2Found = dispatcher.findBleDecoder(id2);
    BluetoothDeviceInfoDecoderPtr bleMock1FoundRepeat = dispatcher.findBleDecoder(id1);
    BluetoothDeviceInfoDecoderPtr bleMock2FoundRepeat = dispatcher.findBleDecoder(id2);

    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock1Found.get())->getName(), "1");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock2Found.get())->getName(), "2");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock1FoundRepeat.get())->getName(), "1");
    QCOMPARE(static_cast<BluetoothDeviceInfoDecoderMock*>(bleMock2FoundRepeat.get())->getName(), "2");
}
