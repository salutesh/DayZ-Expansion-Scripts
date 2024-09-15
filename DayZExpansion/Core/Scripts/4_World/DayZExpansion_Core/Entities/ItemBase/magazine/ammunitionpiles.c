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
		return MiscGameplayFunctions.Expansion_TransferCartridges(this, dst, amount, resetDstAmmoCount, totalDamage);
	}
}
