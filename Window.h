#ifndef WINDOW_H
#define WINDOW_H

class Window {
	static Window* instance;
public:
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps;
	Text temp;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *brickParts[7];
	SDL_Surface *tetrisLogo;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	Game *game;
	BrickType *brickTypes[7];
	DialogBox *dialogBox;
	Ranking *ranking;

	#ifdef DEBUG_MODE
	Debug *debug;
	#endif

	int init();
	void loop();
	void renderWindow();
	void pollEvents();
	void quitWindow();
	void showWindow(DialogBoxType type);

	static Window* getInstance() {
		if (!instance) initInstance();
		return instance;
	}

	~Window();

protected:
	Window();
	static void initInstance() {
		instance = new Window;
	}

};

#endif