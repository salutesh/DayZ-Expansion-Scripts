class ExpansionZoneItemCleanup : ExpansionZoneActor
{
	ItemBase m_Item;

	void ExpansionZoneItemCleanup(ItemBase item)
	{
		m_Item = item;

		//! Using SetLifetimeMax here prevents the lifetime resetting to the value from CE
		//! if this item can receive attachments/cargo and something is attached/put in cargo
		//! while the item is on ground
		m_Item.SetLifetimeMax(GetExpansionSettings().GetSafeZone().ItemLifetimeInSafeZone);
	}

	override void OnUpdate()
	{
		if (m_Item.GetLifetime() < 0)
		{
			GetGame().ObjectDelete(m_Item);
		}
	}
};
