/**
 * ExpansionStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static bool Expansion_Assert_False( bool check, string message )
{
	if ( check == false )
	{
		Assert_Log( message );
		return true;
	}

	return false;
}

// -----------------------------------------------------------
// Expansion EXPrint
// -----------------------------------------------------------
static void EXPrint( string s, string prefix = " [EXPANSION DEBUG]: " )
{
	Print( ExpansionStatic.GetTimestamp() + prefix + s );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( string s )
{
	EXPrint( s, " [EXPANSION LOG]: " );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( float s )
{
	EXLogPrint( s.ToString() );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( int s )
{
	EXLogPrint( s.ToString() );
}

//! Prints a hitch warning if elapsed time since startTime is above threshold
static void EXPrintHitch( string msgPrefix, float startTime, float threshold = 0.0125 )
{
	float elapsedTime = GetGame().GetTickTime() - startTime;
	if (elapsedTime > threshold)
		EXPrint(msgPrefix + "HITCH: " + (elapsedTime * 1000) + "ms");
}

class ExpansionStatic
{
	static const string BASE16 = "0123456789ABCDEF";

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
	// Expansion Float FloatNewPrecision
	// -----------------------------------------------------------
	static float FloatNewPrecision(float n, float i) 
	{ 
		return Math.Floor(Math.Pow(10,i)*n)/Math.Pow(10,i); 
	}

	// -----------------------------------------------------------
	// Expansion TStringArray GetWorkingZombieClasses
	// -----------------------------------------------------------
	static TStringArray GetWorkingZombieClasses()
	{
		return { "ZmbM_HermitSkinny_Beige","ZmbM_HermitSkinny_Black","ZmbM_HermitSkinny_Green",
				 "ZmbM_HermitSkinny_Red","ZmbM_FarmerFat_Beige","ZmbM_FarmerFat_Blue","ZmbM_FarmerFat_Brown",
				 "ZmbM_FarmerFat_Green","ZmbF_CitizenANormal_Beige","ZmbF_CitizenANormal_Brown",
				 "ZmbF_CitizenANormal_Blue","ZmbM_CitizenASkinny_Blue","ZmbM_CitizenASkinny_Brown",
				 "ZmbM_CitizenASkinny_Grey","ZmbM_CitizenASkinny_Red","ZmbM_CitizenBFat_Blue","ZmbM_CitizenBFat_Red",
				 "ZmbM_CitizenBFat_Green","ZmbF_CitizenBSkinny","ZmbM_PrisonerSkinny",
				 "ZmbM_FirefighterNormal","ZmbM_FishermanOld_Blue","ZmbM_FishermanOld_Green",
				 "ZmbM_FishermanOld_Grey","ZmbM_FishermanOld_Red","ZmbM_JournalistSkinny",
				 "ZmbF_JournalistNormal_Blue","ZmbF_JournalistNormal_Green","ZmbF_JournalistNormal_Red","ZmbF_JournalistNormal_White",
				 "ZmbM_ParamedicNormal_Blue","ZmbM_ParamedicNormal_Green","ZmbM_ParamedicNormal_Red",
				 "ZmbM_ParamedicNormal_Black","ZmbF_ParamedicNormal_Blue","ZmbF_ParamedicNormal_Green",
				 "ZmbF_ParamedicNormal_Red","ZmbM_HikerSkinny_Blue","ZmbM_HikerSkinny_Green","ZmbM_HikerSkinny_Yellow",
				 "ZmbF_HikerSkinny_Blue","ZmbF_HikerSkinny_Grey","ZmbF_HikerSkinny_Green","ZmbF_HikerSkinny_Red",
				 "ZmbM_HunterOld_Autumn","ZmbM_HunterOld_Spring","ZmbM_HunterOld_Summer","ZmbM_HunterOld_Winter",
				 "ZmbF_SurvivorNormal_Blue","ZmbF_SurvivorNormal_Orange","ZmbF_SurvivorNormal_Red",
				 "ZmbF_SurvivorNormal_White","ZmbM_SurvivorDean_Black","ZmbM_SurvivorDean_Blue","ZmbM_SurvivorDean_Grey",
				 "ZmbM_PolicemanFat","ZmbF_PoliceWomanNormal",
				 "ZmbM_PolicemanSpecForce","ZmbM_SoldierNormal",
				 "ZmbM_usSoldier_normal_Woodland","ZmbM_usSoldier_normal_Desert","ZmbM_CommercialPilotOld_Blue",
				 "ZmbM_CommercialPilotOld_Olive","ZmbM_CommercialPilotOld_Brown","ZmbM_CommercialPilotOld_Grey",
				 "ZmbM_PatrolNormal_PautRev","ZmbM_PatrolNormal_Autumn","ZmbM_PatrolNormal_Flat","ZmbM_PatrolNormal_Summer",
				 "ZmbM_JoggerSkinny_Blue","ZmbM_JoggerSkinny_Green","ZmbM_JoggerSkinny_Red","ZmbF_JoggerSkinny_Blue",
				 "ZmbF_JoggerSkinny_Brown","ZmbF_JoggerSkinny_Green","ZmbF_JoggerSkinny_Red","ZmbM_MotobikerFat_Beige",
				 "ZmbM_MotobikerFat_Black","ZmbM_MotobikerFat_Blue","ZmbM_VillagerOld_Blue","ZmbM_VillagerOld_Green",
				 "ZmbM_VillagerOld_White","ZmbM_SkaterYoung_Blue","ZmbM_SkaterYoung_Brown","ZmbM_SkaterYoung_Green",
				 "ZmbM_SkaterYoung_Grey","ZmbF_SkaterYoung_Brown","ZmbF_SkaterYoung_Striped","ZmbF_SkaterYoung_Violet",
				 "ZmbF_DoctorSkinny","ZmbF_BlueCollarFat_Blue","ZmbF_BlueCollarFat_Green",
				 "ZmbF_BlueCollarFat_Red","ZmbF_BlueCollarFat_White","ZmbF_MechanicNormal_Beige","ZmbF_MechanicNormal_Green",
				 "ZmbF_MechanicNormal_Grey","ZmbF_MechanicNormal_Orange","ZmbM_MechanicSkinny_Blue","ZmbM_MechanicSkinny_Grey",
				 "ZmbM_MechanicSkinny_Green","ZmbM_MechanicSkinny_Red","ZmbM_ConstrWorkerNormal_Beige",
				 "ZmbM_ConstrWorkerNormal_Black","ZmbM_ConstrWorkerNormal_Green","ZmbM_ConstrWorkerNormal_Grey",
				 "ZmbM_HeavyIndustryWorker","ZmbM_OffshoreWorker_Green","ZmbM_OffshoreWorker_Orange","ZmbM_OffshoreWorker_Red",
				 "ZmbM_OffshoreWorker_Yellow","ZmbF_NurseFat","ZmbM_HandymanNormal_Beige",
				 "ZmbM_HandymanNormal_Blue","ZmbM_HandymanNormal_Green","ZmbM_HandymanNormal_Grey","ZmbM_HandymanNormal_White",
				 "ZmbM_DoctorFat","ZmbM_Jacket_beige","ZmbM_Jacket_black","ZmbM_Jacket_blue","ZmbM_Jacket_bluechecks",
				 "ZmbM_Jacket_brown","ZmbM_Jacket_greenchecks","ZmbM_Jacket_grey","ZmbM_Jacket_khaki","ZmbM_Jacket_magenta","ZmbM_Jacket_stripes",
				 "ZmbF_PatientOld","ZmbM_PatientSkinny","ZmbF_ShortSkirt_beige",
				 "ZmbF_ShortSkirt_black","ZmbF_ShortSkirt_brown","ZmbF_ShortSkirt_green","ZmbF_ShortSkirt_grey","ZmbF_ShortSkirt_checks",
				 "ZmbF_ShortSkirt_red","ZmbF_ShortSkirt_stripes","ZmbF_ShortSkirt_white","ZmbF_ShortSkirt_yellow",
				 "ZmbF_VillagerOld_Blue","ZmbF_VillagerOld_Green","ZmbF_VillagerOld_Red","ZmbF_VillagerOld_White","ZmbM_Soldier","ZmbM_SoldierAlice",
				 "ZmbM_SoldierHelmet","ZmbM_SoldierVest","ZmbM_SoldierAliceHelmet","ZmbM_SoldierVestHelmet",
				 "ZmbF_MilkMaidOld_Beige","ZmbF_MilkMaidOld_Black","ZmbF_MilkMaidOld_Green","ZmbF_MilkMaidOld_Grey",
				 "ZmbM_priestPopSkinny","ZmbM_ClerkFat_Brown","ZmbM_ClerkFat_Grey","ZmbM_ClerkFat_Khaki","ZmbM_ClerkFat_White",
				 "ZmbF_Clerk_Normal_Blue","ZmbF_Clerk_Normal_White","ZmbF_Clerk_Normal_Green","ZmbF_Clerk_Normal_Red" };
	}

	// -----------------------------------------------------------
	// Expansion String GetItemDisplayNameWithType
	// -----------------------------------------------------------
	static string GetItemDisplayNameWithType( string type_name, map<string, string> cache = NULL )
	{
		string cfg_name;
		string cfg_name_path;
		
		if ( cache && cache.Find( type_name, cfg_name ) )
		{
			return cfg_name;
		}
		else if ( GetGame().ConfigIsExisting( CFG_WEAPONSPATH + " " + type_name ) )
		{
			cfg_name_path = CFG_WEAPONSPATH + " " + type_name + " displayName";
			GetGame().ConfigGetText( cfg_name_path, cfg_name );
		} 
		else if ( GetGame().ConfigIsExisting( CFG_VEHICLESPATH + " " + type_name ) )
		{
			cfg_name_path = CFG_VEHICLESPATH + " " + type_name + " displayName";
			GetGame().ConfigGetText( cfg_name_path, cfg_name );
		} 
		else if ( GetGame().ConfigIsExisting( CFG_MAGAZINESPATH + " " + type_name ) )
		{
			cfg_name_path = CFG_MAGAZINESPATH + " " + type_name + " displayName";
			GetGame().ConfigGetText( cfg_name_path, cfg_name );
		}
		if (!cfg_name)
		{
			cfg_name = type_name;
		}

		if ( cache )
		{
			cache.Insert( type_name, cfg_name );
		}
		
		return cfg_name;
	}

	// -----------------------------------------------------------
	// Expansion String GetItemDescriptionWithType
	// -----------------------------------------------------------
	static string GetItemDescriptionWithType( string type_name, map<string, string> cache = NULL )
	{
		string cfg_des;
		string cfg_des_path;

		if ( cache && cache.Find( type_name, cfg_des ) )
		{
			return cfg_des;
		}
		else if ( GetGame().ConfigIsExisting( CFG_WEAPONSPATH + " " + type_name ) )
		{
			cfg_des_path = CFG_WEAPONSPATH + " " + type_name + " descriptionShort";
			GetGame().ConfigGetText( cfg_des_path, cfg_des );
		} 
		else if ( GetGame().ConfigIsExisting( CFG_VEHICLESPATH + " " + type_name ) )
		{
			cfg_des_path = CFG_VEHICLESPATH + " " + type_name + " descriptionShort";
			GetGame().ConfigGetText( cfg_des_path, cfg_des );
		} 
		else if ( GetGame().ConfigIsExisting( CFG_MAGAZINESPATH + " " + type_name ) )
		{
			cfg_des_path = CFG_MAGAZINESPATH + " " + type_name + " descriptionShort";
			GetGame().ConfigGetText( cfg_des_path, cfg_des );
		}

		if ( cache )
		{
			cache.Insert( type_name, cfg_des );
		}
		
		return cfg_des;
	}
	
	static float GetBoundingRadius(string className)
	{
		vector minMax[2];
		return ClippingInfo(className, minMax);
	}

	static float ClippingInfo(string className, out vector minMax[2])
	{
		float radius;

		Object obj = GetGame().CreateObjectEx(className, "0 0 0", ECE_LOCAL);

		if (obj)
		{
			radius = obj.ClippingInfo(minMax);
			GetGame().ObjectDelete(obj);
		}

		return radius;
	}

	//! If collision box does not exist, minMax is set to clipping info (bounding box)
	static bool GetCollisionBox(string className, out vector minMax[2])
	{
		bool hasCollisionBox;

		Object obj = GetGame().CreateObjectEx(className, "0 0 0", ECE_LOCAL);

		if (obj)
		{
			hasCollisionBox = obj.GetCollisionBox(minMax);
			if (!hasCollisionBox)
				obj.ClippingInfo(minMax);
			GetGame().ObjectDelete(obj);
		}

		return hasCollisionBox;
	}

	static bool HasQuantity(string item_name)
	{
		float min;
		float max;

		if (GetGame().IsKindOf(item_name, "Magazine_Base"))
		{
			max = GetGame().ConfigGetInt("CfgMagazines " + item_name + " count");
		}
		else
		{
			min = GetGame().ConfigGetInt("CfgVehicles " + item_name + " varQuantityMin");
			max = GetGame().ConfigGetFloat("CfgVehicles " + item_name + " varStackMax");
			if (!max)
				max = GetGame().ConfigGetInt("CfgVehicles " + item_name + " varQuantityMax");
		}

		return max - min > 0;
	}

	static bool ItemExists(string type_name)
	{
		return GetGame().ConfigIsExisting( CFG_VEHICLESPATH + " " + type_name ) || GetGame().ConfigIsExisting( CFG_WEAPONSPATH + " " + type_name ) || GetGame().ConfigIsExisting( CFG_MAGAZINESPATH + " " + type_name );
	}

	//! TODO: Maybe use CF_Byte after next CF update?
	static TIntArray IntToByteArray(int n)
	{
		TIntArray bytes();

		for (int i = 0; i < 4; i++)
			bytes.Insert((n >> (24 - i * 8)) & 255);

		return bytes;
	}

	//! TODO: Maybe remove and use CF_Encoding::ToHex instead after next CF update?
	static string ByteArrayToHex(TIntArray bytes)
	{
		string hex;

		foreach (int byte: bytes)
			hex += ByteToHex(byte);

		return hex;
	}

	static int ByteArrayToInt(TIntArray bytes)
	{
		int n;

		for (int i = 0; i < 4; i++)
			n |= bytes[i] << (24 - i * 8);

		return n;
	}

	//! TODO: Maybe remove and use CF_Byte::ToHex instead after next CF update?
	static string ByteToHex(int byte)
	{
		return BASE16[(int) Math.Floor(byte / 16)] + BASE16[byte % 16];
	}

	static string IntToHex(int n)
	{
		return ByteArrayToHex(IntToByteArray(n));
	}

	static int HexToInt(string hex)
	{
		hex.ToUpper();

		int n;

		for (int i = 0; i < hex.Length(); i++)
		{
			//! Snatched from Dabs Framework
			n += BASE16.IndexOf(hex[i]) * Math.Pow(16, Math.AbsInt(i - hex.Length() + 1));
		}

		return n;
	}

	//! TODO: These should be moved to ExpansionColor
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

	static bool StringArrayContainsIgnoreCase(TStringArray elements, string search)
	{
		search.ToLower();

		foreach (string element: elements)
		{
			element.ToLower();
			if (element == search)
				return true;
		}

		return false;
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
	// Expansion GetDateTime
	// ------------------------------------------------------------
	static string GetDateTime()
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;

		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);

		string result = year.ToStringLen(4) + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2) + "_" + hour.ToStringLen(2) + "-" + minute.ToStringLen(2) + "-" + second.ToStringLen(2);

		return result;
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
	// Expansion FormatTime (milliseconds)
	// ------------------------------------------------------------
	static string FormatTime( float time, bool include_ms = true )
	{
		return FormatTimestamp( time / 1000, include_ms );
	}


	// ------------------------------------------------------------
	// Expansion FormatTimestamp (seconds)
	// ------------------------------------------------------------
	static string FormatTimestamp( float time, bool include_ms = true )
	{
		int hours = (int) time / 3600;
		time -= hours * 3600;
		int minutes = (int) time / 60;
		time -= minutes * 60;
		int seconds = (int) time;
		
		string timestring = hours.ToStringLen(2) + ":" + minutes.ToStringLen(2) + ":" + seconds.ToStringLen(2);
		
		if ( include_ms )
		{
			time -= seconds;
			int ms = time * 1000;
			timestring += "." + ms.ToStringLen(3);
		}

		return timestring;
	}

	// ------------------------------------------------------------
	// Expansion GetTimestamp
	// ------------------------------------------------------------
	static string GetTimestamp()
	{
		if ( GetDayZGame() )
		{
			//! Accurate, including milliseconds
			return FormatTimestamp( GetDayZGame().ExpansionGetStartTime() + GetDayZGame().GetTickTime() );
		} else
		{
			//! Next best thing
			return GetTime();
		}
	}


	static string GetTimeString( float total_time )
	{
		string time_string;

		if( total_time < 0 )
		{
			time_string =  "0 #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";
			return time_string;
		}

		int time_seconds = total_time; 									//convert total time to int
		
		int hours = time_seconds / 3600;
		if ( hours > 0 )
		{
			time_string += GetValueString( hours ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";			//hours
		}
		
		time_string += " ";												//separator
		
		int minutes = ( time_seconds % 3600 ) / 60;
		time_string += GetValueString( minutes ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_MINUTES";			//minutes
		
		return time_string;
	}

	static string GetWeightString(int weight, bool grams_only = false)
	{
		string weight_string;
		
		if( weight < 0 )
		{
			weight_string =  "0 #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_GRAMS";
			return weight_string;
		}
		
		int weight_grams = weight;
		
		int kilograms = weight_grams / 1000;
		if ( kilograms > 0 && !grams_only )
		{
			weight_string = GetValueString( kilograms ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_KG";				//kilograms
		}
		else
		{
			weight_string = GetValueString( weight_grams ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_GRAMS";	//grams
		}
		
		return weight_string;
	}

	static string GetDistanceString( float total_distance, bool meters_only = false )
	{
		string distance_string;
		
		if( total_distance < 0 )
		{
			distance_string =  "0 #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";
			return distance_string;
		}

		int distance_meters = total_distance;
		
		int kilometers = distance_meters / 1000;
		if ( kilometers > 0 && !meters_only )
		{
			distance_string = GetValueString( kilometers ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_KM";				//kilometers
		}
		else
		{
			distance_string = GetValueString( distance_meters ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";	//meters
		}
		
		return distance_string;
	}

	static string GetValueString( float total_value )
	{
		if( total_value < 0 )
			return "0";

		int value = total_value;
		string out_string;
		
		if ( value >= 1000 )
		{
			string value_string = value.ToString();
			
			int count;		
			int first_length = value_string.Length() % 3;		//calculate position of the first separator
			if ( first_length > 0 )
			{
				count = 3 - first_length;
			}
			
			for ( int i = 0; i < value_string.Length(); ++i )
			{
				out_string += value_string.Get( i );
				count ++;
				
				if ( count >= 3 )
				{
					out_string += " ";			//separator
					count = 0;
				}
			}
		}
		else
		{
			out_string = value.ToString();
		}
		
		return out_string.Trim();
	}

	// ------------------------------------------------------------
	// Expansion GetWeightedRandom
	// ------------------------------------------------------------
	//! Returns an index into the 'weights' array, or -1 if all weights are zero
	static int GetWeightedRandom( array< float > weights )
	{
		float weightSum = 0;
		for ( int i = 0; i < weights.Count(); i++ )
		{
			weightSum += weights[i];
		}

		if ( weightSum == 0 )
			return -1;

		float rnd = Math.RandomFloat( 0, weightSum );
		int index = 0;
		int count = weights.Count();
		while ( index < count )
		{
			if ( rnd < weights[ index ] )
			{
				return index;
			}

			rnd -= weights[index++];
		}

		//! Should never get here
		return -1;
	}

	static array< string > FindFilesInLocation( string folder, string ext = "" )
	{
		array< string > files = new array< string >;
		if (!FileExist(folder))
			return files;
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( folder + "*" + ext, fileName, fileAttr, 0 );
		if ( findFileHandle )
		{
			if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
			{
				files.Insert( fileName );
			}
			
			while ( FindNextFile( findFileHandle, fileName, fileAttr ) )
			{
				if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
				{
					files.Insert( fileName );
				}
			}
		}
		CloseFindFile( findFileHandle );
		return files;
	}

	static bool MakeDirectoryRecursive(string path)
	{
		TStringArray parts();
		path.Split("\\", parts);
		path = "";
		foreach (string part: parts)
		{
			path += part + "\\";
			if (part.IndexOf(":") == part.Length() - 1)
				continue;
			if (!FileExist(path) && !MakeDirectory(path))
			{
				EXPrint("ERROR: MakeDirectoryRecursive " + path + " failed");
				return false;
			}
		}
		return true;
	}

	//! Copies the directory tree rooted in srcDir to dstDir.
	//! If `ext` is given, limit to files and directories with that extension.
	//! If `move` is true and copying is successful, removes srcDir afterwards if empty.
	static bool CopyDirectoryTree(string srcDir, string dstDir, string ext = "", bool move = false)
	{
		ExpansionString srcDirEx = new ExpansionString(srcDir);
		if (srcDirEx.LastIndexOf("\\") != srcDir.Length() - 1)
			srcDir += "\\";

		ExpansionString dstDirEx = new ExpansionString(dstDir);
		if (dstDirEx.LastIndexOf("\\") != dstDir.Length() - 1)
			dstDir += "\\";

		if (!FileExist(dstDir) && !MakeDirectoryRecursive(dstDir))
			return false;

		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile(srcDir + "*" + ext, fileName, fileAttr, 0);

		if (!findFileHandle)
		{
			EXPrint("ERROR: CopyDirectoryTree " + srcDir + " " + dstDir + " findFileHandle is NULL");
			return false;
		}

		if (fileName.Length() > 0 && !CopyFileOrDirectoryTree(srcDir + fileName, dstDir + fileName, ext, move, fileAttr))
		{
			EXPrint("ERROR: CopyDirectoryTree " + srcDir + fileName + " " + dstDir + fileName + " ext " + ext + " move " + move + " failed");
			CloseFindFile(findFileHandle);
			return false;
		}

		while (FindNextFile(findFileHandle, fileName, fileAttr))
		{
			if (fileName.Length() > 0 && !CopyFileOrDirectoryTree(srcDir + fileName, dstDir + fileName, ext, move, fileAttr))
			{
				EXPrint("ERROR: CopyDirectoryTree " + srcDir + fileName + " " + dstDir + fileName + " ext " + ext + " move " + move + " failed");
				CloseFindFile(findFileHandle);
				return false;
			}
		}

		CloseFindFile(findFileHandle);

		if (move && (!ext || !FindFilesInLocation(srcDir).Count()) && !DeleteFile(srcDir))
		{
			EXPrint("ERROR: CopyDirectoryTree remove src failed");
			return false;
		}

		return true;
	}

	//! @note if copying a directory, make sure paths end with "\\" or provide fileAttr parameter
	static bool CopyFileOrDirectoryTree(string srcPath, string dstPath, string ext = "", bool move = false, FileAttr fileAttr = 0)
	{
		bool isDir = (fileAttr & FileAttr.DIRECTORY);
		ExpansionString srcPathEx = new ExpansionString(srcPath);
		if (!isDir && srcPathEx.LastIndexOf("\\") == srcPath.Length() - 1)
			isDir = true;

		if (isDir)
		{
			if (!CopyDirectoryTree(srcPath, dstPath, ext, move))
				return false;
		}
		else
		{
			ExpansionString dstPathEx = new ExpansionString(dstPath);
			string dstDir = dstPath.Substring(0, dstPathEx.LastIndexOf("\\"));
			if (!FileExist(dstDir) && !MakeDirectoryRecursive(dstDir))
				return false;

			if (!CopyFile(srcPath, dstPath))
			{
				EXPrint("ERROR: CopyFileOrDirectoryTree copy failed");
				return false;
			}

			if (move && !DeleteFile(srcPath))
			{
				EXPrint("ERROR: CopyFileOrDirectoryTree remove src failed");
				return false;
			}
		}

		return true;
	}

	#ifdef ENFUSION_AI_PROJECT
	static TStringArray GetWorkingAIClasses()
	{
		return { "eAI_SurvivorM_Mirek", "eAI_SurvivorM_Denis", "eAI_SurvivorM_Boris",
		"eAI_SurvivorM_Cyril", "eAI_SurvivorM_Elias", "eAI_SurvivorM_Francis", "eAI_SurvivorM_Guo",
		"eAI_SurvivorM_Hassan", "eAI_SurvivorM_Indar", "eAI_SurvivorM_Jose",
		"eAI_SurvivorM_Kaito", "eAI_SurvivorM_Lewis", "eAI_SurvivorM_Manua",
		"eAI_SurvivorM_Niki", "eAI_SurvivorM_Oliver", "eAI_SurvivorM_Peter",
		"eAI_SurvivorM_Quinn", "eAI_SurvivorM_Rolf", "eAI_SurvivorM_Seth",
		"eAI_SurvivorM_Taiki", "eAI_SurvivorF_Linda", "eAI_SurvivorF_Maria",
		"eAI_SurvivorF_Frida", "eAI_SurvivorF_Gabi", "eAI_SurvivorF_Helga",
		"eAI_SurvivorF_Irena", "eAI_SurvivorF_Judy", "eAI_SurvivorF_Keiko",
		"eAI_SurvivorF_Lina", "eAI_SurvivorF_Naomi"};
	}
	#endif

	static string IntToCurrencyString(int number, string separator, bool shorten = false)
	{
		string moneyReversed = "";
		string strNumber = number.ToString();
		int processedCount = 0;
		string money = "";
		int i;
		float dec = number;

		if (shorten)
		{
			if (dec >= 1000000)
			{
				dec /= 1000000;
				dec = Math.Round(dec * 100) / 100;
				return dec.ToString() + "M";
			}
			else if (dec >= 1000)
			{
				dec /= 1000;
				dec = Math.Round(dec * 100) / 100;
				return dec.ToString() + "K";
			}
		}
		
		for (i = (strNumber.Length() - 1); i >= 0; i--)
		{
			moneyReversed += strNumber[i];
			processedCount += 1;
			if ((processedCount % 3) == 0 && processedCount < strNumber.Length())
			{
				moneyReversed += separator;
			}
		}

		for (i = (moneyReversed.Length() - 1); i >= 0; i--)
		{
			money += moneyReversed[i];
		}

		return money;
	}

	static bool Key_SHIFT()
	{
		return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
	}

	static bool Key_CTRL()
	{
		return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
	}

	static bool Key_ALT()
	{
		return( ( KeyState( KeyCode.KC_LMENU ) > 0 ) || ( KeyState( KeyCode.KC_RMENU ) > 0 ) );
	}

	static bool Key_Y()
	{
		return KeyState(KeyCode.KC_Y) > 0;
	}

	static bool Key_Z()
	{
		return KeyState(KeyCode.KC_Z) > 0;
	}
			
	static bool Key_C()
	{
		return KeyState(KeyCode.KC_C) > 0;
	}

	static bool Key_B()
	{
		return KeyState(KeyCode.KC_B) > 0;
	}

	static bool Key_S()
	{
		return KeyState(KeyCode.KC_S) > 0;
	}

	static bool Key_UP()
	{
		return KeyState(KeyCode.KC_UP) > 0;
	}

	static bool Key_DOWN()
	{
		return KeyState(KeyCode.KC_DOWN) > 0;
	}

	static bool Key_LEFT()
	{
		return KeyState(KeyCode.KC_LEFT) > 0;
	}

	static bool Key_RIGHT()
	{
		return KeyState(KeyCode.KC_RIGHT) > 0;
	}
	
	static bool INPUT_FORWARD()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveForward", false );
	}
	
	static bool INPUT_BACK()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveBack", false );
	}
	
	static bool INPUT_LEFT()
	{
   		return GetGame().GetInput().LocalPress( "UAMoveLeft", false );
	}
	
	static bool INPUT_RIGHT()
	{		
   		return GetGame().GetInput().LocalPress( "UAMoveRight", false );
	}

	static bool INPUT_GETOVER()
	{
   		return GetGame().GetInput().LocalPress( "UAGetOver", false );
	}

	static bool INPUT_STANCE()
	{
   		return GetGame().GetInput().LocalPress( "UAStance", false );
	}

	static vector GetSurfacePosition(vector position)
	{
		return GetSurfacePosition(position[0], position[2]);
	}

	static vector GetSurfacePosition(float x, float z)
	{
		return Vector(x, GetGame().SurfaceY(x, z), z);
	}

	static float GetSurfaceWaterDepth(vector position)
	{
		return GetSurfaceWaterDepth(position[0], position[2]);
	}

	static float GetSurfaceWaterDepth(float x, float z)
	{
		return GetGame().GetWaterDepth(GetSurfacePosition(x, z));
	}
	
	//! Get ground surface position or water surface position, whichever has higher elevation
	static vector GetSurfaceWaterPosition(vector position)
	{
		return GetSurfaceWaterPosition(position[0], position[2]);
	}

	//! Get ground surface position or water surface position, whichever has higher elevation
	static vector GetSurfaceWaterPosition(float x, float z)
	{
		vector position = GetSurfacePosition(x, z);
		float waterDepth = GetGame().GetWaterDepth(position);
		if (waterDepth > 0)
			position[1] = position[1] + waterDepth;
		return position;
	}

	static bool SurfaceIsWater(vector position)
	{
		string type;
		GetGame().SurfaceGetType3D(position[0], position[1] + 0.1, position[2], type);
		if (type.Contains("water"))
			return true;

		return SurfaceIsWater(position[0], position[2]);
	}

	static bool SurfaceIsWater(float x, float z)
	{
		return GetGame().SurfaceIsSea(x, z) || GetGame().SurfaceIsPond(x, z);
	}

	//! Only kept for compatibility with old mods, remove after next update
	void LoadFlagTextures()
	{
		Error("DEPRECATED - please use ExpansionFlagTextures::Load instead!");
	}

	//! Only kept for compatibility with old mods, remove after next update
	void AddFlagTexture( string path, string name )
	{
		Error("DEPRECATED - please use ExpansionFlagTextures::Add instead!");
	}

	//! Only kept for compatibility with old mods, remove after next update
	bool RemoveFlagTexture( string path )
	{
		Error("DEPRECATED - please use ExpansionFlagTextures::Remove instead!");
		return false;
	}
}
