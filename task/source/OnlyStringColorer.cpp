#include <string>
#include <cstdio>
#include "OnlyStringColorer.h"
string
OnlyStringColorer::process (std::string token, Color color) {
    if (color == BLUE) {
        token += "\033[0;34m";
    }
    token += "\033[0m";
    return token;
}