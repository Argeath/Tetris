#ifndef GAME_H
#define GAME_H

class Game {
public:
	mapArray map;
	int points, stage, nextBrickType;
	Brick *brick, *nextBrick;
	EVENT_KEY lastPressedKey;
	bool lastMoveDeletedRow, tempSpeedUp;
	double lastStageUp, gameTime;
	bool pause;
	InfoDialogBox *pauseBox;
	int saves;

	Game();
	~Game();

	void printGameTime(SDL_Surface *screen) const;
	void update(double delta);
	void render(SDL_Surface *screen, SDL_Surface *brickParts[]);

	void handleKeys(SDL_Keycode key);

	void loadGame();
	void saveGame();
};

struct GameSave {
	int points, stage;
	int brickType, nextBrickType;
	int brickPosX, brickPosY;
	int gameTime, lastStageUp;
	bool lastMoveDeletedRow;
	int map[20][10];
	int saves;
};

#endif