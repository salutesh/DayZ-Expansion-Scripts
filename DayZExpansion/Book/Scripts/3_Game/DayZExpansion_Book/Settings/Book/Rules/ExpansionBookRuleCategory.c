/**
 * ExpansionBookRuleCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookRuleCategory
{
    string CategoryName;
    ref array<ref ExpansionBookRule> Rules = new array<ref ExpansionBookRule>;
	
	void ExpansionBookRuleCategory(string name)
	{
		CategoryName = name;
	}
	
	void AddRule(string paragraph, string text)
	{
		ExpansionBookRule rule = new ExpansionBookRule(paragraph, text);
		Rules.Insert(rule);
	}
};

class ExpansionBookRule
{
    string RuleParagraph;
    string RuleText;

    void ExpansionBookRule(string paragraph, string text)
	{
		RuleParagraph = paragraph;
		RuleText = text;
	}
};