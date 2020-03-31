cd cmake-build-release

cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=OFF ..
make test-lz-index

#build lz77
./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lz77-Escherichia_Coli 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-Escherichia_Coli.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lz77-cere 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-cere.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lz77-coreutils 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-coreutils.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lz77-einstein.en.txt 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-einstein.en.txt.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lz77-influenza 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-influenza.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lz77-kernel 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-kernel.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lz77-para 1 --benchmark_out=/d1/apacheco/results/lz77/lz77-build-para.csv       --benchmark_out_format=csv



cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=ON ..
make test-lz-index

#build lz77
./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lz77-Escherichia_Coli /d1/apacheco/results/lz77/lz77-build-Escherichia_Coli  1
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lz77-cere /d1/apacheco/results/lz77/lz77-build-cere  1
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lz77-coreutils /d1/apacheco/results/lz77/lz77-build-coreutils  1
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lz77-einstein.en.txt /d1/apacheco/results/lz77/lz77-build-einstein.en.txt  1
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lz77-influenza /d1/apacheco/results/lz77/lz77-build-influenza  1
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lz77-ker/d1/apacheco/results/lz77/lz77-build-paranel /d1/apacheco/results/lz77/lz77-build-kernel  1
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lz77-para   1


cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=OFF ..
make test-lz-index
