/**
 * Flag_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		
		return false;
	}
		
	// ------------------------------------------------------------
	// Expansion SetTexture
	// ------------------------------------------------------------
	void SetFlagTexture(string texturePath)
	{
		if ( !IsMissionHost() )
			return;
		
		m_FlagTexturePath = texturePath;
		SetObjectTexture( 0, m_FlagTexturePath );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::SetFlagTexture - Set flag texture path to: " + m_FlagTexturePath);
		#endif
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
		super.OnStoreSave( ctx );
		
		ctx.Write( m_FlagTexturePath );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_FlagTexturePath ), "[" + this + "] Failed reading m_FlagTexturePath" ) )
			return false;
		
		return true;
	}

	override void AfterStoreLoad()
	{
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
class Expansion_Flag_Scottish extends Flag_Base {};
class Expansion_Flag_Finnish extends Flag_Base {};
class Expansion_Flag_Swedish extends Flag_Base {};
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
class Expansion_Flag_Borduriens extends Flag_Base {};
class Expansion_Flag_Biohazard extends Flag_Base {};
class Expansion_Flag_AnyoneInCherno extends Flag_Base {};
class Expansion_Flag_Irish extends Flag_Base {};
class Expansion_Flag_Wales extends Flag_Base {};
class Expansion_Flag_Switzerland extends Flag_Base {};
class Expansion_Flag_Skilanka extends Flag_Base {};
class Expansion_Flag_SouthAfrica extends Flag_Base {};
class Expansion_Flag_Sicily extends Flag_Base {};
class Expansion_Flag_OffWithHead extends Flag_Base {};
class Expansion_Flag_Gubraltar extends Flag_Base {};
class Expansion_Flag_Czech extends Flag_Base {};
class Expansion_Flag_Fari extends Flag_Base {};
class Expansion_Flag_Finland extends Flag_Base {};
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