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
  \class DimeInsert dime/entities/Insert.h
  \brief The dimeInsert class handles an INSERT \e entity.
*/

#include <dime/entities/Insert.h>
#include <dime/entities/Block.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/State.h>

static char entityName[] = "INSERT";

/*!
  Constructor.
*/

DimeInsert::DimeInsert()
{
	this->attributesFollow = 0;
	this->blockName = nullptr;
	this->insertionPoint.setValue(0, 0, 0);
	this->extrusionDir.setValue(0, 0, 1);
	this->scale.setValue(1, 1, 1);
	this->rotAngle = 0.0;
	this->entities = NULL;
	this->numEntities = 0;
	this->seqend = NULL;
	this->block = nullptr;
	this->rowCount = 1;
	this->columnCount = 1;
	this->rowSpacing = 0.0;
	this->columnSpacing = 0.0;
}

/*!
  Destructor.
*/

DimeInsert::~DimeInsert()
{
	delete this->seqend;
	for (int i = 0; i < this->numEntities; i++)
		delete this->entities[i];
	delete [] this->entities;
}

//!

DimeEntity*
DimeInsert::copy(DimeModel* const model) const
{
	auto inst = new DimeInsert;

	bool ok = true;
	if (this->numEntities)
	{
		int realnum = this->numEntities;
		inst->entities = DimeEntity::copyEntityArray(this->entities,
		                                             realnum,
		                                             model);
		if (realnum > 0 && inst->entities == nullptr)
		{
			ok = false;
		}
		else inst->numEntities = realnum;
	}

	inst->attributesFollow = this->attributesFollow;
	inst->insertionPoint = this->insertionPoint;
	inst->scale = this->scale;
	inst->rotAngle = this->rotAngle;
	inst->rowCount = this->rowCount;
	inst->columnCount = this->columnCount;
	inst->rowSpacing = this->rowSpacing;
	inst->columnSpacing = this->columnSpacing;
	inst->extrusionDir = this->extrusionDir;

	if (ok && this->seqend)
	{
		inst->seqend = this->seqend->copy(model);
		if (!inst->seqend) ok = false;
	}

	if (ok && this->blockName)
	{
		inst->blockName = model->findRefStringPtr(this->blockName);
		if (inst->blockName)
		{
			inst->block = static_cast<DimeBlock*>(model->findReference(inst->blockName));
		}
		else
		{
			// probably a forward reference, fixed during fixReferences()
			inst->blockName = (char*)model->addReference(this->blockName, nullptr);
			inst->block = nullptr;
		}
	}

	if (!ok || !this->copyRecords(inst, model))
	{
		delete inst; // delete if allocated on heap
		inst = nullptr; // just return NULL
	}
	return inst;
}

/*!
  Reads an INSERT entity.
*/

bool
DimeInsert::read(DimeInput* const file)
{
	// see handleRecord() to understand what is done with
	// blockName here... Ugly code, but who cares :-)
	this->blockName = nullptr;
	bool ret = DimeEntity::read(file);
	if (ret && this->blockName)
	{
		auto tmp = (char*)this->blockName;
		this->blockName = file->getModel()->findRefStringPtr(tmp);
		if (this->blockName)
		{
			this->block = static_cast<DimeBlock*>(file->getModel()->findReference(tmp));
		}
		else
		{
			// probably a forward reference, just add as reference
			this->blockName = file->getModel()->addReference(tmp, nullptr);
		}
		delete [] tmp;
	}

	if (ret && this->attributesFollow)
	{
		// read following entities.
		dimeArray<DimeEntity*> array;
		ret = DimeEntity::readEntities(file, array, "SEQEND");
		if (ret)
		{
			this->seqend = DimeEntity::createEntity("SEQEND");
			// read the SEQEND entity
			if (!this->seqend || !this->seqend->read(file)) ret = false;
		}
		int n = array.count();
		if (ret && n)
		{
			this->entities = ARRAY_NEW(DimeEntity*, n);
			if (this->entities)
			{
				this->numEntities = n;
				for (int i = 0; i < n; i++)
				{
					this->entities[i] = array[i];
				}
			}
			else ret = false;
		}
	}
	return ret;
}

/*!
  Writes an INSERT entity.  
*/

bool
DimeInsert::write(DimeOutput* const file)
{
	this->preWrite(file);

	if (this->attributesFollow)
	{
		file->writeGroupCode(66);
		file->writeInt16(1);
	}
	file->writeGroupCode(2);
	file->writeString(this->blockName);

	file->writeGroupCode(10);
	file->writeDouble(insertionPoint[0]);
	file->writeGroupCode(20);
	file->writeDouble(insertionPoint[1]);
	file->writeGroupCode(30);
	file->writeDouble(insertionPoint[2]);

	if (this->scale != dimeVec3(1, 1, 1))
	{
		file->writeGroupCode(41);
		file->writeDouble(this->scale[0]);
		file->writeGroupCode(42);
		file->writeDouble(this->scale[1]);
		file->writeGroupCode(43);
		file->writeDouble(this->scale[2]);
	}
	if (this->rotAngle != 0.0)
	{
		file->writeGroupCode(50);
		file->writeDouble(this->rotAngle);
	}

	if (this->columnCount != 1)
	{
		file->writeGroupCode(70);
		file->writeInt16(this->columnCount);
	}
	if (this->rowCount != 1)
	{
		file->writeGroupCode(71);
#ifdef DIME_FIXBIG
    file->writeInt32(this->rowCount);
#else
		file->writeInt16(this->rowCount);
#endif
	}
	if (this->columnSpacing != 0.0)
	{
		file->writeGroupCode(44);
		file->writeDouble(this->columnSpacing);
	}
	if (this->rowSpacing != 0.0)
	{
		file->writeGroupCode(45);
		file->writeDouble(this->rowSpacing);
	}

	bool ret = DimeEntity::write(file); // write unknown records

	if (this->extrusionDir != dimeVec3(0, 0, 1))
	{
		file->writeGroupCode(210);
		file->writeDouble(this->extrusionDir[0]);
		file->writeGroupCode(220);
		file->writeDouble(this->extrusionDir[1]);
		file->writeGroupCode(230);
		ret = file->writeDouble(this->extrusionDir[2]);
	}

	if (this->attributesFollow && this->numEntities)
	{
		int i;
		for (i = 0; i < this->numEntities; i++)
		{
			if (!this->entities[i]->write(file)) break;
		}
		if (this->seqend) ret = this->seqend->write(file);
		else
		{
			file->writeGroupCode(0);
			ret = file->writeString("SEQEND");
		}
	}
	return ret;
}

//!

DimeBase::TypeID
DimeInsert::typeId() const
{
	return DimeBase::dimeInsertType;
}

//!

bool
DimeInsert::handleRecord(const int groupcode,
                         const dimeParam& param)
{
	switch (groupcode)
	{
	case 66:
		this->attributesFollow = param.int16_data;
		return true;
	case 2:
		{
			// will only arrive here during read(). Allocate a temporary buffer
			// to store the blockname. Will be deleted in dimeInsert::read() 
			const char* str = param.string_data;
			if (str)
			{
				this->blockName = new char[strlen(str) + 1];
				if (this->blockName)
				{
					strcpy((char*)this->blockName, str);
				}
			}
			return true;
		}
	case 10:
	case 20:
	case 30:
		this->insertionPoint[groupcode / 10 - 1] = param.double_data;
		return true;
	case 210:
	case 220:
	case 230:
		this->extrusionDir[(groupcode - 210) / 10] = param.double_data;
		return true;
	case 41:
	case 42:
	case 43:
		this->scale[groupcode - 41] = param.double_data;
		return true;
	case 44:
		this->columnSpacing = param.double_data;
		return true;
	case 45:
		this->rowSpacing = param.double_data;
		return true;
	case 50:
		this->rotAngle = param.double_data;
		return true;
	case 70:
		this->columnCount = param.int16_data;
		return true;
	case 71:
#ifdef DIME_FIXBIG
    this->rowCount = param.int32_data;
#else
		this->rowCount = param.int16_data;
#endif
		return true;
	}
	return DimeEntity::handleRecord(groupcode, param);
}

//!

const char*
DimeInsert::getEntityName() const
{
	return entityName;
}

//!

bool
DimeInsert::getRecord(const int groupcode,
                      dimeParam& param,
                      const int index) const
{
	switch (groupcode)
	{
	case 66:
		param.int16_data = this->attributesFollow;
		return true;
	case 2:
		param.string_data = this->blockName;
		return true;
	case 10:
	case 20:
	case 30:
		param.double_data = this->insertionPoint[groupcode / 10 - 1];
		return true;
	case 210:
	case 220:
	case 230:
		param.double_data = this->extrusionDir[(groupcode - 210) / 10];
		return true;
	case 41:
	case 42:
	case 43:
		param.double_data = this->scale[groupcode - 41];
		return true;
	case 44:
		param.double_data = this->columnSpacing;
		return true;
	case 45:
		param.double_data = this->rowSpacing;
		return true;
	case 50:
		param.double_data = this->rotAngle;
		return true;
	case 70:
		param.int16_data = this->columnCount;
		return true;
	case 71:
#ifdef DIME_FIXBIG
    param.int32_data = this->rowCount;
#else
		param.int16_data = this->rowCount;
#endif
		return true;
	}
	return DimeEntity::getRecord(groupcode, param, index);
}

//!

bool
DimeInsert::traverse(const DimeState* const state,
                     dimeCallback const& callback)
{
	DimeState newstate = *state;
	newstate.currentInsert = this;

	if (this->block && (state->getFlags() & DimeState::EXPLODE_INSERTS))
	{
		for (int i = 0; i < this->rowCount; i++)
		{
			for (int j = 0; j < this->columnCount; j++)
			{
				dimeMatrix m = state->getMatrix();
				dimeMatrix m2 = dimeMatrix::identity();
				m2.setTranslate(dimeVec3(j * this->columnSpacing,
				                          i * this->rowSpacing,
				                          0));
				m.multRight(m2);
				this->makeMatrix(m);
				newstate.setMatrix(m);
				if (!block->traverse(&newstate, callback)) return false;
			}
		}
	}
	else if (!this->isDeleted())
	{
		if (!callback(state, this)) return false;
	}

	dimeMatrix m = state->getMatrix();
	this->makeMatrix(m);
	newstate.setMatrix(m);

	// extract internal INSERT entities
	for (int i = 0; i < this->numEntities; i++)
	{
		if (!this->entities[i]->traverse(&newstate, callback)) return false;
	}
	return true;
}

//!

void
DimeInsert::fixReferences(DimeModel* const model)
{
	if (this->block == nullptr && this->blockName)
	{
		this->block = static_cast<DimeBlock*>(model->findReference(this->blockName));
		if (this->block == nullptr)
		{
			fprintf(stderr, "BLOCK %s not found!\n", blockName);
		}
	}
	for (int i = 0; i < this->numEntities; i++)
		this->entities[i]->fixReferences(model);
}

//!

void
DimeInsert::makeMatrix(dimeMatrix& m) const
{
	if (!this->block)
	{
		m.makeIdentity();
		return;
	}
	dimeMatrix m2;

	if (this->extrusionDir != dimeVec3(0, 0, 1))
	{
		// this block has its own coordinate system
		// generated from one vector (the z-vector)
		DimeEntity::generateUCS(this->extrusionDir, m2);
		m.multRight(m2);
	}

	m2.makeIdentity();
	dimeVec3 tmp = this->insertionPoint;

	// disabled for the moment
	// dimeModel::fixDxfCoords(tmp);

	m2.setTranslate(tmp);
	m.multRight(m2);

	m2.setTransform(dimeVec3(0, 0, 0),
	                this->scale,
	                dimeVec3(0, 0, this->rotAngle));
	m.multRight(m2);

	m2.makeIdentity();
	m2.setTranslate(-block->getBasePoint());
	m.multRight(m2);
}

//!

int
DimeInsert::countRecords() const
{
	int cnt = 5; // header + blockName + insertionPoint

	if (this->attributesFollow) cnt++;

	if (this->scale != dimeVec3(1.0, 1.0, 1.0)) cnt += 3;
	if (this->rotAngle != 0.0) cnt++;
	if (this->columnCount != 1) cnt++;
	if (this->rowCount != 1) cnt++;
	if (this->columnSpacing != 0.0) cnt++;
	if (this->rowSpacing != 0.0) cnt++;
	if (this->extrusionDir != dimeVec3(0, 0, 1)) cnt += 3;

	if (this->attributesFollow && this->numEntities)
	{
		int i, n = this->numEntities;
		for (i = 0; i < n; i++)
		{
			cnt += this->entities[i]->countRecords();
		}
		cnt++; // seqend
	}
	return cnt + DimeEntity::countRecords();
}

/*!
  Sets the block for this INSERT entity. This will change the record
  with group code 2.
*/

void
DimeInsert::setBlock(DimeBlock* const block)
{
	this->block = block;
	this->blockName = block->getName();
}
