#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv( rgb in );
static rgb   hsv2rgb( hsv in );


hsv rgb2hsv( rgb in )
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb( hsv in )
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }

//    unsigned int ui_rgb = 0;
//    ui_rgb = (0x00<<24) | (floor(out.r*255)<<16) | (floor(out.g*255)<<8) | floor(iblue*255);

//    return ui_rgb;
    return out;
}


unsigned int getColor( double pct )
{
    uint32_t color = 0;
    hsv hsv_clr =
    {
        .h = floor( pct * 360 ),
        .s = 1,
        .v = 1,
    };

    rgb rgb_clr = hsv2rgb( hsv_clr );

    unsigned int red, green, blue;
    red = floor( rgb_clr.r * 255 );
    green = floor( rgb_clr.g * 255 );
    blue = floor( rgb_clr.b * 255 );
    color = (0x00<<24) | (red<<16) | (green<<8) | blue;

    return color;
}
/*
int main( int argc, char * argv[] )
{
    double pct = atof( argv[1] );
    printf( "input: %f\n", pct );
    uint32_t color = getColor( pct );
    printf( "0x%08x\n", color );

    return 0;
}
*/

unsigned int _getColor( unsigned int pos )
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
