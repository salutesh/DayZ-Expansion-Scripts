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

static void Expansion_Error(string s, inout bool check = false)
{
	if (!check)
	{
		check = true;
		Error(s);
	}
}

class EXTee
{
	FileHandle m_File;
	autoptr TStringArray m_PrintBuffer = {};
	int m_BufferLength;

	void EXTee(string fileName, FileMode mode)
	{
		m_File = OpenFile(fileName, mode);
	}

	void ~EXTee()
	{
		Flush();
		if (m_File)
			CloseFile(m_File);
	}

	bool CanWrite()
	{
		if (m_File)
			return true;

		return false;
	}

	void WriteLine(string str, bool print = true)
	{
		if (m_File)
			FPrintln(m_File, str);

		if (print)
		{
			int strLen = str.Length();
			//! @note max script log line length 256 characters
			if (m_BufferLength + strLen >= 256)
				Flush();
			m_PrintBuffer.Insert(str);
			m_BufferLength += strLen;
		}
	}

	void Flush()
	{
		Print(ExpansionString.JoinStrings(m_PrintBuffer, "\n"));
		m_PrintBuffer.Clear();
		m_BufferLength = 0;
	}
}

class EXError
{
	static ref CF_Date s_Start = CF_Date.Now();
	static string s_BaseName;
	static string s_FileName;

	static void Log(Class instance, string msg, TStringArray stack, inout bool check = false)
	{
		if (check)
			return;

		check = true;

		if (!s_BaseName)
		{
			s_BaseName = string.Format("error_%1.log", s_Start.GetISODateTime("_", "-"));
			s_FileName = "$profile:" + s_BaseName;
		}

		string now = CF_Date.Now().Format(CF_Date.DATETIME);

		//! Try to mimic vanilla error output

		EXTee tee;

		if (FileExist(s_FileName))
		{
			tee = new EXTee(s_FileName, FileMode.APPEND);
		}
		else
		{
			tee = new EXTee(s_FileName, FileMode.WRITE);

			tee.WriteLine("---------------------------------------------", false);
			tee.WriteLine(string.Format("Log %1 started at %2", s_BaseName, now), false);
			tee.WriteLine("", false);
		}

		tee.WriteLine("", false);
		tee.WriteLine("", false);
		tee.WriteLine("------------------------------------", false);
		tee.WriteLine(string.Format("%1, %2", GetMachineName(), now), false);
		tee.WriteLine("", false);

		tee.WriteLine(msg);

		if (instance)
		{
			tee.WriteLine(string.Format("Class:      '%1'", instance.ClassName()));
			IEntity entity;
			if (Class.CastTo(entity, instance))
				tee.WriteLine(string.Format("Entity id:%1", entity.GetID()));
			tee.WriteLine("");
		}

		tee.WriteLine(string.Format("Function: '%1'", stack[0].Substring(0, stack[0].IndexOf("("))));

		tee.WriteLine("Stack trace:");

		bool print = true;
		foreach (string line: stack)
		{
			line.Replace(" : ", ":");
			int index = line.IndexOf(")") + 2;
			tee.WriteLine(line.Substring(index, line.Length() - index), print);
			print = false;
		}
	}
}

// -----------------------------------------------------------
// Expansion EXPrint
// -----------------------------------------------------------
static void EXPrint( string s, string prefix = " [EXPANSION DEBUG]: " )
{
	PrintFormat("%1%2%3", ExpansionStatic.GetISOTime(), prefix, s );
}

static void EXPrint( Class instance, string s, string prefix = " [EXPANSION DEBUG]: " )
{
	PrintFormat("%1%2%3 %4", ExpansionStatic.GetISOTime(), prefix, instance.ToString(), s );
}

// -----------------------------------------------------------
// Expansion EXLogPrint
// -----------------------------------------------------------
static void EXLogPrint( string s )
{
	EXPrint( s, " [EXPANSION LOG]: " );
}

static void EXLogPrint( Class instance, string s )
{
	EXPrint( instance, s, " [EXPANSION LOG]: " );
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

//! Prints a hitch warning if elapsed time since startTime is above threshold (DEPRECATED, use EXHitch)
static void EXPrintHitch( string msgPrefix, float startTime, float threshold = 0.0125 )
{
	float elapsedTime = GetGame().GetTickTime() - startTime;
	if (elapsedTime > threshold)
		EXPrint(msgPrefix + "HITCH: " + (elapsedTime * 1000) + "ms");
}

class EXTimeIt
{
	protected int m_Ticks;

	void EXTimeIt()
	{
		m_Ticks = TickCount(0);
	}

	int GetElapsed()
	{
		return TickCount(m_Ticks);
	}
}

class EXHitch: EXTimeIt
{
	protected int m_Threshold;
	protected string m_MsgPrefix;

	void EXHitch(string msgPrefix, int threshold = 125000)
	{
		m_Threshold = threshold;
		m_MsgPrefix = msgPrefix;
	}

	void ~EXHitch()
	{
		int elapsed = TickCount(m_Ticks);
		if (elapsed > m_Threshold)
			PrintFormat("%1 %2HITCH: %3ms", ExpansionStatic.GetISOTime(), m_MsgPrefix, (elapsed / 10000.0).ToString());
	}
}

enum ExpansionVectorToString
{
	Plain,
	Beautify,
	Labels
}

//! Bitmask
enum ExpansionFindFileMode
{
	FILES = 1,
	DIRECTORIES = 2
}

class ExpansionStatic
{
	static const string BASE16 = "0123456789ABCDEF";
	static const typename NULLTYPE;

	static string BitmaskEnumToString(typename e, int enumValue)
	{
		int cnt = e.GetVariableCount();
		int val;

		string ret;
		for (int i = 0; i < cnt; i++)
		{
			if (e.GetVariableType(i) == int && e.GetVariableValue(null, i, val) && (enumValue & val) == val)
			{
				if (ret.Length() > 0)
					ret += "|";
				ret += e.GetVariableName(i);
			}
		}

		if (ret.Length() > 0)
			return ret;

		return "unknown";
	}

	/**
	 * @brief Set value to enum value if it exists
	 * 
	 * @param e typename/enum
	 * @param enumName name
	 * @param [out] value (if found)
	 * 
	 * @return true if found, false if not
	 * 
	 * @note use this instead of typename.StringToEnum if you need to know if value exists or not
	 */
	static bool StringToEnumEx(typename e, string enumName, out int value)
	{
		int count = e.GetVariableCount();
	   
		for (int i = 0; i < count; i++)
		{
			if (e.GetVariableType(i) == int && e.GetVariableName(i) == enumName && e.GetVariableValue(null, i, value))
				return true;
		}
		
		return false;
	}

	// -----------------------------------------------------------
	// Expansion String FloatToString
	// -----------------------------------------------------------
	//! @note unlike float::ToString(), returns non-scientific notation for any number
	static string FloatToString(float nmb)
	{
		int i = nmb;
		float f = i;

		if (nmb == f)
			return i.ToString();

		float frac = nmb - i;
		string tmp = frac.ToString();

		if (tmp.Contains("e"))
		{
			TStringArray parts = {};
			tmp.Split("e", parts);

			string real = parts[0];
			real.Replace(".", "");

			if (i < 0)
				real = real.Substring(1, real.Length() - 1);

			int count = parts[1].Substring(1, parts[1].Length() - 1).ToInt();
			string zeros;
			while (--count)
			{
				zeros += "0";
			}

			string str = "0." + zeros + real;
			if (i < 0)
				str = "-" + str;

			return str;
		}

		return i.ToString() + tmp.Substring(1, tmp.Length() - 1);
	}

	static string FloatFixedDecimals(float f, int decimals = 4)
	{
		string str = f.ToString();
		int i = str.IndexOf(".");

		if (i > -1)
		{
			while (str.Length() - i < decimals + 1)
			{
				str += "0";
			}

			int len = str.Length() - i - 1;
			if (len > decimals)
				str = str.Substring(0, i + decimals + 1);
		}

		return str;
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
				 "ZmbF_SurvivorNormal_White",
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
				 "ZmbF_VillagerOld_Blue","ZmbF_VillagerOld_Green","ZmbF_VillagerOld_Red","ZmbF_VillagerOld_White",
				 "ZmbF_MilkMaidOld_Beige","ZmbF_MilkMaidOld_Black","ZmbF_MilkMaidOld_Green","ZmbF_MilkMaidOld_Grey",
				 "ZmbM_priestPopSkinny","ZmbM_ClerkFat_Brown","ZmbM_ClerkFat_Grey","ZmbM_ClerkFat_Khaki","ZmbM_ClerkFat_White",
				 "ZmbF_Clerk_Normal_Blue","ZmbF_Clerk_Normal_White","ZmbF_Clerk_Normal_Green","ZmbF_Clerk_Normal_Red",
				 "ZmbM_NBC_Grey", "ZmbM_NBC_Yellow", "ZmbM_PolicemanSpecForce_Heavy", "ZmbM_Santa", "ZmbM_usSoldier_Heavy_Woodland",
				 "ZmbM_usSoldier_Officer_Desert" };
	}

	// -----------------------------------------------------------
	// Expansion String GetItemDisplayNameWithType
	// -----------------------------------------------------------
	//! @note this is for the rare cases where we only have a classname, not an object.
	//! If we have an object, should use <object>.GetDisplayName() instead.
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
	//! @note this is for the rare cases where we only have a classname, not an object.
	//! If we have an object, should use <object>.ConfigGetString('descriptionShort') instead.
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
			hasCollisionBox = GetCollisionBox(obj, minMax);
			GetGame().ObjectDelete(obj);
		}

		return hasCollisionBox;
	}

	static bool GetCollisionBox(Object obj, out vector minMax[2])
	{
		bool hasCollisionBox = obj.GetCollisionBox(minMax);
		if (!hasCollisionBox)
			obj.ClippingInfo(minMax);
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

	static bool TypeExists(string type_name)
	{
		typename nullType;
		return type_name.ToType() != nullType;
	}

	//! Inheritance check based on rvConfig class name or EnforceScript typename
	static bool IsAnyOf(string className, TStringArray parentNames)
	{
		foreach (string parentName: parentNames)
		{
			if (Is(className, parentName))
				return true;
		}
		return false;
	}

	static bool IsAnyOf(string className, TTypenameArray parent_types)
	{
		foreach (typename parent_type: parent_types)
		{
			if (Is(className, parent_type))
				return true;
		}
		return false;
	}

	//! Inheritance check based on rvConfig class name or EnforceScript typename
	//! @note `check_script_inheritance` is a legacy parameter and no longer used (script inheritance is always checked).
	static bool IsAnyOf(Object obj, TStringArray parentNames, bool check_script_inheritance = false)
	{
		foreach (string parentName: parentNames)
		{
			typename parentType = parentName.ToType();
			if (parentType && obj.IsInherited(parentType))
				return true;
			if (g_Game.ObjectIsKindOf(obj, parentName))
				return true;
		}
		return false;
	}

	//! Inheritance check based on typename
	static bool IsAnyOf(typename type, TTypenameArray parent_types)
	{
		foreach (typename parent_type: parent_types)
		{
			if (type.IsInherited(parent_type))
				return true;
		}
		return false;
	}

	//! Inheritance check based on instance
	static bool IsAnyOf(Class instance, TTypenameArray parent_types)
	{
		foreach (typename parent_type: parent_types)
		{
			if (instance.IsInherited(parent_type))
				return true;
		}
		return false;
	}

	//! Inheritance check based on rvConfig class name or EnforceScript typename
	static bool Is(string className, string parentName)
	{
		typename classType = className.ToType();
		if (classType)
		{
			typename parentType = parentName.ToType();

			if (parentType && classType.IsInherited(parentType))
				return true;
		}

		return g_Game.IsKindOf(className, parentName);
	}

	//! Inheritance check based on rvConfig class name or EnforceScript typename
	static bool Is(Object obj, string parentName)
	{
		typename parentType = parentName.ToType();

		if (parentType && obj.IsInherited(parentType))
			return true;

		return g_Game.ObjectIsKindOf(obj, parentName);
	}

	//! Inheritance check based on rvConfig class name or EnforceScript typename
	static bool Is(string className, typename parentType)
	{
		typename classType = className.ToType();
		if (classType && classType.IsInherited(parentType))
			return true;

		return g_Game.IsKindOf(className, parentType.ToString());
	}

	//! Special case: Any inventory item (rvConfig Inventory_Base, CfgWeapons, CfgMagazines or EnforceScript ItemBase type)
	static bool IsInventoryItem(string className)
	{
		typename classType = className.ToType();
		if (classType)
		{
			string itemClass = "ItemBase";
			typename itemType = itemClass.ToType();

			if (classType.IsInherited(itemType))
				return true;
		}

		if (g_Game.IsKindOf(className, "Inventory_Base"))
			return true;

		if (g_Game.ConfigIsExisting("CfgMagazines " + className))
			return true;

		if (g_Game.ConfigIsExisting("CfgWeapons " + className))
			return true;

		return false;
	}

	//! Special case: Any player (rvConfig SurvivorBase or EnforceScript PlayerBase type)
	static bool IsPlayer(string className)
	{
		typename classType = className.ToType();
		if (classType)
		{
			string playerClass = "PlayerBase";
			typename playerType = playerClass.ToType();

			if (classType.IsInherited(playerType))
				return true;
		}

		return g_Game.IsKindOf(className, "SurvivorBase");
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

	static string IntToHex(TIntArray ints)
	{
		string hex;
		foreach (int n: ints)
		{
			hex += IntToHex(n);
		}
		return hex;
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

	static string GetISODate(bool useUTC = false)
	{
		int year;
		int month;
		int day;

		if (useUTC)
			GetYearMonthDayUTC(year, month, day);
		else
			GetYearMonthDay(year, month, day);

		return year.ToStringLen(4) + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2);
	}

	static string GetISODateTime(bool useUTC = false, string delim = "T", string delimHMS = ":")
	{
		return GetISODate(useUTC) + delim + GetISOTime(useUTC, false, delimHMS);
	}

	/*!
	 * @brief format time (value in seconds)
	 * @return one of the following formats as string (depending on arguments):
	 * "HH:MM:SS.SSS" (default)
	 * "HH:MM:SS"     (include_ms = false)
	 * "MM:SS.SSS"    (include_hours = false)
	 * "MM:SS"        (include_ms = false, include_hours = false)
	 * @note HH is the total number of hours and can exceed 24 unless `discard_days` is set to true!
	*/
	static string FormatTime(float time, bool include_ms = true, bool include_hours = true, bool discard_days = false, string delimHMS = ":")
	{
		if (include_hours)
		{
			int hours = (int) time / 3600;
			time -= hours * 3600;

			if (discard_days && hours >= 24)
				hours = hours % 24;
		}

		int minutes = (int) time / 60;
		time -= minutes * 60;
		int seconds = (int) time;

		string timestring;

		if (include_hours)
			timestring = hours.ToStringLen(2) + delimHMS + minutes.ToStringLen(2) + delimHMS + seconds.ToStringLen(2);
		else
			timestring = minutes.ToStringLen(2) + delimHMS + seconds.ToStringLen(2);

		if ( include_ms )
		{
			time -= seconds;
			int ms = time * 1000;
			timestring += "." + ms.ToStringLen(3);
		}

		return timestring;
	}

	static string GetISOTime(bool useUTC = false, bool include_ms = true, string delimHMS = ":")
	{
		if ( include_ms && GetDayZGame() )
		{
			//! Accurate, including milliseconds
			return FormatTime(GetDayZGame().ExpansionGetStartTime(useUTC) + GetDayZGame().GetTickTime(), true, true, true, delimHMS);
		}
		else
		{
			//! Next best thing
			int hour;
			int minute;
			int second;

			if (useUTC)
				GetHourMinuteSecondUTC(hour, minute, second);
			else
				GetHourMinuteSecond(hour, minute, second);

			return hour.ToStringLen(2) + delimHMS + minute.ToStringLen(2) + delimHMS + second.ToStringLen(2);
		}
	}

	//! Fast timestamp, use this instead of CF_Date.Now().GetTimestamp() when performance counts
	static int GetTimestamp(bool useUTC = false)
	{
		return GetDayZGame().ExpansionGetStartTimestamp(useUTC) + (int) GetDayZGame().GetTickTime();
	}

	//! Fast accurate time in seconds, not including years/months/days
	static float GetTime(bool useUTC = false)
	{
		return GetDayZGame().ExpansionGetStartTime(useUTC) + GetDayZGame().GetTickTime();
	}

	static string GetTimeString( float total_time, bool include_seconds = false )
	{
		string time_string;

		if ( total_time <= 0 )
		{
			time_string =  "0 #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_SECONDS";
			return time_string;
		}

		int time_seconds = total_time;	//! Convert total time to int

		int days = time_seconds / 86400;
		if ( days > 0 )
		{
			time_string += GetValueString( days ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_DAYS";	//! Days
		}

		int hours = ( time_seconds % 86400 ) / 3600;
		if ( hours > 0 )
		{
			if (time_string)
				time_string += " ";	//! Separator
			time_string += GetValueString( hours ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";	//! Hours
		}

		int minutes = ( time_seconds % 3600 ) / 60;
		if ( minutes > 0 )
		{
			if (time_string)
				time_string += " ";	//! Separator
			time_string += GetValueString( minutes ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_MINUTES";	//! Minutes
		}

		if ( include_seconds )
		{
			int seconds = time_seconds - days * 86400 - hours * 3600 - minutes * 60;
			if ( seconds > 0 )
			{
				if (time_string)
					time_string += " ";	//! Separator
				time_string += GetValueString( seconds ) + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_SECONDS";
			}
		}

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

	static string GetPersistentIDString(EntityAI entity)
	{
		int b1, b2, b3, b4;
		entity.GetPersistentID(b1, b2, b3, b4);
		return GetPersistentIDString(b1, b2, b3, b4);
	}

	static string GetPersistentIDString(int b1, int b2, int b3, int b4)
	{
		string id;
		id += IntToHex(b1);
		id += IntToHex(b2);
		id += IntToHex(b3);
		id += IntToHex(b4);
		return id;
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

	static array< string > FindInLocation( string folder, string ext = "", int mode = ExpansionFindFileMode.FILES )
	{
		array< string > files = new array< string >;
		if (!FileExist(folder))
			return files;
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( folder + "*" + ext, fileName, fileAttr, 0 );
		if ( findFileHandle )
		{
			bool isValid = true;
			bool includeFiles = mode & ExpansionFindFileMode.FILES;
			bool includeDirs = mode & ExpansionFindFileMode.DIRECTORIES;

			while (isValid)
			{
				bool isDir = fileAttr & FileAttr.DIRECTORY;

				if (fileName.Length() > 0 && ((includeFiles && !isDir) || (includeDirs && isDir)))
				{
					files.Insert( fileName );
				}

				isValid = FindNextFile(findFileHandle, fileName, fileAttr);
			}

			CloseFindFile( findFileHandle );
		}
		return files;
	}

	static array< string > FindFilesInLocation( string folder, string ext = "" )
	{
		return FindInLocation(folder, ext, ExpansionFindFileMode.FILES);
	}

	static array< string > FindDirectoriesInLocation( string folder, string ext = "" )
	{
		return FindInLocation(folder, ext, ExpansionFindFileMode.DIRECTORIES);
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
			//! FindFileHandle of NULL means there were no files found
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
		"eAI_SurvivorF_Eva", "eAI_SurvivorF_Naomi", "eAI_SurvivorF_Baty"};
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

	static void MessageNearPlayers(vector position, float radius, string msg)
	{
		float radiusSq = radius * radius;
		array<Man> players();
		GetGame().GetPlayers(players);
		foreach (Man player: players)
		{
			if (vector.DistanceSq(position, player.GetPosition()) < radiusSq)
			{
				GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(msg), true, player.GetIdentity());
			}
		}
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

	static string GetImpactSurfaceType(Object directHit, vector hitPosition, vector relativeVelocityBefore)
	{
		string surfaceImpact;

		if (directHit)
		{
			vector add = relativeVelocityBefore.Normalized() * 0.1;
			if (DayZPhysics.GetHitSurface(
				directHit,
				hitPosition - add,
				hitPosition + add,
				surfaceImpact) && surfaceImpact != string.Empty)
			{
			#ifdef DIAG
				EXTrace.Print(EXTrace.AI, null, "GetHitSurface " + surfaceImpact);
			#endif

				return surfaceImpact;
			}

			//! Sometimes, GetHitSurface fails to return something useful
			if (directHit.IsMan() || directHit.IsInherited(DayZCreature))
				surfaceImpact = "Hit_MeatBones";
			else if (directHit.IsBuilding())
				surfaceImpact = "Hit_Concrete";
			else if (directHit.IsBush())
				surfaceImpact = "Hit_Foliage";
			else if (directHit.IsTree())
				surfaceImpact = "Hit_Wood";
			else if (directHit.IsRock())
				surfaceImpact = "Hit_Gravel";
			else if (directHit.IsWeapon() || directHit.IsMagazine() || directHit.IsTransport())
				surfaceImpact = "Hit_Metal";
			else if (directHit.IsClothing())
				surfaceImpact = "Hit_Textile";
			else if (SurfaceIsWater(hitPosition))
				surfaceImpact = "Hit_Water";
			else
				surfaceImpact = "Hit_Dirt";
		}
		else
		{
			if (SurfaceIsWater(hitPosition))
				surfaceImpact = "Hit_Water";
			else
				surfaceImpact = "Hit_Dirt";
		}

		return surfaceImpact;
	}

	//! @brief return lowercase game world name, with "gloom" suffix removed (if present)
	static string GetCanonicalWorldName(CGame game = NULL)
	{
		if (!game)
			game = GetGame();

		string worldName;
		game.GetWorldName(worldName);
		worldName.ToLower();

		if (worldName.Length() < 5)
			return worldName;

		if (worldName.IndexOf("gloom") == worldName.Length() - 5)
			worldName = worldName.Substring(0, worldName.Length() - 5);

		return worldName;
	}

	static string VectorToString(vector vec, ExpansionVectorToString format = ExpansionVectorToString.Beautify, int precision = 2)
	{
		float m = Math.Pow(10, precision);
		TStringArray output();

		for (int i = 0; i < 3; i++)
			output.Insert((Math.Round(vec[i] * m) / m).ToString());

		switch (format)
		{
			case ExpansionVectorToString.Beautify:
				return "<" + ExpansionString.JoinStrings(output, ", ") + ">";
			case ExpansionVectorToString.Labels:
				return "X: " + output[0] + " Y: " + output[1] + " Z: " + output[2];
		}

		return ExpansionString.JoinStrings(output, " ");
	}

	static bool GetVariableIntByName(typename type, string name, out int val)
	{
		return StringToEnumEx(type, name, val);
	}

	static void LockInventoryRecursive(EntityAI entity, int lockType = HIDE_INV_FROM_SCRIPT)
	{
		int attcount = entity.GetInventory().AttachmentCount();
		for (int att = 0; att < attcount; att++)
		{
			EntityAI attachmentEntity = entity.GetInventory().GetAttachmentFromIndex(att);
			if (attachmentEntity)
				LockInventoryRecursive(attachmentEntity, lockType);
		}
		entity.GetInventory().LockInventory(lockType);
	}

	static void UnlockInventoryRecursive(EntityAI entity, int lockType = HIDE_INV_FROM_SCRIPT)
	{
		int attcount = entity.GetInventory().AttachmentCount();
		for (int att = 0; att < attcount; att++)
		{
			EntityAI attachmentEntity = entity.GetInventory().GetAttachmentFromIndex(att);
			if (attachmentEntity)
				UnlockInventoryRecursive(attachmentEntity, lockType);
		}
		entity.GetInventory().UnlockInventory(lockType);
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
