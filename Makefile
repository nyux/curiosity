sorting:
	clang -o sorting sorting.c

prefix:
	clang -o prefix prefix.c `pkg-config --libs --cflags libedit`
