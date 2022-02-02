/**
 * ActionStartEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStartEngine
{
	CarScript m_Car;

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_VEHICLE_ENGINE_START" + " " + m_Car.Expansion_EngineGetName() + " " + "#STR_EXPANSION_VEHICLE_ENGINE";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player)
			return false;

		IEntity vehicle = player.GetParent();
		if (!vehicle)
			return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if (vehCommand)
		{
			if (Class.CastTo(m_Car, vehCommand.GetTransport()) && !m_Car.Expansion_EngineIsOn())
			{
				m_CommandUID = m_Car.Expansion_EngineStartAnimation();

				if (m_Car.GetHealthLevel("Engine") >= GameConstants.STATE_RUINED)
					return false;

				if (m_Car.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
				{
					if (m_Car.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED)
						return true;

					if (!m_Car.HasKey())
						return true;

					ExpansionCarKey key;
					if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 1)
					{
						return player.HasKeyForCar(m_Car);
					}
					else if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 2)
					{
						if (!Class.CastTo(key, player.GetItemInHands()))
							return false;

						if (!m_Car.IsCarKeys(key))
							return false;

						return true;
					}

					//! GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 0, or invalid number but this still saves checking
					return true;
				}
			}
		}

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		CarScript car; //! Don't use m_Car, use this local variable and assign on action start. This is performed on the server.
		if (!Class.CastTo(car, action_data.m_Player.GetParent()))
			return;

		car.SetCarBatteryStateForVanilla(true);

		if (!car.Expansion_EngineIsOn())
		{
			ExpansionHelicopterScript heli;
			if (Class.CastTo(heli, car))
			{
				EntityAI item2;
				m_SparkCon = false;
				m_BeltCon = false;
				m_BatteryCon = false;

				if (heli.IsVitalIgniterPlug() || heli.IsVitalSparkPlug() || heli.IsVitalGlowPlug())
				{
					item2 = NULL;
					if (heli.IsVitalIgniterPlug())
						item2 = heli.FindAttachmentBySlotName("ExpansionIgniterPlug");
					if (heli.IsVitalSparkPlug())
						item2 = heli.FindAttachmentBySlotName("SparkPlug");
					if (heli.IsVitalGlowPlug())
						item2 = heli.FindAttachmentBySlotName("GlowPlug");

					if (item2 && !item2.IsRuined())
						m_SparkCon = true;
				}
				else
				{
					m_SparkCon = true;
				}

				if (heli.IsVitalHydraulicHoses() || heli.IsVitalRadiator())
				{
					item2 = NULL;
					if (heli.IsVitalHydraulicHoses())
						item2 = heli.FindAttachmentBySlotName("ExpansionHydraulicHoses");
					if (heli.IsVitalRadiator())
						item2 = heli.FindAttachmentBySlotName("CarRadiator");

					if (item2 && !item2.IsRuined())
						m_BeltCon = true;
				}
				else
				{
					m_BeltCon = true;
				}

				if (heli.IsVitalHelicopterBattery() || heli.IsVitalCarBattery() || heli.IsVitalTruckBattery())
				{
					item2 = NULL;
					if (heli.IsVitalHelicopterBattery())
						item2 = heli.FindAttachmentBySlotName("ExpansionHelicopterBattery");
					if (heli.IsVitalCarBattery())
						item2 = heli.FindAttachmentBySlotName("CarBattery");
					if (heli.IsVitalTruckBattery())
						item2 = heli.FindAttachmentBySlotName("TruckBattery");

					if (item2 && !item2.IsRuined())
						m_BatteryCon = true;
				}
				else
				{
					m_BatteryCon = true;
				}
			}
		}

		car.SetCarBatteryStateForVanilla(false);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			CarScript car;
			if (Class.CastTo(car, vehCommand.GetTransport()))
			{
				if (m_FuelCon && m_BeltCon && m_SparkCon && m_BatteryCon)
				{
					int current = car.Expansion_EngineGetCurrent();
					car.Expansion_EngineStart(current);
				}
			}
		}
	}
};
