#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    char stuff[256];
    char stuff2[256];
    scanf("%s", stuff);
    strcpy(stuff2, stuff);
    int i = strlen(stuff2);
    int l = i / 2;
    int j;
    for(j = 0; j < l; ++j) {
        char k = stuff2[i - j - 1];
        stuff2[i - j - 1] = stuff2[j];
        stuff2[j] = k;
    }
    printf("%s\n", stuff);
    printf("%s\n", stuff2);
}
