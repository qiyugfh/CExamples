#include <stdio.h>  
#include <string.h>  
#include <pcre.h>  
#define OVECCOUNT 30 /* should be a multiple of 3 */  
#define EBUFLEN 128  
#define BUFLEN 1024  
  
int main(int argc, char **argv)  
{  
  
  
    pcre *re;  
    const char *error;  
    int  erroffset;  
    int  ovector[OVECCOUNT];  
    int  rc, i;  
  
    char src[] = "123.123.123.123:80|1.1.1.1:88";  
    char pattern[] = "(\\d*.\\d*.\\d*.\\d*):(\\d*)";  
  
    printf("String : %s\n", src);  
    printf("Pattern: \"%s\"\n", pattern);  
  
  
    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);  
    if (re == NULL) {  
        printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);  
        return 1;  
    }  
  
    char *p = src;  
    while ( ( rc = pcre_exec(re, NULL, p, strlen(p), 0, 0, ovector, OVECCOUNT)) != PCRE_ERROR_NOMATCH )  
    {  
        printf("\nOK, has matched ...\n\n");  
  
        for (i = 0; i < rc; i++)  
        {  
            char *substring_start = p + ovector[2*i];  
            int substring_length = ovector[2*i+1] - ovector[2*i];  
            char matched[1024];  
            memset( matched, 0, 1024 );  
            strncpy( matched, substring_start, substring_length );  
  
            printf( "match:%s\n", matched );  
        }  
  
        p += ovector[1];  
        if ( !p )  
        {  
            break;  
        }  
    }  
    pcre_free(re);  
  
    return 0;  
}  
