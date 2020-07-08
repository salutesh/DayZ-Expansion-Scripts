/**
 * ExpansionStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// -----------------------------------------------------------
// Expansion GetARGB
// -----------------------------------------------------------
static void GetARGB( int argb, out int a, out int r, out int g, out int b )
{
	a = ( argb >> 24 ) & 0xFF;
	r = ( argb >> 16 ) & 0xFF;
	g = ( argb >> 8 ) & 0xFF;
	b = argb & 0xFF;
}

// -----------------------------------------------------------
// Expansion EXPrint
// -----------------------------------------------------------
static void EXPrint( string s )
{
	Print( "[EXPANSION DEBUG]: " + s );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( string s )
{
	Print( "[EXPANSION LOG]: " + s );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( float s )
{
	Print( "[EXPANSION LOG]: " + s );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( int s )
{
	Print( "[EXPANSION LOG]: " + s );
}

// -----------------------------------------------------------
// Expansion String FloatToString
// -----------------------------------------------------------
static string FloatToString(float nmb)
{
	//It's a int number so doesn't need to convert it
	if ( nmb == Math.Ceil(nmb) || nmb == Math.Floor(nmb) )
		return nmb.ToString();
	
	//It's a temp string
	string str;

	if (nmb < 0)
	{
		str = (nmb - Math.Ceil(nmb)).ToString();
		return ((Math.Ceil(nmb)).ToString() + (str.Substring(2, 10) ));
	}

	str = (nmb - Math.Floor(nmb)).ToString();
	return ( ( (Math.Floor(nmb)).ToString() ) + ( str.Substring(1, 10) ) );
}

// -----------------------------------------------------------
// Expansion Float LinearConversion
// -----------------------------------------------------------
static float LinearConversion( float minFrom, float maxFrom, float value, float minTo, float maxTo, bool clamp = true )
{
	float newValue = (((value - minFrom) * (maxTo - minTo)) / (maxFrom - minFrom)) + minTo;
	
	if ( clamp )
	{
		float min = Math.Min(minTo, maxTo);
		float max = Math.Max(minTo, maxTo);
		return Math.Clamp( newValue, min, max );
	}

	return newValue;
}

// -----------------------------------------------------------
// Expansion Float FloatNewPrecision
// -----------------------------------------------------------
static float FloatNewPrecision(float n, float i) 
{ 
	return Math.Floor(Math.Pow(10,i)*n)/Math.Pow(10,i); 
}

/**
 * @brief Remove's the last character from the string
 * 
 * @param str input string
 * 
 * @return a copy of str with the last character removed
 */
static string ExpansionRemoveLastChar( string str ) 
{
	return str.Substring( 0, str.Length() - 1 );
}

// -----------------------------------------------------------
// Expansion DisablePhysics
// -----------------------------------------------------------
static void DisablePhysics(Object ent)
{
	SetVelocity( ent, Vector( 0, 0, 0 ) );

	dBodySetAngularVelocity( ent, Vector( 0, 0, 0 ) );
	dBodyActive( ent, ActiveState.INACTIVE );
	dBodyDynamic( ent, false );
}

// -----------------------------------------------------------
// Expansion EnablePhysics
// -----------------------------------------------------------
static void EnablePhysics(Object ent)
{
	if( ent )
	{
		SetVelocity( ent, Vector( 0, 0, 0 ) );

		dBodySetAngularVelocity( ent, Vector( 0, 0, 0 ) );
		dBodyActive( ent, ActiveState.ALWAYS_ACTIVE );
		dBodyDynamic( ent, true );
	}
}


// -----------------------------------------------------------
// Expansion TStringArray ExpansionWorkingZombieClasses
// -----------------------------------------------------------
static TStringArray ExpansionWorkingZombieClasses()
{
	return { "ZmbM_HermitSkinny_Base","ZmbM_HermitSkinny_Beige","ZmbM_HermitSkinny_Black","ZmbM_HermitSkinny_Green",
			 "ZmbM_HermitSkinny_Red","ZmbM_FarmerFat_Base","ZmbM_FarmerFat_Beige","ZmbM_FarmerFat_Blue","ZmbM_FarmerFat_Brown",
			 "ZmbM_FarmerFat_Green","ZmbF_CitizenANormal_Base","ZmbF_CitizenANormal_Beige","ZmbF_CitizenANormal_Brown",
			 "ZmbF_CitizenANormal_Blue","ZmbM_CitizenASkinny_Base","ZmbM_CitizenASkinny_Blue","ZmbM_CitizenASkinny_Brown",
			 "ZmbM_CitizenASkinny_Grey","ZmbM_CitizenASkinny_Red","ZmbM_CitizenBFat_Base","ZmbM_CitizenBFat_Blue","ZmbM_CitizenBFat_Red",
			 "ZmbM_CitizenBFat_Green","ZmbF_CitizenBSkinny_Base","ZmbF_CitizenBSkinny","ZmbM_PrisonerSkinny_Base","ZmbM_PrisonerSkinny",
			 "ZmbM_FirefighterNormal_Base","ZmbM_FirefighterNormal","ZmbM_FishermanOld_Base","ZmbM_FishermanOld_Blue","ZmbM_FishermanOld_Green",
			 "ZmbM_FishermanOld_Grey","ZmbM_FishermanOld_Red","ZmbM_JournalistSkinny_Base","ZmbM_JournalistSkinny","ZmbF_JournalistNormal_Base",
			 "ZmbF_JournalistNormal_Blue","ZmbF_JournalistNormal_Green","ZmbF_JournalistNormal_Red","ZmbF_JournalistNormal_White",
			 "ZmbM_ParamedicNormal_Base","ZmbM_ParamedicNormal_Blue","ZmbM_ParamedicNormal_Green","ZmbM_ParamedicNormal_Red",
			 "ZmbM_ParamedicNormal_Black","ZmbF_ParamedicNormal_Base","ZmbF_ParamedicNormal_Blue","ZmbF_ParamedicNormal_Green",
			 "ZmbF_ParamedicNormal_Red","ZmbM_HikerSkinny_Base","ZmbM_HikerSkinny_Blue","ZmbM_HikerSkinny_Green","ZmbM_HikerSkinny_Yellow",
			 "ZmbF_HikerSkinny_Base","ZmbF_HikerSkinny_Blue","ZmbF_HikerSkinny_Grey","ZmbF_HikerSkinny_Green","ZmbF_HikerSkinny_Red",
			 "ZmbM_HunterOld_Base","ZmbM_HunterOld_Autumn","ZmbM_HunterOld_Spring","ZmbM_HunterOld_Summer","ZmbM_HunterOld_Winter",
			 "ZmbF_SurvivorNormal_Base","ZmbF_SurvivorNormal_Blue","ZmbF_SurvivorNormal_Orange","ZmbF_SurvivorNormal_Red",
			 "ZmbF_SurvivorNormal_White","ZmbM_SurvivorDean_Black","ZmbM_SurvivorDean_Blue","ZmbM_SurvivorDean_Grey",
			 "ZmbM_PolicemanFat_Base","ZmbM_PolicemanFat","ZmbF_PoliceWomanNormal_Base","ZmbF_PoliceWomanNormal","ZmbM_PolicemanSpecForce_Base",
			 "ZmbM_PolicemanSpecForce","ZmbM_SoldierNormal_Base","ZmbM_SoldierNormal","ZmbM_usSoldier_normal_Base",
			 "ZmbM_usSoldier_normal_Woodland","ZmbM_usSoldier_normal_Desert","ZmbM_CommercialPilotOld_Base","ZmbM_CommercialPilotOld_Blue",
			 "ZmbM_CommercialPilotOld_Olive","ZmbM_CommercialPilotOld_Brown","ZmbM_CommercialPilotOld_Grey","ZmbM_PatrolNormal_Base",
			 "ZmbM_PatrolNormal_PautRev","ZmbM_PatrolNormal_Autumn","ZmbM_PatrolNormal_Flat","ZmbM_PatrolNormal_Summer","ZmbM_JoggerSkinny_Base",
			 "ZmbM_JoggerSkinny_Blue","ZmbM_JoggerSkinny_Green","ZmbM_JoggerSkinny_Red","ZmbF_JoggerSkinny_Base","ZmbF_JoggerSkinny_Blue",
			 "ZmbF_JoggerSkinny_Brown","ZmbF_JoggerSkinny_Green","ZmbF_JoggerSkinny_Red","ZmbM_MotobikerFat_Base","ZmbM_MotobikerFat_Beige",
			 "ZmbM_MotobikerFat_Black","ZmbM_MotobikerFat_Blue","ZmbM_VillagerOld_Base","ZmbM_VillagerOld_Blue","ZmbM_VillagerOld_Green",
			 "ZmbM_VillagerOld_White","ZmbM_SkaterYoung_Base","ZmbM_SkaterYoung_Blue","ZmbM_SkaterYoung_Brown","ZmbM_SkaterYoung_Green",
			 "ZmbM_SkaterYoung_Grey","ZmbF_SkaterYoung_Base","ZmbF_SkaterYoung_Brown","ZmbF_SkaterYoung_Striped","ZmbF_SkaterYoung_Violet",
			 "ZmbF_DoctorSkinny_Base","ZmbF_DoctorSkinny","ZmbF_BlueCollarFat_Base","ZmbF_BlueCollarFat_Blue","ZmbF_BlueCollarFat_Green",
			 "ZmbF_BlueCollarFat_Red","ZmbF_BlueCollarFat_White","ZmbF_MechanicNormal_Base","ZmbF_MechanicNormal_Beige","ZmbF_MechanicNormal_Green",
			 "ZmbF_MechanicNormal_Grey","ZmbF_MechanicNormal_Orange","ZmbM_MechanicSkinny_Base","ZmbM_MechanicSkinny_Blue","ZmbM_MechanicSkinny_Grey",
			 "ZmbM_MechanicSkinny_Green","ZmbM_MechanicSkinny_Red","ZmbM_ConstrWorkerNormal_Base","ZmbM_ConstrWorkerNormal_Beige",
			 "ZmbM_ConstrWorkerNormal_Black","ZmbM_ConstrWorkerNormal_Green","ZmbM_ConstrWorkerNormal_Grey","ZmbM_HeavyIndustryWorker_Base",
			 "ZmbM_HeavyIndustryWorker","ZmbM_OffshoreWorker_Base","ZmbM_OffshoreWorker_Green","ZmbM_OffshoreWorker_Orange","ZmbM_OffshoreWorker_Red",
			 "ZmbM_OffshoreWorker_Yellow","ZmbF_NurseFat_Base","ZmbF_NurseFat","ZmbM_HandymanNormal_Base","ZmbM_HandymanNormal_Beige",
			 "ZmbM_HandymanNormal_Blue","ZmbM_HandymanNormal_Green","ZmbM_HandymanNormal_Grey","ZmbM_HandymanNormal_White","ZmbM_DoctorFat_Base",
			 "ZmbM_DoctorFat","ZmbM_Jacket_Base","ZmbM_Jacket_beige","ZmbM_Jacket_black","ZmbM_Jacket_blue","ZmbM_Jacket_bluechecks",
			 "ZmbM_Jacket_brown","ZmbM_Jacket_greenchecks","ZmbM_Jacket_grey","ZmbM_Jacket_khaki","ZmbM_Jacket_magenta","ZmbM_Jacket_stripes",
			 "ZmbF_PatientOld_Base","ZmbF_PatientOld","ZmbM_PatientSkinny_Base","ZmbM_PatientSkinny","ZmbF_ShortSkirt_Base","ZmbF_ShortSkirt_beige",
			 "ZmbF_ShortSkirt_black","ZmbF_ShortSkirt_brown","ZmbF_ShortSkirt_green","ZmbF_ShortSkirt_grey","ZmbF_ShortSkirt_checks",
			 "ZmbF_ShortSkirt_red","ZmbF_ShortSkirt_stripes","ZmbF_ShortSkirt_white","ZmbF_ShortSkirt_yellow","ZmbF_VillagerOld_Base",
			 "ZmbF_VillagerOld_Blue","ZmbF_VillagerOld_Green","ZmbF_VillagerOld_Red","ZmbF_VillagerOld_White","ZmbM_Soldier","ZmbM_SoldierAlice",
			 "ZmbM_SoldierHelmet","ZmbM_SoldierVest","ZmbM_SoldierAliceHelmet","ZmbM_SoldierVestHelmet","ZmbF_MilkMaidOld_Base",
			 "ZmbF_MilkMaidOld_Beige","ZmbF_MilkMaidOld_Black","ZmbF_MilkMaidOld_Green","ZmbF_MilkMaidOld_Grey","ZmbM_priestPopSkinny_Base",
			 "ZmbM_priestPopSkinny","ZmbM_ClerkFat_Base","ZmbM_ClerkFat_Brown","ZmbM_ClerkFat_Grey","ZmbM_ClerkFat_Khaki","ZmbM_ClerkFat_White",
			 "ZmbF_Clerk_Normal_Base","ZmbF_Clerk_Normal_Blue","ZmbF_Clerk_Normal_White","ZmbF_Clerk_Normal_Green","ZmbF_Clerk_Normal_Red" };
}

static TStringArray CustomExpansionWorkingZombieClasses()
{
	return { "ExpansionZmbF_BlueCollarFat_Blue","ExpansionZmbF_BlueCollarFat_Green","ExpansionZmbF_BlueCollarFat_Red","ExpansionZmbF_BlueCollarFat_White" };
}

// -----------------------------------------------------------
// Expansion String ExpansionGetItemDisplayNameWithType
// -----------------------------------------------------------
string ExpansionGetItemDisplayNameWithType( string type_name)
{
	string cfg_name;
	string cfg_name_path;
	
	if ( GetGame().ConfigIsExisting( CFG_WEAPONSPATH + " " + type_name ) )
	{
		cfg_name_path = CFG_WEAPONSPATH + " " + type_name + " displayName";
		GetGame().ConfigGetText( cfg_name_path, cfg_name );
		return cfg_name;
	} 
	
	if ( GetGame().ConfigIsExisting( CFG_VEHICLESPATH + " " + type_name ) )
	{
		cfg_name_path = CFG_VEHICLESPATH + " " + type_name + " displayName";
		GetGame().ConfigGetText( cfg_name_path, cfg_name );
		return cfg_name;
	} 
	
	if ( GetGame().ConfigIsExisting( CFG_MAGAZINESPATH + " " + type_name ) )
	{
		cfg_name_path = CFG_MAGAZINESPATH + " " + type_name + " displayName";
		GetGame().ConfigGetText( cfg_name_path, cfg_name );
		return cfg_name;
	}
	
	return type_name;
}

// -----------------------------------------------------------
// Expansion String ExpansionGetItemDescriptionWithType
// -----------------------------------------------------------
string ExpansionGetItemDescriptionWithType( string type_name )
{
	string cfg_des;
	string cfg_des_path;
	
	if ( GetGame().ConfigIsExisting( CFG_WEAPONSPATH + " " + type_name ) )
	{
		cfg_des_path = CFG_WEAPONSPATH + " " + type_name + " descriptionShort";
		GetGame().ConfigGetText( cfg_des_path, cfg_des );
		return cfg_des;
	} 
	
	if ( GetGame().ConfigIsExisting( CFG_VEHICLESPATH + " " + type_name ) )
	{
		cfg_des_path = CFG_VEHICLESPATH + " " + type_name + " descriptionShort";
		GetGame().ConfigGetText( cfg_des_path, cfg_des );
		return cfg_des;
	} 
	
	if ( GetGame().ConfigIsExisting( CFG_MAGAZINESPATH + " " + type_name ) )
	{
		cfg_des_path = CFG_MAGAZINESPATH + " " + type_name + " descriptionShort";
		GetGame().ConfigGetText( cfg_des_path, cfg_des );
		return cfg_des;
	}
	
	return cfg_des_path;
}

// ------------------------------------------------------------
// Expansion RGBtoInt
// ------------------------------------------------------------
static int RGBtoInt(int r, int g, int b)
{
	return ( r << 0 ) | ( g << 8 ) | ( b << 16 );
}


// ------------------------------------------------------------
// Expansion ARGBtoInt
// ------------------------------------------------------------
static int ARGBtoInt(int a, int r, int g, int b)
{
	return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
}

// ------------------------------------------------------------
// Expansion IntToRGB
// ------------------------------------------------------------
static void IntToRGB(int value, out int red, out int green, out int blue)
{
	red =   ( value >>  0 ) & 255;
	green = ( value >>  8 ) & 255;
	blue =  ( value >> 16 ) & 255;
}

// ------------------------------------------------------------
// Expansion IntToARGB
// ------------------------------------------------------------
static void IntToARGB(int value, out int alpha, out int red, out int green, out int blue)
{
	alpha = ( value >> 24 ) & 255;
	red =   ( value >>  16 ) & 255;
	green = ( value >>  8 ) & 255;
	blue =  ( value ) & 255;
}

// ------------------------------------------------------------
// Expansion GetTime
// ------------------------------------------------------------
static string GetTime()
{
	int hour;
	int minute;
	int second;
	
	GetHourMinuteSecond(hour, minute, second);
	
	string date = hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	
	return date;
}

// ------------------------------------------------------------
// Expansion GetTimeUTC
// ------------------------------------------------------------
static string GetTimeUTC()
{
	int hour;
	int minute;
	int second;
	
	GetHourMinuteSecondUTC(hour, minute, second);
	
	string date = hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	
	return date;
}

// ------------------------------------------------------------
// Expansion FormatTime
// ------------------------------------------------------------
static string FormatTime( float time )
{
	int hours = (int) time / 3600000;
	int minutes = (int) time / 60000 ;
	int seconds = (int) time / 1000 - 60 * minutes;
	
	string timestring = hours.ToStringLen(2) + ":" + minutes.ToStringLen(2) + ":" + seconds.ToStringLen(2);
	
	return timestring;
}

class ExpansionStatic
{
	protected autoptr array<ref ExpansionFlagTexture> m_FlagTextures;
	
	void ExpansionStatic()
	{
		m_FlagTextures = new array<ref ExpansionFlagTexture>;
		
		LoadFlagTextures();
	}
	
	ref array<ref ExpansionFlagTexture> GetFlagTextures()
	{
		return m_FlagTextures;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFlagTextures
	// Contains path and name information for each flag texture
	// ------------------------------------------------------------
	void LoadFlagTextures()
	{
		AddFlagTexture(EXPANSION_FLAG_LOGO_EXPANSION, "Expansion");
		AddFlagTexture(EXPANSION_FLAG_COLOR_WHITE, "White");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_AUSTRALIA, "Australia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CANADA, "Canada");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHERNARUS, "Chernarus");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FRANCE, "France");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_GERMANY, "Germany");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LATVIA, "Latvia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LUXEMBOURG, "Luxembourg");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_MEXICO, "Mexico");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NETHERLANDS, "Netherlands");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NEWZEALAND, "New Zealand");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NORWAY, "Norway");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_POLAND, "Poland");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_RUSSIA, "Russia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_UK, "UK");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_USA, "USA");
		
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SCOTTISH, "Scottish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FINNISH, "Finnish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SWEDISH, "Swedish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SPAIN, "Spain");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BRAZIL, "Brazil");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_PORTUGAL, "Portugal");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BELGIUM, "Belgium");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LIVONIA, "Livonia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_JAPAN, "Japan");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHINA, "China");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SOUTHKOREA, "SouthKorea");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_UN, "UN");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NATO, "NATO");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_PIRATE, "Pirate");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHEDAKI, "Chedaki");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NAPA, "NAPA");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CDF, "CDF");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NUEVORICO, "NuevoRico");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BORDURIENS, "Borduriens");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BIOHAZARD, "Biohazard");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_ANYONEINCHERNO, "AnyoneInCherno");
	}
	
	// ------------------------------------------------------------
	// Expansion AddFlagTexture
	// ------------------------------------------------------------
	void AddFlagTexture( string path, string name )
	{
		ExpansionFlagTexture flagTexture = new ExpansionFlagTexture( path, name );
		
		m_FlagTextures.Insert( flagTexture );
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveFlagTexture
	// ------------------------------------------------------------
	bool RemoveFlagTexture( string path )
	{
		for (int i = 0; i < m_FlagTextures.Count(); ++i)
		{
			if (m_FlagTextures[i] && m_FlagTextures[i].Path == path)
			{
				m_FlagTextures.Remove(i);
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTexture
	// Hardcoded flag texture paths
	// Not used anymore
	// Will be removed in next update
	// ------------------------------------------------------------	
	string GetFlagTexturePath( int id )
	{
		string path = "";
		
		switch (id)
		{
			case 1:
			{
				path = EXPANSION_FLAG_LOGO_EXPANSION;
				break;
			}
			case 2:
			{
				path = EXPANSION_FLAG_COLOR_WHITE;
				break;
			}
			case 3:
			{
				path = EXPANSION_FLAG_COUNTRIES_AUSTRALIA;
				break;
			}
			case 4:
			{
				path = EXPANSION_FLAG_COUNTRIES_CANADA;
				break;
			}
			case 5:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHERNARUS;
				break;
			}
			case 6:
			{
				path = EXPANSION_FLAG_COUNTRIES_FRANCE;
				break;
			}
			case 7:
			{
				path = EXPANSION_FLAG_COUNTRIES_GERMANY;
				break;
			}
			case 8:
			{
				path = EXPANSION_FLAG_COUNTRIES_LATVIA;
				break;
			}
			case 9:
			{
				path = EXPANSION_FLAG_COUNTRIES_LUXEMBOURG;
				break;
			}
			case 10:
			{
				path = EXPANSION_FLAG_COUNTRIES_MEXICO;
				break;
			}
			case 11:
			{
				path = EXPANSION_FLAG_COUNTRIES_NETHERLANDS;
				break;
			}
			case 12:
			{
				path = EXPANSION_FLAG_COUNTRIES_NEWZEALAND;
				break;
			}
			case 13:
			{
				path = EXPANSION_FLAG_COUNTRIES_NORWAY;
				break;
			}
			case 14:
			{
				path = EXPANSION_FLAG_COUNTRIES_POLAND;
				break;
			}
			case 15:
			{
				path = EXPANSION_FLAG_COUNTRIES_RUSSIA;
				break;
			}
			case 16:
			{
				path = EXPANSION_FLAG_COUNTRIES_UK;
				break;
			}
			case 17:
			{
				path = EXPANSION_FLAG_COUNTRIES_USA;
				break;
			}			
			case 18:
			{
				path = EXPANSION_FLAG_COUNTRIES_SCOTTISH;
				break;
			}
			case 19:
			{
				path = EXPANSION_FLAG_COUNTRIES_FINNISH;
				break;
			}
			case 20:
			{
				path = EXPANSION_FLAG_COUNTRIES_SWEDISH;
				break;
			}
			case 21:
			{
				path = EXPANSION_FLAG_COUNTRIES_SPAIN;
				break;
			}
			case 22:
			{
				path = EXPANSION_FLAG_COUNTRIES_BRAZIL;
				break;
			}
			case 23:
			{
				path = EXPANSION_FLAG_COUNTRIES_PORTUGAL;
				break;
			}
			case 24:
			{
				path = EXPANSION_FLAG_COUNTRIES_BELGIUM;
				break;
			}
			case 25:
			{
				path = EXPANSION_FLAG_COUNTRIES_LIVONIA;
				break;
			}
			case 26:
			{
				path = EXPANSION_FLAG_COUNTRIES_JAPAN;
				break;
			}
			case 27:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHINA;
				break;
			}
			case 28:
			{
				path = EXPANSION_FLAG_COUNTRIES_SOUTHKOREA;
				break;
			}
			case 29:
			{
				path = EXPANSION_FLAG_COUNTRIES_UN;
				break;
			}
			case 30:
			{
				path = EXPANSION_FLAG_COUNTRIES_NATO;
				break;
			}
			case 31:
			{
				path = EXPANSION_FLAG_COUNTRIES_PIRATE;
				break;
			}
			case 32:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHEDAKI;
				break;
			}
			case 33:
			{
				path = EXPANSION_FLAG_COUNTRIES_NAPA;
				break;
			}
			case 34:
			{
				path = EXPANSION_FLAG_COUNTRIES_CDF;
				break;
			}
			case 35:
			{
				path = EXPANSION_FLAG_COUNTRIES_NUEVORICO;
				break;
			}
			case 36:
			{
				path = EXPANSION_FLAG_COUNTRIES_BORDURIENS;
				break;
			}
			case 37:
			{
				path = EXPANSION_FLAG_COUNTRIES_BIOHAZARD;
				break;
			}
			case 38:
			{
				path = EXPANSION_FLAG_COUNTRIES_ANYONEINCHERNO;
				break;
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionStatic::GetFlagTexturePath - Return texture path: " + path + " for ID:" + id.ToString() );
		#endif
	
		return path;
	}
}

static ref ExpansionStatic GetExpansionStatic()
{
	static ref ExpansionStatic g_exGlobalStatic;
	
	if ( !g_exGlobalStatic )
	{
		g_exGlobalStatic = new ExpansionStatic();
	}

	return g_exGlobalStatic;
}