#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int getColor( unsigned int pos )
{
    printf("reached\n");

    unsigned int color;
    double red, green, blue;
    double max = 256.0;
    unsigned int ired, igreen, iblue;
    int i=0;
    printf("pos1: %u\n", pos);

    red = (double) pos;

    printf("pos2: %u\n", pos);

    while( red >= max )
    {
        i++;
        printf("red: %f, max: %f\n",red,max);
        red = red/max;
    }

    green = ( red - floor( red ) ) * 256;
    blue = ( green - floor( green ) ) * 256;

    ired = (unsigned int) floor( red );
    igreen = (unsigned int) floor( green );
    iblue = (unsigned int) floor( blue );

    if(i==0)
    {
        iblue = ired;
        ired = igreen = 0;
    }
    else if (i==1)
    {
        iblue = igreen;
        igreen = ired;
        ired = 0;
    }

    color = (ired<<24) | (igreen<<16) | (iblue<<8) | 0;

    printf("clr: 0x%08X\n", color);

    return color;
}

int main( int argc, char* argv[] )
{
    char *a = argv[1];
    unsigned int pos = (unsigned int) atoi( a );
    unsigned int clr = getColor( pos );
    printf("Color: 0x%08X\n", clr);
}
