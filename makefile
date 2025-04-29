SRC = main.c
TEST_SRC = testes.c
EXEC = jogo
TEST_EXEC = testar
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g --coverage
CC = gcc
LIBS = -lcunit
.PHONY: all run_tests clean jogo

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

run_tests: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(SRC) $(TEST_SRC) $(LIBS)
	./$(TEST_EXEC)
	@gcov $(SRC) > /dev/null
	@echo "Cobertura de código:"
	@gcov -b $(SRC) | grep -A 1 "File" | grep -v "function" || true

clean:
	rm -f $(EXEC) $(TEST_EXEC) *.gcno *.gcda *.gcov

jogo: $(EXEC)
