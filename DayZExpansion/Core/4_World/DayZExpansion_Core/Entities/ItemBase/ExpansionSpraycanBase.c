/**
 * ExpansionSpraycanBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSpraycanBase
 * @brief		
 **/
class ExpansionSpraycanBase: ItemBase
{
	// ------------------------------------------------------------
	// ExpansionSpraycanBase NameOverride
	// ------------------------------------------------------------	
	override bool NameOverride( out string output )
	{
		output = ExpansionGetItemDisplayNameWithType(GetType());
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpraycanBase DescriptionOverride
	// ------------------------------------------------------------	
	override bool DescriptionOverride(out string output)
	{
		output = ExpansionGetItemDescriptionWithType(GetType());
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpraycanBase SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
	}

	// ------------------------------------------------------------
	// ExpansionSpraycanBase CanExplodeInFire
	// ------------------------------------------------------------
	override bool CanExplodeInFire()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpraycanBase CanExplodeInFire
	// ------------------------------------------------------------
	ExpansionSkinModule GetSkinModule()
	{
		return ExpansionSkinModule.Cast(GetModuleManager().GetModule(ExpansionSkinModule));
	}
};