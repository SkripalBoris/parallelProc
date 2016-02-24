#ifndef WORDSCOUNTER_H
#define WORDSCOUNTER_H

#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>

class WordsCounter {
private:
    std::string workString;
    std::map <std::string, int> *counterMap;
    std::vector<std::string> *keys;
public:
    WordsCounter();
    WordsCounter(std::string inputString);
    std::map <std::string, int>* getMap();
    std::vector<std::string>* getKeyVector();
    virtual ~WordsCounter();
private:
    void countWoldIncludes(std::string inputString,
        std::map <std::string, int> *map, std::vector<std::string> *keys);
};

#endif /* WORDSCOUNTER_H */

