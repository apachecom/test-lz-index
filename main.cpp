
#include <iostream>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "lzi_lib/src/static_selfindex_lz77.h"
#include "utils/mem_monitor.hpp"


using namespace lz77index;
using namespace std::chrono;

using timer = std::chrono::high_resolution_clock;
auto buildlzEnd = [](benchmark::State &st,
                    const string &file_collection,
                    const string &file_index_output

#ifdef MEM_MONITOR
        ,const string &file_mem_monitor
#endif
                    )
{

    unsigned char br=0;
    unsigned char bs=0;
    unsigned char ss=0;

    lz77index::static_selfindex* idx = nullptr;

#ifdef MEM_MONITOR
    mem_monitor mm(file_mem_monitor+"csv");
#endif
    for (auto _ : st) {

#ifdef MEM_MONITOR
        mm.event("LZEND-INDEX-BUILD");
        sleep(5);
#endif
        if(idx == nullptr)
            idx = lz77index::static_selfindex_lzend::build ((char*)file_collection.c_str(),(char*)file_index_output.c_str(), br, bs, ss);
    }



    st.counters["size"] = idx->size();

    if(idx != nullptr)
        delete idx;
};

auto buildlz77 = [](benchmark::State &st,
        const string &file_collection,
        const string &file_index_output

#ifdef MEM_MONITOR
        ,const string &file_mem_monitor

#endif
        )
{

    unsigned char br=0;
    unsigned char bs=0;
    unsigned char ss=0;

    lz77index::static_selfindex* idx = nullptr;

#ifdef MEM_MONITOR
    mem_monitor mm(file_mem_monitor+"csv");
#endif
    for (auto _ : st) {

#ifdef MEM_MONITOR
        mm.event("LZ77-INDEX-BUILD");
        std::cout<<"MEM-MONITOR"<<std::endl;
        sleep(5);
#endif
            if(idx == nullptr)
                idx = lz77index::static_selfindex_lz77::build ((char*)file_collection.c_str(),(char*)file_index_output.c_str(), br, bs, ss);
    }



    st.counters["size"] = idx->size();

    if(idx != nullptr)
        delete idx;
};



int main (int argc, char *argv[] ){

    if(argc < 1){
        std::cout<<"bad parameters....";
        return 0;
    }

    std::string collection = argv[1];//"../tests/collections/repetitive/reals/para";
    std::string path = argv[2];

#ifdef MEM_MONITOR
    std::string mem_monitor = argv[3];
#endif

    bool type = std::atoi(argv[3]);



    if(type)
            benchmark::RegisterBenchmark("LZ77-INDEX",  buildlz77, collection,path
#ifdef MEM_MONITOR
                    ,mem_monitor
#endif
                    );
    else

        benchmark::RegisterBenchmark("LZEND-INDEX",  buildlzEnd, collection,path
#ifdef MEM_MONITOR
                ,mem_monitor
#endif
        );

        benchmark::Initialize(&argc, argv);
         benchmark::RunSpecifiedBenchmarks();


    return 0;
}
