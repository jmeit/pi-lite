unsigned int getTouchscreenWidth();
unsigned int getTouchscreenHeight();

int startTouchscreenRead( uint8_t *run, void (*callback)(unsigned int*[], unsigned int) );
