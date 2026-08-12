/*
 * Copyright (c) 2026 AK Embedded Base Kit Zephyr port
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * The only CFB font linked into this image.
 *
 * Zephyr's built-in font set (CONFIG_CHARACTER_FRAMEBUFFER_USE_DEFAULT_FONTS)
 * costs 13811 bytes because it links all three sizes -- 10x16, 15x24 and
 * 20x32 -- and on a 128 KiB part that is a quarter of the whole image for two
 * fonts nothing draws with. We ship a single 10x16 face instead, which fills
 * the 128x64 panel exactly 12 columns by 4 rows.
 *
 * The header is generated, not hand-written. To regenerate it:
 *
 *   $ZEPHYR_BASE/scripts/build/gen_cfb_font_header.py \
 *       -i /usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf \
 *       -x 10 -y 16 -s 15 --center-x -n ak \
 *       -o app/src/cfb_font_ak_1016.h
 *
 * The generator insists that the font's own bounding box match -x/-y exactly,
 * so the point size is not free: DejaVu Sans Mono only measures 10x16 at 15pt.
 * A different face will need a different -s (and possibly --y-offset).
 *
 * FONT_ENTRY_DEFINE places the descriptor in the iterable `cfb_font` section,
 * so this translation unit is never referenced from anywhere -- it is pulled
 * in by the linker, not by a call.
 */

#include "cfb_font_ak_1016.h"
