/**
 * TerritoryFlag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class TerritoryFlag
{
	private static ref set<TerritoryFlag> m_Expansion_TerritoryFlags = new set< TerritoryFlag >;

	//! Vanilla META DATA
	const float MAX_ACTION_DETECTION_ANGLE_RAD = 1.3;	//1.3 RAD = ~75 DEG
	const float MAX_ACTION_DETECTION_DISTANCE = 2.0;	//meters

	//! Expansion META DATA
	string m_FlagTexturePath = "";						//! Flag texture path (get saved to flag (this) and attached flag (flag_base).
	string m_OwnerID = "";								//! Player id who placed the flag (is flag owner).
	
	protected ExpansionTerritoryModule m_TerritoryModule;
	
	protected int m_TerritoryID;						//! Unique terriotry id. Used to get and identify the flags territory in the territory module.
	protected autoptr ExpansionTerritory m_Territory;	//! Contains flags ExpansionTerritoryx data if flag is a expansion territory flag.
	protected bool m_IsTerritory = false;				//! Used to check if flag is a expansion territory flag.

	protected bool m_SkipSetRefresherActive;

	// ------------------------------------------------------------
	// TerritoryFlag Constructor
	// ------------------------------------------------------------
	void TerritoryFlag()
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::TerritoryFlag - Start");
		#endif
		
		CF_Modules<ExpansionTerritoryModule>.Get(m_TerritoryModule);
		
		m_TerritoryID = -1;
		m_Territory = new ExpansionTerritory(-1, "", 1, "", vector.Zero, "");
		
		RegisterNetSyncVariableInt( "m_TerritoryID" );
		RegisterNetSyncVariableBool( "m_IsTerritory" );
		
		//SetEventMask( EntityEvent.INIT );

		m_Expansion_TerritoryFlags.Insert( this );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::TerritoryFlag - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag Destructor
	// ------------------------------------------------------------
	void ~TerritoryFlag()
	{
		if ( m_TerritoryModule && m_TerritoryID > -1 )
		{
			m_TerritoryModule.RemoveTerritoryFlag( m_TerritoryID );
		}

		if (!GetGame())
			return;

		int i = m_Expansion_TerritoryFlags.Find( this );
		if ( i >= 0 )
		{
			m_Expansion_TerritoryFlags.Remove( i );
		}
	}
	
	static set<TerritoryFlag> ExpansionGetAll()
	{
		return m_Expansion_TerritoryFlags;
	}

	override void OnCEUpdate()
	{
		//! This is a bit ugly, but I see no other way...

		//! Remember time remaining...
		int refresherTimeRemaining = m_RefresherTimeRemaining;

		//! ...then set it to zero so vanilla TerritoryFlag::OnCEUpdate doesn't update refresh time and lifetimes
		m_RefresherTimeRemaining = 0;

		m_SkipSetRefresherActive = true;

		super.OnCEUpdate();

		//! Restore time remaining
		m_RefresherTimeRemaining = refresherTimeRemaining;

		m_SkipSetRefresherActive = false;

		//! Update refresher (duplicate of vanilla code except using TerritorySize instead of constant)
		int time_elapsed_rounded = Math.Round(m_ElapsedSinceLastUpdate);
		
		if ( m_RefresherTimeRemaining > 0 )
		{
			m_RefreshTimeCounter += time_elapsed_rounded;
			if ( m_RefreshTimeCounter >= m_FlagRefresherFrequency )
			{
				GetCEApi().RadiusLifetimeReset(GetPosition(),GetExpansionSettings().GetTerritory().TerritorySize);
				m_RefresherTimeRemaining = Math.Clamp(m_RefresherTimeRemaining - m_RefreshTimeCounter,0,m_FlagRefresherMaxDuration);
				m_RefreshTimeCounter = 0;
				AnimateFlag( 1 - GetRefresherTime01() );
			}
		}
		
		SetRefresherActive(m_RefresherTimeRemaining > 0);
	}
	
	override void SetRefresherActive(bool state)
	{
		if ( m_SkipSetRefresherActive )
			return;

		//! Set refresher active state (duplicate of vanilla code except using TerritorySize instead of constant)
		if ( m_RefresherActive != state )
		{
			m_RefresherActive = state;
			SetSynchDirty();
			
			GetCEApi().RadiusLifetimeReset(GetPosition(),GetExpansionSettings().GetTerritory().TerritorySize);
		}
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( category_name == "Base" && !HasBase() )
			return !GetExpansionSettings().GetBaseBuilding().SimpleTerritory;
		else if ( category_name == "Support" && HasBase() && !GetConstruction().IsPartConstructed("support") )
			return !GetExpansionSettings().GetBaseBuilding().SimpleTerritory;
		else if ( category_name == "Pole" && GetConstruction().IsPartConstructed("support") && !GetConstruction().IsPartConstructed("pole") )
			return !GetExpansionSettings().GetBaseBuilding().SimpleTerritory;
		else if ( category_name == "Flag" && GetConstruction().IsPartConstructed("pole") )
			if( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Enabled )
				return true;
			else
				return false;
		else
			return false;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryID
	// ------------------------------------------------------------
	int GetTerritoryID()
	{
		return m_TerritoryID;
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryID
	// ------------------------------------------------------------
	void SetTerritoryID( int id )
	{
		if ( !IsMissionHost() )
			return;
		
		m_TerritoryID = id;
		
		SetSynchDirty();
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::SetTerritoryID - Set territory id to " + m_TerritoryID.ToString() + " for territory flag " + this.ToString() + ".");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritory
	// ------------------------------------------------------------
	void SetTerritory(  ExpansionTerritory territory )
	{
		if ( !IsMissionHost() )
			return;

		m_Territory = territory;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::SetTerritory - Set territory to " + territory.ToString() + " [ID:" + m_Territory.GetTerritoryID() + ", Name: " + m_Territory.GetTerritoryName() + " OwnerID: " + m_Territory.GetOwnerID() + "] for territory flag " + this.ToString() + ".");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritory
	// ------------------------------------------------------------
	ExpansionTerritory GetTerritory()
	{
		return m_Territory;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction( ExpansionActionEnterFlagMenu );
		AddAction( ExpansionActionDismantleFlag );
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag HasExpansionTerritoryInformation
	// ------------------------------------------------------------
	bool HasExpansionTerritoryInformation()
	{
		return m_IsTerritory;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag SetTexture
	// ------------------------------------------------------------
	void SetFlagTexture(string texturePath)
	{
		if ( !IsMissionHost() )
			return;
		
		m_FlagTexturePath = texturePath;
		
		Flag_Base flag = Flag_Base.Cast( FindAttachmentBySlotName("Material_FPole_Flag") );
		if ( flag )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("TerritoryFlag::SetFlagTexture - Flag: " + flag.ToString());
			#endif

			flag.SetFlagTexture( m_FlagTexturePath );
		} else
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("TerritoryFlag::SetFlagTexture - [ERROR] Flag is NULL!");
			#endif
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::SetFlagTexture - Set flag texture path to: " + m_FlagTexturePath);
		#endif
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag GetFlagTexturePath
	// ------------------------------------------------------------
	string GetFlagTexturePath()
	{
		return m_FlagTexturePath;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag GetTerritoryMenuID
	// ------------------------------------------------------------
	int GetTerritoryMenuID()
	{
		return MENU_EXPANSION_FLAG_MENU;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag GetTerritoryFlagClass
	// ------------------------------------------------------------
	string GetTerritoryFlagClass()
	{
		return "TerritoryFlag";
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag GetOwnerID
	// ------------------------------------------------------------
	string GetOwnerID()
	{
		return m_OwnerID;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag SetOwnerID
	// ------------------------------------------------------------
	void SetOwnerID( string id )
	{
		m_OwnerID = id;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::SetOwnerID:: - Set flag owner id to: " + m_OwnerID );
		#endif
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_IsTerritory);
		if (m_IsTerritory)
		{
			ctx.Write(m_FlagTexturePath);
			ctx.Write(m_OwnerID);
			ctx.Write(m_TerritoryID);		
			
			m_Territory.OnStoreSave(ctx);
		}
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_IsTerritory))
			return false;

		if (m_IsTerritory)
		{
			if (!ctx.Read(m_FlagTexturePath))
				return false;

			if (!ctx.Read(m_OwnerID))
				return false;

			if (!ctx.Read(m_TerritoryID))
				return false;

			if (!m_Territory.OnStoreLoad(ctx))
				return false;
		}

		return true;
	}
	#endif
	
	// ------------------------------------------------------------
	// Expansion IsInTerritory
	// Check if flag is in a territory
	// ------------------------------------------------------------
	bool IsInTerritory()
	{
		if ( !m_TerritoryModule )
			return false;
			
		return m_TerritoryModule.IsInTerritory( GetPosition() );
	}
	
	// ------------------------------------------------------------
	// Override EOnInit
	// ------------------------------------------------------------
	override void EOnInit(IEntity other, int extra)
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast( CF_ModuleCoreManager.Get( ExpansionTerritoryModule ) );
	}
	
	// ------------------------------------------------------------
	// Expansion SetIsExpansionTerritoryFlag
	// ------------------------------------------------------------
	void SetIsExpansionTerritoryFlag(bool state)
	{
		if ( !IsMissionHost() )
			return;
		
		m_IsTerritory = state;
		
		SetSynchDirty();
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::SetIsExpansionTerritoryFlag - Set flag m_IsTerritory: " + m_IsTerritory.ToString() );
		#endif
	}
	
	// ------------------------------------------------------------
	// Override AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

		super.AfterStoreLoad();
				
		if ( m_Territory && m_IsTerritory )
			m_TerritoryModule.AddTerritoryFlag( this, m_Territory.GetTerritoryID() );
		
		SetSynchDirty();
	}
	
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

		//! Fix state if flag attachment is not present
		int slot = InventorySlots.GetSlotIdFromString("Material_FPole_Flag");
		if ( m_RefresherActive && !GetInventory().FindAttachment( slot ) )
		{
			if ( GetExpansionSettings().GetBaseBuilding().AutomaticFlagOnCreation )
			{
				EXPrint(ToString() + "::EEOnAfterLoad " + GetPosition() + " - creating missing flag");
				bool locked = GetInventory().GetSlotLock( slot );
				GetInventory().SetSlotLock( slot, false );
				Flag_Base flag = Flag_Base.Cast( GetInventory().CreateAttachment( "Flag_DayZ" ) );
				if ( flag )
				{
					flag.SetFlagTexture( m_FlagTexturePath );
					GetInventory().SetSlotLock( slot, locked );
				}
			}
			else
			{
				ExpansionResetTerritoryFlag();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Override EEDelete
	// ------------------------------------------------------------
	override void EEDelete( EntityAI parent )
	{		
		if ( m_Territory && m_TerritoryID > -1 && GetGame() && m_TerritoryModule )
			m_TerritoryModule.Exec_DeleteTerritoryAdmin( m_TerritoryID, null );
			
		super.EEDelete( parent );
	}
	
	// ------------------------------------------------------------
	// Override OnPartBuiltServer
	// ------------------------------------------------------------
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - Start" );
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - part_name: " + part_name );
		#endif

		super.OnPartBuiltServer(player, part_name, action_id);

		if ( GetExpansionSettings().GetBaseBuilding().AutomaticFlagOnCreation && part_name == "pole" ) 
		{
			if ( !FindAttachmentBySlotName("Material_FPole_Flag") )
			{
				Flag_DayZ flag = Flag_DayZ.Cast( GetInventory().CreateAttachment( "Flag_DayZ" ) );
				if ( flag )
				{
					if ( !m_FlagTexturePath )
						m_FlagTexturePath = "dz\\gear\\camping\\Data\\Flag_DAYZ_co.paa";
					
					flag.SetFlagTexture( m_FlagTexturePath );
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - End" );
		#endif
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );

		if ( part_name == "pole" )
		{
			ExpansionResetTerritoryFlag();

			Flag_Base flag = Flag_Base.Cast( FindAttachmentBySlotName( "Material_FPole_Flag" ) );
			if ( flag )
			{
				if ( GetExpansionSettings().GetBaseBuilding().AutomaticFlagOnCreation )
					flag.Delete();
				else
					flag.ExpansionDropServer( PlayerBase.Cast( player ) );
			}
		}
	}
	
	void ExpansionResetTerritoryFlag()
	{
		//! Unlock flag slot and reset raised state
		AnimateFlag( 1 );

		//! Reset refresher
		SetRefreshTimer01( 0 );
		SetRefresherActive( false );
	}

	override ItemBase CreateConstructionKit()
	{
		if ( GetExpansionSettings().GetBaseBuilding().GetTerritoryFlagKitAfterBuild )
			return super.CreateConstructionKit();

		return NULL;
	}

	// ------------------------------------------------------------
	// Override CanReleaseAttachment
	// ------------------------------------------------------------
	override bool CanReleaseAttachment( EntityAI attachment )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::CanReleaseAttachment - Start" );
		#endif
		
		if( !super.CanReleaseAttachment( attachment ) )
			return false;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::CanReleaseAttachment - Attachment Classname: " + attachment.ClassName() );
		EXLogPrint( "TerritoryFlag::CanReleaseAttachment - Attachment Type: " + attachment.GetType() );
		#endif
		
		if ( GetExpansionSettings().GetBaseBuilding().AutomaticFlagOnCreation &&  ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Enabled || GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.NoFlagChoice ) )
		{
			if ( attachment.IsInherited( Flag_Base ) )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint( "TerritoryFlag::CanReleaseAttachment - End and return false!" );
				#endif
				return false;
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::CanReleaseAttachment - End and return true!" );
		#endif

		return true;
	}
};