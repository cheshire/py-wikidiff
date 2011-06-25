#include "DiffEngine.h"
double _dist(const std::string & str1, const std::string & str2)
{
	int s1 = 0, s2 = 0;
	int e1 = str1.size();
	int e2 = str2.size();


	while (s1 < e1 && s2 < e2)
	{
		if (str1[s1] != str2[s2]) break;
		++s1;
		++s2;
	}


	while (e1>s1 && e2>s2) {
		if (str1[e1-1] != str2[e2-1]) break;
		--e1;
		--e2;
	}

	if (s1 == e1 && s2 == e2) return 0.0;
	
	int col1[e1-s1+1];
	int col2[e1-s1+1];

	int * temp;
	int * oldcol = col1;
	int * newcol = col2;
	
	for (int i = 0; i < (e1-s1+1); ++i) oldcol[i] = i;


	for (int j = 0; j < (e2-s2); ++j)
	{
		newcol[0] = j+1;
		for (int i = 0; i < (e1-s1); ++i)
		{
			newcol[i+1] = oldcol[i] + (str1[i]==str2[j] ? 0 : 1);
			if (newcol[i]+1 < newcol[i+1])
				newcol[i+1] = newcol[i]+1;
			if (oldcol[i+1]+1 < newcol[i+1])
				newcol[i+1] = oldcol[i+1]+1;
		}
		
		temp = oldcol;
		oldcol = newcol;
		newcol = temp;
	}
	double total = str1.size() < str2.size() ? str2.size() : str1.size();
	return (double)oldcol[e1-s1]/total;
}

extern "C" double cost(const char *text1, const char *text2)
{
	std::string s1(text1);
	std::string s2(text2);
	return _dist(s1, s2);
}
