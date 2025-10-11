/**
 * ExpansionQuaternion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionQuaternion
{
	float data[4];

	ExpansionTransform m_trans;

	void ExpansionQuaternion()
	{
		data = {0, 0, 0, 1};
	}

	void Update()
	{
		if (m_trans)
			m_trans.UpdateRotation();
	}

	void ToMatrix(out ExpansionMatrix3 matrix)
	{
		if (matrix == NULL)
			matrix = new ExpansionMatrix3;

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

	static ExpansionQuaternion Rotation(float angle, vector axis)
	{
		ExpansionQuaternion q = new ExpansionQuaternion;
		q.SetRotation(angle, axis);

		return q;
	}

	ExpansionQuaternion Add(ExpansionQuaternion other)
	{
		ExpansionQuaternion n = new ExpansionQuaternion;

		n.data[0] = data[0] + other.data[0];
		n.data[1] = data[1] + other.data[1];
		n.data[2] = data[2] + other.data[2];
		n.data[3] = data[3] + other.data[3];

		return n;
	}

	ExpansionQuaternion AddSelf(ExpansionQuaternion other)
	{
		data[0] = data[0] + other.data[0];
		data[1] = data[1] + other.data[1];
		data[2] = data[2] + other.data[2];
		data[3] = data[3] + other.data[3];

		return this;
	}

	ExpansionQuaternion Multiply(float other)
	{
		ExpansionQuaternion n = new ExpansionQuaternion;

		n.data[0] = data[0] * other;
		n.data[1] = data[1] * other;
		n.data[2] = data[2] * other;
		n.data[3] = data[3] * other;

		return n;
	}

	ExpansionQuaternion MultiplySelf(float other)
	{
		data[0] = data[0] * other;
		data[1] = data[1] * other;
		data[2] = data[2] * other;
		data[3] = data[3] * other;

		return this;
	}
};
