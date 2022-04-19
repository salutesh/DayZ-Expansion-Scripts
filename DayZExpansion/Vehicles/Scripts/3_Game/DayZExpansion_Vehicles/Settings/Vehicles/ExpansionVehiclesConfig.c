/**
 * ExpansionVehiclesConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehiclesConfig
{
	string ClassName;
	bool CanPlayerAttach;
	float LockComplexity;

	void ExpansionVehiclesConfig(string classname = "", bool canplayerattach = false, float lockcomplexity = 1.0 )
	{
		ClassName = classname;
		CanPlayerAttach = canplayerattach;
		LockComplexity = lockcomplexity;
	}
};
