/**
 * Flag_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Flag_Base
{
	private string m_Expansion_FlagTexturePath = "";

	// ------------------------------------------------------------
	// Flag_Base Constructor
	// ------------------------------------------------------------
	void Flag_Base()
	{
		TStringArray textures = new TStringArray;
		GetGame().ConfigGetTextArray( "CfgVehicles " + GetType() + " hiddenSelectionsTextures", textures );
		SetFlagTexture( textures[0] );
	}
	
	// ------------------------------------------------------------
	// Expansion SetTexture
	// ------------------------------------------------------------
	void SetFlagTexture(string texturePath)
	{
		if ( !IsMissionHost() || !texturePath )
			return;
		
		m_Expansion_FlagTexturePath = texturePath;
		SetObjectTexture( 0, m_Expansion_FlagTexturePath );
		
		CF_Log.Debug("ExpansionFlagBase::SetFlagTexture - Set flag texture path to: " + m_Expansion_FlagTexturePath);
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTexturePath
	// ------------------------------------------------------------
	string GetFlagTexturePath()
	{
		return m_Expansion_FlagTexturePath;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_Expansion_FlagTexturePath);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_FlagTexturePath))
			return false;

		return true;
	}
	#endif

	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

		super.AfterStoreLoad();
			
		SetFlagTexture( m_Expansion_FlagTexturePath );
	}
};
