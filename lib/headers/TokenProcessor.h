#ifndef TOKENCOLORER_CPP_TOKENPROCESSOR_H
#define TOKENCOLORER_CPP_TOKENPROCESSOR_H
#include "TokenInfo.h"
#include "Colors.h"
/*class describes ways of print some entity*/
class TokenProcessor {
public:
    virtual std::string process (std::string, Color) = 0; //chooses entity, which should be printed according to user mode
    virtual ~TokenProcessor() = default;
};


#endif //TOKENCOLORER_CPP_TOKENPROCESSOR_H
