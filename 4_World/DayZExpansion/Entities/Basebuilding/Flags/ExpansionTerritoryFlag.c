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
	protected ref ExpansionTerritory m_Territory;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
    void ExpansionTerritoryFlag()
    {
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::ExpansionTerritoryFlag - Start");
		#endif

		m_TerritoryID = -1;
		m_Territory = NULL;
		
		RegisterNetSyncVariableInt( "m_TerritoryID" );
		SetEventMask( EntityEvent.INIT );
		
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
		ctx.Write( m_Territory );
		
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
				
		if ( !ctx.Read( m_TerritoryID ) )
			return false;
		
		if ( !ctx.Read( m_Territory ) )
			return false;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad:: - Loaded data for object " + this.ToString() +  ": [m_FlagTextureID: " + m_FlagTextureID + ", m_TerritoryID: " + m_TerritoryID.ToString() + ", m_OwnerID: " + m_OwnerID + "]");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoryFlag::OnStoreLoad - End");
		#endif

		return true;
	}
	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
				
		if ( m_Territory )
			m_TerritoryModule.AddTerritoryFlag( this, m_Territory.GetTerritoryID() );
		
		SetSynchDirty();
	}
	
	override void EEDelete( EntityAI parent )
	{
		super.EEDelete( parent );
		
		if ( m_Territory && m_TerritoryID > -1 && GetGame() && m_TerritoryModule )
		{
			m_TerritoryModule.Exec_DeleteTerritoryAdmin( m_TerritoryID, null );
		}
	}
}