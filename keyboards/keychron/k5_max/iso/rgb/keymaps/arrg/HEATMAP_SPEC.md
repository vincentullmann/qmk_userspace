# Spec: Heatmap Blur RGB effect (K5 Max ISO RGB, `arrg` keymap)

## Problem Statement

The keyboard currently runs `SOLID_MULTISPLASH`: a single-hue splash per keypress. I want lighting that works like a live typing heatmap. Each keypress should light its key and then fade out while spreading wider. Overlapping activity should build up "heat", shown on a thermal colour scale, so the board shows where and how intensely I'm typing.

## Solution

A custom RGB matrix effect, **Heatmap Blur**, that is the keyboard's only lighting mode. Each LED holds a heat value. A keypress adds heat at that key. Over time the heat spreads to physically nearby LEDs and decays, so each press reads as a glow that grows wider and dimmer. Overlapping glows add up. Heat maps to a smooth palette: off → dark purple → blue → green → yellow → red → white-hot. With no typing the board is dark. The existing indicator overlays (FN layer, Shift, Caps, Ctrl, NumLock) keep drawing on top.

## User Stories

1. As a typist, I want a pressed key to light up immediately, so that I get instant visual feedback.
2. As a typist, I want each press's glow to grow wider while getting dimmer, so that it looks like a blur dissolving outward.
3. As a typist, I want a single isolated press to peak around green-yellow at its centre, so that one press is visible but not "hot".
4. As a typist, I want overlapping glows from nearby presses to add up, so that typing in one area heats it up.
5. As a typist, I want about 2–3 overlapping presses to reach red, so that normal bursts of typing show warm colours.
6. As a typist, I want sustained hammering in one area to go past red to white-hot, so that extreme activity looks different from merely busy.
7. As a typist, I want heat to fade fully in about 4 seconds after I stop, so that the glow lingers across a sentence without staying forever.
8. As a typist, I want the colour to move smoothly through dark purple → blue → green → yellow → red → white, so that the blur looks soft with no visible bands.
9. As a typist, I want the board fully dark when idle, so that a cold keyboard looks off and only activity glows.
10. As a typist, I want one press's glow to spread visibly across 4+ keys, so that the effect reads as a big wash, not a single-key highlight.
11. As a typist, I want spreading to follow physical key positions, not matrix rows and columns, so that the blur looks round on the real layout, including wide keys and the numpad.
12. As a typist holding a key (Shift, arrow repeat), I want the held key to keep adding heat, so that long holds glow hotter.
13. As a typist holding a key, I want it to reach white-hot after about 3 s, so that brief holds stay warm and only long holds max out.
14. As a typist, I want the effect at full brightness no matter the stored brightness value, so that the palette always looks as designed.
15. As a user of the FN layer, I want FN-mapped keys to keep lighting green on top of the heatmap, so that I can still see the layer map.
16. As a typist, I want Shift to keep turning letters yellow, Caps Lock letters red, Ctrl the special keys turquoise, and NumLock-off the numpad red, all on top of the heat, so that my existing indicators still work.
17. As a user, I want Heatmap Blur to be the only RGB mode, so that I can't cycle away from it by accident and flash isn't spent on unused effects.
18. As a user, I want all RGB keycodes removed from the FN layer (toggle, mode next/prev, brightness, hue, saturation, speed), so that no dead or confusing keys are left.
19. As a user, I want the effect active right after flashing, even with old RGB settings in EEPROM, so that I don't need a manual mode switch or an EEPROM reset.
20. As a wireless user, I want Keychron's existing sleep and backlight timeout to keep working, so that battery life isn't worse than with any other effect.
21. As a typist, I want the animation to stay smooth when I type fast, so that the firmware never feels laggy and keypresses aren't delayed.
22. As a typist, I want the effect to keep its look under fast typing (no cap of about 8 remembered hits), so that every press adds heat.

## Implementation Decisions

- **New custom effect** through QMK's user RGB matrix effect hook (`RGB_MATRIX_CUSTOM_USER`), defined in a user effects file next to the keymap.
- **Only mode**: all built-in effects are disabled. Heatmap Blur is the default mode, and the keymap forces it on at startup in case EEPROM still holds an older mode. Remove the old `SOLID_MULTISPLASH` and reactive defines.
- **Heat field model** (chosen over analytic ripples): one heat value per LED, kept in RAM in a fixed-point type wide enough to go past the red stop up to white. No hit buffer, so overlapping presses have no cap.
- **Injection**:
  - Keydown: arrives through the RGB matrix keypress handler (`RGB_MATRIX_KEYPRESSES` enabled) and adds a fixed amount at the pressed LED. The amount is tuned so that after the first spread the centre sits around green-yellow.
  - Hold: every frame, every currently pressed key that maps to an LED adds heat at a rate tuned so a held key reaches white after about 3 s.
  - Heat saturates at the top of the scale (white). It never wraps.
- **Diffusion**: fixed simulation timestep, independent of render rate. Each step blends every LED's heat toward a weighted average of its neighbours. Neighbours are LEDs within a physical radius, computed from the LED x/y positions in the LED config, with weights falling off with distance. Neighbour lists and weights are computed once at effect init, not every frame. Diffusion strength is tuned so one press visibly reaches 4+ keys before fading out.
- **Decay**: exponential per step, tuned so a single press fades fully in about 4 s. Fixed: the speed setting is ignored.
- **Palette**: a heat → RGB lookup that interpolates smoothly between stops: black (0) → dark purple → blue → green → yellow → red → white (max). Zero heat means LED off. Hue, saturation and the brightness value (V) are all ignored, so output is always full brightness.
- **Rendering**: the effect writes every LED in the current LED range each frame. The existing `rgb_matrix_indicators_advanced_user` overlay logic stays as it is and draws afterwards.
- **Keymap**: every `UG_*` keycode on the FN layer becomes transparent.
- **Performance**: neighbour iteration is O(LEDs × neighbours) per step, kept cheap enough for the MCU that rendering and key scanning aren't affected noticeably.

## Testing Decisions

- No automated tests, by explicit decision.
- Verify by checking that `qmk compile` succeeds for the `keychron/k5_max/iso/rgb:arrg` target with no new warnings, then flash and check by eye: single-press colour and spread, overlap reaching red then white, about 4 s fade, held key reaching white in about 3 s, overlays still on top, mode active after flash, FN-layer RGB keys gone.
- No test prior art exists in this repo.

## Out of Scope

- Any other RGB modes, or switching between modes.
- Adjusting brightness, hue, saturation or speed at runtime for this effect.
- Toggling RGB off with a key.
- Configuring the palette or tuning values via VIA or EEPROM.
- Changing overlay colours to avoid clashing with the palette (clashes accepted).
- Long-term persistent heatmap or statistics.
- Changes to Keychron wireless or sleep power management.

## Further Notes

- Overlay colours (yellow, red, green, turquoise) overlap with the heat palette. This ambiguity is accepted.
- Tuning constants (injection amount, diffusion radius and strength, decay rate, hold rate, palette stops) will need adjusting by eye on the hardware. Keep them as named constants at the top of the effect file.
