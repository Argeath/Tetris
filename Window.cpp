#include "main.h"

Window* Window::instance = nullptr;

Window::Window() : dialogBox(nullptr) {
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	for (int i = 0; i < 7; i++)
		brickParts[i] = nullptr;

	temp = Text();
	game = new Game();
	ranking = new Ranking();
	for (int i = 1; i <= 7; i++)
		brickTypes[i-1] = new BrickType(i);

	#ifdef DEBUG_MODE
	debug = new Debug();
	#endif
}

Window::~Window() {
	delete game;
	delete ranking;

	if (dialogBox != nullptr)
		delete dialogBox;

	for (int i = 0; i <= 6; i++)
		delete brickTypes[i];

	#ifdef DEBUG_MODE
	delete debug;
	#endif

	game = nullptr;
	instance = nullptr;
}

int Window::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Tetris - Dominik Kinal 160589");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	Color::init(screen);

	// wczytanie obrazka cs8x8.bmp
	charset = LoadSurface("./cs8x8.bmp");
	if (charset == nullptr) {
		quitWindow();
		return 1;
	}

	SDL_SetColorKey(charset, true, 0x000000);

	for (int i = 0; i < 7; i++) {
		brickParts[i] = LoadSurface(T("./k") + T(i + 1) + ".bmp");
		if (brickParts[i] == nullptr) {
			quitWindow();
			return 1;
		}
	}

	tetrisLogo = LoadSurface("./tetris-logo.bmp");
	if (tetrisLogo == nullptr) {
		quitWindow();
		return 1;
	}

	t1 = SDL_GetTicks();

	return 0;
}

void Window::loop() {

	char text[128];

	while (!quit) {
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		SDL_FillRect(screen, nullptr, Color::czarny);

		fpsTimer += delta;
		if (fpsTimer > 0.3) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.3;
		}

		game->update(delta);

		if (game != nullptr) {
			game->render(screen, brickParts);
			// tekst informacyjny
			DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, Color::czarny, Color::czarny);
			sprintf(text, "Tetris, FPS: %.0lf", fps);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
			sprintf(text, "Esc - wyjscie, \032\033 - przesuwanie, \031 - przyspieszenie");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

			renderWindow();

			#ifdef DEBUG_MODE
			debug->printCollision();
			#endif
		}

		if(dialogBox != nullptr)
			dialogBox->render();

		SDL_UpdateTexture(scrtex, nullptr, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		pollEvents();
		
		frames++;
	}
}

void Window::renderWindow() {
	for (int i = 0; i < TETRIS_WIDTH; i++) {
		DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, BRICK_SIZE / 2 + PADDING_TOP);
		DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, (TETRIS_HEIGHT - 1) * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);

		if (i > 10 && i < 20) {
			DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, 4 * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
			DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, 12 * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
			DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, 15 * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
			DrawSurface(screen, brickParts[1], i * BRICK_SIZE + BRICK_SIZE / 2, 18 * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
		}
	}

	for (int i = 1; i <= TETRIS_HEIGHT; i++) {
		DrawSurface(screen, brickParts[1], BRICK_SIZE / 2, i * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
		DrawSurface(screen, brickParts[1], (TETRIS_WIDTH - 1)  * BRICK_SIZE + BRICK_SIZE / 2, i * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);

		DrawSurface(screen, brickParts[1], 11 * BRICK_SIZE + BRICK_SIZE / 2, i * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
		DrawSurface(screen, brickParts[1], 20 * BRICK_SIZE + BRICK_SIZE / 2, i * BRICK_SIZE + BRICK_SIZE / 2 + PADDING_TOP);
	}

	DrawSurface(screen, tetrisLogo, 16 * BRICK_SIZE, 2.5 * BRICK_SIZE + PADDING_TOP);

	DrawString(screen, 14.5 * BRICK_SIZE, 5 * BRICK_SIZE + 6 + PADDING_TOP, "Next Brick:", charset);
	DrawString(screen, 15 * BRICK_SIZE, 16 * BRICK_SIZE + 9 + PADDING_TOP, "Points:", charset);
	sprintf(temp, "%d", game->points);

	double pointsWidth = 15.4;
	if (game->points >= 1000)
		pointsWidth = 15;
	else if (game->points >= 100)
		pointsWidth = 15.15;
	else if (game->points >= 10)
		pointsWidth = 15.25;

	DrawString(screen, pointsWidth * BRICK_SIZE + 8, 17 * BRICK_SIZE + 3 + PADDING_TOP, temp, charset);
	DrawString(screen, 14.8 * BRICK_SIZE + 8, 19 * BRICK_SIZE + 9 + PADDING_TOP, "Stage:", charset);
	sprintf(temp, "%d/%d", game->stage, MAX_STAGE);
	DrawString(screen, 15 * BRICK_SIZE + 8, 20 * BRICK_SIZE + 3 + PADDING_TOP, temp, charset);
	DrawString(screen, 24 * BRICK_SIZE, BRICK_SIZE + 6 + PADDING_TOP, "Ranking:", charset);

	for (int i = 0; i < RANKING_PLACES; i++) {
		if (ranking->ranking[i].points > 0) {
			DrawString(screen, 21.25 * BRICK_SIZE, (i + 1) * 12 + BRICK_SIZE + 6 + PADDING_TOP, T(i + 1) + ". " + ranking->ranking[i].name, charset);
			DrawString(screen, 27 * BRICK_SIZE, (i + 1) * 12 + BRICK_SIZE + 6 + PADDING_TOP, T(ranking->ranking[i].points), charset);
		}

	}
}

void finishGameSuccess() {
	delete Window::getInstance()->game;
	Window::getInstance()->game = new Game();

	delete Window::getInstance()->dialogBox;
	Window::getInstance()->dialogBox = nullptr;
}

void finishGameFailure() {
	delete Window::getInstance()->dialogBox;
	Window::getInstance()->dialogBox = nullptr;

	Window::getInstance()->quitWindow();
}

void finishGameLoadSave() {
	Window::getInstance()->game->loadGame();

	delete Window::getInstance()->dialogBox;
	Window::getInstance()->dialogBox = nullptr;
}

void rankingSuccess() {

	Text name = static_cast<InputDialogBox*>(Window::getInstance()->dialogBox)->inputText;

	RankingRow row = RankingRow();
	strcpy(row.name, name);
	row.points = Window::getInstance()->game->points;

	Window::getInstance()->ranking->addToRanking(row);
	Window::getInstance()->ranking->saveToFile();

	delete Window::getInstance()->dialogBox;
	Window::getInstance()->showWindow(FinishGame);
}

void rankingFailure() {

	delete Window::getInstance()->dialogBox;
	Window::getInstance()->showWindow(FinishGame);
}

void Window::showWindow(DialogBoxType type) {
	switch (type) {
	case FinishGame: {
		InfoDialogBox* box = new InfoDialogBox();
		box->answer = "Czy chcesz zagrac ponownie?";
		box->size.x = 300;

		box->yesKey = SDLK_t;
		box->yesText = "Tak";
		box->noKey = SDLK_n;
		box->noText = "Nie";
		box->thirdKey = SDLK_l;
		box->thirdText = "Wczytaj";

		box->successCallback = &finishGameSuccess;
		box->failureCallback = &finishGameFailure;
		box->thirdCallback = &finishGameLoadSave;

		dialogBox = box;
		break;
	}
	case InputNameToRanking: {

		InputDialogBox* box = new InputDialogBox();
		box->answer = T("Zdobyles ") + T(game->points) + " punktow i osiagnales " + T(ranking->getPlace(game->points)) + " miejsce. Podaj imie:";
		box->size.x = 500;
		box->size.y = 80;

		box->successCallback = &rankingSuccess;
		box->failureCallback = &rankingFailure;

		dialogBox = box;
		break;
	}
	}
}

void Window::pollEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN: {
			if (dialogBox != nullptr) dialogBox->handleKeys(event.key.keysym.sym);
			else if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
			else if (event.key.keysym.sym == SDLK_n) {
				delete game;
				game = new Game();
			}
			else if (event.key.keysym.sym == SDLK_f) {
				ranking->fakeData();
				break;
			}
			else game->handleKeys(event.key.keysym.sym);
			break;
		}
		case SDL_KEYUP:
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}
}

void Window::quitWindow() {
	quit = true;

	for (int i = 0; i < 7; i++)
		if (brickParts[i] != nullptr)
			SDL_FreeSurface(brickParts[i]);

	if(charset != nullptr)
		SDL_FreeSurface(charset);

	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}