//
// Created by via56 on 03-08-20.
//

//
// Created by via56 on 03-04-20.
//

#include <iostream>
#include <fstream>
#include <benchmark/benchmark.h>

#include "../lzi_lib/src/static_selfindex.h"

#define MAX_LEN_PATT 100

#define MAX_OCC 1e9
#define MAX_SAMPLES 1000

std::vector<size_t> pos;

void load_ptos(const std::string& pos_file){
    std::cout<<pos_file<<std::endl;
    pos.resize(MAX_SAMPLES);
    std::fstream f(pos_file, std::ios::in| std::ios::binary);
    if(!f.is_open()){
        std::cout<<"Error the pos file could not opened!!\n";
    }
    std::string buff; uint i = 0;
    while (i < MAX_SAMPLES && !f.eof() && std::getline(f, buff)) {

        pos[i] = atoi(buff.c_str());
//        std::cout<<i<<" "<<pos[i]<<std::endl;
        ++i;
    }
    f.close();
}



auto lzdisplay = [](benchmark::State &st, const std::string &file_index, const uint& len, int op = 0
){
    lz77index::static_selfindex* idx = nullptr;
    if(op > 0){
        std::string file = file_index + "-lzEnd";
        FILE* fd = fopen(file.c_str(),"r");
        idx = lz77index::static_selfindex_lzend::load(fd);
    }
    else{
        std::string file = file_index + "-lz77";
        FILE* fd = fopen(file.c_str(),"r");
        idx = lz77index::static_selfindex_lz77::load(fd);

    }

    uint queries;
    for (auto _ : st)
    {
        queries = 0;
        for (uint ii=  0; ii < MAX_SAMPLES &&  ii < pos.size();++ii) {
            unsigned char * ss = idx->display(pos[ii],pos[ii]+len);
            queries++;
            delete ss;
        }
    }

    st.counters["queries"]   = queries;
    st.counters["pLen"]      = len;
    st.counters["size"]      = idx->size();

    delete idx;
};

int main (int argc, char *argv[] ){

    if(argc < 2){
        std::cout<<"bad parameters...."<<std::endl;
        return 0;
    }

    std::string index_prefix  = argv[1];

    uint min_len_patten = std::atoi(argv[3]);
    uint max_len_patten = std::atoi(argv[4]);
    uint gap_len_patten = std::atoi(argv[5]);

    std::string ptos_file = argv[2];

    load_ptos(ptos_file+"-"+std::to_string(max_len_patten)+".pos");
    std::cout<<"POS LOADED FROM: "<<ptos_file+"-"+std::to_string(max_len_patten)+".pos"<<std::endl;
    std::cout<<"POS LEN: "<<pos.size()<<std::endl;
    std::cout<<"POS EX: "<<pos[0]<<std::endl;

    for (uint i = min_len_patten; i <= max_len_patten; i+=gap_len_patten)
    {
        benchmark::RegisterBenchmark("LZ-INDEX",lzdisplay,index_prefix,i)->Unit({benchmark::kMicrosecond});;
        benchmark::RegisterBenchmark("LZ-END-INDEX",lzdisplay,index_prefix,i,1)->Unit({benchmark::kMicrosecond});;
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;

}
