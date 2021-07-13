/**
 * ExpansionCamoBox.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCamoBox
 * @brief		
 **/
//! TODO: Is there a good reason this inherits from ExpansionBaseBuilding and not DeployableContainer_Base or similar?
class ExpansionCamoBox: ExpansionBaseBuilding
{
	int m_CanBeDamaged = -1;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionCamoBox()
	{
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionCamoBox()
	{
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo (EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return true;
	}

	// ------------------------------------------------------------
	// GetConstructionKitType
	// ------------------------------------------------------------
	override string GetConstructionKitType()
	{
		return "ExpansionCamoBoxKit";
	}

	// ------------------------------------------------------------
	// CanBeDamaged
	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		if ( m_CanBeDamaged == -1 && GetGame() && GetGame().IsMultiplayer() && GetGame().IsServer() )
			m_CanBeDamaged = GetGame().ServerConfigGetInt( "disableContainerDamage" ) == 0;

		return m_CanBeDamaged;
	}

	// ------------------------------------------------------------
	// AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		
	}
}