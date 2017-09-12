LDFLAGS = -lstdc++
BINDIR=/usr/local/bin

afmtoenc: afmtoenc.o

afmtoenc.o: afmtoenc.cpp unidata.c showglyphs.c

clean:
	-rm afmtoenc afmtoenc.o

install:
	install -m 755 -s afmtoenc $(BINDIR)
	install -m 755 ttfencinst.sh $(BINDIR)
	install -m 755 ttfallinst.sh $(BINDIR)
