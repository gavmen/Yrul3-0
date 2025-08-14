#include "string.h"

int strcmp(const char* s1, const char* s2) {
    if (!s1 || !s2) return (s1 == s2) ? 0 : (s1 ? 1 : -1);
    
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strlen(const char* s) {
    if (!s) return 0;
    
    int len = 0;
    while (*s++) {
        len++;
    }
    return len;
}
