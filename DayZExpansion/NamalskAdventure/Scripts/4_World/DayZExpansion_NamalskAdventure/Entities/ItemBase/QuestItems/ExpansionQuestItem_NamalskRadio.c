/**
 * ExpansionQuestItem_NamalskRadio.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS
#ifdef NAMALSK_SURVIVAL
class ExpansionQuestItem_NamalskRadio extends ItemBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		SetQuestID(2);
		SetIsQuestGiver(true);
	}
};

class ExpansionQuestItem_StashNote extends ExpansionQuestItemBook
{
	override void DeferredInit()
	{
		super.DeferredInit();

		SetQuestID(3000);
		SetIsQuestGiver(true);
	}
};
#endif
#endif