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

#ifndef DIME_LINEAR_H
#define DIME_LINEAR_H

#include <dime/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


class DIME_DLL_API dimeVec2
{
public:
	dimeVec2() = default;

	dimeVec2(const dimeVec2& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	dimeVec2(dxfdouble _x, dxfdouble _y)
	{
		x = _x;
		y = _y;
	}


	dxfdouble x, y;
};

class DIME_DLL_API dimeVec3
{
public:

	dimeVec3() = default;

	dimeVec3(const dxfdouble X, const dxfdouble Y, const dxfdouble Z)
	{
		x = X;
		y = Y;
		z = Z;
	};

	dimeVec3(const dxfdouble* xyz)
	{
		x = xyz[0];
		y = xyz[1];
		z = xyz[2];
	}

	dimeVec3(const dimeVec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	};

	dimeVec3 cross(const dimeVec3& v) const
	{
		return dimeVec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	dxfdouble dot(const dimeVec3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	bool equals(const dimeVec3& v)
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool equals(const dimeVec3& v, dxfdouble tol)
	{
		return (fabs(x - v.x) <= tol && fabs(y - v.y) <= tol && fabs(z - v.z) <= tol);
	}

	operator dxfdouble*() { return &x; }
	const dxfdouble* getValue() const { return &x; }

	void getValue(dxfdouble& _x, dxfdouble& _y, dxfdouble& _z) const
	{
		_x = x;
		_y = y;
		_z = z;
	}

	dxfdouble length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	dxfdouble sqrLength(void) const
	{
		return x * x + y * y + z * z;
	}

	void negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void setValue(const dxfdouble* v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	void setValue(const dxfdouble X, const dxfdouble Y, const dxfdouble Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	dxfdouble operator [](const int i) const
	{
		return ((i == 0) ? x : ((i == 1) ? y : z));
	};

	dxfdouble& operator [](const int i)
	{
		return ((i == 0) ? x : ((i == 1) ? y : z));
	};

	dimeVec3& operator *=(const dxfdouble s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	dimeVec3& operator /=(const dxfdouble s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	dimeVec3& operator +=(const dimeVec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	dimeVec3& operator -=(const dimeVec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	dimeVec3 operator -() const
	{
		return dimeVec3(-x, -y, -z);
	}

	friend dimeVec3 operator *(const dimeVec3& v, dxfdouble s)
	{
		return dimeVec3(v.x * s, v.y * s, v.z * s);
	}

	friend dimeVec3 operator *(dxfdouble s, const dimeVec3& v)
	{
		return dimeVec3(v.x * s, v.y * s, v.z * s);
	}

	friend dimeVec3 operator /(const dimeVec3& v, dxfdouble s)
	{
		return dimeVec3(v.x / s, v.y / s, v.z / s);
	}

	friend dimeVec3 operator +(const dimeVec3& v1, const dimeVec3& v2)
	{
		return dimeVec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	friend dimeVec3 operator -(const dimeVec3& v1, const dimeVec3& v2)
	{
		return dimeVec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	friend bool operator ==(const dimeVec3& v1, const dimeVec3& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
	}

	friend bool operator !=(const dimeVec3& v1, const dimeVec3& v2)
	{
		return (v1.x != v2.x || v1.y != v2.y || v1.z != v2.z);
	}

	dimeVec3& operator =(const dimeVec3& v) // extra
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	void multMatrix(dxfdouble* matrix) // extra
	{
		dxfdouble tx, ty, tz;
		tx = ty = tz = 0.0f;
		tx = matrix[0] * x + matrix[1] * y + matrix[2] * z;
		ty = matrix[4] * x + matrix[5] * y + matrix[6] * z;
		tz = matrix[8] * x + matrix[9] * y + matrix[10] * z;
		x = tx, y = ty, z = tz;
	}

	dimeVec3 multComponents(const dimeVec3& v) const
	{
		return dimeVec3(x * v.x, y * v.y, z * v.z);
	}

	dxfdouble angle(const dimeVec3& v2);
	void normalize();

	dxfdouble x, y, z;
}; // class dimeVec3f


class DIME_DLL_API dimeMatrix
{
public:
	dimeMatrix() = default;

	dimeMatrix(const dimeMatrix& matrix);
	// Constructor given all 16 elements in row-major order
	dimeMatrix(dxfdouble a11, dxfdouble a12, dxfdouble a13, dxfdouble a14,
	           dxfdouble a21, dxfdouble a22, dxfdouble a23, dxfdouble a24,
	           dxfdouble a31, dxfdouble a32, dxfdouble a33, dxfdouble a34,
	           dxfdouble a41, dxfdouble a42, dxfdouble a43, dxfdouble a44);
	void transpose();
	void makeIdentity();
	bool isIdentity() const;
	void setTransform(const dimeVec3& translation,
	                  const dimeVec3& scalefactor,
	                  const dimeVec3& rotAngles);
	dimeMatrix& multRight(const dimeMatrix& m); // this = this * m
	dimeMatrix& multLeft(const dimeMatrix& m); // this = m * this

	// Sets matrix to rotate by given rotation
	void setRotate(const dimeVec3& rot);

	void setRotate(const dimeVec3& x, const dimeVec3& y, const dimeVec3& z);

	// sets matrix to rotate around given vector
	void setRotation(const dimeVec3& u, dxfdouble angle);

	// Sets matrix to scale by given uniform factor
	void setScale(dxfdouble s);

	// Sets matrix to scale by given vector
	void setScale(const dimeVec3& s);

	// Sets matrix to translate by given vector
	void setTranslate(const dimeVec3& t);

	// Multiplies matrix by given column vector, giving vector result
	void multMatrixVec(const dimeVec3& src, dimeVec3& dst) const;

	// transforms vector
	void multMatrixVec(dimeVec3& vec) const;

	// Multiplies given row vector by matrix, giving vector result
	//void multVecMatrix(const dimeVec3f &src, dimeVec3f &dst) const;

	// Cast: returns pointer to storage of first element
	operator dxfdouble*() { return &matrix[0][0]; }

	// Make it look like a usual matrix (so you can do m[3][2])
	dxfdouble* operator [](int i) { return &matrix[i][0]; }
	const dxfdouble* operator [](int i) const { return &matrix[i][0]; }

	dimeMatrix& operator =(const dimeMatrix& m);

	// Performs right multiplication with another matrix
	dimeMatrix& operator *=(const dimeMatrix& m) { return multRight(m); }


	static dimeMatrix identity();
	bool inverse();
	bool inverse2();
	dxfdouble determinant(int i = -2, int j = -1);

	void operator *=(dxfdouble val);

private:
	dxfdouble matrix[4][4];
}; // class dimeMatrix

#endif // ! DIME_LINEAR_H
