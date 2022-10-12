CC = g++ -g -std=c++17
LDFLAGS = -Wall -Wextra -Werror -std=c++17 -g

OS=$(shell uname -s)

BD= build/

FILES = $(SBT).a\
	hash_table.a\
	./view/console_interface.cc\
	./controller/controller.cc\
	./main.cc

FILES_HASH_TABLE = ./core/virtual_base.h \
	./hash_table/hash_table.cc

FILES_SBT = ./core/virtual_base.h \
	./self_balancing_binary_search_tree/self_balancing_binary_search_tree.cc

SBT = self_balancing_binary_search_tree

all:

build: clean $(SBT).a hash_table.a
	mkdir -p $(BD)
	$(CC) $(LDFLAGS) $(FILES) -o $(BD)transactions.out -L. -l$(SBT) -lhash_table
	$(BD)transactions.out

tests:

hash_table.a:
	$(CC) -c $(FILES_HASH_TABLE)
	ar rcs hash_table.a *.o
	ranlib $@
	cp $@ lib$@

$(SBT).a: clean_sbt
	$(CC) -c $(FILES_SBT)
	ar rcs $(SBT).a *.o
	ranlib $@
	cp $@ lib$@

clean_sbt:
	rm -f $(SBT).a lib$(SBT).a *o

clean_hash:
	rm -f hash_table.a libhash_table.a *o

clean:
	rm -rf build tests.out *.a *.o *.cfg *.gcda *.gcno \
	*.html *.out *.info *.dSYM report *.gch ./*/*.gch