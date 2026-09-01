// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Mirrors quantum/nvm/eeprom/nvm_eeprom_via_internal.h as of QMK 0.34.0.
// Keeps the same layout so that dynamic keymap storage remains consistent
// when a keyboard switches between VIA firmware and Remap firmware builds.

#ifndef REMAP_EEPROM_MAGIC_ADDR
#    define REMAP_EEPROM_MAGIC_ADDR (EECONFIG_SIZE)
#endif

#define REMAP_EEPROM_LAYOUT_OPTIONS_ADDR (REMAP_EEPROM_MAGIC_ADDR + 3)

// The end of the EEPROM memory used by Remap.
// By default, dynamic keymaps will start at this if there is no
// custom config.
#define REMAP_EEPROM_CUSTOM_CONFIG_ADDR (REMAP_EEPROM_LAYOUT_OPTIONS_ADDR + REMAP_EEPROM_LAYOUT_OPTIONS_SIZE)

#define REMAP_EEPROM_CONFIG_END (REMAP_EEPROM_CUSTOM_CONFIG_ADDR + REMAP_EEPROM_CUSTOM_CONFIG_SIZE)
