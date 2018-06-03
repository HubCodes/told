CC = clang++
STD = c++14

debug:
	$(CC) -g -std=$(STD) *.cpp -o debug

told:
	$(CC) -std=$(STD) *.cpp -o told

clean:
	rm told
	rm debug
