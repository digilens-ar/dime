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

#ifndef DIME_ELLIPSE_H
#define DIME_ELLIPSE_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>
#include <dime/util/Linear.h>

//
// please note that the thickness will always be 0.0 for this entity
//

class DIME_DLL_API DimeEllipse : public DimeExtrusionEntity
{
public:
	DimeEllipse();

	void setCenter(const dimeVec3& c);
	const dimeVec3& getCenter() const;

	void setMajorAxisEndpoint(const dimeVec3& v);
	const dimeVec3& getMajorAxisEndpoint() const;

	void setMinorMajorRatio(dxfdouble ratio);
	dxfdouble getMinorMajorRatio() const;

	void setStartParam(dxfdouble p);
	dxfdouble getStartParam() const;

	void setEndParam(dxfdouble p);
	dxfdouble getEndParam() const;

	virtual DimeEntity* copy(DimeModel* model) const;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	virtual const char* getEntityName() const;
	virtual bool write(DimeOutput* out);
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  DimeMemHandler* memhandler) override;

private:
	dimeVec3 center;
	dimeVec3 majorAxisEndpoint;
	dxfdouble ratio;
	dxfdouble startParam;
	dxfdouble endParam;
}; // class dimeEllipse

inline const dimeVec3&
DimeEllipse::getCenter() const
{
	return this->center;
}

inline void
DimeEllipse::setCenter(const dimeVec3& c)
{
	this->center = c;
}

inline void
DimeEllipse::setMajorAxisEndpoint(const dimeVec3& v)
{
	this->majorAxisEndpoint = v;
}

inline const dimeVec3&
DimeEllipse::getMajorAxisEndpoint() const
{
	return this->majorAxisEndpoint;
}

inline void
DimeEllipse::setMinorMajorRatio(const dxfdouble ratio)
{
	this->ratio = ratio;
}

inline dxfdouble
DimeEllipse::getMinorMajorRatio() const
{
	return this->ratio;
}

inline void
DimeEllipse::setStartParam(const dxfdouble p)
{
	this->startParam = p;
}

inline dxfdouble
DimeEllipse::getStartParam() const
{
	return this->startParam;
}

inline void
DimeEllipse::setEndParam(const dxfdouble p)
{
	this->endParam = p;
}

inline dxfdouble
DimeEllipse::getEndParam() const
{
	return this->endParam;
}

#endif // ! DIME_ELLIPSE_H
