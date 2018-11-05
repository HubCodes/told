CC=../../told
AnL=clang

LIB_PATH=../../lib/stdlib
LIBS=`ls $(LIB_PATH)/fio/* $(LIB_PATH)/string/* $(LIB_PATH)/memory/* | cat | grep -v '\.s' | grep -v '\.td'`

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
