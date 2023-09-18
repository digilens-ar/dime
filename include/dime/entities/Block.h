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

#ifndef DIME_BLOCK_H
#define DIME_BLOCK_H

#include <dime/Basic.h>
#include <dime/entities/Entity.h>
#include <dime/util/Linear.h>

class dimeInput;
class dimeMemHandler;
class dimeModel;
class dimeOutput;

class DIME_DLL_API dimeBlock : public dimeEntity
{
	friend class dimeBlocksSection;
	friend class dimeEntitiesSection;
	friend class dimeInsert;
	friend class dimeModel;

public:
	dimeBlock(dimeMemHandler* memhandler);
	~dimeBlock() override;

	const dimeVec3f& getBasePoint() const;
	void setBasePoint(const dimeVec3f& v);
	int getNumEntities() const;
	dimeEntity* getEntity(int idx);
	void insertEntity(dimeEntity* entity, int idx = -1);
	void removeEntity(int idx, bool deleteIt = true);
	void fitEntities();

	const char* getName() const;
	void setName(const char* name);

	dimeEntity* copy(dimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	bool read(dimeInput* in) override;
	bool write(dimeOutput* out) override;
	virtual int typeId() const;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  dimeMemHandler* memhandler) override;

	void fixReferences(dimeModel* model) override;
	bool traverse(const dimeState* state,
	              dimeCallback callback,
	              void* userdata) override;

private:
	int16 flags;
	const char* name;
	dimeVec3f basePoint;
	dimeArray<dimeEntity*> entities;
	dimeEntity* endblock;
	dimeMemHandler* memHandler;
}; // class dimeBlock

inline const dimeVec3f&
dimeBlock::getBasePoint() const
{
	return this->basePoint;
}

inline void
dimeBlock::setBasePoint(const dimeVec3f& v)
{
	this->basePoint = v;
}

inline int
dimeBlock::getNumEntities() const
{
	return this->entities.count();
}

inline dimeEntity*
dimeBlock::getEntity(const int idx)
{
	assert(idx >= 0 && idx < this->entities.count());
	return this->entities[idx];
}

inline const char*
dimeBlock::getName() const
{
	return this->name;
}

inline void
dimeBlock::setName(const char* const name)
{
	this->name = name;
}

#endif // ! DIME_BLOCK_H
