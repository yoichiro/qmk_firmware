// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>

// Remap-specific command IDs (0x80-0xEF namespace)
enum remap_command_id {
    id_remap_identify = 0x80,
    // 0x81-0xEF reserved for future Remap extensions
};

// Magic bytes returned by id_remap_identify
#define REMAP_MAGIC_0 'R'
#define REMAP_MAGIC_1 'M'
#define REMAP_MAGIC_2 'P'

// Remap firmware protocol version (16-bit, big-endian on wire)
#define REMAP_PROTOCOL_VERSION 0x0001
