NAME1 = keeper
NAME2 = client

CODE_FILES1 = keeper.c
CODE_FILES2 = client1.c

DOC_FILES = docs
DEBUG = YES
ifeq ($(DEBUG),YES)
	D = -g
else
	D =
endif

.PHONY: all clean docs

all: $(CODE_FILES)
	gcc -lrt -Wno-parentheses -fsanitize=undefined -Wall -g -o $(NAME1) $(CODE_FILES1)
	gcc -lrt -Wno-parentheses -fsanitize=undefined -Wall -g -o $(NAME2) $(CODE_FILES2)

#docs: Doxyfile
#	doxygen Doxyfile

#Doxyfile:
#	doxygen -g

clean:
	rm -rf $(NAME1) $(DOC_FILES) *.o
	rm -rf $(NAME2) $(DOC_FILES) *.o
