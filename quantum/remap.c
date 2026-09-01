// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later

#include "raw_hid.h"

#ifdef REMAP_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    // PoC: echo received data back to the host
    raw_hid_send(data, length);
}
#endif
