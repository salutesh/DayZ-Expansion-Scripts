/**
 * Quaternion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Quaternion
{
	float data[4];

	Transform m_trans;

	void Quaternion()
	{
		data = {0, 0, 0, 1};
	}

	void Update()
	{
		if (m_trans)
			m_trans.UpdateRotation();
	}

	void ToMatrix(out Matrix3 matrix)
	{
		if (matrix == NULL)
			matrix = new Matrix3;

		Math3D.QuatToMatrix(data, matrix.data);

		matrix.Update();
	}

	void ToAngles(out vector vec)
	{
	}

	void SetRotation(float angle, vector axis)
	{
		float len = axis.Length();
		if (len == 0)
			return;

		float s = Math.Sin(angle * 0.5) / len;
		data[0] = axis[0] * s;
		data[1] = axis[1] * s;
		data[2] = axis[2] * s;
		data[3] = Math.Cos(angle * 0.5);
	}

	static Quaternion Rotation(float angle, vector axis)
	{
		Quaternion q = new Quaternion;
		q.SetRotation(angle, axis);

		return q;
	}

	Quaternion Add(Quaternion other)
	{
		Quaternion n = new Quaternion;

		n.data[0] = data[0] + other.data[0];
		n.data[1] = data[1] + other.data[1];
		n.data[2] = data[2] + other.data[2];
		n.data[3] = data[3] + other.data[3];

		return n;
	}

	Quaternion AddSelf(Quaternion other)
	{
		data[0] = data[0] + other.data[0];
		data[1] = data[1] + other.data[1];
		data[2] = data[2] + other.data[2];
		data[3] = data[3] + other.data[3];

		return this;
	}

	Quaternion Multiply(float other)
	{
		Quaternion n = new Quaternion;

		n.data[0] = data[0] * other;
		n.data[1] = data[1] * other;
		n.data[2] = data[2] * other;
		n.data[3] = data[3] * other;

		return n;
	}

	Quaternion MultiplySelf(float other)
	{
		data[0] = data[0] * other;
		data[1] = data[1] * other;
		data[2] = data[2] * other;
		data[3] = data[3] * other;

		return this;
	}
};
