all: newtonPC

newtonPC:	
	gcc newtonPC.c -I/usr/local/include -L/usr/local/lib -lmatheval -o newtonPC

clean:
	rm newtonPC