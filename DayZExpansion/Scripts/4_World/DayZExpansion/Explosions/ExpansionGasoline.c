/**
 * ExpansionGasoline.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*modded class CanisterGasoline
{
	bool m_Exploded;
	
	// ------------------------------------------------------------
	void ~CanisterGasoline()
	{
	}
	
	// ------------------------------------------------------------
	void CanisterGasoline()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] Constructor start");
		#endif

		m_Exploded = false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] Constructor end");
		#endif
	}
		
	// ------------------------------------------------------------
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] EEHitBy start");
		#endif

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos);
		
		if( this.GetLiquidType() != LIQUID_GASOLINE )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] EEHitBy end");
		#endif
			return;
		}
		
		if (GetGame().IsServer())
		{
			if (!m_Exploded)
			{		
				ExpansionCreateExplosion( this, ammo, 5, 50 );
				Object explosion = GetGame().CreateObject( "ExplosionSmall", this.GetPosition() );
				m_Exploded = true;
				
				if (!this.IsRuined())
				{
					SetHealth("", "", 0);
				}	
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] EEHitBy end");
		#endif
	}
	
	// ------------------------------------------------------------
	override bool IsContainer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] IsContainer");
		#endif
		return true;
	}
	
	// ------------------------------------------------------------
	override string GetPouringSoundset()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetPouringSoundset");
		#endif
		return "emptyVessle_CanisterGasoline_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingLoopSoundsetHard()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingLoopSoundsetHard");
		#endif
		return "pour_HardGround_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingLoopSoundsetSoft()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingLoopSoundsetSoft");
		#endif
		return "pour_SoftGround_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingLoopSoundsetWater()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingLoopSoundsetWater");
		#endif
		return "pour_Water_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingEndSoundsetHard()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingEndSoundsetHard");
		#endif
		return "pour_End_HardGround_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingEndSoundsetSoft()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingEndSoundsetSoft");
		#endif
		return "pour_End_SoftGround_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override string GetEmptyingEndSoundsetWater()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] GetEmptyingEndSoundsetWater");
		#endif
		return "pour_End_Water_GasolineCanister_SoundSet";
	}
	
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] CanPutInCargo start");
		#endif
		if( !super.CanPutInCargo(parent) )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] CanPutInCargo end");
		#endif
			return false;
		}	
		if ( !(parent.IsKindOf("CanisterGasoline")) )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] CanPutInCargo end");
		#endif
			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CanisterGasoline] CanPutInCargo end");
		#endif
		return false;
	}
}*/