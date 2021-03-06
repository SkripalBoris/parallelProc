#include <map>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

/**********************************************************************/

/*
 * Global map with words
 */
std::map<std::string, int> *wordsMap;
/*
 * Global vector with words
 */
std::vector<std::string> *wordsVector;
/*
 * Text frame size
 */
long frameSize;
/*
 * Number of readed words
 */
long lSize;

/**********************************************************************/

/*
 * Add new words into global map and vector
 */
void addNewMapAndVector(std::map<std::string, int> *newCounterMap,
	std::vector<std::string> *newKeysVector);

/*
 * Count words includes in text string
 */
void countWoldIncludes(char *workCharArr);

/*
 * Generation text frames
 */
void generateWordsFreq(const char *inputString);

void printResult();

/**********************************************************************/

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Please write filename in parameter\n");
		return 1;
	}

	wordsMap = new std::map<std::string, int>;
	wordsVector = new std::vector<std::string>;

	FILE *file = fopen(argv[1], "r");

	if (file == NULL) {
		perror("File error");
		return 2;
	}

	fseek(file, 0, SEEK_END);
	lSize = (size_t)ftell(file);
	frameSize = lSize;
	rewind(file);

	char *buffer = (char *)malloc((size_t)lSize);
	fread(buffer, 1, lSize, file);

	struct timeval tvStart;
	struct timeval tvFinish;

	// Get time of start programm
	gettimeofday(&tvStart, NULL);
	generateWordsFreq(buffer);

	// Get time of finish programm
	gettimeofday(&tvFinish, NULL);
	long int msStart = tvStart.tv_sec * 1000 + tvStart.tv_usec / 1000;
	long int msFinish = tvFinish.tv_sec * 1000 + tvFinish.tv_usec / 1000;

	printf("%ld\n", msFinish - msStart);

	printResult();

	fclose(file);
	delete (buffer);
	delete (wordsVector);
	delete (wordsMap);
	return 0;
}

void addNewMapAndVector(std::map<std::string, int> *newCounterMap,
	std::vector<std::string> *newKeysVector) {
	if (wordsMap == NULL) {
		wordsMap = new std::map<std::string, int>;
		wordsVector = new std::vector<std::string>;

		for (std::vector<std::string>::iterator it = newKeysVector->begin(); it != newKeysVector->end(); ++it) {
			int bufCountB = newCounterMap->at(*it);
			wordsMap->insert(std::pair<std::string, int>(*it, bufCountB));
			wordsVector->push_back(*it);
		}
		return;
	}

	if (newCounterMap == NULL)
		return;

	if (newKeysVector == NULL)
		return;

	for (std::vector<std::string>::iterator it = newKeysVector->begin(); it != newKeysVector->end(); ++it) {
		if (wordsMap->count(*it)) {
			int bufCountA = wordsMap->at(*it);
			int bufCountB = newCounterMap->at(*it);
			std::map<std::string, int>::iterator itMap;
			itMap = wordsMap->find(*it);
			wordsMap->erase(itMap);
			wordsMap->insert(std::pair<std::string, int>(*it, bufCountA + bufCountB));
		}
		else {
			int bufCountB = newCounterMap->at(*it);
			wordsMap->insert(std::pair<std::string, int>(*it, bufCountB));
			wordsVector->push_back(*it);
		}
	}
}

void countWoldIncludes(char *workCharArr) {

	std::map<std::string, int> *wMap = new std::map<std::string, int>;
	std::vector<std::string> *wVector = new std::vector<std::string>;

	char *pch = std::strtok(workCharArr, " ,.: \"!?()\n\t");

	while (pch != NULL) {
		if (wMap->count(pch)) {
			int bufCount = wMap->at(pch);
			std::map<std::string, int>::iterator itMap = wMap->find(pch);
			wMap->erase(itMap);
			wMap->insert(std::pair<std::string, int>(pch, ++bufCount));
		}
		else {
			wMap->insert(std::pair<std::string, int>(pch, 1));
			wVector->push_back(pch);
		}
		pch = strtok(NULL, " ,.: \"!?()\n\t");
	}

	addNewMapAndVector(wMap, wVector);

	delete (wMap);
	delete (wVector);
}

void generateWordsFreq(const char *inputString) {
	if (inputString == NULL)
		return;

	long counterFrom = 0;
	long counterTo = 0;

	while (counterTo < (lSize - 1)) {

		counterTo += frameSize;
		while (inputString[counterTo] != ' ' && counterTo < lSize)
			counterTo++;

		if (counterTo > lSize)
			counterTo = lSize - 1;

		char *workArray = new char[counterTo - counterFrom + 1];

		for (int i = 0; i < counterTo - counterFrom + 1; i++)
			workArray[i] = 0;

		strncpy(workArray, inputString + counterFrom, counterTo - counterFrom);
		counterFrom = counterTo + 1;
		countWoldIncludes(workArray);
		delete (workArray);
	}
}

void printResult() {
	for (std::vector<std::string>::iterator it = wordsVector->begin(); it != wordsVector->end(); ++it) {
		std::string bufName = *it;
		int bufCount = wordsMap->at(*it);
		printf("%s %d\n", bufName.c_str(), bufCount);
	}
}
