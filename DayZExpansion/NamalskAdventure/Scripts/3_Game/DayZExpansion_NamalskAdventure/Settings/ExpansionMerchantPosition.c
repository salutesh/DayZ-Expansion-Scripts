/**
 * ExpansionMerchantPosition.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
class ExpansionMerchantPosition
{
	int ID = -1;
	vector Position;
	vector Orientation;
	string ObjectSetFile;

	void ExpansionMerchantPosition(int id, vector pos, vector ori, string file = "")
	{
		ID = id;
		Position = pos;
		Orientation = ori;
		ObjectSetFile = file;
	}
};
#endif