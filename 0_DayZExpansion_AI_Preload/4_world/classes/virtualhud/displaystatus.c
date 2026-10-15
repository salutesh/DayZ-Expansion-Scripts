modded class VirtualHud
{
	override void SendRPC()
	{
		if (!m_Player.Expansion_IsAI())
			super.SendRPC();
	}
}
