#include <stdlib.h>
#include <iostream>

#include "TextManager.h"

TextManager::TextManager() {
    this->globalMap = new GlobalMap();
}

TextManager::TextManager(std::string inputString) {
    this->globalMap = new GlobalMap();
    generateWordsFreq(inputString);
}

void TextManager::printResult() {
    for (std::vector<std::string>::iterator it = this->globalMap->getKeysVector()->begin() ; it != this->globalMap->getKeysVector()->end(); ++it) {
        //std::cout << (*it) << " " << globalCounterMap->find(*it) << std::endl;
        std::string bufName = *it;
        int bufCount = this->globalMap->getCounterMap()->at(*it);
        printf("%s %d\n",bufName.c_str(),bufCount);
    }
}

TextManager::TextManager(FILE *file) {
     if (file == NULL) {
        perror("File error");
        exit(1);
    }

    this->globalMap = new GlobalMap();
    
    fseek(file, 0, SEEK_END);
    long lSize = (size_t) ftell(file);
    rewind(file);

    char* buffer = new char[lSize];
    fread(buffer, 1, lSize, file);
    
    std::string bufStr(buffer);
    
    generateWordsFreq(bufStr);
    
    fclose(file);
    
    delete(buffer);
}

TextManager::~TextManager() {
    if(this->globalMap != NULL)
        delete(this->globalMap);
}

void TextManager::generateWordsFreq(std::string inputString) {
    std::vector<std::string> *stringVector = new std::vector<std::string>;
    std::string workString = inputString;
    if(inputString.empty())
        return;
    
    while(true) {
        int counterFrom = 0;
        int counterTo = 0;
        for(int i=0;i<WORD_ARRAY_FRAME_SIZE;i++) {
            counterTo = workString.find(' ',counterFrom);
            if(counterTo == std::string::npos) {
                counterTo = workString.length();
                i = WORD_ARRAY_FRAME_SIZE;
            } else {
                counterFrom = counterTo + 1;
            }
        }
        
        if(counterTo != workString.length()) {
            stringVector->push_back(workString.substr(0,counterTo));
            workString.erase(0,counterTo);
        } else {
            stringVector->push_back(workString);
            break;
        }
    }
    
    for (std::vector<std::string>::iterator it = stringVector->begin() ; it != stringVector->end(); ++it) {
        WordsCounter *wordsCounter = new WordsCounter(*it);
        this->globalMap->addNewMapAndVector(wordsCounter->getMap(),wordsCounter->getKeyVector());
    }
    
    delete(stringVector);    
}

GlobalMap* TextManager::getGlobalMap() {
    return this->globalMap;
}