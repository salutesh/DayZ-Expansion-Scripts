/**
 * ExpansionBookDescriptionCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookDescriptionCategory
{
    string CategoryName;
    ref array<ref ExpansionBookDescription> Descriptions = new array<ref ExpansionBookDescription>;
	
	void ExpansionBookDescriptionCategory(string name)
	{
		CategoryName = name;
	}
	
	void AddDescription(string text)
	{
		ExpansionBookDescription desc = new ExpansionBookDescription(text);
		Descriptions.Insert(desc);
	}
};

class ExpansionBookDescription
{
    string DescriptionText;

    void ExpansionBookDescription(string text)
	{
		DescriptionText = text;
	}
};