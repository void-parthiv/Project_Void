#include<stdio.h>

int main()
{
    FILE *fin,*fout;
    char ch,prev;
    int flag=0;
    fin=fopen("1.txt","r");
    fout=fopen("2.txt","w");
    if(fin==NULL || fout==NULL)
        return 0;
    while(fscanf(fin,"%c",&ch) ==1)
    {
        if(ch=='"' && flag==0)
        {
            flag=1;
            fprintf(fout,"%c",ch);
            fscanf(fin,"%c",&ch);
            if(ch=='\'')
                flag=0;
        }
        if(ch=='"' && flag==1)
        {
            if(prev!='\\')
                flag=0;
        }
        if(ch=='/' && flag==0)
        {
            prev=ch;
            fscanf(fin,"%c",&ch);
            if(ch=='/')
            {
                while(ch!='\n')
                    fscanf(fin,"%c",&ch);
            }
            else if(ch=='*')
            {
                while(1)
                {
                    prev=ch;
                    fscanf(fin,"%c",&ch);
                    if(prev=='*'&&ch=='/')
                        break;
                }
                fscanf(fin,"%c",&ch);
            }
            else
                fprintf(fout,"%c",prev);
        }
        fprintf(fout,"%c",ch);
        prev=ch;
    }
    return 0;
}
