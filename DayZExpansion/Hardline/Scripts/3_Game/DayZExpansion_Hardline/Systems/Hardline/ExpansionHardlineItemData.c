/**
 * ExpansionHardlineItemData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHardlineItemData
{
	string ClassName;
	int Rarity;

	void SetType(string type)
	{
		ClassName = type;
	}
	
	string GetType()
	{
		return ClassName;
	}
	
	void SetRatity(int rarity)
	{
		Rarity = rarity;
	}

	int GetRarity()
	{
		return Rarity;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ClassName);
		ctx.Write(Rarity);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ClassName))
			return false;
		
		if (!ctx.Read(Rarity))
			return false;
		
		return true;
	}
};