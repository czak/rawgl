
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

	SDL_Surface *_screen;
	SDL_Surface *_texture;

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
	: _w(0), _h(0), _texW(0), _texH(0), _screen(0), _texture(0) {
}

void SystemStub_SDL1::init(const char *title, const DisplayMode *dm) {
	SDL_Init(SDL_INIT_VIDEO);
	_screen = SDL_SetVideoMode(320, 200, 0, SDL_DOUBLEBUF);
}

void SystemStub_SDL1::fini() {
	if (_texture) {
		SDL_FreeSurface(_texture);
	}
	SDL_Quit();
}

void SystemStub_SDL1::prepareScreen(int &w, int &h, float ar[4]) {
	w = _w;
	h = _h;
	ar[0] = _aspectRatio[0];
	ar[1] = _aspectRatio[1];
	ar[2] = _aspectRatio[2];
	ar[3] = _aspectRatio[3];
	// TODO: CLEAR?
}

void SystemStub_SDL1::updateScreen() {
	SDL_Flip(_screen);
}

void SystemStub_SDL1::setScreenPixels565(const uint16_t *data, int w, int h) {
	if (!_texture) {
		Uint32 rmask, gmask, bmask, amask;
		rmask = 0b1111100000000000;
		gmask = 0b0000011111100000;
		bmask = 0b0000000000011111;
		amask = 0;
		_texture = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 16,
				rmask, gmask, bmask, amask);
		if (!_texture) {
			return;
		}
		_texW = w;
		_texH = h;
	}
	SDL_LockSurface(_texture);
	memcpy(_texture->pixels, data, w * h * sizeof(uint16_t));
	SDL_UnlockSurface(_texture);
	
	SDL_BlitSurface(_texture, NULL, _screen, NULL);
}

void SystemStub_SDL1::processEvents() {
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			_pi.quit = true;
			break;
		case SDL_KEYUP:
			switch(ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case SDLK_LCTRL:
				_pi.button = false;
				break;
			case SDLK_s:
				_pi.screenshot = true;
				break;
			case SDLK_c:
			case SDLK_ESCAPE:
				_pi.code = true;
				break;
			case SDLK_p:
				_pi.pause = true;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			if (ev.key.keysym.mod & KMOD_ALT) {
				if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
				} else if (ev.key.keysym.sym == SDLK_x) {
					_pi.quit = true;
				}
				break;
			} else if (ev.key.keysym.mod & KMOD_CTRL) {
				if (ev.key.keysym.sym == SDLK_f) {
					_pi.fastMode = true;
				}
				break;
			}
			_pi.lastChar = ev.key.keysym.sym;
			switch(ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask |= PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask |= PlayerInput::DIR_DOWN;
				break;
			case SDLK_LCTRL:
				_pi.button = true;
				break;
			case SDLK_END:
				_pi.quit = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
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
