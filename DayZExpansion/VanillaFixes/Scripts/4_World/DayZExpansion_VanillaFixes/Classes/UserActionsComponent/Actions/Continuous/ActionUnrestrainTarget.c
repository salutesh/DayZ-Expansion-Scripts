/**
 * ReplaceAndDestroyLambda.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ReplaceAndDestroyLambda
{
	override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
	{
		if (old_item && new_item && m_TransferAgents && m_TransferVariables && !m_ExcludeQuantity)
		{
			EXPrint(ToString() + "::CopyOldPropertiesToNew " + old_item + " " + new_item);

			//! Properly transfer all data associated with old item
			ScriptReadWriteContext ctx = new ScriptReadWriteContext;
			old_item.OnStoreSave(ctx.GetWriteContext());
			new_item.OnStoreLoad(ctx.GetReadContext(), g_Game.SaveVersion());
			new_item.AfterStoreLoad();
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(new_item.EEOnAfterLoad);  //! Make sure EEOnAfterLoad gets called AFTER whole hierarchy has loaded
		}

		super.CopyOldPropertiesToNew(old_item, new_item);
	}
}
