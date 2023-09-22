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

#ifndef DIME_VERTEX_H
#define DIME_VERTEX_H

#include <dime/Basic.h>
#include <dime/entities/Entity.h>
#include <dime/util/Linear.h>

class DimePolyline;

class  DimeVertex : public DimeEntity
{
	friend class DimePolyline;
	friend class DimeEntity;

public:
	DimeVertex();

	enum Flags
	{
		CURVE_FITTING_VERTEX = 0x01,
		HAS_CURVE_FIT_TANGENT = 0x02,
		SPLINE_VERTEX = 0x08,
		FRAME_CONTROL_POINT = 0x10,
		POLYLINE_3D_VERTEX = 0x20,
		POLYGON_MESH_VERTEX = 0x40,
		POLYFACE_MESH_VERTEX = 0x80
	};

	DimeEntity* copy(DimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	int16_t getFlags() const;
	void setFlags(int16_t flags);

	void setCoords(const dimeVec3& v);
	const dimeVec3& getCoords() const;

	int numIndices() const;
	int getIndex(int idx) const;
	void setIndex(int idx, int val);

	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

private:
	int16_t flags;
#ifdef DIME_FIXBIG
  int32_t indices[4];
#else
	int16_t indices[4];
#endif
	dimeVec3 coords;
	DimePolyline* polyline; // link back to polyline...
}; // class dimeVertex

inline void
DimeVertex::setCoords(const dimeVec3& v)
{
	this->coords = v;
}

inline const dimeVec3&
DimeVertex::getCoords() const
{
	return this->coords;
}

inline void
DimeVertex::setIndex(const int idx, const int val)
{
	assert(idx >= 0 && idx < 4);
	this->indices[idx] = val;
}

inline int16_t
DimeVertex::getFlags() const
{
	return this->flags;
}

inline void
DimeVertex::setFlags(const int16_t flags)
{
	this->flags = flags;
}

#endif // ! DIME_VERTEX_H
