/**
 * ExpansionPhysicsGeometry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPhysicsGeometry: PhysicsGeomDef
{
	void ExpansionPhysicsGeometry( string name, dGeom geom, string materialName, int layerMask )
	{
		Name = name;
		Geometry = geom;
		MaterialName = materialName;
		LayerMask = layerMask;

		Frame[0] = "1 0 0";
		Frame[1] = "0 1 0";
		Frame[2] = "0 0 1";
		Frame[3] = "0 0 0";
	}

	static ref ExpansionPhysicsGeometry Create()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::Create");
		#endif

		return new ExpansionPhysicsGeometry( "", NULL, "material/default", 0xFFFFFFFF );
	}

	static ref ExpansionPhysicsGeometry CreateBox( vector size )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::Create");
		#endif
		
		ExpansionPhysicsGeometry geom = new ExpansionPhysicsGeometry( "", NULL, "material/default", 0xFFFFFFFF );
		geom.Geometry = dGeomCreateBox( size );
		
		return geom;
	}

	ref ExpansionPhysicsGeometry SetGeom( dGeom geom )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::SetGeom");
		#endif
		
		Geometry = geom;

		return this;
	}

	ref ExpansionPhysicsGeometry SetMaterial( string materialName )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::SetMaterial");
		#endif
		
		MaterialName = materialName;
		
		return this;
	}

	ref ExpansionPhysicsGeometry SetMask( int layerMask )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::SetMask");
		#endif
		
		LayerMask = layerMask;
		
		return this;
	}

	ref ExpansionPhysicsGeometry SetTransform( Transform transform )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::SetTransform");
		#endif
		
		Frame[0] = transform.data[0];
		Frame[1] = transform.data[1];
		Frame[2] = transform.data[2];
		Frame[3] = transform.data[3];
		
		return this;
	}

	ref ExpansionPhysicsGeometry SetPosition( vector position )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysicsGeometry::SetPosition");
		#endif
		
		Frame[3] = position;
		
		return this;
	}
}