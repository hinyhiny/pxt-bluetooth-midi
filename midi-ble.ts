// midi-ble.ts  (完全版：MIDI Service を Advertise する)
namespace midi {
    const MIDI_SERVICE_UUID   = "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
    const MIDI_CHAR_UUID      = "7772E5DB-3868-4112-A1A9-F2669D106BF3"

    let connected = false

    //% block="MIDI 接続済み"
    export function isConnected(): boolean { return connected }

    //% block="MIDI BLE サービス開始"
    export function startService(): void {
        // --- 1. GATT テーブルに MIDI Service/Char を定義 ---
        let midiChar = new bluetooth.Characteric(
            MIDI_CHAR_UUID,
            bluetoothCharacteristicProperty.Notify |
            bluetoothCharacteristicProperty.Read,
            Buffer.create(5)
        )
        let midiService = new bluetooth.Service(
            MIDI_SERVICE_UUID,
            [midiChar]
        )
        bluetooth.setServices([midiService])   // ← これが無いと GarageBand は“MIDIデバイス”と認識しない

        // --- 2. 接続／切断コールバック ---
        bluetooth.onBluetoothConnected(() => { connected = true;  basic.showIcon(IconNames.Yes) })
        bluetooth.onBluetoothDisconnected(() => { connected = false; basic.showIcon(IconNames.No) })
    }

    //% block="MIDI note %note|on channel %channel|velocity %velocity"
    //% velocity.min=0 velocity.max=127
    //% channel.min=1 channel.max=16
    export function noteOn(note: number, channel: number, velocity: number): void {
        if (!connected) return
        const b0 = 0x80 | ((channel - 1) & 0x0F)   // header
        const b1 = 0x90 | ((channel - 1) & 0x0F)   // note-on
        const b2 = note & 0x7F
        const b3 = velocity & 0x7F
        const pkt = Buffer.create(5)
        pkt[0] = b0; pkt[1] = b1; pkt[2] = b2; pkt[3] = b3; pkt[4] = 0
        bluetooth.uartWriteBuffer(pkt)   // 実体は Notify でも可だが uartWriteBuffer で Notify 送出
    }
}