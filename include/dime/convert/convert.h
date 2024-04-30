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

#ifndef _DXF2VRML_CONVERT_H_
#define _DXF2VRML_CONVERT_H_

#include <stdio.h>
#include <dime/Basic.h>

class DimeModel;
class dxfLayerData;
class DimeState;
class DimeEntity;

class  dxfConverter
{
public:
	dxfConverter();
	~dxfConverter();

	void setMaxerr(const dxfdouble maxerr)
	{
		this->maxerr = maxerr;
	}

	void findHeaderVariables(DimeModel& model);
	bool doConvert(DimeModel& model);
	bool writeVrml(const char* filename, bool vrml1 = false,
	               bool only2d = false);
	bool writeVrml(FILE* out, bool vrml1 = false,
	               bool only2d = false);

	void setNumSub(const int num)
	{
		this->numsub = num;
	}

	int getNumSub() const
	{
		return numsub;
	}

	dxfdouble getMaxerr() const
	{
		return this->maxerr;
	}

	void setFillmode(const bool fill)
	{
		this->fillmode = fill;
	}

	bool getFillmode() const
	{
		return this->fillmode;
	}

	bool getLayercol() const
	{
		return this->layercol;
	}

	void setLayercol(const bool v)
	{
		this->layercol = v;
	}

	dxfLayerData* getLayerData(int colidx);
	dxfLayerData* getLayerData(const DimeEntity* entity);
	dxfLayerData** getLayerData();
	int getColorIndex(const DimeEntity* entity);

	int getCurrentInsertColorIndex() const
	{
		return currentInsertColorIndex;
	}

private:
	friend class dime2Profit;
	friend class dime2So;

	dxfLayerData* layerData[255];
	int dummy[4];
	dxfdouble maxerr;
	int currentInsertColorIndex;
	DimeEntity* currentPolyline;
	int numsub;
	bool fillmode;
	bool layercol;
};

#endif // _DXF2VRML_CONVERT_H_
