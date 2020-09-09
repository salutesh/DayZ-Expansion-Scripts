class ExpansionSkins : Managed
{
	private ref map< string, ref ExpansionSkin > m_Skins;
	private ref array< string > m_Order;

	private string m_DefaultSkin;
	
	// ------------------------------------------------------------
	// ExpansionSkins Constructor
	// ------------------------------------------------------------	
	void ExpansionSkins( string defaultSkin )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::ExpansionSkins - Start");
		#endif
		
		m_DefaultSkin = defaultSkin;
		m_Skins = new map< string, ref ExpansionSkin >;
		m_Order = new array< string >;
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::ExpansionSkins - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Destructor
	// ------------------------------------------------------------	
	void ~ExpansionSkins()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::~ExpansionSkins - Start");
		#endif
		
		delete m_Skins;
		delete m_Order;
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::~ExpansionSkins - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins AddSkin
	// ------------------------------------------------------------	
	void AddSkin( string name, string path )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::AddSkin - Start");
		#endif
		
		ref ExpansionSkin skin = new ExpansionSkin;
		JsonFileLoader< ExpansionSkin >.JsonLoadFile( path, skin );
		
		m_Skins.Insert( name, skin );
		m_Order.Insert( name );
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::AddSkin - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Count
	// ------------------------------------------------------------
	int Count()
	{
		return m_Skins.Count();
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Find
	// ------------------------------------------------------------	
	int Find( string name )
	{
		return m_Order.Find( name );
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins GetName
	// ------------------------------------------------------------
	string GetName( int index )
	{
		return m_Order[index];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Get
	// ------------------------------------------------------------
	ExpansionSkin Get( int index )
	{
		return m_Skins[GetName(index)];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Get
	// ------------------------------------------------------------
	ExpansionSkin Get( string name )
	{
		return m_Skins[name];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins GetDefaultSkin
	// ------------------------------------------------------------
	string GetDefaultSkin()
	{
		return m_DefaultSkin;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Sort
	// ------------------------------------------------------------
	void Sort()
	{
		m_Order.Sort();
	}
};

class ExpansionSkinModule: JMModuleBase
{	
	private ref map< string, ref ExpansionSkins > m_Skins;
	
	// ------------------------------------------------------------
	// ExpansionSkinModule Constructor
	// ------------------------------------------------------------
	void ExpansionSkinModule()
	{
		m_Skins = new map< string, ref ExpansionSkins >;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionSkinModule()
	{
		delete m_Skins;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::OnInit - Start");
		#endif
		
		int mod_count = GetGame().ConfigGetChildrenCount( "CfgMods" );
		
		for ( int i = 0; i < mod_count; ++i )
		{
			string mod_name;
			GetGame().ConfigGetChildName( "CfgMods", i, mod_name );

			string configSkins = "CfgMods " + mod_name + " expansionSkins";
			if ( !GetGame().ConfigIsExisting( configSkins ) )
			{
				continue;
			}

			TStringArray folders = new TStringArray;
			GetGame().ConfigGetTextArray( configSkins, folders );

			if ( !folders || folders.Count() == 0 )
			{
				continue;
			}

			bool filePatching = IsCLIParam( "filePatching" );

			for ( int j = 0; j < folders.Count(); j++ )
			{
				string folder = folders[j];
				folder.Replace("/", "\\");

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( folder );
				#endif

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
				}
			}
		}

		for ( int k = 0; k < m_Skins.Count(); ++k )
		{
			m_Skins.GetElement( k ).Sort();
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::OnInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule LoadClassSkins
	// ------------------------------------------------------------
	private void LoadClassSkins( string folder, string rootFolder )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::LoadClassSkins - Start");
		#endif
		
		string path = rootFolder + "\\" + folder + "\\";
		
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( path + "*", fileName, fileAttr, FindFileFlags.ALL );
		
		#ifdef EXPANSIONEXLOGPRINT
		Print( findFileHandle );
		#endif

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
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkins::LoadClassSkins - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule LoadSkinsForObject
	// ------------------------------------------------------------
	private void LoadSkinsForObject( string file, string rootFolder )
	{
		#ifdef EXPANSION_SKIN_LOGGING
		Print( file );
		#endif

		int idx = file.IndexOf( "\\" );

		string classname = file.Substring( 0, idx );
		string skinname = file.Substring( idx + 1, file.Length() - idx - 1 );
		classname.ToLower();

		int pos = skinname.IndexOf( "." );
		if ( pos > -1 )
		{
			skinname = skinname.Substring( 0, pos );
		}

		skinname.ToLower();

		string path = "cfgVehicles";
		if ( !GetGame().ConfigIsExisting( path + " " + classname ) )
		{
			path = "cfgWeapons";
			if ( !GetGame().ConfigIsExisting( path + " " + classname ) )
			{
				path = "cfgMagazines";
				if ( !GetGame().ConfigIsExisting( path + " " + classname ) )
				{
					path = "cfgNonAIVehicles";
					if ( !GetGame().ConfigIsExisting( path + " " + classname ) )
					{
						if(classname)
							Print( "ExpansionSkinModule::LoadSkinsForObject - [ERROR]: Invalid class name " + classname );
						
						return;
					}
				}
			}
		}

		TStringArray applySkinsTo = new TStringArray;
		GetGame().ConfigGetTextArray( path + " " + classname + " applySkinsTo", applySkinsTo );

		string defaultSkin = "";
		GetGame().ConfigGetText( path + " " + classname + " defaultSkin", defaultSkin );

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
	
	// ------------------------------------------------------------
	// ExpansionSkinModule GetSkinIndex
	// ------------------------------------------------------------
	int GetSkinIndex( string classname, string skin )
	{
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

		ExpansionSkins skins = m_Skins.Get( classname );
		if ( !skins || index >= skins.Count() )
			return "";

		return skins.GetName( index );
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule RetrieveSkins
	// ------------------------------------------------------------
	void RetrieveSkins( string classname, out array< ExpansionSkin > skinCopy, out string defaultSkin )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::RetrieveSkins - Start");
		#endif
		
		defaultSkin = "";

		classname.ToLower();

		ExpansionSkins skins = m_Skins.Get( classname );
		if ( !skins )
		{
			#ifdef EXPANSIONEXLOGPRINT
			Print("ExpansionSkinModule::RetrieveSkins - No Skins!");
			#endif
			return;
		}
		
		defaultSkin = skins.GetDefaultSkin();

		for ( int i = 0; i < skins.Count(); ++i )
			skinCopy.Insert( skins.Get( i ) );
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::RetrieveSkins - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule PerformCESkinSwap
	// ------------------------------------------------------------
	bool PerformCESkinSwap( EntityAI ent )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::PerformCESkinSwap - Start");
		#endif
		
		string base = ent.ConfigGetString( "skinBase" );
		string skin = ent.ConfigGetString( "skinName" );
		string currentType = ent.GetType();
		
		base.ToLower();
		currentType.ToLower();
		
		if ( base != "" && skin != "" && base != currentType )
		{
			#ifndef EXPANSION_SKIN_REPLACEMENT_DISABLE
				ExpansionSkinObjectLambda lambda = new ExpansionSkinObjectLambda( ent, base, NULL, this, skin );
		
				//InventoryLocation newDst = new InventoryLocation;
				//ent.GetInventory().GetCurrentInventoryLocation( newDst );
				//lambda.OverrideNewLocation( newDst );

				if ( !GetGame().IsClient() )
				{
					ent.GetInventory().ReplaceItemWithNew( InventoryMode.SERVER, lambda );
				}

				#ifdef EXPANSIONEXLOGPRINT
				Print("ExpansionSkinModule::PerformCESkinSwap - End");
				#endif

				return true;
			#else
				Print( "ExpansionSkinModule] EXPANSION_SKIN_REPLACEMENT_DISABLE is defined but a skinned replacement object is set to spawn, please remove \"" + currentType + "\" from your types.xml to make this error go away." );
			#endif
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::PerformCESkinSwap - End");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkinModule PerformSkinFixOldVersion
	// ------------------------------------------------------------
	bool PerformSkinFixOldVersion( EntityAI ent, string skin )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::PerformSkinFixOldVersion - Start");
		#endif
		
		string base = ent.ConfigGetString( "newBase" );
		
		if ( base != "" && base != ent.GetType() )
		{
			ExpansionSkinObjectLambda lambda = new ExpansionSkinObjectLambda( ent, base, NULL, this, skin );
			
			//InventoryLocation newDst = new InventoryLocation;
			//ent.GetInventory().GetCurrentInventoryLocation( newDst );
			//lambda.OverrideNewLocation( newDst );

			if ( GetGame().IsClient() )
			{

			} else
			{
				ent.GetInventory().ReplaceItemWithNew( InventoryMode.SERVER, lambda );
			}

			return true;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionSkinModule::PerformSkinFixOldVersion - End");
		#endif

		return false;
	}
}