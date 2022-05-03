#/bin/bash

make clean
make

likwid-perfctr  -C 03 -g L3 -m ./newtonPC < entrada.dat > saida_likwid_L3.txt
likwid-perfctr  -C 03 -g CACHE -m ./newtonPC < entrada.dat > saida_likwid_L2CACHE.txt
likwid-perfctr  -C 03 -g FLOPS_DP -m ./newtonPC < entrada.dat > saida_likwid_FLOPS_DP.txt



# Filtrando os L3
i=72;
j=69;
for N in 4096 3000 2048 2000 1024 1000 600 512 400 300 256 200 128 100 64 50 32 10 
do
    echo $N
    #cat saida_likwid_L3.txt | grep ":_$N" > group_L3_$N.txt
    cat saida_likwid_L3.txt | grep bandwidth |head -n $i| tail -n $(($i-$j+1)) >> group_L3_$N.txt #cut -d"|" -f3
    #cat saida_likwid.txt | grep "_$N:" | cut -d"|" -f4 >> group_L3_$N.txt
    echo ' ' >> group_L3_$N.txt
    let "i-=4";
    let "j-=4";
done


# Filtrando pelo timestamp()
i=72; 
j=69;
for N in 4096 3000 2048 2000 1024 1000 600 512 400 300 256 200 128 100 64 50 32 10 
do
    echo $N
    #cat saida_likwid.txt | grep ":_$N" > group_TIME_$N.txt
    #cat saida_likwid.txt | grep bandwidth |head -n $i| tail -n $(($i-$j+1))>> group_L3_$N.txt #cut -d"|" -f3
    cat saida_likwid_L3.txt | grep "_$N:" | cut -d"|" -f4 > group_TIME_$N.txt
    echo ' ' >> group_TIME_$N.txt
    let "i-=4";
    let "j-=4";
done


# Filtrando os L2CACHE por data cache miss ratio
i=72; 
j=69;
for N in 4096 3000 2048 2000 1024 1000 600 512 400 300 256 200 128 100 64 50 32 10  
do
    echo $N
    #cat saida_likwid.txt | grep ":_$N" > group_L2CACHE_$N.txt
    cat saida_likwid_L2CACHE.txt | grep  "data cache miss ratio" |head -n $i| tail -n $(($i-$j+1))>> group_L2CACHE_$N.txt #cut -d"|" -f3
    #cat saida_likwid.txt | grep "_$N:" | cut -d"|" -f4 >> group_L3_$N.txt
    echo ' ' >> group_L2CACHE_$N.txt
    let "i-=4";
    let "j-=4";
done


# Filtrando FLOPS_DP por FLOPS_DP e FLOPS_AVS(CPI)
i=72; 
j=69;
for N in 4096 3000 2048 2000 1024 1000 600 512 400 300 256 200 128 100 64 50 32 10  
do
    echo $N
    #cat saida_likwid.txt | grep ":_$N" > group_L2CACHE_$N.txt
    #cat saida_likwid_FLOPS_DP.txt | grep CPI | head -n $i| tail -n $(($i-$j+1)) > group_FLOPS_DP_$N.txt #cut -d"|" -f3
    cat saida_likwid_FLOPS_DP.txt | grep -i 'MFLOP/s' | head -n $i| tail -n $(($i-$j+1)) >> group_FLOPS_DP_$N.txt
    #cat saida_likwid.txt | grep "_$N:" | cut -d"|" -f4 >> group_L3_$N.txt
    echo ' ' >> group_FLOPS_DP_$N.txt
    let "i-=4";
    let "j-=4";
done


# ordem dos fatores apresentados
cat saida_likwid_L3.txt | grep :_ | head -n4 > ordem_resultados_obtidos.txt
mv ordem_resultados_obtidos.txt saida

for N in 4096 3000 2048 2000 1024 1000 600 512 400 300 256 200 128 100 64 50 32 10 
do
    mv *$N.txt saida/D$N/
done