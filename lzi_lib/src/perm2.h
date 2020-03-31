/* perm.h
 * Copyright (C) 2005, Diego Arroyuelo, all rights reserved.
 *
 * Permutation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef PERMINCLUDED2
#define PERMINCLUDED2

#include "basics.h"
#include "static_bitsequence.h"
#include "static_bitsequence_builder.h"

namespace lz77index{
namespace basics{

typedef struct sperm2
{
  unsigned int *elems;                   // elements of the permutation
  unsigned int nelems;                   // # of elements
  static_bitsequence * bmap;                   // bitmap allowing rank() queries in O(1) time
  unsigned int *bwdptrs;                 // array of backward pointers
  unsigned int nbits;                    // log(nelems)
  unsigned int nbwdptrs;                 // # of backward pointers
  unsigned int t;
} *perm2;

typedef struct
{
  unsigned int key;
  unsigned int pointer;
} auxbwd2;

/** Creates a permutation
 *  
 *  @author Diego Arroyuelo
 */
perm2 createPerm2(unsigned int *elems, unsigned int nelems, unsigned int t, static_bitsequence_builder * bmb);

/** Gets the i-th element of the permutation
 *  
 *  @author Diego Arroyuelo
 */
unsigned int getelemPerm2(perm2 P, unsigned int i);

/** Destroys a permutation
 *  
 *  @author Diego Arroyuelo
 */
void destroyPerm2(perm2 P);

/** Get pi(i)^{-1}
 *  
 *  @author Diego Arroyuelo
 */
unsigned int inversePerm2(perm2 P, unsigned int i);

/** Saves a permutation
 *  
 *  @author Diego Arroyuelo
 */
unsigned int savePerm2(perm2 P, FILE *f);

/** Loads a permutation
 *  
 *  @author Diego Arroyuelo
 */
perm2 loadPerm2(FILE *f);

/** Returns the size of the data structure
 *  
 *  @author Diego Arroyuelo
 */
unsigned int sizeofPerm2(perm2 P);

}
}
#endif
