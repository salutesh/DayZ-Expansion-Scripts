/**
 * MetalWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MetalWire
{
	override void UpdateAllSelections()
	{
		super.UpdateAllSelections();
		
		EntityAI energy_source = GetCompEM().GetEnergySource();
		if (energy_source && energy_source.IsAnyInherited({ExpansionHelicopterBattery, ExpansionAircraftBattery}))
		{
			EntityAI powered_device = GetCompEM().GetPluggedDevice();
			if (powered_device)
			{
				// Show metal wire selection
				string selection_wire = SEL_WIRE_PREFIX + "battery_truck" + SEL_WIRE_SUFIX;
				ShowSelection(selection_wire);
				
				// Show plug selection
				string selection_plug = SEL_WIRE_PREFIX + "battery_truck" + SEL_PLUG_SUFIX;
				selection_plug.ToLower();
				ShowSelection(selection_plug);
			
				// Set plug's texture
				int selection_index = GetHiddenSelectionIndex(selection_plug);
				string texture_path = powered_device.GetCompEM().GetCordTextureFile();
				SetObjectTexture( selection_index, texture_path );
			}
			else
			{
				// Show metal wire selection
				string selection_wire2 = SEL_WIRE_PREFIX +  "battery_truck";
				ShowSelection(selection_wire2);
			}
		}
	}
}
