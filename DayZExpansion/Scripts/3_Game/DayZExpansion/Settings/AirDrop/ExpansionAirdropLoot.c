/**
 * ExpansionAirdropLoot.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAirdropLoot
{
	string Name;
	ref array < ref ExpansionAirdropLootAttachments > Loot;
	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;

	void ExpansionAirdropLoot( string name, ref array < ref ExpansionAirdropLootAttachments > loot, ref TStringArray infected, int itemCount, int infectedCount )
	{
		Name = name;
		Loot = loot;
		Infected = infected;
		ItemCount = itemCount;
		InfectedCount = infectedCount;
	}
}