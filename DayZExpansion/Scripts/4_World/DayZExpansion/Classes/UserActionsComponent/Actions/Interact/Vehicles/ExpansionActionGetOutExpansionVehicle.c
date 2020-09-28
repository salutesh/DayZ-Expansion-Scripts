/**
 * ExpansionActionGetOutExpansionVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionGetOutExpansionVehicleActionData : ActionData
{
	ExpansionVehicleScript m_Car;
	vector m_StartLocation;
	float m_CarSpeed;
	bool m_WasJumpingOut = false;
	bool keepInVehicleSpaceAfterLeave = false;
}

/**@class		ExpansionActionGetOutExpansionVehicle
 * @brief		
 **/
class ExpansionActionGetOutExpansionVehicle: ActionInteractBase
{
	void ExpansionActionGetOutExpansionVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = "GetInDriver";
	}

	override ActionData CreateActionData()
	{
		ActionData action_data = new ExpansionActionGetOutExpansionVehicleActionData;
		return action_data;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#leave_vehicle";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player.GetCommand_ExpansionVehicle() )
			return false;

		return true;
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
	
	override void OnUpdate( ActionData action_data )
	{
		if ( action_data.m_State == UA_START )
		{
			if ( !action_data.m_Player.GetCommand_ExpansionVehicle() )
			{
				End( action_data );
			}
		}
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		PerformGetOutTransport( action_data );
	}

	private void PerformGetOutTransport( ActionData action_data )
	{
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
		if ( vehCommand )
		{
			ExpansionVehicleScript veh = vehCommand.GetTransport();

			float speed = veh.GetSpeedometer();
			bool keepInVehicleSpaceAfterLeave = s_ExpansionPlayerAttachment && veh.CanObjectAttach( action_data.m_Player ) && veh.LeavingSeatDoesAttachment( vehCommand.GetVehicleSeat() );

			ExpansionActionGetOutExpansionVehicleActionData got_action_data = ExpansionActionGetOutExpansionVehicleActionData.Cast( action_data );
			got_action_data.m_StartLocation = got_action_data.m_Player.GetPosition();
			got_action_data.m_Car = veh;
			got_action_data.m_CarSpeed = speed;
			got_action_data.keepInVehicleSpaceAfterLeave = keepInVehicleSpaceAfterLeave;

			vehCommand.KeepInVehicleSpaceAfterLeave( keepInVehicleSpaceAfterLeave );

			if ( keepInVehicleSpaceAfterLeave )
			{
				vehCommand.GetOutVehicle();
			} else if ( speed <= 8 )
			{
				vehCommand.GetOutVehicle();
			} else
			{
				got_action_data.m_WasJumpingOut = true;
				vehCommand.JumpOutVehicle();
			}

			GetDayZGame().GetBacklit().OnLeaveCar();
			if ( action_data.m_Player.GetInventory() )
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		}
	}

	override void End( ActionData action_data )
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Unhide,500,false,(action_data.m_Player));
		super.End( action_data );
	}
	
	void Unhide(PlayerBase player)
	{
		player.GetItemAccessor().HideItemInHands(false);
	}

	override void OnEndServer( ActionData action_data )
	{
		ExpansionActionGetOutExpansionVehicleActionData got_action_data = ExpansionActionGetOutExpansionVehicleActionData.Cast( action_data );
		if ( got_action_data.keepInVehicleSpaceAfterLeave )
			return;

		vector endLocation = action_data.m_Player.GetPosition();
		
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		set<Object> result = new set<Object>;
		
		vector offset = Vector(0, 2, 0);
		
		if (DayZPhysics.RaycastRV(got_action_data.m_StartLocation, endLocation, contact_pos, contact_dir, contact_component, result, got_action_data.m_Car, action_data.m_Player, false, false, ObjIntersectView, 0.3))
			got_action_data.m_Player.SetPosition(contact_pos);

		if (got_action_data.m_WasJumpingOut)
			got_action_data.m_Player.OnJumpOutVehicleFinish(got_action_data.m_CarSpeed);
	}
}