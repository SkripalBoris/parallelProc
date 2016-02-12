#include <string.h>

#include "WordsCounter.h"

WordsCounter::WordsCounter() {
    counterMap = NULL;
    keys = NULL;
}

WordsCounter::WordsCounter(std::string inputString) {
    this->workString = inputString;
    counterMap = new std::map<std::string, int>;
    keys = new std::vector<std::string>;
    deleteWorkSymbols(this->workString);
    countWoldIncludes(this->workString,this->counterMap,this->keys);
}

WordsCounter::~WordsCounter() {
    if(counterMap!=NULL)
        delete(counterMap);
    if(keys != NULL)
        delete(keys);
}

std::map <std::string, int>* WordsCounter::getMap() {
    return this->counterMap;
}

std::vector<std::string>* WordsCounter::getKeyVector() {
    return this->keys;
}

void WordsCounter::deleteWorkSymbols(std::string &inputString) {
    if(inputString.length()==0)
        return;
    
    char chars[] = "()-,.!?\"\'";

    for (unsigned int i = 0; i < strlen(chars); ++i)
    {
      inputString.erase (std::remove(inputString.begin(), inputString.end(),
              chars[i]), inputString.end());
    }
}

void WordsCounter::countWoldIncludes(std::string inputString,
        std::map <std::string, int> *map, std::vector<std::string> *keys) {
    if(map == NULL)
        return;
    if(inputString.length() == 0)
        return;
    int fromCounter = 0;
    int toCounter = inputString.find(' ',fromCounter);
        if(toCounter == std::string::npos)
            toCounter = inputString.length();
    
    do {  
        std::string buf = inputString.substr(fromCounter,toCounter-fromCounter);
        if(map->count(buf)) {
            int bufCount = map->at(buf);
            std::map<std::string, int>::iterator itMap = map->begin();
            itMap = map->find(buf);
            map->erase(itMap);
            map->insert(std::pair<std::string,int>(buf,++bufCount));
            keys->push_back(buf);            
        } else {
            map->insert(std::pair<std::string,int>(buf,1));
        }
        
        if(toCounter == inputString.length())
            return;
        
        fromCounter = toCounter+1;
        toCounter = inputString.find(' ',fromCounter);
        if(toCounter == std::string::npos)
            toCounter = inputString.length();  
        
    } while (true);
}