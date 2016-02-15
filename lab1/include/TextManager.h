#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <string>
#include <string.h>
#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>

#include "ProjectOptions.h"
#include "WordsCounter.h"

class TextManager {
private:
    std::vector< std::map <std::string,int > > *globalVectorOfCounterMap;
    std::vector< std::vector < std::string > > *globalVectorOfKeysVector;
    
    std::map <std::string,int > *globalCounterMap;
    std::vector < std::string > *globalKeysVector;
public:
    TextManager();
    TextManager(std::string inputString);
    TextManager(FILE *file);
    void printResult();
    virtual ~TextManager();
    std::map <std::string,int >* getCounterMap();
    std::vector < std::string >* getKeysVector();
private:
    void generateWordsFreq(std::string inputString);
    /*
     *  Функция слияния двух карт счетчиков и ключей. Результат записывается в
     *  первые два указателя карты и мектора ключей соответственно
     */
    void mergeMapsAndVectors(std::map <std::string,int >* aMap, 
        std::vector < std::string >* aKey,
        std::map <std::string,int >* bMap,
        std::vector < std::string >* bKey);
};

#endif /* TEXTMANAGER_H */