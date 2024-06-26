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
#include <dime/convert/convert.h>
#include <dime/convert/layerdata.h>
#include <dime/entities/Trace.h>
#include <dime/util/Linear.h>
#include <dime/State.h>


//
// implementation in solid.cpp
//
extern void
convert_solid_data(dimeVec3* v, dimeVec3& e, dxfdouble thickness,
                   const DimeState* state,
                   dxfLayerData* layerData);


void
convert_trace(const DimeEntity* entity, const DimeState* state,
              dxfLayerData* layerData, dxfConverter* converter)
{
	auto trace = (DimeTrace*)entity;

	// respect the value in the $FILLMODE header variable
	layerData->setFillmode(converter->getFillmode());

	dimeVec3 v[4];
	trace->getVertices(v[0], v[1], v[2], v[3]);

	dimeParam param;
	if (trace->getRecord(38, param))
	{
		v[0][2] = param.double_data;
		v[1][2] = param.double_data;
		v[2][2] = param.double_data;
		v[3][2] = param.double_data;
	}

	dimeVec3 e;
	trace->getExtrusionDir(e);
	dxfdouble thickness = trace->getThickness();

	convert_solid_data(v, e, thickness, state, layerData);
}
