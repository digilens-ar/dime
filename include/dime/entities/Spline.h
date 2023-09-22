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

#ifndef DIME_SPLINE_H
#define DIME_SPLINE_H

#include <dime/entities/ExtrusionEntity.h>
#include <assert.h>

class  DimeSpline : public DimeEntity
{
public:
	DimeSpline();
	~DimeSpline() override;

	enum Flags
	{
		CLOSED = 0x01,
		PERIODIC = 0x02,
		RATIONAL = 0x04,
		PLANAR = 0x08,
		LINEAR = 0x10
	};

	bool hasWeights() const;

	int16_t getFlags() const;
	void setFlags(int16_t flags);

	int16_t getDegree() const;
	void setDegree(int16_t degree);

	dxfdouble getControlPointTolerance() const;
	void setControlPointTolerance(dxfdouble tol);
	dxfdouble getFitPointTolerance() const;
	void setFitPointTolerance(dxfdouble tol);
	dxfdouble getKnotTolerance() const;
	void setKnotTolerance(dxfdouble tol);

	int getNumKnots() const;
	dxfdouble getKnotValue(int idx) const;
	void setKnotValue(int idx, dxfdouble value);
	void setKnotValues(const dxfdouble* values, int numvalues);

	int getNumControlPoints() const;
	const dimeVec3& getControlPoint(int idx) const;
	void setControlPoint(int idx, const dimeVec3& v);
	void setControlPoints(const dimeVec3* pts, int numpts);

	int getNumWeights() const;
	dxfdouble getWeight(int idx) const;
	void setWeight(int idx, dxfdouble w);

	int getNumFitPoints() const;
	const dimeVec3& getFitPoint(int idx) const;
	void setFitPoint(int idx, const dimeVec3& pt);
	void setFitPoints(const dimeVec3* pts, int numpts);

	DimeEntity* copy(DimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index) const override;
	const char* getEntityName() const override;

	bool write(DimeOutput* out) override;
	TypeID typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param) override;

private:
	int16_t flags;
#ifdef DIME_FIXBIG
  int32_t degree;
  int32_t numKnots;
  int32_t numControlPoints;
  int32_t numFitPoints;
#else
	int16_t degree;
	int16_t numKnots;
	int16_t numControlPoints;
	int16_t numFitPoints;
#endif
	dxfdouble knotTolerance;
	dxfdouble fitTolerance;
	dxfdouble cpTolerance;
	dxfdouble* knots;
	dxfdouble* weights;
	dimeVec3* controlPoints;
	dimeVec3* fitPoints;

	// read/handle counters
	int16_t knotCnt;
	int16_t fitCnt;
	int16_t cpCnt;
	int16_t weightCnt;
}; // class dimeSpline

inline int16_t
DimeSpline::getFlags() const
{
	return this->flags;
}

inline void
DimeSpline::setFlags(const int16_t flags)
{
	this->flags = flags;
}

inline int16_t
DimeSpline::getDegree() const
{
	return this->degree;
}

inline void
DimeSpline::setDegree(const int16_t degree)
{
	this->degree = degree;
}

inline dxfdouble
DimeSpline::getControlPointTolerance() const
{
	return this->cpTolerance;
}

inline void
DimeSpline::setControlPointTolerance(const dxfdouble tol)
{
	this->cpTolerance = tol;
}

inline dxfdouble
DimeSpline::getFitPointTolerance() const
{
	return this->fitTolerance;
}

inline void
DimeSpline::setFitPointTolerance(const dxfdouble tol)
{
	this->fitTolerance = tol;
}

inline dxfdouble
DimeSpline::getKnotTolerance() const
{
	return this->knotTolerance;
}

inline void
DimeSpline::setKnotTolerance(const dxfdouble tol)
{
	this->knotTolerance = tol;
}

inline int
DimeSpline::getNumKnots() const
{
	return this->numKnots;
}

inline dxfdouble
DimeSpline::getKnotValue(const int idx) const
{
	assert(idx >= 0 && idx < this->numKnots);
	return this->knots[idx];
}

inline void
DimeSpline::setKnotValue(const int idx, const dxfdouble value)
{
	assert(idx >= 0 && idx < this->numKnots);
	this->knots[idx] = value;
}

inline int
DimeSpline::getNumControlPoints() const
{
	return this->numControlPoints;
}

inline const dimeVec3&
DimeSpline::getControlPoint(const int idx) const
{
	assert(idx >= 0 && idx < this->numControlPoints);
	return this->controlPoints[idx];
}

inline void
DimeSpline::setControlPoint(const int idx, const dimeVec3& v)
{
	assert(idx >= 0 && idx < this->numControlPoints);
	this->controlPoints[idx] = v;
}

inline int
DimeSpline::getNumWeights() const
{
	return this->getNumControlPoints();
}

inline dxfdouble
DimeSpline::getWeight(const int idx) const
{
	if (this->hasWeights())
	{
		assert(idx >= 0 && idx < this->numControlPoints);
		return this->weights[idx];
	}
	return 1.0;
}

inline int
DimeSpline::getNumFitPoints() const
{
	return this->numFitPoints;
}

inline const dimeVec3&
DimeSpline::getFitPoint(const int idx) const
{
	assert(idx >= 0 && idx < this->numFitPoints);
	return this->fitPoints[idx];
}

inline void
DimeSpline::setFitPoint(const int idx, const dimeVec3& pt)
{
	assert(idx >= 0 && idx < this->numFitPoints);
	this->fitPoints[idx] = pt;
}

#endif // ! DIME_SPLINE_H
