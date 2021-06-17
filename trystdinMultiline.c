/* fgetpos example */
#include <stdio.h>
int main ()
{
    FILE * pFile;
    int c;
    int n;
    fpos_t pos;

    pFile = stdin;
   
    c = fgetc (pFile);
    printf ("1st character is %c\n",c);
    fgetpos (pFile,&pos);
    for (n=0;n<3;n++){
        c = fgetc (pFile);
        printf ("2nd character is %c\n",c);
    }
    fsetpos(stdin,&pos);
    for (n=0;n<3;n++){
        c = fgetc (pFile);
        printf ("revolting %c\n",c);
    }
    return 0;
}