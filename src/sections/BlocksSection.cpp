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
  \class dimeBlocksSection dime/sections/BlocksSection.h
  \brief The dimeBlocksSection class handles a BLOCKS \e section.
*/

#include <dime/sections/BlocksSection.h>
#include <dime/entities/Block.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>
#include <dime/util/Array.h>
#include <dime/Model.h>

static constexpr char sectionName[] = "BLOCKS";

/*!
  Constructor.
*/

DimeBlocksSection::DimeBlocksSection(DimeMemHandler* const memhandler)
	: DimeSection(memhandler)
{
}

/*!
  Destructor. Should only be called when no memory handler is used.
*/

DimeBlocksSection::~DimeBlocksSection()
{
	if (!this->memHandler)
	{
		for (int i = 0; i < this->blocks.count(); i++)
			delete this->blocks[i];
	}
}

//!

DimeSection*
DimeBlocksSection::copy(DimeModel* const model) const
{
	auto bs = new DimeBlocksSection(model->getMemHandler());
	for (int i = 0; i < this->blocks.count(); i++)
	{
		bs->blocks.append(static_cast<DimeBlock*>(this->blocks[i]->copy(model)));
	}
	return bs;
}

/*!
  This method reads a DXF BLOCKS section.
*/

bool
DimeBlocksSection::read(DimeInput* const file)
{
	int32 groupcode;
	const char* string;
	bool ok = true;
	DimeBlock* block = nullptr;
	DimeMemHandler* memhandler = file->getMemHandler();

	while (true)
	{
		if (!file->readGroupCode(groupcode) || groupcode != 0)
		{
			fprintf(stderr, "Error reading groupcode: %d\n", groupcode);
			ok = false;
			break;
		}
		string = file->readString();
		if (!strcmp(string, "ENDSEC")) break;
		if (strcmp(string, "BLOCK"))
		{
			fprintf(stderr, "Unexpected string.\n");
			ok = false;
			break;
		}
		block = static_cast<DimeBlock*>(DimeEntity::createEntity(string, memhandler));
		if (block == nullptr)
		{
			fprintf(stderr, "error creating block: %s\n", string);
			ok = false;
			break;
		}
		if (!block->read(file))
		{
			fprintf(stderr, "error reading block: %s.\n", string);
			ok = false;
			break;
		}
		this->blocks.append(block);
	}
	return ok;
}

/*!
  This method writes a DXF BLOCKS section.
*/

bool
DimeBlocksSection::write(DimeOutput* const file)
{
	if (file->writeGroupCode(2) && file->writeString(sectionName))
	{
		int i;
		for (i = 0; i < this->blocks.count(); i++)
		{
			if (!this->blocks[i]->write(file)) break;
		}
		if (i == this->blocks.count())
		{
			return file->writeGroupCode(0) && file->writeString("ENDSEC");
		}
	}
	return false;
}

//!

DimeBase::TypeID
DimeBlocksSection::typeId() const
{
	return DimeBase::dimeBlocksSectionType;
}

/*!
  This function should be called after loading has ended, and will
  find all forward BLOCK references.
*/

void
DimeBlocksSection::fixReferences(DimeModel* const model)
{
	int i, n = this->blocks.count();
	for (i = 0; i < n; i++)
	{
		this->blocks[i]->fixReferences(model);
	}
}

//!

int
DimeBlocksSection::countRecords() const
{
	int cnt = 0;
	int i, n = this->blocks.count();
	for (i = 0; i < n; i++)
		cnt += this->blocks[i]->countRecords();
	return cnt + 2; // two records are written in write() 
}

//!

const char*
DimeBlocksSection::getSectionName() const
{
	return sectionName;
}

/*!
  Returns the number of blocks in this section. 
*/

int
DimeBlocksSection::getNumBlocks() const
{
	return this->blocks.count();
}

/*!
  Returns the block at index \a idx.
*/

DimeBlock*
DimeBlocksSection::getBlock(const int idx)
{
	assert(idx >= 0 && idx < this->blocks.count());
	return this->blocks[idx];
}

/*!
  Removes (and deletes if no memory handler is used) the block at index \a idx.
*/

void
DimeBlocksSection::removeBlock(const int idx)
{
	assert(idx >= 0 && idx < this->blocks.count());
	if (!this->memHandler) delete this->blocks[idx];
	this->blocks.removeElem(idx);
}

/*!
  Inserts a new block at index \a idx. If \a idx is negative, the
  block will be inserted at the end of the list of blocks.
*/

void
DimeBlocksSection::insertBlock(DimeBlock* const block, const int idx)
{
	if (idx < 0) this->blocks.append(block);
	else
	{
		assert(idx <= this->blocks.count());
		this->blocks.insertElem(idx, block);
	}
}
