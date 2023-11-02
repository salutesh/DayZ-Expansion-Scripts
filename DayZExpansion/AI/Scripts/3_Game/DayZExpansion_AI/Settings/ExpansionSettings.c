/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_AI = new ScriptInvoker();
	static ref ScriptInvoker SI_AIPATROL = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionAISettings, true);
		Init(ExpansionAIPatrolSettings);
	}

	void RPC_AISettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionAISettings, ctx);
	}

	ExpansionAISettings GetAI(bool checkLoaded = true)
	{
		return ExpansionAISettings.Cast(Get(ExpansionAISettings, checkLoaded));
	}

	ExpansionAIPatrolSettings GetAIPatrol(bool checkLoaded = true)
	{
		return ExpansionAIPatrolSettings.Cast(Get(ExpansionAIPatrolSettings, checkLoaded));
	}
};
