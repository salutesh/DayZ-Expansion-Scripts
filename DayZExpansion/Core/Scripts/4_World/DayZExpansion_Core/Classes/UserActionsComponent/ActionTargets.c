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
	void ActionTarget(Object object, Object parent, int componentIndex, vector cursorHitPos, float utility, string surfaceName = "")
	{
		ExpansionNavmeshCarver navmeshCarver;
		if (Class.CastTo(navmeshCarver, object))
			m_Object = navmeshCarver.Expansion_GetAssociatedObject();
	}

	//! Should have used Expansion_ prefix, but keeping legacy name since it's been around forever
	Object GetParentOrObject()
	{
		//! Prefer parent
		if ( GetParent() )
		{
			return GetParent();
		}
		else
		{
			return GetObject();
		}
	}

	void Expansion_SetObject(Object object)
	{
		m_Object = object;
	}

	void Expansion_SetParent(Object parent)
	{
		m_Parent = parent;
	}

	Object Expansion_GetObjectOrParent()
	{
		//! Prefer object
		if ( GetObject() )
		{
			return GetObject();
		}
		else
		{
			return GetParent();
		}
	}
};