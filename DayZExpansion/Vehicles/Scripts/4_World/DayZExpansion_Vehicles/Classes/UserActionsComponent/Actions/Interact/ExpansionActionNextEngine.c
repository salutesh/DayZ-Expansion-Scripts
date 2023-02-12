/**
 * ExpansionActionNextEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionNextEngine : ActionInteractBase
{
	void ExpansionActionNextEngine()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_VEHICLE_ENGINE_NEXT" + " " + "#STR_EXPANSION_VEHICLE_ENGINE";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Human driver;
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if (vehCommand)
		{
			CarScript car;
			if (Class.CastTo(car, vehCommand.GetTransport()))
			{
				if (car.Expansion_EngineGetCount() <= 1)
					return false;

				driver = car.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
				if (driver && driver == player)
					return true;
			}
		}

		ExpansionHumanCommandVehicle exVehCommand = player.GetCommand_ExpansionVehicle();
		if (exVehCommand)
		{
			ExpansionVehicleBase veh;
			if (Class.CastTo(veh, exVehCommand.GetObject()))
			{
				if (veh.EngineGetCount() <= 1)
					return false;

				driver = veh.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
				if (driver && driver == player)
					return true;
			}
		}

		return false;
	}

	override void OnAnimationEvent(ActionData action_data)
	{
		if (!action_data.m_WasExecuted)
		{
			HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
			if (vehCommand)
			{
				CarScript car;
				if (Class.CastTo(car, vehCommand.GetTransport()))
				{
					car.Expansion_EngineSetNext();
				}
			}

			ExpansionHumanCommandVehicle exVehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
			if (exVehCommand)
			{
				ExpansionVehicleBase veh;
				if (Class.CastTo(veh, exVehCommand.GetObject()))
				{
					veh.EngineSetNext();
				}
			}
		}

		super.OnAnimationEvent(action_data);
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};

class ExpansionActionNextEngineInput : ExpansionActionNextEngine
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
};
