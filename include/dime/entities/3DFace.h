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

#ifndef DIME_3DFACE_H
#define DIME_3DFACE_H

#include <dime/Basic.h>
#include <dime/entities/FaceEntity.h>
#include <dime/util/Linear.h>

class  dime3DFace : public dimeFaceEntity
{
	friend class DimeEntitiesSection;
	friend class DimeBlock;
	friend class DimeModel;
	friend class DimeEntity;
	friend class DimeSolid;
	friend class DimeTrace;

public:
	dime3DFace();

	DimeEntity* copy(DimeModel* model) const override;

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	enum Flags
	{
		EDGE1_INVISIBLE = 0x0001,
		EDGE2_INVISIBLE = 0x0002,
		EDGE3_INVISIBLE = 0x0004,
		EDGE4_INVISIBLE = 0x0008
	};

	void setFlags(int16_t flags);
	int16_t getFlags() const;

	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

	int16_t flags;

public:
#ifndef NO_RR_DATA
	class DimeBlock* block; // ugly, needed for RR
#endif
}; // class dime3DFace

#endif // ! DIME_3DFACE_H
