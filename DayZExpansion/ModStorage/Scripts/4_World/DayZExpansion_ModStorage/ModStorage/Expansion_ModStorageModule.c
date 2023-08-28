/**
 * @class Expansion_ModStorageModule
 * 
 * @brief Does not support unloading Core from the mods. Once loaded it can't be removed
 */

#ifdef EXPANSION_MODSTORAGE
[CF_RegisterModule(Expansion_ModStorageModule)]
class Expansion_ModStorageModule : CF_ModuleWorld
{
	static const string s_FileName = "modstorageplayers.bin";
	protected static string s_FilePath;
	protected static bool s_FileExist;

	//! We only need to keep track of players, because if s_FileName exists,
	//! we already know that all items that are not in player inventory are OK to load from modstorage
	//! because they have been saved prior (as long as the server ran for a couple minutes at least)
	protected autoptr map<string, ref Param1<bool>> m_IDs = new map<string, ref Param1<bool>>();
	protected bool m_IsLoaded;

	protected autoptr FileSerializer m_Serializer;

	/**
	 * @brief Checks if the item has a player root and if the player ID is in the map. If they aren't in the map then add and write to the file
	 */
	void AddEntity(EntityAI entity)
	{
		if (!g_Game.IsDedicatedServer())
			return;

		Load();

		PlayerBase player = PlayerBase.Cast(entity.GetHierarchyRootPlayer());
#ifdef EXPANSION_MODSTORAGE_DEBUG_SAVE
		if (player)
			EXPrint("[MODSTORAGE] AddEntity " + entity + " player identity = " + player.GetIdentity() + " id = " + player.Expansion_GetQueuedIdentityId());
#endif
		if (!player || !player.Expansion_GetQueuedIdentityId())
		{
			return;
		}

		string id = player.Expansion_GetQueuedIdentityId();
		if (!_AddPlayer(id, false))
		{
			return;
		}

		m_Serializer.Write(id);
	}

	/**
	 * @brief Checks to see if the item has a player root and if the player ID is in the map
	 */
	bool IsEntity(EntityAI entity)
	{
		if (!g_Game.IsDedicatedServer())
			return true;

		Load();

		PlayerBase player = PlayerBase.Cast(entity.GetHierarchyRootPlayer());
#ifdef EXPANSION_MODSTORAGE_DEBUG_LOAD
		if (player)
			EXPrint("[MODSTORAGE] IsEntity " + entity + " player identity = " + player.GetIdentity() + " id = " + player.Expansion_GetQueuedIdentityId());
#endif
		if (!player || !player.Expansion_GetQueuedIdentityId())
		{
			return s_FileExist;
		}

#ifdef EXPANSION_MODSTORAGE_DEBUG_LOAD
		EXPrint("[MODSTORAGE] IsEntity " + entity + " " + m_IDs[player.Expansion_GetQueuedIdentityId()]);
#endif
		return m_IDs[player.Expansion_GetQueuedIdentityId()] != null;
	}

	/**
	 * @brief Reads the modstorage file
	 */
	void Load(bool reload = false)
	{
		if (m_IsLoaded && !reload)
		{
			return;
		}
		
		m_IsLoaded = true;

		int instanceId = g_Game.ServerConfigGetInt("instanceId");

		string folder = "$mission:storage_" + instanceId + "/";
		if (!FileExist(folder))
		{
			MakeDirectory(folder);
		}

		folder += "communityframework/";
		if (!FileExist(folder))
		{
			MakeDirectory(folder);
		}

		s_FilePath = folder + s_FileName;

		if (m_Serializer) m_Serializer.Close();

		// Clear existing ids
		m_IDs.Clear();

		s_FileExist = FileExist(s_FilePath);
		if (s_FileExist)
		{
			_Open(m_Serializer, s_FilePath, FileMode.READ);
			string id;
			while (true)
			{
				m_Serializer.Read(id);  // always returns true
				if (!id) break;
				_AddPlayer(id, true);
			}
			m_Serializer.Close();

			_Open(m_Serializer, s_FilePath, FileMode.APPEND);
		}
		else
		{
			_Open(m_Serializer, s_FilePath, FileMode.WRITE);
		}
	}

	private void _Open(out FileSerializer file, string path, FileMode fileMode)
	{
		file = new FileSerializer();
		file.Open(path, fileMode);
	}

	/**
	 * @param loaded If the entity was added from the file
	 * 
	 * @return True if newly added entity
	 */
	private bool _AddPlayer(string id, bool loaded)
	{
		if (!m_IDs[id])
		{
#ifdef EXPANSION_MODSTORAGE_DEBUG
			EXPrint("[MODSTORAGE] Adding player ID " + id);
#endif
			m_IDs[id] = new Param1<bool>(loaded);

			return true;
		}

		return false;
	}
};
#endif
