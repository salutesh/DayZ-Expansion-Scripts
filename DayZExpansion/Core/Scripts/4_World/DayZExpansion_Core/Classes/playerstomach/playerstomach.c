modded class PlayerStomach
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		ctx.Write(PlayerStomach.CHECKSUM);
		ctx.Write(m_StomachContents.Count());
		foreach (StomachItem stomach_item: m_StomachContents)
		{
			int id = PlayerStomach.GetIDFromClassname(stomach_item.m_ClassName);
			
			int write_result = stomach_item.m_FoodStage | (id << id_bit_offset);
			write_result = write_result | ((int)stomach_item.m_Amount << quantity_bit_offset);
			ctx.Write(write_result);
			ctx.Write(stomach_item.m_Agents);

		#ifndef DAYZ_1_25
			ctx.Write((int) stomach_item.GetTemperature());
		#endif
		}
	}
	
	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		int checksum, count;

		if (!ctx.Read(checksum))
			return false;
		
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{
			int value, agents, temperature;

			if (!ctx.Read(value))
				return false;

			if (!ctx.Read(agents))
				return false;

			if (version >= 140 && !ctx.Read(temperature))
				return false;

			if (checksum == CHECKSUM)
			{
				int amount = value >> quantity_bit_offset;//isolate amount bits
				int id_mask = Math.Pow(2, quantity_bit_offset) - 1;
				int id = (id_mask & value) >> id_bit_offset;//isolate id bits
				int food_mask = Math.Pow(2, id_bit_offset) - 1;
				int food_stage = value & food_mask;
				string classname = GetClassnameFromID(id);
			#ifdef DAYZ_1_25
				AddToStomach(classname, amount, food_stage, agents);
			#else
				//! 1.26+
				AddToStomach(classname, amount, food_stage, agents, temperature);
			#endif
			}
		}

		if (checksum != CHECKSUM)
			Print("Stomach checksum fail");

		return true;
	}
#endif
}
