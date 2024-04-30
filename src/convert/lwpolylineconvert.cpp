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

#include "convert_funcs.h"
#include <dime/convert/layerdata.h>
#include "linesegment.h"
#include <dime/entities/LWPolyline.h>
#include <dime/util/Linear.h>
#include <dime/State.h>

void
convert_lwpolyline(const DimeEntity* entity, const DimeState* state,
                   dxfLayerData* layerData, dxfConverter*)
{
	auto pline = (DimeLWPolyline*)entity;

	dimeMatrix matrix;
	state->getMatrix(matrix);

	dimeVec3 e = pline->getExtrusionDir();
	dxfdouble thickness = pline->getThickness();

	if (e != dimeVec3(0, 0, 1))
	{
		dimeMatrix m;
		DimeEntity::generateUCS(e, m);
		matrix.multRight(m);
	}
	e = dimeVec3(0, 0, 1) * thickness;

	dxfdouble elev = pline->getElevation();
	if (!dime_finite(elev)) elev = 0.0f;

	int n = pline->getNumVertices();
	if (n <= 0) return;

	dxfdouble constantWidth = pline->getConstantWidth();
	const dxfdouble* x = pline->getXCoords();
	const dxfdouble* y = pline->getYCoords();
	const dxfdouble* sw = pline->getStartingWidths();
	const dxfdouble* ew = pline->getEndWidths();

	auto setSegment = [x, y, elev, sw, ew, constantWidth, thickness](dxfLineSegment& s, int i0, int i1)
	{
		s.set(dimeVec3(x[i0], y[i0], elev), \
	        dimeVec3(x[i1], y[i1], elev), \
	        sw ? sw[i0] : constantWidth, \
	        ew ? ew[i0] : constantWidth, \
	        thickness);
	};

	dxfLineSegment segment, nextseg, prevseg;

	bool closed = pline->getFlags() & 1;
	int stop = closed ? n : n - 1;
	int next, next2;

	for (int i = 0; i < stop; i++)
	{
		next = (i + 1) % n;

		if (i == 0)
		{
			setSegment(segment, i, next);
			if (closed)
			{
				setSegment(prevseg, n-1, 0);
			}
		}

		next2 = (i + 2) % n;
		setSegment(nextseg, next, next2);

		segment.convert(i > 0 || closed ? &prevseg : nullptr,
		                i < (stop - 1) ? &nextseg : nullptr,
		                layerData, &matrix);

		prevseg = segment;
		segment = nextseg;
	}
#undef SET_SEGMENT
}
