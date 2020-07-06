/**
 * ExpansionBreader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBreader extends Inventory_Base
{
	const int SPAWNING_EGG_TIMER = 2;
	protected ref Timer m_SpawnTimer;

	void ExpansionBreader()
	{
		m_SpawnTimer = new Timer();
	}

	void ~ExpansionBreader() 
	{
		if (m_SpawnTimer && m_SpawnTimer.IsRunning())
			m_SpawnTimer.Stop();
	}

	void SpawnEgg()
	{
		GetInventory().CreateInInventory("ExpansionEgg");
	}

	override void AfterStoreLoad()
	{
		if ( FindAttachmentBySlotName("Chicken") )
		{
			m_SpawnTimer.Run(SPAWNING_EGG_TIMER, this, "SpawnEgg", NULL, true);
		}
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached( item, slot_name );

		if ( slot_name == "Chicken" && GetGame().IsServer() )
		{
			ExpansionChicken item_chicken = ExpansionChicken.Cast( item );
			if ( item_chicken )
			{
				m_SpawnTimer.Run(SPAWNING_EGG_TIMER, this, "SpawnEgg", NULL, true);
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached( item, slot_name );

		if ( slot_name == "Chicken" && GetGame().IsServer() )
		{
			if (m_SpawnTimer && m_SpawnTimer.IsRunning())
				m_SpawnTimer.Stop();
		}
	}

	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	override bool CanReceiveItemIntoCargo (EntityAI cargo)
	{
		return super.CanReceiveItemIntoCargo( cargo ) && cargo.IsKindOf("ExpansionEgg");
	}
}