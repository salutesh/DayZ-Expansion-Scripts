/**
 * ActionTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ActionTarget
 * @brief		
 **/
modded class ActionTarget
{
	void ActionTarget(Object object, Object parent, int componentIndex, vector cursorHitPos, float utility)
	{
		ExpansionNavmeshCarver navmeshCarver;
		if (Class.CastTo(navmeshCarver, object))
			m_Object = navmeshCarver.Expansion_GetObject();
	}

	Object GetParentOrObject()
	{
		if ( GetParent() )
		{
			return GetParent();
		}
		else
		{
			return GetObject();
		}
	}
};