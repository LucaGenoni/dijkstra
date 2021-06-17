#include <stdio.h>
 
int main(void)
{
    printf("TMP_MAX = %d, FOPEN_MAX = %d\n", TMP_MAX, FOPEN_MAX);
    FILE* tmpf = tmpfile();
    fputs("Hello, world", tmpf);
    rewind(tmpf);
    char buf[6];
    fgets(buf, sizeof buf, tmpf);
    printf("got back from the file: '%s'\n", buf);
 
    
}
