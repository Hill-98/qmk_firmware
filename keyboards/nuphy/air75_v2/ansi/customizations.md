# Customizations & Fixes

Custom firmware for the NuPhy Air75 V2 (ANSI). The shortcuts below match the default keymap in
`keymaps/default/keymap.c`. If you remap keys in VIA, shortcuts move with the keycodes.

## Shortcuts

`M layer` = hold **Fn + Right Shift**, then press the key.
"Hold 3 s" means keep the key pressed for about 3 seconds.

### Base layer

| Key | Function |
|---|---|
| Caps Lock | Tap: Caps Lock. Hold: Left Ctrl |
| Home / End (Mac mode) | Tap: Ctrl+A / Ctrl+E (line start / end). Double tap: Home / End |

### Fn layer

| Keys | Win mode | Mac mode |
|---|---|---|
| Fn + 1 / 2 / 3 | Bluetooth 1 / 2 / 3. Hold 3 s: pair | same |
| Fn + 4 | 2.4G dongle. Hold 3 s: pair | same |
| Fn + F1 / F2 | – | Screen brightness down / up |
| Fn + F3 | – | Mission Control |
| Fn + F4 | – | Spotlight (Cmd+Space) |
| Fn + F5 | – | Dictation |
| Fn + F6 | – | Do Not Disturb |
| Fn + F7 / F8 / F9 | Previous / Play-Pause / Next | same |
| Fn + F10 / F11 / F12 | Mute / Volume down / Volume up | same |
| Fn + Print Screen | – | Area screenshot (Cmd+Shift+4) |
| Fn + W / A / S / D | Arrow keys | same |
| Fn + Q / E | Home / End | Tap: Ctrl+A / Ctrl+E. Double tap: Home / End |
| Fn + [ / ] | Page Up / Page Down | same |
| Fn + Z / X / C / V | Ctrl + Z / X / C / V | Cmd + Z / X / C / V |
| Fn + Home / End key | – | Home / End |
| Fn + ↑ / ↓ | Key backlight brightness up / down | same |
| Fn + ← | Next key backlight effect | same |
| Fn + → | Key backlight hue | same |

Bluetooth / 2.4G keys only work with the mode switch set to wireless.

### M layer (Fn + Right Shift + key)

| Key | Function |
|---|---|
| Esc | Reboot. Keep Esc held while it restarts to enter the bootloader |
| F1 | Toggle debug output (QMK console) |
| F2 | Tap: cycle the RF wake-up delay (80 / 280 / 480 / 680 / 880 ms). Hold 3 s: RF module DFU mode (USB only) |
| F11 / F12 | Debounce time down / up |
| Delete | Hold 3 s: factory reset |
| - / = | Light sleep timeout down / up |
| Backspace | Sleep mode (green / yellow / red) |
| ] | Deep sleep now (battery only) |
| \ | Toggle battery level display on the right side LEDs |
| D | Debounce algorithm |
| G | Tap: game mode on. Hold 3 s: reset game mode settings |
| K | SOCD mode |
| N | Num Lock indicator: off / side LEDs / Insert key LED |
| O | Side LEDs: both sides / right only / left only / alternate every minute |
| , / . | Side LED animation slower / faster |
| ↑ / ↓ | Side LED brightness up / down |
| ← | Side LED mode (wave / spectrum / static / breathing / off) |
| → | Side LED color |
| Win (Mac: Cmd) | GUI key lock on / off |

### Game mode

Game mode uses its own layer. Caps Lock is a plain Caps Lock and the bottom row is in Windows order
(Ctrl, Win, Alt) in both OS modes. The M layer is not reachable in game mode.

| Keys | Function |
|---|---|
| Fn + Esc | Tap: leave game mode. Hold 3 s: reset game mode settings |
| Fn + 1 / 2 / 3 / 4 | Bluetooth 1 / 2 / 3, 2.4G. Hold 3 s: pair |
| Fn + F7 … F12 | Media keys |

Fn + \ is mapped to the battery display on the game layer, but the firmware blocks it in game mode.

### Keycodes not in the default keymap

These exist and can be assigned in VIA: `LNK_USB`, `PRT_SCR` (Mac: full screenshot, Win: Print Screen),
`BAT_NUM` (battery level on the number row while held), `RGB_TEST`, `NUMLOCK_INS` (tap: Insert,
hold 300 ms: Num Lock), `WIN_LOCK`, and the QMK RGB keys for previous effect, hue down, saturation,
speed and on/off. `CAPS_WORD` and `KEYBORD_LOCK` do nothing and only keep VIA keycode numbering stable.

## Behaviour

### Indicators

-  Caps Lock lights the left side LEDs. Num Lock uses white on the side LEDs or the Insert key LED,
   depending on the Num Lock indicator mode (Fn + Right Shift + N).
-  While connecting, the left side LEDs blink and the matching number key lights up: blue on 1–3 for
   Bluetooth, green on 4 for 2.4G.
-  Status indicators scale with the side LED brightness, so they are not full brightness when the side LEDs are dimmed.
-  Settings keys confirm on their own key: debounce type on D, game mode on G, SOCD mode on K,
   RF wake-up delay on F2, GUI lock on the Win key, debounce time and sleep timeout on F1–F10.
-  Battery display (Fn + Right Shift + \): the right side LEDs show the level. Red when at or below 30 %.
   When the battery is low the right side LEDs breathe red.
-  `BAT_NUM` (not mapped by default) shows the battery level on keys 1–0 while held (0 = 100 %).
   Color: red below 20 %, orange up to 50 %, yellow up to 80 %, green above. The last lit key shows the
   units digit: green = exact tens, red 1–3, orange 4–6, green 7–9.

### Side LEDs

-  7 brightness levels, from 0 (off) to 6. Default after reset is 1.
-  Modes: wave, spectrum, static, breathing, off. Default is wave in rainbow colors.

### Key backlight

-  Default effect is Cycle Left/Right at half brightness.
-  Included effects: Solid Color, Alphas Mods, Gradient Up/Down, Gradient Left/Right, Breathing,
   Band Sat., Band Val., Pinwheel Sat., Pinwheel Val., Spiral Sat., Spiral Val., Cycle All,
   Cycle Left/Right and Game Keys (only ESC, W, A, S, D and the arrows lit). Other QMK effects were
   removed to save flash.
-  At brightness 0 the animation stops and the LED power is switched off.
-  When the battery is low, the key backlight is turned off and side LEDs are dimmed to level 1 until it recovers.

### Settings storage

-  Changes are written to EEPROM 30 seconds after the last change, so repeated key presses cause one write.
-  Pending changes are also written right away when the keyboard goes to sleep and when game mode is toggled.
   Switching the keyboard off within 30 seconds of a change still loses it.

### Debounce

-  Default: 5 ms, asym_eager_defer_pk.
-  Algorithms (Fn + Right Shift + D): asym_eager_defer_pk (green, default), sym_eager_pr (yellow),
   sym_defer_pk variant (red).
-  Time (Fn + Right Shift + F11 / F12), shown on F1–F10:
   1–10 ms in steps of 1 (green), 12–30 ms in steps of 2 (yellow), 35–75 ms in steps of 5 (red), 100 ms (purple, F1–F10).
   Example: 6 ms lights F1–F6 green, 18 ms lights F1–F4 yellow, 65 ms lights F1–F7 red.
-  Game mode and normal mode keep separate debounce settings.

### SOCD (Fn + Right Shift + K)

Applies to A/D, Left/Right and Up/Down, each pair independently. The key color shows the mode.

(0) Disabled: <br />
<pre>Keys   | .. | A. | AD | A.
Report | .. | A. | AD | A. </pre>

(1) Cancellation: <br />
<pre>Keys   | .. | A. | AD | A.
Report | .. | A. | .D | .. ----- (D cancels A, no restore on D keyup) </pre>

(2) Exclusion: <br />
<pre>Keys   | .. | A. | AD | A.
Report | .. | A. | .D | A. ----- (D excludes A, restores A on D keyup) </pre>

(3) Nullification: <br />
<pre>Keys   | .. | A. | AD | A.
Report | .. | A. | .. | A. ----- (D nullifies A, neither registered, A restored on D keyup) </pre>

### Game mode

-  Maximizes scan rate: side LED animation and some indicators are disabled, and the keyboard never sleeps.
-  Uses its own key backlight effect, brightness, side LED color and debounce settings. The default
   keymap has no backlight, side LED or debounce keys on the game layer, so these can only be changed
   by mapping the keys in VIA. Fn + Esc held for 3 s resets them to defaults.

### Sleep

Sleep mode is selected with Fn + Right Shift + Backspace. The right side LEDs flash the mode color.
The keyboard never sleeps while connected via USB, while charging, or in game mode.

| Mode | Behaviour |
|---|---|
| Green | 10 s idle: MCU light sleep (WFI), scan rate ~700. After the light sleep timeout (default 2 min): side LEDs off. 4 min later: deep sleep (MCU stop mode, RF sleeps) |
| Yellow | After the light sleep timeout (default 6 min): side LEDs off, the RF module later powers down by itself. No MCU light or deep sleep |
| Red | No sleep. LEDs stay on |

-  The light sleep timeout (Fn + Right Shift + - / =) goes from 1 to 100 minutes, with the same steps and
   F-key display as the debounce time. Green and yellow modes keep separate values.
-  After 30 s idle the matrix scan slows down (~300 scans/s) to save battery. Not on USB power or while charging.
-  Entering deep sleep flashes the side LEDs blue (Bluetooth) or green (2.4G).
-  Wireless key presses made while the keyboard wakes up are buffered (64 events) and sent once the link is
   back. The buffer is dropped if the link is not restored within 6 s. The RF wake-up delay
   (Fn + Right Shift + F2) should be 80 ms for RF firmware 1.x and 480 ms for 2.x.

### Scan rate

| State | Matrix scans per second |
|---|---|
| Normal | ~1700–1900 |
| Idle (battery) | ~300–700 |
| Game mode | ~3200–3900 |

### Other

-  Tap keycode delay 2 ms (QMK default 8 ms). LTO enabled.
-  Keyboard report transmission and crash fixes based on jincao1's work.
-  With debug on, the build is printed to the console, for example:
   `Keyboard: nuphy/air75_v2/ansi @ QMK 0.25.10-62-g2a4e8d | BUILD: 2024-07-09-09:31:29 (1e4798ae3e)`

## Fork changes (2026-09)

-  **Out-of-bounds LED writes / hang fixed.** Indicators look up key LEDs by keycode. When a keycode is not on
   the keymap (for example `KC_CAPS` replaced by `LCTL_T(KC_CAPS)`) the lookup returned 255, which wrote past the
   LED buffer every loop and could hang the keyboard. Such writes are now ignored.
-  **SOCD fixed.** It only worked after the opposite key had been held for ~200 ms, and all three pairs shared one
   state. Each pair is now tracked independently and SOCD applies immediately.
-  **Caps Word removed.** It was disabled in QMK but its indicator and toggle were still active.
-  **Settings saved before sleep and game mode switches**, instead of being lost.
-  **Low-battery power save no longer overwrites saved brightness** when another setting is changed meanwhile.
-  **Status indicators work with the key backlight toggled off.**
-  **Wake-up interrupts are only armed during deep sleep.** They fired on every row scan, USB and LED data edge.
-  **Lighter queue replay after wake-up:** each queued report is sent 4 times, 2 ms apart
   (`RF_QUEUE_REPEAT_COUNT`, `RF_QUEUE_REPEAT_INTERVAL`), instead of 24 back-to-back sends.
-  **No idle slowdown on USB power or while charging.**
-  **Matrix scan cannot hang** on a stuck column (bounded wait).
-  `NUMLOCK_INS` hold time is an accurate 300 ms (`NUMLOCK_HOLD_TIME`).
-  Removed dead configuration: OS detection with automatic keyboard reset, Auto Shift, `redefine.c/h`.
-  The keyboard no longer defines `process_record_user`, so keymaps can use it.
-  VIA definition effect list matches the firmware.
-  Key backlight stays on the bit-bang driver. SPI + DMA is not possible on this board: on STM32F072 it needs the
   SCK pin, and both SPI1 SCK pins (A5, B3) are matrix columns.

## Author

[@adi4086](https://github.com/adi4086)
