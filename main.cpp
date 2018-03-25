# include "rgb_class.h"

int check_file(int argc, std::ifstream& fin);
int check_file(int argc, std::ofstream& fout);
void read_text(std::ifstream& fin, std::vector<char>& v);

int main(int argc, const char *argv[])
{
	setlocale(LC_ALL, "Russian");
	argc = 3;
	
	argv[1] = "C:\\Users\\Anna\\Desktop\\ex.bmp";
	argv[2] = "C:\\Users\\Anna\\Desktop\\11111.bmp";
	

	std::ifstream fin(argv[1], std::ios::binary);
	std::ofstream fout(argv[2], std::ios::binary);

	if ((check_file(argc, fin) != 0) && (check_file(argc, fout) != 0))
	{
		return 1;
	}

	
	BMP b;
	b.readINFO(fin);
	b.readRGB(fin);
	
	b.find_solids();
	std::string s = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	
	md5wrapper m;
	std::string hash = m.getHashFromString(s);
	int x = -1;
	while (b.check_for_solids(x) != true)
	{
		hash = m.getHashFromString(hash);
		x = b.get_x(hash);
	}
	b.code_00xx_0000(s, x, x);
	//std::cout << b.get_mes_from_00xx_0000(s.length(), 0, 0);

	b.writeINFO(fout);
	b.writeRGB(fout);
	
	return 0;
}


