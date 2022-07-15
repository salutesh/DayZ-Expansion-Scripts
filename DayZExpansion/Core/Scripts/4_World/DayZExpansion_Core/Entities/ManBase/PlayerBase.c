/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionInventoryItemType
{
	int Count;
	int WorkingCount;
}

modded class PlayerBase
{
	protected string m_PlayerUID;
	protected string m_PlayerSteam;
	protected string m_PlayerName;

	private static autoptr map< string, PlayerBase > s_Expansion_AllPlayersUID = new map< string, PlayerBase >;

	private static ref set<PlayerBase> s_Expansion_AllPlayers = new set<PlayerBase>;

	//! XXX: Linked list DOES NOT WORK
	//static PlayerBase s_AllPlayers;
	
	PlayerBase m_Expansion_NextPlayer;
	PlayerBase m_Expansion_PrevPlayer;

	protected bool m_Expansion_IsInSafeZoneSynchRemote;
	protected bool m_Expansion_LeavingSafeZone;

	ref map<typename, ref ExpansionInventoryItemType> m_Expansion_InventoryItemTypes = new map<typename, ref ExpansionInventoryItemType>;

	void PlayerBase()
	{
		//m_Expansion_NextPlayer = s_AllPlayers;
		//m_Expansion_PrevPlayer = null;

		//if (m_Expansion_NextPlayer)
		//{
			//m_Expansion_NextPlayer.m_Expansion_PrevPlayer = this;
		//}

		//s_AllPlayers = this;

		s_Expansion_AllPlayers.Insert(this);

		if ( IsMissionClient() && GetGame() && GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ) ) 
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeferredClientInit, 100, false );
	}
	
	void ~PlayerBase()
	{
		if (!GetGame())
			return;

		//if (s_AllPlayers == this)
		//{
			//s_AllPlayers = m_Expansion_NextPlayer;
		//}

		//if (m_Expansion_PrevPlayer)
		//{
			//m_Expansion_PrevPlayer.m_Expansion_NextPlayer = m_Expansion_NextPlayer;
		//}

		//if (m_Expansion_NextPlayer)
		//{
			//m_Expansion_NextPlayer.m_Expansion_PrevPlayer = m_Expansion_PrevPlayer;
		//}
	
		if ( m_PlayerUID && s_Expansion_AllPlayersUID.Get( m_PlayerUID ) == this )
		{
			s_Expansion_AllPlayersUID.Remove( m_PlayerUID );
		}

		int index = s_Expansion_AllPlayers.Find(this);
		if (index > -1)
			s_Expansion_AllPlayers.Remove(index);
	}

	static void Expansion_SendNear(ScriptRPC rpc, int id, vector position, float distance, Object target = null, bool guaranteed = false)
	{
		float distanceSq = distance * distance;
		//PlayerBase player = s_AllPlayers;
		//while (player)
		foreach (string uid, PlayerBase player: s_Expansion_AllPlayersUID)
		{
			if (player.GetIdentity() && vector.DistanceSq(player.GetPosition(), position) < distanceSq)
			{
				rpc.Send(target, id, guaranteed, player.GetIdentity());
			}

			//player = player.m_Expansion_NextPlayer;
		}
	}

	// ------------------------------------------------------------
	// PlayerBase GetIdentitySteam
	// ------------------------------------------------------------
	string GetIdentitySteam()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetPlainId();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerSteam;
	}

	// ------------------------------------------------------------
	// PlayerBase GetIdentityName
	// ------------------------------------------------------------
	string GetIdentityName()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetName();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerName;
	}

	// ------------------------------------------------------------
	// PlayerBase GetAll
	// ------------------------------------------------------------
	//static array< PlayerBase > GetAll()
	//{
		//Error("DEPRECATED");
		//return new array< PlayerBase >();
	//}
	
	static set< PlayerBase > GetAll()
	{
		return s_Expansion_AllPlayers;
	}

	static set<PlayerBase> Expansion_GetInSphere(vector position, int radius)
	{
		int radiusSq = radius * radius;
		set<PlayerBase> players();

		if ( IsMissionHost() )
		{
			foreach (PlayerBase pb: s_Expansion_AllPlayersUID)
			{
				if (vector.DistanceSq(position, pb.GetPosition()) > radiusSq)
					continue;

				players.Insert(pb);
			}
		}
		else
		{
			PlayerBase pbc;
			//! @note ClientData.m_PlayerBaseList only contains players in network bubble
			foreach (Man player: ClientData.m_PlayerBaseList)
			{
				if (!Class.CastTo( pbc, player ))
					continue;

				if (vector.DistanceSq(position, pbc.GetPosition()) > radiusSq)
					continue;

				players.Insert(pbc);
			}
		}
		
		return players;
	}

	// ------------------------------------------------------------
	// PlayerBase GetPlayerByUID
	// string id = PlayerIdentity::GetId()
	// Can be used on server/client side (note that on client, will only see players in network bubble)
	// ------------------------------------------------------------
	static PlayerBase GetPlayerByUID( string id )
	{
		if ( IsMissionHost() )
		{
			return s_Expansion_AllPlayersUID.Get( id );
		}
		else
		{
			PlayerBase pbc;
			//! @note ClientData.m_PlayerBaseList only contains players in network bubble
			foreach (Man player: ClientData.m_PlayerBaseList)
			{
				if ( !Class.CastTo( pbc, player ) || !pbc.GetIdentity() )
					continue;
				
				if ( pbc.GetIdentity().GetId() == id )
					return pbc;
			}
		}
		
		return NULL;
	}

	// ------------------------------------------------------------
	// PlayerBase Expansion_GetByPlainID
	// string id = PlayerIdentity::GetPlainId()
	// Can be used on server/client side (note that on client, will only see players in network bubble)
	static PlayerBase Expansion_GetByPlainID(string id)
	{
		if ( IsMissionHost() )
		{
			foreach (PlayerBase pb: s_Expansion_AllPlayersUID)
			{
				if (!pb.GetIdentity())
					continue;
				
				if ( pb.GetIdentity().GetPlainId() == id )
					return pb;
			}
		}
		else if (ClientData.m_PlayerList && ClientData.m_PlayerList.m_PlayerList)
		{
			//! @note on client, PlayerIdentity::GetPlainId() will return empty string,
			//! so we have to find corresponding SyncPlayer and can then match with associated player using normal (hashed) ID.
			//! While ClientData.m_PlayerList.m_PlayerList contains ALL players, ClientData.m_PlayerBaseList only contains players in network bubble
			foreach (SyncPlayer syncPlayer: ClientData.m_PlayerList.m_PlayerList)
			{
				if (!syncPlayer)
					continue;

				if (syncPlayer.m_UID == id)
					return GetPlayerByUID(syncPlayer.m_RUID);
			}
		}

		return NULL;
	}

	//! GetPlayerByUID should probably be used over this where possible
	static PlayerBase ExpansionGetPlayerByIdentity( PlayerIdentity identity )
	{
		if ( !GetGame().IsMultiplayer() )
			return PlayerBase.Cast( GetGame().GetPlayer() );

		return GetPlayerByUID(identity.GetId());
	}

	static bool Expansion_IsOnline(string uid)
	{
		if ( IsMissionHost() )
		{
			return s_Expansion_AllPlayersUID.Contains(uid);
		}
		else if (ClientData.m_PlayerList && ClientData.m_PlayerList.m_PlayerList)
		{
			foreach (SyncPlayer syncPlayer: ClientData.m_PlayerList.m_PlayerList)
			{
				if (!syncPlayer)
					continue;

				if (syncPlayer.m_RUID == uid)
					return true;
			}
		}
		
		return false;
	}

	// ------------------------------------------------------------
	// PlayerBase GetIdentityUID
	// ------------------------------------------------------------
	string GetIdentityUID()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetId();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerUID;
	}
	
	// ------------------------------------------------------------
	// PlayerBase AddPlayer
	// Only called server side, to get only alive players
	// ------------------------------------------------------------
	static void AddPlayer( PlayerBase player, PlayerIdentity identity )
	{
		if ( !player )
			return;

		if ( identity )
		{
			player.m_PlayerUID = identity.GetId();
			player.m_PlayerSteam = identity.GetPlainId();
			player.m_PlayerName = identity.GetName();
		}
		else
		{
			//! Leave the EXPrint in here. If someone complains about vehicle desync as pilot, ask them about server logs and exact timestamp, then use this to check whether the desyncing player had an identity on connect.
			EXPrint("WARNING: Player without identity cannot be added! " + player);
		}

		if ( player.m_PlayerUID != "" )
			s_Expansion_AllPlayersUID.Set( player.m_PlayerUID, player );
	}
	
	// ------------------------------------------------------------
	// PlayerBase RemovePlayer
	// Only called server side, to get only alive players
	// ------------------------------------------------------------
	static void RemovePlayer( string id )
	{
		s_Expansion_AllPlayersUID.Remove( id );
	}	
		
	// ------------------------------------------------------------
	// PlayerBase DeferredClientInit
	// ------------------------------------------------------------
	void DeferredClientInit()
	{
		
	}

	override void SetActions( out TInputActionMap InputActionMap )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "SetActions");
#endif

		super.SetActions( InputActionMap );

		AddAction( ExpansionActionPaint, InputActionMap );
	}
	
	// ------------------------------------------------------------
	// Expansion OnUnconsciousStart
	// ------------------------------------------------------------
	override void OnUnconsciousStart()
	{
		if ( IsMissionClient() && GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().IsVisible() )
			GetGame().GetUIManager().CloseAll();

		super.OnUnconsciousStart();

		if (!m_Expansion_CanBeLooted)
			Expansion_LockInventory();
	}

	override void OnUnconsciousStop(int pCurrentCommandID)
	{
		super.OnUnconsciousStop(pCurrentCommandID);

		if (!m_Expansion_CanBeLooted)
			Expansion_UnlockInventory();
	}
	
	// ------------------------------------------------------------
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "OnVariablesSynchronized");
#endif

		super.OnVariablesSynchronized();
		
		if (!GetGame().IsClient())
			return;

		if ( m_Expansion_IsInSafeZoneSynchRemote && !m_Expansion_IsInSafeZone )
		{
			OnEnterZone(ExpansionZoneType.SAFE);
		} 
		else if ( !m_Expansion_IsInSafeZoneSynchRemote && m_Expansion_IsInSafeZone )
		{
			OnExitZone(ExpansionZoneType.SAFE);
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Init
	// ------------------------------------------------------------
	override void Init()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "Init");
#endif
		
		super.Init();

		RegisterNetSyncVariableBool("m_Expansion_IsInSafeZoneSynchRemote");
	}

	override void OnConnect()
	{
		super.OnConnect();

		//! Make sure we check straight away if player connects in a safezone
		if (!IsInSafeZone() && GetExpansionSettings().GetSafeZone().Enabled)
			m_Expansion_SafeZoneInstance.Update();
	}

	override void OnReconnect()
	{
		super.OnReconnect();

		//! Make sure we check straight away if player reconnects in a safezone
		if (!IsInSafeZone() && GetExpansionSettings().GetSafeZone().Enabled)
			m_Expansion_SafeZoneInstance.Update();
	}

	// ------------------------------------------------------------
	// PlayerBase OnEnterZone, server + client
	// ------------------------------------------------------------
	override void OnEnterZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "OnEnterZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			OnEnterSafeZone();
		}
	}

	#ifdef ENFUSION_AI_PROJECT
	protected eAIGroup ExpansionGetEAIGroup(bool createIfNoneExists = true)
	{
		#ifdef EXPANSIONMODAI
		eAIGroup group = eAIGroup.GetGroupByLeader(this, false);
		if (group)
			return group;
		#else
		for (int i = 0; i < eAIGroup.GROUPS.Count(); i++) {
			if (!eAIGroup.GROUPS[i]) continue;
			eAIBase GrpLeader = eAIGroup.GROUPS[i].GetLeader();
			if (GrpLeader && GrpLeader == this)
				return eAIGroup.GROUPS[i];
		}
		#endif
		
		if (!createIfNoneExists) return null;
		
		#ifdef EXPANSIONMODAI
		eAIGroup newGroup = eAIGroup.CreateGroup();
		SetGroup(newGroup);
		#else
		eAIGroup newGroup = new eAIGroup();
		newGroup.SetLeader(this);
		eAIBase eAI_PB = eAIBase.Cast(this);
		eAI_PB.SetGroup(newGroup);
		#endif
		return newGroup;
	}
	#endif

	// ------------------------------------------------------------
	// PlayerBase OnExitZone, only server side
	// ------------------------------------------------------------
	override void OnExitZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "OnExitZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			if (GetGame().IsClient())
			{
				// OnLeftSafeZone needs to be called on client
				OnLeftSafeZone();
				return;
			}

			m_Expansion_LeavingSafeZone = true;

			if ( GetIdentity() )
				ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_LEAVING", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
				
			//TODO: expose to settings

			//! Remove any previous call to OnLeftSafeZone from the queue
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnLeftSafeZone);

			//! Delay actually leaving the safezone by 10 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnLeftSafeZone, 10000, false);
		}
	}

	bool Expansion_HasAdminToolGodMode()
	{
		bool godMode;
#ifdef JM_COT
		godMode |= COTHasGodMode();
#endif
#ifdef VPPADMINTOOLS
		godMode |= GodModeStatus();
#endif
		return godMode;
	}

	// ------------------------------------------------------------
	// PlayerBase OnEnterSafeZone, server + client
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		//! Avoid exploit where you are just outside safezone, get shot uncon, fall backwards into safezone,
		//! then disconnect and reconnect to dupe your character
		//! (your other unconscious body will still be on the ground inside safezone due to having gained godmode from it)
		if (IsUnconscious())
			return;

		EXPrint(ToString() + "::OnEnterSafeZone");
		Print(m_Expansion_LeavingSafeZone);
		Print(m_Expansion_IsInSafeZone);
		Print(m_Expansion_IsInSafeZoneSynchRemote);

		m_Expansion_IsInSafeZone = true;
		m_Expansion_LeavingSafeZone = false;

		#ifdef ENFUSION_AI_PROJECT
		if (IsAI())
		{
			//! If this is AI, we still want it to be able to raise hands in safezones so it can reload its weapon
			return;
		}
		#endif

		SetCanRaise(false);

		if (GetGame().IsClient())
			return;

		if (IsRestrained())
		{
			SetRestrained(false);

			EntityAI item_in_hands = GetItemInHands();
			if (item_in_hands)
				MiscGameplayFunctions.TransformRestrainItem(item_in_hands, null, null, this);
		}

		m_Expansion_IsInSafeZoneSynchRemote = true;
		
		if ( GetIdentity() )
		{
			ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_ENTER", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
			
			if ( GetExpansionSettings().GetLog().Safezone )
				GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Entered the safezone" );
		}
	
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// PlayerBase OnLeftSafeZone, server + client
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "OnLeftSafeZone");
#endif

		if (GetGame().IsClient())
		{
			m_Expansion_IsInSafeZone = false;
			SetCanRaise(true);
			return;
		}

		if (!m_Expansion_LeavingSafeZone)
			return;
		
		m_Expansion_LeavingSafeZone = false;

		m_Expansion_IsInSafeZone = false;

		#ifdef ENFUSION_AI_PROJECT
		if (IsAI())
		{
			return;
		}
		#endif

		m_Expansion_IsInSafeZoneSynchRemote = false;

		SetCanRaise(true);
	
		if ( GetIdentity() )
		{
			ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_LEFT", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
	
			if ( GetExpansionSettings().GetLog().Safezone )
				GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Left the safezone" );
		}
	
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// PlayerBase SetCanRaise
	// ------------------------------------------------------------	
	void SetCanRaise(bool mode)
	{		
		HumanInputController hic = GetInputController();
		if (hic)
		{
			hic.OverrideRaise(!mode, false);
		}
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (m_Expansion_IsInSafeZone)
			return false;

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	// ------------------------------------------------------------
	// PlayerBase EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule && GetIdentity() != NULL)
		{
			string playerUID = GetIdentity().GetId();
			vector pos = GetPosition();
			monitorModule.AddLastPlayerDeathPos(playerUID, pos);
		}
	#endif
		super.EEKilled(killer);
	}

	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		if ( IsAttached() )
		{
			Object attachmentObject = Expansion_GetAttachmentObject();
			bool isVehicle;
			isVehicle |= attachmentObject.IsInherited(CarScript);
		#ifdef EXPANSIONMODVEHICLE
			isVehicle |= attachmentObject.IsInherited(ExpansionVehicleBase);
		#endif
			if (isVehicle)
				return CanPickupHeavyItem(item_to_hands);
		}
		
		return super.CanReceiveItemIntoHands(item_to_hands);
	}

	override void OnPlayerLoaded()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN, this);

		//! Workaround for vanilla timing bug. It's basically chance if OnPlayerLoaded gets called before or after OnRespawnEvent is received on client,
		//! which has the effect that if a server is using respawnTime > 0 and OnRespawnEvent is received before OnPlayerLoaded is called,
		//! then the respawn timer screen may be closed before the countdown finishes due to the call to GetGame().GetUIManager().CloseAll() in OnPlayerLoaded.

		Hud hud;
		LoginTimeBase loginTimeScreen = GetDayZGame().GetLoginTimeScreen();
		if (loginTimeScreen && loginTimeScreen.IsRespawn() && loginTimeScreen.IsVisible())
		{
			hud = m_Hud;
			m_Hud = null;
		}

		super.OnPlayerLoaded();

		if (hud)
		{
			m_Hud = hud;

			//! The following is almost equivalent to the relevant part in vanilla OnPlayerLoaded,
			//! EXCEPT we DON'T call GetGame().GetUIManager().CloseAll()
			if (IsControlledPlayer())
			{
				m_Hud.UpdateBloodName();
				PPERequesterBank.GetRequester(PPERequester_DeathDarkening).Stop();
				PPERequesterBank.GetRequester(PPERequester_ShockHitReaction).Stop();
				PPERequesterBank.GetRequester(PPERequester_UnconEffects).Stop();
				//GetGame().GetUIManager().CloseAll();
				GetGame().GetMission().SetPlayerRespawning(false);
				GetGame().GetMission().OnPlayerRespawned(this);
				
				m_Hud.ShowHudUI( true );
				m_Hud.ShowQuickbarUI(true);
			}
		}
	}

	void Expansion_OnInventoryUpdate(ItemBase item, bool inInventory = true, bool checkWorking = false)
	{
#ifdef EXPANSION_TRACE
		auto trace = EXTrace.Start0(ExpansionTracing.GENERAL_ITEMS, this, "Expansion_OnInventoryUpdate");
		EXTrace.Add(trace, item.ToString());
		EXTrace.Add(trace, inInventory);
		EXTrace.Add(trace, checkWorking);
#endif

		typename type = item.Type();
		typename familyType = item.Expansion_GetFamilyType();

		Expansion_OnInventoryUpdateEx(type, item, inInventory, checkWorking);
#ifdef EXPANSION_TRACE
		ExpansionInventoryItemType itemType = m_Expansion_InventoryItemTypes[type];
		if (itemType)
		{
			EXTrace.Add(trace, itemType.Count);
			EXTrace.Add(trace, itemType.WorkingCount);
		}
#endif

		if (familyType != type)
		{
			Expansion_OnInventoryUpdateEx(familyType, item, inInventory, checkWorking);
#ifdef EXPANSION_TRACE
			itemType = m_Expansion_InventoryItemTypes[familyType];
			if (itemType)
			{
				EXTrace.Add(trace, itemType.Count);
				EXTrace.Add(trace, itemType.WorkingCount);
			}
#endif
		}
	}

	void Expansion_OnInventoryUpdateEx(typename type, ItemBase item, bool inInventory = true, bool checkWorking = false)
	{
		ExpansionInventoryItemType itemType;

		bool found = m_Expansion_InventoryItemTypes.Find(type, itemType);

		if (inInventory)
		{
			if (!found)
			{
				itemType = new ExpansionInventoryItemType;
				m_Expansion_InventoryItemTypes[type] = itemType;
			}

			if (!checkWorking)
			{
				//! Item entered inventory
				itemType.Count++;
			}
			else if (item.GetCompEM())
			{
				//! Item already in inventory
				if (item.GetCompEM().IsWorking())
					itemType.WorkingCount++;
				else if (found && itemType.WorkingCount > 0)
					itemType.WorkingCount--;
			}
		}
		else if (found)
		{
			//! Item exited inventory
			if (itemType.Count > 1)
			{
				itemType.Count--;
				if (item.GetCompEM() && item.GetCompEM().IsWorking() && itemType.WorkingCount > 0)
					itemType.WorkingCount--;
			}
			else
			{
				m_Expansion_InventoryItemTypes.Remove(type);
			}
		}
	}

	int Expansion_GetInventoryCount(typename type, bool working = false)
	{
		ExpansionInventoryItemType itemType;

		if (m_Expansion_InventoryItemTypes.Find(type, itemType))
		{
			if (working)
				return itemType.WorkingCount;
			else
				return itemType.Count;
		}

		return 0;
	}
}
