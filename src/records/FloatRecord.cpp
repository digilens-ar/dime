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
  \class dimeFloatRecord dime/records/FloatRecord.h
  \brief The dimeFloatRecord class is a container class for float records.
*/

#include <dime/records/FloatRecord.h>
#include <dime/Input.h>
#include <dime/Output.h>


/*!
  Constructor
*/

dimeFloatRecord::dimeFloatRecord(const int group_code, const float val)
	: DimeRecord(group_code)
{
	this->setValue(val);
}

//!

DimeRecord*
dimeFloatRecord::copy() const
{
	return new dimeFloatRecord(this->groupCode, this->value);
}

/*!
  Sets the float value.
*/

void
dimeFloatRecord::setValue(const float val)
{
	this->value = val;
}

//!

float
dimeFloatRecord::getValue() const
{
	return this->value;
}

//!

DimeBase::TypeID
dimeFloatRecord::typeId() const
{
	return DimeBase::dimeFloatRecordType;
}

//!

bool
dimeFloatRecord::read(DimeInput* const in)
{
	return in->readFloat(this->value);
}

//!

bool
dimeFloatRecord::write(DimeOutput* const out)
{
	if (DimeRecord::write(out))
	{
		return out->writeFloat(this->value);
	}
	return false;
}

//!

void
dimeFloatRecord::setValue(const dimeParam& param)
{
	this->value = param.float_data;
}

//!

void
dimeFloatRecord::getValue(dimeParam& param) const
{
	param.float_data = this->value;
}
