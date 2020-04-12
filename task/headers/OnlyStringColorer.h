#ifndef MAIN_CPP_PRINT_STR_H
#define MAIN_CPP_PRINT_STR_H
#include "TokenProcessor.h"
#include "TokenInfo.h"
class OnlyStringColorer : public TokenProcessor {
public:
    std::string process (string, Color);//the entity, which is printed, is a string literal
};
#endif //MAIN_CPP_PRINT_STR_H
