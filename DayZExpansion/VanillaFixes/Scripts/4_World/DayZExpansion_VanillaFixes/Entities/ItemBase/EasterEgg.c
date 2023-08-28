modded class EasterEgg
{
	//! Vanilla doesn't call super >:(
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		if (m_Expansion_CurrentOwner != player)
			Expansion_OnInventoryEnter(player);
	}

	//! Vanilla doesn't call super >:(
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		if (m_Expansion_CurrentOwner == player)
			Expansion_OnInventoryExit(player);
	}
}
