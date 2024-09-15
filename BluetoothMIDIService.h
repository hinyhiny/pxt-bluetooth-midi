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

#ifndef __BLEMIDI_H__
#define __BLEMIDI_H__

// for V2 - codal
#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"

/** 
 * A class to communicate a BLE MIDI device
 */
class BluetoothMIDIService {
public:
    /**
     * Constructor（共通？）
     */
    BluetoothMIDIService(BLEDevice *device);


    /**
     * Check if a BLE MIDI device is connected
     *
     * @returns true if a midi device is connected
     */
    bool connected();

    //以下は多分CODALも同じ？
    void sendMidiMessage(uint8_t data0);
    void sendMidiMessage(uint8_t data0, uint8_t data1);
    void sendMidiMessage(uint8_t data0, uint8_t data1, uint8_t data2);

private:    
    void onDataRead(const GattReadCallbackParams* params);
    void onDisconnection(const Gap::DisconnectionCallbackParams_t* params);

    uint16_t timestamp;
    uint8_t midiBuffer[5];
    bool firstRead;

    // Bluetooth stack we're running on.
    BLEDevice &ble;

    // Handles to access each characteristic when they are held by Soft Device.
    //GattAttribute::Handle_t midiCharacteristicHandle;    
    //Timer tick;

    // Data for each characteristic when they are held by Soft Device.
    MicroBitBLEChar      chars[ mbbs_cIdxCOUNT];

    //以下不要かも？
    public:
    
    int              characteristicCount()          { return mbbs_cIdxCOUNT; };
    MicroBitBLEChar *characteristicPtr( int idx)    { return &chars[ idx]; };



};

#endif /* __BLEMIDI_H__ */
