/**
 * Quaternion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Quaternion - Start" );
		#endif
		
		data = { 0, 0, 0, 1 };
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Quaternion - End" );
		#endif
	}

	void Update()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Update - Start" );
		#endif
	   
		 if ( m_trans )
			m_trans.UpdateRotation();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Update - End" );
		#endif
	}

	void ToMatrix( out Matrix3 matrix )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::ToMatrix - Start" );
		#endif
		
		if ( matrix == NULL )
			matrix = new Matrix3;

		Math3D.QuatToMatrix( data, matrix.data );
		
		matrix.Update();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::ToMatrix - End" );
		#endif
	}

	void ToAngles( out vector vec )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::ToAngles - Start" );
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::ToAngles - End" );
		#endif
	}

	void SetRotation( float angle, vector axis )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::SetRotation - Start" );
		#endif
		
		float len = axis.Length();
		if ( len == 0 ) return;
		float s = Math.Sin( angle * 0.5 ) / len;
		data[0] = axis[0] * s;
		data[1] = axis[1] * s;
		data[2] = axis[2] * s;
		data[3] = Math.Cos( angle * 0.5 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::SetRotation - End" );
		#endif
	}

	static Quaternion Rotation( float angle, vector axis )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Rotation - Start" );
		#endif
		
		Quaternion q = new Quaternion;
		q.SetRotation( angle, axis );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::SetRotation - Return: " + q.ToString() );
		#endif
		return q;
	}

	Quaternion Add( Quaternion other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Add - Start" );
		#endif
		
		Quaternion n = new Quaternion;

		n.data[0] = data[0] + other.data[0];
		n.data[1] = data[1] + other.data[1];
		n.data[2] = data[2] + other.data[2];
		n.data[3] = data[3] + other.data[3];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Add - Return: " + n.ToString() );
		#endif
		return n;
	}

	Quaternion AddSelf( Quaternion other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::AddSelf - Start" );
		#endif
		
		data[0] = data[0] + other.data[0];
		data[1] = data[1] + other.data[1];
		data[2] = data[2] + other.data[2];
		data[3] = data[3] + other.data[3];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::AddSelf - Return: " + this.ToString() );
		#endif
		return this;
	}

	Quaternion Multiply( float other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Multiply - Start" );
		#endif
		
		Quaternion n = new Quaternion;

		n.data[0] = data[0] * other;
		n.data[1] = data[1] * other;
		n.data[2] = data[2] * other;
		n.data[3] = data[3] * other;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Quaternion MultiplySelf( float other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::MultiplySelf - Start" );
		#endif
		
		data[0] = data[0] * other;
		data[1] = data[1] * other;
		data[2] = data[2] * other;
		data[3] = data[3] * other;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Quaternion::MultiplySelf - Return: " + this.ToString() );
		#endif
		return this;
	}
};