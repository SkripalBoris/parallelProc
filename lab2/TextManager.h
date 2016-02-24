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
#include "GlobalMap.h"

class TextManager {
    GlobalMap* globalMap;
public:
    TextManager();
    TextManager(std::string inputString);
    TextManager(FILE *file);
    void printResult();
    virtual ~TextManager();
    GlobalMap* getGlobalMap();
private:
    void generateWordsFreq(std::string inputString);
};

#endif /* TEXTMANAGER_H */