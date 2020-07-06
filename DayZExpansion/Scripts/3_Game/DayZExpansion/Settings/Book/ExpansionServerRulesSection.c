/**
 * ExpansionServerRulesSection.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionServerRulesSection
 * @brief
 **/
class ExpansionServerRulesSection 
{
	string Lable;
	ref array<ref ExpansionServerRulesPage> RulesPages;
	ref array<ref ExpansionServerRulesPageSection> PageSections;
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection Constructor
	// ------------------------------------------------------------
    void ExpansionServerRulesSection(string lable, array<ref ExpansionServerRulesPage> pages)
	{
		this.Lable = lable;
		this.RulesPages = pages;
		
		PageSections = new array<ref ExpansionServerRulesPageSection>;
		
		BuildSections();
    }
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection BuildSections
	// ------------------------------------------------------------
	void BuildSections()
	{
		if (PageSections.Count() > 0)
			PageSections.Clear();
		
		for (int i = 0; i < this.RulesPages.Count(); ++i)
		{
			int left;
			int right;
			
			if (i % 2 == 0)
			{
				left = i;
			}
			else
			{
				right = i;
			}
			
			ExpansionServerRulesPageSection section = new ExpansionServerRulesPageSection(left, right);
			PageSections.Insert( section );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection NumberOfPages
	// ------------------------------------------------------------
	int NumberOfPages()
	{
		return this.RulesPages.Count();
	}
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection NumberOfPageSections
	// ------------------------------------------------------------
	int NumberOfPageSections()
	{
		return (this.RulesPages.Count() / 2);
	}
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection GetPageSection
	// ------------------------------------------------------------
	ref ExpansionServerRulesPageSection GetPageSection(int index)
	{
		return PageSections.Get( index );
	}
	
	// ------------------------------------------------------------
	// ExpansionServerRulesSection GetPage
	// ------------------------------------------------------------
	ref ExpansionServerRulesPage GetPage(int index)
	{
		return RulesPages.Get( index );
	}
}

class ExpansionServerRulesPageSection
{
	int Left;
	int Right;
	
	void ExpansionServerRulesPageSection(int left = -1, int right = -1)

	{
		this.Left = left;
		this.Right = right;
	}
}