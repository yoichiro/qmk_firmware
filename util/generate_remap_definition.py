#!/usr/bin/env python3
# Copyright 2026 Yoichiro Tanaka
# SPDX-License-Identifier: GPL-2.0-or-later
"""Generate a Remap/VIA-style keyboard definition JSON from a QMK keyboard.json.

Extracts the fields Remap requires (name, vendorId, productId, matrix,
layouts.keymap) and converts the flat/absolute QMK layout into the KLE
nested/relative format Remap expects. Intended to run at firmware build
time so the resulting JSON can be embedded in REMAP_ENABLE firmware and
served over Raw HID.

Reference: Remap client's Ajv schema at
src/services/storage/assets/keyboard-definition-schema.json.
"""
import argparse
import json
import sys
from pathlib import Path


def derive_lighting(features):
    """Map QMK's feature toggles to Remap's `lighting` enum."""
    if features.get("rgblight"):
        return "qmk_rgblight"
    if features.get("rgb_matrix"):
        return "qmk_rgb_matrix"
    if features.get("backlight"):
        return "qmk_backlight"
    return "none"


def derive_matrix(qmk):
    """Derive matrix.rows/cols from matrix_pins and split settings."""
    rows = len(qmk["matrix_pins"]["rows"])
    cols = len(qmk["matrix_pins"]["cols"])
    if qmk.get("split", {}).get("enabled"):
        rows *= 2
    return {"rows": rows, "cols": cols}


def get_primary_layout(qmk):
    """Return the layout array to convert.

    Prefers the `LAYOUT` alias when set, otherwise the first defined
    layout.
    """
    layouts = qmk.get("layouts", {})
    aliases = qmk.get("layout_aliases", {})
    target = aliases.get("LAYOUT")
    if target and target in layouts:
        return layouts[target]["layout"]
    if layouts:
        first_key = next(iter(layouts))
        return layouts[first_key]["layout"]
    return []


def _matrix_label(key):
    return f"{key['matrix'][0]},{key['matrix'][1]}"


def convert_layout_to_kle(keys):
    """Convert QMK flat/absolute keys to the KLE nested/relative layout.

    Non-rotated keys are grouped by exact Y coordinate and emitted as
    KLE rows with X/Y offsets computed against KLE's implicit cursor
    (Y auto-advances by 1 between rows).

    Rotated keys (r != 0) are emitted one per KLE row with the pivot
    (rx, ry) taken from the QMK entry when present, else defaulted to
    the key's own top-left corner. This gives Remap a visible rotation
    even when the QMK schema only carries `r`.
    """
    if not keys:
        return []

    non_rotated = [k for k in keys if not k.get("r")]
    rotated = [k for k in keys if k.get("r")]

    y_groups = {}
    for k in non_rotated:
        y_groups.setdefault(k["y"], []).append(k)
    sorted_ys = sorted(y_groups.keys())

    kle_rows = []
    prev_row_y = None

    for row_y in sorted_ys:
        row_keys = sorted(y_groups[row_y], key=lambda k: k["x"])
        if prev_row_y is None:
            y_delta = row_y
        else:
            y_delta = row_y - prev_row_y - 1  # KLE auto-advances y by 1

        emit = []
        cursor_x = 0.0
        first_key = True
        for key in row_keys:
            keyop = {}
            x_delta = key["x"] - cursor_x
            if first_key and y_delta != 0:
                keyop["y"] = y_delta
            if x_delta != 0:
                keyop["x"] = x_delta
            w = key.get("w", 1)
            h = key.get("h", 1)
            if w != 1:
                keyop["w"] = w
            if h != 1:
                keyop["h"] = h
            if keyop:
                emit.append(keyop)
            emit.append(_matrix_label(key))
            cursor_x = key["x"] + w
            first_key = False
        kle_rows.append(emit)
        prev_row_y = row_y

    for key in rotated:
        rx = key.get("rx", key["x"])
        ry = key.get("ry", key["y"])
        x_local = key["x"] - rx
        y_local = key["y"] - ry
        keyop = {"r": key["r"], "rx": rx, "ry": ry}
        if x_local != 0:
            keyop["x"] = x_local
        if y_local != 0:
            keyop["y"] = y_local
        w = key.get("w", 1)
        h = key.get("h", 1)
        if w != 1:
            keyop["w"] = w
        if h != 1:
            keyop["h"] = h
        kle_rows.append([keyop, _matrix_label(key)])

    return kle_rows


def convert(qmk):
    """Build the Remap-compatible definition from parsed QMK data."""
    return {
        "name": qmk["keyboard_name"],
        "vendorId": qmk["usb"]["vid"],
        "productId": qmk["usb"]["pid"],
        "matrix": derive_matrix(qmk),
        "layouts": {
            "keymap": convert_layout_to_kle(get_primary_layout(qmk)),
        },
        "lighting": derive_lighting(qmk.get("features", {})),
        "customKeycodes": [],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="Path to QMK keyboard.json")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Output path (defaults to stdout)",
    )
    parser.add_argument("--indent", type=int, default=2, help="JSON indent width")
    args = parser.parse_args()

    with args.input.open() as f:
        qmk = json.load(f)

    result = convert(qmk)
    payload = json.dumps(result, indent=args.indent, ensure_ascii=False)

    if args.output:
        args.output.write_text(payload + "\n")
    else:
        sys.stdout.write(payload + "\n")


if __name__ == "__main__":
    main()
