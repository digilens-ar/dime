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
  \class DimeLWPolyline dime/entities/LWPolyline.h
  \brief The dimeLWPolyline class handles an LWPOLYLINE \e entity.
*/

#include <dime/entities/LWPolyline.h>
#include <dime/records/Record.h>
#include <dime/Output.h>

#include <dime/Model.h>

static char entityName[] = "LWPOLYLINE";

//
// FIXME: after the first vertex is found, I should make sure no more 
// unknown records are found. If unknown records are found, the LWPOLYLINE
// will be illegal upon writing. The current code will work for releases
// up to r14, but might not work for r15+ if additional per-vertex data
// is added for those future file formats. 
//

/*!
  Constructor.
*/

DimeLWPolyline::DimeLWPolyline()
	: constantWidth(0.0), elevation(0.0), flags(0), numVertices(0),
	  xcoord(nullptr), ycoord(nullptr), startingWidth(nullptr), endWidth(nullptr),
	  bulge(nullptr)
{
}

/*!
  Destructor.
*/

DimeLWPolyline::~DimeLWPolyline()
{
	delete[] this->xcoord;
	delete[] this->ycoord;
	delete[] this->startingWidth;
	delete[] this->endWidth;
	delete[] this->bulge;
}

//!

DimeEntity*
DimeLWPolyline::copy(DimeModel* const model) const
{
	auto l = new DimeLWPolyline;
	if (!l) return nullptr;


	if (!this->copyRecords(l, model))
	{
		// check if allocated on heap.
		delete l;
		l = nullptr;
	}
	else
	{
		const int num = this->numVertices;
		if (num > 0)
		{
			l->xcoord = ARRAY_NEW(dxfdouble, num);
			l->ycoord = ARRAY_NEW(dxfdouble, num);
			l->bulge = ARRAY_NEW(dxfdouble, num);
			if (this->startingWidth)
			{
				l->startingWidth = ARRAY_NEW(dxfdouble, num);
				l->endWidth = ARRAY_NEW(dxfdouble, num);
			}
			for (int i = 0; i < num; i++)
			{
				l->xcoord[i] = this->xcoord[i];
				l->ycoord[i] = this->ycoord[i];
				l->bulge[i] = this->bulge[i];
				if (this->startingWidth)
				{
					l->startingWidth[i] = this->startingWidth[i];
					l->endWidth[i] = this->endWidth[i];
				}
			}
		}
		l->flags = this->flags;
		l->numVertices = this->numVertices;
		l->constantWidth = this->constantWidth;
		l->elevation = this->elevation;
		l->copyExtrusionData(this);
	}
	return l;
}

//!

bool
DimeLWPolyline::write(DimeOutput* const file)
{
	this->preWrite(file);


	file->writeGroupCode(90);
	bool ret = file->writeInt16(static_cast<int16_t>(this->numVertices));

	if (ret && this->flags != 0)
	{
		file->writeGroupCode(70);
		ret = file->writeInt16(this->flags);
	}
	if (ret && this->elevation != 0.0)
	{
		file->writeGroupCode(38);
		ret = file->writeDouble(this->elevation);
	}
	if (ret && this->constantWidth != 0.0)
	{
		file->writeGroupCode(43);
		ret = file->writeDouble(this->constantWidth);
	}

	if (!ret) return false;

	// write extrusion data and unksnown records
	ret = this->writeExtrusionData(file) &&
		DimeEntity::write(file);

	if (ret)
	{
		const int num = this->numVertices;
		for (int i = 0; ret && i < num; i++)
		{
			file->writeGroupCode(10);
			file->writeDouble(this->xcoord[i]);
			file->writeGroupCode(20);
			ret = file->writeDouble(this->ycoord[i]);

			if (ret && this->startingWidth && this->endWidth)
			{
				if (this->startingWidth[i] != 0.0)
				{
					file->writeGroupCode(40);
					file->writeDouble(this->startingWidth[i]);
				}
				if (this->endWidth[i] != 0.0)
				{
					file->writeGroupCode(41);
					ret = file->writeDouble(this->endWidth[i]);
				}
			}
			if (ret && this->bulge[i] != 0.0)
			{
				file->writeGroupCode(42);
				ret = file->writeDouble(this->bulge[i]);
			}
		}
	}

	return ret;
}

//!

DimeBase::TypeID
DimeLWPolyline::typeId() const
{
	return DimeBase::dimeLWPolylineType;
}

/*!
  Handles the callback from dimeEntity::readRecords().
*/

bool
DimeLWPolyline::handleRecord(const int groupcode,
                             const dimeParam& param)
{
	switch (groupcode)
	{
	case 10:
	case 20:
	case 40:
	case 41:
	case 42:
		{
			if (this->xcoord == nullptr)
			{
				// allocate arrays when first vertex is found
				const int num = this->numVertices;
				if (num <= 0)
				{
					fprintf(stderr, "LWPOLYLINE shouldn't have any vertices, but still found one!\n");
					return true; // data is "handled" so... 
				}
				this->xcoord = ARRAY_NEW(dxfdouble, num);
				this->ycoord = ARRAY_NEW(dxfdouble, num);
				this->bulge = ARRAY_NEW(dxfdouble, num);
				if (this->constantWidth == 0.0)
				{
					this->startingWidth = ARRAY_NEW(dxfdouble, num);
					this->endWidth = ARRAY_NEW(dxfdouble, num);
				}
				// must initialize arrays to default values
				for (int i = 0; i < num; i++)
				{
					this->bulge[i] = 0.0;
					if (this->startingWidth)
					{
						this->startingWidth[i] = 0.0;
						this->endWidth[i] = 0.0;
					}
				}
				this->tmpCounter = 0; // reset counters before going any further
				this->tmpFlags = 0;
			}
			int flagmask;
			dxfdouble* arrayptr;
			switch (groupcode)
			{
			case 10:
				flagmask = 0x1;
				arrayptr = this->xcoord;
				break;
			case 20:
				flagmask = 0x2;
				arrayptr = this->ycoord;
				break;
			case 40:
				flagmask = 0x4;
				arrayptr = this->startingWidth;
				break;
			case 41:
				flagmask = 0x8;
				arrayptr = this->endWidth;
				break;
			case 42:
				flagmask = 0x10;
				arrayptr = this->bulge;
				break;
			default:
				flagmask = 0;
				arrayptr = nullptr;
				assert(0);
				break;
			}
			if (this->tmpFlags & flagmask)
			{
				this->tmpFlags = 0;
				this->tmpCounter++;
			}
			if (this->tmpCounter >= this->numVertices)
			{
				fprintf(stderr, "too many vertices in LWPOLYLINE!\n");
				return true;
			}
			if (arrayptr == nullptr)
			{
				fprintf(stderr, "illegal data found in LWPOLYLINE.\n");
				return true;
			}
			this->tmpFlags |= flagmask;
			arrayptr[this->tmpCounter] = param.double_data;
			return true;
		}
	case 38:
		this->elevation = param.double_data;
		return true;
	case 43:
		this->constantWidth = param.double_data;
		return true;
	case 70:
		this->flags = param.int16_data;
		return true;
	case 90:
		this->numVertices = param.int32_data;
		return true;
	}
	return DimeExtrusionEntity::handleRecord(groupcode, param);
}

//!

const char*
DimeLWPolyline::getEntityName() const
{
	return entityName;
}

//!

bool
DimeLWPolyline::getRecord(const int groupcode,
                          dimeParam& param,
                          const int index) const
{
	switch (groupcode)
	{
	case 10:
		if (this->xcoord && index >= 0 && index < this->numVertices)
		{
			param.double_data = this->xcoord[index];
			return true;
		}
		return false;
	case 20:
		if (this->ycoord && index >= 0 && index < this->numVertices)
		{
			param.double_data = this->ycoord[index];
			return true;
		}
		return false;
	case 40:
		if (this->startingWidth && index >= 0 && index < this->numVertices)
		{
			param.double_data = this->startingWidth[index];
			return true;
		}
		return false;
	case 41:
		if (this->endWidth && index >= 0 && index < this->numVertices)
		{
			param.double_data = this->endWidth[index];
			return true;
		}
		return false;
	case 42:
		if (this->bulge && index >= 0 && index < this->numVertices)
		{
			param.double_data = this->bulge[index];
			return true;
		}
		return false;
	case 38:
		param.double_data = this->elevation;
		return true;
	case 43:
		param.double_data = this->constantWidth;
		return true;
	case 70:
		param.int16_data = this->flags;
		return true;
	case 90:
		param.int32_data = this->numVertices;
		return true;
	}
	return DimeExtrusionEntity::getRecord(groupcode, param, index);
}

DimeEntity::GeometryType
DimeLWPolyline::extractGeometry(dimeArray<dimeVec3>& verts,
                                dimeArray<int>&/*indices*/,
                                dimeVec3& extrusionDir,
                                dxfdouble& thickness)
{
	thickness = this->thickness;
	extrusionDir = this->extrusionDir;

	const int num = this->numVertices;
	for (int i = 0; i < num; i++)
	{
		verts.append(dimeVec3(this->xcoord[i],
		                       this->ycoord[i],
		                       this->elevation));
	}
	// is POLYLINE closed?
	if (this->flags & 1)
	{
		verts.append(dimeVec3(this->xcoord[0],
		                       this->ycoord[0],
		                       this->elevation));
	}
	return DimeEntity::LINES;
}

//!

int
DimeLWPolyline::countRecords() const
{
	int cnt = 2; // header + numVertices

	if (this->elevation != 0.0) cnt++;
	if (this->constantWidth != 0.0) cnt++;
	if (this->flags != 0) cnt++;
	cnt += this->numVertices * 2; // x and y coordinates

	// count optional per-vertex records
	for (int i = 0; i < this->numVertices; i++)
	{
		if (this->bulge[i] != 0.0) cnt++;
		if (this->startingWidth && this->startingWidth[i] != 0.0) cnt++;
		if (this->endWidth && this->endWidth[i] != 0.0) cnt++;
	}

	return cnt + DimeExtrusionEntity::countRecords();
}

int
DimeLWPolyline::getNumVertices() const
{
	return this->numVertices;
}

const dxfdouble*
DimeLWPolyline::getXCoords() const
{
	return this->xcoord;
}

const dxfdouble*
DimeLWPolyline::getYCoords() const
{
	return this->ycoord;
}

const dxfdouble*
DimeLWPolyline::getStartingWidths() const
{
	return this->startingWidth;
}

const dxfdouble*
DimeLWPolyline::getEndWidths() const
{
	return this->endWidth;
}

const dxfdouble*
DimeLWPolyline::getBulges() const
{
	return this->bulge;
}

dxfdouble
DimeLWPolyline::getElevation() const
{
	return this->elevation;
}

dxfdouble
DimeLWPolyline::getConstantWidth() const
{
	return this->constantWidth;
}

int16_t
DimeLWPolyline::getFlags() const
{
	return this->flags;
}
