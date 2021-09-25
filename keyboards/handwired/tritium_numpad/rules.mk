# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   comment out to disable the options.
#
BOOTMAGIC_ENABLE = lite     # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no	# Mouse keys(+4700)
EXTRAKEY_ENABLE = yes	# Audio control and System control(+450)
CONSOLE_ENABLE = yes	# Console for debug(+400)
COMMAND_ENABLE = yes    # Commands for debug and configuration
NKRO_ENABLE = yes		# USB Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
RGBLIGHT_ENABLE = no   # Enable keyboard underlight functionality (+4870)
BACKLIGHT_ENABLE = yes  # Enable keyboard backlight functionality (+1150)
AUDIO_ENABLE = no
UNICODE_ENABLE = no 		# Unicode

LAYOUTS = numpad_6x4 ortho_6x4
