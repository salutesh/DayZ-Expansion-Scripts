#ifdef EXPANSION_MODSTORAGE
modded class CF_ModStorage
{
	static const int VERSION = 5;

	static const int GAME_VERSION_WIPE_FILE = 129;

	int m_CF_Version;

	ref array<ref Expansion_ModStorageDataBase> m_Expansion_Entries;
	int m_Idx;
	int m_MaxIdx;

	string GetModName()
	{
		if (m_Mod)
		{
			return m_Mod.GetName();
		}

		return "unknown<" + m_HashA + "," + m_HashB + ">";
	}

	override bool Read(out bool value)
	{
		if (m_CF_Version < VERSION)
			return super.Read(value);

		if (m_Idx > m_MaxIdx || m_Expansion_Entries[m_Idx].ValueType() != bool)
			return false;

		value = Expansion_ModStorageData<bool>.Cast(m_Expansion_Entries[m_Idx++]).Get();

		return true;
	}

	override bool Read(out int value)
	{
		if (m_CF_Version < VERSION)
			return super.Read(value);

		if (m_Idx > m_MaxIdx || m_Expansion_Entries[m_Idx].ValueType() != int)
			return false;

		value = Expansion_ModStorageData<int>.Cast(m_Expansion_Entries[m_Idx++]).Get();

		return true;
	}

	override bool Read(out float value)
	{
		if (m_CF_Version < VERSION)
			return super.Read(value);

		if (m_Idx > m_MaxIdx || m_Expansion_Entries[m_Idx].ValueType() != float)
			return false;

		value = Expansion_ModStorageData<float>.Cast(m_Expansion_Entries[m_Idx++]).Get();

		return true;
	}

	override bool Read(out vector value)
	{
		if (m_CF_Version < VERSION)
			return super.Read(value);

		if (m_Idx > m_MaxIdx || m_Expansion_Entries[m_Idx].ValueType() != vector)
			return false;

		value = Expansion_ModStorageData<vector>.Cast(m_Expansion_Entries[m_Idx++]).Get();

		return true;
	}

	override bool Read(out string value)
	{
		if (m_CF_Version < VERSION)
			return super.Read(value);

		if (m_Idx > m_MaxIdx || m_Expansion_Entries[m_Idx].ValueType() != string)
			return false;

		value = Expansion_ModStorageData<string>.Cast(m_Expansion_Entries[m_Idx++]).Get();

		return true;
	}

	override void Write(bool value)
	{
		_Insert(value);
	}

	override void Write(int value)
	{
		_Insert(value);
	}

	override void Write(float value)
	{
		_Insert(value);
	}

	override void Write(vector value)
	{
		_Insert(value);
	}

	override void Write(string value)
	{
		_Insert(value);
	}

	override void _CopyStreamTo(Serializer ctx)
	{
		if (m_CF_Version < VERSION)
		{
			super._CopyStreamTo(ctx);
			return;
		}

		auto tmp = m_Expansion_Entries;

		// force resetting early so we can write the latest version
		_ResetStream();

		ctx.Write(m_HashA);
		ctx.Write(m_HashB);
		
		ctx.Write(m_Version);

		ctx.Write(tmp.Count());
		foreach (auto entry: tmp)
		{
			switch (entry.ValueType())
			{
				case bool:
					Expansion_ModStorageData<bool> entryBool = Expansion_ModStorageData<bool>.Cast(entry);
					ctx.Write(Expansion_ModStorageDataType.BOOL);
					ctx.Write(entryBool.Get());
					break;
				case int:
					Expansion_ModStorageData<int> entryInt = Expansion_ModStorageData<int>.Cast(entry);
					ctx.Write(Expansion_ModStorageDataType.INT);
					ctx.Write(entryInt.Get());
					break;
				case float:
					Expansion_ModStorageData<float> entryFloat = Expansion_ModStorageData<float>.Cast(entry);
					ctx.Write(Expansion_ModStorageDataType.FLOAT);
					ctx.Write(entryFloat.Get());
					break;
				case vector:
					Expansion_ModStorageData<vector> entryVector = Expansion_ModStorageData<vector>.Cast(entry);
					ctx.Write(Expansion_ModStorageDataType.VECTOR);
					vector v = entryVector.Get();
					ctx.Write(v[0]);
					ctx.Write(v[1]);
					ctx.Write(v[2]);
					break;
				case string:
					Expansion_ModStorageData<string> entryString = Expansion_ModStorageData<string>.Cast(entry);
					ctx.Write(Expansion_ModStorageDataType.STRING);
					ctx.Write(entryString.Get());
					break;
				default:
					CF_Log.Error("Failed to write unknown data type %1 for mod %2", entry.Type().ToString(), GetModName());
			}
		}
	}

	// Read and Write functions can't be called, so we can't reset the stream
	override void _ResetStream()
	{
		m_Idx = 0;

		if (!m_Mod)
		{
			if (!m_Expansion_Entries)
			{
				m_Expansion_Entries = new array<ref Expansion_ModStorageDataBase>();
				m_MaxIdx = -1;
			}
			return;
		}

		m_CF_Version = VERSION;

		m_Expansion_Entries = new array<ref Expansion_ModStorageDataBase>();
		m_MaxIdx = -1;

		m_Data = string.Empty;

		m_HashA = m_Mod.m_CF_HashA;
		m_HashB = m_Mod.m_CF_HashB;

		m_Version = m_Mod.GetStorageVersion();
	}

	void _Insert(bool b)
	{
		m_Expansion_Entries.Insert(new Expansion_ModStorageData<bool>(b));
		m_MaxIdx++;
	}

	void _Insert(int i)
	{
		m_Expansion_Entries.Insert(new Expansion_ModStorageData<int>(i));
		m_MaxIdx++;
	}

	void _Insert(float f)
	{
		m_Expansion_Entries.Insert(new Expansion_ModStorageData<float>(f));
		m_MaxIdx++;
	}

	void _Insert(vector v)
	{
		m_Expansion_Entries.Insert(new Expansion_ModStorageData<vector>(v));
		m_MaxIdx++;
	}

	void _Insert(string s)
	{
		m_Expansion_Entries.Insert(new Expansion_ModStorageData<string>(s));
		m_MaxIdx++;
	}
};
#endif
