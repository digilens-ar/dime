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

class DimeInput;
class DimeModel;
class DimeOutput;

class  DimeBlock : public DimeEntity
{
	friend class DimeBlocksSection;
	friend class DimeEntitiesSection;
	friend class DimeInsert;
	friend class DimeModel;

public:
	DimeBlock();
	~DimeBlock() override;

	const dimeVec3& getBasePoint() const;
	void setBasePoint(const dimeVec3& v);
	int getNumEntities() const;
	DimeEntity* getEntity(int idx);
	void insertEntity(DimeEntity* entity, int idx = -1);
	void removeEntity(int idx, bool deleteIt = true);
	void fitEntities();

	const char* getName() const;
	void setName(const char* name);

	DimeEntity* copy(DimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	bool read(DimeInput* in) override;
	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

	void fixReferences(DimeModel* model) override;
	bool traverse(const DimeState* state,
	              dimeCallback const& callback) override;

private:
	int16_t flags;
	const char* name;
	dimeVec3 basePoint;
	dimeArray<DimeEntity*> entities;
	DimeEntity* endblock;
}; // class dimeBlock

class DimeEndBlock : public DimeEntity
{
public:
	DimeEndBlock() = default;

	TypeID typeId() const override {return DimeBase::dimeEndBlockType; }
	const char* getEntityName() const override { return "ENDBLK"; }
	DimeEntity* copy(DimeModel* model) const override { return new DimeEndBlock; }
};

inline const dimeVec3&
DimeBlock::getBasePoint() const
{
	return this->basePoint;
}

inline void
DimeBlock::setBasePoint(const dimeVec3& v)
{
	this->basePoint = v;
}

inline int
DimeBlock::getNumEntities() const
{
	return this->entities.count();
}

inline DimeEntity*
DimeBlock::getEntity(const int idx)
{
	assert(idx >= 0 && idx < this->entities.count());
	return this->entities[idx];
}

inline const char*
DimeBlock::getName() const
{
	return this->name;
}

inline void
DimeBlock::setName(const char* const name)
{
	this->name = name;
}

#endif // ! DIME_BLOCK_H
