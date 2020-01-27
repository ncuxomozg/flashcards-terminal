/*
 * Basic terminal-based flashcards game. 
 * It uses csv-like files as a source of words. You can find examples at the "./dicts" directory.
 *
 * Usage:
 *     make && ./words ./dict/000-example
 */

#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>

#define DICT_LIMIT 500
#define WORD_LIMIT 100

struct word_s {
    wchar_t en[WORD_LIMIT];
    wchar_t ru[WORD_LIMIT];
};


struct dict_s {
    int len;
    struct word_s words[DICT_LIMIT];
};


struct dict_s read_the_dict(const char *);
void ask();


int main(int argc, char *argv[]) {
    int rnd = 0;

    if (argc < 2) {
        printf("ERROR: dictionary isn't defined\n");
        printf("Try './words dictionary.txt'\n");
        return 0;
    }

    struct dict_s dict = read_the_dict(argv[1]); 

    system("clear");
    printf("Loaded dictionary \"%s\" with %d words\n", argv[1], dict.len);
    printf("Press 'q' and ENTER if you want to quit\n\n");

    while (1) {
        rnd = rand() % dict.len;

        printf("(%d): %ls", rnd, dict.words[rnd].ru); ask();
        printf("Answer: %ls\n", dict.words[rnd].en);  ask();

        system("clear");
    }
}


struct dict_s read_the_dict(const char *path) {
    struct dict_s dict = {
        .len = 0,
        .words = {},
    }; 

    FILE *fh;

    setlocale(LC_CTYPE, "");

    fh = fopen(path, "r");

    if (!fh) {
        perror("fopen");
        exit(1);
    }

    int i = 0;
    int w_idx = 0;
    int column = 0;

    wchar_t str[WORD_LIMIT];

    while (fgetws(str, WORD_LIMIT, fh) != NULL) {
        column = 0;
        dict.len++;

        for (i = 0; i < wcslen(str); i++) {
            if ( str[i] == L';'
                || str[i] == L'\n'
                || (column && wcslen(dict.words[dict.len - 1].ru) == 0 && str[i] == L' ')
            ) {
               column = 1; w_idx = 0;
               continue;
            }
    
            (column == 0)
                ? ( dict.words[dict.len - 1].en[w_idx++] = str[i] )
                : ( dict.words[dict.len - 1].ru[w_idx++] = str[i] )
                ;
        }
    }

    if (fclose(fh) == EOF) {
        perror("fclose");
        exit(1);
    }

    return dict;
}


void ask() {
    if (fgetc(stdin) == 'q') {
        printf("BYE\n\n");
        exit(0);
    }
}
