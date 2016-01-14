#include "main.h"

Ranking::Ranking() {
	for (int i = 0; i < RANKING_PLACES; i++) {
		ranking[i] = RankingRow();
		ranking[i].points = 0;
	}
	loadFromFile();
}

Ranking::~Ranking() {
}

int Ranking::getPlace(int points) {
	for (int i = 0; i < RANKING_PLACES; i++) {
		if (points > ranking[i].points)
			return i + 1;
	}
	return -1;
}

void Ranking::addToRanking(RankingRow row) {
	int place = getPlace(row.points);
	if (place != -1) {
		for (int i = RANKING_PLACES - 1; i >= place; i--) {
			ranking[i] = ranking[i - 1];
		}
		ranking[place - 1] = row;
	}
}

void Ranking::saveToFile() {
	FILE *f = fopen("ranking.dat", "wb");

	if (f == nullptr)
		return;

	fwrite(this, sizeof(*this), 1, f);

	fclose(f);
}

void Ranking::loadFromFile() {
	FILE *f = fopen("ranking.dat", "rb");

	if (f == nullptr)
		return;

	Ranking *rank = new Ranking;

	fread(this, sizeof(*this), 1, f);

	fclose(f);
}

void Ranking::fakeData() {
	int i = 0;
	Text imiona[10] = { "Dominik", "Ola", "Wojtek", "Marcin", "Paulina", "Kasia", "Magda", "Tadek", "Alan", "Janek" };

	while (i++ < RANKING_PLACES) {
		RankingRow row = RankingRow();
		strcpy(row.name, imiona[rand() % 10]);
		row.points = (rand() % 40) * 100;
		addToRanking(row);
	}

	saveToFile();
}