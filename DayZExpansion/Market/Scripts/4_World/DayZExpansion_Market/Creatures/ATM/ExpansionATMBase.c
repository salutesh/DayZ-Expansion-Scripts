/**
 * ExpansionATMBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMBase extends BuildingSuper
{
	// ------------------------------------------------------------
	// ExpansionATMBase Constructor
	// ------------------------------------------------------------
	void ExpansionATMBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionATMBase IsBuilding
	// ------------------------------------------------------------
	override bool IsBuilding()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionATMBase SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionOpenATMMenu);
	}
}

class ExpansionATMLocker: ExpansionATMBase {}
class ExpansionATM_1: ExpansionATMBase {}
class ExpansionATM_2: ExpansionATMBase {}
class ExpansionATM_3: ExpansionATMBase {}