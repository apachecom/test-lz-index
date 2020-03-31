//
// Created by alejandro on 20-03-19.
//

#include <iostream>
#include <fstream>
//#include <gflags/gflags.h>
#include <benchmark/benchmark.h>
#include <time.h>
#include <chrono>
#include "../lzi_lib/src/static_selfindex.h"

#include <unistd.h>

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;


std::vector<std::pair<size_t ,size_t >>R;
std::string data = "";
int code = 1;
int len  = 5;

using namespace std;


std::map<std::string,uint> collections_code = {
        {"../indices/lz77-cere"              ,1},
        {"../indices/lz77-coreutils"         ,2},
        {"../indices/lz77-einstein.de.txt"   ,3},
        {"../indices/lz77-einstein.en.txt"   ,4},
        {"../indices/lz77-Escherichia_Coli"  ,5},
        {"../indices/lz77-influenza"         ,6},
        {"../indices/lz77-kernel"            ,7},
        {"../indices/lz77-para"              ,8}

};


std::map<std::string,std::string> collections_name = {
        {"../indices/lz77-cere"            , "cere"                      },
        {"../indices/lz77-coreutils"       , "coreutils"                 },
        {"../indices/lz77-einstein.de.txt" , "einstein.de.txt"           },
        {"../indices/lz77-einstein.en.txt" , "einstein.en.txt"           },
        {"../indices/lz77-Escherichia_Coli", "Escherichia_Coli"          },
        {"../indices/lz77-influenza"       , "influenza"                 },
        {"../indices/lz77-kernel"          , "kernel"                    },
        {"../indices/lz77-para"            , "para"                      }

};

void generate_random_queries(const size_t &n, const size_t &len) {

    R.clear();

    std::string rg_file = "../files/";

    std::fstream f("../rangos/"+std::to_string(code)+"_"+std::to_string(len)+".rg", std::ios::in| std::ios::binary);
    std::cout<<"../rangos/"+std::to_string(code)+"_"+std::to_string(len)+".rg"<<std::endl;

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


auto display = [](benchmark::State &st, const std::string &coll,
        const unsigned int & rg_len, const unsigned int& num_ranges, const std::string &rg_file)
{
    unsigned int numocc, tot_numocc = 0;



    unsigned int code = collections_code[coll];
    std::vector<std::pair<uint, uint>> rg;
    //std::cout<<rg_file +std::to_string(code)+"_"+std::to_string(rg_len)+".rg"<<std::endl<<" ---\n";

    std::fstream f(rg_file +std::to_string(code)+"_"+std::to_string(rg_len)+".rg", std::ios::in| std::ios::binary);

    if(!f.is_open()){
        std::cout<<rg_file +std::to_string(code)+"_"+std::to_string(rg_len)+".rg"<<std::endl<<" ---";
        std::cout<<"Error the ranges file could not opened!!\n";
        return 0;
    }
    std::string buff; uint i = 0;
    while (!f.eof() && std::getline(f, buff), i < num_ranges) {
        if(buff.size() > 1){
            uint l,r;
            sscanf(buff.c_str(),"%u %u",&l,&r);
            rg.emplace_back(l,r);
            i++;
        }
    }
    //std::cout<<"Number of ranges "<<rg.size()<<std::endl;
    f.close();

    FILE* fd = fopen(coll.c_str(),"r");
    lz77index::static_selfindex * idx = lz77index::static_selfindex_lz77::load(fd);

	std::fstream ff("lzi_display_words_ec",std::ios::out|std::ios::binary);
    
    for (auto _ : st)
    {   int o = 0;
        for (auto &&  i : rg )
        {
		    //std::cout<<++o<<" "<<i.first<<" "<< i.second<<std::endl;
            unsigned char *extract_str = idx->display(i.first, i.second-1);
	  //  ff << extract_str;
        }
	//sleep(3);
    }

};


int main(int argc, char *argv[]) {

    //benchmark::RegisterBenchmark("LZI len 10 einstein 1000", display, coll,rg_len,num_ranges,rg_file);
    benchmark::RegisterBenchmark("LZI len 5 einstein 1000", display, "../indices/lz77-einstein.en.txt",5,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 10 einstein 1000", display, "../indices/lz77-einstein.en.txt",10,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 20 einstein 1000", display, "../indices/lz77-einstein.en.txt",20,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 30 einstein 1000", display, "../indices/lz77-einstein.en.txt",30,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 50 einstein 1000", display, "../indices/lz77-einstein.en.txt",50,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 100 einstein 1000", display, "../indices/lz77-einstein.en.txt",100,1000,"../../files/rangos/");


    benchmark::RegisterBenchmark("LZI len 5  EC 1000", display,  "../indices/lz77-Escherichia_Coli",5,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 10  EC 1000", display, "../indices/lz77-Escherichia_Coli",10,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 20  EC 1000", display, "../indices/lz77-Escherichia_Coli",20,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 30  EC 1000", display, "../indices/lz77-Escherichia_Coli",30,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 50  EC 1000", display, "../indices/lz77-Escherichia_Coli",50,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 100 EC 1000", display, "../indices/lz77-Escherichia_Coli",100,1000,"../../files/rangos/");

    benchmark::RegisterBenchmark("LZI len 5  Core 1000", display,  "../indices/lz77-coreutils",5,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 10  Core 1000", display, "../indices/lz77-coreutils",10,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 20  Core 1000", display, "../indices/lz77-coreutils",20,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 30  Core 1000", display, "../indices/lz77-coreutils",30,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 50  Core 1000", display, "../indices/lz77-coreutils",50,1000,"../../files/rangos/");
    benchmark::RegisterBenchmark("LZI len 100 Core 1000", display, "../indices/lz77-coreutils",100,1000,"../../files/rangos/");



    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;

}

