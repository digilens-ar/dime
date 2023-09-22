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

#ifndef DIME_CIRCLE_H
#define DIME_CIRCLE_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>
#include <dime/util/Linear.h>

class DimeModel;


class DIME_DLL_API DimeCircle : public DimeExtrusionEntity
{
public:
	DimeCircle();

	const dimeVec3& getCenter() const;
	void setCenter(const dimeVec3& c);

	void setRadius(dxfdouble val);
	dxfdouble getRadius() const;

	virtual DimeEntity* copy(DimeModel* model) const;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	virtual const char* getEntityName() const;
	virtual bool write(DimeOutput* out);
	TypeID typeId() const override;
	int countRecords() const override;

	virtual GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                                     dimeArray<int>& indices,
	                                     dimeVec3& extrusionDir,
	                                     dxfdouble& thickness);

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

private:
	dimeVec3 center;
	dxfdouble radius;
}; // class dimeCircle

inline const dimeVec3&
DimeCircle::getCenter() const
{
	return this->center;
}

inline void
DimeCircle::setCenter(const dimeVec3& c)
{
	this->center = c;
}

inline void
DimeCircle::setRadius(const dxfdouble val)
{
	this->radius = val;
}

inline dxfdouble
DimeCircle::getRadius() const
{
	return this->radius;
}

#endif // ! DIME_CIRCLE_H
