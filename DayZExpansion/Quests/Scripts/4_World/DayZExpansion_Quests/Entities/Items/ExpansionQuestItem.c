/**
 * ExpansionQuesItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestItemBase extends ItemBase
{
	void ExpansionQuestItemBase()
	{
		m_CanBeSkinned = false;

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			SetAllowDamage(false);
	}

	/*override bool CanPutAsAttachment(EntityAI parent)
	{
		ExpansionTemporaryOwnedContainer container;
		PlayerBase parentPlayer;
		if (Class.CastTo(container, parent))
		{
			return true;
		}

		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		ExpansionTemporaryOwnedContainer container;
		PlayerBase parentPlayer;
		if (Class.CastTo(container, parent))
		{
			return true;
		}
		else if (Class.CastTo(container, parentPlayer))
		{
			return true;
		}

		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		ExpansionTemporaryOwnedContainer container;
		PlayerBase parentPlayer;
		if (Class.CastTo(container, GetHierarchyParent()))
		{
			return true;
		}
		else if (Class.CastTo(container, parentPlayer))
		{
			return true;
		}

		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}*/
};

class ExpansionQuestItemPaper: ExpansionQuestItemBase {};
class ExpansionQuestItemPackage: ExpansionQuestItemBase {};