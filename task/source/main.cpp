#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include "Analyzer.h"
#include "OnlyStringColorer.h"
#include "TokenColorer.h"
#include "TokenInfo.h"
#include "ColorText.h"
/* The first parameter argv is an input file, the second parameter is a way of working. Output is in stdout*/
int
main (int argc, char *argv[]) {
    if (argc != 3) {
        printf("First argument should be file and the second is the way of printing(all or string)");
        return 1;
    }
    TokenProcessor *processor;
    Analyzer analyzer(argv[1]);
    ColorText col;
    if (!(strcmp(argv[2], "string"))) {
        TokenProcessor *q = new OnlyStringColorer;
        processor = q;
    } else {
        TokenProcessor *q = new TokenColorer;
        processor = q;
    }
    while (analyzer.has_next()) {
        TokenInfo tok = analyzer.read_token();
        std::string colorized_token = col.coloring(tok, processor);
        std::cout<<colorized_token.c_str();
    }
    delete processor;
    return 0;
}
