
mkdir indices


cd cmake-build-release

make test-lz-index

#build lz77
./test-lz-index /d1/apacheco/collections/Escherichia_Coli  /d1/apacheco/indices/lz77/lz77-Escherichia_Coli 1

./test-lz-index /d1/apacheco/collections/cere  /d1/apacheco/indices/lz77-cere 1

./test-lz-index /d1/apacheco/collections/coreutils  /d1/apacheco/indices/lz77-coreutils 1

./test-lz-index /d1/apacheco/collections/einstein.en.txt  /d1/apacheco/indices/lz77-einstein.en.txt 1

./test-lz-index /d1/apacheco/collections/influenza  /d1/apacheco/indices/lz77-influenza 1

./test-lz-index /d1/apacheco/collections/kernel  /d1/apacheco/indices/lz77-kernel 1

./test-lz-index /d1/apacheco/collections/para  /d1/apacheco/indices/lz77-para 1