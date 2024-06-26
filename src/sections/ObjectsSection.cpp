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
  \class DimeObjectsSection dime/sections/ObjectsSection.h
  \brief The dimeObjectsSection object handles an OBJECTS \e section.
*/

#include <dime/sections/ObjectsSection.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/objects/Object.h>
#include <dime/Model.h>
#include <string.h>

static constexpr char sectionName[] = "OBJECTS";

/*!
  Destructor.
*/

DimeObjectsSection::~DimeObjectsSection()
{
	for (int i = 0; i < this->objects.count(); i++)
		delete this->objects[i];
}

//!

DimeSection*
DimeObjectsSection::copy(DimeModel* const model) const
{
	auto os = new DimeObjectsSection;
	bool ok = os != nullptr;

	int num = this->objects.count();
	if (ok && num)
	{
		os->objects.makeEmpty(num);
		for (int i = 0; i < num; i++)
		{
			os->objects.append(this->objects[i]->copy(model));
			if (os->objects[i] == NULL)
			{
				ok = false;
				break;
			}
		}
	}

	if (!ok)
	{
		delete os;
		os = nullptr;
	}
	//  sim_trace("objects section copy: %p\n", os);
	return os;
}

//!

bool
DimeObjectsSection::read(DimeInput* const file)
{
	int32_t groupcode;
	const char* string;
	bool ok = true;
	DimeObject* object = nullptr;
	this->objects.makeEmpty(64);

	//  sim_trace("Reading section: OBJECTS\n");

	while (true)
	{
		if (!file->readGroupCode(groupcode) || groupcode != 0)
		{
			fprintf(stderr, "Error reading objects groupcode: %d.\n", groupcode);
			//      sim_warning("Error reading objects groupcode: %d.\n", groupcode);
			ok = false;
			break;
		}
		string = file->readString();
		if (!strcmp(string, "ENDSEC")) break;
		object = DimeObject::createObject(string);
		if (object == nullptr)
		{
			fprintf(stderr, "error creating object: %s.\n", string);
			//      sim_warning("error creating object: %s.\n", string);
			ok = false;
			break;
		}
		if (!object->read(file))
		{
			fprintf(stderr, "error reading object: %s.\n", string);
			//      sim_warning("error reading object: %s.\n", string);
			ok = false;
			break;
		}
		this->objects.append(object);
	}
	return ok;
}

//!

bool
DimeObjectsSection::write(DimeOutput* const file)
{
	//  sim_trace("Writing section: OBJECTS\n");

	file->writeGroupCode(2);
	file->writeString(sectionName);

	int i, n = this->objects.count();
	for (i = 0; i < n; i++)
	{
		if (!this->objects[i]->write(file)) break;
	}
	if (i == n)
	{
		file->writeGroupCode(0);
		file->writeString("ENDSEC");
		return true;
	}
	return false;
}

//!

DimeBase::TypeID
DimeObjectsSection::typeId() const
{
	return DimeBase::dimeObjectsSectionType;
}

//!

int
DimeObjectsSection::countRecords() const
{
	int cnt = 0;
	int n = this->objects.count();
	for (int i = 0; i < n; i++)
		cnt += this->objects[i]->countRecords();
	return cnt + 2; // two additional records are written in write()
}

//!

const char*
DimeObjectsSection::getSectionName() const
{
	return sectionName;
}

/*!
  Returns the number of objects in this section. 
*/

int
DimeObjectsSection::getNumObjects() const
{
	return this->objects.count();
}

/*!
  Returns the object at index \a idx.
*/

DimeObject*
DimeObjectsSection::getObject(const int idx)
{
	assert(idx >= 0 && idx < this->objects.count());
	return this->objects[idx];
}

/*!
  Removes (and deletes if no memory handler is used) the object at index \a idx.
*/

void
DimeObjectsSection::removeObject(const int idx)
{
	assert(idx >= 0 && idx < this->objects.count());
	delete this->objects[idx];
	this->objects.removeElem(idx);
}

/*!
  Inserts a new object at index \a idx. If \a idx is negative, the
  object will be inserted at the end of the list of objects.
*/

void
DimeObjectsSection::insertObject(DimeObject* const object, const int idx)
{
	if (idx < 0) this->objects.append(object);
	else
	{
		assert(idx <= this->objects.count());
		this->objects.insertElem(idx, object);
	}
}
