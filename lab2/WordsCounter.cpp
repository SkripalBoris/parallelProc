#include <string.h>
#include <wctype.h>
#include <ios>

#include "WordsCounter.h"

WordsCounter::WordsCounter() {
    counterMap = NULL;
    keys = NULL;
}

WordsCounter::WordsCounter(std::string inputString) {
    this->workString = inputString;
    counterMap = new std::map<std::string, int>;
    keys = new std::vector<std::string>;
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

void WordsCounter::countWoldIncludes(std::string inputString,
        std::map <std::string, int> *map, std::vector<std::string> *keys) {
    if(map == NULL)
        return;
    if(inputString.length() == 0)
        return;
    
    char * workCharArr = new char[inputString.length() + 1];
    std::strcpy(workCharArr,inputString.c_str());
    
    char * pch = std::strtok (workCharArr," ,. \"!?()\n");
    
    while (pch != NULL) {
        int i = 0;
        
        /*while(pch[i]) {
            //boos
            pch[i++] = tolower(pch[i]);
        }*/

        if(map->count(pch)) {
            int bufCount = map->at(pch);
            std::map<std::string, int>::iterator itMap = map->begin();
            itMap = map->find(pch);
            map->erase(itMap);
            map->insert(std::pair<std::string,int>(pch,++bufCount));   
        } else {
            map->insert(std::pair<std::string,int>(pch,1));
            keys->push_back(pch); 
        }
        pch = strtok (NULL, " ,. \"!?()\n");
    }
    delete (workCharArr);
}