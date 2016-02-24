#ifndef GLOBALMAP_H
#define GLOBALMAP_H

#include <string>
#include <string.h>
#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>

#include "ProjectOptions.h"

class GlobalMap {
private:
    std::map <std::string,int > *globalCounterMap;
    std::vector < std::string > *globalKeysVector;
public:
    GlobalMap();
    virtual ~GlobalMap();
    std::map <std::string,int >* getCounterMap();
    std::vector < std::string >* getKeysVector();
    void addNewMapAndVector(std::map <std::string,int > *newCounterMap,
        std::vector < std::string > *newKeysVector);
};

#endif /* GLOBALMAP_H */

