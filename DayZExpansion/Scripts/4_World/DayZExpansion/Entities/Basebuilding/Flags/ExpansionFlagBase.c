/**
 * ExpansionFlagBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagBase
 * @brief		
 **/

class ExpansionFlagBase extends ItemBase
{
	int m_FlagTextureID = 0;
	string m_OwnerID = "";

	protected ExpansionTerritoryModule m_TerritoryModule;
	
	// ------------------------------------------------------------
	// ExpansionFlagBase Constructor
	// ------------------------------------------------------------
	void ExpansionFlagBase()
    {
		SetEventMask( EntityEvent.INIT );
		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );
	}
	
	// ------------------------------------------------------------
	// Expansion IsTerritoryFlag
	// ------------------------------------------------------------
	bool IsTerritoryFlag()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetTexture
	// ------------------------------------------------------------
	void SetFlagTexture(int id)
	{
		if ( !IsMissionHost() ) return;
		
		m_FlagTextureID = id;
		string path = GetFlagTexturePath( id );
		SetObjectTexture( 0, path );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::SetFlagTexture:: - Set flag texture path to: " + m_FlagTextureID);
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTextureID
	// ------------------------------------------------------------
	int GetFlagTextureID()
	{
		return m_FlagTextureID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetOwnerID
	// ------------------------------------------------------------
	string GetOwnerID()
	{
		return m_OwnerID;
	}
	
	// ------------------------------------------------------------
	// Expansion SetOwnerID
	// ------------------------------------------------------------
	void SetOwnerID(string id)
	{
		m_OwnerID = id;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionFlagBase::SetOwnerID:: - Set flag owner id to: " + m_OwnerID );
		#endif
	}
	
	// ------------------------------------------------------------
	// SetFlagTextureID
	// ------------------------------------------------------------
	void SetFlagTextureID(int id)
	{
		m_FlagTextureID = id;
	}
	
	// ------------------------------------------------------------
	// Override SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionEnterFlagMenu );
	}
	
	// ------------------------------------------------------------
	// Override EOnInit
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra)
	{
		super.EOnInit( other, extra );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::OnStoreSave:: - Save data for object <" + this.GetID().ToString() +  ">: [m_FlagTextureID: " + m_FlagTextureID + ", m_OwnerID: " + m_OwnerID + "]");
		#endif
		ctx.Write( m_FlagTextureID );
		ctx.Write( m_OwnerID );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( !ctx.Read( m_FlagTextureID ) )
			return false;
		
		if ( !ctx.Read( m_OwnerID ) )
			return false;
		
		SetFlagTexture( m_FlagTextureID );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::OnStoreLoad:: - Loaded data for object <" + this.ToString() +  ">: [m_FlagTextureID: " + m_FlagTextureID + ", m_OwnerID: " + m_OwnerID + "]");
		#endif
		
		return true;
	}
	
	// ------------------------------------------------------------
	// Override IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return false;
	}

	// ------------------------------------------------------------
	// Override IsHeavyBehaviour
	// ------------------------------------------------------------
	override bool IsHeavyBehaviour()
	{
        return true;
    }

	// ------------------------------------------------------------
	// Override IsTwoHandedBehaviour
	// ------------------------------------------------------------
    override bool IsTwoHandedBehaviour()
	{
        return false;
    }

	// ------------------------------------------------------------
	// Override CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
    {
		return false;
    }

	// ------------------------------------------------------------
	// Override CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo(EntityAI parent)
    {
		return false;
    }
	
	// ------------------------------------------------------------
	// Override OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player )
    {
        super.OnPlacementComplete( player );
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
}