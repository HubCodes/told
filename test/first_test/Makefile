CC=../../bin/debug
AnL=clang

LIB_PATH=../../lib/stdlib
LIBS=$(LIB_PATH)/string/strsize $(LIB_PATH)/fio/putstr $(LIB_PATH)/fio/putc

CODE=./code.txt
ASSEM=./code.s

OUTPUT=./first_test

all: $(OUTPUT)

$(OUTPUT): $(ASSEM)
	clang $^ $(LIBS) -o $@

$(ASSEM): $(CODE)
	$(CC) $^ > $@

clean:
	rm $(OUTPUT) $(ASSEM)