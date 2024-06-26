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
  \class DimeClassesSection dime/sections/ClassesSection.h
  \brief The dimeClassesSection class handles a CLASSES \e section.
*/

#include <dime/sections/ClassesSection.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/classes/Class.h>
#include <dime/Model.h>

#include <string.h>

static constexpr char sectionName[] = "CLASSES";


/*!
  Destructor.
*/

DimeClassesSection::~DimeClassesSection()
{
	for (int i = 0; i < this->classes.count(); i++)
		delete this->classes[i];
}

//!

DimeSection*
DimeClassesSection::copy(DimeModel* const model) const
{
	auto cs = new DimeClassesSection();
	bool ok = cs != nullptr;

	int num = this->classes.count();
	if (ok && num)
	{
		cs->classes.makeEmpty(num);
		for (int i = 0; i < num; i++)
		{
			cs->classes.append(this->classes[i]->copy(model));
			if (cs->classes[i] == NULL)
			{
				ok = false;
				break;
			}
		}
	}

	if (!ok)
	{
		delete cs;
		cs = nullptr;
	}
	//  sim_trace("classes section copy: %p\n", cs);
	return cs;
}

//!

bool
DimeClassesSection::read(DimeInput* const file)
{
	int32_t groupcode;
	const char* string;
	bool ok = true;
	DimeClass* myclass = nullptr;
	this->classes.makeEmpty(64);

	//  sim_trace("Reading section: CLASSES\n");

	while (true)
	{
		if (!file->readGroupCode(groupcode) || (groupcode != 9 && groupcode != 0))
		{
			fprintf(stderr, "Error reading classes groupcode: %d.\n", groupcode);
			//      sim_warning("Error reading classes groupcode: %d.\n", groupcode);
			ok = false;
			break;
		}
		string = file->readString();
		if (!strcmp(string, "ENDSEC")) break;
		myclass = DimeClass::createClass(string);
		if (myclass == nullptr)
		{
			fprintf(stderr, "error creating class: %s.\n", string);
			//      sim_warning("error creating class: %s.\n", string);
			ok = false;
			break;
		}
		if (!myclass->read(file))
		{
			fprintf(stderr, "error reading class: %s.\n", string);
			//      sim_warning("error reading class: %s.\n", string);
			ok = false;
			break;
		}
		this->classes.append(myclass);
	}
	return ok;
}

//!

bool
DimeClassesSection::write(DimeOutput* const file)
{
	//  sim_trace("Writing section: CLASSES\n");

	file->writeGroupCode(2);
	file->writeString(sectionName);

	int i, n = this->classes.count();
	for (i = 0; i < n; i++)
	{
		if (!this->classes[i]->write(file)) break;
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
DimeClassesSection::typeId() const
{
	return DimeBase::dimeClassesSectionType;
}

//!

int
DimeClassesSection::countRecords() const
{
	int cnt = 0;
	int n = this->classes.count();
	for (int i = 0; i < n; i++)
		cnt += this->classes[i]->countRecords();
	return cnt + 2; // two additional records are written in write()
}

//!

const char*
DimeClassesSection::getSectionName() const
{
	return sectionName;
}

/*!
  Returns the number of classes in this section. 
*/

int
DimeClassesSection::getNumClasses() const
{
	return this->classes.count();
}

/*!
  Returns the class at index \a idx.
*/

DimeClass*
DimeClassesSection::getClass(const int idx)
{
	assert(idx >= 0 && idx < this->classes.count());
	return this->classes[idx];
}

/*!
  Removes (and deletes if no memory handler is used) the class at index \a idx.
*/

void
DimeClassesSection::removeClass(const int idx)
{
	assert(idx >= 0 && idx < this->classes.count());
	delete this->classes[idx];
	this->classes.removeElem(idx);
}

/*!
  Inserts a new class at index \a idx. If \a idx is negative, the
  class will be inserted at the end of the list of classes.
*/

void
DimeClassesSection::insertClass(DimeClass* const myclass, const int idx)
{
	if (idx < 0) this->classes.append(myclass);
	else
	{
		assert(idx <= this->classes.count());
		this->classes.insertElem(idx, myclass);
	}
}
