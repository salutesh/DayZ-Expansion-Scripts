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

	#ifdef ENFUSION_AI_PROJECT
	private autoptr eAIFaction m_eAI_Faction_NotInSafeZone;
	#endif

	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<PlayerBase>(this);

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

		if (!Expansion_HasAdminToolGodMode())
			SetAllowDamage(false);
		
		if ( GetIdentity() )
		{
			ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_ENTER", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
			
			if ( GetExpansionSettings().GetLog().Safezone )
				GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Entered the safezone" );
		}

		#ifdef ENFUSION_AI_PROJECT
		eAIFactionCivilian civilian = new eAIFactionCivilian();
		#ifdef EXPANSIONMODAI
		eAIGroup group = GetGroup();
		#else
		eAIBase eAI_PB = eAIBase.Cast(this);
		eAIGroup group = eAI_PB.GetGroup();
		#endif
		if ( !group )
		{
			//! If (re)connecting in a safezone, there will be no group initially
			group = ExpansionGetEAIGroup();
		}
		#ifdef EXPANSIONMODAI
		if ( !group.GetFaction().IsFriendly( civilian ) )
		#else
		if ( !group.GetFaction().isFriendly( civilian ) )
		#endif
		{
			m_eAI_Faction_NotInSafeZone = group.GetFaction();
			EXPrint(ToString() + "::OnEnterZone " + GetPosition() + " - faction " + m_eAI_Faction_NotInSafeZone + " -> " + civilian );
			//! Assign a neutral faction so AI guards do not see us as a threat
			group.SetFaction( civilian );
		}
		#endif
	
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

		if (!Expansion_HasAdminToolGodMode())
			SetAllowDamage(true);

		SetCanRaise(true);
	
		if ( GetIdentity() )
		{
			ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_LEFT", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
	
			if ( GetExpansionSettings().GetLog().Safezone )
				GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Left the safezone" );
		}

		#ifdef ENFUSION_AI_PROJECT
		if ( m_eAI_Faction_NotInSafeZone )
		{
			//! Assign original faction
			EXPrint(ToString() + "::OnLeftSafeZone " + GetPosition() + " - assigning faction " + m_eAI_Faction_NotInSafeZone );
			#ifdef EXPANSIONMODAI
			GetGroup().SetFaction( m_eAI_Faction_NotInSafeZone );
			#else
			eAIBase eAI_PB = eAIBase.Cast(this);
			eAI_PB.GetGroup().SetFaction( m_eAI_Faction_NotInSafeZone );
			#endif
			m_eAI_Faction_NotInSafeZone = NULL;
		}
		#endif
	
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
	
	// ------------------------------------------------------------
	// PlayerBase EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule && GetIdentity() != NULL)
		{
			string playerUID = GetIdentity().GetId();
			vector pos = GetPosition();
			monitorModule.AddLastPlayerDeathPos(playerUID, pos);
		}
	#endif
		super.EEKilled(killer);
	}
}
