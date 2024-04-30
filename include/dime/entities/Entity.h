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

#ifndef DIME_ENTITY_H
#define DIME_ENTITY_H

#include <dime/Base.h>
#include <dime/Basic.h>
#include <dime/util/Array.h>
#include <dime/util/Linear.h>
#include <dime/RecordHolder.h>


// misc flag values used in entityFlags.
#define FLAG_DELETED          0x0001 // used by dimeEntity
#define FLAG_TMP_BUFFER_SET   0x0002 // see dimeEntity::read()
#define FLAG_VERTICES_FOLLOW  0x0004 // used by dimePolyline
#define FLAG_TAGGED           0x0008 // used by dimeEntity
#define FLAG_COLOR_NUMBER     0x0010 // signals a color number was read 
#define FLAG_SUBCLASS_MARKER  0x0020 // will subclass marker need to be written
#define FLAG_HANDLE           0x0040 // entity has handle in RecordHolder
#define FLAG_ACAD_REACTORS    0x0080 // ACAD reactors in entity
#define FLAG_ACAD_XDICTIONARY 0x0100 // ACAD xdictionary in entity
#define FLAG_PAPERSPACE       0x0200 // entity is in paperspace
#define FLAG_LINETYPE         0x0400 // linetype specified in entity
#define FLAG_FIRST_FREE       0x0800 // use this if you want to define your own flags

class dimeLayer;
class DimeModel;

class  DimeEntity : public DimeRecordHolder
{
	friend class DimeEntitiesSection;
	friend class DimeModel;
	friend class DimePolyline;
	friend class DimeBlock;
	friend class DimeInsert;

public:
	DimeEntity();
	~DimeEntity() override;

	int16_t getEntityFlags() const;
	void setEntityFlags(int16_t flags);

	int16_t getColorNumber() const;
	void setColorNumber(int16_t c);

	virtual void setLayer(const dimeLayer* layer);
	virtual const char* getEntityName() const = 0;

	const dimeLayer* getLayer() const;
	const char* getLayerName() const;

	virtual DimeEntity* copy(DimeModel* model) const = 0;
	bool read(DimeInput* in) override;
	bool write(DimeOutput* out) override;
	bool isOfType(int thetypeid) const override;
	int countRecords() const override;

	bool isDeleted() const;
	void setDeleted(bool onOff = true);

	bool isTagged() const;
	void setTagged(bool onOff = true);

	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index = 0) const override;

	enum GeometryType
	{
		NONE,
		POLYGONS,
		LINES,
		POINTS
	};

	virtual GeometryType extractGeometry(dimeArray<dimeVec3>& verts,
	                                     dimeArray<int>& indices,
	                                     dimeVec3& extrusionDir,
	                                     dxfdouble& thickness);

protected:
	bool preWrite(DimeOutput* file);

	virtual bool traverse(const DimeState* state,
	                      dimeCallback const& callback);

	virtual void fixReferences(DimeModel* model);
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;
	bool shouldWriteRecord(int groupcode) const override;

public:
	static DimeEntity* createEntity(const char* name);
	static bool readEntities(DimeInput* file,
	                         dimeArray<DimeEntity*>& array,
	                         const char* stopat);

	static bool copyEntityArray(const DimeEntity* const* const array,
	                            int nument,
	                            DimeModel* model,
	                            dimeArray<DimeEntity*>& destarray);
	static DimeEntity** copyEntityArray(const DimeEntity* const* const array,
	                                    int& nument,
	                                    DimeModel* model);

	static void arbitraryAxis(const dimeVec3& givenaxis, dimeVec3& newaxis);
	static void generateUCS(const dimeVec3& givenaxis, dimeMatrix& m);

protected:
	bool copyRecords(DimeEntity* entity, DimeModel* model) const;

private:
	const dimeLayer* layer;
	int16_t entityFlags;
	int16_t colorNumber;
}; // class dimeEntity

inline const dimeLayer*
DimeEntity::getLayer() const
{
	return this->layer;
}

inline int16_t
DimeEntity::getColorNumber() const
{
	return this->colorNumber;
}

inline void
DimeEntity::setColorNumber(const int16_t c)
{
	this->colorNumber = c;
}

inline int16_t
DimeEntity::getEntityFlags() const
{
	return this->entityFlags;
}

inline void
DimeEntity::setEntityFlags(const int16_t flags)
{
	this->entityFlags = flags;
}


#endif // ! DIME_ENTITY_H
