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
  \class DimeObject dime/objects/Object.h
  \brief The dimeObject class is the superclass for the \e object classes.
*/

#include <dime/objects/Object.h>
#include <dime/objects/UnknownObject.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>

#include <string.h>

/*!
  \fn const char * dimeObject::getObjectName() const = 0
*/

/*!
  \fn dimeObject * dimeObject::copy(dimeModel * const model) const = 0
*/

/*!
  \fn int dimeObject::typeId() const = 0
*/

/*!
  Constructor.
*/

DimeObject::DimeObject()
	: DimeRecordHolder(0) // objects are separated by group code 0
{
}

/*!
  Destructor.
*/

DimeObject::~DimeObject()
{
}

/*!
  Copies the common and unobjectified records.
*/

bool
DimeObject::copyRecords(DimeObject* const myobject, DimeModel* const model) const
{
	DimeMemHandler* memh = model->getMemHandler();
	bool ok = DimeRecordHolder::copyRecords(myobject, memh);
	return ok;
}

/*!
  Writes common and unknown object records to file.
*/

bool
DimeObject::write(DimeOutput* const file)
{
	return DimeRecordHolder::write(file);
}

/*!
  Static function which creates an object based on its name. 
*/

DimeObject*
DimeObject::createObject(const char* const name,
                         DimeMemHandler* const memhandler)
{
	return new(memhandler) dimeUnknownObject(name, memhandler);
}

//!

int
DimeObject::countRecords() const
{
	return DimeRecordHolder::countRecords();
}

//!

bool
DimeObject::isOfType(const int thetypeid) const
{
	return thetypeid == dimeObjectType ||
		DimeRecordHolder::isOfType(thetypeid);
}

/*!
  Reads an object from \a in. Can be overloaded by subobjects, but in most
  cases this will not be necessary.

  \sa dimeObject::handleRecord().
*/

bool
DimeObject::read(DimeInput* const file)
{
	return DimeRecordHolder::read(file);
}

//!

bool
DimeObject::handleRecord(const int /*groupcode*/,
                         const dimeParam&/*param*/,
                         DimeMemHandler* const /*memhandler*/)
{
	// no groupcodes supported yet...
	return false;
}

/*!
  \fn void dimeObject::print() const
*/
