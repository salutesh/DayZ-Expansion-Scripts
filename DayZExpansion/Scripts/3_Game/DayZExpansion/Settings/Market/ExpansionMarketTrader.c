/**
 * ExpansionMarketTrader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTrader
{
	int m_Version = 3;
	string m_FileName;

	string TraderName;
	string DisplayName;

	autoptr array< string > Items;

	// ------------------------------------------------------------
	// ExpansionMarketTrader Constructor
	// ------------------------------------------------------------
	void ExpansionMarketTrader()
	{
		Items = new array< string >;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketTrader Load
	// ------------------------------------------------------------
	static ref ExpansionMarketTrader Load( string name )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[ExpansionMarketTrader] Loading settings file " + name );
		#endif

		ref ExpansionMarketTrader settings = new ExpansionMarketTrader;
		settings.m_FileName = name;

		FileSerializer file = new FileSerializer();

		if ( FileExist( EXPANSION_TRADER_FOLDER + name + ".bin" ) && file.Open( EXPANSION_TRADER_FOLDER + name + ".bin", FileMode.READ ) )
		{
			file.Read( settings.m_Version );

			// if we make any changes to how the data is stored, 
			// use the version to specify how we should modify the data

			file.Read( settings.TraderName );
			file.Read( settings.DisplayName );

			if ( settings.m_Version >= 2 )
			{
				int count;
				file.Read( count );
				for ( int i = 0; i < count; i++ )
				{
					string clsName;
					file.Read( clsName );
					clsName.ToLower();

					settings.Items.Insert( clsName );

					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionMarketTrader::Load - Added item " + clsName + " to trader items array");
					#endif
				}
			} 
			else
			{
				file.Read( settings.Items );
			}

			file.Close();
			
			settings.Save();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "ExpansionMarketTrader::Load - Loaded settings file " + name );
			#endif
		} 
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[ExpansionMarketTrader] Failed to load settings file " + name );
			#endif
		}
		
		return settings;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionMarketTrader] Saving settings");
		#endif
		
		FileSerializer file = new FileSerializer();
		
		if ( file.Open( EXPANSION_TRADER_FOLDER + m_FileName + ".bin", FileMode.WRITE ) )
		{
			// always overwrite with the latest version.

			file.Write( m_Version );
		
			file.Write( TraderName );
			file.Write( DisplayName );
			file.Write( Items.Count() );
			
			for ( int i = 0; i < Items.Count(); i++ )
			{
				file.Write( Items[i] );
			}

			file.Close();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[ExpansionMarketTrader] Saved settings");
			#endif
		} 
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[ExpansionMarketTrader] Failed to save settings file " + m_FileName );
			#endif
		}
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[" + ClassName() + "] Loading default settings");
		#endif
		
		TraderName = "NA";
		DisplayName = "NA";
		m_FileName = "INVALID-FILE-NAME";
	}

	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	void AddItem( string item )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketTrader::AddItem - Start");
		#endif
		
		item.ToLower();
		if ( GetExpansionSettings().GetMarket().ExistsItem( item ) )
		{
			Items.Insert( item );
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMarketTrader::AddItem - Added item " + item + " to trader " + TraderName + " items array");
			#endif

			return;
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[ExpansionMarketTrader] Error: The \"" + item + "\" does not exist in the market!" );
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ItemExists
	// ------------------------------------------------------------
	bool ItemExists( string item )
	{
		item.ToLower();
		return Items.Find( item ) >= 0;
	}
}