CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = tree_test
OBJS = bst.o avl.o splay.o test.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
