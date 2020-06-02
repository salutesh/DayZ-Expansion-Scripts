/**
 * Vector.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

ref Vector3 VecWrap( vector v )
{
	ref Vector3 vec = new ref Vector3;
	vec.data = v;
	return vec;
}

ref Vector3 Vec( float x = 0, float y = 0, float z = 0 )
{
	ref Vector3 vec = new ref Vector3;
	vec.data[0] = x;
	vec.data[1] = y;
	vec.data[2] = z;
	return vec;
}

vector VecEngine( ref Vector3 v )
{
	if ( !v )
		return "0 0 0";

	return v.data;
}

ref Vector3 VecNew( ref Vector3 v )
{
	ref Vector3 vec = new ref Vector3;
	vec.data = v.data;
	return vec;
}

class Vector3
{
	static const Vector3 Up = VecWrap( "0 1 0" );
	static const Vector3 Aside = VecWrap( "1 0 0" );
	static const Vector3 Forward = VecWrap( "0 0 1" );
	static const Vector3 Zero = VecWrap( "0 0 0" );

	vector data;

	Transform m_trans;

	void Vector3()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vector3::Vector3 - Start");
		#endif
		
		data = vector.Zero;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vector3::Vector3 - End");
		#endif
	}

	void Update()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vector3::Update - Start");
		#endif
		
		//if ( m_trans )
		//	m_trans.UpdateOrigin();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vector3::Update - End");
		#endif
	}

	float Normalize()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vector3::Normalize - Start");
		#endif
		
		float len = data.Normalize();

		Update();

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Normalize - Return: " + len.ToString() );
		#endif
		return len;
	}

	Vector3 Normalized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Normalized - Return: " + VecWrap( data.Normalized() ).ToString() );
		#endif
		return VecWrap( data.Normalized() );
	}

	float Length()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Length - Return: " + data.Length().ToString() );
		#endif
		return data.Length();
	}

	float LengthSq()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::LengthSq - Return: " + data.LengthSq().ToString() );
		#endif
		return data.LengthSq();
	}

	static float Distance( Vector3 v1, Vector3 v2 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Distance - Return: " + vector.Distance( v1.data, v2.data ).ToString() );
		#endif
		return vector.Distance( v1.data, v2.data );
	}

	static float DistanceSq( Vector3 v1, Vector3 v2 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::DistanceSq - Return: " + vector.DistanceSq( v1.data, v2.data ).ToString() );
		#endif
		return vector.DistanceSq( v1.data, v2.data );
	}

	Vector3 Perpend()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Perpend - Return: " + VecWrap( data * vector.Up ).ToString() );
		#endif
		return VecWrap( data * vector.Up );
	}

	static Vector3 Direction( Vector3 v1, Vector3 v2 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Direction - Return: " + VecWrap( vector.Direction( v1.data, v2.data ) ).ToString() );
		#endif
		return VecWrap( vector.Direction( v1.data, v2.data ) );
	}

	static Vector3 Direction( vector v1, vector v2 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Direction - Return: " + VecWrap( vector.Direction( v1, v2 ) ).ToString() );
		#endif
		return VecWrap( vector.Direction( v1, v2 ) );
	}

	static float Dot( Vector3 v1, Vector3 v2 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Dot - Return: " + vector.Dot( v1.data, v2.data ).ToString() );
		#endif
		return vector.Dot( v1.data, v2.data );
	}

	float Dot( Vector3 other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Dot - Return: " + vector.Dot( data, other.data ).ToString() );
		#endif
		return vector.Dot( data, other.data );
	}

	float Dot( vector other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Dot - Return: " + vector.Dot( data, other ).ToString() );
		#endif
		return vector.Dot( data, other );
	}

	Vector3 GetRelAngles()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::GetRelAngles - Start" );
		#endif
		
		for ( int i = 0; i < 3; i++ )
		{
			if ( data[i] > 180 )
				data[i] = data[i] - 360;
			if ( data[i] < -180 )
				data[i] = data[i] + 360;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::GetRelAngles - Return: " + this.ToString() );
		#endif
		return this;
	}

	Vector3 Add(Vector3 other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Add - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] + other.data[0];
		n.data[1] = data[1] + other.data[1];
		n.data[2] = data[2] + other.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Add - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Add(vector other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Add - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] + other[0];
		n.data[1] = data[1] + other[1];
		n.data[2] = data[2] + other[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Add - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Sub(Vector3 other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Sub - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] - other.data[0];
		n.data[1] = data[1] - other.data[1];
		n.data[2] = data[2] - other.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Sub - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Sub(vector other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Sub - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] - other[0];
		n.data[1] = data[1] - other[1];
		n.data[2] = data[2] - other[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Sub - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Multiply(Vector3 other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] * other.data[0];
		n.data[1] = data[1] * other.data[1];
		n.data[2] = data[2] * other.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Multiply(vector other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] * other[0];
		n.data[1] = data[1] * other[1];
		n.data[2] = data[2] * other[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Multiply( float other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] * other;
		n.data[1] = data[1] * other;
		n.data[2] = data[2] * other;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Multiply( Matrix3 other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data = data.Multiply3( other.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Multiply( Transform other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data = data.Multiply4( other.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 InvMultiply( Matrix3 other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::InvMultiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data = data.InvMultiply3( other.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::InvMultiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 InvMultiply( Transform other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::InvMultiply - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data = data.InvMultiply4( other.data );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::InvMultiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Quaternion Multiply( Quaternion q )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Start" );
		#endif
		
		Quaternion n = new Quaternion;
		
		n.data[0] =  data[0] * q.data[3] + data[1] * q.data[2] - data[2] * q.data[1];
		n.data[1] =  data[1] * q.data[3] + data[2] * q.data[0] - data[0] * q.data[2];
		n.data[2] =  data[2] * q.data[3] + data[0] * q.data[1] - data[1] * q.data[0];
		n.data[3] = -data[0] * q.data[0] - data[1] * q.data[1] - data[2] * q.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Divide(Vector3 other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Divide - Start" );
		#endif
		
		Vector3 n = new ref Vector3;
		
		n.data[0] = data[0] / other.data[0];
		n.data[1] = data[1] / other.data[1];
		n.data[2] = data[2] / other.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Divide - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Divide(vector other)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Divide - Start" );
		#endif
		
		Vector3 n = new ref Vector3;

		n.data[0] = data[0] / other[0];
		n.data[1] = data[1] / other[1];
		n.data[2] = data[2] / other[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Divide - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Negate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Negate - Start" );
		#endif
		
		Vector3 n = new ref Vector3;

		n.data[0] = -data[0];
		n.data[1] = -data[1];
		n.data[2] = -data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Negate - Return: " + n.ToString() );
		#endif
		return n;
	}

	Vector3 Cross( Vector3 other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Cross - Start" );
		#endif
		
		Vector3 n = new ref Vector3;

		n.data = data * other.data;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "Vector3::Cross - Return: " + n.ToString() );
		#endif
		return n;
	}
}

class VectorHelper
{
	static vector Rotate( Matrix3 a, vector o )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Rotate - Start" );
		#endif
		
		vector n = vector.Zero;

		float x = o[0];
		float y = o[1];
		float z = o[2];

		float xx = a.data[0][0] * x;
		float xy = a.data[0][1] * y;
		float xz = a.data[0][2] * z;

		float yx = a.data[1][0] * x;
		float yy = a.data[1][1] * y;
		float yz = a.data[1][2] * z;

		float zx = a.data[2][0] * x;
		float zy = a.data[2][1] * y;
		float zz = a.data[2][2] * z;

		n[0] = xx + xy + xz;
		n[1] = yx + yy + yz;
		n[2] = zx + zy + zz;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Rotate - Return: " + n.ToString() );
		#endif
		return n;
	}

	static vector Multiply( vector data, vector other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Start" );
		#endif
		
		vector n = vector.Zero;
		
		n[0] = data[0] * other[0];
		n[1] = data[1] * other[1];
		n[2] = data[2] * other[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	static vector Multiply( vector data, float other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Start" );
		#endif
		
		vector n = vector.Zero;
		
		n[0] = data[0] * other;
		n[1] = data[1] * other;
		n[2] = data[2] * other;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	static vector Multiply( vector data, Matrix3 other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Return: " + data.Multiply3( other.data ).ToString() );
		#endif
		return data.Multiply3( other.data );
	}

	static vector Multiply( vector data, Transform other )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Return: " + data.Multiply4( other.data ).ToString() );
		#endif	
		return data.Multiply4( other.data );
	}

	static vector InvMultiply( vector data, Matrix3 other )
	{	  
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::InvMultiply - Return: " + data.InvMultiply3( other.data ).ToString() );
		#endif	
		return data.InvMultiply3( other.data );
	}

	static vector InvMultiply( vector data, Transform other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::InvMultiply - Return: " + data.InvMultiply4( other.data ).ToString() );
		#endif  
		return data.InvMultiply4( other.data );
	}

	static Quaternion Multiply( vector data, Quaternion q )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Start" );
		#endif  
		
		Quaternion n = new Quaternion;
		
		n.data[0] =  data[0] * q.data[3] + data[1] * q.data[2] - data[2] * q.data[1];
		n.data[1] =  data[1] * q.data[3] + data[2] * q.data[0] - data[0] * q.data[2];
		n.data[2] =  data[2] * q.data[3] + data[0] * q.data[1] - data[1] * q.data[0];
		n.data[3] = -data[0] * q.data[0] - data[1] * q.data[1] - data[2] * q.data[2];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Multiply - Return: " + n.ToString() );
		#endif
		return n;
	}

	static vector CrossProduct( vector a, vector b )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::CrossProduct - Start" );
		#endif  
		
		float ox = b[0];
		float oy = b[1];
		float oz = b[2];
		float lx = a[0];
		float ly = a[1];
		float lz = a[2];

		float x = ly * oz - lz * oy;
		float y = lz * ox - lx * oz;
		float z = lx * oy - ly * ox;

		vector n = Vector( x, y, z );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::CrossProduct - Return: " + n.ToString() );
		#endif
		return n;
	}

	static vector Cross( vector data, vector other )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Cross - Start" );
		#endif  
		
		vector n = vector.Zero;
		
		n[0] = data[1] * other[2] - data[2] * other[1];
		n[1] = data[2] * other[0] - data[0] * other[2];
		n[2] = data[0] * other[1] - data[1] * other[0];

		#ifdef EXPANSIONEXPRINT
		EXPrint( "VectorHelper::Cross - Return: " + n.ToString() );
		#endif
		return n;
	}
}