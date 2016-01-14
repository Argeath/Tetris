#include "main.h"

int Color::brazowy = 0;
int Color::czarny = 0;
int Color::prawieCzarny = 0;
int Color::czerwony = 0;
int Color::fioletowy = 0;
int Color::niebieski = 0;
int Color::pomaranczowy = 0;
int Color::rozowy = 0;
int Color::szary = 0;
int Color::zielony = 0;
int Color::zolty = 0;

void Color::init(SDL_Surface *screen) {
	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	prawieCzarny = SDL_MapRGB(screen->format, 0x15, 0x15, 0x15);
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	niebieski = SDL_MapRGB(screen->format, 0x8A, 0x2B, 0xE2);
	fioletowy = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	rozowy = SDL_MapRGB(screen->format, 0xFF, 0x14, 0x93);
	brazowy = SDL_MapRGB(screen->format, 0x8B, 0x45, 0x13);
	szary = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);
	pomaranczowy = SDL_MapRGB(screen->format, 0xFF, 0xA5, 0x00);
}