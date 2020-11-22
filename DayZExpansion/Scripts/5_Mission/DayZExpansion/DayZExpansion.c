/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		DayZExpansion
 * @brief		
 **/
modded class DayZExpansion
{
	protected string m_Version;

	protected int m_MajorVersion;
	protected int m_MinorVersion;
	protected int m_BuildVersion;
	
	// ------------------------------------------------------------
	// DayZExpansion Constructor
	// ------------------------------------------------------------
	void DayZExpansion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::DayZExpansion - Start");
		#endif

		//! Confirm firing
		GetRPCManager().AddRPC( "DayZExpansion", "ConfirmWeaponFire", this, SingeplayerExecutionType.Server );

		//! Version checking
		Expansion_LoadVersion();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::DayZExpansion - End");
		#endif
	}

	// ------------------------------------------------------------
	// DayZExpansion Destructor
	// ------------------------------------------------------------
	void ~DayZExpansion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::~DayZExpansion - Start");
		#endif

		// HandActionThrow.OnThrow.Remove( this.OnThrow );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::DayZExpansion - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Expansion_LoadVersion
	// ------------------------------------------------------------
	void Expansion_LoadVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::Expansion_LoadVersion - Start");
		#endif
		
		m_Version = LoadingScreen.GetClientExpansionVersion();

		array<string> values = new array<string>();
		m_Version.Split( ".", values );
		if ( values.Count() == 3 )
		{
			m_MajorVersion = values[0].ToInt();
			m_MinorVersion = values[1].ToInt();
			m_BuildVersion = values[2].ToInt();

			Print( "Expansion version: " + m_MajorVersion + "." + m_MinorVersion + "." + m_BuildVersion );
		}
		
		string versionTest;
		GetGame().ConfigGetText( "CfgMods DZ_Expansion version", versionTest );
		Print( versionTest );

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::Expansion_LoadVersion - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetVersion
	// ------------------------------------------------------------
	string GetVersion()
	{
		return m_Version;
	}

	// ------------------------------------------------------------
	// Expansion OnStart
	// ------------------------------------------------------------
	override void OnStart()
	{
		super.OnStart();

		GetExpansionSettings();
	}

	// ------------------------------------------------------------
	// Expansion OnLoaded
	// ------------------------------------------------------------
	override void OnLoaded()
	{
		super.OnLoaded();

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionLoaded - Start");
		#endif

		if ( IsMissionHost() )
		{
			ExpansionSettings.SI_General.Invoke();
			ExpansionSettings.SI_Book.Invoke();
			ExpansionSettings.SI_Party.Invoke();
			ExpansionSettings.SI_Map.Invoke();
			ExpansionSettings.SI_SafeZone.Invoke();
			ExpansionSettings.SI_Spawn.Invoke();
			ExpansionSettings.SI_Mission.Invoke();
			ExpansionSettings.SI_Market.Invoke();
			ExpansionSettings.SI_BaseBuilding.Invoke();
			
			GetModuleManager().OnSettingsUpdated();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionLoaded - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnFinish
	// ------------------------------------------------------------
	override void OnFinish()
	{
		super.OnFinish();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionFinish - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionFinish - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ConfirmWeaponFire
	// ------------------------------------------------------------
	void ConfirmWeaponFire( CallType type, ref ParamsReadContext ctx, PlayerIdentity sender, ref Object target )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::ConfirmWeaponFire - Start");
		#endif

		Param1< vector > data;
		if ( !ctx.Read( data ) )
			return;

		if ( type == CallType.Server )
		{
			Weapon_Base weapon = Weapon_Base.Cast( target );
			if ( !weapon )
				return;

			PlayerBase player;

			if ( GetGame().IsMultiplayer() )
			{
				if ( !sender )
					return;

				player = PlayerBase.GetPlayerByUID( sender.GetId() );
			} 
			else
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
			}

			if ( !player || player != weapon.GetHierarchyRootPlayer() )
				return;

			weapon.ExpansionFire( player, data.param1 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::ConfirmWeaponFire - End");
		#endif
	}

	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		if ( g_exGlobalSettings.OnRPC( sender, target, rpc_type, ctx ) )
			return true;

		return super.OnRPC( sender, target, rpc_type, ctx );
	}
};