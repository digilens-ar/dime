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
  \class DimeText dime/entities/Text.h
  \brief The dimeText class handles a Text \e entity.
*/

#include <dime/entities/Text.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

#define CHAR_ASP 0.82

static char entityName[] = "TEXT";

/*!
  Constructor.
*/

DimeText::DimeText()
	: origin(0.0, 0.0, 0.0), second(0.0, 0.0, 0.0), haveSecond(false), height(0.0), width(0.0), rotation(0.0),
	  wScale(0.0), hJust(0), vJust(0), text(nullptr)
{
}

//!

void DimeText::setTextString(const char* s)
{
	size_t l;
	l = strlen(s);
	auto t = new char[l + 1];
	strcpy(t, s);
	this->text = t;

	// Set new width.
	this->width = this->height * CHAR_ASP * strlen(this->text);
	if (this->wScale != 0.0)
		this->width = this->width * this->wScale;

	//??? Set new origin or second if hJust is set?
}

//!

DimeEntity*
DimeText::copy(DimeModel* const model) const
{
	auto t = new DimeText;
	if (!t) return nullptr;

	if (!this->copyRecords(t, model))
	{
		// check if allocated on heap.
		delete t;
		t = nullptr;
	}
	else
	{
		t->origin = this->origin;
		t->second = this->second;
		t->haveSecond = this->haveSecond;
		t->height = this->height;
		t->width = this->width;
		t->rotation = this->rotation;
		t->wScale = this->wScale;
		t->hJust = this->hJust;
		t->vJust = this->vJust;
		size_t l;
		l = strlen(this->text);
		auto s = new char[l + 1];
		strcpy(s, this->text);
		t->text = s;
		t->copyExtrusionData(this);
	}
	return t;
}

//!

bool
DimeText::write(DimeOutput* const file)
{
	this->preWrite(file);

	// Write a text subclass before first controlled record.
	file->writeGroupCode(100);
	file->writeString("AcDbText");

	file->writeGroupCode(1);

	file->writeString(this->text);

	file->writeGroupCode(10);
	file->writeDouble(this->origin[0]);
	file->writeGroupCode(20);
	file->writeDouble(this->origin[1]);
	file->writeGroupCode(30);
	file->writeDouble(this->origin[2]);

	file->writeGroupCode(40);
	file->writeDouble(this->height);

	if (this->wScale != 0.0)
	{
		file->writeGroupCode(41);
		file->writeDouble(this->wScale);
	}

	if (this->rotation != 0.0)
	{
		file->writeGroupCode(50);
		file->writeDouble(this->rotation);
	}

	if (this->hJust != 0)
	{
		file->writeGroupCode(72);
		file->writeInt16(static_cast<int16_t>(this->hJust));
	}

	if (haveSecond)
	{
		file->writeGroupCode(11);
		file->writeDouble(this->second[0]);
		file->writeGroupCode(21);
		file->writeDouble(this->second[1]);
		file->writeGroupCode(31);
		file->writeDouble(this->second[2]);
	}

	// For some reason a new subclass record is needed here.
	file->writeGroupCode(100);
	file->writeString("AcDbText");

	// The write order appears to be an issue???
	if (this->vJust != 0)
	{
		file->writeGroupCode(73);
		file->writeInt16(static_cast<int16_t>(this->vJust));
	}

	return this->writeExtrusionData(file) && DimeEntity::write(file);
}

//!

DimeBase::TypeID
DimeText::typeId() const
{
	return DimeBase::dimeTextType;
}

//!

bool
DimeText::handleRecord(const int groupcode,
                       const dimeParam& param)
{
	switch (groupcode)
	{
	case 1:
		this->setTextString(param.string_data);
		if (this->height != 0.0)
			this->width = this->height * CHAR_ASP * strlen(this->text);
		if (wScale != 0.0)
			this->width = this->width * wScale;
		return true;
	case 10:
	case 20:
	case 30:
		this->origin[(groupcode / 10) - 1] = param.double_data;
		return true;
	case 11:
	case 21:
	case 31:
		this->second[((groupcode - 1) / 10) - 1] = param.double_data;
		this->haveSecond = true;
		return true;
	case 40:
		this->height = param.double_data;
		if (this->text != nullptr)
			this->width = this->height * CHAR_ASP * strlen(this->text);
		if (wScale != 0.0)
			this->width = this->width * wScale;
		return true;
	case 41:
		wScale = param.double_data;
		if (this->width != 0.0)
			this->width = this->width * wScale;
		return true;
	case 50:
		this->rotation = param.double_data;
		return true;
	case 72:
		this->hJust = param.int32_data;
		return true;
	case 73:
		this->vJust = param.int32_data;
		return true;
	case 100:
		// Eat AcDbText records, leave others.
		{
			// Eat AcDbText records, leave others.
			if (strcmp(param.string_data, "AcDbText") == 0)
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
DimeText::getEntityName() const
{
	return entityName;
}

//!

bool
DimeText::getRecord(const int groupcode,
                    dimeParam& param,
                    const int index) const
{
	switch (groupcode)
	{
	case 1:
		param.string_data = this->text;
		return true;
	case 10:
	case 20:
	case 30:
		param.double_data = this->origin[groupcode / 10 - 1];
		return true;
	case 11:
	case 21:
	case 31:
		param.double_data = this->second[(groupcode - 1) / 10 - 1];
		return true;
	case 40:
		param.double_data = this->height;
		return true;
	case 41:
		if (this->wScale == 0) return false;
		param.double_data = this->wScale;
		return true;
	case 50:
		param.double_data = this->rotation;
		return true;
	case 72:
		param.int32_data = this->hJust;
		return true;
	case 73:
		param.int32_data = this->vJust;
		return true;
	}
	return DimeExtrusionEntity::getRecord(groupcode, param, index);
}

DimeEntity::GeometryType
DimeText::extractGeometry(dimeArray<dimeVec3>& verts,
                          dimeArray<int>& indices,
                          dimeVec3& extrusionDir,
                          dxfdouble& thickness)
{
	thickness = this->thickness;
	extrusionDir = this->extrusionDir;

	// find points at corners of box around text.
	verts.append(origin);
	verts.append(dimeVec3(this->origin.x + this->width, this->origin.y, 0.0));
	verts.append(dimeVec3(this->origin.x + this->width, this->origin.y + this->height, 0.0));
	verts.append(dimeVec3(this->origin.x, this->origin.y + this->height, 0.0));

	// close loop with first point.
	verts.append(origin);

	if (this->thickness == 0.0) return DimeEntity::LINES;
	return DimeEntity::POLYGONS;
}

//!

int
DimeText::countRecords() const
{
	int cnt = 1 + 3 + 3 + 1 + 1 + 1 + 1 + 1 + 1 + 1;
	// header + origin + second + haveSecond + height + rotation + wScale + hJust + vJust + text

	return cnt + DimeExtrusionEntity::countRecords();
}
