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

#ifndef _DXF2VRML_CONVERT_FUNCS_H_
#define _DXF2VRML_CONVERT_FUNCS_H_

class DimeEntity;
class DimeState;
class dxfLayerData;
class dxfConverter;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

void convert_3dface(const DimeEntity*, const DimeState*,
                    dxfLayerData*, dxfConverter*);
void convert_line(const DimeEntity*, const DimeState*,
                  dxfLayerData*, dxfConverter*);
void convert_point(const DimeEntity*, const DimeState*,
                   dxfLayerData*, dxfConverter*);
void convert_circle(const DimeEntity*, const DimeState*,
                    dxfLayerData*, dxfConverter*);
void convert_ellipse(const DimeEntity*, const DimeState*,
                     dxfLayerData*, dxfConverter*);
void convert_arc(const DimeEntity*, const DimeState*,
                 dxfLayerData*, dxfConverter*);
void convert_solid(const DimeEntity*, const DimeState*,
                   dxfLayerData*, dxfConverter*);
void convert_trace(const DimeEntity*, const DimeState*,
                   dxfLayerData*, dxfConverter*);
void convert_polyline(const DimeEntity*, const DimeState*,
                      dxfLayerData*, dxfConverter*);
void convert_lwpolyline(const DimeEntity*, const DimeState*,
                        dxfLayerData*, dxfConverter*);

#endif // _DXF2VRML_CONVERT_FUNCS_H_
