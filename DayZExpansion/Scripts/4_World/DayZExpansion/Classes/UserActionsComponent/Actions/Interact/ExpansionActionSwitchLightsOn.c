/**
 * ActionSwitchLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionSwitchLightsOn: ActionSingleUseBase
{
	void ActionSwitchLights()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#switch_on";
	}

	ref array<Object> objects = new array<Object>;
	ref array<CargoBase> proxy = new array<CargoBase>;

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) // What about auto-disable? 
	{
		if( !target ) return false;
		if ( player.GetItemInHands() ) return false;
		
		vector pos_cursor = target.GetCursorHitPos();
		string targetType;
		if ( target.GetObject() )
		{
			targetType = target.GetObject().GetType();
			if (target.IsProxy() && vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT && targetType != "")
			{
				if (GetGame().ConfigGetInt("CfgNonAIVehicles " + targetType + " ExpansionCanBeEnabled") == 1)
				{
					GetGame().GetObjectsAtPosition3D(GetGame().GetPlayer().GetPosition(), 10, objects, proxy);
					for ( int i = 0; i < objects.Count(); i++ ) 
					{
						Object obj = objects.Get(i);
						if (obj.GetType() == "PowerGenerator")
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}
}

