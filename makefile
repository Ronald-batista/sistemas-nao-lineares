all: newtonPC

newtonPC:	
	gcc newtonPC.c -O3 -mavx -march=native -o newtonPC  -L${LIKWID_LIB} -llikwid

clean:
	rm newtonPC



#   gcc -DLIKWID_PERFMON -I$/usr/local/include -L/usr/local/lib  teste.c -o teste -llikwid 
# likwid-perfctr  -C 03 -g CLOCK -m ./teste