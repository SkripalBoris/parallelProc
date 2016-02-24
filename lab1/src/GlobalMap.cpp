#include "GlobalMap.h"

GlobalMap::GlobalMap() {
    this->globalCounterMap = new std::map <std::string,int >;
    this->globalKeysVector = new std::vector < std::string >;
}

GlobalMap::~GlobalMap() {
    if(this->globalCounterMap != NULL)
        delete(this->globalCounterMap);
    if(this->globalKeysVector != NULL)
        delete(this->globalKeysVector);
}

void GlobalMap::addNewMapAndVector(std::map <std::string,int > *newCounterMap,
        std::vector < std::string > *newKeysVector) {
    if( this->globalCounterMap == NULL) {
        this->globalCounterMap = new std::map <std::string,int >;
        this->globalKeysVector = new std::vector < std::string >;
        
        for (std::vector<std::string>::iterator it = newKeysVector->begin() ; it != newKeysVector->end(); ++it) {
            int bufCountB = newCounterMap->at(*it);
            this->globalCounterMap->insert(std::pair<std::string,int>(*it,bufCountB));
            this->globalKeysVector->push_back(*it);
        }
        return;
    }
    
    if(newCounterMap == NULL)
        return;
    
    if(newKeysVector == NULL)
        return;
    
    for (std::vector<std::string>::iterator it = newKeysVector->begin() ; it != newKeysVector->end(); ++it) {
        if(this->globalCounterMap->count(*it)) {
            int bufCountA = this->globalCounterMap->at(*it);
            int bufCountB = newCounterMap->at(*it);
            std::map<std::string, int>::iterator itMap = this->globalCounterMap->begin();
            itMap = this->globalCounterMap->find(*it);
            this->globalCounterMap->erase(itMap);
            this->globalCounterMap->insert(std::pair<std::string,int>(*it,bufCountA+bufCountB));
        } else {
            int bufCountB = newCounterMap->at(*it);
            this->globalCounterMap->insert(std::pair<std::string,int>(*it,bufCountB));
            this->globalKeysVector->push_back(*it);
        }
    }
}

std::map <std::string,int >* GlobalMap::getCounterMap() {
    return this->globalCounterMap;
}

std::vector < std::string >* GlobalMap::getKeysVector() {
    return this->globalKeysVector;
}