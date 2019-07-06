
/*
 * Another World engine rewrite
 * Copyright (C) 2004-2005 Gregory Montoir (cyx@users.sourceforge.net)
 */

#include <SDL/SDL.h>
#include "graphics.h"
#include "systemstub.h"
#include "util.h"

struct SystemStub_SDL1 : SystemStub {

	static const int kJoystickIndex = 0;
	static const int kJoystickCommitValue = 16384;
	static const float kAspectRatio;

	int _w, _h;
	float _aspectRatio[4];
	int _texW, _texH;
	int _screenshot;

	SystemStub_SDL1();
	virtual ~SystemStub_SDL1() {}

	virtual void init(const char *title, const DisplayMode *dm);
	virtual void fini();

	virtual void prepareScreen(int &w, int &h, float ar[4]);
	virtual void updateScreen();
	virtual void setScreenPixels565(const uint16_t *data, int w, int h);

	virtual void processEvents();
	virtual void sleep(uint32_t duration);
	virtual uint32_t getTimeStamp();

	void setAspectRatio(int w, int h);
};

const float SystemStub_SDL1::kAspectRatio = 16.f / 10.f;

SystemStub_SDL1::SystemStub_SDL1()
	: _w(0), _h(0), _texW(0), _texH(0) {
}

void SystemStub_SDL1::init(const char *title, const DisplayMode *dm) {
}

void SystemStub_SDL1::fini() {
}

void SystemStub_SDL1::prepareScreen(int &w, int &h, float ar[4]) {
}

void SystemStub_SDL1::updateScreen() {
}

void SystemStub_SDL1::setScreenPixels565(const uint16_t *data, int w, int h) {
}

void SystemStub_SDL1::processEvents() {
}

void SystemStub_SDL1::sleep(uint32_t duration) {
	SDL_Delay(duration);
}

uint32_t SystemStub_SDL1::getTimeStamp() {
	return SDL_GetTicks();
}

void SystemStub_SDL1::setAspectRatio(int w, int h) {
	const float currentAspectRatio = w / (float)h;
	if (int(currentAspectRatio * 100) == int(kAspectRatio * 100)) {
		_aspectRatio[0] = 0.f;
		_aspectRatio[1] = 0.f;
		_aspectRatio[2] = 1.f;
		_aspectRatio[3] = 1.f;
		return;
	}
	// pillar box
	if (currentAspectRatio > kAspectRatio) {
		const float inset = 1.f - kAspectRatio / currentAspectRatio;
		_aspectRatio[0] = inset / 2;
		_aspectRatio[1] = 0.f;
		_aspectRatio[2] = 1.f - inset;
		_aspectRatio[3] = 1.f;
		return;
	}
	// letter box
	if (currentAspectRatio < kAspectRatio) {
		const float inset = 1.f - currentAspectRatio / kAspectRatio;
		_aspectRatio[0] = 0.f;
		_aspectRatio[1] = inset / 2;
		_aspectRatio[2] = 1.f;
		_aspectRatio[3] = 1.f - inset;
		return;
	}
}

SystemStub *SystemStub_SDL1_create() {
	return new SystemStub_SDL1();
}
