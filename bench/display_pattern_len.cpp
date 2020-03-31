//
// Created by alejandro on 03-07-19.
//

//
// Created by alejandro on 27-06-19.
//

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
#include <unistd.h>


#define  MAX_Q 1e3
#define MAX_OCC 1e7
#define R_I 1
#define LZI 2

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

std::vector<std::pair<size_t ,size_t >>R;
std::string data = "";
int code = 1;
int len_q  = 5;

std::map<uint,std::string> index_code_inv_dir = {
        {1,"../indices/lz77-cere"              },
        {2,"../indices/lz77-coreutils"         },
        {3,"../indices/lz77-einstein.de.txt"   },
        {4,"../indices/lz77-einstein.en.txt"   },
        {5,"../indices/lz77-Escherichia_Coli"  },
        {6,"../indices/lz77-influenza"         },
        {7,"../indices/lz77-kernel"            },
        {8,"../indices/lz77-para"              }

};

std::map<uint,std::string> collection_code_inv_dir = {
        {1,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/cere"              },
        {2,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/coreutils"         },
        {3,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/einstein.de.txt"   },
        {4,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/einstein.en.txt"   },
        {5,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/Escherichia_Coli"  },
        {6,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/influenza"         },
        {7,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/kernel"            },
        {8,"/home/alejandro/Documents/phd/examen_de_calificacion/paper/code/tests/collections/repetitive/reals/para"              }

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

    data = "";
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
    std::cout << "DATA LOADED " <<data.size()<<std::endl;
}


void generate_random_queries(const size_t &n, const size_t &len) {

    R.clear();

    std::string rg_file = "../files/";

    std::fstream f(rg_file + "rangos/"+std::to_string(code)+"_"+std::to_string(len)+".rg", std::ios::in| std::ios::binary);
    std::cout<<rg_file + "rangos/"+std::to_string(code)+"_"+std::to_string(len)+".rg"<<std::endl;

    if(!f.is_open()){
        std::cout<<"Error the ranges file could not opened!!\n";
        return;
    }
    std::string buff; uint i = 0;

    while (!f.eof() && std::getline(f, buff) ) {
        if(buff.size() > 1){
            uint l,r;
            sscanf(buff.c_str(),"%u %u",&l,&r);
            R.emplace_back(l,r);
            i++;
        }
    }
    f.close();
    std::cout<<"R.size(): "<<R.size()<<std::endl;
    if(R.size() == 0) std::cout<<"Error leyendo el fichero\n";
}
static void load_idx(benchmark::State &state) {

    code = state.range(0);
    len_q = state.range(1);


    data.clear();

    load_data(1e19, data, collection_code_inv_dir[code]);

    for (auto _ : state) {
//        load_indice(ind,s,code);
        sleep(3);
    }

    generate_random_queries(data.size(),len_q);

    state.counters["1 n_ptt"]     = 0;
    state.counters["2 n_occ"]     = 0;
    state.counters["3 ptt_len"]   = 0;
    state.counters["4 coll_size"] = 0;
    state.counters["5 coll_id"]   = 0;
    state.counters["6 size"]      = 0;
    state.counters["7 samp"]      = 0;
    state.counters["8 name"]      = 0;
    state.counters["9 valid"]     = 0;


}

static void display_lz77_index(benchmark::State &state) {

    int64_t code = state.range(0);
    int64_t len = state.range(1);
    uint numocc = 0;
    FILE* fd = fopen(index_code_inv_dir[code].c_str(),"r");
    lz77index::static_selfindex * idx = lz77index::static_selfindex_lz77::load(fd);
//    std::cout<<"index loaded: "<<index_code_inv_dir[code]<<std::endl;
    size_t nocc,ptt;


    for (auto _ : state) {

        nocc = 0, ptt = 0;

        for (uint ii =  0;  ii < R.size();++ii) {

            numocc = 0;
            std::string ss = "";
            ss.resize(len);
            auto i = R[ii];
            std::copy(data.begin()+i.first,data.begin()+i.second,ss.begin());
            unsigned char *extract_str = idx->display(i.first, i.second-1);

            int r = strcmp((char*)extract_str,ss.c_str());
            delete extract_str;
            ++ptt;
            ASSERT_EQ(r,0);

        }
    }



    state.counters["1 n_ptt"]     = ptt;
    state.counters["2 n_occ"]     = 0;
    state.counters["3 ptt_len"]   = len;
    state.counters["4 coll_size"] = collections_size[code];
    state.counters["5 coll_id"]   = code;
    state.counters["6 size"]      = idx->size();
    state.counters["7 samp"]      = 0;
    state.counters["8 name"]      = 4;
    state.counters["9 valid"]     = 1;

    //state.counters["occ"] = nocc;
    //state.counters["space"] = idx->size();
    //state.counters["bps"] = idx->size()*(8.0/collections_size[code]);
    //state.counters["len_coll"] = collections_size[code];
    delete idx;
//    delete fd;
}


BENCHMARK(load_idx)            ->Args({1,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({1,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({1,200})->Unit(benchmark::kMicrosecond);



BENCHMARK(load_idx)            ->Args({2,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({2,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({2,200})->Unit(benchmark::kMicrosecond);




BENCHMARK(load_idx)            ->Args({4,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({4,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({4,200})->Unit(benchmark::kMicrosecond);



BENCHMARK(load_idx)           ->Args({5,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)           ->Args({5,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({5,200})->Unit(benchmark::kMicrosecond);


BENCHMARK(load_idx)            ->Args({6,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({6,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({6,200})->Unit(benchmark::kMicrosecond);



BENCHMARK(load_idx)            ->Args({7,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({7,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({7,200})->Unit(benchmark::kMicrosecond);




BENCHMARK(load_idx)            ->Args({8,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,5})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,10})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,20})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,30})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,40})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,50})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,100})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,150})->Unit(benchmark::kMicrosecond);
BENCHMARK(load_idx)            ->Args({8,200})->Unit(benchmark::kMicrosecond);
BENCHMARK(display_lz77_index)  ->Args({8,200})->Unit(benchmark::kMicrosecond);



BENCHMARK_MAIN();
