#include <iostream>
#include <strings.h>

#include "TextManager.h"

int main (int argc, char *argv[]) {
    FILE *file = fopen("res/test1.txt","r");
    TextManager *textManager = new TextManager(file);
    textManager->printResult();
}