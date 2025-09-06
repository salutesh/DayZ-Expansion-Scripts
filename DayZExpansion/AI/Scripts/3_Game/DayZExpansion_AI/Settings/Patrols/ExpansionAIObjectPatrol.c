/**
 * ExpansionAIObjectPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Legacy
class ExpansionAIObjectPatrol: ExpansionAIDynamicSpawnBase
{
	string LoadoutFile;
	string ClassName;

	void ExpansionAIObjectPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "West", string loa = "", bool canbelooted = true, int unlimitedreload = 0, float chance = 1.0, float mindistradius = -1, float maxdistradius = -1, string classname = "Wreck_UH1Y")
	{
		EXError.Error(this, "DEPRECATED, do not use");
	}
};

class ExpansionAICrashPatrol: ExpansionAIObjectPatrol
{
	string EventName;
};
