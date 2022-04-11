/**
 * Transform.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Transform // also known as Matrix4
{
	vector data[4];

	ref Matrix3 m_basis;
	ref Quaternion m_rotation;

	void Transform()
	{
		data = {"1 0 0", "0 1 0", "0 1 0", "0 0 0"};

		m_basis = new Matrix3;
		m_rotation = new Quaternion;
		m_basis.m_trans = this;
		m_rotation.m_trans = this;

		UpdateUnion();
	}

	Transform Clone()
	{
		Transform n = new Transform();

		n.data = data;
		n.UpdateUnion();

		return n;
	}

	void Debug()
	{
		Print("Transform::Debug");
		Print("[0] " + data[0]);
		Print("[1] " + data[1]);
		Print("[2] " + data[2]);
		Print("[3] " + data[3]);
	}

	Transform Add(Transform transform)
	{
		return NULL;
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

	Transform Set(notnull Object obj, bool update = false)
	{
		obj.SetTransform(data);

		if (update)
		{
			obj.Update();
		}

		return this;
	}

	Transform Get(notnull Object obj)
	{
		obj.GetTransform(data);

		UpdateUnion();

		return this;
	}

	static Transform GetArray(vector trans[4])
	{
		Transform n = new Transform();

		n.data[0] = trans[0];
		n.data[1] = trans[1];
		n.data[2] = trans[2];
		n.data[3] = trans[3];

		n.UpdateUnion();

		return n;
	}

	static Transform GetObject(notnull Object obj)
	{
		Transform n = new Transform();

		n.Get(obj);

		return n;
	}

	static Transform GetPlayerBoneWS(notnull Human human, int boneIdx, int mode = 0)
	{
		Transform n = new Transform();

		human.GetBoneTransformWS(boneIdx, n.data);
		if (mode == 1)
		{
			n.data[0] = "1 0 0";
			n.data[1] = "0 1 0";
			n.data[2] = "0 0 1";
		}
		else if (mode == 2)
		{
			n.data[3] = "0 0 0";
		}

		n.UpdateUnion();

		return n;
	}

	static void Set(notnull Object obj, Transform trans)
	{
		trans.Set(obj);
	}

	void UpdateBasis()
	{
		data[0] = m_basis.data[0];
		data[1] = m_basis.data[1];
		data[2] = m_basis.data[2];

		Math3D.MatrixToQuat(m_basis.data, m_rotation.data);
	}

	void UpdateRotation()
	{
		Math3D.QuatToMatrix(m_rotation.data, m_basis.data);

		data[0] = m_basis.data[0];
		data[1] = m_basis.data[1];
		data[2] = m_basis.data[2];
	}

	void UpdateUnion()
	{
		m_basis.data[0] = data[0];
		m_basis.data[1] = data[1];
		m_basis.data[2] = data[2];

		Math3D.MatrixToQuat(m_basis.data, m_rotation.data);
	}

	Matrix3 GetBasis()
	{
		return m_basis;
	}

	void SetBasis(Matrix3 basis)
	{
		m_basis = basis;
		m_basis.m_trans = this;

		UpdateBasis();
	}

	Quaternion GetRotation()
	{
		return m_rotation;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
		m_rotation.m_trans = this;

		UpdateRotation();
	}

	vector GetYawPitchRoll()
	{
		return m_basis.ToYawPitchRoll();
	}

	vector GetOrigin()
	{
		return data[3];
	}

	void SetOrigin(vector origin)
	{
		data[3] = origin;
	}

	static void DirectionAndUp(vector dir, vector up, out Transform trans, vector pos = "0 0 0")
	{
		if (trans == NULL)
			trans = new Transform;

		Math3D.DirectionAndUpMatrix(dir, up, trans.data);
		trans.UpdateUnion();

		trans.data[3] = pos;
	}

	vector Transform(vector v)
	{
		vector n = vector.Zero;

		n[0] = vector.Dot(data[0], v) + data[3][0];
		n[1] = vector.Dot(data[1], v) + data[3][1];
		n[2] = vector.Dot(data[2], v) + data[3][2];

		return n;
	}

	static Transform YawPitchRoll(vector ypr, vector pos = "0 0 0")
	{
		Transform trans = new Transform;

		Math3D.YawPitchRollMatrix(ypr, trans.m_basis.data);

		trans.UpdateBasis();

		trans.data[3] = pos;

		return trans;
	}

	Transform Multiply(Transform m)
	{
		Transform n = new Transform;

		Math3D.MatrixMultiply4(data, m.data, n.data);

		return n;
	}

	Transform InvMultiply(Transform m)
	{
		Transform n = new Transform;

		Math3D.MatrixInvMultiply4(data, m.data, n.data);

		return n;
	}

	void CopyTo(out vector mat[4])
	{
		mat[0] = data[0];
		mat[1] = data[1];
		mat[2] = data[2];
		mat[3] = data[3];
	}
};
