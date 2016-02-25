#include <map>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>

/**********************************************************************************************************************/
#define WORD_ARRAY_FRAME_SIZE 70
/**********************************************************************************************************************/
/*
 * Глобальная мапа с количеством повторений слов
 */
std::map<std::string, int> *wordsMap;
/*
 * Глобальный вектор со словами
 */
std::vector<std::string> *wordsVector;

/**********************************************************************************************************************/
/*
 * Функция добавления новых значений в глобальные структуры
 */
void addNewMapAndVector(std::map<std::string, int> *newCounterMap,
                        std::vector<std::string> *newKeysVector);

/*
 * Функция подсчета слов в строке
 * В качестве параметра передается указатель на массив символов
 */
void countWoldIncludes(char *workCharArr);

/*
 * Функция генерации потоков
 */
void generateWordsFreq(std::string inputString);

/*
 * Печать результатов
 */
void printResult();

/**********************************************************************************************************************/
int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("Please write filename in parameter\n");
        return 1;
    }

    wordsMap = new std::map<std::string, int>;
    wordsVector = new std::vector<std::string>;

    //Открытие файла
    FILE *file = fopen(argv[1],"r");

    if (file == NULL) {
        perror("File error");
        return 2;
    }

    fseek(file, 0, SEEK_END);
    long lSize = (size_t) ftell(file);
    rewind(file);

    char* buffer = new char[lSize];
    fread(buffer, 1, lSize, file);

    std::string bufStr(buffer);

    // Инициализация счетчика
    struct timeval tvStart;
    struct timeval tvFinish;

    // Получение времени начала работы
    gettimeofday(&tvStart,NULL);

    generateWordsFreq(bufStr);
    
    // Получение времени конца работы
    gettimeofday(&tvFinish,NULL);
    long int msStart = tvStart.tv_sec * 1000 + tvStart.tv_usec / 1000;
    long int msFinish = tvFinish.tv_sec * 1000 + tvFinish.tv_usec / 1000;
    
    printf("%ld\n",msFinish - msStart);

    printResult();

    fclose(file);
    delete(buffer);
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
        } else {
            int bufCountB = newCounterMap->at(*it);
            wordsMap->insert(std::pair<std::string, int>(*it, bufCountB));
            wordsVector->push_back(*it);
        }
    }
}

void countWoldIncludes(char *workCharArr) {

    std::map<std::string, int> *wMap = new std::map<std::string, int>;
    std::vector<std::string> *wVector = new std::vector<std::string>;

    char *pch = std::strtok(workCharArr, " ,. \"!?()\n");

    while (pch != NULL) {
        if (wMap->count(pch)) {
            int bufCount = wMap->at(pch);
            std::map<std::string, int>::iterator itMap = wMap->begin();
            itMap = wMap->find(pch);
            wMap->erase(itMap);
            wMap->insert(std::pair<std::string, int>(pch, ++bufCount));
        } else {
            wMap->insert(std::pair<std::string, int>(pch, 1));
            wVector->push_back(pch);
        }
        pch = strtok(NULL, " ,. \"!?()\n");
    }

    addNewMapAndVector(wMap, wVector);

    delete (wMap);
    delete (wVector);
}

void generateWordsFreq(std::string inputString) {
    std::vector<std::string> *stringVector = new std::vector<std::string>;
    std::string workString = inputString;
    if (inputString.empty())
        return;

    while (true) {
        int counterFrom = 0;
        int counterTo = 0;
        for (int i = 0; i < WORD_ARRAY_FRAME_SIZE; i++) {
            counterTo = workString.find(' ', counterFrom);
            if (counterTo == std::string::npos) {
                counterTo = workString.length();
                i = WORD_ARRAY_FRAME_SIZE;
            } else {
                counterFrom = counterTo + 1;
            }
        }

        if (counterTo != workString.length()) {
            stringVector->push_back(workString.substr(0, counterTo));
            workString.erase(0, counterTo);
        } else {
            stringVector->push_back(workString);
            break;
        }
    }

    for (std::vector<std::string>::iterator it = stringVector->begin(); it != stringVector->end(); ++it) {
        char *arg = new char[(*it).length()];
        strcpy(arg, (*it).c_str());
        countWoldIncludes(arg);

    }

    delete (stringVector);
}

void printResult() {
    for (std::vector<std::string>::iterator it = wordsVector->begin(); it != wordsVector->end(); ++it) {
        std::string bufName = *it;
        int bufCount = wordsMap->at(*it);
        printf("%s %d\n", bufName.c_str(), bufCount);
    }
}
