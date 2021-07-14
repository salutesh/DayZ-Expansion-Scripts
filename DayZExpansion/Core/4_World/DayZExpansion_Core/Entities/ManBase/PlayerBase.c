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
	private static autoptr array< PlayerBase > m_AllPlayers = new array< PlayerBase >;
	
	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;
	protected bool m_LeavingSafeZone;

	void PlayerBase()
	{
		m_AllPlayers.Insert( this );
	}
	
	void ~PlayerBase()
	{
		if ( m_AllPlayersUID && m_AllPlayersUID.Get( m_PlayerUID ) == this )
		{
			m_AllPlayersUID.Remove( m_PlayerUID );
		}

		if ( m_AllPlayers )
		{
			int remove_index = m_AllPlayers.Find( this );
			if ( remove_index >= 0 )
			{
				m_AllPlayers.Remove( remove_index );
			}
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
		return m_AllPlayers;
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
					return PlayerBase.Cast( player );
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
		EXPrint("PlayerBase::OnVariablesSynchronized - Start");
		#endif
		
		if (!GetGame().IsClient())
			return;

		if ( m_SafeZoneSynchRemote && !m_SafeZone )
		{
			OnEnterSafeZone();
		} 
		else if ( !m_SafeZoneSynchRemote && m_SafeZone )
		{
			OnLeftSafeZone();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnVariablesSynchronized - End");
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

	// ------------------------------------------------------------
	// PlayerBase OnEnterSafeZone, server + client
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterSafeZone start");
		#endif
		
		if ( IsAlive() )
		{
			m_SafeZone = true;
			m_LeavingSafeZone = false;
	
			SetCanRaise(false);
	
			if (IsMissionHost())
			{
				m_SafeZoneSynchRemote = true;
	
				SetAllowDamage(false);
				
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_ENTER" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, GetIdentity() );
			
				if ( GetIdentity() && GetExpansionSettings().GetLog().Safezone )
					GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Entered the safezone" );
	
				SetSynchDirty();
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterSafeZone end");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase OnLeavingSafeZone, only server side
	// ------------------------------------------------------------
	void OnLeavingSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeavingSafeZone - Start");
		#endif
		
		if ( IsAlive() )
		{
			if (IsMissionHost() && m_SafeZone && !m_LeavingSafeZone)
			{
				m_SafeZone = false;
				m_LeavingSafeZone = true;
	
				GetNotificationSystem().CreateNotification(new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_LEAVING" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, GetIdentity());
				
				//! Wait 10 seconds
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnLeftSafeZone, 10000, false);
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeavingSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase OnLeftSafeZone, server + client
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - Start");
		#endif
		
		if ( IsAlive() )
		{
			if (IsMissionHost() && !m_SafeZone && m_LeavingSafeZone)
			{
				m_SafeZoneSynchRemote = false;
	
				SetAllowDamage(true);
				SetCanRaise(true);
	
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_LEFT" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, GetIdentity() );
			
				m_LeavingSafeZone = false;
	
				if ( GetIdentity() && GetExpansionSettings().GetLog().Safezone )
					GetExpansionSettings().GetLog().PrintLog("[Safezone] Player \"" + GetIdentity().GetName() + "\" (id=" + GetIdentity().GetId() + " pos=" + GetPosition() + ")" + " Left the safezone" );
			
				SetSynchDirty();
			}
			else if (IsMissionClient())
			{
				m_SafeZone = false;
				SetCanRaise(true);
			}
		}
		
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
