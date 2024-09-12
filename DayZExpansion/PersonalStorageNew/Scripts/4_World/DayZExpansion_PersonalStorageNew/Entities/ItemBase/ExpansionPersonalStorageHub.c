/**
 * ExpansionPersonalStorageHub.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageHub: BuildingBase
{
	//! Track created containers globally (client/server)
	static ref map<string, ExpansionPersonalStorageContainer> s_Expansion_PersonalStorageContainers = new map<string, ExpansionPersonalStorageContainer>;

	//! Track opened storage hubs globally (client/server)
	static ref map<string, ExpansionPersonalStorageHub> s_Expansion_PersonalStorageHubs = new map<string, ExpansionPersonalStorageHub>;

	void ExpansionPersonalStorageHub()
	{
		//! @note on client, this should run AFTER Expansion_OnOwnerSync has been invoked on all existing containers,
		//! on server it should run after all existing containers have been loaded from storage
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_FindContainersInVicinity, 1000);
	}

	override bool CanObstruct()
	{
		return false;
	}

	override void SetActions()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif

		super.SetActions();

		AddAction(ExpansionActionOpenPersonalStorage);
		AddAction(ExpansionActionClosePersonalStorage);
	}

	void Expansion_FindContainersInVicinity()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif

		vector pos = GetPosition();
		vector min = Vector(pos[0] - 1, pos[1] - 1, pos[2] - 1);
		vector max = Vector(pos[0] + 1, pos[1] + 1, pos[2] + 1);
		array<EntityAI> entities = {};

		DayZPlayerUtils.SceneGetEntitiesInBox(min, max, entities);

		ExpansionPersonalStorageContainer container;
		foreach (EntityAI entity: entities)
		{
			if (Class.CastTo(container, entity))
			{
				string uid = container.ExpansionGetContainerOwnerUID();
				s_Expansion_PersonalStorageContainers[uid] = container;
				container.Expansion_SetPersonalStorageHub(this, false);
				s_Expansion_PersonalStorageHubs[uid] = this;
			}
		}
	}

	string Expansion_GetContainerDisplayName(PlayerBase player)
	{
		//! If personal storage is currently open, use container display name
		ExpansionPersonalStorageContainer container = s_Expansion_PersonalStorageContainers[player.GetIdentity().GetId()];
		if (container)
			return container.GetDisplayName();

		string containerName = ConfigGetString("expansionPersonalStorageBase");

	#ifdef EXPANSIONMODHARDLINE
		auto settings = GetExpansionSettings().GetHardline(false);
		if (settings.IsLoaded() && settings.UseReputation)
		{
			int currentRestrictedLevel;
			int lvl = Expansion_GetPersonalStorageLevelEx(player, currentRestrictedLevel);

			if (lvl < 1)
				lvl = currentRestrictedLevel;

			containerName += "_Level" + lvl;
		}
	#endif

		string displayName = GetGame().ConfigGetTextOut(CFG_VEHICLESPATH + " " + containerName + " displayName");

		if (!displayName)
			displayName = "#STR_EXPANSION_PERSONALSTORAGE";

		return displayName;
	}

	bool Expansion_OpenPersonalStorage(PlayerBase player)
	{
		string uid = player.GetIdentityUID();

		#ifdef SERVER

		ExpansionPersonalStorageContainer container = s_Expansion_PersonalStorageContainers[uid];
		if (!container)
		{
			int lvl;

			#ifdef EXPANSIONMODHARDLINE
			lvl = Expansion_GetPersonalStorageLevelEx(player);
			#endif

			if (lvl < 1)
				lvl = 1;

			string containerBase = ConfigGetString("expansionPersonalStorageBase");

			Object obj = GetGame().CreateObjectEx(containerBase + "_Level" + lvl, GetPosition(), ECE_KEEPHEIGHT);
			if (!Class.CastTo(container, obj))
			{
				EXError.Error(this, "Cannot cast " + obj + " to ExpansionPersonalStorageContainer", {});
				GetGame().ObjectDelete(obj);
				return false;
			}

			container.ExpansionSetContainerOwner(uid);

			if (FileExist(container.Expansion_GetPersonalStorageFileName()) && !container.Expansion_RestoreContents())
			{
				GetGame().ObjectDelete(obj);
				return false;
			}

			s_Expansion_PersonalStorageContainers[uid] = container;

			container.Expansion_SetPersonalStorageHub(this);
		}
		else
		{
			//! Teleport existing container to hub
			container.SetPosition(GetPosition());
		}

		container.SetOrientation(GetOrientation());

		//! Make sure the container has a high lifetime so it doesn't expire quickly
		if (!GetEconomyProfile() || GetEconomyProfile().GetLifetime() < 3888000)
			EXPrint(container.GetType() + " is not in types.xml or has a lifetime lower than 3888000", " [WARNING]: ");
		container.SetLifetimeMax(3888000);

		#endif

		s_Expansion_PersonalStorageHubs[uid] = this;

		return true;
	}

	bool Expansion_ClosePersonalStorage(PlayerBase player)
	{
		string uid = player.GetIdentityUID();

		#ifdef SERVER

		ExpansionPersonalStorageContainer container = s_Expansion_PersonalStorageContainers[uid];
		if (!container)
			return false;

		if (!container.Expansion_StoreContentsAndDelete())
			return false;

		#endif

		return true;
	}

#ifdef EXPANSIONMODHARDLINE
	//! @note Can't be on PlayerBase, leads to compile error due load order :-(
	static int Expansion_GetPersonalStorageLevelEx(PlayerBase player, out int nextLevel = -1, out int nextLvlRepReq = -1, out int nextLvlQuestID = 0, out bool completed = false)
	{
	#ifdef SERVER
		#ifdef EXTRACE
		auto trace = EXTrace.StartStack(EXTrace.PERSONALSTORAGE, ExpansionPersonalStorageHub);
	#endif
	#endif

		auto settings = GetExpansionSettings().GetPersonalStorageNew(false);
		if (!settings.IsLoaded())
			return 0;

		int initialLvl = player.Expansion_GetPersonalStorageLevel();
		int lvl = initialLvl;
		int rep = player.Expansion_GetReputation();
		int repReq = -1;

	#ifdef EXPANSIONMODQUESTS
		map<int, bool> completedQuests = new map<int, bool>;
	#endif

		int storageLevel;

		//! Only levels w/ requirements, sorted (asc)
		foreach (auto storageLevelConfig: settings.m_StorageLevelsRequirements_Sorted)
		{
			storageLevel = storageLevelConfig.m_Level;

			int lvlRepReq = storageLevelConfig.ReputationRequirement;
			int questID = storageLevelConfig.QuestID;

			if (storageLevel >= lvl && storageLevel <= lvl + 1 && (lvlRepReq >= repReq || questID))
			{
				nextLevel = storageLevel;
				repReq = lvlRepReq;

				if (questID)
				{
					completed = false;

				#ifdef EXPANSIONMODQUESTS
					if (!completedQuests.Find(questID, completed))
					{
						completed = ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, player.GetIdentity().GetId());
						completedQuests[questID] = completed;
					}
				#endif
				}

				if (rep < lvlRepReq || (questID && !completed))
				{
					if (storageLevel == lvl)
						lvl = 0;  //! Player has no access to current level

					break;
				}

				if (storageLevel > lvl && initialLvl < storageLevel)
					lvl = storageLevel;
			}
		}

	#ifdef SERVER
	#ifdef DIAG_DEVELOPER
		Print(lvl);
		Print(storageLevel);
		Print(nextLevel);
		Print(questID);
		Print(completed);
	#endif
	#endif

		ExpansionPersonalStorageLevel nextLevelConfig;
		if (settings.m_StorageLevelsRequirements.Find(nextLevel, nextLevelConfig))
		{
			nextLvlRepReq = nextLevelConfig.ReputationRequirement;
			nextLvlQuestID = nextLevelConfig.QuestID;
		}
		else
		{
			nextLvlRepReq = -1;
			nextLvlQuestID = 0;
		}

		return lvl;
	}
#endif
}
