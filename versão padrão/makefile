all: newtonPC

newtonPC:	
	gcc newtonPC.c -O3 -mavx -march=native -DLIKWID_PERFMON -I$/usr/local/include -L/usr/local/lib -Lutils/utils.a -o newtonPC  -L${LIKWID_LIB} -llikwid -lm

clean:
	rm newtonPC


	