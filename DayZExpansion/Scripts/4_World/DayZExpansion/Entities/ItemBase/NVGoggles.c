/**
 * NVGoggles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		NVGoggles
 * @brief		This class handles modifications on the night-vision googles item
 **/

enum ExpansionNVGRPCs
{
	SyncBatteryState
}

modded class NVGoggles
{
	int	m_BatteryEnergy;
	
	// ------------------------------------------------------------
	// NVGoggles Constructor
	// ------------------------------------------------------------
	void NVGoggles()
	{
		RegisterNetSyncVariableInt("m_BatteryEnergy", 0, 100);
	}
	
	// ------------------------------------------------------------
	// Expansion GetBatteryEnergy
	// ------------------------------------------------------------
	int GetBatteryEnergy()
	{
		return m_BatteryEnergy;
	}
	
	// ------------------------------------------------------------
	// Override OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
		super.OnWork(consumed_energy);
		
		ItemBase battery;
		battery = ItemBase.Cast( FindAttachmentBySlotName("BatteryD") );
		
		if ( GetGame().IsServer() )
		{
			if ( battery )
			{
				SetBatteryEnergy( battery.GetCompEM().GetEnergy0To100() );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetBatteryEnergy
	// ------------------------------------------------------------
	void SetBatteryEnergy(int percent)
	{
		m_BatteryEnergy = percent;
		SetSynchDirty();
	}
}