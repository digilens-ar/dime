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
  \class DimeEntitiesSection dime/sections/EntitiesSection.h
  \brief The dimeEntitiesSection class handles an ENTITIES \e section.
*/

#include <dime/sections/EntitiesSection.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/util/Array.h>
#include <dime/entities/Entity.h>
#include <dime/Model.h>
#include <dime/entities/3DFace.h>
#include <dime/entities/Insert.h>
#include <dime/entities/Block.h>

#include <string.h>

static constexpr char sectionName[] = "ENTITIES";

/*!
  Destructor.
*/

DimeEntitiesSection::~DimeEntitiesSection()
{
	for (int i = 0; i < this->entities.count(); i++)
		delete this->entities[i];
}

//!

DimeSection*
DimeEntitiesSection::copy(DimeModel* const model) const
{
	auto es = new DimeEntitiesSection();
	bool ok = es != nullptr;

	int num = this->entities.count();
	if (ok && num)
	{
		ok = DimeEntity::copyEntityArray(this->entities.constArrayPointer(),
		                                 num,
		                                 model,
		                                 es->entities);
		if (!ok)
		{
			//      sim_trace("copy entities array failed\n");
		}
	}

	if (!ok)
	{
		delete es;
		es = nullptr;
	}
	return es;
}

//!

bool
DimeEntitiesSection::read(DimeInput* const file)
{
	int32_t groupcode;
	const char* string;
	bool ok = true;
	DimeEntity* entity = nullptr;
	this->entities.makeEmpty(1024);

	while (true)
	{
		if (!file->readGroupCode(groupcode) || groupcode != 0)
		{
			fprintf(stderr, "Error reading groupcode: %d.\n", groupcode);
			ok = false;
			break;
		}
		string = file->readString();
		if (!strcmp(string, "ENDSEC")) break;

		entity = DimeEntity::createEntity(string);
		if (entity == nullptr)
		{
			fprintf(stderr, "Error creating entity: %s.\n", string);
			ok = false;
			break;
		}
		if (!entity->read(file))
		{
			fprintf(stderr, "Error reading entity: %s.\n", string);
			ok = false;
			break;
		}
		this->entities.append(entity);
	}
	return ok;
}

//!

bool
DimeEntitiesSection::write(DimeOutput* const file)
{
	//  sim_trace("Writing section: ENTITIES\n");

	file->writeGroupCode(2);
	file->writeString(sectionName);

	int i, n = this->entities.count();
	for (i = 0; i < n; i++)
	{
		if (!this->entities[i]->write(file)) break;
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
DimeEntitiesSection::typeId() const
{
	return DimeBase::dimeEntitiesSectionType;
}

/*!
  This function should be called after loading has ended, and will
  find all forward BLOCK references.
*/

void
DimeEntitiesSection::fixReferences(DimeModel* const model)
{
	int i, n = this->entities.count();
	for (i = 0; i < n; i++)
		this->entities[i]->fixReferences(model);
}

//!

int
DimeEntitiesSection::countRecords() const
{
	int cnt = 0;
	int n = this->entities.count();
	for (int i = 0; i < n; i++)
		cnt += this->entities[i]->countRecords();
	return cnt + 2; // two records are written in write()
}

//!

const char*
DimeEntitiesSection::getSectionName() const
{
	return sectionName;
}

/*!
  Returns the number of entities in this section. Be aware that a POLYLINE
  entity with attached VERTEX entities will count as a single entity.
*/

int
DimeEntitiesSection::getNumEntities() const
{
	return this->entities.count();
}

/*!
  Returns the entity at index \a idx.
*/

DimeEntity*
DimeEntitiesSection::getEntity(const int idx)
{
	assert(idx >= 0 && idx < this->entities.count());
	return this->entities[idx];
}

/*!
  Removes (and deletes if no memory handler is used) the entity at index \a idx.
*/

void
DimeEntitiesSection::removeEntity(const int idx)
{
	assert(idx >= 0 && idx < this->entities.count());
	delete this->entities[idx];
	this->entities.removeElem(idx);
}

/*!
  Inserts a new entity at index \a idx. If \a idx is negative, the
  entity will be inserted at the end of the list of entities.

  Entities should never be allocated on the stack. Use the
  new and delete operators to create/destroy entities.
*/

void
DimeEntitiesSection::insertEntity(DimeEntity* const entity, const int idx)
{
	if (idx < 0) this->entities.append(entity);
	else
	{
		assert(idx <= this->entities.count());
		this->entities.insertElem(idx, entity);
	}
}
