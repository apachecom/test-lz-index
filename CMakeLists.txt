cmake_minimum_required(VERSION 3.12)
project(test-lz-index)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake)

# Guard against in-source builds and bad build-type strings
#
include(ConfigSafeGuards)



#Testing
include(CTest)
#set(LIBS string_dictionaries)

find_package(Threads REQUIRED)
find_package(GTest)
if (GTEST_FOUND)
    set(LIBS ${LIBS} GTest::Main)
else ()

    if (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-Wall -ansi -Wno-deprecated -pthread)
    endif ()

    if (MSVC)
        #vc 2012 fix for vararg templates
        set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
    endif ()

    include(ConfigGTest)
    set(LIBS ${LIBS} gtest gtest_main)
endif ()




#Benchmark
# If you want to self-test benchmark lib too, turn me ON
#
set(BENCHMARK_ENABLE_TESTING OFF)

# Configure google micro benchmark
#

# c++11 is required
#
if ((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    #    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    #    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

include(ConfigGBench)


include_directories(~/include)



set(SOURCE_FILES

        lzi_lib/src/basics.h
        lzi_lib/src/bitrankw32int.h
        lzi_lib/src/bp_hb.h
        lzi_lib/src/deltacodes.h
        lzi_lib/src/dfuds.h
        lzi_lib/src/directcodes.h
        #lzi_lib/src/divsufsort.h
        lzi_lib/src/extractor.h
        lzi_lib/src/hash.h
        lzi_lib/src/lfs.h
        lzi_lib/src/LZ77.h
        lzi_lib/src/LZend.h
        lzi_lib/src/LZparser.h
        lzi_lib/src/mapper.h
        lzi_lib/src/offsets.h
        lzi_lib/src/patricia.h
        lzi_lib/src/patricia_node.h
        lzi_lib/src/perm2.h
        lzi_lib/src/perm.h
        lzi_lib/src/range.h
        lzi_lib/src/RMQ.h
        lzi_lib/src/RMQ_succinct.h
        lzi_lib/src/static_bitsequence_brw32.h
        lzi_lib/src/static_bitsequence_builder_brw32.h
        lzi_lib/src/static_bitsequence_builder.h
        lzi_lib/src/static_bitsequence.h
        lzi_lib/src/static_doublebitmap.h
        lzi_lib/src/static_doublebitmap_s.h
        lzi_lib/src/static_permutation.h
        lzi_lib/src/static_permutation_mrrr2.h
        lzi_lib/src/static_permutation_mrrr.h
        lzi_lib/src/static_range_chazelle.h
        lzi_lib/src/static_range.h
        lzi_lib/src/static_range_none.h
        lzi_lib/src/static_selfindex.h
        lzi_lib/src/static_selfindex_lz77.h
        lzi_lib/src/static_selfindex_lzend.h
        lzi_lib/src/static_selfindex_none.h
        lzi_lib/src/static_sequence_wt.h
        lzi_lib/src/treelist.h
        lzi_lib/src/utils.h
        lzi_lib/src/utils_index.h
        lzi_lib/src/wt_node.h


        lzi_lib/src/bitrankw32int.cpp
        lzi_lib/src/bp_hb.cpp
        lzi_lib/src/deltacodes.cpp
        lzi_lib/src/dfuds.cpp
        lzi_lib/src/directcodes.cpp
        lzi_lib/src/extractor.cpp
        lzi_lib/src/hash.cpp
        lzi_lib/src/LZ77.cpp
        lzi_lib/src/LZend.cpp
        lzi_lib/src/LZparser.cpp
        lzi_lib/src/mapper.cpp
        #lzi_lib/src/maxh.cpp
        lzi_lib/src/patricia.cpp
        lzi_lib/src/patricia_node.cpp
        lzi_lib/src/perm2.cpp
        lzi_lib/src/perm.cpp
        lzi_lib/src/range.cpp
        lzi_lib/src/RMQ.cpp
        lzi_lib/src/RMQ_succinct.cpp
        lzi_lib/src/static_bitsequence_brw32.cpp
        lzi_lib/src/static_bitsequence_builder_brw32.cpp
        lzi_lib/src/static_bitsequence.cpp
        lzi_lib/src/static_doublebitmap.cpp
        lzi_lib/src/static_doublebitmap_s.cpp
        lzi_lib/src/static_permutation.cpp
        lzi_lib/src/static_permutation_mrrr2.cpp
        lzi_lib/src/static_permutation_mrrr.cpp
        lzi_lib/src/static_range_chazelle.cpp
        lzi_lib/src/static_range.cpp
        lzi_lib/src/static_range_none.cpp
        lzi_lib/src/static_selfindex.cpp
        lzi_lib/src/static_selfindex_lz77.cpp
        lzi_lib/src/static_selfindex_lzend.cpp
        lzi_lib/src/static_selfindex_none.cpp
        lzi_lib/src/static_sequence_wt.cpp
        lzi_lib/src/treelist.cpp
        lzi_lib/src/utils.cpp
        lzi_lib/src/utils_index.cpp
        lzi_lib/src/wt_node.cpp

        )


link_directories(~/lib)





add_executable(test-lz-index main.cpp ${SOURCE_FILES})
#add_executable(lzi_locate locate.cpp ${SOURCE_FILES})
#add_executable(lzi_display display.cpp ${SOURCE_FILES} bench/locate_pattern_len.cpp bench/display_pattern_len.cpp)


include(internal_utils)

cxx_executable_with_flags(display "" "${LIBS};benchmark;sdsl;cds;divsufsort;divsufsort64;" bench/display.cpp ${SOURCE_FILES})
cxx_executable_with_flags(locate "" "${LIBS};benchmark;sdsl;cds;divsufsort;divsufsort64;" bench/locate.cpp ${SOURCE_FILES})
cxx_executable_with_flags(locate_pattern_len "" "${LIBS};benchmark;sdsl;cds;divsufsort;divsufsort64;" bench/locate_pattern_len.cpp ${SOURCE_FILES})
cxx_executable_with_flags(display_pattern_len "" "${LIBS};benchmark;sdsl;cds;divsufsort;divsufsort64;" bench/display_pattern_len.cpp ${SOURCE_FILES})


target_link_libraries(test-lz-index cds)
target_link_libraries(test-lz-index divsufsort)
#target_link_libraries(lzi_locate cds)
#target_link_libraries(lzi_locate divsufsort)


#target_link_libraries(lzi_display cds)
#target_link_libraries(lzi_display divsufsort)

#target_link_libraries(lzi_rlcsa divsufsort64)

