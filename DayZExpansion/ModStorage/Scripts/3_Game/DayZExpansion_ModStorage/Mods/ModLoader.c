#ifdef EXPANSION_MODSTORAGE
modded class ModLoader
{
	override static bool _CF_ReadModStorage(Serializer ctx, int version, inout array<ref CF_ModStorage> unloadedMods, inout int stackIndex, inout CF_ModStorageMap loadedMods)
	{
#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_2("ModLoader", "_CF_ReadModStorage").Add(ctx).Add(version);
#endif

		LoadMods();

		int hashA, hashB;
		bool exists = false;
		CF_ModStorage storage;

		if (version > 3)
		{
			if (!ctx.Read(hashA)) return false;
			if (!ctx.Read(hashB)) return false;
		}
		else
		{
			CF_String modName;
			if (!ctx.Read(modName)) return false;

			hashA = modName.Hash();
			hashB = modName.Reverse().Hash();
		}

		auto hash = s_CF_ModStorageHashedMap[hashA];
		if (hash)
		{
			storage = hash[hashB];
			exists = storage != null;
		}

		if (!exists)
		{
			storage = new CF_ModStorage(null);

			storage.m_HashA = hashA;
			storage.m_HashB = hashB;
		}

		storage.m_CF_Version = version;

		if (!ctx.Read(storage.m_Version)) return false;
		bool hasData;
		if (version < CF_ModStorage.VERSION)
		{
			//if (!ctx.Read(storage.m_Data)) return false;
			//hasData = storage.m_Data.Length() > 0;
			//! Bail on old ModStorage data.
			//! Should be safe since it was never in production use and will avoid problems in case of corrupted storage
			//! (e.g. DayZ will consume all available memory and crash when it tries to read a string with negative length).
			int numberOfBytes;
			ctx.Read(numberOfBytes);
			CF_Log.Error("Reading deprecated modstorage v" + version + " is not supported, discarding " + numberOfBytes + " bytes for mod " + storage.GetModName());
			return false;
		}
		else
		{
			int entries;
			if (!ctx.Read(entries)) return false;
			hasData = entries > 0;
#ifdef EXPANSION_MODSTORAGE_DEBUG
			EXPrint("Reading " + entries + " entries for mod " + storage.GetModName());
#endif
			while (entries > 0)
			{
				int type = -1;
				if (!ctx.Read(type)) return false;
				switch (type)
				{
					case Expansion_ModStorageDataType.BOOL:
						bool b = false;
						if (!ctx.Read(b)) return false;
						storage._Insert(b);
						break;
					case Expansion_ModStorageDataType.INT:
						int i = 0;
						if (!ctx.Read(i)) return false;
						storage._Insert(i);
						break;
					case Expansion_ModStorageDataType.FLOAT:
						float f = 0;
						if (!ctx.Read(f)) return false;
						storage._Insert(f);
						break;
					case Expansion_ModStorageDataType.VECTOR:
						float x = 0, y = 0, z = 0;
						if (!ctx.Read(x)) return false;
						if (!ctx.Read(y)) return false;
						if (!ctx.Read(z)) return false;
						storage._Insert(Vector(x, y, z));
						break;
					case Expansion_ModStorageDataType.STRING:
						string s = "";
						if (!ctx.Read(s)) return false;
						storage._Insert(s);
						break;
					default:
						CF_Log.Error("Failed to read unknown data type %1 for mod %2", type.ToString(), storage.GetModName());
						return false;
				}
				entries--;
			}
		}

		if (exists)
		{
			loadedMods.Insert(storage.GetMod().GetName(), storage);
		}
		else if (hasData)
		{
			unloadedMods[stackIndex] = storage;
			stackIndex++;
		}

		return true;
	}
};
#endif
