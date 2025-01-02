// #include "print.h"

RGB_MATRIX_EFFECT(ARRG_SPLASH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS


const uint8_t MAX_AGE = 255; // Maximum age for ripple effect
const uint8_t ANIM_SPEED = 32;

// size of the effect
const uint8_t RADIUS_MIN = 10;
const uint8_t RADIUS_MAX = 200;


// Function to perform linear interpolation and mapping
uint8_t fit(uint8_t val, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {

     // Clamp the result to ensure it fits within the output range
    if (val <= in_min) { return out_min; }
    if (val >= in_max) { return out_max; }

    if (in_min == in_max) {
        // Avoid division by zero, return out_min if the input range is zero
        return out_min;
    }
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


bool ARRG_SPLASH(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // tmp: use only last key press
    uint8_t count = g_last_hit_tracker.count; // Number of recent keypresses tracked

    // uint16_t time = g_rgb_timer;

    // for each led
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        HSV hsv = {0, 0, 0}; // Initialize the HSV values
        uint16_t brightness = 0;
        // HSV new = {0, 0, 0};


        // Apply the ripple effect for each tracked keypress
        for (uint8_t j = 0; j < count; j++) {

            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);

            // if (dist > MAX_DISTANCE) {
            //     continue; // Skip processing for distances beyond the maximum range
            // }

            uint16_t age = g_last_hit_tracker.tick[j];
            age = scale16by8(age, ANIM_SPEED);
            if (age > MAX_AGE) {
                continue; // Skip processing for ages beyond the maximum range
            }

            // uint16_t age = g_rgb_timer - tick;
            // Calculate the ripple effect value
            // int16_t effect = tick - dist;
            // if (effect > 255) effect = 255;
            // effect = 255 - effect;

            // scale radius up over time
            uint8_t radius = fit(age, 0, 255, RADIUS_MIN, RADIUS_MAX);

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
        }


        // Apply the calculated brightness to the HSV values
        hsv.h = 180; // Blue hue
        hsv.s = 175; // High saturation
        hsv.v = (uint8_t)brightness; // Final brightness

        RGB rgb = rgb_matrix_hsv_to_rgb(hsv); // Convert HSV to RGB

        // Set the color for the LED
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } // End of LED loop

    return rgb_matrix_check_finished_leds(led_max);
}

#endif

