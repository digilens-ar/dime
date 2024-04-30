/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef DIME_BASIC_H
#define DIME_BASIC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <functional>
#include <math.h>

// we prefer to use floats to save mem. Applications needing
// scientific calculations should typedef this to double
// typedef float dxfdouble;
using dxfdouble = double;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

#define DXFABS(x) ((x)<0?-(x):(x))
#define DXFMAX(x,y) ((x)>(y)?(x):(y))
#define DXFMIN(x,y) ((x)<(y)?(x):(y))
#define DXFDEG2RAD(x) (M_PI*(x)/180.0)
#define DXFRAD2DEG(x) (180.0*(x)/M_PI)


template <class T>
T DXFSQR(const T x)
{
	return x * x;
}


#define ARRAY_NEW(type, num) \
 new type[num]

#define DXF_STRCPY(d, s) \
d = new char[strlen(s)+1]; if (d) strcpy(d,s)

using dimeCallback = std::function<bool(class DimeState const*, class DimeEntity *)>; // return false to terminate traversal.

using dimeParam = union
{
	int8_t int8_data;
	int16_t int16_data;
	int32_t int32_data;
	float float_data;
	dxfdouble double_data;
	const char* string_data;
	const char* hex_data;
};

int  dime_isnan(double value);
int  dime_isinf(double value);
int  dime_finite(double value);

/* ********************************************************************** */

#endif // !DIME_BASIC_H
