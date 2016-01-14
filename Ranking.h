#ifndef RANKING_H
#define RANKING_H

struct RankingRow {
	char name[20];
	int points;
};

class Ranking {
public:
	RankingRow ranking[RANKING_PLACES];

	Ranking();
	~Ranking();

	int getPlace(int points);
	void addToRanking(RankingRow row);
	void saveToFile();
	void loadFromFile();
	void fakeData();
};

struct RankingSave {
	RankingRow ranking[RANKING_PLACES];
};



#endif