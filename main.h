/*
Window(singleton):
 - uruchomienie SDL
 - wczytanie grafik
 - pêtla z gr¹
 - obs³uga klawiatury

Game (niestatyczna, mo¿liwoœæ zapisu i wczytania stanu):
 - posiada: 
   * tablice z klockami na danych wspolrzednych(1 klocek - 1 wsp.)
   * punkty, czas
   * klocek poruszajacy sie

 - oblicza czas ( + pauza)
 - sprawdza kolizje
 - usuwa pelne wiersze
 - dolicza punkty
 - obsluga klawiszy
 - zapisanie / wczytanie gry

Brick (klocek poruszaj¹cy siê. Gdy spadnie, rozdziela siê na poszczególne "klocuszki"):
 - posiada: 
	* pozycja (srodek)
	* BrickType (rozmiar, grafika, mapa pól)
	* rotacja
	* zrotowana mapa pól
 - wykrywanie kolizji
 - opadanie / poruszanie / rotacja
 - sprawdzanie kolizji

 "Klocuszek" - brickPart -> int(0 - pusty, 1-7 - kolory)

 Pomocnicze:
  - mapa 2D
  - wektor 2D
  - Text

	######################################
	#          #            # 1. ABC 100 #
	#          #   TETRIS   # 2. GHI  70 #
	#          #            # 3. DEF  50 #
	#          ##############            #
	#          # Next Brick #            #
	#          #            #            #
	#          #            #            #
	#          #            #            #
	#          ##############            #
	#          #   Points   #            #
	#          #     0      #            #
	#          ##############            #
	#          # Stage 1/10 #            #
	#          ##############            #
	#          #            #            #
	#          #            #            #
	#          #            #            #
	#          #            #            #
	#          #            #            #
	#          #            #            #
	######################################

	Check-list:
	a) V
	b) V
	c) V
	e) V
	f) V
	g) V
	h) V
	i) V
	j) V
	k) V
	l) V
	m) V
	n) V
*/

enum EVENT_KEY {
	NONE, LEFT, RIGHT
};

#define _USE_MATH_DEFINES
//#define DEBUG_MODE

#define TETRIS_WIDTH	30
#define TETRIS_HEIGHT	22
#define BRICK_SIZE		25
#define PADDING_TOP		40
#define SCREEN_WIDTH	TETRIS_WIDTH * BRICK_SIZE
#define SCREEN_HEIGHT	TETRIS_HEIGHT * BRICK_SIZE + PADDING_TOP
#define MAX_STAGE		10
#define LOWEST_SPEED	50 // 100 - 1 second
#define SPEED_PER_STAGE	4.5
#define SPEED_LIMIT		2
#define STAGE_UP_TIME	30 // 1 - 1 second
#define RANKING_PLACES	15

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Vector2D.h"
#include "text.h"
#include "mapArray.h"
#include "Ranking.h"
#include "SDL.h"

#ifdef DEBUG_MODE
#include "Debug.h"
#endif

#include "Color.h"
#include "DialogBox.h"
#include "BrickType.h"
#include "Brick.h"
#include "Game.h"
#include "Window.h"