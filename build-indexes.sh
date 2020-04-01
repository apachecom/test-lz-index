cd cmake-build-release

cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=OFF  -DUSE_LZEND=OFF..
make test-lz-index

#build lz77
./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lz77-Escherichia_Coli  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-Escherichia_Coli.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lz77-cere  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-cere.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lz77-coreutils  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-coreutils.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lz77-einstein.en.txt  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-einstein.en.txt.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lz77-influenza  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-influenza.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lz77-kernel  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-kernel.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lz77-para  --benchmark_out=/d1/apacheco/results/lz77/lz77-build-para.csv       --benchmark_out_format=csv



cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=ON -DUSE_LZEND=OFF ..
make test-lz-index

#build lz77
./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lz77-Escherichia_Coli /d1/apacheco/results/lz77/lz77-build-Escherichia_Coli-mem
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lz77-cere /d1/apacheco/results/lz77/lz77-build-cere-mem
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lz77-coreutils /d1/apacheco/results/lz77/lz77-build-coreutils-mem
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lz77-einstein.en.txt /d1/apacheco/results/lz77/lz77-build-einstein.en.txt-mem
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lz77-influenza /d1/apacheco/results/lz77/lz77-build-influenza-mem
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lz77-ker/d1/apacheco/results/lz77/lz77-build-paranel /d1/apacheco/results/lz77/lz77-build-kernel-mem
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lz77-para-mem


cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=OFF -DUSE_LZEND=ON ..
make test-lz-index

#build lzEND

./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lzEnd-Escherichia_Coli  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-Escherichia_Coli.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lzEnd-cere  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-cere.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lzEnd-coreutils  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-coreutils.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lzEnd-einstein.en.txt  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-einstein.en.txt.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lzEnd-influenza  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-influenza.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lzEnd-kernel  --benchmark_out=/d1/apacheco/results/lz77/lz77-lzEnd-kernel.csv       --benchmark_out_format=csv
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lzEnd-para  --benchmark_out=/d1/apacheco/results/lz77/lzEnd-build-para.csv       --benchmark_out_format=csv


cmake -DCMAKE_BUILD_TYPE=Release -DUSE_MEM_MONITOR=ON -DUSE_LZEND=ON..
make test-lz-index

#build lzEND

./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lzEnd-Escherichia_Coli-mem
./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lzEnd-cere-mem
./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lzEnd-coreutils-mem
./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lzEnd-einstein.en.txt-mem
./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lzEnd-influenza-mem
./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lzEnd-kernel-mem
./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lzEnd-para-mem
