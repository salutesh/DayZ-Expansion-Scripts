enum ExpansionNotificationType
{
	TOAST = 1,
	BAGUETTE = 2,
	ACTIVITY = 4,
	KILLFEED = 8,
	MARKET = 16
}

modded class NotificationRuntimeData
{
	ExpansionNotificationType m_Type;
	Object m_Object;
	
	override string GetIcon()
	{
		if (!m_StaticData.m_Icon.Contains(":") && !m_StaticData.m_Icon.Contains("\\"))
			return ExpansionIcons.GetPath(m_StaticData.m_Icon);

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