#include <stdlib.h>
#include <string.h>
#include "text.h"

Text::Text() {
	string = (char*)malloc(sizeof(char));
	string[0] = '\0';
}


Text::Text(const char* str) {
	int size = strlen(str);
	string = (char*)malloc(sizeof(char) * (size + 1));
	strcpy(string, str);
}

Text::Text(char* str) {
	int size = strlen(str);
	string = (char*)malloc(sizeof(char) * (size + 1));
	strcpy(string, str);
}

Text::Text(char c) {
	string = (char*)malloc(sizeof(char) * 2);
	string[0] = c;
	string[1] = '\0';
}

Text::Text(int i) {
	string = (char*)malloc(sizeof(int));
	itoa(i, string, 10);
}

Text::Text(const Text &str) {
	int size = strlen(str.string);
	string = (char*)malloc(sizeof(char) * (size + 1));
	strcpy(string, str.string);
}

Text::~Text() {
	free(string);
}

void Text::clear() {
	free(string);
	string = (char*)malloc(sizeof(char));
	string[0] = 0;
}

void Text::clearLastChar() {
	if (length() == 0)
		return;

	string[length() - 1] = '\0';
	cpy(string);
}

int Text::length() const
{
	return strlen(string);
}

bool Text::isEmpty() const
{
	return length() == 0;
}

void Text::cpy(char* str) {
	free(string);
	int size = strlen(str);
	string = (char*)malloc(sizeof(char) * (size + 1));
	strcpy(string, str);
}

void Text::cat(char* str) {
	int size = length() + strlen(str);
	char* tmp = (char*)malloc(sizeof(char) * (size + 1));

	strcpy(tmp, string);
	strcat(tmp, str);

	free(string);
	string = tmp;
}