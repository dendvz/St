# include "functions.h"
#include <cmath>

JSAMPLE* image_buffer;	/* Points to large array of R,G,B-order data */

int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = w; 	/* image width and height, in pixels */
	cinfo.image_height = h;
	cinfo.input_components = comp;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = w * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	jpeg_destroy_compress(&cinfo);

	return 0;
}

int getint(std::string s) //возвращает первое число из строки
{
	int a = 0, i = 0;
	std::vector<char> v;
	while (i < s.length())
	{
		if ((s[i] >= '0') && (s[i] <= '9'))
		{
			v.push_back(s[i]);
			a++;
		}
		else if (a > 0)
		{
			break;
		}
		i++;
	}
	v.reserve(v.size());

	a = 0;
	int j = v.size() - 1;
	for (int i = 0; i < v.size(); i++)
	{
		a += (v[i] - '0') * pow(10, j);
		j--;
	}
	return a;
}
