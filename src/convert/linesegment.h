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

#ifndef _DXF2VRML_LINE_SEGMENT_
#define _DXF2VRML_LINE_SEGMENT_

#include <dime/util/Linear.h>
class dxfLayerData;

class dxfLineSegment
{
public:
	void set(const dimeVec3& p0, const dimeVec3& p1,
	         dxfdouble startWidth, dxfdouble endwidth,
	         dxfdouble thickness);
	void convert(dxfLineSegment* prev, dxfLineSegment* next,
	             dxfLayerData* data, dimeMatrix* matrix);

private:
	void calculate_v();
	void calculate_connect(dxfLineSegment* next);

	dimeVec3 p[2];
	dxfdouble w[2];
	dxfdouble thickness;
	dxfLineSegment* prev;
	dimeVec3 e;
	dimeVec3 dir;
	dimeVec3 wdir;

	// calculated pts
	int flags;
	dimeVec3 v[4];
	dimeVec3 connect[4];
};


#endif // _DXF2VRML_LINE_SEGMENT_
