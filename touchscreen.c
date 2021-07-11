#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define WIDTH      3400
#define HEIGHT     4096
/*
#define EV_ABS     3
#define EV_KEY     1
#define ABS_X      0
#define ABS_Y      1
#define BTN_LEFT   272
*/
#define BTN_DN     1
#define BTN_UP     0

unsigned int getTouchscreenWidth()
{
    return (unsigned int) WIDTH;
}

unsigned int getTouchscreenHeight()
{
    return (unsigned int) HEIGHT;
}

int startTouchscreenRead( uint8_t *run, void (*callback)(unsigned int*[], unsigned int) )
{
    int fd;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int* crds[2] = {&x,&y};
    struct input_event ev;

    if ((fd = open("/dev/input/event0", O_RDONLY)) < 0) {
        perror("evdev open");
        exit(1);
    }

    while( *run ) {
        read( fd, &ev, sizeof( struct input_event ) );
        if( ev.type == EV_ABS )
        {
            if( ev.code == ABS_X )
            {
                x = ev.value;
            }
            else if( ev.code == ABS_Y )
            {
                y = ev.value;
            }
        }
        //else if( ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1 && x && y )
        else if( ev.type == EV_SYN && ev.code == SYN_REPORT && x && y )
        {
            //printf("(%i,%i)\n",x,y);
            (*callback)( crds, 1 );
        }
    }

    return 0;
}
