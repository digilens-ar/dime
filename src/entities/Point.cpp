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

/*!
  \class DimePoint dime/entities/Point.h
  \brief The dimePoint class handles a POINT \e entity.
*/

#include <dime/entities/Point.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>

static char entityName[] = "POINT";

/*!
  Constructor.
*/

DimePoint::DimePoint()
	: coords(0, 0, 0)
{
}

//!

DimeEntity*
DimePoint::copy(DimeModel* const model) const
{
	auto p = new(model->getMemHandler()) DimePoint;

	p->coords = this->coords;
	p->copyExtrusionData(this);

	if (!this->copyRecords(p, model))
	{
		// check if allocated on heap.
		if (!model->getMemHandler()) delete p;
		p = nullptr;
	}
	return p;
}

//!

bool
DimePoint::write(DimeOutput* const file)
{
	bool ret = true;
	if (!this->isDeleted())
	{
		this->preWrite(file);

		file->writeGroupCode(10);
		file->writeDouble(this->coords[0]);
		file->writeGroupCode(20);
		file->writeDouble(this->coords[1]);
		file->writeGroupCode(30);
		file->writeDouble(this->coords[2]);

		ret = this->writeExtrusionData(file) && DimeEntity::write(file);
	}
	return ret;
}

//!

DimeBase::TypeID
DimePoint::typeId() const
{
	return DimeBase::dimePointType;
}

//!

bool
DimePoint::handleRecord(const int groupcode,
                        const dimeParam& param,
                        DimeMemHandler* const memhandler)
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
		this->coords[groupcode / 10 - 1] = param.double_data;
		return true;
	}
	return DimeExtrusionEntity::handleRecord(groupcode, param, memhandler);
}

//!

const char*
DimePoint::getEntityName() const
{
	return entityName;
}

//!

bool
DimePoint::getRecord(const int groupcode,
                     dimeParam& param,
                     const int index) const
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
		param.double_data = this->coords[groupcode / 10 - 1];
		return true;
	}
	return DimeExtrusionEntity::getRecord(groupcode, param, index);
}

//!

DimeEntity::GeometryType
DimePoint::extractGeometry(dimeArray<dimeVec3f>& verts,
                           dimeArray<int>&/*indices*/,
                           dimeVec3f& extrusionDir,
                           dxfdouble& thickness)
{
	thickness = this->thickness;
	extrusionDir = this->extrusionDir;
	verts.append(this->coords);
	return DimeEntity::POINTS;
}

//!

int
DimePoint::countRecords() const
{
	int cnt = 0;
	cnt += 4; // header + coordinates
	return cnt + DimeExtrusionEntity::countRecords();
}
