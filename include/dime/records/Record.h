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

#ifndef DIME_RECORD_H
#define DIME_RECORD_H

#include <dime/Basic.h>
#include <dime/Base.h>
#include <stdio.h>

class DimeInput;
class DimeOutput;

class DIME_DLL_API DimeRecord : public DimeBase
{
public:
	DimeRecord(int group_code);
	~DimeRecord() override;

	virtual void setValue(const dimeParam& param, DimeMemHandler* memhandler = nullptr) = 0;
	virtual void getValue(dimeParam& param) const = 0;
	virtual DimeRecord* copy(DimeMemHandler* memhandler) const = 0;

	void setGroupCode(int group_code);
	int getGroupCode() const;

public:
	virtual bool isEndOfSectionRecord() const;
	virtual bool isEndOfFileRecord() const;
	TypeID typeId() const override = 0;
	virtual bool read(DimeInput* in) = 0;
	virtual bool write(DimeOutput* out);

public:
	static bool readRecordData(DimeInput* in, int group_code,
	                           dimeParam& param);
	static DimeRecord* readRecord(DimeInput* in);
	static DimeRecord* createRecord(int group_code,
	                                DimeMemHandler* memhandler);
	static DimeRecord* createRecord(int group_code,
	                                const dimeParam& param,
	                                DimeMemHandler* memhandler);
	static int getRecordType(int group_code);

protected:
	int groupCode;
}; // class dimeRecord

#endif // ! DIME_RECORD_H
