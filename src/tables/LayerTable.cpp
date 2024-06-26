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
  \class DimeLayerTable dime/tables/LayerTable.h
  \brief The dimeLayerTable class reads and writes LAYER \e tables.
*/

#include <dime/tables/LayerTable.h>
#include <dime/Input.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <dime/records/Record.h>
#include <string.h>

static constexpr char tableName[] = "LAYER";

/*!
  Constructor.
*/

DimeLayerTable::DimeLayerTable()
	: colorNumber(0), layerName(nullptr), layerInfo(nullptr)
{
}

DimeLayerTable::~DimeLayerTable()
{
	delete [] this->layerName;
}

//!

DimeTableEntry*
DimeLayerTable::copy(DimeModel* const model) const
{
	auto l = new DimeLayerTable;
	l->colorNumber = this->colorNumber;
	if (this->layerName)
	{
		DXF_STRCPY(l->layerName, this->layerName);
	}
	if (this->layerInfo)
	{
		l->layerInfo = model->addLayer(this->layerInfo->getLayerName(), DXFABS(this->colorNumber));
	}
	if (!copyRecords(l, model))
	{
		// check if allocated on heap.
		delete l;
		l = nullptr;
	}
	return l;
}

//!

const char*
DimeLayerTable::getTableName() const
{
	return tableName;
}

//!

bool
DimeLayerTable::read(DimeInput* const file)
{
	bool ret = DimeTableEntry::read(file);
	if (ret)
	{
		this->registerLayer(file->getModel());
	}
	return ret;
}

//!

bool
DimeLayerTable::write(DimeOutput* const file)
{
	bool ret = DimeTableEntry::preWrite(file);

	if (this->layerName)
	{
		ret = file->writeGroupCode(2);
		file->writeString(this->layerName);
	}
	file->writeGroupCode(62);
	file->writeInt16(this->colorNumber);

	ret = DimeTableEntry::write(file);
	return ret;
}

//!

DimeBase::TypeID
DimeLayerTable::typeId() const
{
	return DimeBase::dimeLayerTableType;
}

//!

bool
DimeLayerTable::handleRecord(const int groupcode,
                             const dimeParam& param)
{
	switch (groupcode)
	{
	case 2:
		this->setLayerName(param.string_data);
		return true;
	case 62:
		this->setColorNumber(param.int16_data);
		return true;
	}
	return DimeTableEntry::handleRecord(groupcode, param);
}

//!

int
DimeLayerTable::countRecords() const
{
	int cnt = 1; // header
	if (this->layerInfo) cnt++;
	cnt++; // colorNumber
	return cnt + DimeTableEntry::countRecords();
}

/*!
  Sets the layer name.
*/
void
DimeLayerTable::setLayerName(const char* name)
{
	if (this->layerName)
	{
		delete [] this->layerName;
	}
	DXF_STRCPY(this->layerName, name);
}

/*!
  Returns the layer name.
 */
const char*
DimeLayerTable::getLayerName(void) const
{
	return this->layerName;
}

/*!
  Sets the color number.
 */
void
DimeLayerTable::setColorNumber(const int16_t colnum)
{
	this->colorNumber = colnum;
	if (this->layerInfo) this->layerInfo->setColorNumber(DXFABS(this->colorNumber));
}

/*!
  Returns the color number.
 */
int16_t
DimeLayerTable::getColorNumber(void) const
{
	return this->colorNumber;
}

/*!
  Should be called _once_ after you've finished setting up your
  layer (name and color number).  Calling this method more than once
  for a layer might lead to hard-to-find bugs. After calling this
  method, the layer information (color number) will be available to
  entities using this layer.
*/
void
DimeLayerTable::registerLayer(DimeModel* model)
{
	if (this->layerInfo == nullptr && this->layerName != nullptr)
	{
		this->layerInfo = model->addLayer(this->layerName, DXFABS(this->colorNumber));
	}
}
