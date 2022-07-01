/**
 * ActionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionContinuousBase
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		questModule.OnActionUsed(this, action_data);
	}
};

modded class AnimatedActionBase
{
	protected override void OnExecuteServer(ActionData action_data)
	{
		 super.OnExecuteServer(action_data);
		
		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		questModule.OnActionUsed(this, action_data);
	}
};

modded class ActionSplintSelf	
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		
		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		questModule.OnActionUsed(this, action_data);
	}
};

modded class ActionSplintTarget	
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		
		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		questModule.OnActionUsed(this, action_data);
	}
};