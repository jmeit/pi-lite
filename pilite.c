#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lightgrid.h"
#include "mqtt.h"
#include "touchscreen.h"
#include "colours.h"


#define NUM_PANELS  4
// Amount of pixels on each panel in each axis
#define PANEL_W     32
#define PANEL_H     8
// Amount of pixels on the full grid in each axis
#define PXL_GRD_W   PANEL_W
#define PXL_GRD_H   ( PANEL_H * NUM_PANELS )

static uint8_t running = 1;
unsigned int hitbox_w, hitbox_h,
    old_x = 0, old_y = 0,
    pixel = 0;
uint32_t color = 0x00AABBCC;

void draw_to_matrix( unsigned int** crds )
{
    unsigned int x_raw= *crds[0];
    unsigned int y_raw= *crds[1];
    unsigned int x = fmin( floor( x_raw / hitbox_w ), PANEL_W-1 );
    unsigned int y = floor( y_raw / hitbox_h );
    char color_hexstr[15];
    sprintf( color_hexstr, "0x%08X", color );

    if ( x_raw > getTouchscreenWidth() )
    {
        unsigned int tmp = y_raw * ( getTouchscreenHeight() - 1 );
        uint32_t new_color = getColor( tmp );
        if ( new_color == color ){ return; }
        color = new_color;
        drawPixel( pixel, color );
        printf( "color: %s\n", color_hexstr );
        return;
    }

    // Debounce
    if( x == old_x && y == old_y ){ return; }
    old_x = x;
    old_y = y;

    pixel = coords_to_pixel( x, y );
    drawPixel( pixel, color );
    char message[25];
    sprintf( message, "%i,%i %s", x,y,color_hexstr );
    mqtt_publish( message );
}

static void ctrl_c_handler(int signum)
{
    (void)(signum);
    printf("quitting\n");
    running = 0;
    ws2811_destroy();
    mqtt_destroy();
}
static void setup_handlers(void)
{
    struct sigaction sa =
    {
        .sa_handler = ctrl_c_handler,
    };

    sigaction( SIGINT, &sa, NULL );
    sigaction( SIGTERM, &sa, NULL );
}


int main()
{
    hitbox_w = getTouchscreenWidth() / PXL_GRD_W;
    hitbox_h = getTouchscreenHeight() / PXL_GRD_H;

    setup_handlers();

    ws2811_setup( NUM_PANELS, PANEL_W, PANEL_H );
    mqtt_init();

    startTouchscreenRead( &running, draw_to_matrix );

    if( running )
    {
        ws2811_destroy();
        mqtt_destroy();
    }

    return 0;
}
