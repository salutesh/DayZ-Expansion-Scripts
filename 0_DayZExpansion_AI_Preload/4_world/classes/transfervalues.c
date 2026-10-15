modded class TransferValues
{
	override void SendValue(int value_type, float value)
	{
		if (!m_Player.Expansion_IsAI())
			super.SendValue(value_type, value);
	}
}
