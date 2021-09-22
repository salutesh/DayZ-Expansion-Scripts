/**
 * ActionStopEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStopEngine
{
	CarScript m_Car;

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Stop " + m_Car.Expansion_EngineGetName() + " Engine";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();

		if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			Transport trans = vehCmd.GetTransport();
			if (trans)
			{
				if (Class.CastTo(m_Car, trans) && m_Car.Expansion_EngineIsOn())
				{
					m_CommandUID = m_Car.Expansion_EngineStopAnimation();

					//if ( m_Car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
					if (m_Car.GetSpeedometer() <= 8)
						return true;
				}
			}
		}
		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();

		if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			Transport trans = vehCmd.GetTransport();
			if (trans)
			{
				if (Class.CastTo(m_Car, trans) && m_Car.Expansion_EngineIsOn())
				{
					m_Car.Expansion_EngineStop();
					
					if (!GetGame().IsMultiplayer())
					{
						EffectSound sound = SEffectManager.PlaySound(m_Car.m_EngineStopFuel, m_Car.GetPosition());
						sound.SetSoundAutodestroy(true);
					}
				}
			}
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();

		if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			Transport trans = vehCmd.GetTransport();
			if (trans)
			{
				if (Class.CastTo(m_Car, trans) && m_Car.Expansion_EngineIsOn())
				{
					m_Car.Expansion_EngineStop();
					EffectSound sound = SEffectManager.PlaySound(m_Car.m_EngineStopFuel, m_Car.GetPosition());
					sound.SetSoundAutodestroy(true);
				}
			}
		}
	}
};
