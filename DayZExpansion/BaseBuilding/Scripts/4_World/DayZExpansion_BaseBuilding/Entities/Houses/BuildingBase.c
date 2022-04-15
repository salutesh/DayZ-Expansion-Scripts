/**
 * BuildingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BuildingBase
{
	// ------------------------------------------------------------
	// BuildingBase Explode
	// ------------------------------------------------------------
 	override void Explode(int damageType, string ammoType = "")
	{
		super.Explode(damageType, ammoType);

		ExpansionExplode( this, ammoType );
	}

	static void ExpansionExplode( EntityAI entity, string ammoType )
	{
		if ( !IsMissionHost() )
			return;

		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		float blastDropoff = 1;
		float blastDistance;
		float blastRange = 5;
		float blastDropoffRange = 2.5;
		bool dealDamage = !GetExpansionSettings().GetRaid().EnableExplosiveWhitelist;

		for ( int x = 0; x < GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist.Count(); x++ )
		{
			if ( entity.IsKindOf( GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist[x] ) )
			{
				dealDamage = true;
				break;
			}
		}

		if ( !dealDamage )
			return;

		//(point - min ) / (max - min ) 
		if (ammoType == "")
			ammoType = entity.ConfigGetString("ammoType");
		

		string dmgPath = "CfgAmmo" + " " + ammoType + " " + "DamageApplied" + " " + "Health" + " " + "Damage";
		int explosionDamage = GetGame().ConfigGetInt(dmgPath);
		
		array<Object> nearest_objects = new array<Object>;
		array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( entity.GetPosition(), blastRange, nearest_objects, proxy_cargos );
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			ItemBase nearest_item = ItemBase.Cast( nearest_objects.Get(i) );

			if ( nearest_item && nearest_item.IsInherited( ExpansionBaseBuilding ) && nearest_item.CanBeDamaged() )
			{
				blastDistance = vector.Distance(nearest_item.GetPosition(), entity.GetPosition());
				if (blastDistance > blastDropoffRange)
					blastDropoff = (1 - (blastDistance - blastDropoffRange) / (blastRange - blastDropoffRange));
				else 
					blastDropoff = 1;
				
				float health = nearest_item.m_CurrentHealth["GlobalHealth"];
				nearest_item.AddHealth( "GlobalHealth", "Health", -( explosionDamage * blastDropoff * explosionDamageMultiplier ) ); 
				nearest_item.m_CurrentHealth["GlobalHealth"] = nearest_item.GetHealth();
				nearest_item.RaidLog( entity, "GlobalHealth", health, explosionDamage * blastDropoff, explosionDamageMultiplier );
			}
		}
	}
}
