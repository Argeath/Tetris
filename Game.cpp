#include "main.h"

Game::Game() : points(0), stage(1), lastMoveDeletedRow(false), tempSpeedUp(false), lastStageUp(0), gameTime(0), pause(false), saves(0) {
	map = mapArray();
	map.size = V2D(10, 20);
	map.allocateMemory();
	map.fillMap(0);
	brick = nextBrick = nullptr;
	lastPressedKey = NONE;
	nextBrickType = rand() % 7;

	pauseBox = new InfoDialogBox();
	pauseBox->size = V2D(80, 50);
	pauseBox->answer = "PAUZA";
}

Game::~Game() {
	map.freeMemory();
}

void Game::update(double delta) {
	if (pause)
		return;

	gameTime += delta;

	if (stage < 10) {
		lastStageUp += delta;
		if (lastStageUp > STAGE_UP_TIME) {
			stage++;
			lastStageUp -= STAGE_UP_TIME;
		}
	}

	if (brick != nullptr) {
		// Opadanie klocka
		brick->update(delta);

		if (brick->isInCollision(SOUTH)) {
			for (int iy = 0; iy < brick->map.size.y; iy++) {
				for (int ix = 0; ix < brick->map.size.x; ix++) {
					if (brick->map.map[iy][ix] > 0) {
						Vector2D pos = brick->lastPosition + brick->brickType->getVectorFromCenter(V2D(ix, iy));
						if (pos.y >= 0)
							map.map[int(pos.y)][int(pos.x)] = brick->brickType->type;
						else {
							pause = true;
							if (Window::getInstance()->ranking->getPlace(points) != -1) {
								Window::getInstance()->showWindow(InputNameToRanking);
							}
							else {
								Window::getInstance()->showWindow(FinishGame);
							}
							return;
						}
					}
				}
			}

			// Usuwanie pelnych wierszy
			int removedRows = 0;
			for (int iy = 0; iy < map.size.y; iy++) {
				bool toRemove = true;
				for (int ix = 0; ix < map.size.x; ix++) {
					if (map.map[iy][ix] == 0)
						toRemove = false;
				}
				if (toRemove) {
					removedRows++;
					for (int py = iy; py > 0; py--) {
						for (int ix = 0; ix < map.size.x; ix++) {
							map.map[py][ix] = map.map[py - 1][ix];
						}
					}
				}
			}

			// Punktowanie usuwania wierszy
			switch (removedRows) {
			case 1:
				points += 100 * (stage + 1);
				lastMoveDeletedRow = false;
				break;
			case 2:
				points += 200 * (stage + 1);
				lastMoveDeletedRow = false;
				break;
			case 3:
				points += 400 * (stage + 1);
				lastMoveDeletedRow = false;
				break;
			case 4: {
				if (lastMoveDeletedRow) {
					points += 1200 * (stage + 1);
				}
				else {
					lastMoveDeletedRow = true;
					points += 800 * (stage + 1);
				}
				break;
			}
			}

			tempSpeedUp = false;

			delete brick;
			brick = new Brick(nextBrickType);
			nextBrickType = rand() % 7;

			delete nextBrick;
			nextBrick = new Brick(nextBrickType);
		}
	}
	else {
		brick = new Brick(nextBrickType);
		nextBrickType = rand() % 7;
		nextBrick = new Brick(nextBrickType);
	}
}

void Game::render(SDL_Surface *screen, SDL_Surface *brickParts[]) {
	for (int iy = 0; iy < map.size.y; iy++)
		for (int ix = 0; ix < map.size.x; ix++)
			if (map.map[iy][ix] >= 1 && map.map[iy][ix] <= 7)
				DrawSurface(screen, brickParts[map.map[iy][ix] - 1], 36 + ix * BRICK_SIZE, 36 + iy * BRICK_SIZE + PADDING_TOP);

	if (brick != nullptr)
		for (int iy = 0; iy < brick->map.size.y; iy++)
			for (int ix = 0; ix < brick->map.size.x; ix++)
				if(brick->map.map[iy][ix] != 0)
					DrawSurface(screen, brickParts[brick->brickType->type - 1], (brick->position.x + brick->brickType->getVectorFromCenter(V2D(ix, iy)).x + 1) * BRICK_SIZE + 12, (brick->position.y + brick->brickType->getVectorFromCenter(V2D(ix, iy)).y + 1) * BRICK_SIZE + PADDING_TOP + 12);


	if (nextBrick != nullptr)
		for (int iy = 0; iy < nextBrick->map.size.y; iy++)
			for (int ix = 0; ix < nextBrick->map.size.x; ix++)
				if (nextBrick->map.map[iy][ix] != 0)
					DrawSurface(screen, brickParts[nextBrick->brickType->type - 1], (16 + nextBrick->brickType->getVectorFromCenter(V2D(ix, iy)).x) * BRICK_SIZE + 12, (8 + nextBrick->brickType->getVectorFromCenter(V2D(ix, iy)).y) * BRICK_SIZE + PADDING_TOP);
	
	printGameTime(screen);
	if (pause) {
		pauseBox->render();
	}
}

void Game::printGameTime(SDL_Surface *screen) const
{
	Text str;
	int minutes = gameTime / 60;
	int seconds = int(gameTime) % 60;
	str = T((minutes < 10) ? "0" : "") + T(minutes) + ":" + T((seconds < 10) ? "0" : "") + T(seconds);
	DrawString(screen, 14.75 * BRICK_SIZE, 15 * BRICK_SIZE, "Game Time:", Window::getInstance()->charset);
	DrawString(screen, 15.25 * BRICK_SIZE, 15.75 * BRICK_SIZE, str, Window::getInstance()->charset);
}

void Game::handleKeys(SDL_Keycode key) {
	switch (key) {
	case SDLK_s: {
		if (stage < 10) {
			stage++;
			lastStageUp = 0;
		}
		break;
	}
	case SDLK_F3:
		saveGame();
		break;
	case SDLK_l:
	case SDLK_F4:
		loadGame();
		break;
	case SDLK_p:
		pause = !pause;
		break;
	case SDLK_DOWN:
		tempSpeedUp = true;
		break;
	case SDLK_UP:
	case SDLK_SPACE:
		brick->toRotate = true;
		break;
	case SDLK_LEFT:
		lastPressedKey = LEFT;
		break;
	case SDLK_RIGHT:
		lastPressedKey = RIGHT;
		break;
	}
}

void Game::loadGame() {
	FILE *f = fopen("save.dat", "rb");

	GameSave *save = new GameSave;

	fread(save, sizeof(*save), 1, f);

	gameTime = save->gameTime;
	points = save->points;
	stage = save->stage;
	lastMoveDeletedRow = save->lastMoveDeletedRow;
	lastStageUp = save->lastStageUp;
	nextBrickType = save->nextBrickType;
	saves = save->saves;

	if (nextBrick != nullptr)
		delete nextBrick;
	nextBrick = new Brick(nextBrickType);

	if (brick != nullptr)
		delete brick;
	brick = new Brick(save->brickType - 1);
	brick->lastPosition = V2D(save->brickPosX, save->brickPosY);
	brick->nextPosition = V2D(save->brickPosX, save->brickPosY + 1);

	for (int iy = 0; iy < map.size.y; iy++)
		for (int ix = 0; ix < map.size.x; ix++)
			map.map[iy][ix] = save->map[iy][ix];

	pause = false;

	fclose(f);
}

void Game::saveGame() {
	if (saves >= 3)
		return;

	FILE *f = fopen("save.dat", "wb");

	if (f == nullptr)
		return;

	GameSave save;
	save.brickPosX = brick->position.x;
	save.brickPosY = brick->position.y;
	save.brickType = brick->brickType->type;
	save.gameTime = gameTime;
	save.lastMoveDeletedRow = lastMoveDeletedRow;
	save.lastStageUp = lastStageUp;
	save.nextBrickType = nextBrickType;
	save.points = points;
	save.stage = stage;
	save.saves = saves;

	for (int iy = 0; iy < map.size.y; iy++)
		for (int ix = 0; ix < map.size.x; ix++)
			save.map[iy][ix] = map.map[iy][ix];

	fwrite(&save, sizeof(save), 1, f);
	
	fclose(f);

	saves++;
}