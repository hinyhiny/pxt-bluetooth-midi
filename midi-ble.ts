/**
 * Bluetooth MIDI 拡張
 * author: @kimi_k2
 */

namespace midi {
    const MIDI_SERVICE = "03B80E5A-EDE8-4B33-A751-6CE34EC4C700";
    const MIDI_CHAR   = "7772E5DB-3868-4112-A1A9-F2669D106BF3";

    let connected = false;

    /**
     * GarageBand などと BLE MIDI 接続済みか
     */
    //% block="MIDI 接続済み"
    //% weight=90
    export function isConnected(): boolean {
        return connected;
    }

    /**
     * BLE MIDI サービスを開始
     */
    //% block="MIDI BLE サービス開始"
    //% weight=80
    export function startService(): void {
        bluetooth.startUartService();   // 初期化
        bluetooth.onBluetoothConnected(() => { connected = true;  basic.showIcon(IconNames.Yes); });
        bluetooth.onBluetoothDisconnected(() => { connected = false; basic.showIcon(IconNames.No); });
    }

    /**
     * Note ON 送信 (velocity 0 で Note OFF 扱い)
     */
    //% block="MIDI note %note|on channel %channel|velocity %velocity"
    //% velocity.min=0 velocity.max=127
    //% channel.min=1 channel.max=16
    //% weight=70
    export function noteOn(note: number, channel: number, velocity: number): void {
        if (!connected) return;
        // BLE MIDI 仕様: 先頭0x80付き4バイト
        const b0 = 0x80 | ((channel - 1) & 0x0F);
        const b1 = 0x90 | ((channel - 1) & 0x0F);
        const b2 = note & 0x7F;
        const b3 = velocity & 0x7F;
        const pkt = Buffer.create(5);
        pkt[0] = b0;
        pkt[1] = b1;
        pkt[2] = b2;
        pkt[3] = b3;
        pkt[4] = 0x00;  // タイムスタンプダミー
        bluetooth.uartWriteBuffer(pkt);
    }
}