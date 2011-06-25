#include <stdio.h>
#include <string.h>

#include "pywikidiff2.h"

extern "C" void free_diffs(struct pyDiff * diffops, int n_entries)
{
	for (int i = 0; i < n_entries; ++i)
	{
		free(diffops[i].from);
		free(diffops[i].to);
	}
	free(diffops);
}

extern "C" struct pyDiff * diff_lines(const char * text1, const char * text2, int * n_entries)
{

	std::vector<std::string> lines1;
	std::vector<std::string> lines2;

	line_explode(text1, lines1);
	line_explode(text2, lines2);

	Diff<std::string> linediff(lines1, lines2);
	int num_ops = linediff.size();

	*n_entries = 0;

	for (int i = 0; i < num_ops; ++i)
	{
		switch (linediff[i].op) {
			case DiffOp<std::string>::add:
				*n_entries += linediff[i].to.size();
				break;
			case DiffOp<std::string>::del:
				*n_entries += linediff[i].from.size();
				break;
			case DiffOp<std::string>::change:
			case DiffOp<std::string>::copy:
				*n_entries += std::max(linediff[i].from.size(),
						linediff[i].to.size());
				break;
		}
	}

	struct pyDiff * diffops = (struct pyDiff *)calloc(*n_entries, sizeof(struct pyDiff));
	int curop = 0;

	for (int i = 0; i < num_ops; ++i)
	{
		int n,n1,n2;
		switch (linediff[i].op){
			case DiffOp<std::string>::add:
				n = linediff[i].to.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_ADD;
					diffops[curop].from = NULL;
					diffops[curop].to = strdup(linediff[i].to[j]->c_str());
				}
				break;
			case DiffOp<std::string>::del:
				n = linediff[i].from.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_DEL;
					diffops[curop].from = strdup(linediff[i].from[j]->c_str());
					diffops[curop].to = NULL;
				}
				break;
			case DiffOp<std::string>::copy:
				n = linediff[i].to.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_COPY;
					diffops[curop].from = strdup(linediff[i].from[j]->c_str());
					diffops[curop].to = strdup(linediff[i].to[j]->c_str());
				}
				break;
			case DiffOp<std::string>::change:
				n1 = linediff[i].from.size();
				n2 = linediff[i].to.size();
				n = std::min(n1, n2);
				for (int j=0; j<n; ++j, ++curop)
				{
					diffops[curop].op = OP_CHANGE;
					diffops[curop].from = strdup(linediff[i].from[j]->c_str());
					diffops[curop].to = strdup(linediff[i].to[j]->c_str());
				}
				if (n1 > n2)
				{
					for (int j=n2; j<n1; ++j, ++curop)
					{
						diffops[curop].op = OP_DEL;
						diffops[curop].from = strdup(linediff[i].from[j]->c_str());
						diffops[curop].to = NULL;
					}
				}
				else if (n2 > n1)
				{
					for (int j=n1; j<n2; ++j, ++curop)
					{
						diffops[curop].op = OP_ADD;
						diffops[curop].from = NULL;
						diffops[curop].to = strdup(linediff[i].to[j]->c_str());
					}
				}
				break;
		}
	}
	return diffops;
}

extern "C" struct pyDiff * diff_words(const char * line1, const char * line2, int * n_entries)
{

	std::string s1 = line1;
	std::string s2 = line2;

	std::vector<Word> words1;
	std::vector<Word> words2;

	split_tokens(s1, words1);
	split_tokens(s2, words2);

	Diff<Word> worddiff(words1, words2);
	int num_ops = worddiff.size();

	*n_entries = 0;

	for (int i = 0; i < num_ops; ++i)
	{
		switch (worddiff[i].op) {
			case DiffOp<Word>::add:
				*n_entries += worddiff[i].to.size();
				break;
			case DiffOp<Word>::del:
				*n_entries += worddiff[i].from.size();
				break;
			case DiffOp<Word>::change:
			case DiffOp<Word>::copy:
				*n_entries += std::max(worddiff[i].from.size(),
						worddiff[i].to.size());
				break;
		}
	}

	struct pyDiff * diffops = (struct pyDiff *)calloc(*n_entries, sizeof(struct pyDiff));
	int curop = 0;

	for (int i = 0; i < num_ops; ++i)
	{
		int n,n1,n2;
		switch (worddiff[i].op){
			case DiffOp<Word>::add:
				n = worddiff[i].to.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_ADD;
					diffops[curop].from = NULL;
					diffops[curop].to = strdup(worddiff[i].to[j]->whole().c_str());
				}
				break;
			case DiffOp<Word>::del:
				n = worddiff[i].from.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_DEL;
					diffops[curop].from = strdup(worddiff[i].from[j]->whole().c_str());
					diffops[curop].to = NULL;
				}
				break;
			case DiffOp<Word>::copy:
				n = worddiff[i].to.size();
				for (int j = 0; j < n; ++j, ++curop)
				{
					diffops[curop].op = OP_COPY;
					diffops[curop].from = strdup(worddiff[i].from[j]->whole().c_str());
					diffops[curop].to = strdup(worddiff[i].to[j]->whole().c_str());
				}
				break;
			case DiffOp<Word>::change:
				n1 = worddiff[i].from.size();
				n2 = worddiff[i].to.size();
				n = std::min(n1, n2);
				for (int j=0; j<n; ++j, ++curop)
				{
					diffops[curop].op = OP_CHANGE;
					diffops[curop].from = strdup(worddiff[i].from[j]->whole().c_str());
					diffops[curop].to = strdup(worddiff[i].to[j]->whole().c_str());
				}
				if (n1 > n2)
				{
					for (int j=n2; j<n1; ++j, ++curop)
					{
						diffops[curop].op = OP_DEL;
						diffops[curop].from = strdup(worddiff[i].from[j]->whole().c_str());
						diffops[curop].to = NULL;
					}
				}
				else if (n2 > n1)
				{
					for (int j=n1; j<n2; ++j, ++curop)
					{
						diffops[curop].op = OP_ADD;
						diffops[curop].from = NULL;
						diffops[curop].to = strdup(worddiff[i].to[j]->whole().c_str());
					}
				}
				break;
		}
	}
	return diffops;
}
