modded class MagazineStorage
{
	/**
	 * @brief Transfer <amount> catridges from this mag to destination mag
	 * 
	 * @param dst mag to transfer cartridges to
	 * @param amount Number of cartridges to transfer
	 * @param resetDstAmmoCount Reset destination ammo count to zero before transfer
	 * @param totalDamage Will be set to total cartridge damage of all successfully transferred cartridges
	 * 
	 * @return Number of successfully transferred cartridges
	 */
	int Expansion_TransferCartridges(Magazine dst, int amount, bool resetDstAmmoCount = true, out float totalDamage = 0.0)
	{
		return MiscGameplayFunctions.Expansion_TransferCartridges(this, dst, amount, resetDstAmmoCount, totalDamage);
	}
}
