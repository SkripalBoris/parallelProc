#include <stdlib.h>

#include "TextManager.h"
#include "WordsCounter.h"

TextManager::TextManager() {
    globalVectorOfCounterMap = new std::vector< std::map <std::string,int > >;
    globalVectorOfKeysVector = new std::vector < std::vector < std::string > >;
}

TextManager::TextManager(std::string inputString) {
    globalVectorOfCounterMap = new std::vector < std::map <std::string,int > >;
    globalVectorOfKeysVector = new std::vector < std::vector < std::string > >;
    generateWordsFreq(inputString);
}

TextManager::~TextManager() {
    if(globalVectorOfCounterMap != NULL)
        delete(globalVectorOfCounterMap);
    if(globalVectorOfKeysVector != NULL)
        delete(globalVectorOfKeysVector);
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
        globalVectorOfCounterMap->push_back(*wordsCounter->getMap());
        globalVectorOfKeysVector->push_back(*wordsCounter->getKeyVector());
    }
    
    delete(stringVector);
    
    if(this->globalCounterMap != NULL)
        delete(this->globalCounterMap);
    if(this->globalKeysVector != NULL)
        delete(this->globalKeysVector);
    
    this->globalCounterMap = new std::map <std::string,int >;
    this->globalKeysVector = new std::vector < std::string >;
    
    std::vector< std::map <std::string,int > >::iterator itMap = this->globalVectorOfCounterMap->begin();
    std::vector< std::vector < std::string > >::iterator itKey = this->globalVectorOfKeysVector->begin();
    while(true) {
        std::vector < std::string > *bufKeys = &itKey;
        std::map <std::string,int > *bufMap = &itMap;
        mergeMapsAndVectors(this->globalCounterMap,this->globalKeysVector,
                bufMap, bufKeys);
        itMap++;
        itKey++;
        if(itKey == this->globalVectorOfKeysVector->end())
            return;
    }    
}

void mergeMapsAndVectors(std::map <std::string,int > *aMap,
        std::vector < std::string > *aKey,
        std::map <std::string,int > *bMap,
        std::vector < std::string > *bKey) {
    if(aMap == NULL) {
        aMap = new std::map <std::string,int >;
        aKey = new std::vector < std::string >;
        
        for (std::vector<std::string>::iterator it = bKey->begin() ; it != bKey->end(); ++it) {
            int bufCountB = bMap->at(*it);
            aMap->insert(std::pair<std::string,int>(*it,bufCountB));
            aKey->push_back(*it);
        }
        return;
    }
    
    if(bMap == NULL) {
        return;
    }
    
    for (std::vector<std::string>::iterator it = bKey->begin() ; it != bKey->end(); ++it) {
        if(aMap->count(*it)) {
            int bufCountA = aMap->at(*it);
            int bufCountB = bMap->at(*it);
            std::map<std::string, int>::iterator itMap = aMap->begin();
            itMap = aMap->find(*it);
            aMap->erase(itMap);
            aMap->insert(std::pair<std::string,int>(*it,bufCountA+bufCountB));
        } else {
            int bufCountB = bMap->at(*it);
            aMap->insert(std::pair<std::string,int>(*it,bufCountB));
            aKey->push_back(*it);
        }
    }
}

std::map <std::string,int >* TextManager::getCounterMap() {
    return this->globalCounterMap;
}

std::vector < std::string >* TextManager::getKeysVector() {
    return this->globalKeysVector;
}