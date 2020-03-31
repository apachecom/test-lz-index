#include <iostream>
#include <map>
#include <syscall.h>
#include <fstream>
#include "lzi_lib/src/static_selfindex_lz77.h"
using namespace lz77index;



std::map<std::string,uint> collections_code = {
        {"../../reals_collections/cere"              ,1},
        {"../../reals_collections/coreutils"         ,2},
        //{"../../reals_collections/einstein.de.txt"   ,3},
        {"../../reals_collections/einstein.en.txt"   ,4},
        {"../../reals_collections/Escherichia_Coli"  ,5},
        {"../../reals_collections/influenza"         ,6},
        {"../../reals_collections/kernel"            ,7},
        {"../../reals_collections/para"              ,8},
      //  {"../../reals_collections/world_leaders"     ,9},
};


std::map<std::string,std::string> collections_name = {
        //{"../test/collection/repetitve/reals/einstein.de.txt"   ,0},
        //{"einstein.de.txt"   ,0},
      {"../../reals_collections/cere"            , "cere"                      },
      {"../../reals_collections/coreutils"       , "coreutils"                 },
      //{"../../reals_collections/einstein.de.txt" , "einstein.de.txt"           },
      {"../../reals_collections/einstein.en.txt" , "einstein.en.txt"           },
      {"../../reals_collections/Escherichia_Coli", "Escherichia_Coli"          },
      {"../../reals_collections/influenza"       , "influenza"                 },
      {"../../reals_collections/kernel"          , "kernel"                    },
      {"../../reals_collections/para"            , "para"                      },
     // {"../../reals_collections/world_leaders"   , "world_leaders"             }
};






int main (int argc, char *argv[] ){

    if(argc < 1){
        std::cout<<"bad parameters....";
        return 0;
    }

    std::string collection = argv[1];//"../tests/collections/repetitive/reals/para";
    std::cout<<collection<<std::endl;
    std::string path = argv[2];
    std::cout<<"output directory:"<<path<<std::endl;

    bool type = std::atoi(argv[3]);

    unsigned char br=0;
    unsigned char bs=0;
    unsigned char ss=0;

    std::map<std::string,uint>::iterator it_map = collections_code.begin();

//    for (int i = 0; i < 7; ++i) {
//        uint8_t code = it_map->second;
//        std::string coll_name = collections_name[it_map->first];
//        std::cout<<"......Working on "+ coll_name + "............\n";
//        char * filename = (char *)it_map->first.c_str();
//        std::string tt = "../indices/lzend-"+coll_name;
        char * fileindex = (char *) path.c_str();
//        lz77index::static_selfindex* idx = lz77index::static_selfindex_lzend::build(filename,fileindex, br, bs, ss);
//        lzt77index::static_selfindex* idx  = lz77index::static_selfindex_lz77::build (filename,fileindex, br, bs, ss);
        if(type){
            lz77index::static_selfindex* idx  = lz77index::static_selfindex_lz77::build ((char*)collection.c_str(),fileindex, br, bs, ss);
//        std::cout<<"\n\n......Finished "+ coll_name + "............\n\n\n";
            delete idx;
//        ++it_map;
        }else{
            lz77index::static_selfindex* idx  = lz77index::static_selfindex_lzend::build ((char*)collection.c_str(),fileindex, br, bs, ss);
//        std::cout<<"\n\n......Finished "+ coll_name + "............\n\n\n";
            delete idx;
//        ++it_map;
        }

//    }


    return 0;
}
