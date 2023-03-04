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
class ExpansionParkingMeterKit: ExpansionConstructionKitLarge
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
			auto setting = GetExpansionSettings().GetGarage();
			if (setting.GarageMode == ExpansionGarageMode.Territory)
			{
				if (!flag || flag.GetTerritoryID() == -1)
				{
					ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_TERRITORY_REQUIRED").Error(player.GetIdentity());
					return false;
				}
				
				//! Can only place one parking meter per territory
				ExpansionGarageModule garageModule = ExpansionGarageModule.Cast(CF_ModuleCoreManager.Get(ExpansionGarageModule));
				if (garageModule.HasTerritoryParkingMeter(flag.GetTerritoryID()))
				{
					ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "Only one parking meter per territory!").Error(player.GetIdentity());
					return false;
				}
			}
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
			PlayerBase playerPB = PlayerBase.Cast(player);
			auto setting = GetExpansionSettings().GetGarage();
			
			ExpansionParkingMeter parkingMeter = ExpansionParkingMeter.Cast(GetGame().CreateObjectEx("ExpansionParkingMeter", GetPosition(), ECE_PLACE_ON_SURFACE));
			parkingMeter.SetPosition(position);
			parkingMeter.SetOrientation(orientation);
			parkingMeter.m_ConstructionKitHealth = GetHealth("", "");
			
			if (setting.GarageMode == ExpansionGarageMode.Territory && flag && flag.GetTerritoryID() > -1 && playerPB.IsInTerritoryOrPerimeter())
			{
				parkingMeter.SetTerritoryID(flag.GetTerritoryID());
			}
			else if (setting.GarageMode == ExpansionGarageMode.Personal)
			{
				parkingMeter.OnDeployOutsideTerritory();
			}
			
			HideAllSelections(); //! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			SetIsDeploySound(true);
		}
	}
};
#endif
