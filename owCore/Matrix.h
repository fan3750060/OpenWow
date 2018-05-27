#pragma once

#include "MathCommon.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Quaternion.h"

class Matrix4f
{
public:
	union
	{
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};

	// --------------
	// Static methods
	// --------------
	static Matrix4f TransMat(float x, float y, float z)
	{
		Matrix4f m;

		m.c[3][0] = x;
		m.c[3][1] = y;
		m.c[3][2] = z;

		return m;
	}

	static Matrix4f TransMat(const Vec3f& _v)
	{
		return TransMat(_v.x, _v.y, _v.z);
	}

	//

	static Matrix4f ScaleMat(float x, float y, float z)
	{
		Matrix4f m;

		m.c[0][0] = x;
		m.c[1][1] = y;
		m.c[2][2] = z;

		return m;
	}

	static Matrix4f ScaleMat(const Vec3f& _v)
	{
		return ScaleMat(_v.x, _v.y, _v.z);
	}

	//

	static Matrix4f RotMat(float x, float y, float z)
	{
		// Rotation order: YXZ [* Vector]
		return Matrix4f(Quaternion(x, y, z));
	}

	static Matrix4f RotMat(const Vec3f& _v)
	{
		return RotMat(_v.x, _v.y, _v.z);
	}

	static Matrix4f RotMat(Vec3f axis, float angle)
	{
		axis = axis * sinf(angle * 0.5f);
		return Matrix4f(Quaternion(axis.x, axis.y, axis.z, cosf(angle * 0.5f)));
	}

	static Matrix4f RotMat(const Quaternion& _q)
	{
		return Matrix4f(Quaternion(_q.x, _q.y, _q.z, _q.w));
	}

	//

	static Matrix4f PerspectiveMat(float l, float r, float b, float t, float n, float f)
	{
		Matrix4f m;

		m.x[0] = 2 * n / (r - l);
		m.x[5] = 2 * n / (t - b);
		m.x[8] = (r + l) / (r - l);
		m.x[9] = (t + b) / (t - b);
		m.x[10] = -(f + n) / (f - n);
		m.x[11] = -1;
		m.x[14] = -2 * f * n / (f - n);
		m.x[15] = 0;

		return m;
	}

	static Matrix4f OrthoMat(float l, float r, float b, float t, float n, float f)
	{
		Matrix4f m;

		m.x[0] = 2 / (r - l);
		m.x[5] = 2 / (t - b);
		m.x[10] = -2 / (f - n);
		m.x[12] = -(r + l) / (r - l);
		m.x[13] = -(t + b) / (t - b);
		m.x[14] = -(f + n) / (f - n);

		return m;
	}

	static Matrix4f lookAtRH(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
	{
		const Vec3f f = (center - eye).normalized();
		const Vec3f s = (f.cross(up)).normalized();
		const Vec3f u = s.cross(f);

		Matrix4f Result;
		Result.c[0][0] = s.x;
		Result.c[1][0] = s.y;
		Result.c[2][0] = s.z;

		Result.c[0][1] = u.x;
		Result.c[1][1] = u.y;
		Result.c[2][1] = u.z;

		Result.c[0][2] = -f.x;
		Result.c[1][2] = -f.y;
		Result.c[2][2] = -f.z;

		Result.c[3][0] = -(s.dot(eye));
		Result.c[3][1] = -(u.dot(eye));
		Result.c[3][2] = f.dot(eye);

		return Result;
	}

	static void fastMult43(Matrix4f &dst, const Matrix4f &m1, const Matrix4f &m2)
	{
		// Note: dst may not be the same as m1 or m2

		float *dstx = dst.x;
		const float *m1x = m1.x;
		const float *m2x = m2.x;

		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}


	// ------------
	// Constructors
	// ------------
	Matrix4f()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	}

	explicit Matrix4f(Math::NoInitHint)
	{
		// Constructor without default initialization
	}

	Matrix4f(const float* floatArray16)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				c[i][j] = floatArray16[i * 4 + j];
			}
		}
	}

	Matrix4f(const Quaternion &q)
	{
		// Calculate coefficients
		float x2 = q.x + q.x;
		float y2 = q.y + q.y;
		float z2 = q.z + q.z;

		float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
		float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
		float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);
		c[1][0] = xy - wz;
		c[2][0] = xz + wy;
		c[3][0] = 0;

		c[0][1] = xy + wz;
		c[1][1] = 1 - (xx + zz);
		c[2][1] = yz - wx;
		c[3][1] = 0;

		c[0][2] = xz - wy;
		c[1][2] = yz + wx;
		c[2][2] = 1 - (xx + yy);
		c[3][2] = 0;

		c[0][3] = 0;
		c[1][3] = 0;
		c[2][3] = 0;
		c[3][3] = 1;
	}

	// ----------
	// mat4 sum
	// ----------
	Matrix4f operator+(const Matrix4f &m) const
	{
		Matrix4f mf(Math::NO_INIT);

		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];
		mf.x[9] = x[9] + m.x[9];
		mf.x[10] = x[10] + m.x[10];
		mf.x[11] = x[11] + m.x[11];
		mf.x[12] = x[12] + m.x[12];
		mf.x[13] = x[13] + m.x[13];
		mf.x[14] = x[14] + m.x[14];
		mf.x[15] = x[15] + m.x[15];

		return mf;
	}

	Matrix4f& operator+=(const Matrix4f &m)
	{
		return *this = *this + m;
	}

	// ---------------------
	// mat4 multiplication
	// ---------------------
	Matrix4f operator*(const Matrix4f &m) const
	{
		Matrix4f mf(Math::NO_INIT);

		mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
		mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
		mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
		mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

		mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
		mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
		mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
		mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

		mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
		mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
		mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
		mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

		mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
		mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
		mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
		mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

		return mf;
	}

	Matrix4f& operator*= (const Matrix4f& p)
	{
		return *this = this->operator*(p);
	}

	Matrix4f operator*(const float f) const
	{
		Matrix4f m(*this);

		m.x[0] *= f; m.x[1] *= f; m.x[2] *= f; m.x[3] *= f;
		m.x[4] *= f; m.x[5] *= f; m.x[6] *= f; m.x[7] *= f;
		m.x[8] *= f; m.x[9] *= f; m.x[10] *= f; m.x[11] *= f;
		m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

		return m;
	}



	// ----------------------------
	// Vector-mat4 multiplication
	// ----------------------------
	Vec3f operator*(const Vec3f &v) const
	{
		return Vec3f(v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
			v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
			v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2]);
	}

	Vec4f operator*(const Vec4f &v) const
	{
		return Vec4f(v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
			v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
			v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
			v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3]);
	}

	Vec3f mult33Vec(const Vec3f &v) const
	{
		return Vec3f(v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
			v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
			v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2]);
	}



	// ---------------
	// Transformations
	// ---------------
	void translate(const float x, const float y, const float z)
	{
		*this = *this * TransMat(x, y, z);
	}

	void translate(const Vec3f& _v)
	{
		*this = *this * TransMat(_v.x, _v.y, _v.z);
	}

	//

	void rotate(const float x, const float y, const float z)
	{
		*this = *this * RotMat(x, y, z);
	}

	void rotate(const Vec3f& _v)
	{
		*this = *this * RotMat(_v.x, _v.y, _v.z);
	}

	void rotate(Vec3f axis, float angle)
	{
		*this = *this * RotMat(axis, angle);
	}

	void rotate(const Quaternion& _q)
	{
		*this = *this * RotMat(_q);
	}
	//

	void scale(const float x, const float y, const float z)
	{
		*this = *this * ScaleMat(x, y, z);
	}

	void scale(const Vec3f& _v)
	{
		*this = *this * ScaleMat(_v.x, _v.y, _v.z);
	}

	void scale(const float v)
	{
		*this = *this * ScaleMat(v, v, v);
	}



	// ---------------
	// Other
	// ---------------

	Matrix4f transposed() const
	{
		Matrix4f m(*this);

		for (unsigned int y = 0; y < 4; ++y)
		{
			for (unsigned int x = y + 1; x < 4; ++x)
			{
				float tmp = m.c[x][y];
				m.c[x][y] = m.c[y][x];
				m.c[y][x] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return
			c[0][3] * c[1][2] * c[2][1] * c[3][0] - c[0][2] * c[1][3] * c[2][1] * c[3][0] - c[0][3] * c[1][1] * c[2][2] * c[3][0] + c[0][1] * c[1][3] * c[2][2] * c[3][0] +
			c[0][2] * c[1][1] * c[2][3] * c[3][0] - c[0][1] * c[1][2] * c[2][3] * c[3][0] - c[0][3] * c[1][2] * c[2][0] * c[3][1] + c[0][2] * c[1][3] * c[2][0] * c[3][1] +
			c[0][3] * c[1][0] * c[2][2] * c[3][1] - c[0][0] * c[1][3] * c[2][2] * c[3][1] - c[0][2] * c[1][0] * c[2][3] * c[3][1] + c[0][0] * c[1][2] * c[2][3] * c[3][1] +
			c[0][3] * c[1][1] * c[2][0] * c[3][2] - c[0][1] * c[1][3] * c[2][0] * c[3][2] - c[0][3] * c[1][0] * c[2][1] * c[3][2] + c[0][0] * c[1][3] * c[2][1] * c[3][2] +
			c[0][1] * c[1][0] * c[2][3] * c[3][2] - c[0][0] * c[1][1] * c[2][3] * c[3][2] - c[0][2] * c[1][1] * c[2][0] * c[3][3] + c[0][1] * c[1][2] * c[2][0] * c[3][3] +
			c[0][2] * c[1][0] * c[2][1] * c[3][3] - c[0][0] * c[1][2] * c[2][1] * c[3][3] - c[0][1] * c[1][0] * c[2][2] * c[3][3] + c[0][0] * c[1][1] * c[2][2] * c[3][3];
	}

	Matrix4f inverted() const
	{
		Matrix4f m(Math::NO_INIT);

		float d = determinant();
		if (d == 0) return m;
		d = 1.0f / d;

		m.c[0][0] = d * (c[1][2] * c[2][3] * c[3][1] - c[1][3] * c[2][2] * c[3][1] + c[1][3] * c[2][1] * c[3][2] - c[1][1] * c[2][3] * c[3][2] - c[1][2] * c[2][1] * c[3][3] + c[1][1] * c[2][2] * c[3][3]);
		m.c[0][1] = d * (c[0][3] * c[2][2] * c[3][1] - c[0][2] * c[2][3] * c[3][1] - c[0][3] * c[2][1] * c[3][2] + c[0][1] * c[2][3] * c[3][2] + c[0][2] * c[2][1] * c[3][3] - c[0][1] * c[2][2] * c[3][3]);
		m.c[0][2] = d * (c[0][2] * c[1][3] * c[3][1] - c[0][3] * c[1][2] * c[3][1] + c[0][3] * c[1][1] * c[3][2] - c[0][1] * c[1][3] * c[3][2] - c[0][2] * c[1][1] * c[3][3] + c[0][1] * c[1][2] * c[3][3]);
		m.c[0][3] = d * (c[0][3] * c[1][2] * c[2][1] - c[0][2] * c[1][3] * c[2][1] - c[0][3] * c[1][1] * c[2][2] + c[0][1] * c[1][3] * c[2][2] + c[0][2] * c[1][1] * c[2][3] - c[0][1] * c[1][2] * c[2][3]);
		m.c[1][0] = d * (c[1][3] * c[2][2] * c[3][0] - c[1][2] * c[2][3] * c[3][0] - c[1][3] * c[2][0] * c[3][2] + c[1][0] * c[2][3] * c[3][2] + c[1][2] * c[2][0] * c[3][3] - c[1][0] * c[2][2] * c[3][3]);
		m.c[1][1] = d * (c[0][2] * c[2][3] * c[3][0] - c[0][3] * c[2][2] * c[3][0] + c[0][3] * c[2][0] * c[3][2] - c[0][0] * c[2][3] * c[3][2] - c[0][2] * c[2][0] * c[3][3] + c[0][0] * c[2][2] * c[3][3]);
		m.c[1][2] = d * (c[0][3] * c[1][2] * c[3][0] - c[0][2] * c[1][3] * c[3][0] - c[0][3] * c[1][0] * c[3][2] + c[0][0] * c[1][3] * c[3][2] + c[0][2] * c[1][0] * c[3][3] - c[0][0] * c[1][2] * c[3][3]);
		m.c[1][3] = d * (c[0][2] * c[1][3] * c[2][0] - c[0][3] * c[1][2] * c[2][0] + c[0][3] * c[1][0] * c[2][2] - c[0][0] * c[1][3] * c[2][2] - c[0][2] * c[1][0] * c[2][3] + c[0][0] * c[1][2] * c[2][3]);
		m.c[2][0] = d * (c[1][1] * c[2][3] * c[3][0] - c[1][3] * c[2][1] * c[3][0] + c[1][3] * c[2][0] * c[3][1] - c[1][0] * c[2][3] * c[3][1] - c[1][1] * c[2][0] * c[3][3] + c[1][0] * c[2][1] * c[3][3]);
		m.c[2][1] = d * (c[0][3] * c[2][1] * c[3][0] - c[0][1] * c[2][3] * c[3][0] - c[0][3] * c[2][0] * c[3][1] + c[0][0] * c[2][3] * c[3][1] + c[0][1] * c[2][0] * c[3][3] - c[0][0] * c[2][1] * c[3][3]);
		m.c[2][2] = d * (c[0][1] * c[1][3] * c[3][0] - c[0][3] * c[1][1] * c[3][0] + c[0][3] * c[1][0] * c[3][1] - c[0][0] * c[1][3] * c[3][1] - c[0][1] * c[1][0] * c[3][3] + c[0][0] * c[1][1] * c[3][3]);
		m.c[2][3] = d * (c[0][3] * c[1][1] * c[2][0] - c[0][1] * c[1][3] * c[2][0] - c[0][3] * c[1][0] * c[2][1] + c[0][0] * c[1][3] * c[2][1] + c[0][1] * c[1][0] * c[2][3] - c[0][0] * c[1][1] * c[2][3]);
		m.c[3][0] = d * (c[1][2] * c[2][1] * c[3][0] - c[1][1] * c[2][2] * c[3][0] - c[1][2] * c[2][0] * c[3][1] + c[1][0] * c[2][2] * c[3][1] + c[1][1] * c[2][0] * c[3][2] - c[1][0] * c[2][1] * c[3][2]);
		m.c[3][1] = d * (c[0][1] * c[2][2] * c[3][0] - c[0][2] * c[2][1] * c[3][0] + c[0][2] * c[2][0] * c[3][1] - c[0][0] * c[2][2] * c[3][1] - c[0][1] * c[2][0] * c[3][2] + c[0][0] * c[2][1] * c[3][2]);
		m.c[3][2] = d * (c[0][2] * c[1][1] * c[3][0] - c[0][1] * c[1][2] * c[3][0] - c[0][2] * c[1][0] * c[3][1] + c[0][0] * c[1][2] * c[3][1] + c[0][1] * c[1][0] * c[3][2] - c[0][0] * c[1][1] * c[3][2]);
		m.c[3][3] = d * (c[0][1] * c[1][2] * c[2][0] - c[0][2] * c[1][1] * c[2][0] + c[0][2] * c[1][0] * c[2][1] - c[0][0] * c[1][2] * c[2][1] - c[0][1] * c[1][0] * c[2][2] + c[0][0] * c[1][1] * c[2][2]);

		return m;
	}

	void decompose(Vec3f &trans, Vec3f &rot, Vec3f &scale) const
	{
		// Getting translation is trivial
		trans = Vec3f(c[3][0], c[3][1], c[3][2]);

		// Scale is length of columns
		scale.x = sqrtf(c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2]);
		scale.y = sqrtf(c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2]);
		scale.z = sqrtf(c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2]);

		if (scale.x == 0 || scale.y == 0 || scale.z == 0) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if (determinant() < 0) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf(-c[2][1] / scale.z);

		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf(c[2][1] / scale.z);
		if (f > 0.999f && f < 1.001f)
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;

			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f(-c[1][0] / scale.y, c[0][0] / scale.x);
		}
		// Standard case
		else
		{
			rot.y = atan2f(c[2][0] / scale.z, c[2][2] / scale.z);
			rot.z = atan2f(c[0][1] / scale.x, c[1][1] / scale.y);
		}
	}


	void setCol(unsigned int col, const Vec4f& v)
	{
		x[col * 4 + 0] = v.x;
		x[col * 4 + 1] = v.y;
		x[col * 4 + 2] = v.z;
		x[col * 4 + 3] = v.w;
	}

	Vec4f getCol(unsigned int col) const
	{
		return Vec4f(x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3]);
	}

	Vec4f getRow(unsigned int row) const
	{
		return Vec4f(x[row + 0], x[row + 4], x[row + 8], x[row + 12]);
	}

	Vec3f getTrans() const
	{
		return Vec3f(c[3][0], c[3][1], c[3][2]);
	}

	Vec3f getScale() const
	{
		Vec3f scale;
		// Scale is length of columns
		scale.x = sqrtf(c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2]);
		scale.y = sqrtf(c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2]);
		scale.z = sqrtf(c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2]);
		return scale;
	}

	void unit()
	{
		for (uint8 j = 0; j < 4; j++)
		{
			for (uint8 i = 0; i < 4; i++)
			{
				c[j][i] = 0;
			}
		}

		c[0][0] = c[1][1] = c[2][2] = c[3][3] = 1.0f;
	}

	//

	operator float*()
	{
		return (float*)this;
	}
};