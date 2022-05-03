all: newtonPC

newtonPC:	
	gcc newtonPC.c -O3 -mavx -march=native -DLIKWID_PERFMON -I$/usr/local/include -L/usr/local/lib -L utils/utils.a -o newtonPC  -L${LIKWID_LIB} -llikwid -lm

clean:
	rm newtonPC



#   gcc -O3 -mavx -march=native -DLIKWID_PERFMON -I$/usr/local/include -L/usr/local/lib  teste.c -o teste -llikwid 
# likwid-perfctr  -C 03 -g CLOCK -m ./newtonPC < entrada.dat > saida_likwid.txt

