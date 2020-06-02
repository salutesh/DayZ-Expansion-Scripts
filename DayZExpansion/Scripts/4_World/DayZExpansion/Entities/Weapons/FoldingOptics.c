/**
 * FoldingOptics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/* 
modded class Rifle_Base
{
	protected int foldingOpticRaisedId = -1;
	protected int foldingOpticLoweredId = -1;
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Rifle_Base::EEItemAttached - Start");
		#endif

		TStringArray selectionNames = new TStringArray;
		ConfigGetTextArray("simpleHiddenSelections",selectionNames);
		foldingOpticRaisedId = selectionNames.Find("folding_raised");
		foldingOpticLoweredId = selectionNames.Find("folding_lowered");  
		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			//HideSelection("folding_raised");
			//ShowSelection("folding_lowered");
			SetSimpleHiddenSelectionState(foldingOpticRaisedId,false);
			SetSimpleHiddenSelectionState(foldingOpticLoweredId,true);
			
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Rifle_Base::EEItemAttached - End");
		#endif
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Rifle_Base::EEItemDetached - Start");
		#endif

		super.EEItemDetached(item,slot_name);

		if (ItemOptics.Cast(item))
		{
			SetSimpleHiddenSelectionState(foldingOpticRaisedId,true);
			SetSimpleHiddenSelectionState(foldingOpticLoweredId,false);
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("Rifle_Base::EEItemDetached - End");
		#endif
	}
}
*/