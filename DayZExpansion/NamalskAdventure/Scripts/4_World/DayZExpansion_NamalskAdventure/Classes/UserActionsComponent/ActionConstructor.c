/**
 * ActionConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(ExpansionActionUseSatelliteControl);
		actions.Insert(ExpansionActionInsertGeneratorKey);
		actions.Insert(ExpansionActionRemoveGeneratorKey);
		actions.Insert(ExpansionActionInsertSupplyCrateKey);
		actions.Insert(ExpansionActionRemoveSupplyCrateKey);
		actions.Insert(ExpansionActionOpenSupplyCrate);
		actions.Insert(ExpansionActionUseBunkerPanel);
		actions.Insert(ExpansionActionUseRepairCenter);
		
		actions.Insert(ActionTurnOnWorkbenchGenerator);
		actions.Insert(ActionTurnOffWorkbenchGenerator);
	}
};