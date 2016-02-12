#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <string>
#include <string.h>
#include <algorithm>
#include <map>
#include <vector>

#include "ProjectOptions.h"

class TextManager {
private:
    std::vector< std::map <std::string,int > > *globalCounterMap;
    std::vector< std::vector < std::string > > *globalKeysVector;
    
public:
    TextManager();
    TextManager(std::string inputString);
    virtual ~TextManager();
private:
    void generateWordsFreq(std::string inputString);
};

#endif /* TEXTMANAGER_H */