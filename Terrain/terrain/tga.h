#ifndef TGA_H
#define TGA_H


#ifndef byte
typedef unsigned char byte;
#endif


byte*	load_tga( const char *name, int *width, int *height, int *BPP );
int		save_tga( const char *name, int  width, int  height, byte *img, int BPP=24 );



#endif
