#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

unsigned int getColor( unsigned int pos )
{
    uint32_t color;
    double red, green, blue;
    double max = 256.0;
    unsigned int ired, igreen, iblue;
    int i=0;

    red = (double) pos;

    while( red >= max )
    {
        i++;
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

    color = (0x00<<24) | (ired<<16) | (igreen<<8) | iblue;

    return color;
}
