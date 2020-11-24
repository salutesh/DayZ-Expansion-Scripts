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
};