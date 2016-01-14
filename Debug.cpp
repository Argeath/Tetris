#include "main.h"

#ifdef DEBUG_MODE
void Debug::printCollision() {
	DrawString(Window::getInstance()->screen, collision->position.x, collision->position.y, collision->text, Window::getInstance()->charset);
}
#endif