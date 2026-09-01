// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later

#include "raw_hid.h"
#include "remap.h"

#ifdef REMAP_ENABLE

void raw_hid_receive(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &data[0];
    uint8_t *command_data = &data[1];

    switch (*command_id) {
        case id_remap_identify: {
            command_data[0] = REMAP_MAGIC_0;
            command_data[1] = REMAP_MAGIC_1;
            command_data[2] = REMAP_MAGIC_2;
            command_data[3] = (REMAP_PROTOCOL_VERSION >> 8) & 0xFF;
            command_data[4] = REMAP_PROTOCOL_VERSION & 0xFF;
            break;
        }
        default: {
            *command_id = 0xFF;
            break;
        }
    }

    raw_hid_send(data, length);
}

#endif
