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

#ifndef DIME_EXTRUSIONENTITY_H
#define DIME_EXTRUSIONENTITY_H

#include <dime/entities/Entity.h>

class DIME_DLL_API DimeExtrusionEntity : public DimeEntity
{
public:
	DimeExtrusionEntity();

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;

	void setExtrusionDir(const dimeVec3& v);
	const dimeVec3& getExtrusionDir() const;

	void setThickness(dxfdouble val);
	dxfdouble getThickness() const;

	TypeID typeId() const override;
	bool isOfType(int thtypeid) const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  DimeMemHandler* memhandler) override;

	void copyExtrusionData(const DimeExtrusionEntity* entity);
	bool writeExtrusionData(DimeOutput* out);

protected: // should be private :-(
	dimeVec3 extrusionDir;
	dxfdouble thickness;
}; // class dimeExtrusionEntity

inline void
DimeExtrusionEntity::setExtrusionDir(const dimeVec3& v)
{
	this->extrusionDir = v;
}

inline const dimeVec3&
DimeExtrusionEntity::getExtrusionDir() const
{
	return this->extrusionDir;
}

inline void
DimeExtrusionEntity::setThickness(const dxfdouble val)
{
	this->thickness = val;
}

inline dxfdouble
DimeExtrusionEntity::getThickness() const
{
	return this->thickness;
}

#endif // ! DIME_EXTRUSIONENTITY_H
