/**
 * TerritoryFlag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class TerritoryFlag
{
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
	protected bool m_RecivedFlag = false;				//! Used to check if flag recived a flag item already when Expansion base building setting "AddFlagItem" is enabled.
	
	// ------------------------------------------------------------
	// TerritoryFlag Constructor
	// ------------------------------------------------------------
	void TerritoryFlag()
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("TerritoryFlag::TerritoryFlag - Start");
		#endif
		
		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		
		m_TerritoryID = -1;
		m_Territory = new ExpansionTerritory(-1, "", 1, "", vector.Zero, "");
		
		RegisterNetSyncVariableInt( "m_TerritoryID" );
		RegisterNetSyncVariableBool( "m_IsTerritory" );
		RegisterNetSyncVariableBool( "m_RecivedFlag" );
		
		//SetEventMask( EntityEvent.INIT );
		
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
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( category_name == "Base" && !HasBase() )
			return !GetExpansionSettings().GetBaseBuilding().EnableSimpleFlagBuilding;
		else if ( category_name == "Support" && HasBase() && !GetConstruction().IsPartConstructed("support") )
			return !GetExpansionSettings().GetBaseBuilding().EnableSimpleFlagBuilding;
		else if ( category_name == "Pole" && GetConstruction().IsPartConstructed("support") && !GetConstruction().IsPartConstructed("pole") )
			return !GetExpansionSettings().GetBaseBuilding().EnableSimpleFlagBuilding;
		else if ( category_name == "Flag" && GetConstruction().IsPartConstructed("pole") )
			return !GetExpansionSettings().GetBaseBuilding().EnableFlagMenu;
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
	void SetTerritory( ref ExpansionTerritory territory )
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
	ref ExpansionTerritory GetTerritory()
	{
		return m_Territory;
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionEnterFlagMenu);
	}
	
	// ------------------------------------------------------------
	// TerritoryFlag IsTerritoryFlag
	// ------------------------------------------------------------
	bool IsTerritoryFlag()
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
	void SetOwnerID(string id)
	{
		m_OwnerID = id;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::SetOwnerID:: - Set flag owner id to: " + m_OwnerID );
		#endif
	}

	// ------------------------------------------------------------
	// TerritoryFlag OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		ctx.Write( m_IsTerritory );
		if ( m_IsTerritory )
		{
			ctx.Write( m_FlagTexturePath );
			ctx.Write( m_OwnerID );
			ctx.Write( m_TerritoryID );		
			
			m_Territory.OnStoreSave(ctx);
			
			ctx.Write( m_RecivedFlag );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if ( ctx.Read( m_IsTerritory ) && m_IsTerritory )
		{
			if ( Expansion_Assert_False( ctx.Read( m_FlagTexturePath ), "[" + this + "] Failed reading m_FlagTexturePath" ) )
				return false;
			
			SetFlagTexture(m_FlagTexturePath);
			//AnimateFlag(1 - GetRefresherTime01());
			
			if ( Expansion_Assert_False( ctx.Read( m_OwnerID ), "[" + this + "] Failed reading m_OwnerID" ) )
				return false;
			
			if ( Expansion_Assert_False( ctx.Read( m_TerritoryID ), "[" + this + "] Failed reading m_TerritoryID" ) )
				return false;
			
			if ( !m_Territory.OnStoreLoad( ctx, GetExpansionSaveVersion() ) )
				return false;
			
			if ( Expansion_Assert_False( ctx.Read( m_RecivedFlag ), "[" + this + "] Failed reading m_RecivedFlag" ) )
				return false;
			
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("TerritoryFlag::OnStoreLoad:: - Loaded data for object <" + this.ToString() +  ">: [m_FlagTexturePath: " + m_FlagTexturePath + ", m_OwnerID: " + m_OwnerID + "]");
			#endif
		}
		
		return true;
	}
	
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
		super.EOnInit(other,extra);
		
		m_TerritoryModule = ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
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
		super.AfterStoreLoad();
				
		if ( m_Territory && m_IsTerritory )
		{
			m_TerritoryModule.AddTerritoryFlag( this, m_Territory.GetTerritoryID() );
		}
		
		SetSynchDirty();
	}
	
	// ------------------------------------------------------------
	// Override EEDelete
	// ------------------------------------------------------------
	override void EEDelete( EntityAI parent )
	{
		super.EEDelete( parent );
		
		if ( m_Territory && m_TerritoryID > -1 && GetGame() && m_TerritoryModule )
		{
			m_TerritoryModule.Exec_DeleteTerritoryAdmin( m_TerritoryID, null );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnPartBuiltServer
	// ------------------------------------------------------------
	override void OnPartBuiltServer( string part_name, int action_id )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - Start" );
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - part_name: " + part_name );
		#endif
		
		if (GetExpansionSettings().GetBaseBuilding().GetTerritoryFlagKitAfterBuild)
		{
			super.OnPartBuiltServer(part_name, action_id);
		}
		else
		{
			ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
			
			//check base state
			if ( constrution_part.IsBase() )
			{
				SetBaseState( true );
			}
				
			//register constructed parts for synchronization
			RegisterPartForSync( constrution_part.GetId() );
			
			//register action that was performed on part
			RegisterActionForSync( constrution_part.GetId(), action_id );
			
			//synchronize
			SynchronizeBaseState();
			
			//if (GetGame().IsMultiplayer() && GetGame().IsServer())
				SetPartFromSyncData(constrution_part); // server part of sync, client will be synced from SetPartsFromSyncData
			
			//update visuals
			UpdateVisuals();
			
			//reset action sync data
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ResetActionSyncData, 100, false, this );
		}
				
		if ( GetExpansionSettings().GetBaseBuilding().AddFlagItem ) 
		{
			if ( part_name == "pole" )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint( "TerritoryFlag::OnPartBuiltServer - Constructed part_name: " + part_name );
				#endif
				
				if ( m_RecivedFlag )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint( "TerritoryFlag::OnPartBuiltServer - Already recived flag item once! Dont continue." );
					#endif
					return;
				}
				
				if ( !FindAttachmentBySlotName("Material_FPole_Flag") )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint( "TerritoryFlag::OnPartBuiltServer - Constructed part_name " + part_name + " and no flag item attached!" );
					#endif
					
					Flag_DayZ flag = Flag_DayZ.Cast( GetInventory().CreateAttachment( "Flag_DayZ" ) );
					if ( flag )
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("TerritoryFlag::OnPartBuiltServer - Flag: " + flag.ToString());
						#endif
						
						m_FlagTexturePath = "dz\\gear\\camping\\Data\\Flag_DAYZ_co.paa";
						
						flag.SetFlagTexture( m_FlagTexturePath );
						m_RecivedFlag = true;
					}
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint( "TerritoryFlag::OnPartBuiltServer - End" );
		#endif
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
		
		if ( GetExpansionSettings().GetBaseBuilding().AddFlagItem && GetExpansionSettings().GetBaseBuilding().EnableFlagMenu )
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
}
