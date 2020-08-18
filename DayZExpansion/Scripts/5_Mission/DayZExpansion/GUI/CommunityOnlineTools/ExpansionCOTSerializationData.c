class ExpansionCOTSerializationData: UIActionData
{
	private ExpansionSettingSerializationBase m_Data;

	void ExpansionCOTSerializationData( ref ExpansionSettingSerializationBase data )
	{
		m_Data = data;
	}

	ref ExpansionSettingSerializationBase Get()
	{
		return m_Data;
	}
};