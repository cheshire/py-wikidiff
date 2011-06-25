CXXFLAGS=-fPIC
libwikidiff.so : wikidiff2.o pywikidiff2.o DiffEngine.o
	$(CXX) $(CXXFLAGS) -shared DiffEngine.o wikidiff2.o pywikidiff2.o -o wikidiff2.so

clean:
	rm -f *.o wikidiff2.so || true

