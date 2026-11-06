#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Gamepad.h"
#include "MacInput.h"

extern int forwardskey;
extern int backwardskey;
extern int leftkey;
extern int rightkey;
extern int aimkey;
extern int psychicaimkey;
extern int psychickey;

#define MAX_PADS 8

static const float triggerdown = .5;
static const float movedeadzone = .25;
static const float movekeydown = .38;
static const float lookdeadzone = .15;
static const float menustickdown = .5;

/* Mouse counts per second at full deflection: about 430 degrees a second across and
   310 up and down at the default sensitivity, and 80% of that while aiming. */
static const float lookspeedx = 860;
static const float lookspeedy = 610;

/* The menu buttons' centers, as fractions of the window from the top left. */
static const float menux = 340.0f/640;
static const float menuy[2] = { (480-270)/480.0f, (480-147)/480.0f };

static SDL_Gamepad *pads[MAX_PADS];
static int numpads = 0;

static unsigned char padkeys[16];
static bool padbutton = false;
static float lookx = 0;
static float looky = 0;
static Uint64 lastupdate = 0;

static bool wasinmenu = true;
static bool startwasdown = false;
static bool padcursor = false;
static int menuselection = 0;
static bool ignoreclick = false;
static bool ignoredive = false;

void InitGamepads()
{
	if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
		fprintf(stderr, "SDL Init Gamepad failed: %s\n", SDL_GetError());
	}
}

void GamepadEvent(const SDL_Event *event)
{
	switch (event->type) {
		case SDL_EVENT_GAMEPAD_ADDED:
			if (numpads < MAX_PADS) {
				SDL_Gamepad *pad = SDL_OpenGamepad(event->gdevice.which);

				if (pad != NULL) {
					pads[numpads++] = pad;
					padcursor = true;
				}
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			for (int i = 0; i < numpads; i++) {
				if (SDL_GetGamepadID(pads[i]) == event->gdevice.which) {
					SDL_CloseGamepad(pads[i]);
					pads[i] = pads[--numpads];
					break;
				}
			}
			if (numpads == 0) {
				padcursor = false;
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			/* a real mouse or the touchscreen takes the menu cursor back */
			if (event->motion.xrel != 0 || event->motion.yrel != 0) {
				padcursor = false;
			}
			break;
	}
}

static bool PadDown(SDL_GamepadButton button)
{
	for (int i = 0; i < numpads; i++) {
		if (SDL_GetGamepadButton(pads[i], button)) {
			return true;
		}
	}
	return false;
}

static float PadAxis(SDL_GamepadAxis axis)
{
	float value = 0;

	for (int i = 0; i < numpads; i++) {
		float v = SDL_GetGamepadAxis(pads[i], axis) / 32767.0f;

		if (fabsf(v) > fabsf(value)) {
			value = v;
		}
	}
	return value;
}

static void PressKey(int key)
{
	padkeys[key/8] |= 1 << (key%8);
}

static void UpdateMenu()
{
	bool up = PadDown(SDL_GAMEPAD_BUTTON_DPAD_UP) || PadAxis(SDL_GAMEPAD_AXIS_LEFTY) < -menustickdown;
	bool down = PadDown(SDL_GAMEPAD_BUTTON_DPAD_DOWN) || PadAxis(SDL_GAMEPAD_AXIS_LEFTY) > menustickdown;
	bool click = PadDown(SDL_GAMEPAD_BUTTON_SOUTH) || PadAxis(SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > triggerdown;

	if (up || down) {
		padcursor = true;
		menuselection = down ? 1 : 0;
	}

	/* with the cursor elsewhere a click can't land, so use it to come back to the pad */
	if (click && !padcursor) {
		padcursor = true;
		ignoreclick = true;
	}

	if (!click) {
		ignoreclick = false;
	}

	padbutton = click && !ignoreclick;

	/* the press that picks a menu item shouldn't also act in the game */
	ignoredive = PadDown(SDL_GAMEPAD_BUTTON_SOUTH);

	lookx = 0;
	looky = 0;
}

static void UpdatePlay(float elapsed)
{
	float movex = PadAxis(SDL_GAMEPAD_AXIS_LEFTX);
	float movey = PadAxis(SDL_GAMEPAD_AXIS_LEFTY);
	float rightx = PadAxis(SDL_GAMEPAD_AXIS_RIGHTX);
	float righty = PadAxis(SDL_GAMEPAD_AXIS_RIGHTY);
	float amount;

	if (hypotf(movex, movey) > movedeadzone) {
		if (movey < -movekeydown) PressKey(forwardskey);
		if (movey > movekeydown) PressKey(backwardskey);
		if (movex < -movekeydown) PressKey(leftkey);
		if (movex > movekeydown) PressKey(rightkey);
	}

	/* curved for fine aim near the center of the stick's travel; squaring made
	   half deflection too slow */
	amount = hypotf(rightx, righty);
	if (amount > lookdeadzone) {
		float scaled = (fminf(amount, 1) - lookdeadzone) / (1 - lookdeadzone);

		scaled = powf(scaled, 1.5f);
		lookx += rightx / amount * scaled * lookspeedx * elapsed;
		looky += righty / amount * scaled * lookspeedy * elapsed;
	}

	padbutton = PadAxis(SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > triggerdown;

	if (PadAxis(SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > triggerdown) PressKey(aimkey);
	if (PadDown(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) || PadDown(SDL_GAMEPAD_BUTTON_LEFT_STICK)) PressKey(MAC_SHIFT_KEY);
	if (PadDown(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)) PressKey(psychicaimkey);
	if (PadDown(SDL_GAMEPAD_BUTTON_EAST)) PressKey(MAC_CONTROL_KEY);
	if (PadDown(SDL_GAMEPAD_BUTTON_WEST)) PressKey(MAC_R_KEY);
	if (PadDown(SDL_GAMEPAD_BUTTON_NORTH)) PressKey(psychickey);

	if (!PadDown(SDL_GAMEPAD_BUTTON_SOUTH)) {
		ignoredive = false;
	} else if (!ignoredive) {
		PressKey(MAC_SPACE_KEY);
	}

	/* Esc reopens the menu on every frame it's down, so only send one */
	if (PadDown(SDL_GAMEPAD_BUTTON_START) && !startwasdown) {
		PressKey(MAC_ESCAPE_KEY);
	}
}

/* Once a frame; Tick() can run several times a frame, so look is banked for it to spend. */
void UpdateGamepads(bool inmenu)
{
	Uint64 now = SDL_GetTicksNS();
	float elapsed = (float)(now - lastupdate) / SDL_NS_PER_SECOND;

	lastupdate = now;

	/* loading a level is a long frame */
	if (elapsed > .1) {
		elapsed = .1;
	}

	memset(padkeys, 0, sizeof(padkeys));

	if (inmenu && !wasinmenu && numpads > 0) {
		padcursor = true;
		menuselection = 0;
	}
	wasinmenu = inmenu;

	if (inmenu) {
		UpdateMenu();
	} else {
		UpdatePlay(elapsed);
	}

	startwasdown = PadDown(SDL_GAMEPAD_BUTTON_START);
}

void GamepadKeys(unsigned char *keys)
{
	for (size_t i = 0; i < sizeof(padkeys); i++) {
		keys[i] |= padkeys[i];
	}
}

bool GamepadButton()
{
	return padbutton;
}

void GamepadLook(float *x, float *y)
{
	*x += lookx;
	*y += looky;
	lookx = 0;
	looky = 0;
}

bool GamepadCursor(float *x, float *y)
{
	if (padcursor) {
		*x = menux;
		*y = menuy[menuselection];
	}
	return padcursor;
}
