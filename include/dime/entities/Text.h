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

#ifndef DIME_TEXT_H
#define DIME_TEXT_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>

class  DimeText : public DimeExtrusionEntity
{
public:
	DimeText();
	//  dimeText(const char* s);

	void setOrigin(const dimeVec3& o);
	dimeVec3 getOrigin() const;
	void setSecond(const dimeVec3& s);
	bool getSecond(dimeVec3& s) const;
	void setHeight(dxfdouble h);
	dxfdouble getHeight() const;
	void setWidth(dxfdouble w);
	dxfdouble getWidth() const;
	void setRotation(dxfdouble a);
	dxfdouble getRotation() const;
	void setHJust(int32_t h);
	int32_t getHJust() const;
	void setVJust(int32_t v);
	int32_t getVJust() const;
	void setTextString(const char* s);
	char* getTextString() const;

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;
	const char* getEntityName() const override;

	DimeEntity* copy(DimeModel* model) const override;

	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

	GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                             dimeArray<int>& indices,
	                             dimeVec3& extrusionDir,
	                             dxfdouble& thickness) override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

private:
	dimeVec3 origin;
	dimeVec3 second;
	bool haveSecond;
	dxfdouble height;
	dxfdouble width;
	dxfdouble rotation;
	dxfdouble wScale;
	int32_t hJust;
	int32_t vJust;
	char* text;
}; // class dimeText

//
// inline methods
//


inline void
DimeText::setOrigin(const dimeVec3& o)
{
	this->origin = o;
}

inline dimeVec3
DimeText::getOrigin() const
{
	return this->origin;
}

inline void
DimeText::setSecond(const dimeVec3& s)
{
	this->second = s;
}

inline bool
DimeText::getSecond(dimeVec3& s) const
{
	if (haveSecond)
	{
		s = this->second;
		return true;
	}
	return false;
}

inline void
DimeText::setHeight(const dxfdouble h)
{
	this->height = h;
}

inline dxfdouble
DimeText::getHeight() const
{
	return this->height;
}

inline void
DimeText::setWidth(const dxfdouble w)
{
	this->width = w;
}

inline dxfdouble
DimeText::getWidth() const
{
	return this->width;
}

inline void
DimeText::setRotation(const dxfdouble a)
{
	this->rotation = a;
}

inline dxfdouble
DimeText::getRotation() const
{
	return this->rotation;
}

inline void
DimeText::setHJust(const int32_t h)
{
	this->hJust = h;
}

inline int32_t
DimeText::getHJust() const
{
	return this->hJust;
}

inline void
DimeText::setVJust(const int32_t v)
{
	this->vJust = v;
}

inline int32_t
DimeText::getVJust() const
{
	return this->vJust;
}

//inline void 
//dimeText::setTextString(const char* s)
//{
//this->text = s;
//}

inline char*
DimeText::getTextString() const
{
	return this->text;
}

#endif // ! DIME_TEXT_H
