/**
 * ExpansionMarketWeapon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketOutputs
{
	ref map<string, ref ExpansionMarketWeapon> Weapons = new map<string, ref ExpansionMarketWeapon>;
	ref map<string, ref ExpansionMarketClothing> Clothing = new map<string, ref ExpansionMarketClothing>;
}

class ExpansionMarketOutput
{
	string name;
	ref TStringArray attachments = new TStringArray;
}

class ExpansionMarketWeapon: ExpansionMarketOutput
{
	ref TStringArray bullets = new TStringArray;
	ref TStringArray magazines = new TStringArray;
	ref TStringArray optics = new TStringArray;
	ref TStringArray suppressors = new TStringArray;
	ref TStringArray ghillies = new TStringArray;
	ref TStringArray switchables = new TStringArray;
	ref TStringArray buttstocks = new TStringArray;
	ref TStringArray handguards = new TStringArray;
	ref TStringArray bayonets = new TStringArray;
	ref TStringArray rails = new TStringArray;
	ref TStringArray lights = new TStringArray;
}

class ExpansionMarketClothing: ExpansionMarketOutput
{
	ref TStringArray pouches = new TStringArray;
	ref TStringArray holsters = new TStringArray;
	ref TStringArray canteens = new TStringArray;
	ref TStringArray grenades = new TStringArray;
	ref TStringArray patches = new TStringArray;
}