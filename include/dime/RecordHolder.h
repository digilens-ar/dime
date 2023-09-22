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

#ifndef DIME_RECORDHOLDER_H
#define DIME_RECORDHOLDER_H

#include <dime/Base.h>

class DimeInput;

class DimeOutput;
class DimeRecord;

class DIME_DLL_API DimeRecordHolder : public DimeBase
{
public:
	DimeRecordHolder(int separator);
	~DimeRecordHolder() override;

	void setRecord(int groupcode, const dimeParam& value);
	void setRecords(const int* groupcodes,
	                const dimeParam* params,
	                int numrecords);
	void setIndexedRecord(int groupcode,
	                      const dimeParam& value,
	                      int index);

	virtual bool getRecord(int groupcode,
	                       dimeParam& param,
	                       int index = 0) const;

	virtual bool read(DimeInput* in);
	virtual bool write(DimeOutput* out);
	bool isOfType(int thetypeid) const override;
	virtual int countRecords() const;

	DimeRecord* findRecord(int groupcode, int index = 0);

	int getNumRecordsInRecordHolder(void) const;
	DimeRecord* getRecordInRecordHolder(int idx) const;

protected:
	virtual bool handleRecord(int groupcode,
	                          const dimeParam& param);

	bool copyRecords(DimeRecordHolder* rh) const;

	virtual bool shouldWriteRecord(int groupcode) const;

protected:
	DimeRecord** records;
	int numRecords;
	// int separator; // not needed ?

private:
	void setRecordCommon(int groupcode, const dimeParam& param,
	                     int index);
}; // class dimeRecordHolder

#endif // ! DIME_RECORDHOLDER_H
