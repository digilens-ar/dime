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

class DIME_DLL_API dimeSpline : public dimeEntity
{
public:
	dimeSpline();
	~dimeSpline() override;

	enum Flags
	{
		CLOSED = 0x01,
		PERIODIC = 0x02,
		RATIONAL = 0x04,
		PLANAR = 0x08,
		LINEAR = 0x10
	};

	bool hasWeights() const;

	int16 getFlags() const;
	void setFlags(int16 flags);

	int16 getDegree() const;
	void setDegree(int16 degree);

	dxfdouble getControlPointTolerance() const;
	void setControlPointTolerance(dxfdouble tol);
	dxfdouble getFitPointTolerance() const;
	void setFitPointTolerance(dxfdouble tol);
	dxfdouble getKnotTolerance() const;
	void setKnotTolerance(dxfdouble tol);

	int getNumKnots() const;
	dxfdouble getKnotValue(int idx) const;
	void setKnotValue(int idx, dxfdouble value);
	void setKnotValues(const dxfdouble* values, int numvalues,
	                   DimeMemHandler* memhandler = nullptr);

	int getNumControlPoints() const;
	const dimeVec3f& getControlPoint(int idx) const;
	void setControlPoint(int idx, const dimeVec3f& v);
	void setControlPoints(const dimeVec3f* pts, int numpts,
	                      DimeMemHandler* memhandler = nullptr);

	int getNumWeights() const;
	dxfdouble getWeight(int idx) const;
	void setWeight(int idx, dxfdouble w,
	               DimeMemHandler* memhandler = nullptr);

	int getNumFitPoints() const;
	const dimeVec3f& getFitPoint(int idx) const;
	void setFitPoint(int idx, const dimeVec3f& pt);
	void setFitPoints(const dimeVec3f* pts, int numpts,
	                  DimeMemHandler* memhandler = nullptr);

	dimeEntity* copy(dimeModel* model) const override;
	bool getRecord(int groupcode,
	               dimeParam& param,
	               int index) const override;
	const char* getEntityName() const override;

	void print() const override;
	bool write(dimeOutput* out) override;
	int typeId() const override;
	int countRecords() const override;

protected:
	bool handleRecord(int groupcode,
	                  const dimeParam& param,
	                  DimeMemHandler* memhandler) override;

private:
	int16 flags;
#ifdef DIME_FIXBIG
  int32 degree;
  int32 numKnots;
  int32 numControlPoints;
  int32 numFitPoints;
#else
	int16 degree;
	int16 numKnots;
	int16 numControlPoints;
	int16 numFitPoints;
#endif
	dxfdouble knotTolerance;
	dxfdouble fitTolerance;
	dxfdouble cpTolerance;
	dxfdouble* knots;
	dxfdouble* weights;
	dimeVec3f* controlPoints;
	dimeVec3f* fitPoints;

	// read/handle counters
	int16 knotCnt;
	int16 fitCnt;
	int16 cpCnt;
	int16 weightCnt;
}; // class dimeSpline

inline int16
dimeSpline::getFlags() const
{
	return this->flags;
}

inline void
dimeSpline::setFlags(const int16 flags)
{
	this->flags = flags;
}

inline int16
dimeSpline::getDegree() const
{
	return this->degree;
}

inline void
dimeSpline::setDegree(const int16 degree)
{
	this->degree = degree;
}

inline dxfdouble
dimeSpline::getControlPointTolerance() const
{
	return this->cpTolerance;
}

inline void
dimeSpline::setControlPointTolerance(const dxfdouble tol)
{
	this->cpTolerance = tol;
}

inline dxfdouble
dimeSpline::getFitPointTolerance() const
{
	return this->fitTolerance;
}

inline void
dimeSpline::setFitPointTolerance(const dxfdouble tol)
{
	this->fitTolerance = tol;
}

inline dxfdouble
dimeSpline::getKnotTolerance() const
{
	return this->knotTolerance;
}

inline void
dimeSpline::setKnotTolerance(const dxfdouble tol)
{
	this->knotTolerance = tol;
}

inline int
dimeSpline::getNumKnots() const
{
	return this->numKnots;
}

inline dxfdouble
dimeSpline::getKnotValue(const int idx) const
{
	assert(idx >= 0 && idx < this->numKnots);
	return this->knots[idx];
}

inline void
dimeSpline::setKnotValue(const int idx, const dxfdouble value)
{
	assert(idx >= 0 && idx < this->numKnots);
	this->knots[idx] = value;
}

inline int
dimeSpline::getNumControlPoints() const
{
	return this->numControlPoints;
}

inline const dimeVec3f&
dimeSpline::getControlPoint(const int idx) const
{
	assert(idx >= 0 && idx < this->numControlPoints);
	return this->controlPoints[idx];
}

inline void
dimeSpline::setControlPoint(const int idx, const dimeVec3f& v)
{
	assert(idx >= 0 && idx < this->numControlPoints);
	this->controlPoints[idx] = v;
}

inline int
dimeSpline::getNumWeights() const
{
	return this->getNumControlPoints();
}

inline dxfdouble
dimeSpline::getWeight(const int idx) const
{
	if (this->hasWeights())
	{
		assert(idx >= 0 && idx < this->numControlPoints);
		return this->weights[idx];
	}
	return 1.0;
}

inline int
dimeSpline::getNumFitPoints() const
{
	return this->numFitPoints;
}

inline const dimeVec3f&
dimeSpline::getFitPoint(const int idx) const
{
	assert(idx >= 0 && idx < this->numFitPoints);
	return this->fitPoints[idx];
}

inline void
dimeSpline::setFitPoint(const int idx, const dimeVec3f& pt)
{
	assert(idx >= 0 && idx < this->numFitPoints);
	this->fitPoints[idx] = pt;
}

#endif // ! DIME_SPLINE_H
