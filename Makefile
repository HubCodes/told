CC=clang++
STD_FLAG=-std=c++14

OUTPUT_PATH=./bin
SRCS=./src/told/*.cpp
HDRS=./src/told/*.hpp

debug: $(SRCS) $(HDRS)
	$(CC) -g -DDEBUG_R $(STD_FLAG) $(SRCS) -v -o $(OUTPUT_PATH)/debug

told: $(SRCS) $(HDRS)
	$(CC) $(STD_FLAG) $(SRCS) -o $(OUTPUT_PATH)/told

clean:
	rm $(OUTPUT_PATH)/told
	rm $(OUTPUT_PATH)/debug
