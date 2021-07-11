#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bsd/string.h>


#include "lightgrid.h"
#include <mosquitto.h>
#include "mqtt.h"
#include "touchscreen.h"
#include "colours.h"
#include "macaddr.h"


#define NUM_PANELS  4
// Amount of pixels on each panel in each axis
#define PANEL_W     32
#define PANEL_H     8
// Amount of pixels on the full grid in each axis
#define PXL_GRD_W   PANEL_W
#define PXL_GRD_H   ( PANEL_H * NUM_PANELS )

static uint8_t running = 1;
unsigned int hitbox_w, hitbox_h,
    old_x = 5000, old_y = 5000,
    pixel = 0;
uint32_t color = 0x00AABBCC;
char user_id[ MAC_ADDR_LEN+1 ];

void draw_to_matrix( unsigned int** crds, unsigned int raw )
{
    unsigned int x,y;
    char color_hexstr[15];
    sprintf( color_hexstr, "%08X", color );

    if( raw )
    {
        unsigned int x_raw= *crds[0];
        unsigned int y_raw= *crds[1];

        if ( x_raw > getTouchscreenWidth() )
        {
            unsigned int tmp = y_raw * ( getTouchscreenHeight() - 1 );
            uint32_t new_color = getColor( tmp );
            if ( new_color == color ){ return; }
            color = new_color;
            drawPixel( pixel, color );
            // TODO publish color after debouncing (take last color)
            printf( "color: %s\n", color_hexstr );
            return;
        }

        x = fmin( floor( x_raw / hitbox_w ), PANEL_W-1 );
        y = floor( y_raw / hitbox_h );
        // Debounce
        // - Ignores touches to the same spot
        // - Will be removed (or altered) when erasing is implemented
        if( x == old_x && y == old_y ){ return; }
        old_x = x;
        old_y = y;
    }
    else
    {
        x = *crds[0];
        y = *crds[1];
    }

    pixel = coords_to_pixel( x, y );
    drawPixel( pixel, color );
    char message[25];
    sprintf( message, "%s %s %i,%i", user_id,color_hexstr,x,y );

    if( raw )
    {
        printf( "publishing message\n" );
        mqtt_publish( message );
    }
}

static void ctrl_c_handler(int signum)
{
    (void)(signum);
    printf("quitting\n");
    running = 0;
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


void on_mqtt_message( struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg )
{
    //char* topic = msg->topic;
    char* crds_clr = (char *) msg->payload;
    printf( "received: %s\n", crds_clr );

    // TODO Validate msg->payload with regex
    char* token = strtok( crds_clr, " " );

    // Skip publishes from self
    if( ! strcmp( token, user_id ) ) return;

    token = strtok( NULL, " " );
    color = (unsigned int) strtol( token, NULL, 16 );

    char* loc;
    loc = strtok( NULL, " " );
    unsigned int x,y;
    token = strtok( loc, "," );
    x = atoi(token);
    token = strtok( NULL, "," );
    y = atoi(token);
    unsigned int* coords[] = {&x,&y};
    draw_to_matrix( coords, 0 );
}

int main()
{
    hitbox_w = getTouchscreenWidth() / PXL_GRD_W;
    hitbox_h = getTouchscreenHeight() / PXL_GRD_H;

    setup_handlers();

    ws2811_setup( NUM_PANELS, PANEL_W, PANEL_H );
    mqtt_init( user_id, &on_mqtt_message );

    startTouchscreenRead( &running, draw_to_matrix );

    // Gets called when program quits with Ctrl+c
    ws2811_destroy();
    mqtt_destroy();

    return 0;
}
