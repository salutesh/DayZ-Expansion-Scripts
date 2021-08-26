/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	static void AddPlayer( PlayerBase player )
	{
		if ( !player )
			return;

		if ( player.GetIdentity() )
		{
			player.m_PlayerUID = player.GetIdentity().GetId();
			player.m_PlayerSteam = player.GetIdentity().GetPlainId();
			player.m_PlayerName = player.GetIdentity().GetName();
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CORE] PlayerBase::SetActions start");
		#endif

		super.SetActions( InputActionMap );

		AddAction( ExpansionActionPaint, InputActionMap );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[CORE] PlayerBase::SetActions end");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::OnVariablesSynchronized - Start");
		#endif
		
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::OnVariablesSynchronized - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Init
	// ------------------------------------------------------------
	override void Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Init start");
		#endif
		
		super.Init();

		RegisterNetSyncVariableBool("m_SafeZoneSynchRemote");

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Init end");
		#endif
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
		if (!IsInSafeZone())
			m_Expansion_SafeZoneInstance.Update();
	}

	override void OnReconnect()
	{
		super.OnReconnect();

		//! Make sure we check straight away if player reconnects in a safezone
		if (!IsInSafeZone())
			m_Expansion_SafeZoneInstance.Update();
	}

	// ------------------------------------------------------------
	// PlayerBase OnEnterZone, server + client
	// ------------------------------------------------------------
	void OnEnterZone(ExpansionZoneType type)
	{
		EXPrint(ToString() + "::OnEnterZone");
		Print(type);

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterZone start");
		#endif
		
		if (type == ExpansionZoneType.SAFE)
		{
			OnEnterSafeZone();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterZone end");
		#endif
	}

	#ifdef ENFUSION_AI_PROJECT
	protected eAIGroup ExpansionGetEAIGroup(bool createIfNoneExists = true)
	{
		for (int i = 0; i < eAIGroup.GROUPS.Count(); i++) {
			if (!eAIGroup.GROUPS[i]) continue;
			eAIBase GrpLeader = eAIGroup.GROUPS[i].GetLeader();
			if (GrpLeader && GrpLeader == this)
				return eAIGroup.GROUPS[i];
		}
		
		if (!createIfNoneExists) return null;
		
		eAIGroup newGroup = new eAIGroup();
		newGroup.SetLeader(this);
		eAIBase eAI_PB = eAIBase.Cast(this);
		eAI_PB.SetGroup(newGroup);
		return newGroup;
	}
	#endif

	// ------------------------------------------------------------
	// PlayerBase OnExitZone, only server side
	// ------------------------------------------------------------
	void OnExitZone(ExpansionZoneType type)
	{
		EXPrint(ToString() + "::OnExitZone");
		Print(type);

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnExitZone - Start");
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

			//! Delay actually leaving the safezone by 10 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnLeftSafeZone, 10000, false);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnExitZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase OnEnterSafeZone, server + client
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		EXPrint(ToString() + "::OnEnterSafeZone");
		Print(m_LeavingSafeZone);
		Print(m_SafeZone);
		Print(m_SafeZoneSynchRemote);

		m_SafeZone = true;
		m_LeavingSafeZone = false;

		#ifdef ENFUSION_AI_PROJECT
		if (IsInherited(eAIBase))
		{
			#ifdef EXPANSIONMODMARKET
			//! If this is a trader AI, we still want it to be able to raise hands in safezones
			if (IsInherited(ExpansionTraderAIBase))
				return;
			#endif
		}
		#endif

		SetCanRaise(false);

		if (GetGame().IsClient())
			return;

		m_SafeZoneSynchRemote = true;

		SetAllowDamage(false);
		
		if ( GetIdentity() )
		{
			ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", "STR_EXPANSION_SAFEZONE_ENTER", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create(GetIdentity());
			
			if ( GetExpansionSettings().GetLog().Safezone )
				GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Entered the safezone" );
		}

		#ifdef ENFUSION_AI_PROJECT
		eAIFactionCivilian civilian = new eAIFactionCivilian();
		eAIBase eAI_PB = eAIBase.Cast(this);
		eAIGroup group = eAI_PB.GetGroup();
		if ( !group )
		{
			//! If (re)connecting in a safezone, there will be no group initially
			group = ExpansionGetEAIGroup();
		}
		if ( !group.GetFaction().isFriendly( civilian ) )
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
		EXPrint(ToString() + "::OnLeftSafeZone");
		Print(m_LeavingSafeZone);
		Print(m_SafeZone);
		Print(m_SafeZoneSynchRemote);

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - Start");
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
		if (IsInherited(eAIBase))
		{
			#ifdef EXPANSIONMODMARKET
			//! If this is a trader AI, we don't want it to be able to take damage outside a safezone
			if (IsInherited(ExpansionTraderAIBase))
				return;
			#endif
		}
		#endif

		m_SafeZoneSynchRemote = false;

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
			eAIBase eAI_PB = eAIBase.Cast(this);
			EXPrint(ToString() + "::OnLeftSafeZone " + GetPosition() + " - assigning faction " + m_eAI_Faction_NotInSafeZone );
			eAI_PB.GetGroup().SetFaction( m_eAI_Faction_NotInSafeZone );
			m_eAI_Faction_NotInSafeZone = NULL;
		}
		#endif
	
		SetSynchDirty();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - End");
		#endif
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
}
