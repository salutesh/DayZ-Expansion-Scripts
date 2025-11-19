/**
 * ExpansionSettingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSettingBase
{
	int m_Version;

	[NonSerialized()]
	protected bool m_IsUsingDefaults;

	void ExpansionSettingBase()
	{
	}

	void ~ExpansionSettingBase()
	{
	}

	ExpansionScriptRPC CreateRPC()
	{
		return GetExpansionSettings().CreateRPC(Type());
	}

	ExpansionBitStreamReader GetReader(ParamsReadContext ctx)
	{
		return GetExpansionSettings().GetReader(ctx);
	}

	bool OnRecieve( ParamsReadContext ctx )
	{
		Error("Not implemented");

		return false;
	}

	void OnSend( ParamsWriteContext ctx )
	{
		Error("Not implemented");
	}

	bool Load()
	{
		if ( !g_Game.IsDedicatedServer() )
		{
			Defaults();
			return false;
		}

		return OnLoad();
	}

	protected bool OnLoad()
	{
		Error("Not implemented");
		return false;
	}

	void Unload()
	{
		Error("Not implemented");
	}

	bool Save()
	{
		if ( g_Game.IsClient() )
		{
			return false;
		}
		
		if ( !FileExist( EXPANSION_SETTINGS_FOLDER ) )
		{
			ExpansionStatic.MakeDirectoryRecursive( EXPANSION_SETTINGS_FOLDER );
		}
		
		if ( !FileExist( EXPANSION_MISSION_SETTINGS_FOLDER ) )
		{
			ExpansionStatic.MakeDirectoryRecursive( EXPANSION_MISSION_SETTINGS_FOLDER );
		}

		return OnSave();
	}

	protected bool OnSave()
	{
		Error("Not implemented");
		return false;
	}

	void Defaults()
	{
		Error("Not implemented");
	}

	void SetIsUsingDefaults(bool isUsingDefaults = true)
	{
		m_IsUsingDefaults = isUsingDefaults;
	}

	bool IsUsingDefaults()
	{
		return m_IsUsingDefaults;
	}

	bool IsLoaded()
	{
		Error("Not implemented");
		return true;
	}

	bool Copy( ExpansionSettingBase setting )
	{
		Error("Not implemented");
		return false;
	}

	int Send( PlayerIdentity identity )
	{
		Error("Not implemented");
		return 0;
	}

	void Update( ExpansionSettingBase setting )
	{
		if ( setting )
		{
			if ( setting == this )
				EXPrint("ExpansionSettingBase::Update - Warning: " + setting + " is trying to copy from itself. This may be unintended.");
			else
				Copy( setting );

			Save();
		}

		if ( !IsMissionClient() )
			Send( NULL );
	}
	
	string SettingName()
	{
		return "Base Settings";
	}

	//! @note if moving a directory, make sure paths end with "\\"
	static bool MoveSettings(string srcPath, string dstPath, string ext = "")
	{
		EXPrint("Moving " + srcPath + " to " + dstPath);
		if (ExpansionStatic.CopyFileOrDirectoryTree(srcPath, dstPath, ext, true))
		{
			//! Use original src path for info line
			string infoLine = "Info: '" + srcPath + "' has moved to '" + dstPath + "'.";

			ExpansionString src = srcPath;
			int lastIdx = src.LastIndexOf("\\");
			if (lastIdx == srcPath.Length() - 1)
			{
				//! It's a directory. Strip trailing backslash and go again
				srcPath = srcPath.Substring(0, lastIdx);
				src = srcPath;
				lastIdx = src.LastIndexOf("\\");
			}

			string hintFileName = srcPath.Substring(0, lastIdx + 1) + srcPath.Substring(lastIdx + 1, srcPath.Length() - lastIdx - 1) + " ReadMe.txt";

			FileHandle hintFile = OpenFile(hintFileName, FileMode.WRITE);
			FPrintln(hintFile, infoLine);
			FPrintln(hintFile, "Feel free to delete this ReadMe file at your convenience.");
			CloseFile(hintFile);
			return true;
		}
		else
		{
			return false;
		}
	}
}