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
  \class DimeSpline dime/entities/Spline.h
  \brief The dimeSpline class handles a SPLINE \e entity.
*/

#include <dime/entities/Spline.h>
#include <dime/records/Record.h>
#include <dime/Output.h>

#include <dime/Model.h>
#include <string.h>

#define DEFAULT_CP_TOLERANCE 1e-7      // 0.0000001
#define DEFAULT_FIT_TOLERANCE 1e-10    // 0.0000000001
#define DEFAULT_KNOT_TOLERANCE  1e-7   // 0.0000001

static char entityName[] = "SPLINE";

/*! 
  Constructor.
*/

DimeSpline::DimeSpline()
{
	this->flags = PLANAR;
	this->degree = 3; // FIXME: is this a correct default value
	this->numKnots = 0;
	this->numControlPoints = 0;
	this->numFitPoints = 0;
	this->knotTolerance = DEFAULT_KNOT_TOLERANCE;
	this->fitTolerance = DEFAULT_CP_TOLERANCE;
	this->cpTolerance = DEFAULT_CP_TOLERANCE;
	this->knots = NULL;
	this->weights = NULL;
	this->controlPoints = NULL;
	this->fitPoints = NULL;
}

/*!
  Destructor.
*/

DimeSpline::~DimeSpline()
{
	delete [] this->knots;
	delete [] this->weights;
	delete [] this->controlPoints;
	delete [] this->fitPoints;
}

/*!
  Returns \e true if the spline has control point weights.
*/

bool
DimeSpline::hasWeights() const
{
	if (weights == nullptr) return false;
	int i, n = this->numControlPoints;
	for (i = 0; i < n; i++)
	{
		if (this->weights[i] != 1.0) break;
	}
	return i < n;
}

//!

DimeEntity*
DimeSpline::copy(DimeModel* const model) const
{
	auto s = new DimeSpline;
	if (!s) return nullptr;

	if (!this->copyRecords(s, model))
	{
		// check if allocated on heap.
		delete s;
		s = nullptr;
	}
	else
	{
		int i, n;
		s->flags = this->flags;
		s->degree = this->degree;
		s->numKnots = this->numKnots;
		s->numControlPoints = this->numControlPoints;
		s->numFitPoints = this->numFitPoints;
		s->knotTolerance = this->knotTolerance;
		s->fitTolerance = this->fitTolerance;
		s->cpTolerance = this->cpTolerance;
		s->knots = ARRAY_NEW(dxfdouble, this->numKnots);
		if (this->knots)
		{
			n = this->numKnots;
			for (i = 0; i < n; i++)
			{
				s->knots[i] = this->knots[i];
			}
		}
		if (this->weights)
		{
			s->weights = ARRAY_NEW(dxfdouble, this->numControlPoints);
			n = this->numControlPoints;
			for (i = 0; i < n; i++)
			{
				s->weights[i] = this->weights[i];
			}
		}
		if (this->controlPoints)
		{
			s->controlPoints = ARRAY_NEW(dimeVec3, this->numControlPoints);
			n = this->numControlPoints;
			for (i = 0; i < n; i++)
			{
				s->controlPoints[i] = this->controlPoints[i];
			}
		}
		if (this->fitPoints)
		{
			s->fitPoints = ARRAY_NEW(dimeVec3, this->numFitPoints);
			n = this->numFitPoints;
			for (i = 0; i < n; i++)
			{
				s->fitPoints[i] = this->fitPoints[i];
			}
		}
	}
	return s;
}

//!

bool
DimeSpline::write(DimeOutput* const file)
{
	int i;

	DimeEntity::preWrite(file);

	bool ret = DimeEntity::write(file);

	// write counters first
	file->writeGroupCode(70);
	file->writeInt16(this->flags);
#ifdef DIME_FIXBIG
  file->writeGroupCode(71);
  file->writeInt32(this->degree);
  file->writeGroupCode(72);
  file->writeInt32(this->numKnots);
  file->writeGroupCode(73);
  file->writeInt32(this->numControlPoints);
  file->writeGroupCode(74);
  file->writeInt32(this->numFitPoints);
#else
	file->writeGroupCode(71);
	file->writeInt16(this->degree);
	file->writeGroupCode(72);
	file->writeInt16(this->numKnots);
	file->writeGroupCode(73);
	file->writeInt16(this->numControlPoints);
	file->writeGroupCode(74);
	file->writeInt16(this->numFitPoints);
#endif

	file->writeGroupCode(42);
	file->writeDouble(this->knotTolerance);
	file->writeGroupCode(43);
	file->writeDouble(this->cpTolerance);

	if (this->numFitPoints)
	{
		file->writeGroupCode(44);
		file->writeDouble(this->fitTolerance);
	}

	for (i = 0; i < this->numKnots; i++)
	{
		file->writeGroupCode(40);
		file->writeDouble(this->knots[i]);
	}

	if (this->hasWeights())
	{
		for (i = 0; i < this->numControlPoints; i++)
		{
			file->writeGroupCode(41);
			file->writeDouble(this->weights[i]);
		}
	}

	for (i = 0; i < this->numControlPoints; i++)
	{
		file->writeGroupCode(10);
		file->writeDouble(this->controlPoints[i][0]);
		file->writeGroupCode(20);
		file->writeDouble(this->controlPoints[i][1]);
		file->writeGroupCode(30);
		file->writeDouble(this->controlPoints[i][2]);
	}
	for (i = 0; i < this->numFitPoints; i++)
	{
		file->writeGroupCode(11);
		file->writeDouble(this->fitPoints[i][0]);
		file->writeGroupCode(21);
		file->writeDouble(this->fitPoints[i][1]);
		file->writeGroupCode(31);
		file->writeDouble(this->fitPoints[i][2]);
	}

	return ret;
}

//!

DimeBase::TypeID
DimeSpline::typeId() const
{
	return DimeBase::dimeSplineType;
}

//!

bool
DimeSpline::handleRecord(const int groupcode,
                         const dimeParam& param)
{
	switch (groupcode)
	{
	case 10: // control points
	case 20:
	case 30:
		if (this->controlPoints == nullptr && this->numControlPoints)
		{
			this->cpCnt = 0;
			this->controlPoints = ARRAY_NEW(dimeVec3, this->numControlPoints);
		}
		if (this->controlPoints && this->cpCnt < this->numControlPoints)
		{
			this->controlPoints[this->cpCnt][groupcode / 10 - 1] = param.double_data;
		}
		if (groupcode == 30) cpCnt++;
		return true;
	case 11: // fit points
	case 21:
	case 31:
		if (this->fitPoints == nullptr && this->numFitPoints)
		{
			this->fitCnt = 0;
			this->fitPoints = ARRAY_NEW(dimeVec3, this->numFitPoints);
		}
		if (this->fitPoints && this->fitCnt < this->numFitPoints)
		{
			this->fitPoints[this->fitCnt][groupcode / 10 - 1] = param.double_data;
		}
		if (groupcode == 31) fitCnt++;
		return true;
	case 40: // knot value, multiple values
		if (this->knots == nullptr && this->numKnots)
		{
			this->knotCnt = 0;
			this->knots = ARRAY_NEW(dxfdouble, this->numKnots);
		}
		if (this->knots && this->knotCnt < this->numKnots)
		{
			this->knots[this->knotCnt] = param.double_data;
		}
		this->knotCnt++;
		return true;
	case 41: // weight, multiple values, prersent if not all 1
		if (this->weights == nullptr && this->numControlPoints)
		{
			this->weights = ARRAY_NEW(dxfdouble, this->numControlPoints);
			this->weightCnt = 0;
		}
		if (this->weights && this->weightCnt < this->numControlPoints)
		{
			this->weights[this->weightCnt] = param.double_data;
		}
		this->weightCnt++;
		return true;
	case 42: // knot tol
		this->knotTolerance = param.double_data;
		return true;
	case 43: // cp tol
		this->cpTolerance = param.double_data;
		return true;
	case 44: // fit tol
		this->fitTolerance = param.double_data;
		return true;
	case 70: // flags
		this->flags = param.int16_data;
		return true;
	case 71: // degree
#ifdef DIME_FIXBIG
    this->degree = param.int32_data;
#else
		this->degree = param.int16_data;
#endif
		return true;
	case 72: // num knots
#ifdef DIME_FIXBIG
    this->numKnots = param.int32_data;
#else
		this->numKnots = param.int16_data;
#endif
		return true;
	case 73: // num ctrl points
#ifdef DIME_FIXBIG
    this->numControlPoints = param.int32_data;
#else
		this->numControlPoints = param.int16_data;
#endif
		return true;
	case 74: // num fit points
#ifdef DIME_FIXBIG
    this->numFitPoints = param.int32_data;
#else
		this->numFitPoints = param.int16_data;
#endif
		return true;
	}
	return DimeEntity::handleRecord(groupcode, param);
}

//!

const char*
DimeSpline::getEntityName() const
{
	return entityName;
}

//!

bool
DimeSpline::getRecord(const int groupcode,
                      dimeParam& param,
                      const int index) const
{
	switch (groupcode)
	{
	case 10: // control points
	case 20:
	case 30:
		if (this->controlPoints != nullptr && index < this->numControlPoints)
		{
			param.double_data = this->controlPoints[index][groupcode / 10 - 1];
		}
		return true;
	case 11: // fit points
	case 21:
	case 31:
		if (this->fitPoints != nullptr && index < this->numFitPoints)
		{
			param.double_data = this->fitPoints[index][groupcode / 10 - 1];
		}
		return true;
	case 40: // knot value, multiple values
		if (this->knots != nullptr && index < this->numKnots)
		{
			param.double_data = this->knots[index];
		}
		return true;
	case 41: // weight, multiple values, present if not all 1
		if (this->weights != nullptr && this->numControlPoints)
		{
			param.double_data = this->weights[index];
		}
		else param.double_data = 1.0; // default
		return true;
	case 42: // knot tol
		param.double_data = this->knotTolerance;
		return true;
	case 43: // cp tol
		param.double_data = this->cpTolerance;
		return true;
	case 44: // fit tol
		param.double_data = this->fitTolerance;
		return true;
	case 70: // flags
		param.int16_data = this->flags;
		return true;
	case 71: // degree
#ifdef DIME_FIXBIG
    param.int32_data = this->degree;
#else
		param.int16_data = this->degree;
#endif
		return true;
	case 72: // num knots
#ifdef DIME_FIXBIG
    param.int32_data = this->numKnots;
#else
		param.int16_data = this->numKnots;
#endif
		return true;
	case 73: // num ctrl points
#ifdef DIME_FIXBIG
    param.int32_data = this->numControlPoints;
#else
		param.int32_data = this->numControlPoints;
#endif
		return true;
	case 74: // num fit points
#ifdef DIME_FIXBIG
    param.int32_data = this->numFitPoints;
#else
		param.int16_data = this->numFitPoints;
#endif
		return true;
	}
	return DimeEntity::getRecord(groupcode, param, index);
}


int
DimeSpline::countRecords() const
{
	int cnt = 7; // name + flags + degree + numKnots + numCP + knotTol + cpTol
	cnt += this->numControlPoints * 3;
	if (this->hasWeights()) cnt += this->numControlPoints;
	cnt += this->numFitPoints * 3;
	if (this->numFitPoints) cnt++; // fitTolerance
	cnt += this->numKnots;

	return cnt + DimeEntity::countRecords();
}

void
DimeSpline::setKnotValues(const dxfdouble* const values, const int numvalues)
{
	if (this->numKnots != numvalues)
	{
		delete [] this->knots;
		this->knots = NULL;
	}
	if (this->knots == nullptr || numvalues > this->numKnots)
	{
		this->knots = ARRAY_NEW(dxfdouble, numvalues);
	}
	memcpy(this->knots, values, numvalues * sizeof(dxfdouble));
	this->numKnots = numvalues;
}

/*!
  Sets new control points for this spline. It is the user's 
  responsibility to update the weights whenever the control
  point are changed.
*/

void
DimeSpline::setControlPoints(const dimeVec3* const pts, const int numpts)
{
	// update weights array (if present)
	if (this->hasWeights())
	{
		if (this->numControlPoints != numpts)
		{
			dxfdouble* newweights = ARRAY_NEW(dxfdouble, numpts);
			memcpy(newweights, this->weights,
			       sizeof(dxfdouble) * DXFMIN(numpts, this->numControlPoints));
			for (int i = this->numControlPoints; i < numpts; i++)
			{
				newweights[i] = 1.0;
			}
			delete [] this->weights;
			this->weights = newweights;
		}
		else if (numpts > this->numControlPoints)
		{
			dxfdouble* newweights = ARRAY_NEW(dxfdouble, numpts);
			memcpy(newweights, this->weights, this->numControlPoints);
			for (int i = this->numControlPoints; i < numpts; i++)
			{
				newweights[i] = 1.0;
			}
			this->weights = newweights;
		}
	}
	if (this->numControlPoints != numpts)
	{
		delete [] this->controlPoints;
		delete [] this->weights;
		this->controlPoints = NULL;
		this->weights = NULL;
	}
	if (this->controlPoints == nullptr || numpts > this->numControlPoints)
	{
		this->controlPoints = ARRAY_NEW(dimeVec3, numpts);
	}
	memcpy(this->controlPoints, pts, sizeof(dimeVec3) * numpts);
	this->numControlPoints = numpts;
}

/*!
  Sets the weight of control point \a idx.
*/

void
DimeSpline::setWeight(const int idx, const dxfdouble w)
{
	if (!this->hasWeights() && w != 1.0)
	{
		this->weights = ARRAY_NEW(dxfdouble,
		                          this->numControlPoints);
		for (int i = 0; i < this->numControlPoints; i++)
		{
			this->weights[i] = 1.0;
		}
	}
	assert(idx >= 0 && idx < this->numControlPoints);
	if (w != 1.0)
	{
		this->weights[idx] = w;
	}
}

void
DimeSpline::setFitPoints(const dimeVec3* const pts, const int numpts)
{
	if (this->numFitPoints != numpts)
	{
		delete [] this->fitPoints;
		this->fitPoints = NULL;
	}
	if (this->fitPoints == nullptr || numpts > this->numFitPoints)
	{
		this->fitPoints = ARRAY_NEW(dimeVec3, numpts);
	}
	memcpy(this->fitPoints, pts, numpts * sizeof(dimeVec3));
	this->numFitPoints = numpts;
}
