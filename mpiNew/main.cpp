#include <map>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>
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

int rank, size;

MPI::Status status;

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

/***********************************************************************/
int main(int argc, char *argv[]) {
	if (argc < 2) {
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
	rewind(file);

	char *buffer = (char *)malloc((size_t)lSize);
	fread(buffer, 1, lSize, file);

	struct timeval tvStart;
	struct timeval tvFinish;

	// Get time of start programm
	gettimeofday(&tvStart, NULL);

	MPI_Init(&argc, &argv);      /* starts MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &size);        /* get number of processes */

	frameSize = lSize / (size - 1);

	generateWordsFreq(buffer);
	if (rank == 0) {
		// Get time of finish programm
		gettimeofday(&tvFinish, NULL);
		long int msStart = tvStart.tv_sec * 1000 + tvStart.tv_usec / 1000;
		long int msFinish = tvFinish.tv_sec * 1000 + tvFinish.tv_usec / 1000;

		//printf("%ld\n", msFinish - msStart);

		printResult();
	}

	MPI_Finalize();

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
	
	struct timeval tvStart;
		struct timeval tvFinish;

	// Get time of start programm
	gettimeofday(&tvStart, NULL);
	// Get time of finish programm

	std::map<std::string, int> *wMap = new std::map<std::string, int>;
	std::vector<std::string> *wVector = new std::vector<std::string>;

	char *pch = std::strtok(workCharArr, " ,:. \"!?()\n");

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
		pch = strtok(NULL, " ,:. \"!?()\n");
	}

	std::string sendString = "";
	for (std::vector<std::string>::iterator it = wVector->begin(); it != wVector->end(); ++it) {
		std::string bufString = *it;
		char bufNumber[20];
		int bufNum = wMap->at(*it);
		sprintf(bufNumber, "%d", bufNum);
		std::string intString(bufNumber);
		sendString = sendString + ' ' + bufString + ' ' + intString;
	}
	
	//TEST
	gettimeofday(&tvFinish, NULL);
		long int msStart = tvStart.tv_sec * 1000 + tvStart.tv_usec / 1000;
		long int msFinish = tvFinish.tv_sec * 1000 + tvFinish.tv_usec / 1000;
		
		MPI::COMM_WORLD.Send(&msStart, 1, MPI::LONG, 0, 3);
		MPI::COMM_WORLD.Send(&msFinish, 1, MPI::LONG, 0, 4);

	int string_lenght = sendString.size() + 1;
	MPI::COMM_WORLD.Send(&string_lenght, 1, MPI::INT, 0, 0);
	MPI::COMM_WORLD.Send(sendString.c_str(), string_lenght, MPI::CHAR, 0, 1);

	delete (wMap);
	delete (wVector);
}

void generateWordsFreq(const char *inputString) {
	
	if (inputString == NULL)
		return;

	long counterFrom = 0;
	long counterTo = 0;

	if (rank == 0) {
		int i = 1;
		while (i < size) {

			counterTo += frameSize;
			while (inputString[counterTo] != ' ' && counterTo < lSize)
				counterTo++;

			if (counterTo > lSize)
				counterTo = lSize - 1;

			char *workArray = new char[counterTo - counterFrom + 1];

			for (int i = 0; i < counterTo - counterFrom + 1; i++)
				workArray[i] = 0;

			strncpy(workArray, inputString + counterFrom, counterTo - counterFrom);
			
			//printf("start send string %d\n",i);

			int string_lenght = counterTo - counterFrom + 1;
			MPI::COMM_WORLD.Send(&string_lenght, 1, MPI::INT, i, 0);
			MPI::COMM_WORLD.Send(workArray, string_lenght, MPI::CHAR, i, 1);
			
			//printf("finish send string %d\n",i);

			counterFrom = counterTo + 1;
			i++;

			delete (workArray);
		}
		
		for(int k=1;k<size;k++)
		{
			int string_lenght = 0;
			//printf("start send %d\n",k);
			MPI::COMM_WORLD.Send(&string_lenght, 1, MPI::INT, k, 0);
			//printf("finish send %d\n",k);
		}
			
	}
	else {
		int frameLenght;
		MPI::COMM_WORLD.Recv(&frameLenght, 1, MPI::INT, 0, 0, status);
		char *i_buffer = new char[frameLenght];
		MPI::COMM_WORLD.Recv(i_buffer, frameLenght, MPI::CHAR, 0, 1, status);
		int count = status.Get_count(MPI::CHAR);
		
		//printf("start recv %d\n",rank);
		MPI::COMM_WORLD.Recv(&frameLenght, 1, MPI::INT, 0, 0, status);
		//printf("finish recv %d\n",rank);
		
		countWoldIncludes(i_buffer);
		
		//printf("%d %ld %ld\n",rank,msStart,msFinish);
		
		delete(i_buffer);
	}

	if (rank == 0) {
		long int *timeStartArr = new long int [size - 1];
		long int *timeFinishArr = new long int [size - 1];
		int counter = 0;
		
		for (int i = 1; i < size; i++) {
			std::map<std::string, int> *wMap = new std::map<std::string, int>;
			std::vector<std::string> *wVector = new std::vector<std::string>;

			MPI::COMM_WORLD.Recv(&timeStartArr[i-1], 1, MPI::INT, i, 3, status);
			MPI::COMM_WORLD.Recv(&timeFinishArr[i-1], 1, MPI::INT, i, 4, status);

			// Get text string
			int frameLenght;
			MPI::COMM_WORLD.Recv(&frameLenght, 1, MPI::INT, i, 0, status);
			char *i_buffer = new char[frameLenght];
			MPI::COMM_WORLD.Recv(i_buffer, frameSize, MPI::CHAR, i, 1, status);

			char *pch = std::strtok(i_buffer, " ,. \"!?()\n");
			while (pch != NULL) {
				std::string b(pch);
				wVector->push_back(b);
				pch = strtok(NULL, " ,. \"!?()\n");

				std::string buf = wVector->back();
				wMap->insert(std::pair<std::string, int>(buf, atoi(pch)));
				pch = strtok(NULL, " ,. \"!?()\n");
			}

			addNewMapAndVector(wMap, wVector);

			delete(i_buffer);
			delete(wMap);
			delete(wVector);
		}
		
		long int timeStartBuf = timeStartArr[0];
		long int timeFinishBuf = timeFinishArr[0];
		
		for (int i = 1; i < size - 1 ; i++) {
			if(timeStartBuf > timeStartArr[i])
				timeStartBuf = timeStartArr[i];
			if(timeFinishBuf < timeFinishArr[i])
				timeFinishBuf = timeFinishArr[i];
		}
		
		printf("%ld\n",timeFinishBuf - timeStartBuf);
	}
}

void printResult() {
	for (std::vector<std::string>::iterator it = wordsVector->begin(); it != wordsVector->end(); ++it) {
		std::string bufName = *it;
		int bufCount = wordsMap->at(*it);
		printf("%s %d\n", bufName.c_str(), bufCount);
	}
}
