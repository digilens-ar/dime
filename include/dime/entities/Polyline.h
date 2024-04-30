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

#ifndef DIME_POLYLINE_H
#define DIME_POLYLINE_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>
#include <dime/util/Array.h>
#include <dime/util/Linear.h>


class DimeVertex;

class  DimePolyline : public DimeExtrusionEntity
{
public:
	DimePolyline();
	~DimePolyline() override;

	enum Type
	{
		POLYLINE,
		POLYFACE_MESH,
		POLYGON_MESH
	};

	enum Flags
	{
		CLOSED = 0x01,
		POLYMESH_CLOSED_M = 0x01,
		CURVE_FIT = 0x02,
		SPLINE_FIT = 0x04,
		IS_POLYLINE_3D = 0x08,
		IS_POLYMESH_3D = 0x10,
		POLYMESH_CLOSED_N = 0x20,
		IS_POLYFACE_MESH = 0x40,
		CONTINOUS_PATTERN = 0x80
	};

	enum SurfaceType
	{
		NONE = 0,
		QUADRIC_BSPLINE = 5,
		CUBIC_BSPLINE = 6,
		BEZIER = 8
	};

	int16_t getFlags() const;
	void setFlags(int16_t flags);

	int getType() const;

	const dimeVec3& getElevation() const;
	void setElevation(const dimeVec3& e);

	int16_t getPolymeshCountN() const;
	int16_t getPolymeshCountM() const;
	int16_t getSmoothSurfaceMdensity() const;
	int16_t getSmoothSurfaceNdensity() const;

	int getNumCoordVertices() const;
	int getNumIndexVertices() const;
	int getNumSplineFrameControlPoints() const;

	int16_t getSurfaceType() const;
	void setSurfaceType(int16_t type);

	DimeVertex* getCoordVertex(int index);
	DimeVertex* getIndexVertex(int index);
	DimeVertex* getSplineFrameControlPoint(int index);

	void setCoordVertices(DimeVertex** vertices, int num);
	void setIndexVertices(DimeVertex** vertices, int num);
	void setSplineFrameControlPoints(DimeVertex** vertices, int num);

	// KRF, 02-16-2006, added to enable ::copy of new polyline
	void setSeqend(const DimeEntity* ent);

	DimeEntity* copy(DimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;

	void setLayer(const dimeLayer* layer) override;
	const char* getEntityName() const override;

	bool read(DimeInput* in) override;
	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

	GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                             dimeArray<int>& indices,
	                             dimeVec3& extrusionDir,
	                             dxfdouble& thickness) override;

	void clearSurfaceData();

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;
	bool traverse(const DimeState* state,
	              dimeCallback const& callback) override;

private:
	int numCoordVertices() const;
	int numIndexVertices() const;

	int16_t flags;

#ifdef DIME_FIXBIG
  int32_t countM;
  int32_t countN;
  int32_t smoothCountM;
  int32_t smoothCountN;
#else
	int16_t countM;
	int16_t countN;
	int16_t smoothCountM;
	int16_t smoothCountN;
#endif

	int16_t surfaceType;

	int32_t coordCnt; // real # of coordinate vertices
	int32_t indexCnt; // real # of index vertices
	int32_t frameCnt;

	DimeVertex** coordVertices;
	DimeVertex** indexVertices;
	DimeVertex** frameVertices;
	DimeEntity* seqend;
	dimeVec3 elevation;
}; // class dimePolyline

inline int16_t
DimePolyline::getFlags() const
{
	return this->flags;
}

inline void
DimePolyline::setFlags(const int16_t flags)
{
	this->flags = flags;
}

inline const dimeVec3&
DimePolyline::getElevation() const
{
	return elevation;
}

inline void
DimePolyline::setElevation(const dimeVec3& e)
{
	this->elevation = e;
}

inline int16_t
DimePolyline::getPolymeshCountN() const
{
	return this->countN;
}

inline int16_t
DimePolyline::getPolymeshCountM() const
{
	return this->countM;
}

inline int16_t
DimePolyline::getSmoothSurfaceMdensity() const
{
	return this->smoothCountM;
}

inline int16_t
DimePolyline::getSmoothSurfaceNdensity() const
{
	return this->smoothCountN;
}

inline int
DimePolyline::getNumCoordVertices() const
{
	return this->coordCnt;
}

inline int
DimePolyline::getNumIndexVertices() const
{
	return this->indexCnt;
}

inline int
DimePolyline::getNumSplineFrameControlPoints() const
{
	return this->frameCnt;
}

inline DimeVertex*
DimePolyline::getCoordVertex(const int index)
{
	return this->coordVertices[index];
}

inline DimeVertex*
DimePolyline::getIndexVertex(const int index)
{
	return this->indexVertices[index];
}

inline DimeVertex*
DimePolyline::getSplineFrameControlPoint(const int index)
{
	return this->frameVertices[index];
}

inline int16_t
DimePolyline::getSurfaceType() const
{
	return this->surfaceType;
}

inline void
DimePolyline::setSurfaceType(const int16_t type)
{
	this->surfaceType = type;
}


#endif // ! DIME_POLYLINE_H
