modded class VicinityItemManager
{
	override void AddVicinityItems(Object object)
	{
		int count = m_VicinityItems.Count();

		super.AddVicinityItems(object);

		if (m_VicinityItems.Count() != count)
		{
			m_VicinityItems.Sort();
		}
	}
}
