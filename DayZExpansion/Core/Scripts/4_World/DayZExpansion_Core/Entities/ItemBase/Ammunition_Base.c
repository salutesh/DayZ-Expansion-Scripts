modded class Ammunition_Base
{
	/**
	 * @brief Transfer <amount> catridges from this ammo pile to destination pile
	 * 
	 * @param dst Ammo pile to transfer cartridges to
	 * @param amount Number of cartridges to transfer
	 * @param resetDstAmmoCount Reset destination ammo count to zero before transfer
	 * @param totalDamage Will be set to total cartridge damage of all successfully transferred cartridges
	 * 
	 * @return Number of successfully transferred cartridges
	 */
	int Expansion_TransferCartridges(Magazine dst, int amount, bool resetDstAmmoCount = true, out float totalDamage = 0.0)
	{
		if (resetDstAmmoCount)
			dst.ServerSetAmmoCount(0);

		amount = Math.Min(amount, Math.Min(GetAmmoCount(), dst.GetAmmoMax() - dst.GetAmmoCount()));

		int numberOfTransferredCartridges;
		for (int i = 0; i < amount; ++i)
		{
			float damage;
			string cartrige_name;
			if (ServerAcquireCartridge(damage, cartrige_name) && dst.ServerStoreCartridge(damage, cartrige_name))
			{
				numberOfTransferredCartridges++;
				totalDamage += damage;
			}
		}
		dst.SetSynchDirty();
		SetSynchDirty();

		return numberOfTransferredCartridges;
	}
}
