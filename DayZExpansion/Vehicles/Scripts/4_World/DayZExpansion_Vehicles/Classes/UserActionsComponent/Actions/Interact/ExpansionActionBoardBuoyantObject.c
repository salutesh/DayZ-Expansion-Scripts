/**
 * ExpansionActionBoardBuoyantObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note This exists mainly to easily allow getting onto boat/cover while swimming
class ExpansionActionBoardBuoyantObject: ActionInteractBase
{
	void ExpansionActionBoardBuoyantObject()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "#enter_ladder";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget	= new CCTCursorNoRuinCheck();
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool IsInstant()
	{
		return true;
	}

	override bool CanBeUsedSwimming()
	{
		return true;
	}

	override bool UseAcknowledgment()
	{
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player.GetCommand_Swim())
			return false;

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;
		Object obj = action_data.m_Target.GetParentOrObject();

		vector tmpPos = player.GetPosition();
		vector objPos = obj.GetPosition();

		tmpPos[1] = objPos[1];

		vector minMax[2];
		obj.GetCollisionBox(minMax);

		vector begPos;
		vector endPos;

		PhxInteractionLayers layerMask;
		layerMask |= PhxInteractionLayers.ITEM_LARGE;
		layerMask |= PhxInteractionLayers.VEHICLE;

		Object hitObject;
		vector hitPosition, hitNormal;
		float hitFraction;

		//! 1st raycast - determine distance to outer hull of obj
		endPos = tmpPos + player.GetDirection() * (UAMaxDistances.DEFAULT + 1.0);
		if (DayZPhysics.RayCastBullet(tmpPos, endPos, layerMask, player, hitObject, hitPosition, hitNormal, hitFraction))
			endPos = hitPosition;

		//! 2nd raycast - final
		begPos = tmpPos + Vector(0, minMax[1][1] + 0.5, 0);
		endPos = endPos + Vector(0, minMax[0][1], 0) + player.GetDirection();
		if (DayZPhysics.RayCastBullet(begPos, endPos, layerMask, player, hitObject, hitPosition, hitNormal, hitFraction))
			player.SetPosition(hitPosition);
	}
}
