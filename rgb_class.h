# pragma once

# include "rgb_structure.h"
# include <fstream>
# include <iostream>
# include <string>
# include <vector>
# include "md5\md5wrapper.h"
# include <bitset>
int getint(std::string s);

class BMP
{
public:
	BITMAPFILEHEADER	header;
	BITMAPINFOHEADER	bmiHeader;
	RGBQUAD**			rgb;
	int** sol;

	BMP() {}
	~BMP()
	{
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			delete rgb[i];
		}
		delete[] rgb;
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			delete sol[i];
		}
		delete[] sol;
	}
	void readINFO(std::ifstream& fin)
	{
		//header
		fin.read((char*)&header.bfType, sizeof(header.bfType));
		fin.read((char*)&header.bfSize, sizeof(header.bfSize));
		fin.read((char*)&header.bfReserved1, sizeof(header.bfReserved1));
		fin.read((char*)&header.bfReserved2, sizeof(header.bfReserved2));
		fin.read((char*)&header.BfOffBits, sizeof(header.BfOffBits));
		//bmiHeader
		fin.read((char*)&bmiHeader.biSize, sizeof(bmiHeader.biSize));
		fin.read((char*)&bmiHeader.biWidth, sizeof(bmiHeader.biWidth));
		fin.read((char*)&bmiHeader.biHeight, sizeof(bmiHeader.biHeight));
		fin.read((char*)&bmiHeader.biPlanes, sizeof(bmiHeader.biPlanes));
		fin.read((char*)&bmiHeader.biBitCount, sizeof(bmiHeader.biBitCount));
		fin.read((char*)&bmiHeader.biCompression, sizeof(bmiHeader.biCompression));
		fin.read((char*)&bmiHeader.biSizeImage, sizeof(bmiHeader.biSizeImage));
		fin.read((char*)&bmiHeader.biXPelsPerMeter, sizeof(bmiHeader.biXPelsPerMeter));
		fin.read((char*)&bmiHeader.biYPelsPerMeter, sizeof(bmiHeader.biYPelsPerMeter));
		fin.read((char*)&bmiHeader.biClrUsed, sizeof(bmiHeader.biClrUsed));
		fin.read((char*)&bmiHeader.biClrImportant, sizeof(bmiHeader.biClrImportant));
	}
	void readRGB(std::ifstream& fin)
	{
		rgb = new RGBQUAD*[bmiHeader.biHeight * sizeof(RGBQUAD*)];
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			rgb[i] = new RGBQUAD[bmiHeader.biWidth * sizeof(RGBQUAD)];
		}
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			for (int j = 0; j < bmiHeader.biWidth; j++)
			{
				rgb[i][j].rgbBlue = fin.get();	
				rgb[i][j].rgbGreen = fin.get();
				rgb[i][j].rgbRed = fin.get();
			}
		}
	}
	void writeINFO(std::ofstream& fout)
	{
		//header
		fout.write((char*)&header.bfType, sizeof(header.bfType));
		fout.write((char*)&header.bfSize, sizeof(header.bfSize));
		fout.write((char*)&header.bfReserved1, sizeof(header.bfReserved1));
		fout.write((char*)&header.bfReserved2, sizeof(header.bfReserved2));
		fout.write((char*)&header.BfOffBits, sizeof(header.BfOffBits));
		//bmiHeader
		fout.write((char*)&bmiHeader.biSize, sizeof(bmiHeader.biSize));
		fout.write((char*)&bmiHeader.biWidth, sizeof(bmiHeader.biWidth));
		fout.write((char*)&bmiHeader.biHeight, sizeof(bmiHeader.biHeight));
		fout.write((char*)&bmiHeader.biPlanes, sizeof(bmiHeader.biPlanes));
		fout.write((char*)&bmiHeader.biBitCount, sizeof(bmiHeader.biBitCount));
		fout.write((char*)&bmiHeader.biCompression, sizeof(bmiHeader.biCompression));
		fout.write((char*)&bmiHeader.biSizeImage, sizeof(bmiHeader.biSizeImage));
		fout.write((char*)&bmiHeader.biXPelsPerMeter, sizeof(bmiHeader.biXPelsPerMeter));
		fout.write((char*)&bmiHeader.biYPelsPerMeter, sizeof(bmiHeader.biYPelsPerMeter));
		fout.write((char*)&bmiHeader.biClrUsed, sizeof(bmiHeader.biClrUsed));
		fout.write((char*)&bmiHeader.biClrImportant, sizeof(bmiHeader.biClrImportant));
	}
	void writeRGB(std::ofstream& fout)
	{
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			for (int j = 0; j < bmiHeader.biWidth; j++)
			{
				fout.put(rgb[i][j].rgbBlue);
				fout.put(rgb[i][j].rgbGreen);
				fout.put(rgb[i][j].rgbRed);
			}
		}
	}

	void brightness(int percent)
	{
		for (int i = 0; i < bmiHeader.biWidth; i++)
		{
			for (int j = 0; j < bmiHeader.biHeight; j++)
			{
				rgb[i][j].rgbBlue = rgb[i][j].rgbBlue + percent * 128 / 100;
				rgb[i][j].rgbGreen = rgb[i][j].rgbGreen + percent * 128 / 100;
				rgb[i][j].rgbRed = rgb[i][j].rgbRed + percent * 128 / 100;
				//rgb[i][j].rgbReserved = rgb[i][j].rgbReserved + percent * 128 / 100;
				////контроль переполнения
				if (rgb[i][j].rgbRed < 0) rgb[i][j].rgbRed = 0;
				if (rgb[i][j].rgbRed > 255) rgb[i][j].rgbRed = 255;
				if (rgb[i][j].rgbGreen < 0) rgb[i][j].rgbGreen = 0;
				if (rgb[i][j].rgbGreen > 255) rgb[i][j].rgbGreen = 255;
				if (rgb[i][j].rgbBlue < 0) rgb[i][j].rgbBlue = 0;
				if (rgb[i][j].rgbBlue > 255) rgb[i][j].rgbBlue = 255;
			}
		}
	}
	void change_pixel(int i, int j, int val)
	{
		rgb[i][j].rgbBlue = val;
		rgb[i][j].rgbGreen = val;
		rgb[i][j].rgbRed = val;
	}
	bool solid_square(int j_0, int i_0, int N)	//working
	{
		if ((j_0 > bmiHeader.biHeight) || (i_0 > bmiHeader.biWidth))
		{
			//std::cout << "Out of boundaries";
			return false;
		}
		double v[8] = {0};
		int k = 0;
		int res_l = 100, res_h = 0;
		int length = 0, height = 0;
		for (int j = j_0; j < j_0 + N - 1; j++)
		{
			for (int i = i_0; i < i_0 + N - 1; i++)
			{
				if (((abs(rgb[j][i].rgbBlue - rgb[j][i + 1].rgbBlue)) < 10) && ((abs(rgb[j][i].rgbGreen - rgb[j][i + 1].rgbGreen)) < 10) && ((abs(rgb[j][i].rgbRed - rgb[j][i + 1].rgbRed)) < 10))
				{
					v[k] += (rgb[j][i].rgbBlue * 0.1 + rgb[j][i].rgbGreen * 0.6 + rgb[j][i].rgbRed * 0.3);/
					continue;
				}
				else
				{
					return false;
				}
			}
			if ((k > 0)&&(abs(v[k] - v[k - 1]) >= 50))
			{
				return false;
			}
			k++;
		}
		return true;
	}		
	void find_solids()
	{
		sol = new int*[bmiHeader.biHeight * sizeof(int*)];
		for (int i = 0; i < bmiHeader.biHeight; i++)
		{
			sol[i] = new int[bmiHeader.biWidth * sizeof(int)];
		}
		for (int i = 0; i < bmiHeader.biHeight - 8; i += 8)
		{
			for (int j = 0; j < bmiHeader.biWidth - 8; j += 8)
			{
				if (solid_square(i, j, 8) == true)
				{
					sol[i][j] = true;
				}
			}
		}
	}

	void code_00xx_0000(std::string s, int row, int col)
	{
		if ((row > bmiHeader.biHeight) || (col > bmiHeader.biWidth))
		{
			//std::cout << "Out of boundaries";
			return;
		}
		int mask = 207;	////207(10) = 1100 1111(2)
		int k = 0;
		char cur;
		int pos = 0, colour = 0;
		std::string finale;
		for (int i = row; i < row + 8; i++)
		{
			for (int j = col; j < col + 8; )							
			{															
				if (k < s.length())
				{
					int cur_val = s[k];
					pos = pos % 4;
					switch (pos)
					{
					case 0:	cur = (cur_val & 192) >> 2; pos++; break;	//1100 0000
					case 1:	cur = cur_val & 48; pos++; break;			//0011 0000
					case 2:	cur = (cur_val & 12) << 2; pos++; break;		//0000 1100
					case 3:	cur = (cur_val & 3) << 4; pos++; break;		//0000 0011
					}
				
					if (pos == 4) 
					{ 
						k++; 
					}

					colour = colour % 3;
					switch (colour)
					{
					case 0: rgb[i][j].rgbBlue = rgb[i][j].rgbBlue & mask; rgb[i][j].rgbBlue = rgb[i][j].rgbBlue | cur; colour++; break;
					case 1: rgb[i][j].rgbGreen = rgb[i][j].rgbGreen & mask; rgb[i][j].rgbGreen = rgb[i][j].rgbGreen | cur; colour++; break;
					case 2: rgb[i][j].rgbRed = rgb[i][j].rgbRed & mask; rgb[i][j].rgbRed = rgb[i][j].rgbRed | cur; colour++; j++; break;
					}
					cur = 0;
				}
				else { return; }
			}
		}
	}	//00xx 0000
	void code_xx00_0000(std::string s, int row, int col)	//xx00 0000
	{
		if ((row > bmiHeader.biHeight) || (col > bmiHeader.biWidth))
		{
			//std::cout << "Out of boundaries";
			return;
		}
		int mask = 63;	////207(10) = 1100 1111(2)
		int k = 0;
		char cur;
		int pos = 0, colour = 0;
		std::string finale;
		for (int i = row; i < row + 8; i++)
		{
			for (int j = col; j < col + 8; )
			{
				if (k < s.length())
				{
					int cur_val = s[k];
					pos = pos % 4;
					switch (pos)
					{
					case 0:	cur = (cur_val & 192); pos++; break;	//1100 0000
					case 1:	cur = (cur_val & 48) << 2; pos++; break;			//0011 0000
					case 2:	cur = (cur_val & 12) << 4; pos++; break;		//0000 1100
					case 3:	cur = (cur_val & 3) << 6; pos++; break;		//0000 0011
					}

					if (pos == 4)
					{
						k++;
					}

					colour = colour % 3;
					switch (colour)
					{
					case 0: rgb[i][j].rgbBlue = rgb[i][j].rgbBlue & mask; rgb[i][j].rgbBlue = rgb[i][j].rgbBlue | cur; colour++; break;
					case 1: rgb[i][j].rgbGreen = rgb[i][j].rgbGreen & mask; rgb[i][j].rgbGreen = rgb[i][j].rgbGreen | cur; colour++; break;
					case 2: rgb[i][j].rgbRed = rgb[i][j].rgbRed & mask; rgb[i][j].rgbRed = rgb[i][j].rgbRed | cur; colour++; j++; break;
					}
					cur = 0;
				}
				else { return; }
			}
		}
	}
	std::string get_mes_from_00xx_0000(int max_len, int row, int col)
	{
		if ((row > bmiHeader.biHeight) || (col > bmiHeader.biWidth))
		{
			return "Out of boundaries";
		}
		std::string finalle;
		int mask = 48;//0011 0000
		int str = 0, pos = 0, cur = 0;
		char colour = 0;
		int color = 0;
		char word = 0;
		int count = 0;
		
		for (int i = row; i < row + 8; i++)
		{
			for (int j = col; j < col + 8;)							//252(10) = 1111 1100(2)
			{
				if (count < max_len)
				{
					colour = colour % 3;
					switch (colour)
					{
					case 0:	color = rgb[i][j].rgbBlue; colour++; break;//1100 0000
					case 1:	color = rgb[i][j].rgbGreen; colour++; break;	//0011 0000
					case 2:	color = rgb[i][j].rgbRed; colour++; j++;  break;	//0000 1100
					}

					pos = pos % 4;
					switch (pos)
					{
					case 0: word = word | (color & mask) << 2; pos++; break;
					case 1: word = word | (color & mask); pos++; break;
					case 2: word = word | (color & mask) >> 2; pos++;  break;
					case 3: word = word | (color & mask) >> 4; pos++; break;
					}
					if (pos == 4)
					{
						finalle += word;
						word = 0;
						count++;
					}
				}
				else 
				{
					return finalle;
				}
			}
		}
		return finalle;
	}
	std::string get_mes_from_xx00_0000(int max_len, int row, int col)
	{
		if ((row > bmiHeader.biHeight) || (col > bmiHeader.biWidth))
		{
			return "Out of boundaries";
		}
		std::string finale;
		int mask = 192; //0011 0000
		int str = 0, pos = 0, cur = 0;
		char colour = 0;
		int color = 0;
		char word = 0;
		int count = 0;

		for (int i = row; i < row + 8; i++)
		{
			for (int j = col; j < col + 8;)							//252(10) = 1111 1100(2)
			{
				if (count < max_len)
				{
					colour = colour % 3;
					switch (colour)
					{
					case 0:	color = rgb[i][j].rgbBlue; colour++; break;//1100 0000
					case 1:	color = rgb[i][j].rgbGreen; colour++; break;	//0011 0000
					case 2:	color = rgb[i][j].rgbRed; colour++; j++;  break;	//0000 1100
					}

					pos = pos % 4;
					switch (pos)
					{
					case 0: word = word | (color & mask); pos++; break;
					case 1: word = word | (color & mask) >> 2; pos++; break;
					case 2: word = word | (color & mask) >> 4; pos++;  break;
					case 3: word = word | (color & mask) >> 6; pos++; break;
					}
					if (pos == 4)
					{
						finale += word;
						word = 0;
						count++;
					}
				}
				else
				{
					return finale;
				}
			}
		}
		return finale;
	}

	std::string md5String(std::string str)
	{
		md5wrapper md5;
		return md5.getHashFromString(str);
	}
	int get_x(std::string s)
	{
		int x = getint(s);
		if ((x < bmiHeader.biHeight) && (bmiHeader.biWidth))
		{
			return x;
		}
		else
		{
			return -1;
		}
	}
};

