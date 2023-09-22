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
  \class DimeRecord dime/records/Record.h
  \brief The dimeRecord class is the superclass of all \e record classes.
*/

#include <dime/records/Record.h>

#include <dime/records/StringRecord.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/records/FloatRecord.h>
#include <dime/records/DoubleRecord.h>
#include <dime/records/HexRecord.h>
#include <dime/records/Int8Record.h>
#include <dime/records/Int16Record.h>
#include <dime/records/Int32Record.h>

/*!
  \fn void dimeRecord::setValue(const dimeParam &param, dimeMemHandler * const memhandler = NULL) = 0
  Sets the value of this record. The memory handler is needed by dimeStringRecord.
*/

/*!
  \fn void dimeRecord::getValue(dimeParam &param) const = 0
  Returns the value of this record.
*/

/*!
  Constructor which sets the group code.
*/

DimeRecord::DimeRecord(const int group_code)
	: groupCode(group_code)
{
}

/*!
  Destructor.
*/

DimeRecord::~DimeRecord()
{
}

/*!
  Sets the group code of this record.
*/

void
DimeRecord::setGroupCode(const int group_code)
{
	this->groupCode = group_code;
}

/*!
  Returns the group code for this record.
*/

int
DimeRecord::getGroupCode() const
{
	return this->groupCode;
}

/*!
  Returns \e true if this record is an end of section record.
*/

bool
DimeRecord::isEndOfSectionRecord() const
{
	return false;
}

/*!
  Returns \e true if this record is an end of file record.
*/

bool
DimeRecord::isEndOfFileRecord() const
{
	return false;
}

/*!
  \fn int dimeRecord::typeId() const
  This virtual function will return the type of the record. 
*/

/*!
  \fn bool dimeRecord::read(dimeInput * const in)
  This function will read the record from the dimeInput file.
*/

/*!
  This function will write the record to the dimeOutput file.
*/

bool
DimeRecord::write(DimeOutput* const out)
{
	return out->writeGroupCode(groupCode);
}

// * static methods *******************************************************

/*!
  Reads and returns the next record int file \a in.
*/

DimeRecord*
DimeRecord::readRecord(DimeInput* const in)
{
	int32_t groupcode;
	DimeRecord* rec = nullptr;
	if (in->readGroupCode(groupcode))
	{
		rec = createRecord(groupcode);
		if (rec) rec->read(in);
	}
	return rec;
}

/*!
  Static function that creates a record based on the group code.
*/

DimeRecord*
DimeRecord::createRecord(const int group_code)
{
	int type = getRecordType(group_code);
	DimeRecord* record = nullptr;
	switch (type)
	{
	case dimeStringRecordType:
		record = new dimeStringRecord(group_code);
		break;
	case dimeFloatRecordType:
		record = new dimeFloatRecord(group_code);
		break;
	case dimeDoubleRecordType:
		record = new dimeDoubleRecord(group_code);
		break;
	case dimeInt8RecordType:
		record = new dimeInt8Record(group_code);
		break;
	case dimeInt16RecordType:
		record = new dimeInt16Record(group_code);
		break;
	case dimeInt32RecordType:
		record = new dimeInt32Record(group_code);
		break;
	case dimeHexRecordType:
		record = new dimeHexRecord(group_code);
		break;
	default:
		assert(0);
		break;
	}
	return record;
}

//!

DimeRecord*
DimeRecord::createRecord(const int group_code,
                         const dimeParam& param)
{
	DimeRecord* record = createRecord(group_code);
	if (record) record->setValue(param);
	return record;
}

//
// local function that returns the type based on the group code
// used to build a look-up table
//

static int
get_record_type(const int group_code)
{
	int type = DimeBase::dimeStringRecordType;

	if (group_code < 0)
	{
		// not normally used in DXF files, but return string record to 
		// ensure correct read & write
		type = DimeBase::dimeStringRecordType;
	}

	else if (group_code <= 9)
	{
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 59)
	{
		// double precision floating point values.
		type = DimeBase::dimeDoubleRecordType;
	}
	// FIXME: this is a fix for some illegal files !!!!
#ifdef DIME_FIXBIG
  else if (group_code <= 70) {
    return dimeBase::dimeInt16RecordType;
  }
  else if (group_code <= 74) {
    return dimeBase::dimeInt32RecordType;
  }
#endif // DIME_FIXBIG
	else if (group_code <= 79)
	{
		type = DimeBase::dimeInt16RecordType;
	}
	else if (group_code <= 89)
	{
		// not defined yet. Use string.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 99)
	{
		type = DimeBase::dimeInt32RecordType;
	}
	else if (group_code < 140)
	{
		// only 100, 102 and 105 are defined. But use string for the rest also. 
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 147)
	{
		type = DimeBase::dimeDoubleRecordType;
	}
	else if (group_code < 170)
	{
		// not defined. Use string.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 178)
	{
		type = DimeBase::dimeInt16RecordType;
	}
	// XXX: this is not specified in the spec., but...
	else if (group_code == 210 || group_code == 220 || group_code == 230)
	{
		type = DimeBase::dimeDoubleRecordType;
	}
	else if (group_code < 270)
	{
		// not defined. Use string.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 275)
	{
		type = DimeBase::dimeInt8RecordType;
	}
	else if (group_code < 280)
	{
		// not defined.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 289)
	{
		type = DimeBase::dimeInt8RecordType;
	}
	else if (group_code < 300)
	{
		// not defined.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 309)
	{
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 319)
	{
		// binary chunk of data
		type = DimeBase::dimeHexRecordType;
	}
	else if (group_code <= 329)
	{
		// hex handle value
		type = DimeBase::dimeHexRecordType;
	}
	else if (group_code <= 369)
	{
		// hexvalue for object ID
		type = DimeBase::dimeHexRecordType;
	}
	else if (group_code < 999)
	{
		// not defined.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code == 999)
	{
		// comment
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 1009)
	{
		// 255 character max string.
		type = DimeBase::dimeStringRecordType;
	}
	else if (group_code <= 1059)
	{
		type = DimeBase::dimeStringRecordType;
		// should be float according to spec, but I have found
		// _huge_ numbers here that do not fit into a float.
		//    type = dimeBase::dimeFloatRecordType;
	}
	else if (group_code <= 1070)
	{
		type = DimeBase::dimeInt16RecordType;
	}
	else if (group_code == 1071)
	{
		type = DimeBase::dimeInt32RecordType;
	}
	else type = DimeBase::dimeStringRecordType;
	return type;
}

/*!
  Static function that returns the record type based on
  the group code.
*/

int
DimeRecord::getRecordType(const int group_code)
{
	static int first = 1;
	static int translation[1072];
	if (first)
	{
		first = 0;
		for (int i = 0; i < 1072; i++)
		{
			translation[i] = get_record_type(i);
		}
	}
	if (group_code < 0 || group_code >= 1072)
		return dimeStringRecordType;
	return translation[group_code];
}

/*!
  Will read the next item from \a in, and store result in
  \a param, based on the \a group_code.
*/

bool
DimeRecord::readRecordData(DimeInput* const in, const int group_code,
                           dimeParam& param)
{
	bool ret;
	int type = getRecordType(group_code);

	switch (type)
	{
	case dimeInt8RecordType:
		ret = in->readInt8(param.int8_data);
		break;
	case dimeInt16RecordType:
		ret = in->readInt16(param.int16_data);
		break;
	case dimeInt32RecordType:
		ret = in->readInt32(param.int32_data);
		break;
	case dimeFloatRecordType:
		ret = in->readFloat(param.float_data);
		break;
	case dimeDoubleRecordType:
		ret = in->readDouble(param.double_data);
		break;
	case dimeStringRecordType:
		if (group_code == 1)
		{
			param.string_data = in->readStringNoSkip();
		}
		else
		{
			param.string_data = in->readString();
		}
		ret = param.string_data != nullptr;
		break;
	case dimeHexRecordType:
		param.hex_data = in->readString();
		ret = param.hex_data != nullptr;
		break;
	default:
		assert(0);
		ret = false;
		break;
	}
	return ret;
}
