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
  \class DimeUnknownTable dime/tables/UnknownTable.h
  \brief The dimeUnknownTable class reads and writes undefined tables.
*/

#include <dime/tables/UnknownTable.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/records/Record.h>
#include <string.h>

/*!
  Constructor.
*/

DimeUnknownTable::DimeUnknownTable(const char* const name)
{
	DXF_STRCPY(this->tableName, name);
}

/*!
  Destructor. Should only be called if no memory handler is used.
*/

DimeUnknownTable::~DimeUnknownTable()
{
	delete [] this->tableName;
}

//!

DimeTableEntry*
DimeUnknownTable::copy(DimeModel* const model) const
{
	auto u = new DimeUnknownTable(this->tableName);
	if (!this->copyRecords(u, model))
	{
		// check if allocated on heap.
		delete u;
		u = nullptr;
	}
	return u;
}

//!

bool
DimeUnknownTable::write(DimeOutput* const file)
{
	bool ret = DimeTableEntry::preWrite(file);
	if (ret)
	{
		ret = DimeTableEntry::write(file);
	}
	return ret;
}

//!

DimeBase::TypeID
DimeUnknownTable::typeId() const
{
	return DimeBase::dimeUnknownTableType;
}

//!

int
DimeUnknownTable::countRecords() const
{
	return 1 + DimeTableEntry::countRecords();
}

//!

const char*
DimeUnknownTable::getTableName() const
{
	return this->tableName;
}
