/**
 * ExpansionMarketNetworkItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNetworkItem
{
	int CategoryID;

	string ClassName;
	TStringArray SkinsClassNames;

	int Stock;

	// todo: optimize and add min/max stock/price
	// todo: write a byte buffer class to push optimized strings

	// type byte
	// catid byte
	// numSkins byte
	// clsNameLen byte
	// className byte[]
	// arr[numSkins]:
	// 	skinNameLen byte
	// 	skinName byte[]
	// maxStock int16
	// minStock int16
	// maxPrice int16
	// minPrice int16
	// stock in16
	// endBuffer byte[]
}