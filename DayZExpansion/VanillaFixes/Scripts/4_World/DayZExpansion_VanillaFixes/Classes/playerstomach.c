#ifndef DAYZ_1_25
//! Bugfix for 1.26 always trying to read temp regardless of version https://feedback.bistudio.com/T183495
modded class PlayerStomach
{
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		int checksum, count;
		if (!ctx.Read(checksum))
		{
			return false;
		}
		
		if (!ctx.Read(count))
		{
			return false;
		}

		for(int i = 0; i < count; i++)
		{
			int value, agents, temperature;
			if (!ctx.Read(value))
			{
				return false;
			}	
			if (!ctx.Read(agents))
			{
				return false;
			}
			//! Bugfix for 1.26 always trying to read temp regardless of version
			if (version >= 140 && !ctx.Read(temperature))
			{
				return false;
			}
			if (checksum == CHECKSUM)//if checksum matches, add to stomach, otherwise throw the data away but go through the de-serialization to keep the stream intact
			{
				int amount = value >> quantity_bit_offset;//isolate amount bits
				int id_mask = Math.Pow(2, quantity_bit_offset) - 1;
				int id = (id_mask & value) >> id_bit_offset;//isolate id bits
				int food_mask = Math.Pow(2, id_bit_offset) - 1;
				int food_stage = value & food_mask;
				//Print("LOAD value:" + value);
				//Print("LOAD id:" + id);
				//Print("LOAD id_bit_offset:" + id_bit_offset);
				//Print("LOAD food_stage:" + food_stage);
				string classname = GetClassnameFromID(id);
				AddToStomach(classname, amount, food_stage, agents, temperature);
			}
		}
		//Print("LOAD checksum:" + checksum);
		if (checksum != CHECKSUM)
		{
			Print("Stomach checksum fail");
		}
		return true;
	}
}
#endif
