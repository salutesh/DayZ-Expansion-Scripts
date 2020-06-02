class ExpansionSkinModule: JMModuleBase
{	
	private autoptr map< string, ref array< ref ExpansionSkin > > m_Skins;

	void ExpansionSkinModule()
	{
		m_Skins = new map< string, ref array< ref ExpansionSkin > >;
	}

	override void OnInit()
	{
		int mod_count = GetGame().ConfigGetChildrenCount( "CfgMods" );
		
		for( int i = 0; i < mod_count; i++ )
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
					if ( folderName.Length() > 0 ) // && ( fileAttr & FileAttr.DIRECTORY) )
					{
						if ( filePatching )
						{
							LoadClassSkins( folderName, folder );
						} else 
						{
							LoadSkinsForObject( folderName, folder );
						}
					}
					
					while ( FindNextFile( findFileHandle, folderName, fileAttr ) )
					{
						if ( folderName.Length() > 0 ) // && ( fileAttr & FileAttr.DIRECTORY) )
						{
							if ( filePatching )
							{
								LoadClassSkins( folderName, folder );
							} else 
							{
								LoadSkinsForObject( folderName, folder );
							}
						}
					}
				}
			}
		}
	}

	private void LoadClassSkins( string folder, string rootFolder )
	{
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
	}

	private void LoadSkinsForObject( string file, string rootFolder )
	{
		SkinPrint( file );

		int idx = file.IndexOf( "\\" );

		string classname = file.Substring( 0, idx );
		string skinname = file.Substring( idx + 1, file.Length() - idx - 1 );

		skinname.ToLower();

		int pos = skinname.IndexOf( "." );
		if ( pos > -1 )
		{
			skinname = skinname.Substring( 0, pos );
		}

		TStringArray applySkinsTo = new TStringArray;
		GetGame().ConfigGetTextArray( "cfgVehicles " + classname + " applySkinsTo", applySkinsTo );

		applySkinsTo.Insert( classname );

		ref ExpansionSkin skin = new ExpansionSkin;
		JsonFileLoader< ExpansionSkin >.JsonLoadFile( rootFolder + file, skin );
		skin.SkinFileName = skinname;

		for ( int i = 0; i < applySkinsTo.Count(); i++ )
		{
			classname = applySkinsTo[i];
			classname.ToLower();

			SkinPrint( "Trying to apply " + skinname + " to " + classname );

			array< ref ExpansionSkin > skins = m_Skins.Get( classname );
			if ( !skins )
			{
				m_Skins.Insert( classname, new array< ref ExpansionSkin > );
				skins = m_Skins.Get( classname );

				SkinPrint( "  Creating new array" );
			}

			skins.Insert( skin );

			SkinPrint( "  Added skin=" + skin + " to " + skins + " for " + classname );
		}
	}

	int GetSkinIndex( string classname, string skin )
	{
		classname.ToLower();
		skin.ToLower();

		SkinPrint( "GetSkinIndex " + classname + ", " + skin );

		array< ref ExpansionSkin > skins;

		if ( m_Skins.Find( classname, skins ) )
		{
			for ( int i = 0; i < skins.Count(); i++ )
			{
				SkinPrint( "  skin " + skins[i].SkinFileName );
				
				if ( skins[i].SkinFileName == skin )
				{
					return i;
				}
			}
		}

		return -1;
	}

	string GetSkinName( string classname, int index )
	{
		if ( index < 0 )
			return "";

		classname.ToLower();

		array< ref ExpansionSkin > skins;

		if ( m_Skins.Find( classname, skins ) )
		{
			if ( index >= skins.Count() )
				return "";

			return skins[index].SkinFileName;
		}

		return "";
	}

	array< ref ExpansionSkin > RetrieveSkins( string classname )
	{
		classname.ToLower();

		array< ref ExpansionSkin > skins;
		if ( m_Skins.Find( classname, skins ) )
		{
			return skins;
		}

		return NULL;
	}

	void SkinCarDoor( CarDoor door )
	{
		return;

		string mainType = door.ExpansionMainType();
		string doorType = door.GetType();
		mainType.ToLower();
		doorType.ToLower();
		
		if ( mainType != "" && mainType != doorType )
		{
			if ( GetGame().IsKindOf( doorType, mainType ) )
			{
				int doorLen = doorType.Length();
				int mainLen = mainType.Length();

				int start = doorLen + 1;
				int end = doorLen - mainLen - 1;

				string skin = mainType.Substring( start, end );

				ReplaceItemWithNewLambdaBase lambda = new ExpansionSkinCarDoorFromObjectLambda( door, mainType, this, skin );
				lambda.Execute( NULL );
			}
		}
	}

	void SkinPrint( string message )
	{
		//! Print( message );
	}
}