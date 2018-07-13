CC=../../bin/debug
AnL=clang

LIB_PATH=../../lib/stdlib
LIBS=$(LIB_PATH)/string/strsize $(LIB_PATH)/fio/putstr $(LIB_PATH)/fio/putc

CODE=./code.td
ASSEM=./code.s

OUTPUT=./first_test

all: $(OUTPUT)

$(OUTPUT): $(ASSEM)
	clang $^ $(LIBS) -o $@

$(ASSEM): $(CODE)
	$(CC) $^ -o $@

clean:
	rm $(OUTPUT) $(ASSEM)