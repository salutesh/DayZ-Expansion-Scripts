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
	string m_FlagTexturePath = "";
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
	void SetFlagTexture(string texturePath)
	{
		if ( !IsMissionHost() ) return;
		
		m_FlagTexturePath = texturePath;
		SetObjectTexture( 0, m_FlagTexturePath );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::SetFlagTexture:: - Set flag texture path to: " + m_FlagTexturePath);
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTexturePath
	// ------------------------------------------------------------
	string GetFlagTexturePath()
	{
		return m_FlagTexturePath;
	}

	int GetTerritoryMenuID()
	{
		return MENU_EXPANSION_FLAG_MENU;
	}

	string GetTerritoryFlagClass()
	{
		return "ExpansionTerritoryFlag";
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
	// Override SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionEnterFlagMenu );
		AddAction( ExpansionActionDismantleFlag );
	}
	
	// ------------------------------------------------------------
	// Override EOnInit
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra)
	{
		SetAllowDamage( false );
		
		super.EOnInit( other, extra );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		//#ifdef EXPANSIONEXLOGPRINT
		//EXLogPrint("ExpansionFlagBase::OnStoreSave:: - Save data for object <" + this.GetID().ToString() +  ">: [m_FlagTextureID: " + m_FlagTextureID + ", m_OwnerID: " + m_OwnerID + "]");
		//#endif
		ctx.Write( m_FlagTexturePath );
		ctx.Write( m_OwnerID );
	}
	
	// ------------------------------------------------------------
	// Override OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( GetExpansionSaveVersion() >= 3 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_FlagTexturePath ), "[" + this + "] Failed reading m_FlagTexturePath" ) )
				return false;
			
			SetFlagTexture(m_FlagTexturePath);
		} else
		{
			int flagTextureID
			if ( Expansion_Assert_False( ctx.Read( flagTextureID ), "[" + this + "] Failed reading flagTextureID" ) )
				return false;
			
			string texturePath = GetExpansionStatic().GetFlagTexturePath( flagTextureID );
		
			SetFlagTexture(texturePath);
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_OwnerID ), "[" + this + "] Failed reading m_OwnerID" ) )
			return false;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagBase::OnStoreLoad:: - Loaded data for object <" + this.ToString() +  ">: [m_FlagTexturePath: " + m_FlagTexturePath + ", m_OwnerID: " + m_OwnerID + "]");
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
		
	// ------------------------------------------------------------
	// Override AfterStoreLoad
	// ------------------------------------------------------------	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		#ifdef DAYZ_1_09
		GetInventory().ReplaceItemWithNew( InventoryMode.SERVER, new ExpansionReplaceTerritoryWithNewLambda( this, "TerritoryFlag", NULL ) );
		#endif
	}
}