#include <iostream>
#include "Analyzer.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include "digit.h"
#include "letter.h"
#define CALC 15

using std::string;

static const char *keyword[] = {
    "if", "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union"
};
/*this function checks whether identifier is a keyword*/
int Analyzer::key_check(string buffer) {
    for (int i = 0; i < CALC; i++) {
        if (!strcmp(buffer.c_str(), keyword[i])) {
            return 1;
        }
    }
    return 0;
}

TokenInfo Analyzer::string_check (string buffer, Types type) {
    FILE *file = stream;
    int new_symb;
    int symbol = symb;
    type = STRING;
    while ((symbol != '"') && (symbol != EOF) && (symbol != '\n')) {
        if (symbol == '\\') {
            new_symb = fgetc(file);
            if ((new_symb != 'b') && (new_symb != 'f') && (new_symb != 'n') && (new_symb != 'r')
                && (new_symb != 't') && (new_symb != 'v') && (new_symb != '\\') && (new_symb != '\'') && (new_symb != '\"') && (new_symb != '?')) {//wrong escape
                type = NOTYPE;
            }
            buffer += (char)symbol + (char)new_symb;
        } else {
            buffer += (char)symbol;
        }
        symbol = fgetc(file);
    }
    if (symbol != '"') {
        type = NOTYPE;
    }
    buffer += (char)symbol;
    if (symbol != EOF) {
        symbol = fgetc(file);
    }
    symb = symbol;
    TokenInfo tok(buffer, type);
    return tok;
}

TokenInfo Analyzer::identifier_check (string buffer, Types type) {
    FILE *file = stream;
    int symbol = symb;
    type = IDENT;
    while ((is_big_letter(symbol)) || (is_small_letter(symbol)) || (symbol == '_')
             || (is_digit(symbol))) {
        buffer += (char) symbol;
        symbol = fgetc(file);
    }
    if (key_check(buffer)) {
        type = KEYWORD;
    }
    symb = symbol;
    TokenInfo tok(buffer, type);
    return tok;
}

/* this function works with two types of comments-> Comment which begins with '/*' and ends with EOF is wrong*/
TokenInfo Analyzer::comment_check(string buffer, Types type) {
    FILE *file = stream;
    int flag = 0, new_symb;
    int symbol = symb;
    type = COMMENT;
    if (buffer.at(1) == '/' ) {
        while ((symbol != '\n') && (symbol != EOF)) {
            buffer += (char) symbol;
            symbol = fgetc(file);
        }
    } else {        
        while ((flag != 1) && (symbol != EOF)) {
            buffer += (char) symbol;
            if (symbol == '*') {
                new_symb = fgetc(file);
                if (new_symb == '/') {
                    buffer += (char) new_symb;
                    flag = 1;
                    symbol = fgetc(file);
                } else {
                    symbol = new_symb;
                }
            } else {
                symbol = fgetc(file);
            }
        }
        if (flag != 1) {//wrong comment            
            type = NOTYPE;
        }
        symbol = fgetc(file);
    }
    symb = symbol;
    TokenInfo tok(buffer, type);
    return tok;
}
/*this function works with hexademical floating constants*/
TokenInfo Analyzer::hexa_float (string buffer, Types type) {
    int control;
    FILE *file = stream;
    int symbol = symb;    
    int iter = buffer.size();
    control = iter;
    type = FLOAT_CONST;
    while ((is_digit(symbol)) || (is_big_letter(symbol))) {//0x
        buffer += (char) symbol;
        iter++;
        symbol = fgetc(file);
    }
    if (symbol == '.') {//0x{1}.
        buffer += (char) symbol;
        iter++;
        symbol = fgetc(file);
        if ((is_digit(symbol)) || (is_big_letter(symbol))) {//0x{1}.1
            while (is_digit(symbol) || (is_big_letter(symbol))) {
                buffer += (char) symbol;
                iter++;
                symbol = fgetc(file);
            }
        } else if (control == (iter - 1)) {//0x.
            symb = symbol;
            type = NOTYPE;
            TokenInfo tok(buffer, type);
            return tok;
        }
        if ((symbol == 'p') || (symbol == 'P')) {//0x{1}.p
            buffer += (char) symbol;
            symbol = fgetc(file);
            if ((symbol == '+') || (symbol == '-')) {//0x{1}.1p+
                buffer += (char) symbol;
                symbol = fgetc(file);
                if (is_digit(symbol)) {//0x{1}.1p+1
                    while (is_digit(symbol)) {
                        buffer += (char) symbol;
                        symbol = fgetc(file);
                    }
                    if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {//0x{1}.1p+1L
                        buffer += (char) symbol;
                        symbol = fgetc(file);
                    }
                    symb = symbol;
                    TokenInfo tok(buffer, type);
                    return tok;
                } else {//0x{1}.p+
                    symb = symbol;
                    type = NOTYPE;
                    TokenInfo tok(buffer, type);
                    return tok;
                }
            } else  if (is_digit(symbol)) {//0x{1}.1p1
                while (is_digit(symbol)) {
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
                if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {//0x{1}.1p1L
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
                symb = symbol;
                TokenInfo tok(buffer, type);
                return tok;
            } else {//0x{1}.1p
                symb = symbol;
                type = NOTYPE;
                TokenInfo tok(buffer, type);
                return tok;
            }
        } else { //0x{1}.1
            symb = symbol;
            type = NOTYPE;
            TokenInfo tok(buffer, type);
            return tok;
        }
    } else {//0x{1}
        symb = symbol;
        type = NOTYPE;
        TokenInfo tok(buffer, type);
        return tok;
    }
}
/*this function works with decimal floating constants*/
 TokenInfo Analyzer::decimal_float (string buffer, Types type) {
    int point = 0;
    FILE *file = stream;
    int iter = buffer.size();
    int symbol = symb;
    type = FLOAT_CONST;
    if (is_digit(buffer.at(0)))  {//1
        while (is_digit(symbol)) {//123
            buffer += (char) symbol;
            iter++;
            symbol = fgetc(file);
        }
    } 
    if ((symbol == '.') || (symbol == 'e') || (symbol == 'E')) {//1-> or 1e
        if (symbol == '.') {//1.
            point = 1;
            if (iter != buffer.size()) {//1.
                buffer += (char) symbol;
            }
            symbol = fgetc(file);
            if (is_digit(symbol)) {//1.1
                while (is_digit(symbol)) {
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
                if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {//1.1L
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                    symb = symbol;
                    TokenInfo tok(buffer, type);
                    return tok;
                }                    
            } else {
                if ((symbol != 'e') && (symbol != 'E')) {//1.
                    symb = symbol;
                    TokenInfo tok(buffer, type);
                    return tok;
                }
            }
        }
        if ((symbol == 'e') || (symbol == 'E')) {//1e or 1.1e
            buffer += (char) symbol;
            symbol = fgetc(file);
            if ((symbol == '+') || (symbol == '-')) {//1{.1}E+
                buffer += (char) symbol;
                symbol = fgetc(file);
                if (is_digit(symbol)) {//1{->}E+1
                    while (is_digit(symbol)) {
                        buffer += (char) symbol;
                        symbol = fgetc(file);
                    }
                    if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {//1{.1}E+1L
                        buffer += (char) symbol;
                        symbol = fgetc(file);
                    }
                    symb = symbol;
                    TokenInfo tok(buffer, type);
                    return tok;
                } else {//1{.1}E+
                    symb = symbol;
                    type = NOTYPE;
                    TokenInfo tok(buffer, type);
                    return tok;
                }
            } else if (is_digit(symbol)) {//1{.1}E1
                while (is_digit(symbol)) {
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
                if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {//1{.1}E1L
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
                symb = symbol;
                TokenInfo tok(buffer, type);
                return tok;
            } else {//1{.1}E
                symb = symbol;
                type = NOTYPE;
                TokenInfo tok(buffer, type);
                return tok;
            }
        } else {
            if (point != 1) {//123
                type = NOTYPE;
            }
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } 
    } else if (buffer.at(0) == '.') { //float begins with '.'
        if (is_digit(symbol)) {//.1
            while (is_digit(symbol)) {
                buffer += (char) symbol;
                symbol = fgetc(file);
            }
            if ((symbol == 'l') || (symbol == 'L') || (symbol == 'f') || (symbol == 'F')) {
                buffer += (char) symbol;
                symbol = fgetc(file);
            }
        } else {// .
            type = NOTYPE;
        }
        symb = symbol;
        TokenInfo tok(buffer, type);
        return tok;
    } else {//not a floating constant
        symb = symbol;
        type = NOTYPE;
        TokenInfo tok(buffer, type);
        return tok;
    }
}

TokenInfo Analyzer::read_token() {
    FILE *file = stream;
    Types type = NOTYPE;
    int new_symb;
    string buffer = "";
    int symbol = symb;
    if (symbol != EOF) {
        if (is_digit(symbol)) {
            buffer += (char) symbol;
            new_symb = fgetc(file);
            if (symbol == '0') {
                if (new_symb == 'x') {
                    buffer += (char) new_symb;
                    symbol = fgetc(file);
                    symb = symbol;
                    return hexa_float(buffer, type);
                } else {
                    symbol = new_symb;
                    symb = symbol;
                    return decimal_float(buffer, type);
                }                                
            } else {
                symbol = new_symb;
                symb = symbol;
                return decimal_float(buffer, type);
            }
        } else if ((is_big_letter(symbol)) || (is_small_letter(symbol)) || (symbol == '_') ||
                   (is_digit(symbol))) {
            buffer += (char) symbol;
            if ((symbol == 'L') || (symbol == 'u') || (symbol == 'U')) {
                new_symb = fgetc(file);
                if (new_symb == '"') {
                    buffer += (char) new_symb;
                    symbol = fgetc(file);
                    symb = symbol;
                    return string_check(buffer, type);
                } else if ((new_symb == '8') && (symbol == 'u')) {
                    new_symb = fgetc(file);
                    buffer += '8';
                    if (new_symb == '"') {
                        buffer += (char) new_symb;
                        symbol = fgetc(file);
                        symb = symbol;
                        return string_check(buffer, type);
                    } else {
                        symbol = new_symb;
                        symb = symbol;
                        return identifier_check(buffer, type);
                    }                    
                } else {
                    symbol = new_symb;
                    symb = symbol;
                    return identifier_check(buffer, type);
                }         
            } else {
                symbol = fgetc(file);
                symb = symbol;
                return identifier_check(buffer, type);
            }
        } else if (symbol == '"') {
            buffer += (char) symbol;
            symbol = fgetc(file);
            symb = symbol;
            return string_check(buffer, type);
        } else if (symbol == '/') {
            buffer += (char) symbol;
            new_symb = fgetc(file);
            if ((new_symb == '/') || (new_symb == '*')) { //comment
                buffer += (char) new_symb;
                symbol = fgetc(file);
                symb = symbol;
                return comment_check(buffer, type);
            } else if (new_symb == '=') {
                buffer += (char) new_symb;
                symbol = fgetc(file);
            } else {                
                symbol = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '=') {
            if ((new_symb = fgetc(file)) == '=') {
                buffer += (char) symbol + (char) new_symb;                
                symbol = fgetc(file);
            } else {
                buffer += (char) symbol;
                symbol = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '%') {
            new_symb = fgetc(file);
            buffer += (char) symbol;
            if ((new_symb == '=') || (new_symb == '>') || (new_symb == ':')) {
                buffer += (char) new_symb;
                symbol = fgetc(file);
            } else {
                symb = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '<') {
            new_symb = fgetc(file);
            buffer += (char) symbol;
            if ((new_symb == '=') || (new_symb == '%') || (new_symb == ':') || (new_symb == '<')) {
                buffer += (char) new_symb;
                symbol = fgetc(file);
                if ((new_symb == '<') && (symbol == '=')) {
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
            } else {
                buffer += (char) symbol;
                symbol = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '>') {
            new_symb = fgetc(file);
            buffer += (char) symbol;
            if ((new_symb == '=') || (new_symb == '>')) {
                buffer += (char) new_symb;
                symbol = fgetc(file);
                if ((new_symb == '>') && (symb == '=')) {
                    buffer += (char) symbol;
                    symbol = fgetc(file);
                }
            } else {
                symbol = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '|') {
            new_symb = fgetc(file);
            buffer += (char) symbol;
            if ((new_symb == '|') || (new_symb == '=')) {
                buffer += (char) new_symb;
                symbol = fgetc(file);
            } else {
                symbol = new_symb;
            }
            type = PUNCTUATOR;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '^') {
            buffer += (char) symbol;
            if ((new_symb = fgetc(file)) == '=') {
                buffer += (char) new_symb;
                symbol = fgetc(file);
            } else {
                symbol = new_symb;
            }
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '&') {
            new_symb = fgetc(file);
            buffer += (char) symbol;
            if ((new_symb == '&') || (new_symb == '=')) {
                buffer += (char) new_symb;
                symbol = fgetc(file);
                type = PUNCTUATOR;
            } else {
                type = NOTYPE;
                symbol = new_symb;
            }
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if ((symbol == '!') || (symbol == '*') || (symbol == '+') || (symbol == '-')) {
            buffer += (char) symbol;
            if ((new_symb = fgetc(file)) == '=') {
                buffer += (char) new_symb;
                symbol = fgetc(file);
                type = PUNCTUATOR;
            } else {
                type = NOTYPE;
                symbol = new_symb;
            }
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == ':') {
            buffer += (char) symbol;
            if ((new_symb = fgetc(file)) == '>') {
                buffer += (char) new_symb;
                symbol = fgetc(file);
                type = PUNCTUATOR;
            } else {
                type = NOTYPE;
                symbol = new_symb;
            }
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        } else if (symbol == '.') {
            buffer += (char) symbol;
            symbol = fgetc(file);
            symb = symbol;
            return decimal_float(buffer, type);
        } else {
            buffer += (char) symbol;
            symbol = fgetc(file);
            type = NOTYPE;
            symb = symbol;
            TokenInfo tok(buffer, type);
            return tok;
        }
    } else {
        type = NOTYPE;
        TokenInfo tok(buffer, type);
        return tok;
    }
}

int Analyzer :: has_next() {//checks whether the next symbol in the stream is EOF
    return (symb != EOF);
};
