/*
 * WoodBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class WoodBase
{
	bool m_Cut = false;
	
	bool IsCut()
	{
		return m_Cut;
	}
	
	override void OnTreeCutDown( EntityAI cutting_entity )
	{
		super.OnTreeCutDown( cutting_entity );
		
		m_Cut = true;
	}
}