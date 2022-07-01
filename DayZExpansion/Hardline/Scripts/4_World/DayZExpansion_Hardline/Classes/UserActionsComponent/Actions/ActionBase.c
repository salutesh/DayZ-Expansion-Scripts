/**
 * ActionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionBase
{
	ExpansionHardlineModule m_HardlineModule;

    void ActionBase()
	{
		if (!m_HardlineModule)
			m_HardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
	}
}