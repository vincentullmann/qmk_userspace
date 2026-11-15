# QMK Userspace: Keychron K5 Max (ISO, RGB), keymap `arrg`

The K5 Max isn't in upstream QMK. It only lives in Keychron's fork, so you build against
[Keychron/qmk_firmware](https://github.com/Keychron/qmk_firmware) on branch **`2025q3`**,
not `qmk/qmk_firmware`.

- Keyboard: `keychron/k5_max/iso/rgb`
- Keymap: `arrg` (`keyboards/keychron/k5_max/iso/rgb/keymaps/arrg`)

## Build

### Option A: Docker (no local toolchain)

```sh
# one-time: clone Keychron's fork next to this repo
git clone --depth 1 -b 2025q3 --recurse-submodules --shallow-submodules \
    https://github.com/Keychron/qmk_firmware ../qmk_firmware

# build
docker run --rm \
    --user "$(id -u):$(id -g)" -e HOME=/tmp \
    -v "$(realpath ../qmk_firmware)":/qmk_firmware \
    -v "$(pwd)":/qmk_userspace \
    -w /qmk_firmware \
    ghcr.io/qmk/qmk_cli \
    bash -c 'qmk config user.overlay_dir=/qmk_userspace && make keychron/k5_max/iso/rgb:arrg'
```

Output: `keychron_k5_max_iso_rgb_arrg.bin`, written to this repo and to `../qmk_firmware`.

### Option B: Local QMK CLI

```sh
pipx install qmk            # or: uv tool install qmk
qmk setup -H ../qmk_firmware Keychron/qmk_firmware -b 2025q3
qmk config user.overlay_dir="$(realpath .)"
qmk compile -kb keychron/k5_max/iso/rgb -km arrg
```

`qmk userspace-compile` builds every target listed in `qmk.json`.

### Updating to a newer Keychron firmware

```sh
cd ../qmk_firmware
git fetch --depth 1 origin <new-branch>
git checkout FETCH_HEAD
git submodule update --init --recursive --depth 1
```

Then diff `keyboards/keychron/k5_max/iso/rgb/keymaps/keychron/keymap.c` against
`arrg/keymap.c` and carry over any upstream changes, such as renamed keycodes.

## Install (flash)

1. Put the keyboard into bootloader (DFU) mode, using one of these:
   - **Fn + V** (`QK_BOOT` in this keymap), while it's connected by cable.
   - Set the mode switch to **Cable**, unplug it, then hold **Esc** while plugging the USB cable back in.
   - Remove the space bar keycap, then hold the reset button underneath while plugging in.

   In DFU mode the backlight is off and `lsusb` shows `STMicroelectronics STM Device in DFU Mode` (`0483:df11`).
2. Flash it:
   ```sh
   # with the QMK CLI (handles DFU mode for you)
   qmk flash -kb keychron/k5_max/iso/rgb -km arrg
   # or flash the built .bin directly
   dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D keychron_k5_max_iso_rgb_arrg.bin
   ```
3. The keyboard restarts on its own. If keys or lighting act strangely after a big change,
   reset the EEPROM: hold Esc while plugging in, or remap through VIA / [Keychron Launcher](https://launcher.keychron.com/).

On Linux, flashing without root needs QMK's udev rules:
`sudo cp ../qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/ && sudo udevadm control --reload-rules`.

## Keymap summary

| Layer | What it does |
|---|---|
| `WIN_BASE` | Stock Windows layout. Top-right keys: PrtSc, F14, RGB toggle, Macro 1, Macro 2, Key Lock, (empty) |
| `WIN_FN` | Stock Keychron Fn layer + record Macro 1/2 (Fn + the Macro 1/Macro 2 keys), bootloader (Fn+V) |

RGB indicators: Caps → letters red, Shift → letters yellow, Ctrl → special keys turquoise,
Num Lock off → numpad red, Fn held → Fn-layer keys green.

Keep the side switch on **Win**: the Mac position selects a layer this keymap doesn't define.
