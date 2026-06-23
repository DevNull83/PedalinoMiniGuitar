# PedalinoMiniGuitar

Custom fork of [PedalinoMini](https://github.com/alf45tar/PedalinoMini), adapted for a guitar-oriented MIDI foot controller setup.

This project keeps the original PedalinoMini architecture and Web UI, while adding a few custom changes focused on live guitar performance, ESP32-S3 hardware support, MIDI reliability and usability on stage.

## Based on PedalinoMini

All core concepts, architecture and most of the original firmware come from the upstream project:

- Original project: [alf45tar/PedalinoMini](https://github.com/alf45tar/PedalinoMini)
- Original author: alf45star

Please refer to the original repository for the general documentation, hardware concepts, Web UI usage and baseline firmware behavior.

## Main changes in this fork

### ESP32-S3 N16R8 support

This fork is mainly used with an ESP32-S3 N16R8 board, using:

- 16 MB flash
- 8 MB PSRAM
- custom PlatformIO environment
- PSRAM enabled
- custom partition table using the full 16 MB flash

The partition layout was adjusted to provide larger OTA slots and more filesystem space while keeping the original firmware behavior stable.

### Guitar-focused hardware mapping

The firmware is configured for a custom guitar pedalboard layout based on:

- multiple footswitches
- resistor ladder inputs
- WS2812 pedal LEDs
- 1,54" OLED display feedback
- expression pedal input

The pin mapping is based on the `ARDUINO_BPI_LEAF_S3` configuration.

### OLED display adaptation

This fork includes display-related adaptations for a low-cost 1.54" AliExpress OLED display using a CH1116/CH116-compatible controller.

The display code was adjusted to work correctly with this specific module, including boot/status feedback and pedal action tags shown during live use.

### Custom display tag behavior

This fork includes custom display behavior based on special tags used in action and bank labels.

These tags are used to improve live feedback on the OLED display and to control how bank names, pedal states and the last triggered actions are shown.

#### `_B_`

The `_B_` tag is used to identify the first action executed at boot.

This allows the display logic to initialize or show the proper startup/bank-related action state when the pedalboard is powered on.

#### `_H_`

The `_H_` tag is used for hold/latch-style pedal actions.

Actions marked with `_H_` are excluded from the normal alternating display behavior between:

- bank name
- last clicked action

Instead, `_H_` represents the last action state associated with that pedal.

The display highlights the action when the state is ON and shows it normally when the state is OFF.

#### `EXP`

The `EXP` tag is used for expression pedal related actions.

Like `_H_`, `EXP` actions are not handled as normal blinking/alternating action labels.  
They are treated as persistent pedal-related display states and can be shown as active/inactive depending on their current state.

#### Purpose

These tags make the OLED display more useful during live performance by separating:

- temporary action feedback
- bank name display
- persistent pedal states
- expression pedal information

This helps keep the display readable and meaningful while playing, especially when several pedals, latch actions and sequences are active at the same time.

### Shared MIDI latch state for CC messages

This fork adds a shared runtime latch state for MIDI Control Change messages.

The goal is to keep MIDI state consistent when the same CC is controlled by different sources, such as:

- direct pedal actions
- sequences
- manual overrides
- LED feedback
- display feedback

This makes it possible to trigger a sequence that enables multiple effects, and later disable one of those effects from its dedicated footswitch without requiring an extra click.

### Improved LED and display synchronization

Pedal LEDs and display tags are synchronized with the shared MIDI latch state for CC-based actions.

This helps keep visual feedback consistent with the actual MIDI state sent to the connected device.

### Sequence cloning

This fork adds a sequence cloning feature to simplify configuration and live setup.

Sequences can be duplicated from an existing one instead of being recreated manually step by step.  
This is useful when building similar effect scenes, where only a few MIDI messages or LED colors need to be changed.

The clone feature helps speed up the creation of:

- effect scenes
- multi-CC macros
- ON/OFF sequence pairs
- preset-style live configurations

### Web UI improvements

This fork includes several Web UI adjustments to make configuration faster and more suitable for the custom guitar pedalboard workflow.

The Web UI has been adapted to support the custom features of this build, including:

- sequence cloning and sequence number spacing in the web UI editor
- improved sequence editing workflow
- custom OLED display tag conventions
- ESP32-S3 N16R8 specific configuration

These changes are intended to make live configuration and maintenance easier without changing the original PedalinoMini concept.