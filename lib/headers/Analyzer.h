#include "TokenInfo.h"

using std::string;
class Analyzer {
    int symb;
    FILE *stream;
    int key_check(string);//return 1, if the identifire is a keyword, and 0 if not
    TokenInfo string_check (string, Types);//return 0 if the entity is a string literal and 1 if not
    TokenInfo identifier_check (string, Types);//return 0 if the entity a identifire and 1 if not
    TokenInfo comment_check(string, Types);//return 0 if the entity a comment and 1 if not
    TokenInfo hexa_float (string, Types);//return 0 if the entity is a hexademical floating constant and 1 if not
    TokenInfo decimal_float (string, Types);//return 0 if the entity is a decimal floating constant and 1 if not
public:
    Analyzer(char *path) {
        try {
            stream = fopen(path, "r");
            if (stream == NULL) {
                throw 1;
            }
        }
        catch (int a) {
            printf("can't open the file\n");
            return;
        }
        symb = fgetc(stream);
    };
    ~Analyzer() {
        fclose (stream);
    };    
    int has_next(); //checks whether the next symbol in the stream is EOF
    TokenInfo read_token();//function gets token from the stream
};
