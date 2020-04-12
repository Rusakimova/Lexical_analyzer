#ifndef TOKENCOLORER_CPP_TOKENCOLORER_H
#define TOKENCOLORER_CPP_TOKENCOLORER_H
#include "TokenProcessor.h"
#include "TokenInfo.h"
class TokenColorer : public TokenProcessor {
public:
    std::string process (string, Color); //the entity, which is printed, is all tokens
};

#endif //TOKENCOLORER_CPP_TOKENCOLORER_H
