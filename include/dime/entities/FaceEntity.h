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

#ifndef DIME_FACEENTITY_H
#define DIME_FACEENTITY_H

#include <dime/Basic.h>
#include <dime/entities/Entity.h>
#include <dime/util/Linear.h>

class DIME_DLL_API dimeFaceEntity : public DimeEntity
{
public:
	bool isQuad() const;

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;

	void setVertex(int idx, const dimeVec3& v);
	void setTriangle(const dimeVec3& v0, const dimeVec3& v1,
	                 const dimeVec3& v2);
	void setQuad(const dimeVec3& v0, const dimeVec3& v1,
	             const dimeVec3& v2, const dimeVec3& v3);
	const dimeVec3& getVertex(int idx) const;
	void getVertices(dimeVec3& v0, dimeVec3& v1,
	                 dimeVec3& v2, dimeVec3& v3) const;

	virtual dxfdouble getThickness() const;
	virtual void getExtrusionDir(dimeVec3& ed) const;

	GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                             dimeArray<int>& indices,
	                             dimeVec3& extrusionDir,
	                             dxfdouble& thickness) override;

	TypeID typeId() const override;
	bool isOfType(int thetypeid) const override;
	int countRecords() const override;

protected:
	virtual bool swapQuadCoords() const;

	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  DimeMemHandler* memhandler) override;
	void copyCoords(const dimeFaceEntity* entity);
	bool writeCoords(DimeOutput* file);

	dimeFaceEntity();
	dimeVec3 coords[4];
}; // class dimeFaceEntity

inline const dimeVec3&
dimeFaceEntity::getVertex(const int idx) const
{
	assert(idx >= 0 && idx < 4);
	return this->coords[idx];
}

inline bool
dimeFaceEntity::isQuad() const
{
	return (coords[2] != coords[3]);
}

inline void
dimeFaceEntity::setVertex(const int idx, const dimeVec3& v)
{
	assert(idx >= 0 && idx < 4);
	this->coords[idx] = v;
}

#endif // ! DIME_FACEENTITY_H
