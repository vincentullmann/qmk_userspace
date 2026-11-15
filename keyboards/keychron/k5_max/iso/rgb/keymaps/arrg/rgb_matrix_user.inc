// !!! DO NOT ADD #pragma once !!! //

// Heatmap Blur ("speed splash"): multisplash-style expanding rings. Each ring
// gets its colour once, at keypress time, from the recent typing speed
// (slow = blue, faster = green/yellow, fast = red, very fast = white).
// Prototype: prototype/heatmap_ideas.html (panel "Splash colour = typing speed, per ring").

RGB_MATRIX_EFFECT(heatmap_blur)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// ---- Tuning constants (adjust by eye on hardware) --------------------------
// Max rings alive at once (oldest dropped).
#    define SPLASH_MAX_HITS 12
// Ring lifetime (ms).
#    define SPLASH_LIFE_MS 2500
// Ring expansion speed (LED units per second; key pitch is ~10 units).
#    define SPLASH_SPEED 25.0f
// Ring width (LED units).
#    define SPLASH_WIDTH 25.0f
// Soft fill inside the ring (0..1).
#    define SPLASH_FILL 0.2f
// Typing speed memory (ms): each press adds 1 that decays with this time constant.
#    define SPEED_TAU_MS 2000.0f
// speed value = SPEED_BASE + recent_presses / SPEED_DIV  (palette position, 1.0 = blue).
// A lone press is SPEED_BASE + 1/SPEED_DIV = 1.0; ~5 keys/s sustained is ~4.6 (red).
#    define SPEED_BASE 0.6f
#    define SPEED_DIV 2.5f
// Smoothing of the speed value (what rings are coloured from): low-pass time
// constant (ms) and how much each press pulls it towards the new target (0..1).
#    define SPEED_SMOOTH_MS 500.0f
#    define SPEED_PRESS_PULL 0.5f
// Rings never start colder than this palette position (1.0 = blue).
#    define SPEED_MIN_RING 1.0f
// Hue fading: as a ring fades out its palette position slides (linearly in age)
// from its press-time value down to this one (0.0 = through green, blue, purple to black).
#    define HUE_FADE_END 0.0f
// Intensity of a held key (0..1), in the current speed colour.
#    define SPLASH_HELD 0.8f

// Palette: speed value -> RGB, linearly interpolated.
typedef struct {
    float   pos;
    uint8_t r, g, b;
} speed_stop_t;

static const speed_stop_t speed_palette[] = {
    {0.0f, 0, 0, 0}, {0.3f, 70, 0, 120}, {1.0f, 30, 80, 255}, {2.0f, 0, 255, 60}, {3.2f, 255, 230, 0}, {4.6f, 255, 20, 0},
};
#    define SPEED_PALETTE_SIZE (sizeof(speed_palette) / sizeof(speed_palette[0]))

// ---- State -----------------------------------------------------------------
typedef struct {
    uint32_t t0;
    uint8_t  led;
    float    pos; // speed value (palette position) at press time
    bool     active;
} splash_hit_t;

// Pairwise LED distances (LED units, max ~233 fits uint8), built once.
static uint8_t led_dist[RGB_MATRIX_LED_COUNT][RGB_MATRIX_LED_COUNT];
static bool    led_dist_ready = false;

static void build_led_dist(void) {
    for (uint8_t a = 0; a < RGB_MATRIX_LED_COUNT; a++) {
        for (uint8_t b = 0; b < RGB_MATRIX_LED_COUNT; b++) {
            float dx       = (float)g_led_config.point[a].x - g_led_config.point[b].x;
            float dy       = (float)g_led_config.point[a].y - g_led_config.point[b].y;
            float d        = sqrtf(dx * dx + dy * dy);
            led_dist[a][b] = d > 255 ? 255 : (uint8_t)(d + 0.5f);
        }
    }
    led_dist_ready = true;
}

static splash_hit_t splash_hits[SPLASH_MAX_HITS];
static uint8_t      splash_next = 0;
static bool         splash_held[RGB_MATRIX_LED_COUNT];

static float    speed_acc    = 0;
static float    speed_smooth = SPEED_BASE;
static uint32_t speed_last_ms;

static void speed_palette_rgb(float v, float *r, float *g, float *b) {
    if (v <= 0) {
        *r = *g = *b = 0;
        return;
    }
    for (uint8_t s = 1; s < SPEED_PALETTE_SIZE; s++) {
        if (v <= speed_palette[s].pos) {
            const speed_stop_t *lo = &speed_palette[s - 1];
            const speed_stop_t *hi = &speed_palette[s];
            float               t  = (v - lo->pos) / (hi->pos - lo->pos);
            *r                     = lo->r + (hi->r - lo->r) * t;
            *g                     = lo->g + (hi->g - lo->g) * t;
            *b                     = lo->b + (hi->b - lo->b) * t;
            return;
        }
    }
    *r = speed_palette[SPEED_PALETTE_SIZE - 1].r; // past the last stop: stay on it (no white)
    *g = speed_palette[SPEED_PALETTE_SIZE - 1].g;
    *b = speed_palette[SPEED_PALETTE_SIZE - 1].b;
}

// Decay the press accumulator and low-pass the speed value towards its target.
static float speed_update(uint32_t now) {
    uint32_t dt = now - speed_last_ms;
    if (dt) {
        speed_acc *= expf(-(float)dt / SPEED_TAU_MS);
        speed_smooth += (1.0f - expf(-(float)dt / SPEED_SMOOTH_MS)) * (SPEED_BASE + speed_acc / SPEED_DIV - speed_smooth);
        speed_last_ms = now;
    }
    return speed_smooth;
}

// Called from keymap process_record_user.
void heatmap_blur_key_event(uint8_t row, uint8_t col, bool pressed) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return;
    uint8_t led = g_led_config.matrix_co[row][col];
    if (led == NO_LED || led >= RGB_MATRIX_LED_COUNT) return;
    splash_held[led] = pressed;
    if (!pressed) return;

    uint32_t now = timer_read32();
    speed_update(now);
    speed_acc += 1.0f;
    speed_smooth += SPEED_PRESS_PULL * (SPEED_BASE + speed_acc / SPEED_DIV - speed_smooth);

    splash_hit_t *h = &splash_hits[splash_next];
    splash_next     = (splash_next + 1) % SPLASH_MAX_HITS;
    h->t0           = now;
    h->led          = led;
    h->pos          = speed_smooth > SPEED_MIN_RING ? speed_smooth : SPEED_MIN_RING;
    h->active       = true;
}

static bool heatmap_blur(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (!led_dist_ready) build_led_dist();

    if (params->init) {
        memset(splash_hits, 0, sizeof(splash_hits));
        memset(splash_held, 0, sizeof(splash_held));
        speed_acc     = 0;
        speed_smooth  = SPEED_BASE;
        speed_last_ms = timer_read32();
    }

    const uint32_t now = timer_read32();
    float          vr, vg, vb;
    {
        float v = speed_update(now);
        speed_palette_rgb(v > SPEED_MIN_RING ? v : SPEED_MIN_RING, &vr, &vg, &vb);
    }

    // Per-ring state for this frame.
    float ring_r[SPLASH_MAX_HITS], ring_fade[SPLASH_MAX_HITS], ring_cr[SPLASH_MAX_HITS], ring_cg[SPLASH_MAX_HITS], ring_cb[SPLASH_MAX_HITS];
    for (uint8_t k = 0; k < SPLASH_MAX_HITS; k++) {
        splash_hit_t *h = &splash_hits[k];
        if (!h->active) continue;
        uint32_t age = now - h->t0;
        if (age >= SPLASH_LIFE_MS) {
            h->active = false;
            continue;
        }
        float f      = 1.0f - (float)age / SPLASH_LIFE_MS;
        ring_fade[k] = f * f * (3.0f - 2.0f * f); // smoothstep: eases in and out
        ring_r[k]    = SPLASH_SPEED * (float)age / 1000.0f;
        float t      = (float)age / SPLASH_LIFE_MS;
        float end    = h->pos < HUE_FADE_END ? h->pos : HUE_FADE_END;
        speed_palette_rgb(h->pos + (end - h->pos) * t, &ring_cr[k], &ring_cg[k], &ring_cb[k]);
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        float r = 0, g = 0, b = 0, sum_i = 0; // weighted colour average: sum(I*c)/sum(I), brightness min(1, sum(I))
        for (uint8_t k = 0; k < SPLASH_MAX_HITS; k++) {
            const splash_hit_t *h = &splash_hits[k];
            if (!h->active) continue;
            float d  = led_dist[h->led][i];
            float rr = ring_r[k];
            if (d > rr + SPLASH_WIDTH) continue; // ahead of the ring: no ring, no fill
            float u = (d - rr) / SPLASH_WIDTH;
            float I  = 1.0f - u * u;
            I        = I > 0 ? I * I : 0; // (1-u^2)^2: soft edges on both sides
            if (d < rr) {
                float q = 1.0f - d / (rr + 3.0f);
                I += SPLASH_FILL * q * q;
            }
            if (I > 1) I = 1;
            I *= ring_fade[k];
            r += ring_cr[k] * I;
            g += ring_cg[k] * I;
            b += ring_cb[k] * I;
            sum_i += I;
        }
        if (splash_held[i]) {
            r += vr * SPLASH_HELD;
            g += vg * SPLASH_HELD;
            b += vb * SPLASH_HELD;
            sum_i += SPLASH_HELD;
        }
        if (sum_i > 0.003f) {
            float k = (sum_i > 1.0f ? 1.0f : sum_i) / sum_i;
            r *= k;
            g *= k;
            b *= k;
        }
        rgb_matrix_set_color(i, r > 255 ? 255 : (uint8_t)r, g > 255 ? 255 : (uint8_t)g, b > 255 ? 255 : (uint8_t)b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
