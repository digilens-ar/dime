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

#ifndef DIME_MODEL_H
#define DIME_MODEL_H

#include <dime/Basic.h>
#include <dime/util/Array.h>
#include <dime/util/Linear.h>
#include <dime/Base.h>
#include <dime/Layer.h>
#include <stdlib.h>

class DimeInput;
class DimeOutput;
class dimeDict;
class DimeSection;
class DimeEntitiesSection;
class DimeBlocksSection;
class DimeBlock;
class DimeEntity;
class DimeRecord;

class  DimeModel
{
public:
	DimeModel();
	~DimeModel();

	DimeModel* copy() const;

	bool init();
	bool read(DimeInput* in);
	bool write(DimeOutput* out);

	int countRecords() const;

	bool traverseEntities(dimeCallback const& callback,
	                      bool traverseBlocksSection = false,
	                      bool explodeInserts = true,
	                      bool traversePolylineVertices = false);

	const char* addReference(const char* name, void* id);
	void* findReference(const char* name) const;
	const char* findRefStringPtr(const char* name) const;
	void removeReference(const char* name);

	int getNumLayers() const;
	const  dimeLayer* getLayer(int idx) const;
	const  dimeLayer* getLayer(const char* layername) const;
	dimeLayer* addLayer(const char* layername,
	                    int16_t colnum = 7,
	                    int16_t flags = 0);

	const char* getDxfVersion() const;

	static const char* getVersionString();
	static void getVersion(int& major, int& minor);
	const char* addBlock(const char* blockname, DimeBlock* block);
	DimeBlock* findBlock(const char* blockname);

	DimeSection* findSection(const char* sectionname);
	const DimeSection* findSection(const char* sectionname) const;

	int getNumSections() const;
	DimeSection* getSection(int idx);
	void insertSection(DimeSection* section, int idx = -1);
	void removeSection(int idx);

	void registerHandle(int handle);
	void registerHandle(const char* handle);
	int getUniqueHandle();
	const char* getUniqueHandle(char* buf, int bufsize);
	void addEntity(DimeEntity* entity);

private:
	dimeDict* refDict;
	dimeDict* layerDict;
	dimeArray<DimeSection*> sections;
	dimeArray<dimeLayer*> layers;
	dimeArray<DimeRecord*> headerComments;

	int largestHandle;
}; 

#endif // ! DIME_MODEL_H
