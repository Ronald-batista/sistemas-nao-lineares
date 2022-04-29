all: newtonPC

newtonPC:	
	gcc newtonPC.c -o newtonPC

clean:
	rm newtonPC