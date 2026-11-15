// !!! DO NOT ADD #pragma once !!! //

// Heatmap Blur: per-LED heat field. Keypresses inject heat, heat diffuses to
// physically nearby LEDs and decays, heat maps to a thermal palette.
// See HEATMAP_SPEC.md.

RGB_MATRIX_EFFECT(heatmap_blur)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// ---- Tuning constants (adjust by eye on hardware) --------------------------
// Heat scale: 0 .. HEAT_MAX (white).
#    define HEAT_MAX 1024
// Fixed simulation timestep (ms), independent of render rate.
#    define HEAT_STEP_MS 20
// Max sim steps catch-up per frame (avoids long stalls after a hiccup).
#    define HEAT_MAX_STEPS_PER_FRAME 4
// Heat added to the pressed LED on keydown.
#    define HEAT_INJECT 800
// Heat added per sim step to every held key (white after ~3 s).
#    define HEAT_HOLD_PER_STEP 12
// Diffusion: neighbour radius in LED coordinate units (key pitch is ~11).
#    define HEAT_RADIUS 20
// Diffusion: fraction (of 256) each LED moves toward its neighbour average per step.
#    define HEAT_DIFFUSE 64
// Decay: heat is multiplied by HEAT_DECAY/256 per step (~4 s to fade out).
#    define HEAT_DECAY 250
// Max neighbours stored per LED.
#    define HEAT_MAX_NEIGHBOURS 8

// Palette stops: heat -> RGB, linearly interpolated.
typedef struct {
    uint16_t heat;
    uint8_t  r, g, b;
} heat_stop_t;

static const heat_stop_t heat_palette[] = {
    {0, 0, 0, 0},                // off
    {120, 40, 0, 70},            // dark purple
    {280, 0, 0, 255},            // blue
    {500, 0, 255, 0},            // green
    {660, 255, 255, 0},          // yellow
    {840, 255, 0, 0},            // red
    {HEAT_MAX, 255, 255, 255},   // white-hot
};
#    define HEAT_PALETTE_SIZE (sizeof(heat_palette) / sizeof(heat_palette[0]))

// ---- State -----------------------------------------------------------------
static uint16_t heat[RGB_MATRIX_LED_COUNT];
static uint16_t heat_next[RGB_MATRIX_LED_COUNT];
static bool     heat_held[RGB_MATRIX_LED_COUNT];

static uint8_t  neigh_count[RGB_MATRIX_LED_COUNT];
static uint8_t  neigh_idx[RGB_MATRIX_LED_COUNT][HEAT_MAX_NEIGHBOURS];
static uint8_t  neigh_w[RGB_MATRIX_LED_COUNT][HEAT_MAX_NEIGHBOURS]; // sums to ~255
static bool     neigh_ready = false;
static uint32_t heat_last_step;

static inline uint16_t heat_add(uint16_t h, uint16_t amount) {
    uint32_t sum = (uint32_t)h + amount;
    return sum > HEAT_MAX ? HEAT_MAX : (uint16_t)sum;
}

// Called from keymap process_record_user.
void heatmap_blur_key_event(uint8_t row, uint8_t col, bool pressed) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return;
    uint8_t led = g_led_config.matrix_co[row][col];
    if (led == NO_LED || led >= RGB_MATRIX_LED_COUNT) return;
    heat_held[led] = pressed;
    if (pressed) heat[led] = heat_add(heat[led], HEAT_INJECT);
}

static void heatmap_blur_build_neighbours(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        uint8_t  count = 0;
        uint16_t dist[HEAT_MAX_NEIGHBOURS];
        for (uint8_t j = 0; j < RGB_MATRIX_LED_COUNT; j++) {
            if (i == j) continue;
            int16_t  dx = (int16_t)g_led_config.point[i].x - g_led_config.point[j].x;
            int16_t  dy = (int16_t)g_led_config.point[i].y - g_led_config.point[j].y;
            uint16_t d  = sqrt16((uint16_t)(dx * dx + dy * dy));
            if (d > HEAT_RADIUS) continue;
            // Insert sorted by distance, keep nearest HEAT_MAX_NEIGHBOURS.
            uint8_t pos = count < HEAT_MAX_NEIGHBOURS ? count : HEAT_MAX_NEIGHBOURS - 1;
            if (count == HEAT_MAX_NEIGHBOURS && d >= dist[pos]) continue;
            while (pos > 0 && dist[pos - 1] > d) {
                dist[pos]      = dist[pos - 1];
                neigh_idx[i][pos] = neigh_idx[i][pos - 1];
                pos--;
            }
            dist[pos]      = d;
            neigh_idx[i][pos] = j;
            if (count < HEAT_MAX_NEIGHBOURS) count++;
        }
        neigh_count[i] = count;
        // Weights fall off linearly with distance; normalise to sum 255.
        uint32_t total = 0;
        for (uint8_t n = 0; n < count; n++) total += (HEAT_RADIUS + 1 - dist[n]);
        for (uint8_t n = 0; n < count; n++) neigh_w[i][n] = (uint32_t)(HEAT_RADIUS + 1 - dist[n]) * 255 / total;
    }
    neigh_ready = true;
}

static void heatmap_blur_step(void) {
    // Hold: keep adding heat.
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (heat_held[i]) heat[i] = heat_add(heat[i], HEAT_HOLD_PER_STEP);
    }
    // Diffuse + decay.
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        uint32_t h   = heat[i];
        uint8_t  cnt = neigh_count[i];
        if (cnt) {
            uint32_t acc = 0;
            for (uint8_t n = 0; n < cnt; n++) acc += (uint32_t)heat[neigh_idx[i][n]] * neigh_w[i][n];
            int32_t avg = (int32_t)(acc >> 8); // ~ weighted average
            int32_t d   = (avg - (int32_t)h) * HEAT_DIFFUSE / 256;
            int32_t v   = (int32_t)h + d;
            h           = v < 0 ? 0 : (uint32_t)v;
        }
        h              = (h * HEAT_DECAY) >> 8;
        heat_next[i]   = h > HEAT_MAX ? HEAT_MAX : (uint16_t)h;
    }
    memcpy(heat, heat_next, sizeof(heat));
}

static void heat_to_rgb(uint16_t h, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t s = 1;
    while (s < HEAT_PALETTE_SIZE - 1 && h > heat_palette[s].heat) s++;
    const heat_stop_t *lo = &heat_palette[s - 1];
    const heat_stop_t *hi = &heat_palette[s];
    if (h >= hi->heat) {
        *r = hi->r; *g = hi->g; *b = hi->b;
        return;
    }
    int32_t span = hi->heat - lo->heat;
    int32_t pos  = h - lo->heat;
    *r = lo->r + ((int32_t)hi->r - lo->r) * pos / span;
    *g = lo->g + ((int32_t)hi->g - lo->g) * pos / span;
    *b = lo->b + ((int32_t)hi->b - lo->b) * pos / span;
}

static bool heatmap_blur(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init || !neigh_ready) {
        if (!neigh_ready) heatmap_blur_build_neighbours();
        if (params->init) {
            memset(heat, 0, sizeof(heat));
            heat_last_step = timer_read32();
        }
    }

    if (params->iter == 0) {
        uint32_t steps = timer_elapsed32(heat_last_step) / HEAT_STEP_MS;
        if (steps > HEAT_MAX_STEPS_PER_FRAME) {
            steps          = HEAT_MAX_STEPS_PER_FRAME;
            heat_last_step = timer_read32();
        } else {
            heat_last_step += steps * HEAT_STEP_MS;
        }
        while (steps--) heatmap_blur_step();
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t r, g, b;
        heat_to_rgb(heat[i], &r, &g, &b);
        rgb_matrix_set_color(i, r, g, b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
