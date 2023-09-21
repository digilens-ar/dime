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

#ifndef DIME_ARC_H
#define DIME_ARC_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>
#include <dime/util/Linear.h>

class DIME_DLL_API DimeArc : public DimeExtrusionEntity
{
public:
	DimeArc();

	void setCenter(const dimeVec3& c);
	dimeVec3 getCenter() const;
	void setRadius(dxfdouble r);
	dxfdouble getRadius() const;
	void setStartAngle(dxfdouble a);
	dxfdouble getStartAngle() const;
	void setEndAngle(dxfdouble a);
	dxfdouble getEndAngle() const;

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	virtual const char* getEntityName() const;

	virtual DimeEntity* copy(DimeModel* model) const;

	virtual bool write(DimeOutput* out);
	TypeID typeId() const override;
	int countRecords() const override;

	virtual GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                                     dimeArray<int>& indices,
	                                     dimeVec3& extrusionDir,
	                                     dxfdouble& thickness);

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  DimeMemHandler* memhandler) override;

private:
	dimeVec3 center;
	dxfdouble radius;
	dxfdouble startAngle;
	dxfdouble endAngle;
}; // class dimeArc

//
// inline methods
//

inline void
DimeArc::setCenter(const dimeVec3& c)
{
	this->center = c;
}

inline dimeVec3
DimeArc::getCenter() const
{
	return this->center;
}

inline void
DimeArc::setRadius(const dxfdouble r)
{
	this->radius = r;
}

inline dxfdouble
DimeArc::getRadius() const
{
	return this->radius;
}

inline void
DimeArc::setStartAngle(const dxfdouble a)
{
	this->startAngle = a;
}

inline dxfdouble
DimeArc::getStartAngle() const
{
	return this->startAngle;
}

inline void
DimeArc::setEndAngle(const dxfdouble a)
{
	this->endAngle = a;
}

inline dxfdouble
DimeArc::getEndAngle() const
{
	return this->endAngle;
}

#endif // ! DIME_ARC_H
