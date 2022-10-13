/**
 * ExpansionParkingMeterKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGARAGE
class ExpansionParkingMeterKit extends ExpansionConstructionKitLarge
{
	override bool CanBePlaced(Man player, vector position)
	{
		if (!super.CanBePlaced(player, position))
			return false;
		
		if (GetGame().IsServer())
		{
			//! Make sure player places this in his own territory only.
			ExpansionTerritoryModule territoyModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
			TerritoryFlag flag = territoyModule.GetFlagAtPosition3D(GetPosition());
			if (!flag || flag.GetTerritoryID() == -1)
				return false;
			
			//! Can only place one parking meter per territory
			ExpansionGarageModule garageModule = ExpansionGarageModule.Cast(CF_ModuleCoreManager.Get(ExpansionGarageModule));
			if (garageModule.HasTerritoryParkingMeter(flag.GetTerritoryID()))	
				return false;		
		}
		
		return true;
	}
	
	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete( player, position, orientation );

		if (GetGame().IsServer())
		{
			ExpansionTerritoryModule module = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
			TerritoryFlag flag = module.GetFlagAtPosition3D(GetPosition());
			if (flag && flag.GetTerritoryID() > -1)
			{
				ExpansionParkingMeter parkingMeter = ExpansionParkingMeter.Cast(GetGame().CreateObjectEx("ExpansionParkingMeter", GetPosition(), ECE_PLACE_ON_SURFACE));
				parkingMeter.SetPosition(position);
				parkingMeter.SetOrientation(orientation);
				parkingMeter.SetTerritoryID(flag.GetTerritoryID());
	
				//make the kit invisible, so it can be destroyed from deploy UA when action ends
				HideAllSelections();
	
				SetIsDeploySound(true);
			}
		}
	}
};
#endif
