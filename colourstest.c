#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main( int argc, char* argv[] )
{
    char *a = argv[1];
    int guess = atoi(a);
    int red, green, blue;
    int i=0;
    int max = 256;
    unsigned int color;

    double gred = (double)guess;
    double ggreen, gblue;

    unsigned int size = sizeof(unsigned int);
    printf("size: %i\n", size);

    printf("guess: %i\n",guess);

    while( gred >= 256 )
    {
        i++;
        gred = gred/max;
        printf("gred: %f\n",gred);
    }

    ggreen = ( gred - floor( gred ) ) * 256;
    gblue = ( ggreen - floor( ggreen ) ) * 256;

    unsigned int igred = (unsigned int)floor(gred);
    unsigned int iggreen = (unsigned int)floor(ggreen);
    unsigned int igblue = (unsigned int)floor(gblue);

    if(i==0)
    {
        igblue = igred;
        igred = iggreen = 0;
    }
    else if (i==1)
    {
        igblue = iggreen;
        iggreen = igred;
        igred = 0;
    }

    color = (igred<<24) | (iggreen<<16) | (igblue<<8) | 0;

    printf("hex 0x%08X\n", color);

    printf( "Guessed RGB: (%i,%i,%i)\n", igred, iggreen, igblue );

    i=0;
    for ( red = 0; red < 256; red++ ) {
        for ( green = 0; green < 256; green++ ) {
            for ( blue = 0; blue < 256; blue++ ) {
                if( i == guess )
                {
//                    red = fmin( red, 255 );
//                    green = fmin( green, 255 );
//                    blue = fmin( blue, 255 );
                    printf("RGB: (%i,%i,%i)\n",red,green,blue);
                    return 0;
                }
                i++;
            }
        }
    }

    return 0;
}
