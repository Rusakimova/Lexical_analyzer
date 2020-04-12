#ifndef MAIN_CPP_WORD_H
#define MAIN_CPP_WORD_H

#include <iostream>
enum Types { NOTYPE ,
    FLOAT_CONST,
    PUNCTUATOR ,
    IDENT,
    KEYWORD ,
    COMMENT,
    STRING};

using std :: string;
class TokenInfo {
    string token;
    Types token_type;
public:
    TokenInfo (string buffer, Types type) {
        token_type = type;
        token = buffer;
    };
    ~TokenInfo(){};
    string get_token ();
    Types get_type ();
};

#endif //MAIN_CPP_WORD_H
