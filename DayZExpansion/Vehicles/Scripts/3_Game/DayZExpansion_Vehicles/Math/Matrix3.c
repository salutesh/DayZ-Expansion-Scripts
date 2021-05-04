/**
 * Matrix3.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Matrix3 - Start");
		#endif
		
		data = { "1 0 0", "0 1 0", "0 0 1" };
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Matrix3 - End");
		#endif
	}

	static Matrix3 SetRotationX( float angle )
	{
		Matrix3 mat = new ref Matrix3();

		float s = Math.Sin( angle );
		float c = Math.Cos( angle );

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
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Update - Start");
		#endif
		
		if ( m_trans )
			m_trans.UpdateBasis();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Update - End");
		#endif
	}

	void ToQuaternion( out Quaternion quat )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::ToQuaternion - Start");
		#endif
		
		if ( quat == NULL )
			quat = new Quaternion;

		Math3D.MatrixToQuat( data, quat.data );

		quat.Update();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::ToQuaternion - End");
		#endif
	}

	Matrix3 Invert()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Invert - Start");
		#endif
		
		Matrix3 n = new ref Matrix3;

		vector co = Vector( Cofactor( 1, 1, 2, 2 ), Cofactor( 1, 2, 2, 0 ), Cofactor( 1, 0, 2, 1 ) );

		float det = vector.Dot( data[0], co );
		if ( det == 0 )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Invert - Return: " + n.ToString() );
		#endif
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

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Invert - Return: " + n.ToString() );
		#endif
		return n;
	}

	Matrix3 Scaled( vector scale )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Scaled - Start");
		#endif
		
		Matrix3 n = new ref Matrix3;
		
		n.data[0][0] = data[0][0] * scale[0];
		n.data[1][0] = data[1][0] * scale[1];
		n.data[2][0] = data[2][0] * scale[2];

		n.data[0][1] = data[0][1] * scale[0];
		n.data[1][1] = data[1][1] * scale[1];
		n.data[2][1] = data[2][1] * scale[2];

		n.data[0][2] = data[0][2] * scale[0];
		n.data[1][2] = data[1][2] * scale[1];
		n.data[2][2] = data[2][2] * scale[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Scaled - Return: " + n.ToString() );
		#endif
		return n;
	}

	Matrix3 Transpose()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Transpose - Start");
		#endif
		
		Matrix3 n = new ref Matrix3;
		
		n.data[0][0] = data[0][0];
		n.data[0][1] = data[1][0];
		n.data[0][2] = data[2][0];

		n.data[1][0] = data[0][1];
		n.data[1][1] = data[1][1];
		n.data[1][2] = data[2][1];

		n.data[2][0] = data[0][2];
		n.data[2][1] = data[1][2];
		n.data[2][2] = data[2][2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Transpose - Return: " + n.ToString() );
		#endif
		return n;
	}

	Matrix3 Adjoint()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Matrix3::Adjoint - Start");
		#endif
		
		Matrix3 n = new ref Matrix3;

		n.data[0][0] = Cofactor( 1, 1, 2, 2 ); 
		n.data[0][1] = Cofactor( 0, 2, 2, 1 ); 
		n.data[0][2] = Cofactor( 0, 1, 1, 2 );

		n.data[1][0] = Cofactor( 1, 2, 2, 0 ); 
		n.data[1][1] = Cofactor( 0, 0, 2, 2 ); 
		n.data[1][2] = Cofactor( 0, 2, 1, 0 );

		n.data[2][0] = Cofactor( 1, 0, 2, 1 ); 
		n.data[2][1] = Cofactor( 0, 1, 2, 0 ); 
		n.data[2][2] = Cofactor( 0, 0, 1, 1 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Adjoint - Return: " + n.ToString() );
		#endif
		return n;
	}

	float TDotX( vector v )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::TDotX - Return: " + (data[0][0] * v[0] + data[1][0] * v[1] + data[2][0] * v[2]).ToString() );
		#endif
		return data[0][0] * v[0] + data[1][0] * v[1] + data[2][0] * v[2];
	}

	float TDotY( vector v )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::TDotY - Return: " + (data[0][1] * v[0] + data[1][1] * v[1] + data[2][1] * v[2]).ToString() );
		#endif
		return data[0][1] * v[0] + data[1][1] * v[1] + data[2][1] * v[2];
	}

	float TDotZ( vector v )
	{
		return data[0][2] * v[0] + data[1][2] * v[1] + data[2][2] * v[2];
	}

	float Cofactor( int r1, int c1, int r2, int c2 ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Cofactor - Start" );
		#endif

		float a = data[r1][c1];
		float b = data[r2][c2];
		float c = data[r1][c2];
		float d = data[r2][c1];

		float ab = a * b;
		float cd = c * d;
		float ret = ab - cd;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Cofactor - Return: " + ret );
		#endif
		return ret;
	}

	vector Multiply( vector other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Start" );
		#endif
		
		vector n = vector.Zero;

		n[0] = vector.Dot( data[0], other );
		n[1] = vector.Dot( data[1], other );
		n[2] = vector.Dot( data[2], other );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	void FromYawPitchRoll( vector ypr )
	{
		Math3D.YawPitchRollMatrix( ypr, data );

		Update();
	}

	static Matrix3 YawPitchRoll( vector ypr )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::YawPitchRoll - Start" );
		#endif
		Matrix3 mat = new ref Matrix3;

		Math3D.YawPitchRollMatrix( ypr, mat.data );

		mat.Update();

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::YawPitchRoll - Return: " + mat.ToString() );
		#endif
		return mat;
	}

	vector ToYawPitchRoll()
	{
		return Math3D.MatrixToAngles( data );
	}

	static void Tilda( vector vIn, out Matrix3 mat )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Tilda - Start" );
		#endif
		
		if ( mat == NULL )
			mat = new ref Matrix3;

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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Tilda - End" );
		#endif
	}

	Matrix3 Multiply( Matrix3 m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Start" );
		#endif
		
		Matrix3 n = new ref Matrix3;

		Math3D.MatrixMultiply3( data, m.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	void Multiply( Matrix3 m, out Matrix3 n )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Start" );
		#endif
		
		if ( n == NULL )
			n = new ref Matrix3;

		Math3D.MatrixMultiply3( data, m.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Return: " + n.ToString() );
		#endif
	}

	static void Multiply( Matrix3 a, Matrix3 b, out Matrix3 n )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Start" );
		#endif
		
		if ( n == NULL )
			n = new ref Matrix3;

		Math3D.MatrixMultiply3( a.data, b.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::Multiply - Return: " + n.ToString() );
		#endif
	}

	Matrix3 InvMultiply( Matrix3 m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::InvMultiply - Start" );
		#endif
		
		Matrix3 n = new ref Matrix3;

		Math3D.MatrixInvMultiply3( data, m.data, n.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::InvMultiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	static void DirectionAndUp( vector dir, vector up, out Matrix3 mat )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::DirectionAndUp - Start" );
		#endif
		
		if ( mat == NULL )
			mat = new ref Matrix3;

		vector temp[4];
		Math3D.DirectionAndUpMatrix( dir, up, temp );

		mat.data[0] = temp[0];
		mat.data[1] = temp[1];
		mat.data[2] = temp[2];
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::DirectionAndUp - End" );
		#endif
	}

	Matrix3 BISAdd( Matrix3 m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISAdd - Start" );
		#endif
		
		Matrix3 n = new ref Matrix3;
		
		n.data[0] = data[0] + m.data[0];
		n.data[1] = data[1] + m.data[1];
		n.data[2] = data[2] + m.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISAdd - Return: " + n.ToString() );
		#endif
		return n;
	}

	Matrix3 BISSubtract( Matrix3 m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISSubtract - Start" );
		#endif
		
		Matrix3 n = new ref Matrix3;
		
		n.data[0] = data[0] - m.data[0];
		n.data[1] = data[1] - m.data[1];
		n.data[2] = data[2] - m.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISSubtract - Return: " + n.ToString() );
		#endif
		return n;
	}

	Matrix3 BISMultiply( float m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISMultiply - Start" );
		#endif
		
		Matrix3 n = new ref Matrix3;
		
		n.data[0][0] = data[0][0] * m;
		n.data[1][0] = data[1][0] * m;
		n.data[2][0] = data[2][0] * m;

		n.data[0][1] = data[0][1] * m;
		n.data[1][1] = data[1][1] * m;
		n.data[2][1] = data[2][1] * m;

		n.data[0][2] = data[0][2] * m;
		n.data[1][2] = data[1][2] * m;
		n.data[2][2] = data[2][2] * m;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Matrix3::BISMultiply - Return: " + n.ToString() );
		#endif
		return n;
	}
};