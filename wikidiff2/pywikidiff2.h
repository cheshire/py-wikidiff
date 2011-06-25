#ifndef PYWIKIDIFF2_H
#define PYWIKIDIFF2_H

#include "wikidiff2.h"

struct pyDiff {
	int op;
	char * from;
	char * to;
};

void line_explode(const char *, std::vector<std::string> &);
void split_tokens(const std::string &, std::vector<Word> &);


#define OP_ADD 0
#define OP_DEL 1
#define OP_COPY 2
#define OP_CHANGE 3

#endif

