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

#ifndef _DXF2VRML_LAYERDATA_H_
#define _DXF2VRML_LAYERDATA_H_

#include <dime/util/Linear.h>
#include <dime/util/Array.h>
#include <dime/util/BSPTree.h>
#include <stdio.h>

class  dxfLayerData
{
public:
	dxfLayerData(int colidx);
	~dxfLayerData();

	void setFillmode(bool fillmode);

	void addLine(const dimeVec3& v0, const dimeVec3& v1,
	             const dimeMatrix* matrix = nullptr);

	void addPoint(const dimeVec3& v,
	              const dimeMatrix* matrix = nullptr);

	void addTriangle(const dimeVec3& v0,
	                 const dimeVec3& v1,
	                 const dimeVec3& v2,
	                 const dimeMatrix* matrix = nullptr);
	void addQuad(const dimeVec3& v0,
	             const dimeVec3& v1,
	             const dimeVec3& v2,
	             const dimeVec3& v3,
	             const dimeMatrix* matrix = nullptr);

	void writeWrl(FILE* fp, int indent, bool vrml1,
	              bool only2d);

	//private:
public: // 20011001 thammer - please don't kill me for this ;-)

	friend class dime2So;
	friend class dime2Profit;

	bool fillmode;
	int colidx;
	dimeBSPTree facebsp;
	dimeArray<int> faceindices;
	dimeBSPTree linebsp;
	dimeArray<int> lineindices;
	dimeArray<dimeVec3> points;
};

#endif // _DXF2VRML_LAYERDATA_H_
