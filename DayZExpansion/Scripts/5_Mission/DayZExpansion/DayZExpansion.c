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
	
	ref ExpansionStartingClothing m_ExpansionClothing;
	
	int m_topexp = -1;
	int m_shoesexp = -1;
	int m_bottomexp = -1;
	int m_headgearexp = -1;
	int m_glassesexp = -1;
	int m_maskexp = -1;
	int m_vestexp = -1;
	int m_glovesexp = -1;
	int m_beltexp = -1;
	int m_bandexp = -1;
	int m_backpackexp = -1;
	
	PlayerBase m_player;
	
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
			/*ExpansionSettings.SI_General.Invoke();
			ExpansionSettings.SI_Book.Invoke();
			ExpansionSettings.SI_Party.Invoke();
			ExpansionSettings.SI_Map.Invoke();
			ExpansionSettings.SI_SafeZone.Invoke();
			ExpansionSettings.SI_Spawn.Invoke();
			ExpansionSettings.SI_Mission.Invoke();
			ExpansionSettings.SI_Market.Invoke();
			ExpansionSettings.SI_BaseBuilding.Invoke();*/
			
			ExpansionSettings.SI_Airdrop.Invoke();
			ExpansionSettings.SI_Book.Invoke();
			ExpansionSettings.SI_BaseBuilding.Invoke();
			ExpansionSettings.SI_General.Invoke();
			ExpansionSettings.SI_Map.Invoke();
			ExpansionSettings.SI_Market.Invoke();
			ExpansionSettings.SI_Mission.Invoke();
			ExpansionSettings.SI_Notification.Invoke();
			ExpansionSettings.SI_Party.Invoke();
			ExpansionSettings.SI_Raid.Invoke();
			ExpansionSettings.SI_SafeZone.Invoke();
			ExpansionSettings.SI_Spawn.Invoke();
			ExpansionSettings.SI_Territory.Invoke();
			ExpansionSettings.SI_Vehicle.Invoke();
			ExpansionSettings.SI_Debug.Invoke();

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
	
	void ExpansionEquipCharacter(PlayerBase player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::EquipCharacter - UseCustomClothing == TRUE");
		#endif
		
		m_player = player;
		if (Class.CastTo(m_ExpansionClothing, GetExpansionSettings().GetSpawn().StartingClothing))
		{
			if (m_ExpansionClothing.Headgear.Count() > 0)
			{
				if (m_ExpansionClothing.Headgear.Count() == 1)
				{
					m_headgearexp = 0;
				}
				else if (m_ExpansionClothing.Headgear.Count() > 1)
				{
					m_headgearexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Headgear.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_headgearexp: " + m_headgearexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Glasses.Count() > 0)
			{
				if (m_ExpansionClothing.Glasses.Count() == 1)
				{
					m_glassesexp = 0;
				}
				else if (m_ExpansionClothing.Glasses.Count() > 1)
				{
					m_glassesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Glasses.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_glassesexp: " + m_glassesexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Masks.Count() > 0)
			{
				if (m_ExpansionClothing.Masks.Count() == 1)
				{
					m_maskexp = 0;
				}
				else if (m_ExpansionClothing.Masks.Count() > 1)
				{
					m_maskexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Masks.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_maskexp: " + m_maskexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Tops.Count() > 0)
			{
				if (m_ExpansionClothing.Tops.Count() == 1)
				{
					m_topexp = 0;
				}
				else if (m_ExpansionClothing.Tops.Count() > 1)
				{
					m_topexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Tops.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_topexp: " + m_topexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Vests.Count() > 0)
			{
				if (m_ExpansionClothing.Vests.Count() == 1)
				{
					m_vestexp = 0;
				}
				else if (m_ExpansionClothing.Vests.Count() > 1)
				{
					m_vestexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Vests.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_vestexp: " + m_vestexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Gloves.Count() > 0)
			{
				if (m_ExpansionClothing.Gloves.Count() == 1)
				{
					m_glovesexp = 0;
				}
				else if (m_ExpansionClothing.Gloves.Count() > 1)
				{
					m_glovesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Gloves.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_glovesexp: " + m_glovesexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Pants.Count() > 0)
			{
				if (m_ExpansionClothing.Pants.Count() == 1)
				{
					m_bottomexp = 0;
				}
				else if (m_ExpansionClothing.Pants.Count() > 1)
				{
					m_bottomexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Pants.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_bottomexp: " + m_bottomexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Belts.Count() > 0)
			{
				if (m_ExpansionClothing.Belts.Count() == 1)
				{
					m_beltexp = 0;
				}
				else if (m_ExpansionClothing.Belts.Count() > 1)
				{
					m_beltexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Belts.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_beltexp: " + m_beltexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Shoes.Count() > 0)
			{
				if (m_ExpansionClothing.Shoes.Count() == 1)
				{
					m_shoesexp = 0;
				}
				else if (m_ExpansionClothing.Shoes.Count() > 1)
				{
					m_shoesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Shoes.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_shoesexp: " + m_shoesexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Armbands.Count() > 0)
			{
				if (m_ExpansionClothing.Armbands.Count() == 1)
				{
					m_bandexp = 0;
				}
				else if (m_ExpansionClothing.Armbands.Count() > 1)
				{
					m_bandexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Armbands.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_bandexp: " + m_bandexp.ToString());
				#endif
			}
			
			if (m_ExpansionClothing.Backpacks.Count() > 0)
			{
				if (m_ExpansionClothing.Backpacks.Count() == 1)
				{
					m_backpackexp = 0;
				}
				else if (m_ExpansionClothing.Backpacks.Count() > 1)
				{
					m_backpackexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Backpacks.Count() );
				}
				
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_backpackexp: " + m_backpackexp.ToString());
				#endif
			}
			
			if (m_headgearexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Headgear: " + m_ExpansionClothing.Headgear.Get( m_headgearexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Headgear.Get(m_headgearexp), InventorySlots.HEADGEAR);
			}
			
			if (m_glassesexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Glasses: " + m_ExpansionClothing.Glasses.Get( m_glassesexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Glasses.Get(m_glassesexp), InventorySlots.EYEWEAR);
			}
			
			if (m_maskexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Masks: " + m_ExpansionClothing.Masks.Get( m_maskexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Masks.Get(m_maskexp), InventorySlots.MASK);
			}
			
			if (m_topexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Tops: " + m_ExpansionClothing.Tops.Get( m_topexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Tops.Get(m_topexp), InventorySlots.BODY);
			}
			
			if (m_vestexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Vests: " + m_ExpansionClothing.Vests.Get( m_vestexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Vests.Get(m_vestexp), InventorySlots.VEST);
			}
			
			if (m_glovesexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Gloves: " + m_ExpansionClothing.Gloves.Get( m_glovesexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Gloves.Get(m_glovesexp), InventorySlots.GLOVES);
			}
			
			if (m_bottomexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Pants: " + m_ExpansionClothing.Pants.Get( m_bottomexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Pants.Get(m_bottomexp), InventorySlots.LEGS);
			}
			
			if (m_beltexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Belts: " + m_ExpansionClothing.Belts.Get( m_beltexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Belts.Get(m_beltexp), InventorySlots.HIPS);
			}
			
			if (m_shoesexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Shoes: " + m_ExpansionClothing.Shoes.Get( m_shoesexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Shoes.Get(m_shoesexp), InventorySlots.FEET);
			}
			
			if (m_bandexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Armbands: " + m_ExpansionClothing.Armbands.Get( m_bandexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Armbands.Get(m_bandexp), InventorySlots.ARMBAND);
			}
			
			if (m_backpackexp != -1)
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZExpansion::EquipCharacter - m_ExpansionClothing.Backpacks: " + m_ExpansionClothing.Backpacks.Get( m_backpackexp ));
				#endif
				m_player.GetInventory().CreateAttachmentEx(m_ExpansionClothing.Backpacks.Get(m_backpackexp), InventorySlots.BACK);
			}
		}
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