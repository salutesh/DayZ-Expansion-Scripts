/**
 * ActionUnrestrainTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionUnrestrainTarget
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		eAIBase ai;
		if (!Class.CastTo(ai, action_data.m_Target.GetObject()))
		{
			super.OnFinishProgressServer(action_data);
			return;
		}

		if (CanReceiveAction(action_data.m_Target) && ai.IsRestrained())
		{
			EntityAI unrestraining_tool = action_data.m_MainItem;
			EntityAI restraining_item = ai.GetItemInHands();

			ai.SetRestrained(false);

			CachedObjectsArrays.ARRAY_STRING.Clear();

			restraining_item.ConfigGetTextArray("CanBeUnrestrainedBy", CachedObjectsArrays.ARRAY_STRING);
			restraining_item.ConfigGetFloatArray("CanBeUnrestrainedByDMG", CachedObjectsArrays.ARRAY_FLOAT);

			string item_in_hands_name = unrestraining_tool.GetType();
			float damageToTool = 0;

			for (int i = 0; i < CachedObjectsArrays.ARRAY_STRING.Count(); i++)
			{
				string class_name = CachedObjectsArrays.ARRAY_STRING.Get(i);
				if (GetGame().IsKindOf(item_in_hands_name, class_name))
				{
					damageToTool = CachedObjectsArrays.ARRAY_FLOAT.Get(i / 2);
					break;
				}
			}

			MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, damageToTool);

			eAI_Unrestrain(restraining_item, unrestraining_tool, ai);

			action_data.m_Player.GetSoftSkillsManager().AddSpecialty(m_SpecialtyWeight);
		}
	}

	static void eAI_Unrestrain(EntityAI current_item, EntityAI tool, eAIBase ai)
	{
		bool type;
		
		if (tool)
			type = tool.ConfigGetBool("RestrainUnlockType");

		string new_item_name = current_item.ConfigGetString( "OnRestrainChange");
		
		if (new_item_name != "")
		{
			current_item.DeleteSafe();
			ai.SetRestrained(false);
			ai.OnItemInHandsChanged();

			GetGame().CreateObjectEx(new_item_name, ai.GetPosition(), ECE_PLACE_ON_SURFACE);
		}
		else
		{
			Error("current_item:" +current_item+ ", tool:" +tool +". No value for 'OnRestrainChange' config parameter");
		}
	}
}
