modded class BleedingSourcesManagerServer
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		int active_bits = m_Player.GetBleedingBits();

		ctx.Write(active_bits);
		
		int bit_offset = 0;

		for (int i = 0; i < BIT_INT_SIZE; i++)
		{
			int bit = 1 << bit_offset;

			if ((bit & active_bits) != 0)
			{
				int active_time = GetBleedingSourceActiveTime(bit);
				eBleedingSourceType type = GetBleedingSourceType(bit);
				
				ctx.Write(active_time);
				ctx.Write(type);
			}

			bit_offset++;
		}
	}

	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		int active_bits;
		if (!ctx.Read(active_bits))
			return false;
	
		int bit_offset = 0;

		for (int i = 0; i < BIT_INT_SIZE; i++)
		{
			int bit = 1 << bit_offset;

			if ((bit & active_bits) != 0 && CanAddBleedingSource(bit))
			{
				AddBleedingSource(bit);

				int active_time = 0;
				if (!ctx.Read(active_time))
					return false;
				else
					SetBleedingSourceActiveTime(bit, active_time);

				if (version >= 121)
				{
					eBleedingSourceType type = eBleedingSourceType.NORMAL;
					if (!ctx.Read(type))
						return false;
					else
						SetBleedingSourceType(bit, type);
				}
			}

			bit_offset++;
		}

		return true;
	}
#endif
}
