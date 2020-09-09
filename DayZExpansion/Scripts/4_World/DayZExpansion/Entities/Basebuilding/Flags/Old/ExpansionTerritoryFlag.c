/**
 * ExpansionTerritoryFlag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTerritoryFlag
 * @brief		
 **/

class ExpansionTerritoryFlag extends ExpansionFlagBase
{
	protected int m_TerritoryID;
	protected autoptr ExpansionTerritory m_Territory;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionTerritoryFlag()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::ExpansionTerritoryFlag - Start");
		#endif

		m_TerritoryID = -1;
		m_Territory = new ExpansionTerritory(-1, "", 1, "", vector.Zero, "");
		
		RegisterNetSyncVariableInt( "m_TerritoryID" );
		SetEventMask( EntityEvent.INIT );
		
		//Make it invicible
		SetAllowDamage(false);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::ExpansionTerritoryFlag - End");
		#endif
	}	
	
	void ~ExpansionTerritoryFlag()
	{
		if ( m_TerritoryModule && m_TerritoryID > -1 )
		{
			m_TerritoryModule.RemoveTerritoryFlag( m_TerritoryID );
		}
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
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::SetTerritoryID - Set territory id to " + m_TerritoryID.ToString() + " for territory flag " + this.ToString() + ".");
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
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::SetTerritory - Set territory to " + territory.ToString() + " [ID:" + m_Territory.GetTerritoryID() + ", Name: " + m_Territory.GetTerritoryName() + " OwnerID: " + m_Territory.GetOwnerID() + "] for territory flag " + this.ToString() + ".");
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
	// Expansion IsTerritoryFlag
	// ------------------------------------------------------------
	override bool IsTerritoryFlag()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion EOnInit
	// ------------------------------------------------------------
	override void EOnInit(IEntity other, int extra)
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
	}

	// ------------------------------------------------------------
	// Override OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreSave - Start");
		#endif
		
		//! Saves flag owner and texture from ExpansionFlagBase
		super.OnStoreSave( ctx );
		
		ctx.Write( m_TerritoryID );
		
		if (GetExpansionSaveVersion() >= 3)
		{
			m_Territory.OnStoreSave(ctx);
		}
		else
		{
			ctx.Write( m_Territory );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreSave - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad - Start");
		#endif
		
		//! Loads flag owner and texture from ExpansionFlagBase
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_TerritoryID ), "[" + this + "] Failed reading m_TerritoryID" ) )
			return false;
		
		if ( GetExpansionSaveVersion() >= 3 )
		{
			if ( !m_Territory.OnStoreLoad( ctx, GetExpansionSaveVersion() ) )
				return false;
		} else
		{
			ExpansionOldTerritory oldTerritory;
			if ( !ctx.Read( oldTerritory ) )
				return false;
			
			string texturePath = GetExpansionStatic().GetFlagTexturePath(oldTerritory.TerritoryFlagTextureID);
			
			ExpansionTerritory newTerritory = new ExpansionTerritory(oldTerritory.TerritoryID, oldTerritory.TerritoryName, oldTerritory.TerritoryLevel, oldTerritory.TerritoryOwnerID, oldTerritory.TerritoryPosition, texturePath);
			newTerritory.SetMembers(oldTerritory.TerritoryMembers);
			newTerritory.SetInvites(oldTerritory.Invites);
			m_Territory = newTerritory;
		
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad:: - Loaded data for object " + this.ToString() +  ": [m_FlagTextureID: " + oldTerritory.TerritoryFlagTextureID + ", m_TerritoryID: " + m_TerritoryID.ToString() + ", m_OwnerID: " + m_OwnerID + "]");
			#endif
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		//EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad:: - Loaded data for object " + this.ToString() +  ": [m_FlagTextureID: " + m_FlagTextureID + ", m_TerritoryID: " + m_TerritoryID.ToString() + ", m_OwnerID: " + m_OwnerID + "]");
		//#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Override AfterStoreLoad
	// ------------------------------------------------------------	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
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
}