#include <fstream> 
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "lzi_lib/src/utils.h"
#include "lzi_lib/src/static_selfindex.h"
#include "lzi_lib/src/offsets.h"

#include <iostream>
#include <vector>
#include <map>
#include <time.h>


#include <chrono>

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;


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





/* Open patterns file and read header */
void pfile_info(unsigned int* length, unsigned int* numpatt){
	int error;
	unsigned char c;
	unsigned char origfilename[257];

	error = fscanf(stdin, "# number=%u length=%u file=%s forbidden=", numpatt, length, origfilename);
	if (error != 3)	{
		fprintf (stderr, "Error: Patterns file header not correct\n");
		perror ("run_queries");
		exit (1);
	}
	//fprintf (stderr, "# patterns = %lu length = %lu file = %s forbidden chars = ", *numpatt, *length, origfilename);
	while ( (c = fgetc(stdin)) != 0) {
		if (c == '\n') break;
		//fprintf (stderr, "%d",c);
	}
	//fprintf(stderr, "\n");
}

void pfile_info(unsigned int* numpatt){
	int error;
	unsigned char c;

	error = fscanf(stdin, "#numberOfFrases::%u", numpatt);

	if (error != 1)	{
		fprintf (stderr, "Error: Patterns file header not correct\n");
		perror ("run_queries");
		exit (1);
	}
	//fprintf (stderr, "# patterns = %lu length = %lu file = %s forbidden chars = ", *numpatt, *length, origfilename);
	while ( (c = fgetc(stdin)) != 0) {
		if (c == '\n') break;
		//fprintf (stderr, "%d",c);
	}
	//fprintf(stderr, "\n");
}

int main(int argc, char** argv){


	unsigned int numocc, tot_numocc = 0;
	double tot_time = 0, load_time = 0, aggregated_time = 0;
    	std::vector<unsigned int>* occ_pos;

	unsigned int RUNS = 10;

	unsigned int l_patt = atoi(argv[2]);
    unsigned int n_patt = atoi(argv[3]);

    std::string coll = argv[1];
    std::string dir_patt = argv[4];
    unsigned int code_coll = collections_code[coll];
    std::vector<string> patt;

    std::cout<<"Patterns file\n"<<std::endl;
    std::cout<<dir_patt + "/" + std::to_string(code_coll)+"_len_200.ptt"<<std::endl;

    std::fstream f(dir_patt + "/" + std::to_string(code_coll)+"_len_200.ptt",std::ios::in| std::ios::binary);

    if(!f.is_open()){
        std::cout<<"Error the pattern file could not opened!!\n";
    }
    std::string buff; uint i = 0;

    while (!f.eof() && std::getline(f, buff) && i < n_patt) {
        std::string sss;
        sss.resize(l_patt);

	
        std::copy(buff.begin(),buff.begin()+l_patt,sss.begin());
	if(sss == "NNNNNNNNNN") continue;
        patt.push_back(sss);
        i++;
    }
    f.close();
    std::cout<<"Number of patterns "<<patt.size()<<std::endl;
	std::cout<<"len of patterns "<<l_patt<<std::endl;

    /*Load Index*/
    	lz77index::utils::startTime();
    FILE* fd = fopen(argv[1],"r");
	lz77index::static_selfindex* idx = lz77index::static_selfindex_lz77::load(fd);
	load_time += lz77index::utils::endTime();
    std::cout<<"Load index time "<<load_time<<std::endl;

		// PRINTF Index Size



    double overall_mean = 0;
    double overall_time = 0;

    std::cout<<"index size "<<idx->size()<<std::endl;

		for (unsigned int i=0; i < RUNS; i++)
		{
		        double mean = 0;
			

			if(l_patt <= 5)
				n_patt = 10;
			for (unsigned int j=0; j<n_patt; j++)
			{
				/* Locate */
				//lz77index::utils::startTime();
				//std::cout<<j<<std::endl;
                		auto start = timer::now();
				//unsigned char* pattern = (unsigned char * )patt[j].c_str();
                		unsigned char* pattern = new unsigned char[l_patt];
				strcpy((char*)pattern, (char*)patt[j].c_str());
//				std::cout<<pattern<<std::endl;
				occ_pos = idx->locate(pattern, l_patt, &numocc);
				delete occ_pos;
				
				//tot_time += lz77index::utils::endTime();
		                auto stop = timer::now();
                		mean += (duration_cast<microseconds>(stop - start).count());
				tot_numocc += numocc;
				numocc = 0;
			}

			std::cout<<"tiempo total: "<<mean<<std::endl;
			overall_time += mean/(RUNS*1.0);
			mean = mean/(tot_numocc*1.0);

			std::cout<<"tiempo medio de occ: "<<mean<<"  occ"<<tot_numocc<<std::endl;
			
			overall_mean += mean * 1.0/10.0;
			// PRINTF Occurrences
			/*
				if (i==0) fprintf(stderr, "%u;;", tot_numocc);
				// PRINTF Run locate time
				fprintf(stderr, "%.2f;", tot_time);
				fflush(stderr);
            		*/
			aggregated_time += tot_time;
			tot_time = 0;
			tot_numocc = 0;
			//sleep(5);
		}

    std::cout<<"overall time: "<<overall_time<<std::endl;
    fprintf(stderr, "overall mean  %.10f \n", overall_mean);
	// PRINTF Averaged locate time
	//fprintf(stderr, ";%.9f \n", aggregated_time/RUNS);
	//fflush(stderr);

        delete idx;

	/*fprintf(stderr, "Total Num occs found = %u\n", tot_numocc);
	fprintf(stderr, "Locate time = %.9f secs\n", tot_time);
	fprintf(stderr, "time/occ:%.9f\n\n", (tot_time * 1000) / tot_numocc);
	fprintf(stderr, "(Load_time+Locate_time)/Num_occs = %.9f msecs/occs\n\n", ((tot_time+load_time) * 1000) / tot_numocc);
*/


    	return EXIT_SUCCESS;
}
