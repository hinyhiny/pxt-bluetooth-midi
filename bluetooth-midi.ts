/**
 * Extension for Bluetooth MIDI communication with GarageBand on iPad
 */
//% weight=100 color=#0078D7 icon="\uf001"
namespace bluetoothMIDI {

    // Bluetooth MIDI Service UUID and Characteristic UUID
    const MIDI_SERVICE_UUID = "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
    const MIDI_CHAR_UUID = "7772E5DB-3868-4112-A1A9-F2669D106BF3"

    let midiConnected = false
    let midiCharacteristic: number = 0

    /**
     * Initialize Bluetooth MIDI and start advertising
     */
    //% block="Bluetooth MIDI を初期化"
    //% weight=100
    export function init(): void {
        bluetooth.startUartService();
        bluetooth.setTransmitPower(7); // Maximum power for better connectivity
        
        // Set device name for easy identification
        bluetooth.setName("micro:bit-MIDI")
        
        midiConnected = false
    }

    /**
     * Check if MIDI is connected to a device
     */
    //% block="MIDI が接続されている"
    //% weight=90
    export function isConnected(): boolean {
        return bluetooth.uartIsConnected()
    }

    /**
     * Send a MIDI note on message
     * @param note Note number (0-127)
     * @param velocity Velocity (0-127)
     * @param channel MIDI channel (1-16)
     */
    //% block="ノートオン note %note|velocity %velocity|チャンネル %channel"
    //% note.min=0 note.max=127 note.fieldOptions.precision=0
    //% velocity.min=0 velocity.max=127 velocity.fieldOptions.precision=0
    //% channel.min=1 channel.max=16 channel.fieldOptions.precision=0
    //% weight=80
    export function noteOn(note: number, velocity: number, channel: number): void {
        if (bluetooth.uartIsConnected()) {
            // MIDI message: 0x9n where n is channel-1, followed by note and velocity
            const status = 0x90 | (channel - 1)
            bluetooth.uartWriteValue(status, note)
            bluetooth.uartWriteValue(0, velocity)
        }
    }

    /**
     * Send a MIDI note off message
     * @param note Note number (0-127)
     * @param velocity Velocity (0-127)
     * @param channel MIDI channel (1-16)
     */
    //% block="ノートオフ note %note|velocity %velocity|チャンネル %channel"
    //% note.min=0 note.max=127 note.fieldOptions.precision=0
    //% velocity.min=0 velocity.max=127 velocity.fieldOptions.precision=0
    //% channel.min=1 channel.max=16 channel.fieldOptions.precision=0
    //% weight=70
    export function noteOff(note: number, velocity: number, channel: number): void {
        if (bluetooth.uartIsConnected()) {
            // MIDI message: 0x8n where n is channel-1, followed by note and velocity
            const status = 0x80 | (channel - 1)
            bluetooth.uartWriteValue(status, note)
            bluetooth.uartWriteValue(0, velocity)
        }
    }

    /**
     * Send a MIDI control change message
     * @param controller Controller number (0-127)
     * @param value Value (0-127)
     * @param channel MIDI channel (1-16)
     */
    //% block="コントロールチェンジ controller %controller|value %value|チャンネル %channel"
    //% controller.min=0 controller.max=127 controller.fieldOptions.precision=0
    //% value.min=0 value.max=127 value.fieldOptions.precision=0
    //% channel.min=1 channel.max=16 channel.fieldOptions.precision=0
    //% weight=60
    export function controlChange(controller: number, value: number, channel: number): void {
        if (bluetooth.uartIsConnected()) {
            // MIDI message: 0xBn where n is channel-1
            const status = 0xB0 | (channel - 1)
            bluetooth.uartWriteValue(status, controller)
            bluetooth.uartWriteValue(0, value)
        }
    }

    /**
     * Send a MIDI program change (instrument change)
     * @param program Program number (0-127)
     * @param channel MIDI channel (1-16)
     */
    //% block="プログラムチェンジ program %program|チャンネル %channel"
    //% program.min=0 program.max=127 program.fieldOptions.precision=0
    //% channel.min=1 channel.max=16 channel.fieldOptions.precision=0
    //% weight=50
    export function programChange(program: number, channel: number): void {
        if (bluetooth.uartIsConnected()) {
            // MIDI message: 0xCn where n is channel-1
            const status = 0xC0 | (channel - 1)
            bluetooth.uartWriteValue(status, program)
        }
    }

    /**
     * Play a note with specified duration
     * @param note Note number (0-127)
     * @param duration Duration in beats
     */
    //% block="ノートを鳴らす note %note|長さ %duration|拍子"
    //% note.min=0 note.max=127 note.fieldOptions.precision=0
    //% duration.min=0 duration.max=10 duration.fieldOptions.precision=1
    //% weight=40
    export function playNote(note: number, duration: number): void {
        noteOn(note, 100, 1)
        basic.pause(duration * 500)
        noteOff(note, 0, 1)
    }

    /**
     * Send raw MIDI bytes (advanced)
     * @param data Array of MIDI bytes
     */
    //% block="MIDIデータを送信 %data"
    //% weight=30
    export function sendRaw(data: number[]): void {
        if (bluetooth.uartIsConnected()) {
            for (let i = 0; i < data.length; i++) {
                bluetooth.uartWriteValue(0, data[i])
            }
        }
    }

    /**
     * Convert note name to MIDI number
     * @param noteName Note name like "C4", "D#5", etc.
     */
    //% block="ノート名からMIDI番号 %noteName"
    //% weight=20
    export function noteNameToMidi(noteName: string): number {
        const notes = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
        let result = 0
        
        // Parse note name (e.g., "C4", "D#5")
        if (noteName.length >= 2) {
            const notePart = noteName.replace(/[0-9]/g, '')
            const octave = parseInt(noteName.replace(/[^0-9]/g, ''))
            
            const noteIndex = notes.indexOf(notePart)
            if (noteIndex >= 0 && octave != NaN) {
                result = (octave + 1) * 12 + noteIndex
            }
        }
        return result
    }
}