CC=g++
CFLAGS=-Wc++11-extensions -Wall -std=c++0x -pedantic -g
PNAME=betterSpellChecker 
spellChecker: main.cpp
	$(CC) $(CFLAGS) -o $(PNAME) main.cpp

clean:
	rm $(PNAME)