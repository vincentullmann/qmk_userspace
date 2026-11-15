#pragma once

// Heatmap Blur is the only RGB effect (see rgb_matrix_user.inc, HEATMAP_SPEC.md).
// No ENABLE_RGB_MATRIX_* defines: all built-in effects are disabled.
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_heatmap_blur
