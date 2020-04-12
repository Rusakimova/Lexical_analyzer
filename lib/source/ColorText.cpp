#include "ColorText.h"
std::string ColorText::coloring(TokenInfo tok, TokenProcessor *processor) {
    Color color;
    switch(tok.get_type()) {
        case FLOAT_CONST:
            color = RED;
            break;
        case PUNCTUATOR:
            color = PINK;
            break;
        case IDENT:
            color = ORANGE;
            break;
        case KEYWORD:
            color = YELLOW;
            break;
        case COMMENT:
            color = GREEN;
            break;
        case STRING:
            color = BLUE;
            break;
        case NOTYPE:
            color = NOCOLOR;
            break;
    }
    std::string buffer = processor->process(tok.get_token(), color);
    return buffer;
}
