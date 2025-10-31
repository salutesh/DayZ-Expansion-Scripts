/**
 * @brief this class duplicates some of the functionality of EntityAI to work-around the latter not being moddable
 */
class ExpansionEntity
{
	EntityAI m_Entity;

	void ExpansionEntity(EntityAI entity)
	{
		m_Entity = entity;
	}

#ifdef EXPANSION_MODSTORAGE
	//! EntityAI::OnStoreSave, but for ModStorage
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		if (m_Entity.m_EM)
		{
			ctx.Write(m_Entity.m_EM.GetEnergy());
			ctx.Write(m_Entity.m_EM.IsPassive());
			ctx.Write(m_Entity.m_EM.IsSwitchedOn());
			ctx.Write(m_Entity.m_EM.IsPlugged());

			EntityAI energy_source = m_Entity.m_EM.GetEnergySource();

			int b1 = 0;
			int b2 = 0;
			int b3 = 0;
			int b4 = 0;

			if (energy_source)
				energy_source.GetPersistentID(b1, b2, b3, b4);

			ctx.Write(b1);
			ctx.Write(b2);
			ctx.Write(b3);
			ctx.Write(b4);
		}

		CF_SaveVariables(ctx);
	}

	//! EntityAI::OnStoreLoad, but for ModStorage
	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		if (m_Entity.m_EM)
		{
			float f_energy;
			if (!ctx.Read(f_energy))
				return false;
			m_Entity.m_EM.SetEnergy(f_energy);

			bool b_is_passive;
			if (!ctx.Read(b_is_passive))
				return false;
			m_Entity.m_EM.SetPassiveState(b_is_passive);

			bool b_is_on;
			if (!ctx.Read(b_is_on))
			{
				m_Entity.m_EM.SwitchOn();
				return false;
			}

			bool b_is_plugged;
			if (!ctx.Read(b_is_plugged))
				return false;

			if (version <= 103)
			{
				int tmp;

				if (!ctx.Read(tmp))
					return false;

				if (!ctx.Read(tmp))
					return false;
			}
			else
			{
				int b1 = 0;
				int b2 = 0;
				int b3 = 0;
				int b4 = 0;

				if (!ctx.Read(b1)) return false;
				if (!ctx.Read(b2)) return false;
				if (!ctx.Read(b3)) return false;
				if (!ctx.Read(b4)) return false;

				if (b_is_plugged)
				{
					// Because function GetEntityByPersitentID() cannot be called here, ID values must be stored and used later.
					m_Entity.m_EM.StoreEnergySourceIDs(b1, b2, b3, b4);
					m_Entity.m_EM.RestorePlugState(true);
				}
			}

			if (b_is_on)
				m_Entity.m_EM.SwitchOn();
		}

		if (version >= 140)
		{
			if (!CF_LoadVariables(ctx, version))
				return false;
		}
		
		return true;
	}

	void CF_SaveVariables(CF_ModStorage ctx)
	{
		int varFlags;
		
		if (m_Entity.m_VariablesMask)
			varFlags = ItemVariableFlags.FLOAT;
		
		ctx.Write(varFlags);
		
		if (m_Entity.m_VariablesMask)
			CF_WriteVarsToCTX(ctx);
	}

	void CF_WriteVarsToCTX(CF_ModStorage ctx)
	{
		ctx.Write(m_Entity.m_VariablesMask);
		
		if (m_Entity.IsVariableSet(VARIABLE_TEMPERATURE))
		{
			ctx.Write(m_Entity.GetTemperature());
			ctx.Write(m_Entity.GetIsFrozen());
		}
	}

	bool CF_LoadVariables(CF_ModStorage ctx, int version)
	{
		int varFlags;
		
		if (!ctx.Read(varFlags))
		{
			EXPrint(this, "---- CF_OnStoreLoad failed to load variable flags  ----");
			return false;
		}
		
		if (varFlags & ItemVariableFlags.FLOAT)
		{
			if (!CF_ReadVarsFromCTX(ctx, version))
				return false;
		}

		return true;
	}

	bool CF_ReadVarsFromCTX(CF_ModStorage ctx, int version)
	{
		int mask;
		
		if (!ctx.Read(mask))
		{
			EXPrint(this, "---- CF_OnStoreLoad failed to read variables mask  ----");
			return false;
		}

		m_Entity.m_VariablesMask = mask;

		if (mask & VARIABLE_TEMPERATURE)
		{
			float temperature;
			if (!ctx.Read(temperature))
			{
				EXPrint(this, "---- CF_OnStoreLoad failed to read temperature  ----");
				return false;
			}

			m_Entity.SetTemperatureDirect(temperature);

			bool isFrozen;
			if (!ctx.Read(isFrozen))
			{
				EXPrint(this, "---- CF_OnStoreLoad failed to read frozen  ----");
				return false;
			}

			m_Entity.SetFrozen(isFrozen);
		}
		
		return true;
	}
#endif
}
