# Bluetooth MIDI Extension for micro:bit

This extension allows micro:bit V2 to communicate with GarageBand via Bluetooth MIDI.

## Blocks

- `Bluetooth MIDI を初期化` - Start Bluetooth MIDI advertising
- `MIDI が接続されている` - Check connection status
- `ノートオン/ノートオフ` - Send MIDI notes
- `コントロールチェンジ` - Send MIDI CC messages
- `ノートを鳴らす` - Play a note with duration

## Example: Simple Piano

```blocks
bluetoothMIDI.init()

basic.forever(function () {
    if (input.buttonA.isPressed()) {
        bluetoothMIDI.noteOn(60, 100, 1)
        basic.pause(100)
        bluetoothMIDI.noteOff(60, 0, 1)
    }
})