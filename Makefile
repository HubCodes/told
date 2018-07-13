<<<<<<< Updated upstream
CC=g++
=======
<<<<<<< Updated upstream
CC=clang++
>>>>>>> Stashed changes
STD_FLAG=-std=c++14
=======
CC=g++
STD_FLAG=-std=c++17
>>>>>>> Stashed changes

OUTPUT_PATH=./bin
SRCS=./src/told/*.cpp
HDRS=./src/told/*.hpp

debug: $(SRCS) $(HDRS)
	$(CC) -g $(STD_FLAG) $(SRCS) -o $(OUTPUT_PATH)/debug

told: $(SRCS) $(HDRS)
	$(CC) $(STD_FLAG) $(SRCS) -o $(OUTPUT_PATH)/told

clean:
	rm $(OUTPUT_PATH)/told
	rm $(OUTPUT_PATH)/debug

install:
	mkdir /usr/lib/told/
	cp .$(OUTPUT_PATH)/told /usr/bin/told
	cp -f -R ./lib/stdlib/*/*.th /usr/lib/told/th/
	cp -f -R ./lib/stdlib/*/*.to /usr/lib/told/to/