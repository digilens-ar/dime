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
  \class dimeUnknownObject dime/objects/UnknownObject.h
  \brief The dimeUnknownObject class reads and writes undefined objects.
*/

#include <dime/objects/UnknownObject.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <string.h>

/*!
  Constructor.
*/

dimeUnknownObject::dimeUnknownObject(const char* const name)
{
	DXF_STRCPY(this->objectName, name);
}

/*!
  Destructor. Should only be called if no memory handler is used.
*/

dimeUnknownObject::~dimeUnknownObject()
{
	delete [] this->objectName;
}

//!

DimeObject*
dimeUnknownObject::copy(DimeModel* const model) const
{
	auto u = new dimeUnknownObject(this->objectName);
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
dimeUnknownObject::write(DimeOutput* const file)
{
	if (file->writeGroupCode(0) && file->writeString(this->objectName))
		return DimeObject::write(file);
	return false;
}

//!

DimeBase::TypeID
dimeUnknownObject::typeId() const
{
	return DimeBase::dimeUnknownObjectType;
}

//!

int
dimeUnknownObject::countRecords() const
{
	return 1 + DimeObject::countRecords();
}

//!

const char*
dimeUnknownObject::getObjectName() const
{
	return this->objectName;
}
