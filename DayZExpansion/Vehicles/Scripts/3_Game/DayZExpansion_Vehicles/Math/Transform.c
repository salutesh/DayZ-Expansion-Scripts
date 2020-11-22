/**
 * Transform.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Transform - Start");
		#endif
		
		data = { "1 0 0", "0 1 0", "0 1 0", "0 0 0" };

		m_basis = new Matrix3;
		m_rotation = new Quaternion;
		m_basis.m_trans = this;
		m_rotation.m_trans = this;

		UpdateUnion();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Transform - End");
		#endif
	}

	ref Transform Clone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Clone - Start");
		#endif
		
		Transform n = new ref Transform();

		n.data = data;
		n.UpdateUnion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Clone - Return: " + n.ToString() );
		#endif
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
	
	Transform Set( notnull Object obj, bool update = false )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Set - Start");
		#endif
		
		obj.SetTransform( data );

		if ( update )
		{
			obj.Update();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Set - Return: " + this.ToString() );
		#endif
		return this;
	}

	Transform Get( notnull Object obj )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Get - Start");
		#endif
		
		obj.GetTransform( data );

		UpdateUnion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Get - Return: " + this.ToString() );
		#endif
		return this;
	}

	static ref Transform GetArray( vector trans[4] )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetArray - Start");
		#endif
		
		Transform n = new ref Transform();

		n.data[0] = trans[0];
		n.data[1] = trans[1];
		n.data[2] = trans[2];
		n.data[3] = trans[3];

		n.UpdateUnion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetArray - Return: " + n.ToString() );
		#endif
		return n;
	}

	static ref Transform GetObject( notnull Object obj )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetObject - Start");
		#endif
		
		Transform n = new ref Transform();

		n.Get( obj );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetObject - Return: " + n.ToString() );
		#endif
		return n;
	}

	static ref Transform GetPlayerBoneWS( notnull Human human, int boneIdx, int mode = 0 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetPlayerBoneWS - Start");
		#endif
		
		Transform n = new ref Transform();

		human.GetBoneTransformWS( boneIdx, n.data );
		if ( mode == 1 )
		{
			n.data[0] = "1 0 0";
			n.data[1] = "0 1 0";
			n.data[2] = "0 0 1";
		} else if ( mode == 2 )
		{
			n.data[3] = "0 0 0";
		}

		n.UpdateUnion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::GetPlayerBoneWS - Return: " + n.ToString() );
		#endif
		return n;
	}

	static void Set( notnull Object obj, Transform trans )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Set - Start");
		#endif
		
		trans.Set( obj );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Set - End");
		#endif
	}

	void UpdateBasis()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateBasis - Start");
		#endif
		
		data[0] = m_basis.data[0];
		data[1] = m_basis.data[1];
		data[2] = m_basis.data[2];

		Math3D.MatrixToQuat( m_basis.data, m_rotation.data );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateBasis - End");
		#endif
	}

	void UpdateRotation()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateRotation - Start");
		#endif
		
		Math3D.QuatToMatrix( m_rotation.data, m_basis.data );

		data[0] = m_basis.data[0];
		data[1] = m_basis.data[1];
		data[2] = m_basis.data[2];
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateRotation - End");
		#endif
	}

	void UpdateUnion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateUnion - Start");
		#endif
		
		m_basis.data[0] = data[0];
		m_basis.data[1] = data[1];
		m_basis.data[2] = data[2];

		Math3D.MatrixToQuat( m_basis.data, m_rotation.data );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::UpdateUnion - End");
		#endif
	}

	Matrix3 GetBasis()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::GetBasis - Return: " + m_basis.ToString() );
		#endif
		return m_basis;
	}

	void SetBasis( ref Matrix3 basis )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetBasis - Start");
		#endif
		
		m_basis = basis;
		m_basis.m_trans = this;

		UpdateBasis();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetBasis - End");
		#endif
	}

	Quaternion GetRotation()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::GetRotation - Return: " + m_rotation.ToString() );
		#endif
		return m_rotation;
	}

	void SetRotation( ref Quaternion rotation )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetRotation - Start");
		#endif
		
		m_rotation = rotation;
		m_rotation.m_trans = this;

		UpdateRotation();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetRotation - End");
		#endif
	}

	vector GetYawPitchRoll()
	{
		return m_basis.ToYawPitchRoll();
	}

	vector GetOrigin()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::GetOrigin - Return: " + data[3].ToString() );
		#endif
		return data[3];
	}

	void SetOrigin( vector origin )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetOrigin - Start");
		#endif
		
		data[3] = origin;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::SetOrigin - End");
		#endif
	}

	static void DirectionAndUp( vector dir, vector up, out Transform trans, vector pos = "0 0 0" )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::DirectionAndUp - Start");
		#endif
		
		if ( trans == NULL )
			trans = new ref Transform;

		Math3D.DirectionAndUpMatrix( dir, up, trans.data );
		trans.UpdateUnion();

		trans.data[3] = pos;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::DirectionAndUp - End");
		#endif
	}

	vector Transform( vector v )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Transform - Start");
		#endif
		
		vector n = vector.Zero;
		
		n[0] = vector.Dot( data[0], v ) + data[3][0];
		n[1] = vector.Dot( data[1], v ) + data[3][1];
		n[2] = vector.Dot( data[2], v ) + data[3][2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::Transform - Return: " + n.ToString() );
		#endif
		return n;
	}

	static Transform YawPitchRoll( vector ypr, vector pos = "0 0 0" )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::YawPitchRoll - Start");
		#endif
		
		Transform trans = new ref Transform;

		Math3D.YawPitchRollMatrix( ypr, trans.m_basis.data );

		trans.UpdateBasis();

		trans.data[3] = pos;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::YawPitchRoll - Return: " + trans.ToString() );
		#endif
		return trans;
	}

	Transform Multiply( Transform m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::Multiply - Start");
		#endif
		
		Transform n = new ref Transform;

		Math3D.MatrixMultiply4( data, m.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Transform InvMultiply( Transform m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::InvMultiply - Start");
		#endif
		
		Transform n = new ref Transform;

		Math3D.MatrixInvMultiply4( data, m.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Transform::InvMultiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	void CopyTo( out vector mat[4] )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::CopyTo - Start");
		#endif
		
		mat[0] = data[0];
		mat[1] = data[1];
		mat[2] = data[2];
		mat[3] = data[3];
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Transform::CopyTo - End");
		#endif
	}
};