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
  \class DimeArc dime/entities/Arc.h
  \brief The dimeArc class handles an ARC \e entity.
*/

#include <dime/entities/Arc.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static char entityName[] = "ARC";

// FIXME: should be configurable

#define ARC_NUMPTS 20 // num pts for a 2PI arc

/*!
  Constructor.
*/

DimeArc::DimeArc()
	: center(0, 0, 0), radius(0.0), startAngle(0.0), endAngle(2 * M_PI)
{
}

//!

DimeEntity*
DimeArc::copy(DimeModel* const model) const
{
	auto a = new DimeArc;
	if (!a) return nullptr;

	if (!this->copyRecords(a, model))
	{
		// check if allocated on heap.
		delete a;
		a = nullptr;
	}
	else
	{
		a->center = this->center;
		a->radius = this->radius;
		a->startAngle = this->startAngle;
		a->endAngle = this->endAngle;
		a->copyExtrusionData(this);
	}
	return a;
}

//!

bool
DimeArc::write(DimeOutput* const file)
{
	this->preWrite(file);

	file->writeGroupCode(10);
	file->writeDouble(this->center[0]);
	file->writeGroupCode(20);
	file->writeDouble(this->center[1]);
	file->writeGroupCode(30);
	file->writeDouble(this->center[2]);

	file->writeGroupCode(40);
	file->writeDouble(this->radius);

	// For some reason a 73 record needs a new subclass record.
	file->writeGroupCode(100);
	file->writeString("AcDbArc");

	file->writeGroupCode(50);
	file->writeDouble(this->startAngle);
	file->writeGroupCode(51);
	file->writeDouble(this->endAngle);

	return this->writeExtrusionData(file) && DimeEntity::write(file);
}

//!

DimeBase::TypeID
DimeArc::typeId() const
{
	return DimeBase::dimeArcType;
}

//!

bool
DimeArc::handleRecord(const int groupcode,
                      const dimeParam& param)
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
		this->center[groupcode / 10 - 1] = param.double_data;
		return true;
	case 40:
		this->radius = param.double_data;
		return true;
	case 50:
		this->startAngle = param.double_data;
		return true;
	case 51:
		this->endAngle = param.double_data;
		return true;
	case 100:
		// Eat AcDbArc records, leave others.
		{
			// Eat AcDbArc records, leave others.
			if (strcmp(param.string_data, "AcDbArc") == 0)
			{
				return true;
			}
			return DimeExtrusionEntity::handleRecord(groupcode, param);
		}
	}
	return DimeExtrusionEntity::handleRecord(groupcode, param);
}

//!

const char*
DimeArc::getEntityName() const
{
	return entityName;
}

//!

bool
DimeArc::getRecord(const int groupcode,
                   dimeParam& param,
                   const int index) const
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
		param.double_data = this->center[groupcode / 10 - 1];
		return true;
	case 40:
		param.double_data = this->radius;
		return true;
	case 50:
		param.double_data = this->startAngle;
		return true;
	case 51:
		param.double_data = this->endAngle;
		return true;
	}
	return DimeExtrusionEntity::getRecord(groupcode, param, index);
}


DimeEntity::GeometryType
DimeArc::extractGeometry(dimeArray<dimeVec3>& verts,
                         dimeArray<int>& indices,
                         dimeVec3& extrusionDir,
                         dxfdouble& thickness)
{
	verts.setCount(0);
	indices.setCount(0);

	thickness = this->thickness;
	extrusionDir = this->extrusionDir;

	// split the arc into lines

	double end = this->endAngle;
	if (end < this->startAngle) end += 360.0;

	double delta = DXFDEG2RAD(end - this->startAngle);
	if (delta == 0.0)
	{
#ifndef NDEBUG
		fprintf(stderr, "ARC with startAngle == endAngle!\n");
#endif
		end += 2 * M_PI;
		//return dimeEntity::NONE;
	}

	// find the number of this ARC that fits inside 2PI
	int parts = static_cast<int>(DXFABS((2*M_PI)/delta));

	// find # pts to use for arc
	// add one to avoid arcs with 0 line segments
	int numpts = ARC_NUMPTS / parts + 1;
	if (numpts > ARC_NUMPTS) numpts = ARC_NUMPTS;

	double inc = delta / numpts;
	double rad = DXFDEG2RAD(this->startAngle);
	int i;
	for (i = 0; i < numpts; i++)
	{
		verts.append(dimeVec3(this->center.x + this->radius * cos(rad),
		                       this->center.y + this->radius * sin(rad),
		                       this->center.z));
		rad += inc;
	}
	rad = DXFDEG2RAD(end);

	verts.append(dimeVec3(this->center.x + this->radius * cos(rad),
	                       this->center.y + this->radius * sin(rad),
	                       this->center.z));

	return DimeEntity::LINES;
}

//!

int
DimeArc::countRecords() const
{
	int cnt = 1 + 3 + 1 + 2; // header + center point + radius + angles

	return cnt + DimeExtrusionEntity::countRecords();
}
