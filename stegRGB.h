# pragma once

# include <fstream>
# include <string>
# include "md5\md5wrapper.h"
# include "functions.h"



class RGB
{
private:
	int Height;
	int Width;
	int Comp;
	int** sol;  //массив, содержащий информацию о сплошных блоках
public:
	RGBQUAD**			rgb;

	RGB(int height, int width, int comp)
	{
		Height = height;
		Width = width;
		Comp = comp;
		rgb = new RGBQUAD*[height * sizeof(RGBQUAD*)];
		for (int i = 0; i < height; i++)
		{
			rgb[i] = new RGBQUAD[width * sizeof(RGBQUAD)];
		}

		sol = new int*[height * sizeof(int*)];
		for (int i = 0; i < height; i++)
		{
			sol[i] = new int[width * sizeof(int)];
		}
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				sol[i][j] = false;
	}
	~RGB()
	{
		for (int i = 0; i < Height; i++)
		{
			delete rgb[i];
		}
		delete[] rgb;
		for (int i = 0; i < Height; i++)
		{
			delete sol[i];
		}
		delete[] sol;
	}
	
	void		into_RGB(uint8_t* d, const JpegLoader::ImageInfo* pImageInfo)
	{
		int scanline = 0, j = 0, k = 0;
		int max_k = pImageInfo->nHeight * pImageInfo->nWidth * pImageInfo->nNumComponent;
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (k < max_k)
				{
					rgb[i][j].rgbBlue = d[k++];
					rgb[i][j].rgbGreen = d[k++];
					rgb[i][j].rgbRed = d[k++];
				}
			}
		}
	}
	void		out_RGB(uint8_t* d)
	{
		int k = 0;
		int max_k = Height * Width * Comp;
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (k < max_k)
				{
					d[k++] = rgb[i][j].rgbBlue;
					d[k++] = rgb[i][j].rgbGreen;
					d[k++] = rgb[i][j].rgbRed;
				}
			}
		}
	}
	
	void		find_solids()
	{
		for (int i = 0; i < Height - 8; i += 8)
		{
			for (int j = 0; j < Width - 8; j += 8)
			{
				if (solid_square(i, j, 8) == true)
				{
					sol[i][j] = true;
				}
			}
		}
	}
	bool		solid_square(int j_0, int i_0, int N)	//working
	{
		if ((j_0 > Height - 8) || (i_0 > Width))
		{
			//std::cout << "Out of boundaries";
			return false;
		}
		double v[8] = { 0 };
		int k = 0;
		int res_l = 100, res_h = 0;
		int length = 0, height = 0;
		for (int j = j_0; j < j_0 + N - 1; j++)
		{
			for (int i = i_0; i < i_0 + N - 1; i++)
			{
				if (((abs(rgb[j][i].rgbBlue - rgb[j][i + 1].rgbBlue)) < 10) && ((abs(rgb[j][i].rgbGreen - rgb[j][i + 1].rgbGreen)) < 10) && ((abs(rgb[j][i].rgbRed - rgb[j][i + 1].rgbRed)) < 10))
				{
					v[k] += (rgb[j][i].rgbBlue * 0.1 + rgb[j][i].rgbGreen * 0.6 + rgb[j][i].rgbRed * 0.3);//v[j] += (rgb[j][i].rgbBlue * 0.114 + rgb[j][i].rgbGreen * 0.587 + rgb[j][i].rgbRed * 0.299);
					continue;
				}
				else
				{
					return false;
				}
			}
			if ((k > 0) && (abs(v[k] - v[k - 1]) >= 50))
			{
				return false;
			}
			k++;
		}
		return true;
	}
	bool		check_for_solids(int x)
	{
		if (x < 0)
		{
			return false;
		}
		if (sol[x][x] == true)
			return true;
	}

	void		code_00xx_0000(std::string s, int row, int col)
	{
		if ((row > Height - 8) || (col > Width - 8))
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
					case 0:	cur = (cur_val & 192) >> 2; pos++; break;	    //1100 0000
					case 1:	cur = cur_val & 48; pos++; break;			        //0011 0000
					case 2:	cur = (cur_val & 12) << 2; pos++; break;		  //0000 1100
					case 3:	cur = (cur_val & 3) << 4; pos++; break;		    //0000 0011
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
	void		code_xx00_0000(std::string s, int row, int col)	//xx00 0000
	{
		if ((row > Height - 8) || (col > Width - 8))
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
					case 0:	cur = (cur_val & 192); pos++; break;	        //1100 0000
					case 1:	cur = (cur_val & 48) << 2; pos++; break;			//0011 0000
					case 2:	cur = (cur_val & 12) << 4; pos++; break;		  //0000 1100
					case 3:	cur = (cur_val & 3) << 6; pos++; break;		    //0000 0011
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
	int			find_block_and_hide(std::string for_hash, std::string to_hide)
	{
		find_solids();
		int secret_number = 0;
		std::string hash = get_hash(for_hash);
		int x = get_x(hash);
		while (check_for_solids(x) != true)
		{
			hash = get_hash(hash);
			x = get_x(hash);
			secret_number += 2;
			if (secret_number > 100 && x > 0) { break; }  //если вычисленно больше 50 хэшей подряд
		}
		code_00xx_0000(to_hide, x, x);
		return secret_number;
	}

	std::string get_mes_from_00xx_0000(int max_len, int row, int col)
	{
		if ((row > Height - 8) || (col > Width - 8))
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
					case 0:	color = rgb[i][j].rgbBlue; colour++; break;       //1100 0000
					case 1:	color = rgb[i][j].rgbGreen; colour++; break;	    //0011 0000
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
		if ((row > Height - 8) || (col > Width - 8))
		{
			return "Out of boundaries";
		}
		std::string finalle;
		int mask = 192;//0011 0000
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
					case 0:	color = rgb[i][j].rgbBlue; colour++; break;       //1100 0000
					case 1:	color = rgb[i][j].rgbGreen; colour++; break;	    //0011 0000
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
	std::string find_block_and_return_mes(std::string for_hash, int secret_number)
	{
		int count = 0;
		std::string hash = get_hash(for_hash);
		int x = get_x(hash);
		while (count != secret_number)
		{
			hash = get_hash(hash);
			x = get_x(hash);
			count += 2;
		}
		return get_mes_from_00xx_0000(for_hash.length()*3, x, x);
	}

	std::string get_hash(std::string str)
	{
		md5wrapper md5;
		return md5.getHashFromString(str);
	}
	int get_x(std::string s)
	{
		int x = getint(s);
		if ((x < Height) && (Width))
		{
			return x;
		}
		else
		{
			return -1;
		}
	}
};

