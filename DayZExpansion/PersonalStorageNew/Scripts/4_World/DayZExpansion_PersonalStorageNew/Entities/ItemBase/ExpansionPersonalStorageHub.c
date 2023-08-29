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
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		super.SetActions();

		AddAction(ExpansionActionOpenPersonalStorage);
		AddAction(ExpansionActionClosePersonalStorage);
	}

	void Expansion_FindContainersInVicinity()
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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

	bool Expansion_OpenPersonalStorage(PlayerBase player)
	{
		string uid = player.GetIdentityUID();

		#ifdef SERVER

		ExpansionPersonalStorageContainer container = s_Expansion_PersonalStorageContainers[uid];
		if (!container)
		{
			int lvl;

			#ifdef EXPANSIONMODHARDLINE
			lvl = player.Expansion_GetPersonalStorageLevel();
			int rep = player.Expansion_GetReputation();
			int repReq = -1;
			auto settings = GetExpansionSettings().GetPersonalStorageNew();
			foreach (int storageLevel, auto storageLevelConfig: settings.StorageLevels)
			{
				int lvlRepReq = storageLevelConfig.ReputationRequirement;
				if (storageLevel > lvl && lvlRepReq > repReq && rep >= lvlRepReq)
				{
					lvl = storageLevel;
					repReq = lvlRepReq;
				}
			}
			#endif

			if (lvl < 1)
				lvl = 1;

			string containerBase = ConfigGetString("expansionPersonalStorageBase");

			if (!Class.CastTo(container, GetGame().CreateObjectEx(containerBase + "_Level" + lvl, GetPosition(), ECE_KEEPHEIGHT)))
				return false;

			container.ExpansionSetContainerOwner(uid);

			if (FileExist(container.Expansion_GetPersonalStorageFileName()) && !container.Expansion_RestoreContents())
				return false;

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
}
