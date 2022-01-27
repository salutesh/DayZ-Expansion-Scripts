/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	protected ref ExpansionElectricityConnection m_ElectricitySource;
	protected ref array< ItemBase > m_ElectricityConnections;

	/*! We need to keep track of base parts health so we can reliably 'heal' them when damage multiplier is zero
	    (for that, we need to know the health of the item before it was damaged, i.e. *before* EEHitBy runs).

		Items considered 'base parts' are currently anything inheriting from ExpansionBaseBuildingBase and ExpansionSafeBase.

		All actions that affect those base part's health (e.g. raiding, repairing) should update m_CurrentHealth afterwards
		by either setting the respective damage zone health in m_CurrentHealth directly or calling UpdateCurrentHealthMap,
		but there is a 'catch-all' in EEHealthLevelChanged to catch at least major changes in health we didn't think about.
	*/
	ref TStringArray m_DmgZones;
	ref map< string, float > m_CurrentHealth;

	protected bool m_Locked;
	protected string m_Code;  //! Only set on server, not synced to client. NEVER set this directly. Use SetCode()
	protected int m_CodeLength;  //! Unlike m_Code, this should be synched to clients for items that use codes

	protected ref TStringArray m_KnownUIDs;
	protected bool m_KnownUIDsRequested;
	protected bool m_KnownUIDsSet;

	//============================================
	// ItemBase Constructor
	//============================================
	void ItemBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase - Start");
		EXPrint("ItemBase::ItemBase - Classname: " + ClassName());
		EXPrint("ItemBase::ItemBase - Type: " + GetType());
		#endif
		
		m_ElectricitySource = new ExpansionElectricityConnection( this );
		m_ElectricityConnections = new array< ItemBase >();
		if ( ExpansionCanRecievePower() )
		{
			//RegisterNetSyncVariableBool( "m_IsPairedSynch" );
			//RegisterNetSyncVariableBool( "m_IsOnSynch" );
			//RegisterNetSyncVariableInt( "m_SourceNetLow" );
			//RegisterNetSyncVariableInt( "m_SourceNetHigh" );
		}
		
		if ( IsMissionHost() && ( IsInherited( ExpansionBaseBuilding ) || IsInherited( ExpansionSafeBase ) ) )
		{
			m_DmgZones = new TStringArray;
			GetDamageZones( m_DmgZones );
			if ( m_DmgZones.Count() == 0 )
				m_DmgZones.Insert( "GlobalHealth" );
			//string dmgZones;
			//for ( int i = 0; i < m_DmgZones.Count(); i++ )
			//{
				//dmgZones += " " + m_DmgZones[i];
			//}
			//Print(GetType() + " ItemBase::ItemBase dmgZones :" + dmgZones);
			m_CurrentHealth = new map< string, float >;
			//! Init current health map to max health
			//! EEOnAfterLoad will call this again to set actual health if loading from storage
			UpdateCurrentHealthMap( "ItemBase::ItemBase" );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase - End");
		#endif
	}
	
	//============================================
	// ItemBase Destructor
	//============================================
	void ~ItemBase()
	{
	}

	void UpdateCurrentHealthMap( string caller = "" )
	{
		if ( IsInherited( ExpansionBaseBuilding ) || IsInherited( ExpansionSafeBase ) )
		{
			//Print(GetType() + " " + caller + " -> ItemBase::UpdateCurrentHealthMap");
			for ( int i = 0; i < m_DmgZones.Count(); i++ )
			{
				float health = GetHealth( m_DmgZones[i], "Health" );
				if ( m_CurrentHealth[m_DmgZones[i]] != health )
				{
					//Print(GetType() + " " + caller + " -> ItemBase::UpdateCurrentHealthMap " + m_DmgZones[i] + " health : " + health);
					m_CurrentHealth[m_DmgZones[i]] = health;
				}
			}
		}
	}

	/**
	 * /note	This is a static boolean, do not change during runtime
	 * 
	 * /brief	Override this to determine if the item can be powered using the expansion power system
	 */
	bool ExpansionCanRecievePower()
	{
		return false;
	}

	/**
	 * /note	This is a static boolean, do not change during runtime
	 * 
	 * /brief	Override this to determine if the item powers other items
	 */
	bool ExpansionIsPowerSource()
	{
		return false;
	}

	bool ExpansionIsConnected( ItemBase source = NULL )
	{
		return m_ElectricitySource.IsConnected( source );
	}

	void ExpansionDisconnect()
	{
		m_ElectricitySource.Disconnect();

		SetSynchDirty();
	}

	void ExpansionAddConnection( ItemBase item )
	{
		Print( "[" + this + "] ExpansionAddConnection item=" + item );
		if ( !ExpansionIsPowerSource() || !item.ExpansionCanRecievePower() )
			return;

		item.m_ElectricitySource.Pair( this );

		SetSynchDirty();
		
		Print( "[" + this + "] ~ExpansionAddConnection" );
	}

	void _ExpansionAddConnection( ItemBase item )
	{
		int idx = m_ElectricityConnections.Find( item );
		if ( idx == -1 )
			m_ElectricityConnections.Insert( item );
	}

	void _ExpansionRemoveConnection( ItemBase item )
	{
		int idx = m_ElectricityConnections.Find( item );
		if ( idx != -1 )
			m_ElectricityConnections.Remove( idx );
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		for ( int i = 0; i < m_ElectricityConnections.Count(); ++i )
		{
			m_ElectricityConnections[i].OnWorkStart();
		}
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();
		
		for ( int i = 0; i < m_ElectricityConnections.Count(); ++i )
		{
			m_ElectricityConnections[i].OnWorkStop();
		}
	}
	
	//============================================
	// IsBasebuilding
	//============================================	
	/**
	\brief Returning if can be built
		\param 	
	*/
	bool IsBasebuilding()
	{
		return false;
	}
	
	//============================================
	// ExpansionIsOpenable
	//============================================	
	/**
	\brief Returning if openable in principle (regardless if player can actually open in current state)
		\param 	
	*/
	bool ExpansionIsOpenable()
	{
		return false;
	}

	/**
	\brief Returning if selection is openable in principle (regardless if player can actually open in current state)
		\param 	
	*/
	bool ExpansionIsOpenable( string selection )
	{
		return ExpansionIsOpenable();
	}
	
	/**
	\brief Returning if item is open
		\param 	
	*/
	bool IsOpened()
	{
		return false;
	}

	/**
	\brief Returning if player can open gate/safe from selection
		\param 
	*/
	bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		return false;
	}

	/**
	\brief Returning if player can close gate/safe from selection
		\param 
	*/
	bool ExpansionCanClose( PlayerBase player, string selection )
	{
		return CanClose( selection );
	}
	
	/**
	\brief Returning if player can close gate/safe from selection
		\param 	
	*/
	bool CanClose( string selection )
	{
		return false;
	}

	/**
	\brief Opening gate/safe on defined selection
		\param 	
	*/
	void Open( string selection ) 
	{
	}

	void UnlockAndOpen( string selection ) 
	{
		Unlock();

		Open( selection );
	}
	
	/**
	\brief Closing gate/safe on defined selection
		\param 	
	*/
	void Close( string selection ) 
	{
	}

	void CloseAndLock( string selection )
	{
		if ( IsOpened() )
			Close( selection );

		ExpansionLock();
	}
	
	/**
	\brief Set code of item
		\param 	
	*/
	void SetCode( string code, PlayerBase player = NULL, bool setUser = true, bool updateLock = true )
	{
		if (!GetGame().IsServer())
		{
			Error("ERROR: ItemBase::SetCode called on client!");
			return;
		}

		//! Check for m_Code allows to set empty code on parent after migration of code to attached codelock
		if (!m_Code && !IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
			{
				codelock.SetCode(code, player, setUser, updateLock);
				return;
			}
		}

		m_Code = code;
		m_CodeLength = code.Length();
		m_Locked = false;

		if (m_KnownUIDs && setUser && GetExpansionSettings().GetBaseBuilding().RememberCode)
			SetUser( player );

		if (IsOpened() || !updateLock)
			SetSynchDirty();
		else if (m_Code)
			ExpansionLock();  //! Will call SetSynchDirty
		else
			Unlock();  //! Will call SetSynchDirty
	}
	
	/**
	\brief Returning code of item
		\param 	
		@note Remember that m_Code only exists on server!
		
	*/
	string GetCode()
	{
		if (!m_Code && !IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
				return codelock.GetCode();
		}

		return m_Code;
	}

	/**
	\brief Returning code length of item
		\param 	
	*/
	int GetCodeLength()
	{
		if (!m_CodeLength && !IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
				return codelock.GetCodeLength();
		}

		return m_CodeLength;
	}

	/**
	\brief Returning if the item has a code
		\param 	
		@note Remember that m_Code only exists on server! Use m_CodeLength > 0 to check for code on both server and client
	*/
	bool HasCode()
	{
		if (!m_CodeLength && !IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
				return codelock.HasCode();
		}

		return m_CodeLength > 0;
	}

	override bool ExpansionIsLocked()
	{
		if (!m_CodeLength && !IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
				return codelock.IsLocked();
		}

		return m_CodeLength > 0 && m_Locked;
	}

	override bool IsLocked()
	{
		return ExpansionIsLocked();
	}

	/**
	\brief Returning if the item has a codelock attached
		\param "selection" selection for codelock
	*/
	bool ExpansionHasCodeLock( string selection )
	{
		return ExpansionHasCodeLock();
	}

	/**
	\brief Returning if the item has a codelock attached
	*/
	bool ExpansionHasCodeLock()
	{
		return ExpansionGetCodeLock() != NULL;
	}

	bool ExpansionHasCodeLockSlot()
	{
		TStringArray attachments();
		TStringArray slots();
		string path;

		if (IsWeapon())
			path = "CfgWeapons";
		else if (IsMagazine())
			path = "CfgMagazines";
		else
			path = "CfgVehicles";

		GetGame().ConfigGetTextArray(path + " " + GetType() + " attachments", attachments);
		GetGame().ConfigGetTextArray("CfgVehicles ExpansionCodeLock inventorySlot", slots);

		foreach (string attachment: attachments)
		{
			attachment.ToLower();
			foreach (string slot: slots)
			{
				slot.ToLower();
				if (attachment == slot)
					return true;
			}
		}

		return false;
	}

	/**
	\brief Returning codelock entity
		\param
	*/
	ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(GetAttachmentByConfigTypeName("ExpansionCodeLock"));
	}

	void SetSlotLock( EntityAI parent, bool state )
	{
		InventoryLocation inventory_location = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( inventory_location );			
		parent.GetInventory().SetSlotLock( inventory_location.GetSlot(), state );
	}

	/**
	\brief Locking base build/safe
		\param 	
	*/
	void ExpansionLock()
	{
		if (!IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
			{
				codelock.ExpansionLock();
				return;
			}
		}

		if (m_Code)
		{
			m_Locked = true;

			if (GetHierarchyParent() && GetInventory().IsAttachment())
			{
				SetSlotLock( GetHierarchyParent(), true );
				SetTakeable( false );
			}
		}

		SetSynchDirty();
	}
	
	/**
	\brief Unlocking base build/safe
		\param 	
	*/
	void Unlock()
	{
		if (!IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
			{
				codelock.Unlock();
				return;
			}
		}

		m_Locked = false;

		if (GetHierarchyParent() && GetInventory().IsAttachment())
		{
			SetSlotLock( GetHierarchyParent(), false );
			SetTakeable( true );
		}

		SetSynchDirty();
	}
	
	/**
	\brief Failed attempt to unlock item
		\param 	
	*/
	void FailedUnlock()
	{
		SoundCodeLockFailedUnlock();
	}
	
	protected void SoundCodeLockFailedUnlock()
	{
		if ( !IsMissionClient() )
			return;

		string SOUND_CODE_DENIED = "";		

		if ( GetExpansionSettings().GetBaseBuilding().DoDamageWhenEnterWrongCodeLock )
		{
			SOUND_CODE_DENIED = "Expansion_Shocks_SoundSet";
		} else {
			SOUND_CODE_DENIED = "Expansion_Denied_SoundSet";
		}

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) // client side
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_CODE_DENIED, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}

	/**
	\brief Return if player is a known user of (attached) code lock or safe.
	
	NOTE: If IsKnownUser is called on the client instead of the server, initial calls will return false
	until the asynchronous request for known user data completes (unless the player had earlier set, changed,
	or entered the correct code while in the entity's network bubble).
	This is fine though, as IsKnownUser is primarily meant to be used by action conditions,
	and will function as intended in that context.

		\param 	player
	*/
	bool IsKnownUser( PlayerBase player )
	{
		if ( !player || !player.GetIdentity() || !GetExpansionSettings().GetBaseBuilding().RememberCode )
			return false;

		if (!IsInherited(ExpansionCodeLock) && !IsInherited(ExpansionSafeBase))
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if ( codelock )
				return codelock.IsKnownUser( player );
		}

		if (!m_KnownUIDs)
			return false;

		if ( GetGame().IsClient() && !m_KnownUIDsSet && !m_KnownUIDsRequested )
			RequestKnownUIDs();

		return m_KnownUIDs.Find( player.GetIdentityUID() ) > -1;
	}

	void AddUser( PlayerBase player )
	{
		if ( player && player.GetIdentity() && !IsKnownUser( player ) )
		{
			EXPrint("ItemBase::AddUser " + this + " (parent=" + GetHierarchyParent() + ") " + player.GetIdentityUID());
			m_KnownUIDs.Insert( player.GetIdentityUID() );
			SendKnownUIDs();
		}
	}

	void SetUser( PlayerBase player )
	{
		EXPrint("ItemBase::SetUser " + this + " (parent=" + GetHierarchyParent() + ")");
		m_KnownUIDs.Clear();
		AddUser( player );
	}

	//! Request known UIDs (players that know the code and have entered it correctly once) from server
	void RequestKnownUIDs()
	{
		EXPrint("ItemBase::RequestKnownUIDs " + this + " (parent=" + GetHierarchyParent() + ")");
		ScriptRPC rpc = new ScriptRPC;
		rpc.Send( this, ExpansionLockRPC.KNOWNUSERS_REQUEST, true, NULL );
		m_KnownUIDsRequested = true;
	}

	//! Send known UIDs (players that know the code and have entered it correctly once) to client
	void SendKnownUIDs()
	{
		EXPrint("ItemBase::SendKnownUIDs " + this + " (parent=" + GetHierarchyParent() + ")");
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( m_KnownUIDs );
		rpc.Send( this, ExpansionLockRPC.KNOWNUSERS_REPLY, true, NULL );
	}
	
	//============================================
	// SendServerLockReply
	//============================================	
	private void SendServerLockReply(bool reply, bool injuring, PlayerIdentity sender)
	{
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( reply );
		rpc.Write( injuring );
		rpc.Send( this, ExpansionLockRPC.SERVERREPLY, true, sender );
	}
	
	//============================================
	// OnRPC
	//============================================	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		//! Due to some weird dayz bug sender may sometimes not be null even when it could be so this check isn't really needed
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() && !sender )
			return;

		PlayerBase player;
		string playerId;
		string playerName;
		string playerDesc;

		if ( sender )
		{
			player = PlayerBase.GetPlayerByUID( sender.GetId() );

			if ( player )
			{
				playerId = player.GetIdentityUID();
				playerName = player.GetIdentityName();
				playerDesc = "player \"" + playerName + "\" (ID = \"" + playerId + "\" at " + player.GetPosition() + ")";
			}
		}
		
		string code = "";
		string selection = "";
		switch (rpc_type)
		{	
			case ExpansionLockRPC.LOCK:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.LOCK can't read selection");
					SendServerLockReply( false, false, sender );
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_SELECTION", "STR_EXPANSION_BB_CODE_CLOSE_LOCK")).Error(sender);
					return;
				}
				
				if ( !HasCode() || IsLocked() )
				{
					SendServerLockReply( false, false, sender );
					return;
				}

				CloseAndLock( selection );
				
				SendServerLockReply( true, false, sender );
				
				return;
			}
			
			case ExpansionLockRPC.UNLOCK:
			{
				if ( !IsMissionHost() || !GetExpansionSettings().GetBaseBuilding() )
					return;
				
				if ( !ctx.Read( code ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read code");
					SendServerLockReply( false, false, sender );
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_BADREAD", "STR_EXPANSION_BB_CODE_UNLOCK")).Error(sender);
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read selection");
					SendServerLockReply( false, false, sender );
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_SELECTION", "STR_EXPANSION_BB_CODE_UNLOCK")).Error(sender);
					return;
				}
				
				if ( !HasCode() || !IsLocked() )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ItemBase::OnRPC ExpansionLockRPC.UNLOCK !HasCode() || !IsLocked()");
					#endif
					
					SendServerLockReply( false, false, sender );
					return;
				}

				if ( !IsKnownUser( player ) && GetCode() != code )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ItemBase::OnRPC ExpansionLockRPC.UNLOCK GetCode() != code");
					#endif
					
					
					bool InjuryPlayer = GetExpansionSettings().GetBaseBuilding().DoDamageWhenEnterWrongCodeLock;

					SendServerLockReply( false, InjuryPlayer, sender );

					if ( player )
					{
						if ( InjuryPlayer )
						{
							FailedUnlock();

							if ( GetExpansionSettings().GetLog().CodeLockRaiding )
								GetExpansionSettings().GetLog().PrintLog( "ExpansionCodelock (" + GetPosition() + ") Damaged " + playerDesc + " by " + GetExpansionSettings().GetBaseBuilding().DamageWhenEnterWrongCodeLock + " health points. Reason: Failed to enter the correct code." );

							//! Vanilla EnviroDmg is 1 0 1 (health blood shock)
							player.ProcessDirectDamage( DT_CUSTOM, player, "", "EnviroDmg", "0.5 0.5 0.5", GetExpansionSettings().GetBaseBuilding().DamageWhenEnterWrongCodeLock );
						} else {
							if ( GetExpansionSettings().GetLog().CodeLockRaiding )
								GetExpansionSettings().GetLog().PrintLog( "ExpansionCodelock (" + GetPosition() + ") " + playerDesc + " failed to enter the correct code." );
						}
						if ( GetExpansionSettings().GetLog().CodeLockRaiding )
							GetExpansionSettings().GetLog().PrintLog( "ExpansionCodelock (" + GetPosition() + ") The correct code was " + GetCode() + " and the player tried " + code );
					}

					return;
				}

				if ( GetExpansionSettings().GetBaseBuilding().RememberCode )
				{
					ExpansionCodeLock codelock = ExpansionGetCodeLock();
					if ( codelock )
						codelock.AddUser( player );
					else if ( IsInherited( ExpansionSafeBase ) )
						AddUser( player );
				}

				Unlock();
				SendServerLockReply( true, false, sender );
				return;
			}
			
			case ExpansionLockRPC.SET:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != GetExpansionSettings().GetBaseBuilding().CodeLockLength )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read code");
					SendServerLockReply( false, false, sender );
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_BADREAD", "STR_EXPANSION_BB_CODE_SET")).Error(sender);
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read selection");
					SendServerLockReply( false, false, sender );
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_SELECTION", "STR_EXPANSION_BB_CODE_SET")).Error(sender);
					return;
				}
				
				if ( HasCode() )
				{
					// Base already has code so don't try setting it to another.
					SendServerLockReply( false, false, sender );
					return;
				}

				SetCode( code, player );

				if ( GetExpansionSettings().GetLog().CodeLockRaiding )
					GetExpansionSettings().GetLog().PrintLog( "ExpansionCodelock ("+ GetPosition() + ") Code set by " + playerDesc + " and the code is "+ code );

				SendServerLockReply( true, false, sender );
				return;
			}

			case ExpansionLockRPC.CHANGE:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != GetExpansionSettings().GetBaseBuilding().CodeLockLength )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read code");
					SendServerLockReply( false, false, sender );
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_BADREAD", "STR_EXPANSION_BB_CODE_LOCK_CHANGE")).Error(sender);
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read selection");
					SendServerLockReply( false, false, sender );
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_ERROR_DESC_CODE_SELECTION", "STR_EXPANSION_BB_CODE_LOCK_CHANGE")).Error(sender);
					return;
				}
				
				SetCode( code, player );

				if ( GetExpansionSettings().GetLog().CodeLockRaiding )
					GetExpansionSettings().GetLog().PrintLog( "ExpansionCodelock ("+ GetPosition() + ") Code changed by " + playerDesc + " and the new code is "+ code );

				SendServerLockReply( true, false, sender );
				return;
			}
			
			case ExpansionLockRPC.SERVERREPLY:
			{
				if ( !IsMissionClient() )
					return;
				
				bool reply;
				if ( !ctx.Read( reply ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read reply");
					return;
				}

				bool injuring;
				if ( !ctx.Read( injuring ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read injuring");
					return;
				}

				if ( injuring )
					FailedUnlock();

				ExpansionLockUIBase menu;
				if ( !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_CODELOCK_MENU ) ) && !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_NUMPAD_MENU ) ) )
					return;
					
				menu.OnServerResponse( reply, injuring );
				return;
			}
			
			case ExpansionLockRPC.KNOWNUSERS_REQUEST:
			{
				SendKnownUIDs();
				return;
			}
			
			case ExpansionLockRPC.KNOWNUSERS_REPLY:
			{
				if ( !ctx.Read( m_KnownUIDs ) )
				{
					Error("ItemBase::OnRPC " + this + " ExpansionLockRPC.KNOWNUSERS_REPLY can't read reply");
					return;
				}
				EXPrint("ItemBase::OnRPC " + this + " (parent=" + GetHierarchyParent() + ") - received m_KnownUIDs");
				#ifdef EXPANSIONEXPRINT
				for ( int i = 0; i < m_KnownUIDs.Count(); i++ )
				{
					EXPrint(m_KnownUIDs[i]);
				}
				#endif
				m_KnownUIDsSet = true;
				return;
			}
		}
	}
	
	//============================================
	// OnStoreSave
	//============================================
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - Start");
		#endif

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("ItemBase::OnStoreSave " + this + " " + GetGame().SaveVersion());
		#endif

		super.OnStoreSave( ctx );

		//! If we are saving game version target for ModStorage support (1st stable) or later
		#ifdef CF_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
			return;
		#endif

		m_ElectricitySource.OnStoreSave_OLD( ctx );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - End");
		#endif
	}
		
	//============================================
	// OnStoreLoad
	//============================================
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		//! With CF_ModStorage enabled, the code below won't be ran unless an old CE is loaded. To prevent server wipes, the code below will stay.

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreLoad - Start");
		#endif

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("ItemBase::OnStoreLoad " + this + " " + version);
		#endif

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( GetExpansionSaveVersion() < 22 )
		{
			string currentSkinName = m_CurrentSkinName;

			if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
				return false;

			if ( m_CurrentSkinName == "" )
				m_CurrentSkinName = currentSkinName;
		}

		if (GetExpansionSaveVersion() < 32)
		{
			bool isAttached;
			if ( Expansion_Assert_False( ctx.Read( isAttached ), "[" + this + "] Failed reading isAttached" ) )
				return false;
		}

		if ( GetExpansionSaveVersion() < 8 )
		{
			m_ElectricitySource.Setup();
		} else
		{
			m_ElectricitySource.OnStoreLoad_OLD( ctx, version, GetExpansionSaveVersion() );
		}

		//SetSynchDirty();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreLoad - End");
		#endif

		return true;
	}

	#ifdef CF_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return;

		m_ElectricitySource.OnStoreSave(ctx);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return true;

		if (!m_ElectricitySource.OnStoreLoad(ctx))
			return false;

		return true;
	}
	#endif
	
	//============================================
	// EEOnAfterLoad
	//============================================
	override void EEOnAfterLoad()
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - Start");
		#endif

		super.EEOnAfterLoad();

		m_ElectricitySource.OnAfterLoad();

		UpdateCurrentHealthMap( "ItemBase::EEOnAfterLoad" );
		
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - End");
		#endif
	}
	
	//============================================
	// SkinMessage
	//============================================
	protected void SkinMessage( string message )
	{
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );
		} else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( message );
			#endif
		}	
	}

	//============================================
	// GetHiddenSelectionIndex
	//============================================
	override int GetHiddenSelectionIndex( string selection )
	{
		array<string> config_selections	= new array<string>;
		ConfigGetTextArray( "hiddenSelections", config_selections );
		
		for ( int i = 0; i < config_selections.Count(); ++i )
		{
			if ( config_selections.Get( i ) == selection )
			{
				return i;
			}
		}
		
		return -1;
	}

	//============================================
	// OnVariablesSynchronized
	//============================================
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();

		m_ElectricitySource.OnVariablesSynchronized();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::OnVariablesSynchronized - End");
		#endif
	}
	
	//============================================
	// Explode
	//============================================
 	override void Explode(int damageType, string ammoType = "")
	{
		super.Explode( damageType, ammoType );

		BuildingBase.ExpansionExplode( this, ammoType );
	}

	//============================================
	// EEHealthLevelChanged
	//============================================
	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::EEHealthLevelChanged Start");
		#endif

		if ( IsMissionHost() )
			UpdateCurrentHealthMap( "ItemBase::EEHealthLevelChanged" );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::EEHealthLevelChanged End");
		#endif
	}
	
	/**
	 * @param damageResult 
	 * @param source 
	 * @param component 
	 * @param dmgZone 
	 * @param ammo 
	 * @param modelPos 
	 * @param speedCoef 
	 *  
	 * This override only exists to either increase or negate (partly or fully) the base damage by applying any respective damage multipliers,
	 * and logs the result to the admin log.
	 * 
	 * TODO: Better linking to the vanilla damage system but still keep damage multipliers
	 */
	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef )
	{
		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef );

		if ( !IsInherited( ExpansionBaseBuilding ) && !IsInherited( ExpansionSafeBase ) )
			return;
		string damageZone = dmgZone;
		if ( damageZone == "" )
			damageZone = "GlobalHealth";

		//Print(GetType() + " ItemBase::EEHitby m_CurentHealth " + m_CurrentHealth[damageZone] + " health " + GetHealth(damageZone, "Health"));

		float health = m_CurrentHealth[damageZone];  // Health before damage
		float dmg = damageResult.GetDamage( damageZone, "Health" );  // Base damage
		float damageMultiplier;

		if ( damageType == DT_EXPLOSION || damageType == DT_FIRE_ARM )
		{
			if ( IsInherited( ExpansionSafeBase ) )
			{
				if ( damageType == DT_EXPLOSION )
				{
					damageMultiplier = GetExpansionSettings().GetRaid().SafeExplosionDamageMultiplier;
				} else
				{
					damageMultiplier = GetExpansionSettings().GetRaid().SafeProjectileDamageMultiplier;
				}
			} else
			{
				if ( damageType == DT_EXPLOSION )
				{
					damageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
				} else
				{
					damageMultiplier = GetExpansionSettings().GetRaid().ProjectileDamageMultiplier;
				}
			}

			// damageMultiplier > 1 applies bonus damage
			// damageMultiplier < 1 negates damage (partly if multiplier > 0 or fully if 0)
			// damageMultiplier == 1 effectively does nothing
			if ( health > 0 )
				SetHealth( damageZone, "Health", Math.Max( health - ( dmg * damageMultiplier ), 0 ) );
		}

		m_CurrentHealth[damageZone] = GetHealth( damageZone, "Health" );

		RaidLog( source, damageZone, health, dmg, damageMultiplier );
	}

	void RaidLog( EntityAI source, string damageZone, float health, float dmg, float damageMultiplier )
	{
		PlayerBase player;
		string playerId;
		string playerName;
		string playerDesc;

		if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
		{
			playerId = player.GetIdentityUID();
			playerName = player.GetIdentityName();

			playerDesc = "Player \"" + playerName + "\" (ID = \"" + playerId + "\" at " + player.GetPosition() + ")";
		} else
		{
			playerDesc = "A player";
		}

		if ( GetExpansionSettings().GetLog().BaseBuildingRaiding )
		{
			if ( ( dmg * damageMultiplier ) != 0 )
			{
				GetExpansionSettings().GetLog().PrintLog( "------------------------- Expansion BaseRaiding Damage Report -------------------------" );
				GetExpansionSettings().GetLog().PrintLog( "BaseRaiding: " + playerDesc + " damaged a base part (" + GetType() + ") (" + health + " current health)" );
				GetExpansionSettings().GetLog().PrintLog( "BaseRaiding: They dealt "  + dmg + " * " + damageMultiplier + " = " + ( dmg * damageMultiplier ) + " damage with a " + source.GetType() + " at " + GetPosition() );
				GetExpansionSettings().GetLog().PrintLog( "Expansion BaseRaiding: Health after damage applied: " + GetHealth( damageZone, "Health" ) );
				GetExpansionSettings().GetLog().PrintLog( "---------------------------------------------------------------------------------------" );
			}
		}
	}

	void UpdateLaser()
	{
		
	}

	void ExpansionDropServer( PlayerBase player )
	{
		if ( !IsMissionHost() )
			return;

		EntityAI parent = GetHierarchyParent();

		if ( !parent )
			return;

		InventoryLocation inventory_location = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( inventory_location );

		if ( inventory_location.IsValid() )
			parent.GetInventory().SetSlotLock( inventory_location.GetSlot(), false );

		if ( player )
			player.ServerDropEntity( this );
		else
			parent.GetInventory().DropEntity( InventoryMode.SERVER, parent, this );

		BaseBuildingBase base_building = BaseBuildingBase.Cast( parent );
		if ( base_building )
			SetPosition( base_building.GetKitSpawnPosition() );
		else
			SetPosition( parent.GetPosition() );
		PlaceOnSurface();

		SetTakeable( true );
		SetSynchDirty();
	}
};
