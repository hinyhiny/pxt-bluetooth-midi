/* Copyright (c) 2014 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "MicroBit.h"
#include "pxt.h"
#include "BluetoothMIDIService.h"


/* FOR DAL
// MIDI characteristic MIDI Data I/O Characteristic
const uint8_t midiCharacteristicUuid[] = {
        0x77, 0x72, 0xe5, 0xdb, 0x38, 0x68, 0x41, 0x12, 
        0xa1, 0xa9, 0xf2, 0x66, 0x9d, 0x10, 0x6b, 0xf3
};

// MIDI service  MIDI Service
const uint8_t midiServiceUuid[] = {
        0x03, 0xb8, 0x0e, 0x5a, 0xed, 0xe8, 0x4b, 0x33, 
        0xa7, 0x51, 0x6c, 0xe3, 0x4e, 0xc4, 0xc7, 0x00
};
*/

//FOR CODAL
const uint8_t  BluetoothMIDIService::midiCharacteristicUuid[ 16] =
{
        0x77, 0x72, 0xe5, 0xdb, 0x38, 0x68, 0x41, 0x12, 
        0xa1, 0xa9, 0xf2, 0x66, 0x9d, 0x10, 0x6b, 0xf3
};

const uint8_t  BluetoothMIDIService::midiServiceUuid[ 16] =
{
        0x03, 0xb8, 0x0e, 0x5a, 0xed, 0xe8, 0x4b, 0x33, 
        0xa7, 0x51, 0x6c, 0xe3, 0x4e, 0xc4, 0xc7, 0x00
};


//CODAL
const uint16_t BluetoothMIDIService::serviceUUID               = 0xf5e6;
const uint16_t BluetoothMIDIService::charUUID[ mbbs_cIdxCOUNT] = { 0xf5e6 };

BluetoothMIDIService::BluetoothMIDIService(BLEDevice &_ble) {
    timestamp = 0;
    firstRead = true;

    /* FOR DAL
    // Initialise our characteristic values.
    memset(midiBuffer, 0, sizeof(midiBuffer));

    // setup GATT table(BLEの構造？)
    GattCharacteristic midiCharacteristic(midiCharacteristicUuid, midiBuffer, 0, sizeof(midiBuffer), 
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
        );
    GattCharacteristic *midiChars[] = {&midiCharacteristic};

    midiCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattService midiService(midiServiceUuid, midiChars, sizeof(midiChars) / sizeof(GattCharacteristic *));

    ble.addService(midiService);
   
    // retreive handles
    midiCharacteristicHandle = midiCharacteristic.getValueHandle();

     // initialize data
    ble.gattServer().onDataRead(this, &BluetoothMIDIService::onDataRead);

    ble.onDisconnection(this, &BluetoothMIDIService::onDisconnection);

    tick.start();
     */

    //FOR CODAL

     // Initialise our characteristic values.
    memset(&midiBuffer, 0, sizeof(midiBuffer));
    
    // Register the base UUID and create the service.
    RegisterBaseUUID( midiServiceUuid);
    CreateService( serviceUUID);

    RegisterBaseUUID( midiCharacteristicUuid);
    CreateCharacteristic( mbbs_cIdxMESSAGE, charUUID[ mbbs_cIdxMESSAGE],
                         midiBuffer,
                         sizeof(midiBuffer), sizeof(midiBuffer),
                         microbit_propREAD  | microbit_propNOTIFY);//CODAL MIDI
}

void BluetoothMIDIService::onDataRead(const GattReadCallbackParams* params) //OK
{
    //if (params->handle == midiCharacteristicHandle)//DAL
    /if (params->handle == mbbs_cIdxMESSAGE)//CODAL
    {
        if (firstRead) {
            // send empty payload upon first connect
            //ble.gattServer().notify(midiCharacteristicHandle, (uint8_t *)midiBuffer, 0);//DAL
            notifyChrValue(mbbs_cIdxMESSAGE, (uint8_t *)midiBuffer, 0);//CODAL
            firstRead = false;
        }
    }
}

void BluetoothMIDIService::onDisconnection(const Gap::DisconnectionCallbackParams_t* params) {//たぶんそのまま
    // clear read bit
    firstRead = true;
}

bool BluetoothMIDIService::connected() {//OK
    //return ble.getGapState().connected;//DAL
    return getConnected();//CODAL
}

void BluetoothMIDIService::sendMidiMessage(uint8_t data0) {//OK
    if (connected()) {//OK
        unsigned int ticks = tick.read_ms() & 0x1fff;
        midiBuffer[0] = 0x80 | ((ticks >> 7) & 0x3f);
        midiBuffer[1] = 0x80 | (ticks & 0x7f);
        midiBuffer[2] = data0;
        
        //ble.gattServer().notify(midiCharacteristicHandle, (uint8_t *)midiBuffer, 3);DAL
        notifyChrValue(mbbs_cIdxMESSAGE, (uint8_t *)midiBuffer, 3);//CODAL
    }
}

void BluetoothMIDIService::sendMidiMessage(uint8_t data0, uint8_t data1) {//OK
    if (connected()) {
        unsigned int ticks = tick.read_ms() & 0x1fff;
        midiBuffer[0] = 0x80 | ((ticks >> 7) & 0x3f);
        midiBuffer[1] = 0x80 | (ticks & 0x7f);
        midiBuffer[2] = data0;
        midiBuffer[3] = data1;
        
        //ble.gattServer().notify(midiCharacteristicHandle, (uint8_t *)midiBuffer, 4);DAL
        notifyChrValue(mbbs_cIdxMESSAGE, (uint8_t *)midiBuffer, 4);//CODAL
    }
}

void BluetoothMIDIService::sendMidiMessage(uint8_t data0, uint8_t data1, uint8_t data2) {//OK
    if (connected()) {
        unsigned int ticks = tick.read_ms() & 0x1fff;
        midiBuffer[0] = 0x80 | ((ticks >> 7) & 0x3f);
        midiBuffer[1] = 0x80 | (ticks & 0x7f);
        midiBuffer[2] = data0;
        midiBuffer[3] = data1;
        midiBuffer[4] = data2;
        
        //ble.gattServer().notify(midiCharacteristicHandle, (uint8_t *)midiBuffer, 5);DAL
        notifyChrValue(mbbs_cIdxMESSAGE, (uint8_t *)midiBuffer, 4);//CODAL
    }
}
