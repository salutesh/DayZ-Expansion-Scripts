/**
 * BaseBuildingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BaseBuildingBase
{
	void ExpansionUpdateBaseBuildingStateFromPartBuilt(string part_name) {};

	void Expansion_OnPartBuiltServer(ConstructionPart constructionPart)
	{
		//! Equivalent to vanilla BaseBuildingBase::OnPartBuiltServer (minus creating construction kit and requirement for player)

		RegisterPartForSync(constructionPart.GetId());
		RegisterActionForSync(constructionPart.GetId(), AT_BUILD_PART);
		SynchronizeBaseState();
		SetPartFromSyncData(constructionPart);
		UpdateNavmesh();
		UpdateVisuals();

		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false);
	}
};
