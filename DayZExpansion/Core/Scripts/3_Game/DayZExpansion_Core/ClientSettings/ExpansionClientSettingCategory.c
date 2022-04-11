/**
 * ExpansionClientSettingCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionClientSettingCategory
{
	autoptr array< ref ExpansionSettingSerializationBase > m_Settings;

	string m_Name;
	string m_DisplayName;

	void ExpansionClientSettingCategory()
	{
		m_Settings = new array< ref ExpansionSettingSerializationBase >;
	}
};