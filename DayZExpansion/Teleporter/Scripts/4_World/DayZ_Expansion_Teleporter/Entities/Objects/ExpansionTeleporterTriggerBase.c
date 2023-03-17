/**
 * ExpansionTeleporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionQuestSExpansionTeleporterBase
 * @brief		This class handles teleporter objects used to head-out or take-in quests
 **/

class ExpansionTeleporterTriggerBase: Trigger
{
	protected ref ExpansionTeleportData m_TeleportData;
	protected Expansion_Teleporter_Base m_TeleporterObj;
	protected ref map<PlayerBase, float> m_TeleportingPlayers;
#ifdef EXPANSIONMODHARDLINE
	protected bool m_UseReputation;
#endif
#ifdef EXPANSIONMODQUESTS
	protected bool m_UseQuests;
#endif
	
	protected const float TELEPORT_TIME = 5.0;
	
	void ExpansionTeleporterTriggerBase()
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME | EntityEvent.ENTER | EntityEvent.LEAVE );
		
		m_TeleportingPlayers = new map<PlayerBase, float>;
	#ifdef EXPANSIONMODQUESTS
		m_UseQuests = GetExpansionSettings().GetQuest().EnableQuests;
	#endif
	#ifdef EXPANSIONMODHARDLINE
		m_UseReputation = GetExpansionSettings().GetHardline().UseReputation;
	#endif
	}

	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}
	
	override protected void RemoveInsiderByObject(Object object)
	{
		//! Do nothing..
	}
	
	override void EOnInit(IEntity other, int extra)
	{	
		//SetExtents("-2 -4 -2", "2 4 2");
		SetCollisionSphere(1);
	}
	
	override void EOnEnter(IEntity other, int extra)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{	
			if (!m_TeleportData)
				return;
			
			array<ref ExpansionTeleportPosition> teleportPositions = m_TeleportData.GetTeleportPositions();
			Print(ToString() + "::EOnEnter - Teleport Positions: " + teleportPositions);
			if (!teleportPositions || teleportPositions.Count() == 0)
				return;
			
			bool result = EntityConditions(other);
			if (!result)
			{
				Print(ToString() + "::EOnEnter - Entity conditions result: " + result);
				return;
			}
			
			SetupTeleport(other);
		}
	}
	
	override void EOnLeave(IEntity other, int extra)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			PlayerBase player;
			if (!Class.CastTo(player, other))
				return;
			
			float teleportTime;
			if (m_TeleportingPlayers.Find(player, teleportTime))
				m_TeleportingPlayers.Remove(player);
		}
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		UpdateTeleportingPlayers(timeSlice);
	}
	
	void UpdateTeleportingPlayers(float timeSlice)
	{
		if (!m_TeleportingPlayers || m_TeleportingPlayers.Count() == 0)
			return;

		foreach (PlayerBase player, float teleportTime: m_TeleportingPlayers)
		{
			if (teleportTime < TELEPORT_TIME)
			{
				float newTime = teleportTime + timeSlice;
				m_TeleportingPlayers.Set(player, newTime);
			}
			else if (teleportTime >= TELEPORT_TIME)
			{
				Expansion_Teleport(player);
				m_TeleportingPlayers.Remove(player);
			}
		}
	}
	
	protected bool EntityConditions(IEntity other)
	{
		PlayerBase player;
		if (!Class.CastTo(player, other))
			return false;
		
		float teleportTime;
		if (m_TeleportingPlayers.Find(player, teleportTime))
			return false;

		if (!player.GetIdentity())
			return false;
		
		if (!m_TeleportData)
			return false;

	#ifdef EXPANSIONMODAI
		if (m_TeleportData.GetFactionName() != string.Empty)
		{
			//! Check if player is in required faction
			bool hasFaction;
			eAIGroup playerGroup = player.GetGroup();
			if (playerGroup)
			{		
				eAIFaction playerfaction = playerGroup.GetFaction();
				if (playerfaction)
				{
					if (playerfaction.GetName() == m_TeleportData.GetFactionName())
						hasFaction = true;
				}
			}
			
			if (!hasFaction)
			{
				ExpansionNotification(new StringLocaliser("Teleporter is locked"), new StringLocaliser("You have no access to this teleporter. This teleporter is only for members of the \"" + m_TeleportData.GetFactionName() + "\" faction."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
				return false;
			}
		}
	#endif
	#ifdef EXPANSIONMODHARDLINE
		if (m_UseReputation && m_TeleportData.GetReputation() > 0)
		{
			//! Check if player has required reputation
			int reputation = player.Expansion_GetReputation();
			if (reputation < m_TeleportData.GetReputation())
			{
				ExpansionNotification(new StringLocaliser("Teleporter is locked"), new StringLocaliser("You have no access to this teleporter yet. You need at least " + m_TeleportData.GetReputation() + " reputation to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
				return false;
			}
		}
	#endif
	#ifdef EXPANSIONMODQUESTS
		if (m_UseQuests && m_TeleportData.GetQuestID() > -1)
		{
			//! Check if player has completed required quest
			if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(m_TeleportData.GetQuestID(), player.GetIdentity().GetId()))
			{
				ExpansionQuestConfig questConig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(m_TeleportData.GetQuestID());
				if (!questConig)
				{
					Error(ToString() + "::EntityConditions - Could not get quest config for quest ID: " + m_TeleportData.GetQuestID());
					return false;
				}
	
				ExpansionNotification(new StringLocaliser("Teleporter is locked"), new StringLocaliser("You have no access to this teleporter yet. You need to compelete the quest " + questConig.GetTitle() + " first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
				return false;
			}
		}
	#endif

		return true;
	}
	
	void SetTeleportData(ExpansionTeleportData teleportData, Expansion_Teleporter_Base teleporterObj)
	{
		m_TeleportData = teleportData;
		m_TeleporterObj = teleporterObj;
	}
	
	protected void SetupTeleport(IEntity other)
	{
		PlayerBase player;
		if (!Class.CastTo(player, other))
			return;
		
		m_TeleportingPlayers.Insert(player, 0);
	}
	
	protected void Expansion_Teleport(PlayerBase player)
	{
		if (!m_TeleportData)
			return;
		
		vector position, orientation;
		if (m_TeleportData.GetTeleportPositions().Count() == 1)
		{
			position = m_TeleportData.GetTeleportPositions()[0].GetPosition();
			orientation = m_TeleportData.GetTeleportPositions()[0].GetOrientation();
		}
		else
		{
			ExpansionTeleportPosition randomPos = m_TeleportData.GetTeleportPositions().GetRandomElement();
			if (randomPos)
			{
				position = randomPos.GetPosition();
			orientation = randomPos.GetOrientation();
			}
		}

		if (position[1] == 0)
			position[1] = GetGame().SurfaceY(position[0], position[2]);
		player.SetPosition(position);
		player.SetOrientation(orientation);
		DayZPlayerSyncJunctures.ExpansionTeleport(player, position, orientation);
	#ifdef NAMALSK_SURVIVAL
		ExpansionTeleporterModule.GetModuleInstance().PlayTeleportSound(position);
	#endif
	}
};
class ExpansionTeleporterTrigger: ExpansionTeleporterTriggerBase {};