#include "letter.h"
bool is_big_letter (int c) {
    return ((c >= 'A') && (c <= 'Z'));
}
bool is_small_letter (int c) {
    return ((c >= 'a') && (c <= 'z'));
}
