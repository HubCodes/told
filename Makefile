CC = clang++
STD = c++14

debug: *.cpp *.hpp
	$(CC) -g -DDEBUG_R -std=$(STD) *.cpp -o debug

told: *.cpp *.hpp
	$(CC) -std=$(STD) *.cpp -o told

clean:
	rm told
	rm debug
