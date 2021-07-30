/**
 * ExpansionExplosion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// ------------------------------------------------------------
// Expansion ExpansionCreateExplosion
// ------------------------------------------------------------	
static void ExpansionCreateExplosion( Object ent, string ammoType, float radius = 5, float healthDiv = 1 )
{
	#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreateExplosion - Start");
		#endif

	array<CargoBase> cargos = new array<CargoBase>;
	array<Object> objects = new array<Object>;

	vector pos = ent.GetPosition();
		
	GetGame().GetObjectsAtPosition3D( pos, radius, objects, cargos ); 
	for ( int i = 0; i < objects.Count(); i++ ) 
	{
		Object object = objects.Get(i);

		if ( object == ent )
			continue;

		/*
		if ( object.IsTransport() )
		{
			object.Explode( DT_EXPLOSION, ammoType );
		} else if ( object.IsEntityAI() || object.IsItemBase() ) 
		*/
		{
			float dist = vector.Distance( pos, object.GetPosition() );

			float dmg = healthDiv;
			if ( dist != 0 )
				dmg = dmg / dist;

			if ( object.IsInherited( CarScript ) )
			{
				object.DecreaseHealth( "", "", dmg * 100 );
			} else 
			{
				object.DecreaseHealth( "", "", dmg );
			}
		}
	}

	#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreateExplosion - End");
		#endif
}