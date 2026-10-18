#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define MAX 1024

typedef enum {
    NORMAL,
    SLASH,
    SINGLE_CMT,
    MULIT_CMT,
    STAR,
    STR,
    CHAR
} State;

State nextState(State, int);
void handleEsc(FILE *, FILE *);
bool isEmpty(const char *);
void parseChar(int,State *, FILE *, FILE *);
void removeComments(FILE *, FILE *);
int main(int, char **);

State nextState(State cur, int c) {
    switch(cur) {
    case NORMAL:
        if( c == '/') return SLASH;
        if( c == '"') return STR;
        if( c == '\'') return CHAR;
        return NORMAL;

    case SLASH:
        if( c == '/') return SINGLE_CMT;
        if( c == '*') return MULIT_CMT;
        return NORMAL;

    case SINGLE_CMT:
        if( c == '\n') return NORMAL;
        return SINGLE_CMT;

    case MULIT_CMT:
        if( c == '*') return STAR;
        return MULIT_CMT;

    case STAR:
        if( c == '/') return NORMAL;
        if( c == '*') return STAR;
        return MULIT_CMT;

    case STR:
        if( c == '"') return NORMAL;
        return STR;

    case CHAR:
        if( c == '\'') return NORMAL;
        return CHAR;

    default:
        return NORMAL;
    }
}

void handleEsc(FILE* in, FILE *out) {
    int next_c = fgetc(in);
    if (next_c != EOF)
        fputc(next_c,out);
}

bool isEmpty(const char *s) {
    while(*s != '\0') {
        if(*s != ' ' && *s != '\t' && *s != '\n' && *s != '\r')
            return false;
        s++;
    }
    return true;
}

void parseChar(int c, State*st, FILE* in, FILE* temp) {
    State prev = *st;

    *st = nextState(prev, c);

    if(prev == NORMAL) {
        if(c != '/')
            fputc(c,temp);
    }
    else if(prev == SLASH) {
        if(*st == NORMAL) {
            fputc('/',temp);
            if(c == '"' || c == '\'')
                fputc(c,temp);
            else if(c != '/')
                fputc(c,temp);
        }
    }
    else if(prev == SINGLE_CMT) {
        if(c == '\n')
            fputc('\n',temp);
    }
    else if(prev == MULIT_CMT || prev == STAR) {
        if(c == '\n')
            fputc('\n',temp);
    }
    else if(prev == STR || prev == CHAR) {
        fputc(c,temp);
        if(c == '\\')
            handleEsc(in,temp);
    }
}

void removeComments(FILE *in, FILE * out) {
 FILE *temp = tmpfile();
 if(temp == NULL) {
    perror("temp file open failed");
    return;
    }

    int c;
    State st = NORMAL;

    while(( c = fgetc(in)) != EOF) {
        parseChar(c, &st, in, temp);
    }
    if(st == SLASH)
        fputc('/',temp);

    rewind(temp);

    char buf[MAX];

    while(fgets(buf,sizeof(buf), temp) != NULL) {
        if(! isEmpty(buf))
            fputs(buf, out);
    }
    fclose(temp);
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("Usage: %s <input.c> <output.c>\n",argv[0]);
        return 1;
    }

    FILE *in  = fopen(argv[1], "r");
    if(in == NULL) {
        perror("input file failed to open");
        return 1;
    }

    FILE *out = fopen(argv[2],"w");
    if(out == NULL) {
        perror("output file failed to open");
        fclose(in);
        return 1;
    }

    removeComments(in,out);
    printf("Done! Comments Are Removed Successfully");

    fclose(in);
    fclose(out);

    return 0;
}
