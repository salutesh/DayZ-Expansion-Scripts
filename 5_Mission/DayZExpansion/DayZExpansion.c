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
class DayZExpansion: ExpansionWorld
{
	protected string m_Version;

	protected int m_MajorVersion;
	protected int m_MinorVersion;
	protected int m_BuildVersion;

	protected bool m_IsSpawnMenuOpen;
	
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

		if ( IsMissionClient() )
		{
			//! Version checking
			LoadModVersion();
		}
		
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
	// Expansion LoadModVersion
	// ------------------------------------------------------------
	void LoadModVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::LoadModVersion - Start");
		#endif
		
		array<ref ModInfo> mods = new array<ref ModInfo>;
		GetGame().GetModInfos( mods );
		for (int i = 0; i < mods.Count(); ++i)
		{
			if (mods[i].GetName() == "DayZ Expansion")
			{
				m_Version = mods[i].GetVersion();
			}
		}

		array<string> values = new array<string>;
		m_Version.Split(".", values);

		if (values.Count() == 3)
		{
			m_MajorVersion = values[0].ToInt();
			m_MinorVersion = values[1].ToInt();
			m_BuildVersion = values[2].ToInt();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Major: " + m_MajorVersion + " Minor: " + m_MinorVersion + " Build: " + m_BuildVersion );
			#endif
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::LoadModVersion - End");
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
	// Expansion OnMissionStart
	// ------------------------------------------------------------
	void OnMissionStart()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionStart - Start");
		#endif

		GetExpansionSettings();

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::OnMissionStart - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnMissionLoaded
	// ------------------------------------------------------------
	void OnMissionLoaded()
	{
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
	// Expansion OnMissionFinish
	// ------------------------------------------------------------
	void OnMissionFinish()
	{
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
}

static ref DayZExpansion g_exDayZ;

static ref DayZExpansion GetDayZExpansion()
{
	return g_exDayZ;
}

static void CreateDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - Start");
	#endif
	
	if ( g_exDayZ )
	{
		DestroyDayZExpansion();
	}

	g_exDayZ = new DayZExpansion;

	GetDayZGame().SetExpansionGame( g_exDayZ );
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - End");
	#endif
}

static void DestroyDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - Start");
	#endif
	
	delete g_exDayZ;
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - End");
	#endif
}