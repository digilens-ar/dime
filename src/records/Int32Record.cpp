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
  \class dimeInt32Record dime/records/Int32Record.h
  \brief The dimeInt32Record class is a container class for 32-bit integer records.
*/

#include <dime/records/Int32Record.h>
#include <dime/Input.h>
#include <dime/Output.h>

/*!
  Constructor
*/

dimeInt32Record::dimeInt32Record(const int group_code, const int32_t val)
	: DimeRecord(group_code)
{
	this->setValue(val);
}

//!

DimeRecord*
dimeInt32Record::copy() const
{
	return new dimeInt32Record(this->groupCode, this->value);
}

/*!
  Sets the int32_t value to \a val.
*/

void
dimeInt32Record::setValue(const int32_t val)
{
	this->value = val;
}

/*!
  Returns the int32_t value.
*/

int32_t
dimeInt32Record::getValue() const
{
	return this->value;
}

//!

DimeBase::TypeID
dimeInt32Record::typeId() const
{
	return DimeBase::dimeInt32RecordType;
}

//!

bool
dimeInt32Record::read(DimeInput* const in)
{
	return in->readInt32(this->value);
}

//!

bool
dimeInt32Record::write(DimeOutput* const out)
{
	if (DimeRecord::write(out))
	{
		return out->writeInt32(this->value);
	}
	return false;
}

//!

void
dimeInt32Record::setValue(const dimeParam& param)
{
	this->value = param.int32_data;
}

//!

void
dimeInt32Record::getValue(dimeParam& param) const
{
	param.int32_data = this->value;
}
