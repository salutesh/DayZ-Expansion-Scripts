/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{	
	//============================================
	// ItemBase Constructor
	//============================================
	void ItemBase()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredInit );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );
	}

	//============================================
	// LongDeferredInit
	//============================================	
	void LongDeferredInit()
	{
	}
	
	//============================================
	// DeferredInit
	//============================================	
	void DeferredInit()
	{
	}

	#ifdef CF_MOD_STORAGE
	override void OnModStoreSave( ModStorage storage, string modName )
	{
		super.OnModStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Vehicles" )
			return;

		//! Write here
	}
	
	override bool OnModStoreLoad( ModStorage storage, string modName )
	{
		if ( !super.OnModStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Vehicles" )
			return true;

		//! Read here

		return true;
	}
	#endif
	
	//============================================
	// EEOnAfterLoad
	//============================================
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();
	}

	//============================================
	// GetHiddenSelectionIndex
	//============================================
	override int GetHiddenSelectionIndex( string selection )
	{
		array<string> config_selections	= new array<string>;
		ConfigGetTextArray( "hiddenSelections", config_selections );
		
		for ( int i = 0; i < config_selections.Count(); ++i )
		{
			if ( config_selections.Get( i ) == selection )
			{
				return i;
			}
		}
		
		return -1;
	}

	//============================================
	// OnVariablesSynchronized
	//============================================
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
	}
	
	//============================================
	// Explode
	//============================================
 	override void Explode(int damageType, string ammoType = "")
	{
		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		float blastDropoff = 1;
		float blastDistance;
		float blastRange = 5;
		float blastDropoffRange = 2.5;
		super.Explode(damageType, ammoType);
		//(point - min ) / (max - min ) 
		if (ammoType == "")
			ammoType = this.ConfigGetString("ammoType");

		string dmgPath = "CfgAmmo" + " " + ammoType + " " + "DamageApplied" + " " + "Health" + " " + "Damage";
		int explosionDamage = GetGame().ConfigGetInt(dmgPath);
		
		ref array<Object> nearest_objects = new array<Object>;
		ref array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), blastRange, nearest_objects, proxy_cargos );
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			bool dealDamage = !GetExpansionSettings().GetRaid().EnableExplosiveWhitelist;
			Object nearest_object = nearest_objects.Get(i);
			//TODO Add support back for exploding buildings
			/*
			if ( nearest_object.IsInherited( ExpansionBaseBuilding ) )
			{
				blastDistance = vector.Distance(nearest_object.GetPosition(), this.GetPosition());
				if (blastDistance > blastDropoffRange)
					blastDropoff = (1 - (blastDistance - blastDropoffRange) / (blastRange - blastDropoffRange));
				else 
					blastDropoff = 1;
				
				
				for (int x = 0; x < GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist.Count(); ++x)
				{

					if (this.IsKindOf(GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist[x]))
					{
						dealDamage = true;
					}
				}
				if (dealDamage)
					nearest_object.AddHealth( "GlobalHealth", "Health", ( explosionDamage * blastDropoff * explosionDamageMultiplier * -1) ); 
			}
			*/
		}
	}
	
	bool IsLocked()
	{
		return false;
	}
};