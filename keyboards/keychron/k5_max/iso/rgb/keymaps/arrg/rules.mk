VIA_ENABLE = yes

KEY_LOCK_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes

RGB_MATRIX_CUSTOM_USER = yes

# GCC 16 (Arch arm-none-eabi-gcc) errors on Keychron's lkbt51.c; keep it a warning
EXTRAFLAGS += -Wno-error=unused-but-set-variable
