// Copyright (C) 2011 Dave Griffiths
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef FLX_TYPES
#define FLX_TYPES

#include <math.h>

// autoconf can tell us if we are on Pi
// only support Pi and linux for the moment
#ifdef HAVE_LIBBCM_HOST
#define FLX_RPI
#endif

#ifndef FLX_RPI
#define FLX_LINUX
#endif

#ifndef _EE
typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long int u64;
typedef long long int s64;
typedef float f32;
#endif



typedef double rk_real;

/* These parameters affect accuracy (hence speed)*/
#define N 25 /* Number of host and parasite phenotypes */

#endif // FLX_TYPES
