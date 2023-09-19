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
  \class DimeVertex dime/entities/Vertex.h
  \brief The dimeVertex class handles a VERTEX \e entity.
*/

#include <dime/entities/Vertex.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>

static char entityName[] = "VERTEX";

/*!
  Constructor.
*/

DimeVertex::DimeVertex()
{
	this->polyline = nullptr;
	this->flags = 0;
	this->indices[0] = 0;
	this->indices[1] = 0;
	this->indices[2] = 0;
	this->indices[3] = 0;
	this->coords.setValue(0.0f, 0.0f, 0.0f);
}

//!

DimeEntity*
DimeVertex::copy(DimeModel* const model) const
{
	auto v = new(model->getMemHandler()) DimeVertex;

	v->flags = this->flags;
	v->indices[0] = this->indices[0];
	v->indices[1] = this->indices[1];
	v->indices[2] = this->indices[2];
	v->indices[3] = this->indices[3];
	v->coords = this->coords;
	v->polyline = this->polyline;

	if (!this->copyRecords(v, model))
	{
		// check if allocated on heap.
		if (!model->getMemHandler()) delete v;
		v = nullptr;
	}
	return v;
}

//!

bool
DimeVertex::write(DimeOutput* const file)
{
	bool ret = true;
	if (!this->isDeleted())
	{
		this->preWrite(file);

		file->writeGroupCode(70);
		file->writeInt16(this->flags);
		file->writeGroupCode(10);
		file->writeDouble(this->coords[0]);
		file->writeGroupCode(20);
		file->writeDouble(this->coords[1]);
		file->writeGroupCode(30);
		file->writeDouble(this->coords[2]);

		for (int i = 0; i < this->numIndices(); i++)
		{
			file->writeGroupCode(i + 71);
#ifdef DIME_FIXBIG
      file->writeInt32(this->indices[i]);
#else
			file->writeInt16(this->indices[i]);
#endif
		}
		ret = DimeEntity::write(file);
	}
	return ret;
}

/*!
  Returns the number of indices stored in this vertex.
*/

int
DimeVertex::numIndices() const
{
	int cnt = 0;
	if ((flags & 128) && !(flags & 64))
	{
		// does vertex store indices?
		for (int i = 0; this->indices[i] && i < 4; i++, cnt++);
	}
	return cnt;
}

/*!
  Returns index number \a num.
*/

int
DimeVertex::getIndex(const int num) const
{
	return this->indices[num];
}

//!

int
DimeVertex::typeId() const
{
	return DimeBase::dimeVertexType;
}

//!

bool
DimeVertex::handleRecord(const int groupcode,
                         const dimeParam& param,
                         DimeMemHandler* const memhandler)
{
	switch (groupcode)
	{
	case 70:
		this->flags = param.int16_data;
		return true;
	case 10:
	case 20:
	case 30:
		this->coords[groupcode / 10 - 1] = param.double_data;
		return true;
	case 71:
	case 72:
	case 73:
	case 74:
#ifdef DIME_FIXBIG
    this->indices[groupcode-71] = param.int32_data;
#else
		this->indices[groupcode - 71] = param.int16_data;
#endif
		return true;
	}
	return DimeEntity::handleRecord(groupcode, param, memhandler);
}

//!

const char*
DimeVertex::getEntityName() const
{
	return entityName;
}

//!

bool
DimeVertex::getRecord(const int groupcode,
                      dimeParam& param,
                      const int index) const
{
	switch (groupcode)
	{
	case 70:
		param.int16_data = this->flags;
		return true;
	case 10:
	case 20:
	case 30:
		param.double_data = this->coords[groupcode / 10 - 1];
		return true;
	case 71:
	case 72:
	case 73:
	case 74:
#ifdef DIME_FIXBIG
    param.int16_data = this->indices[groupcode-71];
#else
		param.int32_data = this->indices[groupcode - 71];
#endif
		return true;
	}
	return DimeEntity::getRecord(groupcode, param, index);
}

//!

int
DimeVertex::countRecords() const
{
	int cnt = 0;
	if (!this->isDeleted())
	{
		cnt += 5; // header + flags + coords
		cnt += this->numIndices();
		cnt += DimeEntity::countRecords();
	}
	return cnt;
}
