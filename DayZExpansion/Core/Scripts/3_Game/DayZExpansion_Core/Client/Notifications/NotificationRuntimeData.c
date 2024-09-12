/**
 * NotificationRuntimeData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionNotificationType
{
	TOAST = 1,
	BAGUETTE = 2,
	ACTIVITY = 4,
	KILLFEED = 8,
	MARKET = 16,
	GARAGE = 32
}

modded class NotificationRuntimeData
{
	ExpansionNotificationType m_Type;
	Object m_Object;
	bool m_LeaveJoinNotif = false;
	
	override string GetIcon()
	{
		if (!m_StaticData.m_Icon.Contains(":") && !m_StaticData.m_Icon.Contains("\\") && !m_StaticData.m_Icon.Contains("/"))
		{
			auto icon = ExpansionIcons.Get(m_StaticData.m_Icon);
			if (icon)
				return icon.IconPath;
		}

		return m_StaticData.m_Icon;
	}
	
	void SetType(ExpansionNotificationType type)
	{
		m_Type = type;
	}
	
	ExpansionNotificationType  GetType()
	{
		return m_Type;
	}
	
	void SetObject(Object obj)
	{
		m_Object = obj;
	}
	
	Object  GetObject()
	{
		return m_Object;
	}
}
