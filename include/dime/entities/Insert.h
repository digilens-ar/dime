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

#ifndef DIME_INSERT_H
#define DIME_INSERT_H

#include <dime/Basic.h>
#include <dime/entities/Entity.h>
#include <dime/util/Linear.h>

class dimeBlock;

class DIME_DLL_API dimeInsert : public dimeEntity
{
	friend class dimeEntitiesSection;
	friend class dimeBlocksSection;

public:
	dimeInsert();
	~dimeInsert() override;

	void setBlock(dimeBlock* block);
	dimeBlock* getBlock() const;

	dimeEntity* copy(dimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	bool read(dimeInput* in) override;
	bool write(dimeOutput* out) override;
	int typeId() const override;
	int countRecords() const override;

	void setInsertionPoint(const dimeVec3f& v);
	const dimeVec3f& getInsertionPoint() const;

	void setScale(const dimeVec3f& v);
	const dimeVec3f& getScale() const;

	void setRotAngle(dxfdouble angle);
	dxfdouble getRotAngle() const;

	// FIXME: more set and get methods

protected:
	void fixReferences(dimeModel* model) override;
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  dimeMemHandler* memhandler) override;
	bool traverse(const dimeState* state,
	              dimeCallback callback,
	              void* userdata) override;

private:
	void makeMatrix(dimeMatrix& m) const;

	int16 attributesFollow;
	const char* blockName;
	dimeVec3f insertionPoint;
	dimeVec3f extrusionDir;
	dimeVec3f scale;
	dxfdouble rotAngle;
	dimeEntity** entities;
	int numEntities;
#ifdef DIME_FIXBIG
  int32 rowCount;
#else
	int16 rowCount;
#endif
	int16 columnCount;
	dxfdouble rowSpacing;
	dxfdouble columnSpacing;
	dimeEntity* seqend;
	dimeBlock* block;
}; // class dimeInsert


inline void
dimeInsert::setInsertionPoint(const dimeVec3f& v)
{
	this->insertionPoint = v;
}

inline const dimeVec3f&
dimeInsert::getInsertionPoint() const
{
	return this->insertionPoint;
}

inline dimeBlock*
dimeInsert::getBlock() const
{
	return this->block;
}

inline void
dimeInsert::setScale(const dimeVec3f& v)
{
	this->scale = v;
}

inline const dimeVec3f&
dimeInsert::getScale() const
{
	return this->scale;
}

inline void
dimeInsert::setRotAngle(dxfdouble angle)
{
	this->rotAngle = angle;
}

inline dxfdouble
dimeInsert::getRotAngle() const
{
	return this->rotAngle;
}


#endif // ! DIME_INSERT_H
