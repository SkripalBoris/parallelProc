#include <map>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
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
 * Количество созданных потоков
 */
int createThreadsCounter;
/*
 * Number of finish threads
 */
int finishThreadsCounter;
/*
 * Mutex
 */
pthread_mutex_t lock;
/*
 *	Frame size
 */
long frameSize;
/*
 * Number of readed words
 */
long lSize;
/*
 * Number of threads
 */
int threadNumber;

/**********************************************************************/
/*
 * Add new words into global map and vector
 */
void addNewMapAndVector(std::map<std::string, int> *newCounterMap,
	std::vector<std::string> *newKeysVector);

/*
 * Count words includes in text string
 */
void *countWoldIncludes(void *arg);

/*
 * Generation text frames
 */
void generateWordsFreq(const char *inputString);

void printResult();

/**********************************************************************/
int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Please write filename in parameter\n");
		return 1;
	}

	wordsMap = new std::map<std::string, int>;
	wordsVector = new std::vector<std::string>;
	createThreadsCounter = 0;
	finishThreadsCounter = 0;

	threadNumber = atoi(argv[1]);
	
	FILE *file = fopen(argv[2], "r");

	if (file == NULL) {
		perror("File error");
		return 2;
	}

	fseek(file, 0, SEEK_END);
	lSize = (size_t)ftell(file);
	frameSize = lSize / threadNumber + 1;
	rewind(file);

	char *buffer = (char *)malloc((size_t)lSize);
	fread(buffer, 1, lSize, file);

	// init mutex
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("\n mutex init failed\n");
		return 1;
	}

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

	// Delete mutex
	pthread_mutex_destroy(&lock);
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

void *countWoldIncludes(void *arg) {
	char *workCharArr = (char *)arg;

	std::map<std::string, int> *wMap = new std::map<std::string, int>;
	std::vector<std::string> *wVector = new std::vector<std::string>;

	char *saveptr;

	char *pch = strtok_r(workCharArr, " ,.: \"!?()\n", &saveptr);

	int i = 0;
	while (pch != NULL) {
		i++;
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
		pch = strtok_r(NULL, " ,.: \"!?()\n", &saveptr);
	}

	// on mutex
	pthread_mutex_lock(&lock);
	addNewMapAndVector(wMap, wVector);
	finishThreadsCounter++;
	// off mutex
	pthread_mutex_unlock(&lock);

	delete (wMap);
	delete (wVector);
	delete (workCharArr);
	delete (pch);
}

void generateWordsFreq(const char *inputString) {
	if (inputString == NULL)
		return;

	long counterFrom = 0;
	long counterTo = 0;

	int i = 0;
	while (counterTo < (lSize - 1)) {
		if (counterTo == (lSize - 1))
			break;

		counterTo += frameSize;
		while (inputString[counterTo] != ' ' && counterTo < lSize)
			counterTo++;

		if (counterTo > lSize)
			counterTo = lSize - 1;

		char *workArray = new char[counterTo - counterFrom + 1];

		for (int i = 0; i < counterTo - counterFrom + 1; i++) {
			workArray[i] = 0;
		}
		
		strncpy(workArray, inputString + counterFrom, counterTo - counterFrom);
		counterFrom = counterTo + 1;

		pthread_t thread;
		// create threads
		createThreadsCounter++;
		pthread_create(&thread, NULL, countWoldIncludes, (void *)workArray);
		// detach threads
		pthread_detach(thread);
	}

	while (finishThreadsCounter < createThreadsCounter)
		usleep(10);
}

void printResult() {
	for (std::vector<std::string>::iterator it = wordsVector->begin(); it != wordsVector->end(); ++it) {
		std::string bufName = *it;
		int bufCount = wordsMap->at(*it);
		printf("%s %d\n", bufName.c_str(), bufCount);
	}
}
