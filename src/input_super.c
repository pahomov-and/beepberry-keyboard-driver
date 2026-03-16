// SPDX-License-Identifier: GPL-2.0-only
// Super (Mod4) layer: remaps keys pressed while KEY_LEFTMETA is held.
//
// Mapping:
//   Mod4 + 1..0  -> F1..F10
//   Mod4 + t     -> F11
//   Mod4 + y     -> F12
//   Mod4 + h/j/k/l -> Left/Down/Up/Right
//   Mod4 + u/o   -> PgUp/PgDn
//   Mod4 + n/m   -> Home/End
//
// Lone Meta tap (no mapped key pressed) is forwarded as-is so Sway still
// sees it (e.g. to open an app launcher).

#include <linux/input.h>

#include "config.h"
#include "input_iface.h"

// Is KEY_LEFTMETA currently held down?
static bool g_super_held;
// Was KEY_LEFTMETA already sent to the input system
// (because an unmapped key needed the real modifier)?
static bool g_super_sent;

// Returns the remapped keycode, or 0 if not in the Super layer.
//
// F1-F9  — physical keys that carry digit labels in the SYM layer:
//   SYM+W=1  SYM+E=2  SYM+R=3
//   SYM+S=4  SYM+D=5  SYM+F=6
//   SYM+Z=7  SYM+X=8  SYM+C=9
// F10 — KEY_V  (no SYM digit for 0; pick another key if desired)
// F11 — KEY_T  (SYM+T = "(")
// F12 — KEY_Y  (SYM+Y = ")")
//
// Navigation — physical letter keys, no SYM needed.
static uint8_t remap_super_key(uint8_t keycode)
{
	switch (keycode) {
	// F-keys: use the physical key that carries the matching digit in SYM layer
	case KEY_W: return KEY_F1;
	case KEY_E: return KEY_F2;
	case KEY_R: return KEY_F3;
	case KEY_S: return KEY_F4;
	case KEY_D: return KEY_F5;
	case KEY_F: return KEY_F6;
	case KEY_Z: return KEY_F7;
	case KEY_X: return KEY_F8;
	case KEY_C: return KEY_F9;
	case KEY_V: return KEY_F10;
	case KEY_T: return KEY_F11;
	case KEY_Y: return KEY_F12;

	// Navigation
	case KEY_H: return KEY_LEFT;
	case KEY_J: return KEY_DOWN;
	case KEY_K: return KEY_UP;
	case KEY_L: return KEY_RIGHT;

	case KEY_U: return KEY_PAGEUP;
	case KEY_O: return KEY_PAGEDOWN;

	case KEY_N: return KEY_HOME;
	case KEY_M: return KEY_END;
	}

	return 0;
}

int input_super_probe(struct i2c_client *i2c_client, struct kbd_ctx *ctx)
{
	g_super_held = false;
	g_super_sent = false;
	return 0;
}

void input_super_shutdown(struct i2c_client *i2c_client, struct kbd_ctx *ctx)
{
	if (g_super_sent) {
		input_report_key(ctx->kbd_dev, KEY_LEFTMETA, 0);
		input_sync(ctx->kbd_dev);
	}
	g_super_held = false;
	g_super_sent = false;
}

// Returns 1 if the event was consumed (caller should not process further).
int input_super_consumes_keycode(struct kbd_ctx *ctx,
	uint8_t *remapped_keycode, uint8_t keycode, uint8_t state)
{
	uint8_t mapped;

	// --- Handle the Meta key itself ---
	if (keycode == KEY_LEFTMETA) {
		if (state == KEY_STATE_PRESSED) {
			g_super_held = true;
			g_super_sent = false;
			// Defer sending - wait to see if the next key is mapped.
			return 1;

		} else if (state == KEY_STATE_HOLD) {
			return 1;

		} else if (state == KEY_STATE_RELEASED) {
			if (g_super_sent) {
				// Balance the press we deferred-sent earlier.
				input_report_key(ctx->kbd_dev, KEY_LEFTMETA, 0);
				input_sync(ctx->kbd_dev);
			} else {
				// Lone Meta tap - forward to Sway (launcher, etc.).
				input_report_key(ctx->kbd_dev, KEY_LEFTMETA, 1);
				input_sync(ctx->kbd_dev);
				input_report_key(ctx->kbd_dev, KEY_LEFTMETA, 0);
				input_sync(ctx->kbd_dev);
			}
			g_super_held = false;
			g_super_sent = false;
			return 1;
		}
	}

	// --- Handle regular keys while Meta is held ---
	if (!g_super_held)
		return 0;

	mapped = remap_super_key(keycode);

	if (mapped) {
		// Remapped key: emit without Meta modifier.
		if (state == KEY_STATE_PRESSED || state == KEY_STATE_RELEASED) {
			input_report_key(ctx->kbd_dev, mapped,
				state == KEY_STATE_PRESSED);
			input_sync(ctx->kbd_dev);
		}
		// Hold state: just consume, no repeat event.
		return 1;
	}

	// Unmapped key while Meta held: send deferred Meta press first
	// so userspace sees a proper Mod4+key combination.
	if (!g_super_sent && state == KEY_STATE_PRESSED) {
		input_report_key(ctx->kbd_dev, KEY_LEFTMETA, 1);
		input_sync(ctx->kbd_dev);
		g_super_sent = true;
	}

	// Let the key pass through normally.
	return 0;
}
