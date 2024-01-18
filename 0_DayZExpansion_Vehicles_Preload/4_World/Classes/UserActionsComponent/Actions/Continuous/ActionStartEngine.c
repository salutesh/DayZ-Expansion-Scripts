/**
 * ActionStartEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	//! Can NOT call super here
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
					switch (m_Car.GetLockedState())
					{
						case ExpansionVehicleLockState.NOLOCK:
						case ExpansionVehicleLockState.FORCEDUNLOCKED:
							return true;
						case ExpansionVehicleLockState.FORCEDLOCKED:
							return false;
					}

					if (!GetExpansionSettings().GetVehicle(false).IsLoaded())
						return false;

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

	//! Can NOT call super here
	override void OnFinishProgressServer(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				CarScript car;
				if (Class.CastTo(car, trans))
				{
					float engineHealth = car.GetHealth01("Engine", "");
					//! @note chance when engine is damaged (health level 0.5)
					float chance = GetExpansionSettings().GetVehicle().DamagedEngineStartupChancePercent / 100.0;
					//! @note calculated chance follows a power curve (linear if chance == 0.5)
					float chanceMin = 0.0025;
					bool clamp = chance == 0;
					if (chance < chanceMin)
						chance = chanceMin;
					chance = Math.Pow(engineHealth, -ExpansionMath.Log2(chance));
					if (clamp)
						chance = ExpansionMath.LinearConversion(chanceMin, 1.0, chance, 0.0, 1.0);
					if (chance >= Math.RandomFloatInclusive(0.0, 1.0))
					{
						int current = car.Expansion_EngineGetCurrent();
						car.Expansion_EngineStart(current);
						
						if (action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleEngine)
							GetExpansionSettings().GetLog().PrintLog("[VehicleEngine] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] started vehicle " + car.GetDisplayName() + " (id=" + car.GetVehiclePersistentIDString() + " pos=" + car.GetPosition() + ")");
					}
				}
			}
		}
	}

	override bool Expansion_CheckSuccess(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				CarScript car;
				if (Class.CastTo(car, trans))
					return car.Expansion_EngineIsOn();
			}
		}

		return false;
	}
};
