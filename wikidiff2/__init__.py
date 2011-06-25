from ctypes import cdll, byref, c_char_p, c_int, c_double, Structure, POINTER
import os

PATH = os.path.dirname(__file__)
WIKIDIFF_NAME = "wikidiff2.so"
WIKIDIFF_HTML_FUNC = "wikidiff2_do_diff"
WIKIDIFF_HTML_WORD_FUNC = "wikidiff2_do_word_diff"
WIKIDIFF_LINE_FUNC = "diff_lines"
WIKIDIFF_WORD_FUNC = "diff_words"
WIKIDIFF_FREE_FUNC = "free_diffs"

OP_ADD = 0
OP_DEL = 1
OP_COPY = 2
OP_CHANGE = 3

class pyDiff(Structure):
	_fields_ = [("op", c_int),
				("from", c_char_p),
				("to", c_char_p),]

pyDiff_p = POINTER(pyDiff)

c_int_p = POINTER(c_int)


wikidiff = cdll.LoadLibrary(os.path.join(PATH,WIKIDIFF_NAME))

cost = getattr(wikidiff, "cost")
cost.restype = c_double
cost.argtypes = [c_char_p, c_char_p]

wikidiff_html_func = getattr(wikidiff, WIKIDIFF_HTML_FUNC)
wikidiff_html_func.restype = c_char_p
wikidiff_html_func.argtypes = [c_char_p, c_char_p, c_int]

wikidiff_html_word_func = getattr(wikidiff, WIKIDIFF_HTML_WORD_FUNC)
wikidiff_html_word_func.restype = c_char_p
wikidiff_html_word_func.argtypes = [c_char_p, c_char_p]

wikidiff_line_func = getattr(wikidiff, WIKIDIFF_LINE_FUNC)
wikidiff_line_func.restype = pyDiff_p
wikidiff_line_func.argtypes = [c_char_p, c_char_p, c_int_p]

wikidiff_word_func = getattr(wikidiff, WIKIDIFF_WORD_FUNC)
wikidiff_word_func.restype = pyDiff_p
wikidiff_word_func.argtypes = [c_char_p, c_char_p, c_int_p]

wikidiff_free_func = getattr(wikidiff, WIKIDIFF_FREE_FUNC)
wikidiff_free_func.restype = None
wikidiff_free_func.argtypes = [pyDiff_p, c_int]

def _table_wrap(s):
	return u"""<table class='diff'>
				<col class='diff-marker' />
				<col class='diff-content' />
				<col class='diff-marker' />
				<col class='diff-content' />
				%s
			</table>""" % s



def diff2html(text1, text2, num_lines=5):
	if not isinstance(text1, (unicode,str)) or not isinstance(text2, (unicode,str)):
		raise ValueError("First two arguments must be type unicode or str.")
	
	if isinstance(text1, unicode):
		text1 = text1.encode("utf-8","ignore")
	if isinstance(text2, unicode):
		text2 = text2.encode("utf-8","ignore")

	return _table_wrap(wikidiff_html_func(text1, text2, num_lines).decode('utf-8'))


def diff2wordhtml(text1, text2):
	if not isinstance(text1, (unicode,str)) or not isinstance(text2, (unicode,str)):
		raise ValueError("First two arguments must be type unicode or str.")
	
	if isinstance(text1, unicode):
		text1 = text1.encode("utf-8","ignore")
	if isinstance(text2, unicode):
		text2 = text2.encode("utf-8","ignore")

	return _table_wrap(wikidiff_html_word_func(text1, text2).decode('utf-8'))

def diff_generic(func, text1, text2):
	if not isinstance(text1, (unicode,str)) or not isinstance(text2, (unicode,str)):
		raise ValueError("First two arguments must be type unicode or str.")

	if isinstance(text1, unicode):
		text1 = text1.encode("utf-8","ignore")
	if isinstance(text2, unicode):
		text2 = text2.encode("utf-8","ignore")

	n = c_int(0)
	diffops = func(text1, text2, byref(n))

	res = []
	for i in xrange(n.value):
		diffop = diffops[i]
		f = getattr(diffop, 'from')
		t = getattr(diffop, 'to')
		if f:
			f = f.decode('utf-8')
		if t:
			t = t.decode('utf-8')
		if f or t:
			res.append((diffop.op,f,t))

	wikidiff_free_func(diffops, n)
	
	return res

def diff_lines(text1, text2):
	return diff_generic(wikidiff_line_func, text1, text2)

def diff_words(line1, line2):
	return diff_generic(wikidiff_word_func, line1, line2)
