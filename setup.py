from distutils.core import setup, Extension

wikidiff2 = Extension('wikipedia.wikidiff2.wikidiff2',
		['wikidiff2/wikidiff2.cpp', 'wikidiff2/pywikidiff2.cpp', 'wikidiff2/DiffEngine.cpp'])

setup(name="wikipedia",
	ext_modules=[wikidiff2],
	packages = ['wikipedia',
		'wikipedia.wikidiff2'],
	package_dir = {'wikipedia':''},
	)
