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
	private string m_FlagTexturePath = "";

	// ------------------------------------------------------------
	// Flag_Base Constructor
	// ------------------------------------------------------------
	void Flag_Base()
	{
		TStringArray textures = new TStringArray;
		GetGame().ConfigGetTextArray( "CfgVehicles " + ClassName() + " hiddenSelectionsTextures", textures );
		SetFlagTexture( textures[0] );
	}
	
	// ------------------------------------------------------------
	// Flag_Base Destructor
	// ------------------------------------------------------------
	void ~Flag_Base()
	{
		
	}
	
	// ------------------------------------------------------------
	// Flag_Base IsVanillaFlag
	// ------------------------------------------------------------
	bool IsVanillaFlag( string classname )
	{
		if ( classname == "Flag_Chernarus" || classname == "Flag_Chedaki" || classname == "Flag_NAPA" || classname == "Flag_CDF" || classname == "Flag_Livonia" )
			return true;
		
		if ( classname == "Flag_Altis" || classname == "Flag_SSahrani" || classname == "Flag_NSahrani" || classname == "Flag_DayZ" || classname == "Flag_LivoniaArmy" )
			return true;
		
		if ( classname == "Flag_White" || classname == "Flag_Bohemia" || classname == "Flag_APA" || classname == "Flag_UEC" || classname == "Flag_Pirates" )	
			return true;
		
		if ( classname == "Flag_Cannibals" || classname == "Flag_Bear" || classname == "Flag_Wolf" || classname == "Flag_BabyDeer" || classname == "Flag_Rooster" )	
			return true;
		
		if ( classname == "Flag_LivoniaPolice" || classname == "Flag_CMC" || classname == "Flag_TEC" || classname == "Flag_CHEL" || classname == "Flag_Zenit" )	
			return true;

		if ( classname == "Flag_HunterZ" || classname == "Flag_BrainZ" )
			return true;

		if ( classname == "Flag_Refuge" || classname == "Flag_RSTA" || classname == "Flag_Snake" )
			return true;

		return false;
	}
		
	// ------------------------------------------------------------
	// Expansion SetTexture
	// ------------------------------------------------------------
	void SetFlagTexture(string texturePath)
	{
		if ( !IsMissionHost() || !texturePath )
			return;
		
		m_FlagTexturePath = texturePath;
		SetObjectTexture( 0, m_FlagTexturePath );
		
		CF_Log.Debug("ExpansionFlagBase::SetFlagTexture - Set flag texture path to: " + m_FlagTexturePath);
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTexturePath
	// ------------------------------------------------------------
	string GetFlagTexturePath()
	{
		return m_FlagTexturePath;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSION_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif
		
		super.OnStoreSave( ctx );
		
		ctx.Write( m_FlagTexturePath );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef EXPANSION_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_FlagTexturePath ), "[" + this + "] Failed reading m_FlagTexturePath" ) )
			return false;

		//! TODO: Remove this again after next update...
		ExpansionFixFlagTexturePath();
		
		return true;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_FlagTexturePath);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_FlagTexturePath))
			return false;

		//! TODO: Remove this again after next update...
		ExpansionFixFlagTexturePath();

		return true;
	}
	#endif

	//! TODO: Remove this again after next update...
	private void ExpansionFixFlagTexturePath()
	{
		string texturePath = m_FlagTexturePath;
		texturePath.ToLower();
		ExpansionString exFlagTexturePath = new ExpansionString(texturePath);
		if (exFlagTexturePath.EndsWith("\\flag_borduriens_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_BORDURIA;
		else if (exFlagTexturePath.EndsWith("\\flag_czech_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_CZECHIA;
		else if (exFlagTexturePath.EndsWith("\\flag_finnish_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_BORDURIA;
		else if (exFlagTexturePath.EndsWith("\\flag_irish_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_IRELAND;
		else if (exFlagTexturePath.EndsWith("\\flag_scottish_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_SCOTLAND;
		else if (exFlagTexturePath.EndsWith("\\flag_swedish_co.paa"))
			m_FlagTexturePath = EXPANSION_FLAG_COUNTRIES_SWEDEN;
	}

	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

		super.AfterStoreLoad();
			
		SetFlagTexture( m_FlagTexturePath );
	}
};

class Expansion_Flag_Expansion extends Flag_Base {};
class Expansion_Flag_White extends Flag_Base {};
class Expansion_Flag_Australia extends Flag_Base {};
class Expansion_Flag_Canada extends Flag_Base {};
class Expansion_Flag_Chernarus extends Flag_Base {};
class Expansion_Flag_France extends Flag_Base {};
class Expansion_Flag_Germany extends Flag_Base {};
class Expansion_Flag_Latvia extends Flag_Base {};
class Expansion_Flag_Luxembourg extends Flag_Base {};
class Expansion_Flag_Mexico extends Flag_Base {};
class Expansion_Flag_Netherlands extends Flag_Base {};
class Expansion_Flag_Newzealand extends Flag_Base {};
class Expansion_Flag_Norway extends Flag_Base {};
class Expansion_Flag_Poland extends Flag_Base {};
class Expansion_Flag_Russia extends Flag_Base {};
class Expansion_Flag_UK extends Flag_Base {};
class Expansion_Flag_USA extends Flag_Base {};
class Expansion_Flag_Scotland extends Flag_Base {};
class Expansion_Flag_Scottish : Expansion_Flag_Scotland {};  //! Just for backwards compatibility
class Expansion_Flag_Sweden extends Flag_Base {};
class Expansion_Flag_Swedish : Expansion_Flag_Sweden {};  //! Just for backwards compatibility
class Expansion_Flag_Spain extends Flag_Base {};
class Expansion_Flag_Brazil extends Flag_Base {};
class Expansion_Flag_Portugal extends Flag_Base {};
class Expansion_Flag_Belgium extends Flag_Base {};
class Expansion_Flag_Livonia extends Flag_Base {};
class Expansion_Flag_Japan extends Flag_Base {};
class Expansion_Flag_China extends Flag_Base {};
class Expansion_Flag_SouthKorea extends Flag_Base {};
class Expansion_Flag_UN extends Flag_Base {};
class Expansion_Flag_NATO extends Flag_Base {};
class Expansion_Flag_Pirate extends Flag_Base {};
class Expansion_Flag_Chedaki extends Flag_Base {};
class Expansion_Flag_Napa extends Flag_Base {};
class Expansion_Flag_Cdf extends Flag_Base {};
class Expansion_Flag_NuevoRico extends Flag_Base {};
class Expansion_Flag_Borduria extends Flag_Base {};
class Expansion_Flag_Borduriens : Expansion_Flag_Borduria {};  //! Just for backwards compatibility
class Expansion_Flag_Biohazard extends Flag_Base {};
class Expansion_Flag_AnyoneInCherno extends Flag_Base {};
class Expansion_Flag_Ireland extends Flag_Base {};
class Expansion_Flag_Irish : Expansion_Flag_Ireland {};  //! Just for backwards compatibility
class Expansion_Flag_Wales extends Flag_Base {};
class Expansion_Flag_Switzerland extends Flag_Base {};
class Expansion_Flag_Skilanka extends Flag_Base {};
class Expansion_Flag_SouthAfrica extends Flag_Base {};
class Expansion_Flag_Sicily extends Flag_Base {};
class Expansion_Flag_OffWithHead extends Flag_Base {};
class Expansion_Flag_Gubraltar extends Flag_Base {};
class Expansion_Flag_Czechia extends Flag_Base {};
class Expansion_Flag_Czech : Expansion_Flag_Czechia {};  //! Just for backwards compatibility
class Expansion_Flag_Fari extends Flag_Base {};
class Expansion_Flag_Finland extends Flag_Base {};
class Expansion_Flag_Finnish : Expansion_Flag_Finland {};  //! Just for backwards compatibility
class Expansion_Flag_DayZWhite extends Flag_Base {};
class Expansion_Flag_DayZBlack extends Flag_Base {};
class Expansion_Flag_DoubleAxe extends Flag_Base {};
class Expansion_Flag_Grenade extends Flag_Base {};
class Expansion_Flag_Red extends Flag_Base {};
class Expansion_Flag_Blue extends Flag_Base {};
class Expansion_Flag_Green extends Flag_Base {};
class Expansion_Flag_Yellow extends Flag_Base {};
class Expansion_Flag_Orange extends Flag_Base {};
class Expansion_Flag_Pink extends Flag_Base {};
class Expansion_Flag_Purple extends Flag_Base {};
class Expansion_Flag_Rainbow extends Flag_Base {};