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

#ifndef DIME_OUTPUT_H
#define DIME_OUTPUT_H

#include <dime/Basic.h>
#include <stdio.h>

class DimeModel;

class  DimeOutput
{
public:
	DimeOutput();
	~DimeOutput();

	void setCallback(int numrecords,
	                 int (*cb)(float, void*), void* cbdata);
	bool setFileHandle(FILE* fp);
	bool setFilename(const char* filename);
	void setBinary(bool state = true);
	bool isBinary() const;

	bool writeHeader() { return true; }
	bool writeGroupCode(int groupcode);
	bool writeInt8(int8_t val);
	bool writeInt16(int16_t val);
	bool writeInt32(int32_t val);
	bool writeFloat(float val);
	bool writeDouble(dxfdouble val);
	bool writeString(const char* str);

	int getUniqueHandleId();

private:
	friend class DimeModel;
	DimeModel* model;
	FILE* fp;
	bool binary;

	int (*callback)(float, void*);
	void* callbackdata;
	int numrecords;
	int numwrites;
	bool aborted;
	bool didOpenFile;
}; // class dimeOutput

#endif // ! DIME_OUTPUT_H
