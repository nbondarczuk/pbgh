all:
	cc -g3 -I.. -I../include -c rtx_tree.c
	cc -g3 -I.. -I../include -c main.c
	cc -g3 -o main main.o rtx_tree.o

clean:
	rm -f *~ *.o main
