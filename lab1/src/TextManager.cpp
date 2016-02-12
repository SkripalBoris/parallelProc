#include <stdlib.h>

#include "TextManager.h"
#include "WordsCounter.h"

TextManager::TextManager() {
    globalCounterMap = new std::vector< std::map <std::string,int > >;
    globalKeysVector = new std::vector < std::vector < std::string > >;
}

TextManager::TextManager(std::string inputString) {
    globalCounterMap = new std::vector < std::map <std::string,int > >;
    globalKeysVector = new std::vector < std::vector < std::string > >;
    generateWordsFreq(inputString);
}

TextManager::~TextManager() {
    if(globalCounterMap != NULL)
        delete(globalCounterMap);
    if(globalKeysVector != NULL)
        delete(globalKeysVector);
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
        globalCounterMap->push_back(*wordsCounter->getMap());
        globalKeysVector->push_back(*wordsCounter->getKeyVector());
    }
    
    delete(stringVector);
}