/**
 * Matrix3.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Matrix3
{
	vector data[3];

	Transform m_trans;

	static void Transpose(vector a[3], out vector b[3])
	{
		b[0][0] = a[0][0];
		b[0][1] = a[1][0];
		b[0][2] = a[1][0];
		b[1][0] = a[0][1];
		b[1][1] = a[1][1];
		b[1][2] = a[1][1];
		b[2][0] = a[0][2];
		b[2][1] = a[1][2];
		b[2][2] = a[1][2];
	}

	void Matrix3()
	{
		data = {"1 0 0", "0 1 0", "0 0 1"};
	}

	static Matrix3 SetRotationX(float angle)
	{
		Matrix3 mat = new Matrix3();

		float s = Math.Sin(angle);
		float c = Math.Cos(angle);

		mat.data[1][1] = c;
		mat.data[1][2] = -s;
		mat.data[2][1] = s;
		mat.data[2][2] = c;

		return mat;
	}

	void Set(int _index, vector _value)
	{
		data[_index] = _value;
	}

	void Set(int _index, string _value)
	{
		data[_index] = _value.ToVector();
	}

	vector Get(int _index)
	{
		return data[_index];
	}

	void Update()
	{
		if (m_trans)
			m_trans.UpdateBasis();
	}

	void ToQuaternion(out Quaternion quat)
	{
		if (quat == NULL)
			quat = new Quaternion;

		Math3D.MatrixToQuat(data, quat.data);

		quat.Update();
	}

	Matrix3 Invert()
	{
		Matrix3 n = new Matrix3;

		vector co = Vector(Cofactor(1, 1, 2, 2), Cofactor(1, 2, 2, 0), Cofactor(1, 0, 2, 1));

		float det = vector.Dot(data[0], co);
		if (det == 0)
		{
			return n;
		}

		float s = 1.0 / det;

		n.data[0][0] = co[0] * s;
		n.data[0][1] = Cofactor(0, 2, 2, 1) * s;
		n.data[0][2] = Cofactor(0, 1, 1, 2) * s;

		n.data[1][0] = co[1] * s;
		n.data[1][1] = Cofactor(0, 0, 2, 2) * s;
		n.data[1][2] = Cofactor(0, 2, 1, 0) * s;

		n.data[2][0] = co[2] * s;
		n.data[2][1] = Cofactor(0, 1, 2, 0) * s;
		n.data[2][2] = Cofactor(0, 0, 1, 1) * s;

		return n;
	}

	Matrix3 Scaled(vector scale)
	{
		Matrix3 n = new Matrix3;

		n.data[0][0] = data[0][0] * scale[0];
		n.data[1][0] = data[1][0] * scale[1];
		n.data[2][0] = data[2][0] * scale[2];

		n.data[0][1] = data[0][1] * scale[0];
		n.data[1][1] = data[1][1] * scale[1];
		n.data[2][1] = data[2][1] * scale[2];

		n.data[0][2] = data[0][2] * scale[0];
		n.data[1][2] = data[1][2] * scale[1];
		n.data[2][2] = data[2][2] * scale[2];

		return n;
	}

	Matrix3 Transpose()
	{
		Matrix3 n = new Matrix3;

		n.data[0][0] = data[0][0];
		n.data[0][1] = data[1][0];
		n.data[0][2] = data[2][0];

		n.data[1][0] = data[0][1];
		n.data[1][1] = data[1][1];
		n.data[1][2] = data[2][1];

		n.data[2][0] = data[0][2];
		n.data[2][1] = data[1][2];
		n.data[2][2] = data[2][2];

		return n;
	}

	Matrix3 Adjoint()
	{
		Matrix3 n = new Matrix3;

		n.data[0][0] = Cofactor(1, 1, 2, 2);
		n.data[0][1] = Cofactor(0, 2, 2, 1);
		n.data[0][2] = Cofactor(0, 1, 1, 2);

		n.data[1][0] = Cofactor(1, 2, 2, 0);
		n.data[1][1] = Cofactor(0, 0, 2, 2);
		n.data[1][2] = Cofactor(0, 2, 1, 0);

		n.data[2][0] = Cofactor(1, 0, 2, 1);
		n.data[2][1] = Cofactor(0, 1, 2, 0);
		n.data[2][2] = Cofactor(0, 0, 1, 1);

		return n;
	}

	float TDotX(vector v)
	{
		return data[0][0] * v[0] + data[1][0] * v[1] + data[2][0] * v[2];
	}

	float TDotY(vector v)
	{
		return data[0][1] * v[0] + data[1][1] * v[1] + data[2][1] * v[2];
	}

	float TDotZ(vector v)
	{
		return data[0][2] * v[0] + data[1][2] * v[1] + data[2][2] * v[2];
	}

	float Cofactor(int r1, int c1, int r2, int c2)
	{
		float a = data[r1][c1];
		float b = data[r2][c2];
		float c = data[r1][c2];
		float d = data[r2][c1];

		float ab = a * b;
		float cd = c * d;
		float ret = ab - cd;

		return ret;
	}

	vector Multiply(vector other)
	{
		vector n = vector.Zero;

		n[0] = vector.Dot(data[0], other);
		n[1] = vector.Dot(data[1], other);
		n[2] = vector.Dot(data[2], other);

		return n;
	}

	void FromYawPitchRoll(vector ypr)
	{
		Math3D.YawPitchRollMatrix(ypr, data);

		Update();
	}

	static Matrix3 YawPitchRoll(vector ypr)
	{
		Matrix3 mat = new Matrix3;

		Math3D.YawPitchRollMatrix(ypr, mat.data);

		mat.Update();

		return mat;
	}

	vector ToYawPitchRoll()
	{
		return Math3D.MatrixToAngles(data);
	}

	static void Tilda(vector vIn, out Matrix3 mat)
	{
		if (mat == NULL)
			mat = new Matrix3;

		mat.data[0][0] = 0.0;
		mat.data[1][1] = 0.0;
		mat.data[2][2] = 0.0;

		mat.data[0][1] = -vIn[2];
		mat.data[1][0] = vIn[2];
		mat.data[2][0] = -vIn[1];
		mat.data[0][2] = vIn[1];
		mat.data[1][2] = -vIn[0];
		mat.data[2][1] = vIn[0];

		mat.Update();
	}

	Matrix3 Multiply(Matrix3 m)
	{
		Matrix3 n = new Matrix3;

		Math3D.MatrixMultiply3(data, m.data, n.data);

		return n;
	}

	void Multiply(Matrix3 m, out Matrix3 n)
	{
		if (n == NULL)
			n = new Matrix3;

		Math3D.MatrixMultiply3(data, m.data, n.data);
	}

	static void Multiply(Matrix3 a, Matrix3 b, out Matrix3 n)
	{
		if (n == NULL)
			n = new Matrix3;

		Math3D.MatrixMultiply3(a.data, b.data, n.data);
	}

	Matrix3 InvMultiply(Matrix3 m)
	{
		Matrix3 n = new Matrix3;

		Math3D.MatrixInvMultiply3(data, m.data, n.data);

		return n;
	}

	static void DirectionAndUp(vector dir, vector up, out Matrix3 mat)
	{
		if (mat == NULL)
			mat = new Matrix3;

		vector temp[4];
		Math3D.DirectionAndUpMatrix(dir, up, temp);

		mat.data[0] = temp[0];
		mat.data[1] = temp[1];
		mat.data[2] = temp[2];
	}

	Matrix3 BISAdd(Matrix3 m)
	{
		Matrix3 n = new Matrix3;

		n.data[0] = data[0] + m.data[0];
		n.data[1] = data[1] + m.data[1];
		n.data[2] = data[2] + m.data[2];

		return n;
	}

	Matrix3 BISSubtract(Matrix3 m)
	{
		Matrix3 n = new Matrix3;

		n.data[0] = data[0] - m.data[0];
		n.data[1] = data[1] - m.data[1];
		n.data[2] = data[2] - m.data[2];

		return n;
	}

	Matrix3 BISMultiply(float m)
	{
		Matrix3 n = new Matrix3;

		n.data[0][0] = data[0][0] * m;
		n.data[1][0] = data[1][0] * m;
		n.data[2][0] = data[2][0] * m;

		n.data[0][1] = data[0][1] * m;
		n.data[1][1] = data[1][1] * m;
		n.data[2][1] = data[2][1] * m;

		n.data[0][2] = data[0][2] * m;
		n.data[1][2] = data[1][2] * m;
		n.data[2][2] = data[2][2] * m;

		return n;
	}
};
