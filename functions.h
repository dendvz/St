#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "JpegLoader.h"
#include "stb_image_write.h"
#include <iostream>
#include "rgb_structure.h"
#include <vector>

extern int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);
extern int getint(std::string s);


extern JSAMPLE* image_buffer;	/* Points to large array of R,G,B-order data */

#endif
