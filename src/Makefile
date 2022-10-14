CC = g++ -g -std=c++17
LDFLAGS = -Wall -Wextra -Werror -std=c++17 -g

OS=$(shell uname -s)

BD= build/

SRC = $(FILES_FOR_TEST)\
	main.cc\
	./view/console_interface.cc\
	./controller/controller.cc

FILES = $(SBT).a\
	hash_table.a\
	./view/console_interface.cc\
	./controller/controller.cc\
	./main.cc

FILES_HASH_TABLE = ./hash_table/hash_table.cc

FILES_SBT =	./self_balancing_binary_search_tree/self_balancing_binary_search_tree.cc

FILES_FOR_TEST = ./test/test.cc\
	$(FILES_SBT)\
	$(FILES_HASH_TABLE)

SBT = self_balancing_binary_search_tree

all: build
	$(BD)transactions.out

build: clean $(SBT).a hash_table.a
	mkdir -p $(BD)
	$(CC) $(LDFLAGS) $(FILES) -o $(BD)transactions.out -L. -l$(SBT) -lhash_table

test_build: 
ifeq ($(OS), Darwin)
	$(CC) -o tests.out $(FILES_FOR_TEST) -lm -lgtest -lpthread
else
	$(CC) -o tests.out $(FILES_FOR_TEST) -lm -lgtest -lpthread
endif

test: test_build
	./tests.out

gcov_report: clean 
	$(CC) --coverage -o testresult.out $(FILES_FOR_TEST) -lgtest -lpthread
	./testresult.out
	lcov -t "testresult" -o testresult.info -c -d . --no-external
# lcov --remove testresult.info \
# 	'*/containers/*'\
# 	'*/matrix/*'\
# 	'*/bonus_research.*'\
# 	-o testresult_filtered.info
	genhtml -o report testresult.info
	open ./report/index.html

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

memcheck: test_build
	valgrind -s --tool=memcheck --track-origins=yes --leak-check=full --log-file=leaks.txt  ./tests.out
	cat leaks.txt | grep -E 'lost|leaks'

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(SRC)

leaks:
	leaks -atExit -- ./tests | grep LEAK

cppcheck:
	cppcheck --enable=all $(suppress) $(SRC)

clean_sbt:
	rm -f $(SBT).a lib$(SBT).a *o

clean_hash:
	rm -f hash_table.a libhash_table.a *o

clean:
	rm -rf build tests.out *.a *.o *.cfg *.gcda *.gcno \
	*.html *.out *.info *.dSYM report *.gch ./*/*.gch