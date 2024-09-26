#ifdef EXPANSION_MODSTORAGE
class CF_ModStorageObject<Class T> : CF_ModStorageBase
{
	T m_Entity;

	autoptr array<ref CF_ModStorage> m_UnloadedMods;

	Expansion_ModStorageModule m_Expansion_Module;

	bool m_HasModStorage;

	void CF_ModStorageObject(T entity)
	{
		m_Entity = entity;
		m_UnloadedMods = new array<ref CF_ModStorage>();

		ModLoader.LoadMods();
		
		// Reset the stream for 'OnStoreSave'
		foreach (auto mod : ModLoader.s_CF_ModStorages)
		{
			mod._ResetStream();
		}

		m_Expansion_Module = CF_Modules<Expansion_ModStorageModule>.Get();
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
#ifdef CF_MODSTORAGE_TRACE
		auto trace = CF_Trace_1(this, "OnStoreSave").Add(ctx);
#endif

#ifdef EXPANSION_MODSTORAGE_DEBUG_SAVE
		EXPrint("[MODSTORAGE] " + ToString() + " " + m_Entity.GetType() + "::OnStoreSave");
#endif

		if (GetGame().SaveVersion() < CF_ModStorage.GAME_VERSION_FIRST_INSTALL)
		{
			return;
		}

		if (!m_HasModStorage)
		{
			// Add the entity to the file so on next load the game knows that it can read the modstorage for the entity
			m_Expansion_Module.AddEntity(m_Entity);

			m_HasModStorage = true;
		}

		// Write the CF modstorage version
		ctx.Write(CF_ModStorage.VERSION);

		// Reset the stream for 'OnStoreSave'
		foreach (auto mod : ModLoader.s_CF_ModStorages)
		{
			mod._ResetStream();
		}

		m_Entity.CF_OnStoreSave(ModLoader.s_CF_ModStorageMap);

		int modsWithDataCount;
		foreach (auto mod2 : ModLoader.s_CF_ModStorages)
		{
			if (mod2.m_MaxIdx > -1) modsWithDataCount++;
		}

#ifdef EXPANSION_MODSTORAGE_DEBUG_SAVE
		EXPrint("Writing data for " + modsWithDataCount + " loaded and " + m_UnloadedMods.Count() + " unloaded mods")
#endif

		ctx.Write(modsWithDataCount + m_UnloadedMods.Count());

		if (modsWithDataCount)
		{
			foreach (auto mod3 : ModLoader.s_CF_ModStorages)
			{
#ifdef EXPANSION_MODSTORAGE_DEBUG_SAVE
				EXPrint("Writing " + mod3.m_Expansion_Entries.Count() + " entries for mod " + mod3.GetModName())
#endif

				// also resets the stream for next 'OnStoreSave'
				if (mod3.m_MaxIdx > -1) mod3._CopyStreamTo(ctx);
			}
		}

		foreach (auto unloadedMod : m_UnloadedMods)
		{
#ifdef EXPANSION_MODSTORAGE_DEBUG_SAVE
			EXPrint("Writing " + unloadedMod.m_Expansion_Entries.Count() + " entries for unloaded mod " + unloadedMod.GetModName())
#endif
			// Since mod is unloaded, the stream is not reset
			unloadedMod._CopyStreamTo(ctx);
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
#ifdef CF_MODSTORAGE_TRACE
		auto trace = CF_Trace_2(this, "OnStoreLoad").Add(ctx).Add(version);
#endif

#ifdef EXPANSION_MODSTORAGE_DEBUG_LOAD
		EXPrint("[MODSTORAGE] " + ToString() + " " + m_Entity.GetType() + "::OnStoreLoad game save version " + version);
#endif

		// Clearing the unloaded mods data
		m_UnloadedMods.Clear();

		// Persistence version is prior to 1.10
		if (version < CF_ModStorage.GAME_VERSION_FIRST_INSTALL)
		{
			return true;
		}

#ifdef CF_MODSTORAGE
//! Only if CF_MODSTORAGE is defined, it will be possible to add CF without wiping, so the following check only makes sense in that case.
//! Otherwise, CF version data will always be present.

		// If the version is less than the wipe file, the entity will be added automatically in 'OnStoreSave'
		if (version >= CF_ModStorage.GAME_VERSION_WIPE_FILE)
		{
#ifdef EXPANSION_MODSTORAGE_DEBUG_LOAD
			EXPrint("[MODSTORAGE] " + ToString() + " " + m_Entity.GetType() + "::OnStoreLoad game save version " + version + " >= " + CF_ModStorage.GAME_VERSION_WIPE_FILE);
#endif
			if (!m_Expansion_Module.IsEntity(m_Entity))
			{
				// Since the entity wasn't found we can assume that we don't have CF data
				return true;
			}
		}
#endif

		int cf_version;
		if (!ctx.Read(cf_version))
		{
			return false;
		}

#ifdef EXPANSION_MODSTORAGE_DEBUG_LOAD
		Print("[MODSTORAGE] " + ToString() + " " + m_Entity.GetType() + "::OnStoreLoad CF version " + cf_version);
#endif

		// CF version is prior to ModStorage implementation
		if (cf_version < CF_ModStorage.MODSTORAGE_INITIAL_IMPLEMENTATION)
		{
			return true;
		}

		int numMods;
		if (!ctx.Read(numMods))
		{
			return false;
		}

		m_UnloadedMods.Resize(numMods);

		CF_ModStorageMap loadedMods();

		int unloadedModsRead;
		for (int modsRead = 0; modsRead < numMods; modsRead++)
		{
			if (!ModLoader._CF_ReadModStorage(ctx, cf_version, m_UnloadedMods, unloadedModsRead, loadedMods))
			{
				CF_Log.Error("Failed to read modstorage for entity Type=%1, Position=%2", m_Entity.GetType(), m_Entity.GetPosition().ToString());
				break;
			}
		}
		
		m_UnloadedMods.Resize(unloadedModsRead);

		if (modsRead < numMods)
			return false;

		if (!m_Entity.CF_OnStoreLoad(loadedMods))
			ErrorEx(m_Entity.ToString() + "::CF_OnStoreLoad failed", ErrorExSeverity.WARNING);

		// Reset the stream for 'OnStoreSave'
		foreach (auto mod : ModLoader.s_CF_ModStorages)
		{
			mod._ResetStream();
		}

		return true;
	}
};
#endif
