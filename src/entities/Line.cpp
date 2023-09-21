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
  \class DimeLine dime/entities/Line.h
  \brief The dimeLine class handles a LINE \e entity.
*/

#include <dime/entities/Line.h>
#include <dime/records/Record.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>

static char entityName[] = "LINE";

/*!
  Constructor.
*/

DimeLine::DimeLine()
{
	coords[0].setValue(0, 0, 0);
	coords[1].setValue(0, 0, 0);
}

//!

DimeEntity*
DimeLine::copy(DimeModel* const model) const
{
	auto l = new(model->getMemHandler()) DimeLine;
	if (!l) return nullptr;

	for (int i = 0; i < 2; i++)
		l->coords[i] = this->coords[i];

	if (!this->copyRecords(l, model))
	{
		// check if allocated on heap.
		if (!model->getMemHandler()) delete l;
		l = nullptr;
	}
	else
	{
		l->copyExtrusionData(this);
	}
	return l;
}

/*!
  Writes a \e Line entity.  
*/

bool
DimeLine::write(DimeOutput* const file)
{
	this->preWrite(file);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			file->writeGroupCode((j + 1) * 10 + i);
			file->writeDouble(this->coords[i][j]);
		}
	}
	return this->writeExtrusionData(file) &&
		DimeEntity::write(file);
}

//!

DimeBase::TypeID
DimeLine::typeId() const
{
	return DimeBase::dimeLineType;
}

/*!
  Handles the callback from dimeEntity::readRecords().
*/

bool
DimeLine::handleRecord(const int groupcode,
                       const dimeParam& param,
                       DimeMemHandler* const memhandler)
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
	case 11:
	case 21:
	case 31:
		this->coords[groupcode % 10][groupcode / 10 - 1] = param.double_data;
		return true;
	}
	return DimeExtrusionEntity::handleRecord(groupcode, param, memhandler);
}

//!

const char*
DimeLine::getEntityName() const
{
	return entityName;
}

//!

bool
DimeLine::getRecord(const int groupcode,
                    dimeParam& param,
                    const int index) const
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 30:
	case 11:
	case 21:
	case 31:
		param.double_data = this->coords[groupcode % 10][groupcode / 10 - 1];
		return true;
	}
	return DimeExtrusionEntity::getRecord(groupcode, param, index);
}

DimeEntity::GeometryType
DimeLine::extractGeometry(dimeArray<dimeVec3f>& verts,
                          dimeArray<int>&/*indices*/,
                          dimeVec3f& extrusionDir,
                          dxfdouble& thickness)
{
	thickness = this->thickness;
	extrusionDir = this->extrusionDir;

	verts.append(coords[0]);
	verts.append(coords[1]);
	return DimeEntity::LINES;
}

//!

int
DimeLine::countRecords() const
{
	int cnt = 1; // header
	cnt += 6; // coordinates
	return cnt + DimeExtrusionEntity::countRecords();
}
