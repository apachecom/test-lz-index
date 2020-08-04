//
// Created by via56 on 03-08-20.
//


#include <iostream>
#include <fstream>
#include <benchmark/benchmark.h>

#include "../lzi_lib/src/static_selfindex.h"

#define MAX_LEN_PATT 100

#define MAX_OCC 1e9
#define MAX_SAMPLES 1000

std::vector<std::string> patterns;

void load_patterns(const std::string& pattern_file,uint max_len, uint samples){
    std::cout<<pattern_file<<std::endl;
//    patterns.resize(MAX_SAMPLES);
    std::fstream f(pattern_file, std::ios::in| std::ios::binary);
    if(!f.is_open()){
        std::cout<<"Error the pattern file could not opened!!\n";
    }


    char *buff = new char[max_len]; uint i = 0;

    for (int k = 0; k < samples && !f.eof() ; ++k)
    {
        f.read(buff,max_len);
        std::string pp; pp.resize(max_len);
        std::copy(buff,buff+max_len,pp.begin());
        patterns.push_back(pp);

    }

    delete buff;

    f.close();
}

auto lzlocate = [](benchmark::State &st, const string &file_index, const uint& len, int op = 0
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

    uint queries,nocc;

    std::string ss;
    for (auto _ : st)
    {
        queries = 0;
        nocc =0;
        for (uint ii=  0; ii < MAX_SAMPLES &&  ii < patterns.size();++ii) {

            uint n_occ = 0;
            std::string query;
            query.resize(len);
            std::copy(patterns[ii].begin(),patterns[ii].begin()+len,query.begin());

            std::vector<unsigned int>* X = idx->locate((unsigned char *)query.c_str(), query.size(), &n_occ);

            nocc += X->size();
            queries++;
            delete X;
        }

    }


    st.counters["pLen"] = len;
    st.counters["queries"] = queries;
    st.counters["nOcc"] = nocc;
    st.counters["size"] = idx->size();


};


int main (int argc, char *argv[] ){

    if(argc < 2){
        std::cout<<"bad parameters...."<<std::endl;
        return 0;
    }

    std::string index_prefix  = argv[1];
    std::string pattern_file = argv[2];

    uint min_len_patten = std::atoi(argv[3]);
    uint max_len_patten = std::atoi(argv[4]);
    uint gap_len_patten = std::atoi(argv[5]);

    load_patterns(pattern_file+"-"+std::to_string(max_len_patten)+".ptt",max_len_patten,1000);
    std::cout<<"PATTERNS LOADED FROM: "<<pattern_file+"-"+std::to_string(max_len_patten)+".ptt"<<std::endl;

    for (uint i = min_len_patten; i <= max_len_patten; i+=gap_len_patten)
    {
        benchmark::RegisterBenchmark("LZ-INDEX",lzlocate,index_prefix,i)->Unit(benchmark::kMicrosecond);
        benchmark::RegisterBenchmark("LZ-END-INDEX",lzlocate,index_prefix,i,1)->Unit(benchmark::kMicrosecond);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;

}