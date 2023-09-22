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

#ifndef DIME_UCSTABLE_H
#define DIME_UCSTABLE_H

#include <dime/tables/TableEntry.h>
#include <dime/util/Linear.h>

class DIME_DLL_API DimeUCSTable : public DimeTableEntry
{
public:
	DimeUCSTable();

	DimeTableEntry* copy(DimeModel* model) const override;
	const char* getTableName() const override;

	const dimeVec3& getOrigin() const;
	const dimeVec3& getXaxis() const;
	const dimeVec3& getYaxis() const;

	void setOrigin(const dimeVec3& v);
	void setXaxis(const dimeVec3& v);
	void setYaxis(const dimeVec3& v);

	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcodes,
	                  const dimeParam& param) override;

private:
	dimeVec3 origin;
	dimeVec3 xaxis;
	dimeVec3 yaxis;
}; // class dimeUCSTable

inline const dimeVec3&
DimeUCSTable::getOrigin() const
{
	return this->origin;
}

inline const dimeVec3&
DimeUCSTable::getXaxis() const
{
	return this->xaxis;
}

inline const dimeVec3&
DimeUCSTable::getYaxis() const
{
	return this->yaxis;
}

inline void
DimeUCSTable::setOrigin(const dimeVec3& v)
{
	this->origin = v;
}

inline void
DimeUCSTable::setXaxis(const dimeVec3& v)
{
	this->origin = v;
}

inline void
DimeUCSTable::setYaxis(const dimeVec3& v)
{
	this->origin = v;
}

#endif // ! DIME_UCSTABLE_H
