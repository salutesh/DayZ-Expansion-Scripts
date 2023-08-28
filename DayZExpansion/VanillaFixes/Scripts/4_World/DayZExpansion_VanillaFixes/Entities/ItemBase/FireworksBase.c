modded class FireworksBase
{
	//! Vanilla doesn't call super >:(
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		if (m_Expansion_CurrentOwner != player)
			Expansion_OnInventoryEnter(player);
	}
}
