/**
 * ExpansionSpraycanBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	// ExpansionSpraycanBase DescriptionOverride
	// ------------------------------------------------------------	
	override bool DescriptionOverride(out string output)
	{
		output = ConfigGetString("descriptionShort");
		if (output.IndexOf("STR_EXPANSION_SPRAYCAN_") == 0)
			output = "#STR_EXPANSION_SPRAYCAN_BASE_DESC";
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
		return ExpansionSkinModule.Cast(CF_ModuleCoreManager.Get(ExpansionSkinModule));
	}
};