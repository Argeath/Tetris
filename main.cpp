#include "main.h"

int main(int argc, char **argv) {

	srand(time(nullptr));

	Window* window = Window::getInstance();
	
	if (window->init() == 1)
		return 1;

	window->loop();
	window->quitWindow();

	delete window;
	return 0;
	};
