#include<stdio.h>
#include<stdlib.h>

typedef enum
{
    Normal,
    Slash,
    Comment
} State;

State NextState(State CurrState, int ch)
{
    switch(CurrState)
    {
    case Normal:
        if(ch == '/')
            return Slash;
        return Normal;

    case Slash:
        if(ch == '/')
            return Comment;
        return Normal;

    case Comment:
        if(ch == '\n')
            return Normal;
        return Comment;

    default:
        return Normal;
    }
}

void CharProcess(int ch, State* CurrState, FILE* dest)
{
    State PrevState = *CurrState;
    *CurrState = NextState(PrevState,ch);

    switch(PrevState)
    {
    case Normal:
        if(ch != '/')
            fputc(ch,dest);
    break;

    case Slash:
        if(*CurrState == Comment)
            break;
        else
        {
            fputc('/',dest);
            fputc(ch,dest);
        }
    break;

    case Comment:
        if(ch=='\n')
            fputc('\n',dest);

    break;
    }
}

void RemoveComment(FILE* src, FILE* dest)
{
    int ch;

    State CurrState = Normal;

    while((ch=fgetc(src)) != EOF)
    {
        CharProcess(ch,&CurrState,dest);
    }

    if(CurrState == Slash)
        fputc('/',dest);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s <input file> <output file>\n",argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1],"r");
    if(input == NULL)
    {
        perror("Error Opening Input File");
        return 1;
    }

    FILE *output = fopen(argv[2],"w");
    if(output == NULL)
    {
        perror("Error Opening Input File");
        fclose(input);
        return 1;
    }

    RemoveComment(input,output);
    printf("Single Line Comment Remove Successfully");

    fclose(input);
    fclose(output);

    return 0;
}
