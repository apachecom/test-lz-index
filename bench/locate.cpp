//
// Created by alejandro on 20-03-19.
//

//
// Created by alejandro on 23-05-19.
//


#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <fstream>
#include <chrono>
#include "../lzi_lib/src/static_selfindex.h"

#define  MAX_Q 1e3
#define MAX_OCC 1e19
#define len_q 10
#define R_I 1
#define LZI 2

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

std::string data;

std::vector<std::string> R(MAX_Q);

std::map<uint,std::string> collections_code_inv_dir = {
        {1,"../indices/lz77-cere"              },
        {2,"../indices/lz77-coreutils"         },
        {3,"../indices/lz77-einstein.de.txt"   },
        {4,"../indices/lz77-einstein.en.txt"   },
        {5,"../indices/lz77-Escherichia_Coli"  },
        {6,"../indices/lz77-influenza"         },
        {7,"../indices/lz77-kernel"            },
        {8,"../indices/lz77-para"              }

};


std::map<uint,uint> collections_size = {
        {1,461286644},
        {2,205281778},
        {4,467626544},
        {5,112689515},
        {6,154808555},
        {7,257961616},
        {8,429265758}
};




void load_data(const size_t &_max,std::string& data, const std::string &coll) {

    std::fstream f(coll, std::ios::in | std::ios::binary);
    //std::string data;
    if (!f.is_open()) {
        std::cout << "Error the file could not opened!!\n";
        return;
    }
    std::string buff;
    unsigned char buffer[1000];
    while (!f.eof()) {
        f.read((char *) buffer, 1000);
        data.append((char *) buffer, f.gcount());
    }
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == 0 || data[i] == 1 || data[i] == 2)
            data[i] = 65;
    }

    f.close();
    //std::cout << "DATA LOADED " <<data.size()<<std::endl;
}


void generate_random_queries(const int & c,const size_t &n, const size_t &max_q) {
    std::srand(std::time(nullptr));
    R.clear();
    ////////////////reading patterns/////////////

    std::string pattern_file = "../files/patrones/" + std::to_string(c)+"_len_200.ptt";
    //std::cout<<pattern_file<<std::endl;
    std::fstream f(pattern_file, std::ios::in| std::ios::binary);
    if(!f.is_open()){
        std::cout<<"Error the pattern file could not opened!!\n";
    }
    std::string buff; uint i = 0;
    std::string N = "";
    for (int j = 0; j < len_q; ++j) {
        N+="N";
    }
    while (!f.eof() && std::getline(f, buff) && i < max_q) {
        std::string sss;
        sss.resize(len_q);
        std::copy(buff.begin(),buff.begin()+len_q,sss.begin());
        ///std::cout<<i<<"\t"<<sss<<std::endl;

        if(sss == N){
//            std::cout<<"patron malo malo \t"<<sss<<std::endl;
            continue;
        }
//        bool badpattern = true;
        for (int k = 0; k < sss.size(); ++k){

//            if( sss[k]!= 'N' )
//                badpattern = false;

            if(int(sss[k]) == 0 || int(sss[k]) == 1 || int(sss[k]) == 2 ){
                sss[k] = 65;
                //return 0;
            }
        }
//        if(!badpattern){
//
            R.push_back(sss);
            i++;
//        }

//        std::cout<<sss<<std::endl;
    }

//    std::cout<<R[R.size()-1]<<std::endl;
    f.close();


}


static void load_idx(benchmark::State &state) {

    int64_t code = state.range(0);
    data.clear();

    //load_data(1e19, data, collections_code_inv_dir[code]);

    for (auto _ : state) {
//        load_indice(ind,s,code);
        sleep(1);
    }

    generate_random_queries(code,data.size(), MAX_Q);


    state.counters["occ"] = 0;
    state.counters["space"] = 0;
    state.counters["bps"] = 0;
    state.counters["len_coll"] = data.size();
}

static void locate_lz77_index(benchmark::State &state) {

    int64_t code = state.range(0);
    uint numocc = 0;
    FILE* fd = fopen(collections_code_inv_dir[code].c_str(),"r");
    lz77index::static_selfindex * idx = lz77index::static_selfindex_lz77::load(fd);
    std::cout<<"index loaded: "<<collections_code_inv_dir[code]<<std::endl;
    size_t nocc;
    for (auto _ : state) {
        nocc = 0;
        for (uint ii=  0; ii < MAX_Q &&  ii < R.size();++ii) {

//            std::vector<unsigned int>* occ_pos = NULL;
//            unsigned char* pattern = new unsigned char[len_q];

//            std::copy(R[ii].begin(),R[ii].end(),(char*)pattern);
//            pattern[len_q] = '\0';
//            std::cout<<pattern<<std::endl;

            std::vector<unsigned int>* occ_pos = idx->locate((unsigned char *)R[ii].c_str(), len_q, &numocc);
//            occ_pos = idx->locate(pattern, len_q, &numocc);
//            if(occ_pos == NULL){
//                std::cout<<"BAD LOCATE"<<std::endl;
//                continue;
//            }

//            std::cout<<" | "<<occ_pos->size()<<std::endl;
            nocc += occ_pos->size();
            delete occ_pos;
//            delete pattern;

            if(nocc > MAX_OCC)
                break;
        }
    }

    state.counters["occ"] = nocc;
    state.counters["space"] = idx->size();
    state.counters["bps"] = idx->size()*(8.0/collections_size[code]);
    state.counters["len_coll"] = collections_size[code];
    delete idx;
}


BENCHMARK(load_idx)->Args({1})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({1})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({2})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({2})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({4})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({4})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({5})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({6})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({6})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({7})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({7})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)->Args({8})->Unit(benchmark::kMicrosecond);
BENCHMARK(locate_lz77_index)  ->Args({8})->Unit(benchmark::kMicrosecond);


BENCHMARK_MAIN();
