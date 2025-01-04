


RGB_MATRIX_EFFECT(ARRG_SPLASH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "quantum/action.h"
#include "quantum/keymap_common.h"
#include "led.h"
#include "host.h"


const uint8_t MAX_AGE = 200; // Maximum age for ripple effect
const uint8_t ANIM_SPEED = 42;

// size of the effect
const uint8_t RADIUS_MIN = 10;
const uint8_t RADIUS_MAX = 200;


// Function to perform linear interpolation and mapping
uint8_t fit8(uint8_t val, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {

     // Clamp the result to ensure it fits within the output range
    if (val <= in_min) { return out_min; }
    if (val >= in_max) { return out_max; }

    if (in_min == in_max) {
        // Avoid division by zero, return out_min if the input range is zero
        return out_min;
    }
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t fit16(uint16_t val, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {

     // Clamp the result to ensure it fits within the output range
    if (val <= in_min) { return out_min; }
    if (val >= in_max) { return out_max; }

    if (in_min == in_max) {
        // Avoid division by zero, return out_min if the input range is zero
        return out_min;
    }
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}





#define NUMPAD_RANGE KC_NUM_LOCK ... KC_KP_DOT


void handle_flags(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    led_t led_state = host_keyboard_led_state();

    dprintf("led_state.caps_lock=%u\n", led_state.caps_lock);
    uint8_t layer = 2;


    if (led_state.caps_lock) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {

                    uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    if (keycode >= KC_A && keycode <= KC_Z) {
                        rgb_matrix_set_color(index, RGB_RED);
                    } // if keycode
                } // if index
            }
        }
    }

    // light up numblock if numlocks is OFF
    if (!led_state.num_lock) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {

                    uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    if (keycode >= KC_NUM_LOCK && keycode <= KC_KP_DOT) {
                        rgb_matrix_set_color(index, RGB_RED);
                    } // if keycode
                } // if index
            }
        }
        return;
    }
}




bool ARRG_SPLASH(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // tmp: use only last key press
    uint8_t count = g_last_hit_tracker.count; // Number of recent keypresses tracked

    // uint16_t time = g_rgb_timer;


    // for each led
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // HSV new = {0, 0, 0};

        uint16_t led_effect = 0;
        // uint16_t led_age = UINT16_MAX;

        // Apply the ripple effect for each tracked keypress
        for (uint8_t j = 0; j < count; j++) {


            uint16_t hit_effect = 0;


            // if (dist > MAX_DISTANCE) {
            //     continue; // Skip processing for distances beyond the maximum range
            // }

            uint16_t hit_age = g_last_hit_tracker.tick[j];
            hit_age = scale16by8(hit_age, ANIM_SPEED);
            if (hit_age > MAX_AGE) {
                continue; // Skip processing for distances beyond the maximum range
            }

            // scale based on age. start at 255 and fade to 0
            hit_effect = fit16(hit_age, 20, MAX_AGE, 255, 0);
            // hit_effect = scale8(hit_effect, hit_effect);


            int16_t dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t dist = sqrt16(dx * dx + dy * dy);

            if (dist > hit_age) {
                // cut off outside effect radius
                continue;
            }
            // hit_effect = dist < hit_age ? hit_effect : 0;

            // gradient along radius
            dist = fit8(dist, hit_age/2, hit_age, 255, 0);
            hit_effect = scale8(hit_effect, dist);

            // add the distance to the age, to fade off old values
            // anything less than 2- units will get nothing added
            // anything >200 units will get the full 100 added
            // uint8_t dist_age = fit8(dist, 20, 200, 0, 100);
            // hit_age += dist_age;

            /*
            // uint16_t age = g_rgb_timer - tick;
            // Calculate the ripple effect value
            // int16_t effect = tick - dist;
            // if (effect > 255) effect = 255;
            // effect = 255 - effect;


            uint8_t effect = 0;
            // effect = 255 - scale8(dist, radius);
            // effect = dist > radius ? 0 : 255;
            effect = fit(dist, RADIUS_MIN, radius, 255, 0);

            if (i == 0 && j == count - 1) {
                dprintf("age=%5u | radius=%2u | effect=%2u\n", age, radius, effect);
            }

            // dim over time
            uint8_t decay = scale8(255 - age, 255 - age); // decay quadratically
            effect = scale8(effect, decay);

            // Scale the distance relative to MAX_DISTANCE
            // effect *= float(dist / MAX_DISTANCE);
            // effect = 255 - age;
            // uint8_t effect8 = (uint8_t)effect;
            // effect8 = scale8(effect8, effect8);

            // Decay the brightness over distance and time
            brightness += effect; // blend8(200, 0, effect8);
            if (brightness > 255) {
                brightness = 255; // Clamp to maximum brightness
            }
            */
            led_effect += hit_effect;
        }

        if (led_effect > 255) {
            led_effect = 255; // Clamp to maximum brightness
        }

        // quadracti falloff
        uint8_t val = scale8(led_effect, led_effect);

        HSV hsvA = {160, 175, val}; // Initialize the HSV values
        HSV hsvB = {185, 220, val}; // Initialize the HSV values

        // // Apply the calculated brightness to the HSV values
        // hsv.h = fit8(led_effect, 50, 255, 180, 0) ; // Blue hue
        // hsv.s = 175;
        // hsv.v = 

        RGB rgbA = rgb_matrix_hsv_to_rgb(hsvA); // Convert HSV to RGB
        RGB rgbB = rgb_matrix_hsv_to_rgb(hsvB); // Convert HSV to RGB


        uint8_t r = fit8(led_effect, 32, 0, rgbA.r, rgbB.r);
        uint8_t g = fit8(led_effect, 32, 0, rgbA.g, rgbB.g);
        uint8_t b = fit8(led_effect, 32, 0, rgbA.b, rgbB.b);
        rgb_matrix_set_color(i, r, g, b);
    } // End of LED loop


    handle_flags(params);

    return rgb_matrix_check_finished_leds(led_max);
}

#endif
