int coords_to_pixel( unsigned int c, unsigned int y );
void drawPixel( unsigned int pixel, uint32_t color );
int ws2811_setup( int _num_panels, int _panel_w, int _panel_h );
int ws2811_destroy();

