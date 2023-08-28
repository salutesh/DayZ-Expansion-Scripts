/**
 * ExpansionP2PMarketPlayerItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketPlayerItem: ExpansionP2PMarketListingBase
{
	protected string OwnerUID;

	protected string ItemSlotName;

	void SetFromItem(EntityAI object, string playerUID)
	{
		EXPrint(ToString() + "::SetFromItem - Start");
		EXPrint(ToString() + "::SetFromItem - Item: " + object.GetType());
		
		SetFromItem(object);

		EXPrint(ToString() + "::SetFromItem - End");
	}

	string GetOwnerUID()
	{
		return OwnerUID;
	}

	void SetSlotName(string slotName)
	{
		ItemSlotName = slotName;
	}

	string GetSlotName()
	{
		return ItemSlotName;
	}
};