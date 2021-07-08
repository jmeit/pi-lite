#include <stdio.h>
#include <stdlib.h>

int openTouchScreen()
{
    FILE* fd;
    fd = fopen("/dev/input/event0", "rb" );
    printf("%d\n",fd);
    if ( fd == NULL )
    {
        printf("bronze\n");
        return 0;
    }
    else
    {
        printf("gold\n");
        return 1;
    }
}


int main( char *argv, int argc )
{
    printf( "Checking for touchscreen...\n" );
    if ( openTouchScreen() )
    {
        printf( "Found.\n" );
    }
    else
    {
        printf( "Not found.\n" );
    }

    return 0;
}
