/**
 * ExpansionSkinModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionSkinModule)]
class ExpansionSkinModule: CF_ModuleWorld
{
	static ExpansionSkinModule s_Instance;

	private autoptr map< string, ref ExpansionSkins > m_Skins = new map< string, ref ExpansionSkins >;
	private autoptr map< string, string > m_SkinBase = new map< string, string >;
	private autoptr map< string, string > m_SkinName = new map< string, string >;

	void ExpansionSkinModule()
	{
		s_Instance = this;
	}
		
	// ------------------------------------------------------------
	// ExpansionSkinModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "OnInit");
#endif

		super.OnInit();

		int mod_count = g_Game.ConfigGetChildrenCount( "CfgMods" );
		
		for ( int i = 0; i < mod_count; ++i )
		{
			string mod_name;
			g_Game.ConfigGetChildName( "CfgMods", i, mod_name );

			string configSkins = "CfgMods " + mod_name + " expansionSkins";
			if ( !g_Game.ConfigIsExisting( configSkins ) )
			{
				continue;
			}

			TStringArray folders = new TStringArray;
			g_Game.ConfigGetTextArray( configSkins, folders );

			if ( !folders || folders.Count() == 0 )
			{
				continue;
			}

			bool filePatching = IsCLIParam( "filePatching" );

			for ( int j = 0; j < folders.Count(); j++ )
			{
				string folder = folders[j];
				folder.Replace("/", "\\");

				CF_Log.Debug( folder );

				string folderName;
				FileAttr fileAttr;
				FindFileHandle findFileHandle = FindFile( folder + "*", folderName, fileAttr, FindFileFlags.ALL );
				
				if ( findFileHandle )
				{
					if ( folderName.Length() > 0 )
					{
						//if ( filePatching )
						//{
						//	LoadClassSkins( folderName, folder );
						//} else 
						{
							LoadSkinsForObject( folderName, folder );
						}
					}
					
					while ( FindNextFile( findFileHandle, folderName, fileAttr ) )
					{
						if ( folderName.Length() > 0 )
						{
							//if ( filePatching )
							//{
							//	LoadClassSkins( folderName, folder );
							//} else 
							{
								LoadSkinsForObject( folderName, folder );
							}
						}
					}

					CloseFindFile(findFileHandle);
				}
			}
		}

		foreach (string classname, ExpansionSkins skins: m_Skins )
		{
			skins.Sort();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule LoadClassSkins
	// ------------------------------------------------------------
	private void LoadClassSkins( string folder, string rootFolder )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "LoadClassSkins");
#endif

		string path = rootFolder + "\\" + folder + "\\";
		
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( path + "*", fileName, fileAttr, FindFileFlags.ALL );
		
		if ( findFileHandle )
		{
			if ( fileName.Length() > 0 ) // && ( fileAttr & FileAttr.DIRECTORY) )
			{
				LoadSkinsForObject( folder + "\\" + fileName, rootFolder );
			}
			
			while ( FindNextFile( findFileHandle, fileName, fileAttr ) )
			{
				if ( fileName.Length() > 0 ) // && ( fileAttr & FileAttr.DIRECTORY) )
				{
					LoadSkinsForObject( folder + "\\" + fileName, rootFolder );
				}
			}

			CloseFindFile(findFileHandle);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule LoadSkinsForObject
	// ------------------------------------------------------------
	private void LoadSkinsForObject( string file, string rootFolder )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "LoadSkinsForObject");
#endif

		int idx = file.IndexOf( "\\" );
		if ( idx == -1 )
			idx = file.IndexOf( "/" );
		if ( idx == -1 )
			return;  //! Fuck off Workbench

		string classname = file.Substring( 0, idx );
		string skinname = file.Substring( idx + 1, file.Length() - idx - 1 );
		classname.ToLower();

		int pos = skinname.IndexOf( "." );
		if ( pos > -1 )
		{
			skinname = skinname.Substring( 0, pos );
		}

		skinname.ToLower();

		string path = GetConfigPath( classname );
		if ( !path )
			return;

		TStringArray applySkinsTo = new TStringArray;
		g_Game.ConfigGetTextArray( path + " " + classname + " applySkinsTo", applySkinsTo );

		string defaultSkin = "";
		g_Game.ConfigGetText( path + " " + classname + " defaultSkin", defaultSkin );

		applySkinsTo.Insert( classname );

		for ( int i = 0; i < applySkinsTo.Count(); i++ )
		{
			classname = applySkinsTo[i];
			classname.ToLower();

			ExpansionSkins skins = m_Skins.Get( classname );
			if ( !skins )
			{
				m_Skins.Insert( classname, new ExpansionSkins( defaultSkin ) );
				skins = m_Skins.Get( classname );
			}

			skins.AddSkin( skinname, rootFolder + file );
		}
	}
	
	string GetConfigPath( string classname )
	{
		string path = "cfgVehicles";
		if ( !g_Game.ConfigIsExisting( path + " " + classname ) )
		{
			path = "cfgWeapons";
			if ( !g_Game.ConfigIsExisting( path + " " + classname ) )
			{
				path = "cfgMagazines";
				if ( !g_Game.ConfigIsExisting( path + " " + classname ) )
				{
					path = "cfgNonAIVehicles";
					if ( !g_Game.ConfigIsExisting( path + " " + classname ) )
					{
						if(classname)
							Print( "ExpansionSkinModule::GetConfigPath - [ERROR]: Invalid class name " + classname );
						
						return "";
					}
				}
			}
		}
		return path;
	}

	// ------------------------------------------------------------
	// ExpansionSkinModule GetSkinIndex
	// ------------------------------------------------------------
	int GetSkinIndex( string classname, string skin )
	{
		string skinBase = GetSkinBase( classname );
		if ( skinBase )
			classname = skinBase;

		classname.ToLower();
		skin.ToLower();

		ExpansionSkins skins = m_Skins.Get( classname );
		if ( !skins || skins.Count() == 0 )
			return -1;

		for ( int i = 0; i < skins.Count(); i++ )
			if ( skins.GetName( i ) == skin )
				return i;

		return -1;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule GetSkinName
	// ------------------------------------------------------------
	string GetSkinName( string classname, int index )
	{
		if ( index < 0 )
			return "";

		string skinBase = GetSkinBase( classname );
		if ( skinBase )
			classname = skinBase;

		classname.ToLower();

		ExpansionSkins skins = m_Skins.Get( classname );
		if ( !skins || index >= skins.Count() )
			return "";

		return skins.GetName( index );
	}
	
	string GetSkinName( string classname )
	{
		string path = GetConfigPath( classname );
		if ( path && g_Game.ConfigIsExisting( path + " " + classname + " skinName" ) )
			return g_Game.ConfigGetTextOut( path + " " + classname + " skinName" );
		return "";
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule RetrieveSkins
	// ------------------------------------------------------------
	void RetrieveSkins( string classname, out array< ExpansionSkin > skinCopy, out string defaultSkin )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "RetrieveSkins");
#endif

		defaultSkin = "";

		string path;

		string skinBase = GetSkinBase( classname );
		if ( !skinBase )
		{
			path = GetConfigPath( classname );

			//! If it exists, use skinBase instead of given classname
			//! (so when spawning <thing>_<skinname>, it has the correct skin)
			if ( g_Game.ConfigIsExisting( path + " " + classname + " skinBase" ) )
				g_Game.ConfigGetText( path + " " + classname + " skinBase", skinBase );

			if ( !skinBase )
				skinBase = classname;

			m_SkinBase.Insert( classname, skinBase );
		}

		skinBase.ToLower();

		ExpansionSkins skins = m_Skins.Get( skinBase );
		if ( !skins )
		{
			return;
		}

		string skinName = m_SkinName.Get( classname );
		if ( !skinName )
		{
			if ( !path )
				path = GetConfigPath( classname );

			//! If it exists, use skinName instead of default skin
			//! (so when spawning <thing>_<skinname>, it has the correct skin)
			if ( g_Game.ConfigIsExisting( path + " " + classname + " skinName" ) )
				g_Game.ConfigGetText( path + " " + classname + " skinName", skinName );

			if ( !skinName )
				skinName = skins.GetDefaultSkin();

			m_SkinName.Insert( classname, skinName );
		}
		
		defaultSkin = skinName;

		for ( int i = 0; i < skins.Count(); ++i )
			skinCopy.Insert( skins.Get( i ) );
	}

	string GetSkinBase( string classname )
	{
		return m_SkinBase.Get( classname );
	}

	ExpansionSkins GetSkins( string classname )
	{
		string skinBase = GetSkinBase( classname );
		if ( skinBase )
			classname = skinBase;

		classname.ToLower();

		return m_Skins.Get( classname );
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule HasSkins
	// ------------------------------------------------------------
	bool HasSkins(string classname)
	{
		return GetSkins( classname ) != NULL;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule HasSkin
	// ------------------------------------------------------------
	bool HasSkin(string classname, int skinIndex)
	{
		ExpansionSkins skins = GetSkins( classname );
		if (!skins || !skins.Get(skinIndex))
			return false;
		
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule RetrieveSkins
	// ------------------------------------------------------------	
	map< string, ref ExpansionSkins > GetAllSkins()
	{
		return m_Skins;
	}
};
