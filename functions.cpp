#include "functions.h"

#include <cmath>
#include <vector>

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
