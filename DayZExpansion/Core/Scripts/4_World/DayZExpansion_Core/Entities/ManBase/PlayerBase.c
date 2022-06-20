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

	private static autoptr map< string, PlayerBase > m_AllPlayersUID = new map< string, PlayerBase >;

	static PlayerBase s_AllPlayers;
	
	PlayerBase m_Expansion_NextPlayer;
	PlayerBase m_Expansion_PrevPlayer;

	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;
	protected bool m_LeavingSafeZone;

	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<PlayerBase>(this);

	ref map<typename, ref ExpansionInventoryItemType> m_Expansion_InventoryItemTypes = new map<typename, ref ExpansionInventoryItemType>;

	void PlayerBase()
	{
		m_Expansion_NextPlayer = s_AllPlayers;
		m_Expansion_PrevPlayer = null;

		if (m_Expansion_NextPlayer)
		{
			m_Expansion_NextPlayer.m_Expansion_PrevPlayer = this;
		}

		s_AllPlayers = this;

		if ( IsMissionClient() && GetGame() && GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ) ) 
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeferredClientInit, 100, false );
	}
	
	void ~PlayerBase()
	{
		if (m_Expansion_PrevPlayer)
		{
			m_Expansion_PrevPlayer.m_Expansion_NextPlayer = m_Expansion_NextPlayer;
		}

		if (m_Expansion_NextPlayer)
		{
			m_Expansion_NextPlayer.m_Expansion_PrevPlayer = m_Expansion_PrevPlayer;
		}
	
		if ( m_AllPlayersUID && m_PlayerUID && m_AllPlayersUID.Get( m_PlayerUID ) == this )
		{
			m_AllPlayersUID.Remove( m_PlayerUID );
		}
	}

	static void Expansion_SendNear(ScriptRPC rpc, int id, vector position, float distance, Object target = null, bool guaranteed = false)
	{
		PlayerBase player = s_AllPlayers;
		while (player)
		{
			if (player.GetIdentity() && vector.Distance(player.GetPosition(), position) < distance)
			{
				rpc.Send(target, id, guaranteed, player.GetIdentity());
			}

			player = player.m_Expansion_NextPlayer;
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
	static array< PlayerBase > GetAll()
	{
		Error("DEPRECATED");
		return new array< PlayerBase >();
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetPlayerByUID
	// string id = PlayerIdentity::GetId()
	// Can be used on server/client side
	// ------------------------------------------------------------
	static PlayerBase GetPlayerByUID( string id )
	{
		if ( IsMissionHost() )
		{
			return m_AllPlayersUID.Get( id );
		} else
		{
			for ( int j = 0; j < ClientData.m_PlayerBaseList.Count(); ++j )
			{
				PlayerBase player;
				if ( !Class.CastTo( player, ClientData.m_PlayerBaseList[j] ) || !player.GetIdentity() )
					continue;
				
				if ( player.GetIdentity().GetId() == id )
					return player;
			}
		}
		
		return NULL;
	}

	//! GetPlayerByUID should probably be used over this where possible because it incurs less cost.
	//! Functionality-wise, this is identical to COT's GetPlayerObjectByIdentity
	static PlayerBase ExpansionGetPlayerByIdentity( PlayerIdentity identity )
	{
		if ( !GetGame().IsMultiplayer() )
			return PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( identity == NULL )
			return NULL;

		int networkIdLowBits;
		int networkIdHighBits;
		GetGame().GetPlayerNetworkIDByIdentityID( identity.GetPlayerId(), networkIdLowBits, networkIdHighBits );

		return PlayerBase.Cast( GetGame().GetObjectByNetworkId( networkIdLowBits, networkIdHighBits ) );
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
			m_AllPlayersUID.Set( player.m_PlayerUID, player );
	}
	
	// ------------------------------------------------------------
	// PlayerBase RemovePlayer
	// Only called server side, to get only alive players
	// ------------------------------------------------------------
	static void RemovePlayer( string id )
	{
		m_AllPlayersUID.Remove( id );
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

		if ( m_SafeZoneSynchRemote && !m_SafeZone )
		{
			OnEnterZone(ExpansionZoneType.SAFE);
		} 
		else if ( !m_SafeZoneSynchRemote && m_SafeZone )
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

		RegisterNetSyncVariableBool("m_SafeZoneSynchRemote");
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInSafeZone
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
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
	void OnEnterZone(ExpansionZoneType type)
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
	void OnExitZone(ExpansionZoneType type)
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

			m_LeavingSafeZone = true;

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
		Print(m_LeavingSafeZone);
		Print(m_SafeZone);
		Print(m_SafeZoneSynchRemote);

		m_SafeZone = true;
		m_LeavingSafeZone = false;

		#ifdef ENFUSION_AI_PROJECT
		#ifdef EXPANSIONMODMARKET
		if (IsAI())
		{
			//! If this is a trader AI, we still want it to be able to raise hands in safezones
			if (IsInherited(ExpansionTraderAIBase))
				return;
		}
		#endif
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

		m_SafeZoneSynchRemote = true;
		
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
			m_SafeZone = false;
			SetCanRaise(true);
			return;
		}

		if (!m_LeavingSafeZone)
			return;
		
		m_LeavingSafeZone = false;

		m_SafeZone = false;

		#ifdef ENFUSION_AI_PROJECT
		#ifdef EXPANSIONMODMARKET
		if (IsAI())
		{
			//! If this is a trader AI, we don't want it to be able to take damage outside a safezone
			if (IsInherited(ExpansionTraderAIBase))
				return;
		}
		#endif
		#endif

		m_SafeZoneSynchRemote = false;

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
		if (m_SafeZone)
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
