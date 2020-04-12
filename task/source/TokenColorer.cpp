#include <string>
#include <cstdio>
#include "TokenColorer.h"
std::string
TokenColorer::process(std::string token, Color color) {
    switch(color) {
        case RED:
            token = "\033[01;31m" + token;
            break;
        case PINK:
            token = "\033[0;35m" + token;
            break;
        case ORANGE:
            token = "\033[0;33m" + token;
            break;
        case YELLOW:
            token = "\033[1;33m" + token;
            break;
        case GREEN:
            token = "\033[0;32m" + token;
            break;
        case BLUE:
            token = "\033[0;34m" + token;
            break;
        case NOCOLOR:
            break;
    }
    token += "\033[0m";
    return token;
}

