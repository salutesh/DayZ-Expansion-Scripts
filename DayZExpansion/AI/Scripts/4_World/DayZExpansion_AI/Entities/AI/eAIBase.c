/**
 * eAIBase.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum eAIStance
{
	UNKNOWN = -1,
	ERECT,
	CROUCH,
	PRONE
}

class eAIBase: PlayerBase
{
	const int EAI_COMMANDID_MOVE = 1;

	static bool AI_HANDLEDOORS = true;
	static bool AI_HANDLEVAULTING = true;

	private static autoptr array<eAIBase> s_AllAI = new array<eAIBase>();

	protected autoptr eAIFSM m_FSM;
	bool m_eAI_IsFightingFSM;

	// Targeting data
	private autoptr array<ref eAITarget> m_eAI_Targets;
	private autoptr map<eAITargetInformation, ref eAITargetInformationState> m_eAI_TargetInformationStates;
	float m_ThreatClearedTimeout;
	float m_eAI_CurrentThreatToSelf;
	float m_eAI_PreviousThreatToSelf;
	float m_eAI_CurrentThreatToSelfActive;
	float m_eAI_PreviousThreatToSelfActive;
	float m_eAI_AccuracyMin;
	float m_eAI_AccuracyMax;
	float m_eAI_ThreatDistanceLimit;
	float m_eAI_DamageMultiplier;
	bool m_eAI_SyncCurrentTarget;

	// Command handling
	private ExpansionHumanCommandScript m_eAI_Command;
	private int m_eAI_CurrentCommandID;
	private float m_eAI_CommandTime;

	private bool m_eAI_UnconsciousVehicle;

	private Transport m_eAI_Transport;
	private int m_eAI_Transport_SeatIndex;

	private bool m_eAI_Melee;
	private bool m_eAI_MeleeDidHit;
	private int m_eAI_MeleeTime;

	private ref eAIAimingProfile m_AimingProfile;

	private ref eAIActionManager m_eActionManager;
	private ref eAIMeleeCombat m_eMeleeCombat;

	private vector m_eAI_LastMovementCheckTargetPosition;
	private vector m_eAI_LastMovementCheckUnitPosition;
	private float m_eAI_MovementCheckTimeout;

	// Position for aiming/looking in the world
	private vector m_eAI_LookPosition_WorldSpace;
	private vector m_eAI_AimPosition_WorldSpace;

	// A direction vector (not YPR) in Model Space, not World Space
	private vector m_eAI_LookRelAngles;
	private vector m_eAI_LookDirectionTarget_ModelSpace;
	private bool m_eAI_LookDirection_Recalculate;

	private vector m_eAI_AimRelAngles;
	protected float m_eAI_AimRelAngleLR;
	protected float m_eAI_AimRelAngleUD;
	private vector m_eAI_AimDirectionTarget_ModelSpace;
	private bool m_eAI_AimDirection_Recalculate;

	private bool m_MovementSpeedActive;
	private int m_MovementSpeed;
	private int m_MovementSpeedLimit = 3;
	private int m_MovementSpeedLimitUnderThreat = 3;
	private bool m_MovementDirectionActive;
	private float m_MovementDirection;
	private float m_SideStepAngle;
	private bool m_eAI_TurnTargetActive;
	private float m_eAI_TurnTarget;

	float m_eAI_FormationPositionUpdateTime;
	vector m_eAI_FormationPosition;

	private bool m_WeaponRaised;
	private bool m_WeaponRaisedPrev;

	private float m_WeaponRaisedTimer;
	private float m_WeaponLowerTimeout;

	ref array<ItemBase> m_Weapons = {};
	ref array<ItemBase> m_MeleeWeapons = {};
	ref array<ItemBase> m_Bandages = {};

	ref map<typename, Magazine> m_eAI_EvaluatedFirearmTypes = new map<typename, Magazine>;

	private float m_eAI_SideStepTimeout;

	private bool m_eAI_UnlimitedReload;
	private float m_eAI_SniperProneDistanceThreshold;

	// Path Finding
#ifndef EAI_USE_LEGACY_PATHFINDING
	/*private*/ ref ExpansionPathHandler m_PathFinding;
#else
	/*private*/ ref eAIPathFinding m_PathFinding;
#endif
	bool m_eAI_TargetPositionIsFinal;
	bool m_eAI_PositionIsFinal;

	private Apple m_DebugTargetApple;
	private vector m_DebugTargetOrientation;

	float m_eAI_UpdateTargetsTick;
	autoptr array<EntityAI> m_eAI_PotentialTargetEntities = new array<EntityAI>();
	int m_eAI_CurrentPotentialTargetIndex;
	CF_DoublyLinkedNode_WeakRef<PlayerBase> m_eAI_PotentialTargetPlayer;
	float m_eAI_UpdateNearTargetsTime;
	int m_eAI_UpdateNearTargetsCount;

	int m_Expansion_EmoteID;
	bool m_Expansion_EmoteAutoCancel;
	int m_Expansion_EmoteAutoCancelDelay; //! ms

	int m_eAI_MinTimeTillNextFire;
	int m_eAI_QueuedShots;

	private float m_Expansion_UpdateTime;
	private float m_Expansion_UpdateTimeThreshold = 5.0;

	float m_Expansion_DaylightVisibility = -1;
	float m_Expansion_Visibility = 0.1;
	float m_Expansion_VisibilityDistThreshold = 90.0;
	ref array<ItemBase> m_Expansion_ActiveVisibilityEnhancers = {};
	bool m_Expansion_TriedTurningOnVisibilityEnhancers;

	ref set<Man> m_eAI_InteractingPlayers = new set<Man>;

	bool m_eAI_DespawnOnLoosingAggro;

	ref map<ItemBase, bool> m_eAI_ItemThreatOverride = new map<ItemBase, bool>;

	ref Timer m_eAI_ClientUpdateTimer;

	void eAIBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIBase");
#endif

#ifdef CF_DEBUG
		CF_Debug.Create(this);
#endif

		s_AllAI.Insert(this);

		if (GetGame().IsServer())
			AI_HANDLEVAULTING = GetExpansionSettings().GetAI().Vaulting;

		SetEventMask(EntityEvent.INIT);

		if (GetGame().IsClient())
		{
			m_eAI_ClientUpdateTimer = new Timer(CALL_CATEGORY_SYSTEM);
			m_eAI_ClientUpdateTimer.Run(1.0 / 30.0, this, "eAI_ClientUpdate", NULL, true);
		}
	}

	static eAIBase Get(int index)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIBase", "Get").Add(index);
#endif
		
		if (index >= s_AllAI.Count())
		{
			return null;
		}

		return s_AllAI[index];
	}

	override void Init()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Init");
#endif

		super.Init();

		m_eAI_Targets = new array<ref eAITarget>();
		m_eAI_TargetInformationStates = new map<eAITargetInformation, ref eAITargetInformationState>();

		m_AimingProfile = new eAIAimingProfile(this);

		m_eMeleeCombat = new eAIMeleeCombat(this);

		m_MeleeCombat = m_eMeleeCombat;
		m_MeleeFightLogic = new DayZPlayerMeleeFightLogic_LightHeavy(this);

		m_WeaponManager = new eAIWeaponManager(this);
		m_ShockHandler = new eAIShockHandler(this);

		if (GetGame().IsServer())
		{
			eAI_SetAccuracy(-1, -1);
			eAI_SetThreatDistanceLimit(-1);
			eAI_SetDamageMultiplier(-1);
			m_eAI_SniperProneDistanceThreshold = GetExpansionSettings().GetAI().SniperProneDistanceThreshold;

#ifndef EAI_USE_LEGACY_PATHFINDING
			m_PathFinding = new ExpansionPathHandler(this);
#else
			m_PathFinding = new eAIPathFinding(this);
#endif

			LoadFSM();

			s_Expansion_AllPlayers.m_OnRemove.Insert(eAI_OnRemovePlayer);
		}

		LookAtDirection("0 0 1");
		AimAtDirection("0 0 1");

		//! Vehicles mod will set this in PlayerBase::Init if loaded
		if (!m_ExpansionST)
			m_ExpansionST = new ExpansionHumanST(this);
	}

	override void Expansion_Init()
	{
		if (GetGame().IsServer() && !m_eAI_FactionType)
		{
			m_eAI_FactionType = eAIFactionRaiders;
		}

		super.Expansion_Init();

		RegisterNetSyncVariableBool("m_Expansion_CanBeLooted");
		RegisterNetSyncVariableFloat("m_eAI_AccuracyMin");
		RegisterNetSyncVariableFloat("m_eAI_AccuracyMax");

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		eAI_Cleanup();
	}

	static void ReloadAllFSM()
	{
		array<eAIBase> ai();
		foreach (auto ai0 : s_AllAI)
		{
			if (!ai0.GetFSM())
			{
				continue;
			}

			ai0.m_FSM = null;

			ai.Insert(ai0);
		}

		ExpansionFSMType.UnloadAll();

		foreach (auto ai1 : ai)
		{
			ai1.LoadFSM();
		}
	}

	void LoadFSM()
	{
		ExpansionFSMType type = ExpansionFSMType.LoadXML("DayZExpansion/AI/scripts/FSM", "Master");
		ExpansionFSMOwnerType owner = this;
		if (type && Class.CastTo(m_FSM, type.Spawn(owner, null)))
		{
			m_FSM.StartDefault();
		}
		else
		{
			CF_Log.Error("Invalid FSM");
		}
	}

	void ~eAIBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "~eAIBase");
#endif

		if (!GetGame())
			return;

#ifdef CF_DEBUG
		CF_Debug.Destroy(this);
#endif

		s_AllAI.RemoveItem(this);
	}

	protected override void EOnInit(IEntity other, int extra)
	{
		EXTrace.Print(EXTrace.AI, this, "EOnInit");

		OnSelectPlayer();
	}

	override void OnSelectPlayer()
	{
		EXTrace.Print(EXTrace.AI, this, "OnSelectPlayer");

		m_QuickBarBase.updateSlotsCount();

		m_PlayerSelected = true;

		m_WeaponManager.SortMagazineAfterLoad();

		if (GetGame().IsServer())
		{
			//! add callbacks for ai target system
			SetAITargetCallbacks(new AITargetCallbacksPlayer(this));

			GetSoftSkillsManager().InitSpecialty(GetStatSpecialty().Get());
			GetModifiersManager().SetModifiers(true);

			SetSynchDirty();
		}

		CheckForGag();

		m_eActionManager = new eAIActionManager(this);
		m_ActionManager = m_eActionManager;
	}

#ifdef DIAG
	ref map<EntityAI, bool> m_eAI_PlayerIsEnemyStatus = new map<EntityAI, bool>;
	void eAI_UpdatePlayerIsEnemyStatus(EntityAI player, bool state, string reason)
	{
		bool currentState;
		if (!m_eAI_PlayerIsEnemyStatus.Find(player, currentState) || state != currentState)
		{
			EXPrint(this, "::eAI_UpdatePlayerIsEnemyStatus " + GetGroup().GetFaction() + " | " + reason + " | " + player.ToString() + " | is enemy? " + state);
			m_eAI_PlayerIsEnemyStatus[player] = state;
		}
	}
#endif

	bool PlayerIsEnemy(EntityAI other)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "PlayerIsEnemy").Add(other);
#endif

		PlayerBase player;
		if (!Class.CastTo(player, other))
		{
#ifdef DIAG
			eAI_UpdatePlayerIsEnemyStatus(player, true, "target not a player");
#endif
			return true;
		}

		if (player.Expansion_HasAdminToolInvisibility())
		{
#ifdef DIAG
			eAI_UpdatePlayerIsEnemyStatus(player, false, "target invisible");
#endif
			return false;
		}

		if (GetGroup().GetFaction().IsObserver())
		{
			//! Actual player are always "enemies" to observers (will be looked at)
			if (!player.IsAI())
				return true;

			//! Don't look at other AI until they move
			return player.Expansion_GetMovementSpeed() > 0 || player.IsClimbing() || player.IsFalling() || player.IsFighting();
		}

		//! Are we targeting them?
		bool targeted = player.GetTargetInformation().IsTargetted(GetGroup());

		if (player.GetGroup())
		{
			if (player.GetGroup() == GetGroup())
			{
#ifdef DIAG
				eAI_UpdatePlayerIsEnemyStatus(player, false, "target has same group");
#endif
				return false;
			}

			//! If we are not targeting them, are they targeting us?
			if (!targeted && !player.GetGroup().GetFaction().IsObserver())
				targeted = GetTargetInformation().IsTargetted(player.GetGroup());

			//! Other faction friendly to our faction or this specific AI?
			if (player.GetGroup().GetFaction().IsFriendly(GetGroup().GetFaction()) || player.GetGroup().GetFaction().IsFriendly(this))
			{
#ifdef DIAG
				if (eAI_IsPassive())
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "passive | target is friendly " + player.GetGroup().GetFaction());
				else
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "target is friendly " + player.GetGroup().GetFaction());
#endif

				return targeted;
			}

			//! If the other player is a guard and we are not, consider them an enemy if they raised their weapon and
			//! either aren't AI or consider us a threat
			if (player.GetGroup().GetFaction().IsGuard())
			{
				eAIBase ai;
				bool hostile;
				//! https://feedback.bistudio.com/T173348
				if ((player.IsRaised() || player.IsFighting()) && (!Class.CastTo(ai, player) || ai.eAI_GetTargetThreat(GetTargetInformation()) >= 0.4))
					hostile = true;
#ifdef DIAG
				eAI_UpdatePlayerIsEnemyStatus(player, hostile, "target is guard");
#endif
				return hostile;
			}
		}

		//! Our faction fiendly to specific player?
		if (GetGroup().GetFaction().IsFriendly(other))
		{
#ifdef DIAG
			if (eAI_IsPassive())
				eAI_UpdatePlayerIsEnemyStatus(player, targeted, "passive | friendly | target has no group");
			else
				eAI_UpdatePlayerIsEnemyStatus(player, targeted, "friendly");
#endif

			return targeted;
		}

#ifdef EXPANSIONMODGROUPS
		if (GetGame().IsServer())
		{
			PlayerBase leader;
			PlayerBase otherLeader;
			if (player.GetGroup())
				Class.CastTo(otherLeader, player.GetGroup().GetLeader());
			else
				otherLeader = player;
			if (otherLeader && otherLeader.Expansion_GetParty() && Class.CastTo(leader, GetGroup().GetLeader()) && leader.Expansion_GetParty())
			{
				//! If leaders of both AI groups are actual players or other player has no AI group,
				//! check if they are in the same Expansion party, and if so,
				//! AI will not be hostile to the other group/player
				if (leader.Expansion_GetParty() == otherLeader.Expansion_GetParty())
				{
#ifdef DIAG
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "target party is leader party");
#endif
					return targeted;
				}
			}
		}
#endif

		// at this point we know both we and they have groups, and the groups aren't friendly towards each other
#ifdef DIAG
		eAI_UpdatePlayerIsEnemyStatus(player, true, "not friendly");
#endif
		return true;
	}

	void TryFireWeapon()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "TryFireWeapon");
#endif

		if (GetGame().GetTime() < m_eAI_MinTimeTillNextFire)
			return;

		Weapon_Base weapon = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
		if (!weapon)
			return;

		if (!eAI_CanFire(weapon))
			return;

		if (!eAI_HasLOS())
			return;

		if (m_eAI_QueuedShots > 0)
		{
			//! Continue firing in current mode until all queued shots fired
		}
		else if (Math.RandomFloat(0.0, 1.0) > 0.40)
		{
			//! Determine firing mode

			float distSq = GetTarget().GetDistanceSq(this, true);

			bool burst;
			bool fullAuto;

			if (distSq < 200)
			{
				if (Math.RandomFloat(0.0, 1.0) > 0.60 || !weapon.Expansion_SetFireMode(ExpansionFireMode.FullAuto))
					burst = weapon.Expansion_SetFireMode(ExpansionFireMode.Burst);
				else
					fullAuto = true;
			}
			else if (distSq < 1500)
			{
				burst = weapon.Expansion_SetFireMode(ExpansionFireMode.Burst);
			}

			if (burst)
			{
				weapon.ResetBurstCount();
				m_eAI_QueuedShots = weapon.GetCurrentModeBurstSize(weapon.GetCurrentMuzzle());
			}
			else if (fullAuto)
			{
				Magazine mag;
				if (Class.CastTo(mag, weapon.GetMagazine(weapon.GetCurrentMuzzle())))
					m_eAI_QueuedShots = Math.Ceil(((200 - distSq) / 200) * Math.Min(mag.GetAmmoMax() / 4, mag.GetAmmoCount()));
			}
			else
			{
				weapon.Expansion_SetFireMode(ExpansionFireMode.SemiAuto);
			}
		}
		else
		{
			weapon.Expansion_SetFireMode(ExpansionFireMode.SemiAuto);
		}

	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "Firing, queued shots: " + m_eAI_QueuedShots);
	#endif

		GetWeaponManager().Fire(weapon);

		if (m_eAI_QueuedShots > 0)
			m_eAI_QueuedShots--;

		if (m_eAI_QueuedShots > 0)
			m_eAI_MinTimeTillNextFire = GetGame().GetTime() + weapon.GetReloadTime(weapon.GetCurrentMuzzle()) * 1000;
		else
			m_eAI_MinTimeTillNextFire = GetGame().GetTime() + Math.RandomIntInclusive(200, 300);
	}

	bool eAI_CanFire(Weapon_Base weapon)
	{
		if (IsClimbing() || IsFalling() || IsFighting())
			return false;

		if (GetDayZPlayerInventory().IsProcessing())
			return false;

		if (!IsRaised())
			return false;

		if (!weapon.CanFire())
			return false;

		if (GetWeaponManager().IsRunning())
			return false;

		int muzzleIndex = weapon.GetCurrentMuzzle();
		if (!weapon.CanFire(muzzleIndex))
			return false;

		return true;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetGroup())
			return;

	#ifdef DIAG
		EXTrace.PrintHit(EXTrace.AI, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	#endif

		ZombieBase zmb;
		if (Class.CastTo(zmb, source))
		{
			if (!zmb.GetTargetInformation().IsTargettedBy(this))
			{
				zmb.GetTargetInformation().AddAI(this);
			}

			return;
		}

		PlayerBase player;
		if (Class.CastTo(player, source.GetHierarchyRootPlayer()))
		{
			//! If attacking player is AI, check if we are their current target, else it was accidental friendly fire
			eAIBase ai;
			if (!Class.CastTo(ai, player) || (ai.GetTarget() && ai.GetTarget().GetEntity() == this))
			{
				//! Target the attacking player for up to 2 minutes
				if (!player.GetTargetInformation().IsTargettedBy(this))
				{
					player.GetTargetInformation().AddAI(this, 120000);
				}
				else
				{
					//! Update target found at time if already targeting
					player.GetTargetInformation().Update(GetGroup());
				}
			}

			return;
		}

		AnimalBase animal;
		if (Class.CastTo(animal, source))
		{
			if (!animal.GetTargetInformation().IsTargettedBy(this))
			{
				animal.GetTargetInformation().AddAI(this);
			}

			return;
		}

		//! If AI looses 5.55555% of current damage zone health or more by vehicle hit, add vehicle as target
		CarScript vehicle;
		if (Class.CastTo(vehicle, source) && damageResult.GetDamage(dmgZone, "Health") >= GetHealth(dmgZone, "Health") * 0.055555)
		{
			if (!vehicle.GetTargetInformation().IsTargettedBy(this))
			{
				vehicle.GetTargetInformation().AddAI(this);
			}
		}
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		eAI_Cleanup();

		if (GetGame().IsServer())
		{
			if (!m_Expansion_EmoteAutoCancel)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_PlayEmote);
		}
	}

	void eAI_Cleanup()
	{
		if (GetGroup() && GetGroup().Count() > 1)
		{
			GetGroup().RemoveMember(this);
		}

		if (GetGame().IsServer() && !IsDamageDestroyed())
			s_Expansion_AllPlayers.m_OnRemove.Remove(eAI_OnRemovePlayer);

		if (m_eAI_ClientUpdateTimer && m_eAI_ClientUpdateTimer.IsRunning())
			m_eAI_ClientUpdateTimer.Stop();
	}

	override bool IsAI()
	{
		return true;
	}

#ifndef EAI_USE_LEGACY_PATHFINDING
	ExpansionPathHandler GetPathFinding()
#else
	eAIPathFinding GetPathFinding()
#endif
	{
		return m_PathFinding;
	}

	eAIFSM GetFSM()
	{
		return m_FSM;
	}

	ItemBase GetBandageToUse()
	{
		foreach (ItemBase bandage: m_Bandages)
		{
			if (bandage && !bandage.IsDamageDestroyed())
			{
				return bandage;
			}
		}

		return null;
	}

	ItemBase GetWeaponToUse(bool requireAmmo = false)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(EXTrace.AI, this).Add(requireAmmo);
#endif

		// very messy :)
		Weapon_Base gun;
		Magazine mag;
		foreach (ItemBase weapon: m_Weapons)
		{
			if (weapon && !weapon.IsDamageDestroyed())
			{
				if (!requireAmmo)
					return weapon;

				if (!Class.CastTo(gun, weapon))
					continue;

				if (eAI_HasAmmoForFirearm(gun, mag, false))
					return weapon;
			}
		}

		return null;
	}

	ItemBase GetMeleeWeaponToUse()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetMeleeWeaponToUse");
#endif

		// very messy :)
		foreach (ItemBase melee: m_MeleeWeapons)
		{
			if (melee && !melee.IsDamageDestroyed())
			{
				return melee;
			}
		}

		return null;
	}

	//! Unlike GetMagazineToReload, this can be used to check if there is a mag/ammo for a weapon that is not in hands
	EntityAI GetMagazineInInventory(EntityAI weapon)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase, weapon.ToString());

		Weapon_Base weapon_base = Weapon_Base.Cast(weapon);
		eAIWeaponManager weapon_manager = eAIWeaponManager.Cast(GetWeaponManager());

		EntityAI ammo_magazine; // magazine
		int last_ammo_magazine_count;
		int ammo_magazine_count;

		EntityAI ammo_pile; // ammo pile
		int last_ammo_pile_count;
		int ammo_pile_count;

		// Get all magazines in (player) inventory
		for (int att_i = 0; att_i < GetInventory().AttachmentCount(); ++att_i)
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(att_i);
			CargoBase attachment_cargo = attachment.GetInventory().GetCargo();

			if (attachment_cargo)
			{
				for (int cgo_i = 0; cgo_i < attachment_cargo.GetItemCount(); ++cgo_i)
				{
					EntityAI cargo_item = attachment_cargo.GetItem(cgo_i);

					// check for proper magazine
					if (cargo_item.IsMagazine())
					{
						Magazine magazine = Magazine.Cast(cargo_item);
						ammo_pile_count = magazine.GetAmmoCount();

						// magazines (get magazine with max ammo count)
						if (weapon_manager.CanAttachMagazine_NoHandsCheck(weapon_base, magazine) || weapon_manager.CanSwapMagazine_NoHandsCheck(weapon_base, magazine))
						{
							if (ammo_pile_count > 0 || m_eAI_UnlimitedReload)
							{
								if (last_ammo_magazine_count == 0)
								{
									ammo_magazine = magazine;
									last_ammo_magazine_count = ammo_pile_count;
								}
								else
								{
									if (last_ammo_magazine_count < ammo_pile_count)
									{
										ammo_magazine = magazine;
										last_ammo_magazine_count = ammo_pile_count;
									}
								}
								if (m_eAI_UnlimitedReload)
								{
									break;
								}
							}
						}
						// bullets (get ammo pile with min ammo count)
						else if (weapon_manager.CanLoadBullet_NoHandsCheck(weapon_base, magazine))
						{
							if (ammo_pile_count > 0 || m_eAI_UnlimitedReload)
							{
								if (last_ammo_pile_count == 0)
								{
									ammo_pile = magazine;
									last_ammo_pile_count = ammo_pile_count;
								}
								else
								{
									if (last_ammo_pile_count > ammo_pile_count)
									{
										ammo_pile = magazine;
										last_ammo_pile_count = ammo_pile_count;
									}
								}
								if (m_eAI_UnlimitedReload)
								{
									break;
								}
							}
						}
					}
				}
				if (magazine && m_eAI_UnlimitedReload)
				{
					magazine.ServerSetAmmoMax();
					break;
				}
			}
		}

#ifdef DIAG
		if (ammo_magazine)
			EXTrace.Print(EXTrace.AI, this, "GetMagazineInInventory " + ammo_magazine + " ammo count " + last_ammo_magazine_count);
		else
			EXTrace.Print(EXTrace.AI, this, "GetMagazineInInventory " + ammo_pile + " ammo count " + last_ammo_pile_count);
#endif

		// prioritize magazine
		if (ammo_magazine)
		{
			return ammo_magazine;
		}

		return ammo_pile;
	}

	void eAI_SetUnlimitedReload(bool unlimitedReload)
	{
		m_eAI_UnlimitedReload = unlimitedReload;
	}

	void eAI_SetAccuracy(float accuracyMin, float accuracyMax)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, accuracyMin);
		EXTrace.Add(trace, accuracyMax);

		if (accuracyMin <= 0)
			accuracyMin = GetExpansionSettings().GetAI().AccuracyMin;

		m_eAI_AccuracyMin = accuracyMin;

		if (accuracyMax <= 0)
			accuracyMax = GetExpansionSettings().GetAI().AccuracyMax;

		m_eAI_AccuracyMax = accuracyMax;

		EXTrace.Add(trace, m_eAI_AccuracyMin);
		EXTrace.Add(trace, m_eAI_AccuracyMax);

		SetSynchDirty();
	}

	void eAI_SetThreatDistanceLimit(float distance)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, distance);

		if (distance <= 0)
			distance = GetExpansionSettings().GetAI().ThreatDistanceLimit;

		m_eAI_ThreatDistanceLimit = distance;

		EXTrace.Add(trace, distance);
	}

	void eAI_SetDamageMultiplier(float multiplier)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, multiplier);

		if (multiplier <= 0)
			multiplier = GetExpansionSettings().GetAI().DamageMultiplier;

		m_eAI_DamageMultiplier = multiplier;

		EXTrace.Add(trace, multiplier);
	}

	override bool IsFighting()
	{
		return m_IsFighting || (m_eAI_MeleeTime > 0 && GetGame().GetTime() - m_eAI_MeleeTime < 1500);
	}

	eAIAimingProfile GetAimingProfile()
	{
		return m_AimingProfile;
	}

	array<ref eAITarget> GetTargets()
	{
		return m_eAI_Targets;
	}

	int TargetCount()
	{
		return m_eAI_Targets.Count();
	}

	eAITarget GetTarget(int index = 0)
	{
		return m_eAI_Targets[index];
	}

	void OnAddTarget(eAITarget target)
	{
		m_eAI_Targets.Insert(target);
		if (m_eAI_Targets.Count() == 1)
			m_eAI_SyncCurrentTarget = true;
#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "OnAddTarget " + target.info.GetEntityDebugName() + " - found at time " + target.found_at_time + " - max time " + target.max_time + " - target count " + m_eAI_Targets.Count());
#endif
	}

	void OnRemoveTarget(eAITarget target)
	{
		m_eAI_Targets.RemoveItem(target);
		m_eAI_TargetInformationStates.Remove(target.info);
#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "OnRemoveTarget " + target.info.GetEntityDebugName() + " - time remaining " + (target.found_at_time + target.max_time - GetGame().GetTime()) + " - target count " + m_eAI_Targets.Count());
#endif
	}

	float GetThreatToSelf(bool ignoreLOS = false)
	{
		if (ignoreLOS)
			return m_eAI_CurrentThreatToSelf;

		return m_eAI_CurrentThreatToSelfActive;
	}

	//! @note all targets except item targets (no state)
	float eAI_GetTargetThreat(eAITargetInformation info, bool ignoreLOS = false)
	{
		eAITargetInformationState state;
		if (m_eAI_TargetInformationStates.Find(info, state))
		{
			if (ignoreLOS)
				return state.m_ThreatLevel;

			return state.m_ThreatLevelActive;
		}

		return 0.0;
	}

	void DetermineThreatToSelf(float pDt)
	{
		m_eAI_PreviousThreatToSelf = m_eAI_CurrentThreatToSelf;
		m_eAI_PreviousThreatToSelfActive = m_eAI_CurrentThreatToSelfActive;

		if (m_eAI_Targets.Count() > 0)
		{
			auto target = GetTarget();

			m_eAI_CurrentThreatToSelf = target.GetThreat(this);

			EntityAI targetEntity = target.GetEntity();

			if (targetEntity.IsInherited(ItemBase))
			{
				m_eAI_CurrentThreatToSelfActive = m_eAI_CurrentThreatToSelf;
			}
			else
			{
				auto state = m_eAI_TargetInformationStates[target.info];
				m_eAI_CurrentThreatToSelfActive = state.m_ThreatLevelActive;
			}
		}
		else
		{
			m_eAI_CurrentThreatToSelf = 0.0;
			m_eAI_CurrentThreatToSelfActive = 0.0;
		}
	}

	/*!
	 * @brief Get state for given target info.
	 * New state will be created if none exists.
	 * @return eAITargetInformationState
	 * @note should not be used for item targets as they wouldn't be removed from the cache like other targets
	 */
	eAITargetInformationState eAI_GetTargetInformationState(eAITargetInformation info)
	{
		eAITargetInformationState state;
		if (!m_eAI_TargetInformationStates.Find(info, state))
		{
#ifdef DIAG
			EXTrace.Print(EXTrace.AI, this, "Added new target info state");
#endif
			state = new eAITargetInformationState(this, info);
			m_eAI_TargetInformationStates[info] = state;
		}

		return state;
	}

	void eAI_SetDespawnOnLoosingAggro(bool state)
	{
		m_eAI_DespawnOnLoosingAggro = state;
	}

	void eAI_Despawn()
	{
		ItemBase hands = GetItemInHands();
		if (hands)
		{
			hands.Expansion_SetLootable(false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 5000, false, hands);
		}
		SetAllowDamage(true);
		//! Kill AI to remove collision
		SetHealth(0);
		//! Delete body after delay (don't remove it too early after death or invisible collision will still be there, five seconds seems to work well)
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 5000, false, this);
	}

	void ReactToThreatChange(float pDt)
	{
		if (m_eAI_CurrentThreatToSelf >= 0.2)
		{
			if (m_eAI_PreviousThreatToSelf < 0.2)
				EXTrace.Print(EXTrace.AI, this, "current threat to self >= 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");

			if (!m_Expansion_DaylightVisibility && m_eAI_CurrentThreatToSelfActive >= 0.4 && m_eAI_PreviousThreatToSelfActive < 0.4 && !m_Expansion_ActiveVisibilityEnhancers.Count())
			{
				float nightVisibility;
				Expansion_TryTurningOnAnyLightsOrNVG(nightVisibility, false, true);  //! Switch on lights at night (skip NVG)
			}
		}
		else
		{
			if (m_eAI_PreviousThreatToSelf >= 0.2)
			{
				EXTrace.Print(EXTrace.AI, this, "current threat to self < 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");

				if (!m_Expansion_DaylightVisibility && m_Expansion_ActiveVisibilityEnhancers.Count())
					Expansion_TryTurningOffAnyLightsOrNVG(true);  //! Switch off any lights (skip NVG)
			}
		}

		if (m_eAI_CurrentThreatToSelfActive >= 0.4)
		{
			if (m_ThreatClearedTimeout <= 0)
				m_ThreatClearedTimeout = Math.RandomFloat(1, 3);
		}
		else
		{
			if (m_ThreatClearedTimeout > 0)
			{
				m_ThreatClearedTimeout -= pDt;
				if (m_ThreatClearedTimeout <= 0)
				{
					if (m_eAI_DespawnOnLoosingAggro)
					{
						GetGroup().ClearAI();
					}
					else if (m_eAI_CurrentThreatToSelf <= 0.2 && !GetExpansionSettings().GetAI().Manners)
					{
						int emoteId;
						switch (Math.RandomIntInclusive(1, 5))
						{
							case 0:
								emoteId = EmoteConstants.ID_EMOTE_TAUNT;
								break;
							case 1:
								emoteId = EmoteConstants.ID_EMOTE_TAUNTELBOW;
								break;
							case 2:
								emoteId = EmoteConstants.ID_EMOTE_THROAT;
								break;
							case 3:
								emoteId = EmoteConstants.ID_EMOTE_DABBING;
								break;
							case 4:
								emoteId = EmoteConstants.ID_EMOTE_CLAP;
								break;
							case 5:
								emoteId = EmoteConstants.ID_EMOTE_DANCE;
								break;
						}
						//EXPrint(ToString() + " can play emote? " + m_EmoteManager.CanPlayEmote(emoteId));
						Expansion_SetEmote(emoteId, true);
					}
				}
			}
		}

		if (IsRaised() && !GetWeaponManager().IsRunning())
		{
			//! During sidestep, CanRaiseWeapon will return false so we are able to lower weapon instantly
			if (!CanRaiseWeapon() || !eAI_HasLOS())
			{
				RaiseWeapon(false);
			}
			else if (m_eAI_CurrentThreatToSelfActive < 0.2)
			{
				if (m_eAI_PreviousThreatToSelfActive >= 0.2)
					m_WeaponLowerTimeout = Math.RandomFloat(0.5, 1.5);

				m_WeaponLowerTimeout -= pDt;
		
				if (m_WeaponLowerTimeout <= 0)
					RaiseWeapon(false);
			}
		}
	}

	void UpdateTargets(float pDt)
	{
#ifdef DIAG
		auto trace = EXTrace.Profile(EXTrace.AI, eAIBase);
#endif

#ifdef EAI_TRACE
		int ticks;
		int elapsed;
#endif
		// TODO: use particle system instead
		// XXX: I have no clue what the above comment means -lava76

		if (!GetGroup())  //! This should never happen
			return;

		if (eAI_IsPassive())
			return;

#ifdef DIAG
		auto hitch = new EXHitch(ToString() + "::UpdateTargets ", 20000);
#endif

		vector center = GetPosition();

		m_eAI_UpdateTargetsTick += pDt;
		if (m_eAI_CurrentPotentialTargetIndex >= m_eAI_PotentialTargetEntities.Count() && m_eAI_UpdateTargetsTick > Math.RandomFloat(0.1, 0.2))
		{
			m_eAI_UpdateTargetsTick = 0;
			m_eAI_CurrentPotentialTargetIndex = 0;

			//! Get objects in near range (30 m)

#ifdef EAI_TRACE
			ticks = TickCount(0);
#endif

			vector min = Vector(center[0] - 30, center[1] - 30, center[2] - 30);
			vector max = Vector(center[0] + 30, center[1] + 30, center[2] + 30);

			DayZPlayerUtils.SceneGetEntitiesInBox(min, max, m_eAI_PotentialTargetEntities);

#ifdef EAI_TRACE
			elapsed = TickCount(ticks);
			float time = elapsed / 10000.0;
			m_eAI_UpdateNearTargetsTime += time;
			m_eAI_UpdateNearTargetsCount++;
			float timeAvg = m_eAI_UpdateNearTargetsTime / m_eAI_UpdateNearTargetsCount;

			EXTrace.Add(trace, "objects in near range " + m_eAI_PotentialTargetEntities.Count() + " time (ms) " + time + " timeAvg (ms) " + timeAvg);
#endif
		}

		//! Get other players/AI in extended range (1000 m radius) - check one player per tick (30 players per second)

#ifdef EAI_TRACE
		ticks = TickCount(0);
#endif

		if (!GetGroup().GetFaction().IsObserver())  //! Observers only react to near players
		{
			if (!m_eAI_PotentialTargetPlayer)
				m_eAI_PotentialTargetPlayer = s_Expansion_AllPlayers.m_Head;

			PlayerBase player = m_eAI_PotentialTargetPlayer.m_Value;

			EntityAI playerEntity = player;
			if (player && player != this && m_eAI_PotentialTargetEntities.Find(playerEntity) == -1 && Math.IsPointInCircle(center, 1000, player.GetPosition()))
			{
				m_eAI_PotentialTargetEntities.Insert(playerEntity);

#ifdef EAI_TRACE
				elapsed = TickCount(ticks);

				EXTrace.Add(trace, "player/AI in extended range " + player.GetType() + " time (ms) " + (elapsed / 10000.0).ToString());
#endif
			}

			m_eAI_PotentialTargetPlayer = m_eAI_PotentialTargetPlayer.m_Next;
		}

		PlayerBase playerThreat;
		ItemBase targetItem;
		EntityAI targetEntity;

		float group_count = GetGroup().Count();

		while (m_eAI_CurrentPotentialTargetIndex < m_eAI_PotentialTargetEntities.Count())
		{
			EntityAI obj = m_eAI_PotentialTargetEntities[m_eAI_CurrentPotentialTargetIndex++];
			if (!obj)
				continue;

			if (Class.CastTo(playerThreat, obj))
			{
				if (playerThreat == this)
					continue;
				if (playerThreat.eAI_IsPassive())
					continue;
				if (!playerThreat.IsPlayerLoaded())
					continue;
				if (!PlayerIsEnemy(playerThreat))
					continue;
			}
			else if (obj.IsInherited(DayZPlayerImplement))
				continue;  //! Ignore non PlayerBase NPCs
			else if (Class.CastTo(targetItem, obj))
			{
				//! If the object is an item, ignore it if any of the following conditions are met
				if (targetItem.IsSetForDeletion() || GetGroup().GetFaction().IsObserver())
					continue;
			}
			else if (obj.IsInherited(Building))
				continue;
			else if (obj.IsInherited(Transport))
				continue;
			else if (Class.CastTo(targetEntity, obj) && GetGroup().GetFaction().IsFriendly(targetEntity))
				continue;

			eAITargetInformation target = eAITargetInformation.GetTargetInformation(obj);
			if (!target)
				continue;

			if (!target.IsActive())
				continue;

			if (obj.IsInherited(ItemBase))
			{
				if (target.ShouldRemove(this))
					continue;
			}
			else if (target.ShouldRemove())
			{
				continue;
			}

			int num_ai_in_group_targetting = 0;
			if (m_eAI_Targets.Count() > 0 && m_eAI_CurrentThreatToSelf >= 0.4 && target.IsTargetted(GetGroup(), num_ai_in_group_targetting))
			{
				float num_ai_in_group_not_targeting = group_count - num_ai_in_group_targetting;
				if (!num_ai_in_group_not_targeting)
					continue;
				float threatLevel = target.GetThreat(this);
				float frac = num_ai_in_group_not_targeting / group_count;
				if ((frac * threatLevel) < (1.0 / group_count))
					continue;
			}

			target.AddAI(this);

			if (m_eAI_Targets.Count() * 2 > group_count)
				break;
		}

		if (m_eAI_CurrentPotentialTargetIndex >= m_eAI_PotentialTargetEntities.Count())
			m_eAI_PotentialTargetEntities.Clear();
	}

	void eAI_OnRemovePlayer(CF_DoublyLinkedNode_WeakRef<PlayerBase> node)
	{
		if (node == m_eAI_PotentialTargetPlayer)
			m_eAI_PotentialTargetPlayer = node.m_Next;
	}

	bool eAI_RemoveTargets()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAI_RemoveTargets");
#endif

#ifdef DIAG
		auto hitch = new EXHitch(ToString() + "::eAI_RemoveTargets ", 20000);
#endif

		eAITarget target = m_eAI_Targets[0];

		int count = m_eAI_Targets.Count();

		for (int i = count - 1; i >= 0; i--)
		{
			if (m_eAI_Targets[i].ShouldRemove(this))
			{
#ifdef DIAG
				EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTargets - removing target " + m_eAI_Targets[i].info.GetEntityDebugName());
#endif
				m_eAI_Targets[i].RemoveAI(this);
			}
		}

#ifdef DIAG
		if (count > 0 && m_eAI_Targets.Count() == 0)
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTargets - no more targets");
#endif

		if (m_eAI_Targets[0] != target)
			m_eAI_SyncCurrentTarget = true;

		return count != m_eAI_Targets.Count();
	}

	void eAI_PrioritizeTargets()
	{
#ifdef DIAG
		auto trace = EXTrace.Profile(EXTrace.AI, eAIBase);

		auto hitch = new EXHitch(ToString() + "::eAI_PrioritizeTargets ", 20000);
#endif

		//! find the target with the highest threat level, no sorting
		//! (sorting performance would be abysmal if there are many targets due to processing n*(n/2)-n/2 targets when comparing threat levels)

		float threat;
		float max_threat;
		int max_threat_idx;

		for (int i = 0; i < m_eAI_Targets.Count(); i++)
		{
			threat = m_eAI_Targets[i].GetThreat(this);
			if (threat > max_threat)
			{
				max_threat_idx = i;
				max_threat = threat;
			}
		}

		if (max_threat_idx > 0)
		{
			eAITarget target = m_eAI_Targets[0];
			m_eAI_Targets[0] = m_eAI_Targets[max_threat_idx];
			m_eAI_Targets[max_threat_idx] = target;
			m_eAI_SyncCurrentTarget = true;

#ifdef DIAG
			EXTrace.Print(EXTrace.AI, this, "eAI_PrioritizeTargets - prioritizing target " + max_threat_idx + " " + m_eAI_Targets[0].info.GetEntityDebugName() + " threat lvl " + max_threat);
#endif
		}
	}

	void eAI_SyncCurrentTarget()
	{
		m_eAI_SyncCurrentTarget = false;

	#ifdef SERVER
		eAITarget target = GetTarget();
		if (!target || !target.GetEntity() || target.GetEntity().IsInherited(ItemBase))
			return;

		auto rpc = ExpansionScriptRPC.Create();
		int idLow, idHigh;
		target.GetEntity().GetNetworkID(idLow, idHigh);
		rpc.Write(idLow);
		rpc.Write(idHigh);
		rpc.Send(this, ExpansionRPC.SyncCurrentAITarget, true, null);
	#endif
	}

#ifndef SERVER
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		switch (rpc_type)
		{
			case ExpansionRPC.SyncCurrentAITarget:
				int idLow, idHigh;
				if (!ctx.Read(idLow))
					break;
				if (!ctx.Read(idHigh))
					break;
				Object entity = GetGame().GetObjectByNetworkId(idLow, idHigh);
				if (!entity)
					break;
				eAITargetInformation info = eAITargetInformation.GetTargetInformation(entity);
				if (!info)
					break;
				eAITarget target = GetTarget();
				if (target)
					target.RemoveAI(this);
				target = info.AddAI(this);
				EXTrace.Print(EXTrace.AI, this, "Prioritizing target " + info.GetEntityDebugName());
				break;
		}
	}
#endif

	eAICommandMove GetCommand_MoveAI()
	{
		return eAICommandMove.Cast(GetCommand_Script());
	}

	eAICommandMove StartCommand_MoveAI()
	{
		EXTrace.Print(EXTrace.AI, this, "StartCommand_MoveAI");
		// WARNING: memory leak
		eAICommandMove cmd = new eAICommandMove(this, m_ExpansionST);
		StartCommand_Script(cmd);
		m_eAI_Command = cmd;
		return cmd;
	}

	eAICommandVehicle GetCommand_VehicleAI()
	{
		return eAICommandVehicle.Cast(GetCommand_Script());
	}

	eAICommandVehicle StartCommand_VehicleAI(Transport vehicle, int seatIdx, int seat_anim, bool fromUnconscious = false)
	{
		// WARNING: memory leak
		eAICommandVehicle cmd = new eAICommandVehicle(this, m_ExpansionST, vehicle, seatIdx, seat_anim, fromUnconscious);
		StartCommand_Script(cmd);
		m_eAI_Command = cmd;
		return cmd;
	}

/*
	HumanCommandVehicle GetCommand_VehicleAI()
	{
		return GetCommand_Vehicle();
	}
	HumanCommandVehicle StartCommand_VehicleAI(Transport vehicle, int seatIdx, int seat_anim, bool fromUnconscious = false)
	{
		return StartCommand_Vehicle(vehicle, seatIdx, seat_anim, fromUnconscious);
	}
*/

	void Notify_Transport(Transport vehicle, int seatIndex)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Notify_Transport").Add(vehicle).Add(seatIndex);
#endif

		m_eAI_Transport = vehicle;
		m_eAI_Transport_SeatIndex = seatIndex;
	}

	Transport eAI_GetTransport()
	{
		return m_eAI_Transport;
	}

	void Notify_Melee()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Notify_Melee");
#endif

		Expansion_GetUp();

		m_eAI_Melee = true;
	}

	override void OnCommandMelee2Start()
	{
		super.OnCommandMelee2Start();
		EXTrace.Print(EXTrace.AI, this, "OnCommandMelee2Start");
		m_eAI_MeleeTime = GetGame().GetTime();
	}

	override void EndFighting()
	{
		super.EndFighting();
		EXTrace.Print(EXTrace.AI, this, "EndFighting - melee time " + (GetGame().GetTime() - m_eAI_MeleeTime) + " ms");
	}

	/**
	 * @brief Overrides the desired position to generate a new path
	 *
	 * @param pPosition the target position for path finding
	 */
	void OverrideTargetPosition(vector pPosition, bool isFinal = true)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OverrideTargetPosition").Add(pPosition);
#endif

#ifndef EAI_USE_LEGACY_PATHFINDING
		m_PathFinding.SetTarget(pPosition);
#else
		m_PathFinding.OverridePosition(pPosition);
#endif
		m_eAI_TargetPositionIsFinal = isFinal;
	}

	void OverrideTurnTarget(bool pActive, float angle)
	{
		m_eAI_TurnTargetActive = pActive;
		m_eAI_TurnTarget = angle;
	}

	/**
	 * @brief Overrides speed target at which the ai wants to move at
	 *
	 * @param pActive is the override active
	 * @param pPosition 0 = IDLE, 1 = WALK, 2 = RUN, 3 = SPRINT
	 */
	void OverrideMovementSpeed(bool pActive, int pSpeed)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OverrideMovementSpeed").Add(pActive).Add(pSpeed);
#endif

		if (EXTrace.AI && (pActive != m_MovementSpeedActive || pSpeed != m_MovementSpeed))
			EXTrace.Print(true, this, "OverrideMovementSpeed " + m_MovementSpeedActive + " -> " + pActive + " " + m_MovementSpeed + " -> " + pSpeed);

		m_MovementSpeedActive = pActive;
		m_MovementSpeed = pSpeed;
	}

	/**
	 * @brief Overrides movement direction (forwards, backwards or strafing)
	 *
	 * @param pActive is the override active
	 * @param pDirection Relative angle on current model orientation from [-180, 180] where 0 is forward.
	 */
	void OverrideMovementDirection(bool pActive, float pDirection)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OverrideMovementDirection").Add(pActive).Add(pDirection);
#endif

		if (!pActive && m_SideStepAngle)
			return;

		if (EXTrace.AI && (pActive != m_MovementDirectionActive || pDirection != m_MovementDirection))
			EXTrace.Print(true, this, "OverrideMovementDirection " + m_MovementDirectionActive + " -> " + pActive + " " + m_MovementDirection + " -> " + pDirection);

		m_MovementDirectionActive = pActive;
		m_MovementDirection = pDirection;
	}

	//! @note returns whether stance was changed or not
	bool OverrideStance(int pStanceIdx, bool force = false)
	{
		eAICommandMove move = GetCommand_MoveAI();

		if (!move && force)
			move = StartCommand_MoveAI();

		if (move)
		{
			int stanceIdx = move.GetStance();
			if (move.OverrideStance(pStanceIdx))
			{
				if (EXTrace.AI)
					EXTrace.Print(true, this, "OverrideStance " + typename.EnumToString(eAIStance, stanceIdx) + " -> " + typename.EnumToString(eAIStance, pStanceIdx) + " " + force);

				return true;
			}
		}

		return false;
	}

	override bool IsPlayerInStance(int pStanceMask)
	{
		auto cmd = GetCommand_MoveAI();
		if (cmd)
			return ((1 << cmd.GetStance()) & pStanceMask) != 0;

		//! @note IMPORTANT need to call super if not in eAICommandMove else it breaks actions!
		return super.IsPlayerInStance(pStanceMask);
	}

	bool eAI_IsChangingStance()
	{
		auto cmd = GetCommand_MoveAI();
		return cmd && cmd.IsChangingStance();
	}

	void eAI_SetSniperProneDistanceThreshold(float distance)
	{
		m_eAI_SniperProneDistanceThreshold = distance;
	}

	bool eAI_AdjustStance(Weapon_Base gun, float distToTargetSq)
	{
		//! Go prone if target is farther than threshold, we weren't hit in the last 10 s, we face the target, and we have a long range optic,
		//! otherwise stand back up

		float distThreshSq = m_eAI_SniperProneDistanceThreshold * m_eAI_SniperProneDistanceThreshold;
		if (distThreshSq > 0.0 && distToTargetSq > distThreshSq && GetGame().GetTickTime() - m_eAI_LastHitTime > 10.0)
		{
			vector targetDirection = vector.Direction(GetPosition(), m_eAI_AimPosition_WorldSpace).Normalized();
			ItemOptics optics;
			if (vector.Dot(GetDirection(), targetDirection) > 0.9 && Class.CastTo(optics, gun.GetAttachedOptics()) && optics.GetZeroingDistanceZoomMax() >= 800)
				return OverrideStance(DayZPlayerConstants.STANCEIDX_PRONE);
		}

		return OverrideStance(DayZPlayerConstants.STANCEIDX_ERECT);
	}

	bool eAI_AdjustStance(int lastFireTime, int timeSinceLastFire, int timeBetweenFiringAndGettingUp)
	{
		bool getUp;

		if (lastFireTime > 0 && timeSinceLastFire > timeBetweenFiringAndGettingUp)
		{
			getUp = Expansion_GetUp();
			if (getUp && EXTrace.AI)
				EXTrace.Print(true, this, "eAI_AdjustStance " + timeSinceLastFire + " > " + timeBetweenFiringAndGettingUp);
		}

		return getUp;
	}

	int eAI_GetStance()
	{
		auto cmd = GetCommand_MoveAI();
		if (cmd)
			return cmd.GetStance();

		return eAIStance.UNKNOWN;
	}

	void SetMovementSpeedLimit(int pSpeed)
	{
		m_MovementSpeedLimit = eAI_GetMovementSpeed(pSpeed);
	}

	void SetMovementSpeedLimits(int pSpeed, int pSpeedUnderThreat = -1)
	{
		SetMovementSpeedLimit(pSpeed);
		if (pSpeedUnderThreat == -1)
			pSpeedUnderThreat = pSpeed;
		m_MovementSpeedLimitUnderThreat = eAI_GetMovementSpeed(pSpeedUnderThreat);
	}

	int GetMovementSpeedLimit()
	{
		return m_MovementSpeedLimit;
	}

	float eAI_GetCurrentMovementSpeedLimit()
	{
		auto cmd = GetCommand_MoveAI();
		if (cmd)
			return cmd.GetSpeedLimit();

		return -1;
	}

	static int eAI_GetMovementSpeed(eAIMovementSpeed pSpeed)
	{
		switch (pSpeed)
		{
			case eAIMovementSpeed.RANDOM:
				return Math.RandomIntInclusive(0, 3);
			case eAIMovementSpeed.RANDOM_NONSTATIC:
				return Math.RandomIntInclusive(1, 3);
		}

		return pSpeed;
	}

	void CreateDebugApple()
	{
		Class.CastTo(m_DebugTargetApple, GetGame().CreateObject("Apple", GetPosition(), true));
	}

	void DestroyDebugApple()
	{
		if (m_DebugTargetApple)
			m_DebugTargetApple.Delete();
	}

	EntityAI Expansion_CloneItemToHands(EntityAI src)
	{
		InventoryLocation il_dst = new InventoryLocation();

		il_dst.SetHands(this, src);

		return Expansion_CloneItemToLocation(src, il_dst);
	}

	EntityAI Expansion_CloneItemToGround(EntityAI src, vector position = "0 0 0")
	{
		InventoryLocation ground();

		vector mat[4];
		Math3D.MatrixIdentity4(mat);
		mat[3] = position;

		ground.SetGround(NULL, mat);

		return Expansion_CloneItemToLocation(src, ground);
	}

	EntityAI Expansion_CloneItemToInventory(EntityAI src, FindInventoryLocationType flags = 0)
	{
		InventoryLocation location();

		/*!
		 * ATTACHMENT          = 4
		 * CARGO               = 8
		 * HANDS               = 16
		 * PROXYCARGO          = 32
		 * ANY_CARGO           = 40 (CARGO | PROXYCARGO)
		 * ANY                 = 60 (ATTACHMENT | ANY_CARGO | HANDS)
		 * NO_SLOT_AUTO_ASSIGN = 64
		 */
		if (!flags)
			flags = FindInventoryLocationType.ATTACHMENT | FindInventoryLocationType.ANY_CARGO;

		if (!GetInventory().FindFirstFreeLocationForNewEntity(src.GetType(), flags, location))
			return null;

		return Expansion_CloneItemToLocation(src, location);
	}

	EntityAI Expansion_CloneItemToLocation(EntityAI src, InventoryLocation location)
	{
		if (!src || src.IsSetForDeletion())
			return null;

		if (location.GetType() == InventoryLocationType.HANDS)
		{
			//! Forcing switch to HumanCommandMove before taking to hands,
			//! and hiding/showing item in hands after, unbreaks hand anim state
			if (!GetCommand_Move())
				StartCommand_Move();
			else
				m_eAI_CommandTime = 0.0;
		}

		EntityAI dst = ExpansionItemSpawnHelper.Clone(src, true, location);
		if (dst)
		{
			ItemBase item;
			if (Class.CastTo(item, src) && item.GetHierarchyRootPlayer() == this)
				eAI_RemoveItem(item);

			ItemBase dstItem;
			if (Class.CastTo(dstItem, dst))
				dstItem.m_Expansion_IsOwnerPlayer = item.m_Expansion_IsOwnerPlayer;

			GetGame().ObjectDelete(src);

			if (location.GetType() == InventoryLocationType.HANDS)
			{
				//! Forcing switch to HumanCommandMove before taking to hands,
				//! and hiding/showing item in hands after, unbreaks hand anim state
				GetItemAccessor().HideItemInHands(true);
				GetItemAccessor().HideItemInHands(false);
			}
			else if (location.GetType() == InventoryLocationType.GROUND && !m_Expansion_CanBeLooted)
			{
				if (dstItem && !dstItem.m_Expansion_IsOwnerPlayer)
				{
					ExpansionItemBaseModule.SetLootable(dst, false);
					dst.SetLifetimeMax(120);  //! Make sure it despawns quickly when left alone
				}
			}
		}

		return dst;
	}

	void eAI_AddItem(ItemBase item)
	{
		if (item.IsClothing() && m_eAI_ItemThreatOverride.Count())
			m_eAI_ItemThreatOverride.Clear();

		if (item.IsWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - gun " + item);
			m_Weapons.Insert(item);
			return;
		}

		//! When item is cloned to a shoulder slot or similar, GetInventory().IsAttachment() is true, but GetHierarchyRootPlayer() is NULL
		//! GetHierarchyRoot() is the item itself in that case
		if (item.Expansion_IsMeleeWeapon() && (!item.GetInventory().IsAttachment() || !item.GetHierarchyRootPlayer() || item.GetHierarchyRootPlayer() == this))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - melee weapon " + item);
			m_MeleeWeapons.Insert(item);
			return;
		}

		if (item.Expansion_CanBeUsedToBandage())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - bandage/rag " + item);
			m_Bandages.Insert(item);
			return;
		}

		//! Ammo/magazines
		if (item.IsInherited(Magazine) && !item.GetInventory().IsAttachment())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - mag " + item);
			//! Force re-evaluation of any gun (loot) targets/guns in inventory
			eAI_EvaluateFirearmTypes();
		}
	}

	void eAI_RemoveItem(ItemBase item)
	{
		if (m_eAI_ItemThreatOverride.Count())
			m_eAI_ItemThreatOverride.Clear();

		if (item.IsWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - gun " + item);
			m_Weapons.RemoveItem(item);
			m_eAI_EvaluatedFirearmTypes.Remove(item.Type());
			return;
		}

		if (item.Expansion_IsMeleeWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - melee weapon " + item);
			m_MeleeWeapons.RemoveItem(item);
			return;
		}

		if (item.Expansion_CanBeUsedToBandage())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - bandage/rag " + item);
			m_Bandages.RemoveItem(item);
			return;
		}

		//! Ammo/magazines
		if (item.IsInherited(Magazine))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - mag " + item);
			//! Force re-evaluation of any gun (loot) targets/guns in inventory
			eAI_EvaluateFirearmTypes();
		}
	}

	bool eAI_HasWeaponInInventory()
	{
		return m_Weapons.Count() > 0;
	}

	bool eAI_HasAmmoForFirearm(Weapon_Base gun, out Magazine mag, bool checkMagsInInventory = true)
	{
		if (gun.Expansion_HasAmmo(mag))
			return true;

		bool found = m_eAI_EvaluatedFirearmTypes.Find(gun.Type(), mag);
		bool hasAmmo;

		if (found && mag && mag.GetAmmoCount())
			hasAmmo = true;

		if (!found || (checkMagsInInventory && !hasAmmo))
		{
			//! GetMagazineInInventory will only return non-empty mags/ammo. EXPENSIVE, use with care.
			Class.CastTo(mag, GetMagazineInInventory(gun));

			EXTrace.Print(EXTrace.AI, this, "eAI_HasAmmoForFirearm - inserting " + gun.Type() + " " + mag);
			m_eAI_EvaluatedFirearmTypes[gun.Type()] = mag;

			return mag != null;
		}

		return hasAmmo;
	}

	void eAI_EvaluateFirearmTypes()
	{
		TTypenameArray toRemove();

		foreach (typename type, Magazine mag: m_eAI_EvaluatedFirearmTypes)
		{
			if (!mag || mag.GetHierarchyRootPlayer() != this)
				toRemove.Insert(type);
		}

		foreach (typename removeType: toRemove)
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_EvaluateFirearmTypes - removing " + removeType + " " + m_eAI_EvaluatedFirearmTypes[removeType]);
			m_eAI_EvaluatedFirearmTypes.Remove(removeType);
		}
	}

	string Expansion_CommandIDToString(int cmdID)
	{
		string cmd;
		switch (cmdID)
		{
			case DayZPlayerConstants.COMMANDID_NONE:
				cmd = "COMMANDID_NONE";
				break;
			case DayZPlayerConstants.COMMANDID_MOVE:
				cmd = "COMMANDID_MOVE";
				break;
			case DayZPlayerConstants.COMMANDID_ACTION:
				cmd = "COMMANDID_ACTION";
				break;
			case DayZPlayerConstants.COMMANDID_MELEE:
				cmd = "COMMANDID_MELEE";
				break;
			case DayZPlayerConstants.COMMANDID_MELEE2:
				cmd = "COMMANDID_MELEE2";
				break;
			case DayZPlayerConstants.COMMANDID_FALL:
				cmd = "COMMANDID_FALL";
				break;
			case DayZPlayerConstants.COMMANDID_DEATH:
				cmd = "COMMANDID_DEATH";
				break;
			case DayZPlayerConstants.COMMANDID_DAMAGE:
				cmd = "COMMANDID_DAMAGE";
				break;
			case DayZPlayerConstants.COMMANDID_LADDER:
				cmd = "COMMANDID_LADDER";
				break;
			case DayZPlayerConstants.COMMANDID_UNCONSCIOUS:
				cmd = "COMMANDID_UNCONSCIOUS";
				break;
			case DayZPlayerConstants.COMMANDID_SWIM:
				cmd = "COMMANDID_SWIM";
				break;
			case DayZPlayerConstants.COMMANDID_VEHICLE:
				cmd = "COMMANDID_VEHICLE";
				break;
			case DayZPlayerConstants.COMMANDID_CLIMB:
				cmd = "COMMANDID_CLIMB";
				break;
			case DayZPlayerConstants.COMMANDID_SCRIPT:
				cmd = "COMMANDID_SCRIPT";
				break;
		}

		cmd += "(" + cmdID.ToString() + ")";

		return cmd;
	}

	void eAI_ClientUpdate()
	{
		float pDt = 1.0 / 30.0;

		GetTransform(m_ExTransformPlayer);

		if (!eAI_HandleAiming(pDt))
			return;

		HumanInputController hic = GetInputController();
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();

		if (hic && entityInHands && entityInHands.IsInherited(Weapon_Base))
		{
			bool exitIronSights = false;
			HandleWeapons(pDt, entityInHands, hic, exitIronSights);
		}
	}

	bool eAI_HandleAiming(float pDt, bool hasLOS = false)
	{
		eAITarget target = m_eAI_Targets[0];

		bool isServer = GetGame().IsServer();

		if (target)
		{
			vector aimPosition = target.GetPosition(this, !isServer) + target.GetAimOffset(this);
			if (isServer)
			{
				bool lookDirectionRecalculate;
				if (m_eAI_CurrentThreatToSelfActive > 0.1 && hasLOS)
					lookDirectionRecalculate = true;
				LookAtPosition(aimPosition, lookDirectionRecalculate);
				if (!m_eAI_LookDirection_Recalculate)
					LookAtDirection("0 0 1");
			}
			bool aimDirectionRecalculate;
			if (!isServer || m_eAI_CurrentThreatToSelfActive > 0.15)
				aimDirectionRecalculate = true;
			AimAtPosition(aimPosition, aimDirectionRecalculate);
		}
		else
		{
			if (isServer && m_eAI_LookDirection_Recalculate)
				LookAtDirection("0 0 1");

			if (m_eAI_AimDirection_Recalculate)
				AimAtDirection("0 0 1");
		}

		vector neck = GetBonePositionWS(GetBoneIndexByName("neck"));

		if (isServer && m_eAI_LookDirection_Recalculate)
		{
			m_eAI_LookDirectionTarget_ModelSpace = vector.Direction(neck, m_eAI_LookPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

		if (m_eAI_AimDirection_Recalculate)
		{
			m_eAI_AimDirectionTarget_ModelSpace = vector.Direction(neck, m_eAI_AimPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

		if (target)
			return true;
		return false;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_3(this, "CommandHandler").Add(pDt).Add(pCurrentCommandID).Add(pCurrentCommandFinished);
#endif

		//! Used for animated/continuous action progress
		m_dT = pDt;

		// CarScript car;
		// if (Class.CastTo(car, GetParent()))
		//{
		//	car.Control(pDt);
		// }
#ifdef DIAG
#ifndef SERVER
		for (int i = m_Expansion_DebugShapes.Count() - 1; i >= 0; i--)
			m_Expansion_DebugShapes[i].Destroy();
		m_Expansion_DebugShapes.Clear();
#endif
#endif
		if (pCurrentCommandID != m_eAI_CurrentCommandID)
		{
			if (EXTrace.AI)
				EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(m_eAI_CurrentCommandID) + " -> " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
			m_eAI_CurrentCommandID = pCurrentCommandID;
			m_eAI_CommandTime = 0.0;
		}
		else if (pCurrentCommandFinished && EXTrace.AI)
		{
			EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
		}

		m_eAI_CommandTime += pDt;

		Expansion_UpdateBonePositionTimes(pDt);

		GetMovementState(m_MovementState);

		//! handle death with high priority
		if (HandleDeath(pCurrentCommandID))
		{
			return;
		}

		if (!GetGame().IsServer())  //! @note ComandHandler will not run on MP client for AI anyway
			return;

		int simulationPrecision = 0;

		UpdateTargets(pDt);
		if (eAI_RemoveTargets() || m_eAI_UpdateTargetsTick == 0)
			eAI_PrioritizeTargets();
		if (m_eAI_SyncCurrentTarget)
			eAI_SyncCurrentTarget();

		GetTransform(m_ExTransformPlayer);

		bool hasLOS = EnforceLOS();

		DetermineThreatToSelf(pDt);
		ReactToThreatChange(pDt);

		eAI_HandleAiming(pDt, hasLOS);

		if (pCurrentCommandID != DayZPlayerConstants.COMMANDID_CLIMB)
			m_PathFinding.OnUpdate(pDt, simulationPrecision);

		if (m_eAI_SideStepTimeout > 0)
		{
			m_eAI_SideStepTimeout -= pDt;
			if (m_eAI_SideStepTimeout <= 0)
			{
				EXTrace.Print(EXTrace.AI, this, "sidestep 0");
				m_SideStepAngle = 0;
				OverrideMovementDirection(false, 0);
				//StartCommand_MoveAI();
			}
		}

		HumanInputController hic = GetInputController();
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();

		if (hic)
		{
			bool exitIronSights = false;
			//! @note HandleWeapons also deals with hands lowering/raising if no weapon in hands
			HandleWeapons(pDt, entityInHands, hic, exitIronSights);
		}

		//GetDayZPlayerInventory().HandleInventory(pDt);

		if (m_WeaponManager)
			m_WeaponManager.Update(pDt);
		if (m_EmoteManager && IsPlayerSelected())
			m_EmoteManager.Update(pDt);
		if (m_RGSManager)
			 m_RGSManager.Update();
		if (m_StaminaHandler)
			m_StaminaHandler.Update(pDt, pCurrentCommandID);
		if (m_InjuryHandler)
			m_InjuryHandler.Update(pDt);
		if (m_ShockHandler)
			m_ShockHandler.Update(pDt);
		if (m_HCAnimHandler)
			m_HCAnimHandler.Update(pDt, m_MovementState);

		GetPlayerSoundManagerServer().Update();
		GetHumanInventory().Update(pDt);
		UpdateDelete();

		OnScheduledTick(pDt);

		//! Do FSM update only after current command has been running for at least one command handler tick, else initial gun holding will look scuffed
		if (m_FSM && m_eAI_CommandTime > pDt)
			m_FSM.Update(pDt, simulationPrecision);

		bool skipScript;

		if (m_ActionManager)
		{
			m_ActionManager.Update(pCurrentCommandID);

			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				OnUnconsciousUpdate(pDt, m_LastCommandBeforeUnconscious);
				if (!m_IsUnconscious)
				{
					m_IsUnconscious = true;
					OnUnconsciousStart();
				}

				if (!m_ShouldBeUnconscious && m_UnconsciousTime > 2)
				{
					HumanCommandUnconscious hcu = GetCommand_Unconscious();
					if (hcu)
						hcu.WakeUp();
				}
			}
			else
			{
				if (m_ShouldBeUnconscious)
				{
					m_LastCommandBeforeUnconscious = pCurrentCommandID;
					m_eAI_UnconsciousVehicle = false;

					auto vehCmd = GetCommand_VehicleAI();
					if (vehCmd)
					{
						m_eAI_UnconsciousVehicle = true;

						// not going to bother supporting knocking players out at this current moment
						m_TransportCache = vehCmd.GetTransport();

						vehCmd.KeepInVehicleSpaceAfterLeave(true);
					}

					SetOrientation(GetOrientation());
					StartCommand_Unconscious(0);

					skipScript = true;
				}

				if (m_IsUnconscious)
				{
					m_IsUnconscious = false;
					OnUnconsciousStop(pCurrentCommandID);

					skipScript = true;
				}
			}
		}

		OnCommandHandlerTick(pDt, pCurrentCommandID);

		m_eAI_Command = ExpansionHumanCommandScript.Cast(GetCommand_Script());

		if (pCurrentCommandFinished)
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				if (m_eAI_UnconsciousVehicle && (m_TransportCache != null))
				{
					int crew_index = m_TransportCache.CrewMemberIndex(this);
					int seat = m_TransportCache.GetSeatAnimationType(crew_index);
					StartCommand_VehicleAI(m_TransportCache, crew_index, seat, true);
					m_TransportCache = null;
					return;
				}
			}

			if (PhysicsIsFalling(true))
			{
				SetOrientation(GetOrientation());
				StartCommand_Fall(0);

				return;
			}

			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
				StartCommand_Move();
			else
				StartCommand_MoveAI();

			return;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_CLIMB)
		{
			HumanCommandClimb hcc = GetCommand_Climb();

			if (hcc)
			{
				int hccState = hcc.GetState();
				if (hccState == ClimbStates.STATE_FINISH)
				{
					m_PathFinding.OnUpdate(pDt, -1);
					//StartCommand_MoveAI();
				}
				else if (hccState == ClimbStates.STATE_FALLING)
				{
					//StartCommand_MoveAI();
				}
				else if (hccState == ClimbStates.STATE_ONTOP)
				{
					//StartCommand_MoveAI();
				}
			}
		}

		// taken from vanilla DayZPlayerImplement
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_FALL)
		{
			HumanCommandFall fall = GetCommand_Fall();
			if (fall && fall.PhysicsLanded())
			{
				DayZPlayerType type = GetDayZPlayerType();
				NoiseParams npar;

				FallDamageData fallDamageData = new FallDamageData();
				fallDamageData.m_Height = m_FallYDiff - GetPosition()[1];

				// land
				if (fallDamageData.m_Height < 0.5)
				{
					fallDamageData.m_LandType = HumanCommandFall.LANDTYPE_NONE; 
					fall.Land(fallDamageData.m_LandType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (fallDamageData.m_Height < 3.0)
				{
					if (m_MovementState.IsInProne() || m_MovementState.IsInRaisedProne())
						fallDamageData.m_LandType = HumanCommandFall.LANDTYPE_NONE;
					else
						fallDamageData.m_LandType = HumanCommandFall.LANDTYPE_LIGHT;
					
					fall.Land(fallDamageData.m_LandType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (fallDamageData.m_Height < 5.0)
				{
					fallDamageData.m_LandType = HumanCommandFall.LANDTYPE_MEDIUM;
					fall.Land(fallDamageData.m_LandType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}
				else
				{
					fallDamageData.m_LandType = HumanCommandFall.LANDTYPE_HEAVY;
					fall.Land(fallDamageData.m_LandType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}
				
				if (fallDamageData.m_Height >= DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
				{
					OnPlayerRecievedHit();
				}

				m_FallDamage.HandleFallDamage(fallDamageData);
				m_JumpClimb.CheckAndFinishJump(fallDamageData.m_LandType);
			}

			// return;
		}
		else if (PhysicsIsFalling(false))
		{
			SetOrientation(GetOrientation());
			StartCommand_Fall(0);

			m_FallYDiff = GetPosition()[1];
			// return;

			//! Prevent NULL pointers by skipping COMMANDID_SCRIPT handling
			skipScript = true;
		}

		if (HandleDamageHit(pCurrentCommandID))
		{
			// return;

			skipScript = true;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			HumanCommandVehicle hcv = GetCommand_Vehicle();
			if (hcv.WasGearChange())
			{
				GearChangeActionCallback cb = GearChangeActionCallback.Cast(AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, GearChangeActionCallback));
				cb.SetVehicleCommand(hcv);
			}

			return;
		}
		else if (m_eAI_Transport)
		{
			int seat_anim_type = m_eAI_Transport.GetSeatAnimationType(m_eAI_Transport_SeatIndex);
			auto vehCommand = StartCommand_VehicleAI(m_eAI_Transport, m_eAI_Transport_SeatIndex, seat_anim_type);
			vehCommand.SetVehicleType(m_eAI_Transport.GetAnimInstance());

			m_eAI_Transport = null;
			return;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{
			//! COMMANDID_MOVE will be running if the AI was just spawned
			//! IMPORTANT: Start AI move only after delay, else hand anim state will be broken!
			if (m_eAI_CommandTime > pDt)
				StartCommand_MoveAI();
			// return;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			HumanCommandMelee2 hcm2 = GetCommand_Melee2();
			if (hcm2)
			{
				if (hcm2.WasHit())
				{
					m_eMeleeCombat.OnHit();

					m_eAI_MeleeDidHit = true;
				}

				if (m_eAI_Melee && m_eAI_MeleeDidHit)
				{
					m_eAI_Melee = false;
					m_eAI_MeleeTime = GetGame().GetTime();

					m_eMeleeCombat.Combo(hcm2);
				}
			}
		}
		else if (m_eAI_Melee)
		{
			m_eAI_Melee = false;

			m_eMeleeCombat.Start();

			//! Prevent NULL pointers by skipping COMMANDID_SCRIPT handling
			skipScript = true;
		}

		m_eAI_MeleeDidHit = false;

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT && m_eAI_Command && !skipScript)
		{
			vector lookTargetRelAngles = m_eAI_LookDirectionTarget_ModelSpace.VectorToAngles();

			if (IsRaised())
			{
				//! Need to adjust look direction when aiming
				vector aimTargetRelAngles = m_eAI_AimDirectionTarget_ModelSpace.VectorToAngles();

				lookTargetRelAngles[0] = lookTargetRelAngles[0] - aimTargetRelAngles[0];
				if (entityInHands && entityInHands.IsWeapon())
					lookTargetRelAngles[1] = lookTargetRelAngles[1] - aimTargetRelAngles[1];
			}

			//! We want to interpolate rel angles for looking! Otherwise, if the conversion to rel angles happens later,
			//! there will be a sudden jump in the unit's head rotation between 180 and -180 due to the way the head animation is set up
			lookTargetRelAngles[0] = ExpansionMath.RelAngle(lookTargetRelAngles[0]);
			lookTargetRelAngles[1] = ExpansionMath.RelAngle(lookTargetRelAngles[1]);

			lookTargetRelAngles[1] = Math.Clamp(lookTargetRelAngles[1], -85.0, 85.0);  //! Valid range is [-85, 85]

			//TODO: quaternion slerp instead for better, accurate results
			m_eAI_LookRelAngles = ExpansionMath.InterpolateAngles(m_eAI_LookRelAngles, lookTargetRelAngles, pDt, Math.RandomFloat(3.0, 5.0), Math.RandomFloat(1.0, 3.0));

			m_eAI_Command.SetLookAnglesRel(m_eAI_LookRelAngles[0], m_eAI_LookRelAngles[1]);

			bool shouldVault = false;

			eAICommandMove hcm;

			int performCommand;
			if (Class.CastTo(hcm, m_eAI_Command))
				performCommand = EAI_COMMANDID_MOVE;

			switch (performCommand)
			{
			case EAI_COMMANDID_MOVE:
				if (AI_HANDLEVAULTING && HandleVaulting(pDt))
				{
					shouldVault = true;
					break;
				}

				if (AI_HANDLEDOORS)
				{
					HandleBuildingDoors(pDt);
				}
				
				float speedLimit;
				if (m_eAI_CurrentThreatToSelfActive >= 0.4)
					speedLimit = m_MovementSpeedLimitUnderThreat;
				else
					speedLimit = m_MovementSpeedLimit;

				if (m_WeaponRaised)
					speedLimit = Math.Min(speedLimit, 1);

				float turnTarget;
				bool setTurnTarget;

				if (m_eAI_TurnTargetActive)
				{
					turnTarget = m_eAI_TurnTarget;
					setTurnTarget = true;
				}
				else if (m_eAI_Targets.Count() > 0)
				{
					if (m_eAI_AimDirection_Recalculate || Math.AbsFloat(m_eAI_AimRelAngles[0]) > 90)
					{
						turnTarget = vector.Direction(GetPosition(), m_eAI_AimPosition_WorldSpace).Normalized().VectorToAngles()[0];
						setTurnTarget = true;
					}
				}
				else if (IsInherited(eAINPCBase) && speedLimit == 0)
				{
					turnTarget = GetOrientation()[0];
					setTurnTarget = true;
				}

				if (setTurnTarget)
					hcm.SetTurnTarget(turnTarget, m_eAI_TurnTargetActive);

				if (m_StaminaHandler && !CanConsumeStamina(EStaminaConsumers.SPRINT) || !CanSprint())
				{
					speedLimit = Math.Min(speedLimit, 2);
				}

				if (!m_MovementDirectionActive)
					m_MovementDirection = 0;

				hcm.SetSpeedOverrider(m_MovementSpeedActive);
				hcm.SetTargetSpeed(m_MovementSpeed);
				hcm.SetSpeedLimit(speedLimit);
				hcm.SetTargetDirection(m_MovementDirection, m_MovementDirectionActive);

				m_SprintFull = false;
				if (hcm.GetCurrentMovementSpeed() > 2.99)
				{
					m_SprintedTime += pDt;
				}
				else
				{
					m_SprintedTime = 0.0;
				}

				if (m_SprintedTime > 0.5)
					m_SprintFull = true;

				break;
			default:
			#ifdef DIAG
				Expansion_DebugObject(22222, "0 0 0", "ExpansionDebugBox_White");
				Expansion_DebugObject(22223, "0 0 0", "ExpansionDebugBox_Black");
			#endif
				break;
			}

			if (shouldVault)
			{
				m_JumpClimb.Expansion_Climb();
			}
		}

#ifndef EXPANSION_DISABLE_AI_ATTACHMENT
		Expansion_RunAttachment(pDt, pCurrentCommandID, pCurrentCommandFinished, true);
#endif
	}

	bool EnforceLOS()
	{
		//! Prevent core dump on server shutdown
		if (!GetGame())
			return false;

		if (!m_eAI_Targets.Count())
		{
			return false;
		}

		auto target = GetTarget();

		EntityAI targetEntity = target.GetEntity();

		if (targetEntity.IsInherited(ItemBase))
		{
			return false;
		}

		EntityAI parent = EntityAI.Cast(targetEntity.GetParent());

		auto state = m_eAI_TargetInformationStates[target.info];
		if (!state)
			return false;

		//if (targetEntity.IsInherited(Transport))
		//{
			//state.m_LOS = true;
			//return true;
		//}

		vector begPos = GetBonePositionWS(GetBoneIndexByName("Head"));
		vector aimOffset = target.GetAimOffset(this);
		vector endPos = targetEntity.GetPosition() + aimOffset;

		vector contactPos;
		vector contactDir;
		int contactComponent;

		set< Object > results = new set< Object >;
		bool hadLos = state.m_LOS;
		state.m_LOS = DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, null, this, false, false, ObjIntersectView, 0.05);
		if (!state.m_LOS && hadLos)
		{
			EXTrace.Print(EXTrace.AI, this, "lost line of sight to target " + targetEntity);
			EXTrace.Print(EXTrace.AI, this, "aim offset " + aimOffset + " end pos " + endPos);
		}
		if (!state.m_LOS)
			return false;

		if (results.Count() && results[0].IsBuilding())
		{
			//! Check again in case view is "blocked" by a window/other see-through object
			//! TODO: If a player is behind an Expansion BaseBuilding wall, the AI will "see" the player if he is standing next to a window position
			//! (regardless if the wall actually has windows or not or if they are closed) due to the wall's firegeo and use of ObjIntersectFire
			results.Clear();
			//Expansion_DebugObject_Deferred(9999, contactPos, "ExpansionDebugBox_Red", contactDir);
			state.m_LOS = DayZPhysics.RaycastRV(contactPos, endPos, contactPos, contactDir, contactComponent, results, null, this, false, false, ObjIntersectFire, 0.05);
		}

		//float targetDistSq = vector.DistanceSq(begPos, endPos);
		float contactToTargetDistSq = vector.DistanceSq(contactPos, endPos);

		DayZPlayerImplement player;

		bool sideStep;

		state.m_LOS = false;
		Object hitObj;
		EntityAI hitEntity;
		foreach (Object obj: results)
		{
			hitObj = obj;

			if (Class.CastTo(hitEntity, obj))
				obj = hitEntity.GetHierarchyRoot();

			//! Tree with player more than 2 m away from contact pos
			if (obj.IsTree() && contactToTargetDistSq > 4 && !state.m_SearchPositionUpdateCount)
			{
				sideStep = state.m_ThreatLevelActive >= 0.4;
				break;
			}

			//if (obj.IsBuilding())
			//{
			//	state.m_LOS = false;
			//	break;
			//}

			if (obj != targetEntity && obj != parent)
			{
				if (Class.CastTo(player, obj))
				{
					if (eAI_GetTargetThreat(player.GetTargetInformation()) < 0.2)
					{
						sideStep = m_eAI_IsFightingFSM;  //! Sidestep if we are in fighting FSM
					}
					else
					{
						//! If player is a threat, but not the target, we don't care if they get shot when they are in the way
						state.m_LOS = true;
					}
				}
				else if (obj.IsInherited(ZombieBase) || obj.IsInherited(AnimalBase) || ((obj.IsTree() || obj.IsBush()) && contactToTargetDistSq <= 4) || contactToTargetDistSq <= 0.0625)
				{
					//! If object is zombie, animal or tree/bush but not the target or its parent, we don't care if they get shot when they are in the way
					state.m_LOS = true;
				}

				break;
			}
			else
			{
				//! Right on target
				state.m_LOS = true;
				break;
			}
		}

		if (state.m_LOS != hadLos)
		{
			EXTrace.Print(EXTrace.AI, this, "has line of sight to target (" + targetEntity + ")? " + state.m_LOS);
			EXTrace.Print(EXTrace.AI, this, "aim offset " + aimOffset + " end pos " + endPos);
		}

		if (sideStep)
		{
			if (m_eAI_SideStepTimeout > 0 || m_MovementDirectionActive)
				return state.m_LOS;

			//! First check if we are roughly facing the target
			//! @note vector.Direction(GetPosition(), GetTarget().GetPosition(this)).Normalized() returns zero vector,
			//! have to first assign target position to variable. Why is this language so INCREDIBLY fucked? :-(
			vector targetPos = state.m_SearchPosition;
			vector targetDirection = vector.Direction(GetPosition(), targetPos).Normalized();
			float dot = vector.Dot(GetDirection(), targetDirection);
			if (dot >= 0.75)
			{
				eAI_ForceSideStep(1.5, hitObj);
			}
		}

		return state.m_LOS;
	}

	void eAI_ForceSideStep(float duration, Object obj = null, float angle = 0.0, bool allowBackPedaling = true)
	{
		bool blockedLeft;
		bool blockedRight;

		if (angle == 0.0)
		{
			eAICommandMove cmd = GetCommand_MoveAI();
			if (cmd)
			{
				blockedLeft = cmd.CheckBlockedLeft();
				blockedRight = cmd.CheckBlockedRight();
			}

			vector transform[4];
			GetTransform(transform);

			if (!blockedLeft)
				blockedLeft = m_PathFinding.IsBlocked(transform[3], transform[3] + (-0.5 * transform[0]));

			if (!blockedRight)
				blockedRight = m_PathFinding.IsBlocked(transform[3], transform[3] + (0.5 * transform[0]));

		#ifdef DIAG
			if (blockedLeft)
				Expansion_DebugObject(9999, transform[3] + (-0.5 * transform[0]) + "0 1.5 0", "ExpansionDebugBox_Red", GetDirection());
			else
				Expansion_DebugObject(9999, "0 0 0", "ExpansionDebugBox_Red", GetDirection());

			if (blockedRight)
				Expansion_DebugObject(10000, transform[3] + (0.5 * transform[0]) + "0 1.5 0", "ExpansionDebugBox_Blue", GetDirection());
			else
				Expansion_DebugObject(10000, "0 0 0", "ExpansionDebugBox_Blue", GetDirection());
		#endif

			if (blockedLeft && blockedRight)
			{
				if (!allowBackPedaling)
					return;

				//! Backpedal
				m_SideStepAngle = -180;
			}
			else if (blockedLeft || (!blockedRight && Math.RandomIntInclusive(0, 1)))
			{
				//! Go right if only blocked left or 50% chance go right if neither blocked left/right
				m_SideStepAngle = 90;
			}
			else
			{
				//! Go left
				m_SideStepAngle = -90;
			}
		}
		else
		{
			m_SideStepAngle = angle;
		}

		//! Move, b*tch, get out the way :-)
		m_eAI_SideStepTimeout = duration;

		EXTrace.Print(EXTrace.AI, this, "sidestep " + m_SideStepAngle + " " + obj);
		OverrideMovementDirection(true, m_SideStepAngle);
	}

	void eAI_CancelSidestep()
	{
		m_eAI_SideStepTimeout = 0.0;
		m_SideStepAngle = 0.0;
		OverrideMovementDirection(true, 0.0);
	}

	bool eAI_IsSideStepping()
	{
		return m_eAI_SideStepTimeout > 0.0;
	}

	override bool eAI_IsSideSteppingObstacles()
	{
		eAICommandMove cmd = GetCommand_MoveAI();
		return cmd && cmd.IsSideSteppingObstacles();
	}

	void eAI_ItemThreatOverride(ItemBase item, bool state)
	{
		if (state)
			m_eAI_ItemThreatOverride[item] = true;
		else
			m_eAI_ItemThreatOverride.Remove(item);
	}

	bool eAI_GetItemThreatOverride(ItemBase item)
	{
		return m_eAI_ItemThreatOverride[item];
	}

	bool eAI_HasLOS()
	{
		auto target = GetTarget();

		if (!target)
			return false;

		if (target.info.IsInherited(eAIItemTargetInformation))
			return true;

		auto state = m_eAI_TargetInformationStates[target.info];
		if (!state)
			return false;

		return state.m_LOS;
	}

	void eAI_SetIsFightingFSM(bool state)
	{
	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "SetIsFightingFSM " + state);
	#endif
		m_eAI_IsFightingFSM = state;
	}

	override void OnScheduledTick(float deltaTime)
	{
		if (!IsPlayerSelected() || !IsAlive())
			return;

		if (m_ModifiersManager)
			m_ModifiersManager.OnScheduledTick(deltaTime);
		//if (m_NotifiersManager)
			//m_NotifiersManager.OnScheduledTick();
		//! These send RPC to identity which is NULL for AI and thus affects all actual players HUDs (shows AI values instead of player)
		// if( m_TrasferValues )
			//m_TrasferValues.OnScheduledTick(deltaTime);
		// if( m_VirtualHud )
			//m_VirtualHud.OnScheduledTick();
		if (GetBleedingManagerServer())
			GetBleedingManagerServer().OnTick(deltaTime);
		if ( m_Environment )
			m_Environment.Update(deltaTime);

		// Check if electric device needs to be unplugged
		ItemBase heldItem = GetItemInHands();
		if (heldItem && heldItem.HasEnergyManager() && heldItem.GetCompEM().IsPlugged())
		{
			// Now we know we are working with an electric device which is plugged into a power source.
			EntityAI placed_entity = heldItem;

			// Unplug the device when the player is too far from the power source.
			placed_entity.GetCompEM().UpdatePlugState();
		}

		m_Expansion_UpdateTime += deltaTime;
		if ( m_Expansion_UpdateTime > m_Expansion_UpdateTimeThreshold )
		{
			m_Expansion_UpdateTime = 0;
			Expansion_OnAIUpdate();
		}

	}

	// We should integrate this into ReloadWeapon
	void ReloadWeaponAI(EntityAI weapon, EntityAI magazine)
	{
		EXTrace trace;

		// CF_Log.Debug(this.ToString() + "(DayZPlayerInstanceType." + GetInstanceType().ToString() + ") is trying to reload " + magazine.ToString() + " into " + weapon.ToString());
		eAIActionManager mngr_ai;
		CastTo(mngr_ai, GetActionManager());

		if (mngr_ai && FirearmActionLoadMultiBulletRadial.Cast(mngr_ai.GetRunningAction()))
		{
			trace = EXTrace.Start0(EXTrace.AI, this, "Interrupting current reload action for " + weapon);
			mngr_ai.Interrupt();
		}
		else if (GetHumanInventory().GetEntityInHands() != magazine)
		{
			Weapon_Base wpn;
			Magazine mag;
			Class.CastTo(wpn, weapon);
			Class.CastTo(mag, magazine);
			int mi = wpn.GetCurrentMuzzle();
			if (GetWeaponManager().CanUnjam(wpn))
			{
				//! NEVER unjam while reloading, unjamming should ALWAYS be handled by unjamming state in FSM so that weapon manager timeout can be dealt with
				trace = EXTrace.Start0(EXTrace.AI, this, "Not reloading jammed " + wpn);
			}
			else if (wpn.IsChamberFiredOut(mi) && wpn.GetInternalMagazineCartridgeCount(mi) > 0 && GetWeaponManager().CanEjectBullet(wpn))
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Ejecting bullet and chambering from internal mag " + wpn);
				GetWeaponManager().EjectBullet();
			}
			else if (GetWeaponManager().CanAttachMagazine(wpn, mag))
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Attaching mag to " + wpn + " " + mag);
				GetWeaponManager().AttachMagazine(mag);
			}
			else if (GetWeaponManager().CanSwapMagazine(wpn, mag))
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Swapping mag " + wpn + " " + mag);
				GetWeaponManager().SwapMagazine(mag);
			}
			else if (GetWeaponManager().CanLoadBullet(wpn, mag))
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Loading bullet " + wpn + " " + mag);
				// GetWeaponManager().LoadMultiBullet(mag);

				ActionTarget atrg = new ActionTarget(mag, this, -1, vector.Zero, -1.0);
				if (mngr_ai && !mngr_ai.GetRunningAction() && mngr_ai.GetAction(FirearmActionLoadMultiBulletRadial).Can(this, atrg, wpn))
					mngr_ai.PerformActionStart(mngr_ai.GetAction(FirearmActionLoadMultiBulletRadial), atrg, wpn);
			}
			else if (GetWeaponManager().CanEjectBullet(wpn))
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Ejecting bullet/cycling action " + wpn);
				GetWeaponManager().EjectBullet();
			}
			else
			{
				trace = EXTrace.Start0(EXTrace.AI, this, "Can't reload " + wpn + " - not implemented");
			}
		}
	}

	override void QuickReloadWeapon(EntityAI weapon)
	{
		ReloadWeaponAI(weapon, GetMagazineToReload(weapon));
	}

	// Returns true only if there is a weapon in hands, and the weapon has no rounds ready,
	// and the inventory does have magazines to reload to.
	bool ShouldReload(out EntityAI magazine, out EntityAI weapon)
	{
		Weapon weapon_in_hands;
		if (!Class.CastTo(weapon_in_hands, GetItemInHands()))
			return false;

		int mi = weapon_in_hands.GetCurrentMuzzle();
		Magazine mag = weapon_in_hands.GetMagazine(mi);
		if (mag && mag.GetAmmoCount() > 0)
			return false;

		magazine = GetMagazineToReload(weapon_in_hands);
		weapon = weapon_in_hands;
		if (!magazine || !weapon)
			return false;

		return true;
	}

#ifdef CF_DEBUG
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("FSM", m_FSM);
		if (m_FSM)
		{
			instance.IncrementTab();
			instance.Add(m_FSM);
			instance.DecrementTab();
		}

		return true;
	}
#endif

	//! @note: do not call frequently
	void Expansion_OnAIUpdate()
	{
		Expansion_UpdateVisibility();
	}

	void Expansion_TryTurningOnAnyLightsOrNVG(out float nightVisibility, bool skipNonNVG = false, bool skipNVG = false)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);

		ItemBase itembs;
		
		itembs = GetItemOnHead();
		if ( itembs && itembs.Expansion_TryTurningOnAnyLightsOrNVG(nightVisibility, this, skipNonNVG, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.Insert(itembs);
		
		itembs = GetItemOnSlot("Eyewear");
		if ( itembs && itembs.Expansion_TryTurningOnAnyLightsOrNVG(nightVisibility, this, skipNonNVG, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.Insert(itembs);

		itembs = GetItemInHands();
		if ( itembs && itembs.Expansion_TryTurningOnAnyLightsOrNVG(nightVisibility, this, skipNonNVG, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.Insert(itembs);

		m_Expansion_TriedTurningOnVisibilityEnhancers = true;
	}

	void Expansion_TryTurningOffAnyLightsOrNVG(bool skipNVG = false)
	{
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);

		ItemBase itembs;
		
		itembs = GetItemOnHead();
		if ( itembs && itembs.Expansion_TryTurningOffAnyLightsOrNVG(this, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.RemoveItemUnOrdered(itembs);
		
		itembs = GetItemOnSlot("Eyewear");
		if ( itembs && itembs.Expansion_TryTurningOffAnyLightsOrNVG(this, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.RemoveItemUnOrdered(itembs);

		itembs = GetItemInHands();
		if ( itembs && itembs.Expansion_TryTurningOffAnyLightsOrNVG(this, skipNVG) )
			m_Expansion_ActiveVisibilityEnhancers.RemoveItemUnOrdered(itembs);
	}

	void Expansion_UpdateVisibility(bool force = false)
	{
		float fogVisibility;
		float overcastVisibility;
		float rainVisibility;
		float daylightVisibility = g_Game.GetWorld().GetSunOrMoon();  //! 0/1 Night/Day

		m_Environment.Expansion_GetWeatherVisibility(fogVisibility, overcastVisibility, rainVisibility);
		if (!fogVisibility)
			EXPrint(this, "ERROR: Fog visibility is zero!");
		if (!overcastVisibility)
			EXPrint(this, "ERROR: Overcast visibility is zero!");
		if (!rainVisibility)
			EXPrint(this, "ERROR: Rain visibility is zero!");

		if (force || daylightVisibility != m_Expansion_DaylightVisibility)
		{
			m_Expansion_DaylightVisibility = daylightVisibility;
			if (daylightVisibility)
				m_Expansion_Visibility = 1.0;
			else
				m_Expansion_Visibility = 0.1;  //! 100 m

			m_Expansion_TriedTurningOnVisibilityEnhancers = false;
		}

		if (!daylightVisibility)
		{
			if (force || !m_Expansion_TriedTurningOnVisibilityEnhancers)
			{
				//! Try switching on any lights (only if threat lvl is above 0.4) or NVG at night
				Expansion_TryTurningOnAnyLightsOrNVG(m_Expansion_Visibility, m_eAI_CurrentThreatToSelfActive < 0.4);
				EXTrace.Print(EXTrace.AI, this, "nighttime base visibility " + m_Expansion_Visibility);
			}
		}
		else if (m_Expansion_ActiveVisibilityEnhancers.Count())
		{
			Expansion_TryTurningOffAnyLightsOrNVG();
		}

		if (!m_Expansion_Visibility)
			EXPrint(this, "ERROR: Base visibility is zero!");

		float visibilityLimit = m_eAI_ThreatDistanceLimit * 0.001;
		if (!visibilityLimit)
			EXPrint(this, "ERROR: Visibility limit is zero! Threat distance limit: " + m_eAI_ThreatDistanceLimit);
		if (visibilityLimit > 0 && m_Expansion_Visibility > visibilityLimit)
			m_Expansion_Visibility = visibilityLimit;

		//! Limit visibility in contaminated areas due to gas clouds
		if (m_Expansion_Visibility > 0.2 && GetModifiersManager().IsModifierActive(eModifiers.MDF_AREAEXPOSURE))
			m_Expansion_Visibility = 0.2;  //! 200 m

		//! Final visibility
		m_Expansion_Visibility = Math.Min(Math.Min(fogVisibility, Math.Min(overcastVisibility, rainVisibility)), m_Expansion_Visibility);
		m_Expansion_VisibilityDistThreshold = 900 * m_Expansion_Visibility;
	}

	float Expansion_GetVisibility(float distance)
	{
		if (distance > m_Expansion_VisibilityDistThreshold)
			return ExpansionMath.PowerConversion(1100 * m_Expansion_Visibility, m_Expansion_VisibilityDistThreshold, distance, 0.0, 1.0, 2.0);

		return 1.0;
	}

	float Expansion_GetVisibilityDistThreshold()
	{
		return m_Expansion_VisibilityDistThreshold;
	}

	override void HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_3(this, "HandleWeapons").Add(pDt).Add(pInHands).Add(pInputs);
#endif

		HumanCommandWeapons hcw = GetCommandModifier_Weapons();
		GetDayZPlayerInventory().HandleWeaponEvents(pDt, pExitIronSights);

		Weapon_Base weapon;
		Class.CastTo(weapon, pInHands);

		if (m_WeaponRaised != m_WeaponRaisedPrev)
		{
			m_WeaponRaisedPrev = m_WeaponRaised;
			m_WeaponRaisedTimer = 0.0;

			if (g_Game.IsServer())
				AnimSetBool(m_ExpansionST.m_VAR_Raised, m_WeaponRaised);
		}

		if (m_WeaponRaised || g_Game.IsClient())
		{
			if (g_Game.IsServer())
				m_WeaponRaisedTimer += pDt;

			vector aimTargetRelAngles = m_eAI_AimDirectionTarget_ModelSpace.VectorToAngles();

			//! We want to interpolate rel angles for aiming! Otherwise, if the conversion to rel angles happens later,
			//! there will be a sudden jump in the unit's rotation between 180 and -180 due to the way the animation is set up
			aimTargetRelAngles[0] = ExpansionMath.RelAngle(aimTargetRelAngles[0]);
			aimTargetRelAngles[1] = ExpansionMath.RelAngle(aimTargetRelAngles[1]);

			aimTargetRelAngles[1] = Math.Clamp(aimTargetRelAngles[1], -85.0, 85.0);  //! Valid range is [-85, 85]

			//TODO: quaternion slerp instead for better, accurate results
			m_eAI_AimRelAngles = ExpansionMath.InterpolateAngles(m_eAI_AimRelAngles, aimTargetRelAngles, pDt, Math.RandomFloat(3.0, 5.0), Math.RandomFloat(1.0, 3.0));

			GetAimingProfile().Update();
			vector direction = GetAimingProfile().GetAimDirection();
			vector orientation = direction.VectorToAngles();
			m_eAI_AimRelAngleLR = orientation[0];
			m_eAI_AimRelAngleUD = orientation[1];

		/*
		#ifndef SERVER
		#ifdef DIAG
			vector position = GetBonePositionWS(GetBoneIndexByName("neck"));

			vector points[2];
			points[0] = position;
			points[1] = position + (direction * 1000.0);
			m_Expansion_DebugShapes.Insert(Shape.CreateLines(COLOR_BLUE, ShapeFlags.VISIBLE, points, 2));
		#endif
		#endif
		*/

			if (g_Game.IsServer())
			{
				float dist = vector.Distance(GetBonePositionWS(GetBoneIndexByName("neck")), m_eAI_AimPosition_WorldSpace);
				dist = Math.Clamp(dist, 1.0, 360.0);

				float aimX = ExpansionMath.RelAngle(ExpansionMath.AbsAngle(m_eAI_AimRelAngles[0]) + (-15.0 / dist));
				float aimY = m_eAI_AimRelAngles[1];

				AnimSetFloat(m_ExpansionST.m_VAR_AimX, aimX);
				AnimSetFloat(m_ExpansionST.m_VAR_AimY, aimY);
			}
		}
		else if (g_Game.IsServer())
		{
			//! Interpolate to look direction if not raised so the next time we raise it animates in the direction we are looking

			vector lookTargetRelAngles = m_eAI_LookDirectionTarget_ModelSpace.VectorToAngles();

			lookTargetRelAngles[0] = ExpansionMath.RelAngle(lookTargetRelAngles[0]);
			lookTargetRelAngles[1] = ExpansionMath.RelAngle(lookTargetRelAngles[1]);

			lookTargetRelAngles[1] = Math.Clamp(lookTargetRelAngles[1], -85.0, 85.0);  //! Valid range is [-85, 85]

			m_eAI_AimRelAngles = ExpansionMath.InterpolateAngles(m_eAI_AimRelAngles, lookTargetRelAngles, pDt, Math.RandomFloat(3.0, 5.0), Math.RandomFloat(1.0, 3.0));
		}
	}

	// As with many things we do, this is an almagomation of the client and server code
	override void CheckLiftWeapon()
	{
		if (!GetGame().IsServer())
			return;

		Weapon_Base weap;
		if (Weapon_Base.CastTo(weap, GetItemInHands()))
		{
			m_LiftWeapon_player = weap.LiftWeaponCheck(this);
		}

		return;
	}

	bool CanRaiseWeapon()
	{
		if (IsClimbing() || IsFalling())
			return false;

		return m_eAI_SideStepTimeout <= 0;
	}

	// @param true to put weapon up, false to lower
	void RaiseWeapon(bool up)
	{
		m_WeaponRaised = up;
		if (!up)
			m_eAI_QueuedShots = 0;
	}

	override bool IsRaised()
	{
		return m_WeaponRaised;
	}

	override bool IsWeaponRaiseCompleted()
	{
		return m_WeaponRaisedTimer > 0.5;
	}

	void eAI_AddInteractingPlayer(Man player)
	{
		m_eAI_InteractingPlayers.Insert(player);
	}

	void eAI_RemoveInteractingPlayer(Man player)
	{
#ifdef DAYZ_1_20
		int remove_index = m_eAI_InteractingPlayers.Find(player);
		if (remove_index >= 0)
			m_eAI_InteractingPlayers.Remove(remove_index);
#else
		//! 1.21+
		m_eAI_InteractingPlayers.RemoveItem(player);
#endif
	}

	bool eAI_HasInteractingPlayers()
	{
		return m_eAI_InteractingPlayers.Count() > 0;
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(eAIActionTakeItemToHands, InputActionMap);
		AddAction(eAIActionTakeItem, InputActionMap);
		AddAction(eAIActionDropItem, InputActionMap);
	}

	ActionBase StartAction(typename actionType, ActionTarget target, ItemBase mainItem = null)
	{
		ActionBase action = m_eActionManager.GetAction(actionType);

		if (!mainItem)
			mainItem = GetItemInHands();

		m_eActionManager.PerformActionStart(action, target, mainItem);

		return action;
	}

	ActionBase StartActionObject(typename actionType, Object target, ItemBase mainItem = null)
	{
		return StartAction(actionType, new ActionTarget(target, null, -1, vector.Zero, -1.0), mainItem);
	}

	// @param LookWS a position in WorldSpace to look at
	void LookAtPosition(vector pPositionWS, bool recalculate = true)
	{
		m_eAI_LookPosition_WorldSpace = pPositionWS;
		m_eAI_LookDirection_Recalculate = recalculate;
	}

	// @param AimWS a position in WorldSpace to Aim at
	void AimAtPosition(vector pPositionWS, bool recalculate = true)
	{
		m_eAI_AimPosition_WorldSpace = pPositionWS;
		m_eAI_AimDirection_Recalculate = recalculate;
	}

	void LookAtDirection(vector pDirectionMS)
	{
		m_eAI_LookDirectionTarget_ModelSpace = pDirectionMS;
		m_eAI_LookDirection_Recalculate = false;
	}

	void AimAtDirection(vector pDirectionMS)
	{
		m_eAI_AimDirectionTarget_ModelSpace = pDirectionMS;
		m_eAI_AimDirection_Recalculate = false;
	}

	vector GetLookRelAngles()
	{
		return m_eAI_LookRelAngles;
	}

	vector GetLookDirection()
	{
		return m_eAI_LookRelAngles.AnglesToVector().Multiply3(m_ExTransformPlayer);
	}

	bool GetLookDirectionRecalculate()
	{
		return m_eAI_LookDirection_Recalculate;
	}

	//! Used to calculate aim in aiming profile
	vector GetAimRelAngles()
	{
		return m_eAI_AimRelAngles;
	}

	vector GetAimDirection()
	{
		return m_eAI_AimRelAngles.AnglesToVector().Multiply3(m_ExTransformPlayer);
	}

	//! Only valid while weapon raised
	vector GetWeaponAimDirection()
	{
		vector aimRelAngles = Vector(m_eAI_AimRelAngleLR, m_eAI_AimRelAngleUD, 0.0);
		return aimRelAngles.AnglesToVector();
	}

	override vector Expansion_GetAimDirection()
	{
		return GetAimDirection();
	}

	override vector GetAimPosition()
	{
		return m_eAI_AimPosition_WorldSpace;
	}

	bool GetAimDirectionRecalculate()
	{
		return m_eAI_AimDirection_Recalculate;
	}

	override bool AimingModel(float pDt, SDayZPlayerAimingModel pModel)
	{
		return false;
	}

	override bool HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		if (GetCommand_VehicleAI())
		{
			m_fLastHeadingDiff = 0;

			return false;
		}

		pModel.m_fHeadingAngle = GetOrientation()[0] * Math.DEG2RAD;
		pModel.m_fOrientationAngle = GetOrientation()[0] * Math.DEG2RAD;
		//Print(pModel.m_fHeadingAngle);
		//GetMovementState(m_MovementState);
		//return DayZPlayerImplementHeading.RotateOrient(pDt, pModel, m_fLastHeadingDiff);
		return true;
	}

	override void OnUnconsciousUpdate(float pDt, int last_command)
	{
		m_UnconsciousTime += pDt;

		if (GetGame().IsServer())
		{
			int shock_simplified = SimplifyShock();

			if (m_ShockSimplified != shock_simplified)
			{
				m_ShockSimplified = shock_simplified;
				SetSynchDirty();
			}

			if (m_UnconsciousTime > PlayerConstants.UNCONSCIOUS_IN_WATER_TIME_LIMIT_TO_DEATH && last_command == DayZPlayerConstants.COMMANDID_SWIM)
			{
				SetHealth("", "", -100);
			}
		}
	}

	//! Hack to make AI responsive again when weapon manager or action bugs out.
	//! Starts the damage hit animation (fullbody forward) w/o actually applying any damage, afterwards it should be fine again.
	//! @note (for documentation purposes) other tried ands tested workarounds that are less desirable:
	//! - Send AI uncon and wake it up again (more awkward and takes more time)
	//! - Let AI fall a short distance (unreliable in fixing the bug)
	void eAI_Unbug(string what)
	{

#ifdef DIAG
		string msg = "Action timed out for " + Debug.GetDebugName(this) + " while trying to " + what + " " + Debug.GetDebugName(GetItemInHands());
		ExpansionNotification("ACTION TIMEOUT", msg).Error();
#endif

		EXTrace.Print(true, this, "Applying SMACK OF GOD (='-')-o )'~')");
		StartCommand_Damage(1, 180);
	}

	override void OnUnconsciousStart()
	{
		eAI_DropItemInHandsImpl();

		super.OnUnconsciousStart();
	}

	void eAI_DropItemInHands(bool switchOff = true)
	{
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands)
			eAI_DropItem(itemInHands, switchOff);
	}

	void eAI_DropItemInHandsImpl()
	{
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands)
			eAI_DropItemImpl(itemInHands);
	}

	void eAI_DropItem(ItemBase item, bool switchOff = true)
	{
		if (switchOff)
			item.Expansion_TryTurningOffAnyLightsOrNVG(this);

		StartAction(eAIActionDropItem, null, item);

		m_Expansion_ActiveVisibilityEnhancers.RemoveItemUnOrdered(item);

		if (switchOff)
			Expansion_UpdateVisibility(true);
	}

	void eAI_DropItemImpl(ItemBase item)
	{
		InventoryLocation il_dst = new InventoryLocation();

		GameInventory.SetGroundPosByOwner(this, item, il_dst);

		//eAI_TakeItemToLocation(item, il_dst);
		Expansion_CloneItemToLocation(item, il_dst);
	}

	bool eAI_TakeItemToHands(ItemBase item)
	{
		bool result;

		if (item.GetHierarchyRootPlayer() == this)
			result = eAI_TakeItemToHandsImpl(item);
		else if (StartActionObject(eAIActionTakeItemToHands, item))
			result = true;

		Expansion_UpdateVisibility(true);

		return result;
	}

	bool eAI_TakeItemToHandsImpl(ItemBase item)
	{
		InventoryLocation il_dst = new InventoryLocation();

		il_dst.SetHands(this, item);

		//return eAI_TakeItemToLocation(item, il_dst);
		if (Expansion_CloneItemToLocation(item, il_dst))
			return true;

		return false;
	}

	bool eAI_FindFreeInventoryLocationFor(ItemBase item, FindInventoryLocationType flags = 0, out InventoryLocation il_dst = null)
	{
		/*!
		 * ATTACHMENT          = 4
		 * CARGO               = 8
		 * HANDS               = 16
		 * PROXYCARGO          = 32
		 * ANY_CARGO           = 40 (CARGO | PROXYCARGO)
		 * ANY                 = 60 (ATTACHMENT | ANY_CARGO | HANDS)
		 * NO_SLOT_AUTO_ASSIGN = 64
		 */
		if (!flags)
			flags = FindInventoryLocationType.ATTACHMENT | FindInventoryLocationType.ANY_CARGO;

		il_dst = new InventoryLocation();

		return GetInventory().FindFreeLocationFor(item, flags, il_dst);
	}

	bool eAI_TakeItemToInventory(ItemBase item, FindInventoryLocationType flags = 0)
	{
		InventoryLocation il_dst;

		if (!eAI_FindFreeInventoryLocationFor(item, flags, il_dst))
			return false;

		item.Expansion_TryTurningOffAnyLightsOrNVG(this);

		bool result;

		if (item == GetItemInHands())
		{
			//result = eAI_TakeItemToLocation(item, il_dst);
			if (Expansion_CloneItemToLocation(item, il_dst))
				result = true;
		}
		else if (StartActionObject(eAIActionTakeItem, item))
		{
			result = true;
		}

		if (result)
			m_Expansion_ActiveVisibilityEnhancers.RemoveItemUnOrdered(item);

		Expansion_UpdateVisibility(true);

		return result;
	}

	bool eAI_TakeItemToInventoryImpl(ItemBase item, FindInventoryLocationType flags = 0)
	{
		InventoryLocation il_dst;

		if (!eAI_FindFreeInventoryLocationFor(item, flags, il_dst))
			return false;

		//return eAI_TakeItemToLocation(item, il_dst);
		if (Expansion_CloneItemToLocation(item, il_dst))
			return true;

		return false;
	}

	bool eAI_TakeItemToLocation(ItemBase item, InventoryLocation il_dst)
	{
		InventoryLocation il_src = new InventoryLocation();

		item.GetInventory().GetCurrentInventoryLocation(il_src);

		if (il_src.CompareLocationOnly(il_dst) && il_src.GetFlip() == il_dst.GetFlip())
			return false;

		if (il_dst.GetType() != InventoryLocationType.GROUND)
		{
			if (il_dst.GetType() == InventoryLocationType.HANDS || il_src.GetType() == InventoryLocationType.HANDS)
			{
				//! Forcing switch to HumanCommandMove before taking to/from hands,
				//! and hiding/showing item in hands after, unbreaks hand anim state
				if (!GetCommand_Move())
					StartCommand_Move();
				else
					m_eAI_CommandTime = 0.0;
			}

			if (il_src.GetType() == InventoryLocationType.HANDS)
			{
				//! Forcing switch to HumanCommandMove before taking from hands,
				//! and hiding item in hands before, unbreaks hand anim state
				GetItemAccessor().HideItemInHands(true);
			}
		}

		if (GetGame().IsMultiplayer())
			GetGame().RemoteObjectTreeDelete(item);

		bool result = LocalTakeToDst(il_src, il_dst);

		if (il_src.GetType() == InventoryLocationType.HANDS && il_dst.GetType() != InventoryLocationType.GROUND)
		{
			//! Forcing switch to HumanCommandMove before taking from hands,
			//! and showing item in hands after, unbreaks hand anim state
			GetItemAccessor().HideItemInHands(false);
		}

		if (result)
		{
			if (il_dst.GetType() == InventoryLocationType.HANDS)
			{
				//! Forcing switch to HumanCommandMove before taking to hands,
				//! and hiding/showing item in hands after, unbreaks hand anim state
				GetItemAccessor().HideItemInHands(true);
				GetItemAccessor().HideItemInHands(false);
			}
			else if (il_src.GetType() != InventoryLocationType.GROUND && il_dst.GetType() == InventoryLocationType.GROUND)
			{
				ItemBase itemBs;
				if (Class.CastTo(itemBs, item))
				{
					eAI_RemoveItem(itemBs);

					if (!m_Expansion_CanBeLooted && !itemBs.m_Expansion_IsOwnerPlayer)
					{
						ExpansionItemBaseModule.SetLootable(item, false);
						item.SetLifetimeMax(120);  //! Make sure it despawns quickly when left alone
					}
				}
			}
		}

		if (GetGame().IsMultiplayer())
			GetGame().RemoteObjectTreeCreate(item);

		return result;
	}

	bool Expansion_GetUp(bool force = false)
	{
		if (!force)
		{
			if (IsPlayerInStance(DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_RAISEDERECT))
				return false;
		}

		return OverrideStance(DayZPlayerConstants.STANCEIDX_ERECT, force);
	}

	//! @brief Set emote for playing
	//! Setting autoCancel to true will automatically cancel the emote if it is a static pose
	//! @note Emote will not play instantly! Will play when AI FSM can transition to PlayEmote state
	void Expansion_SetEmote(int emoteID, bool autoCancel = false, int autoCancelDelay = 0)
	{
		m_Expansion_EmoteID = emoteID;
		m_Expansion_EmoteAutoCancel = autoCancel;
		m_Expansion_EmoteAutoCancelDelay = autoCancelDelay;
	}

	void Expansion_PlayEmote()
	{
		if (!m_Expansion_EmoteID)
			return;

		if (!m_EmoteManager.IsEmotePlaying() && m_EmoteManager.CanPlayEmote(m_Expansion_EmoteID))
		{
			m_EmoteManager.PlayEmote(m_Expansion_EmoteID);
			if (m_Expansion_EmoteAutoCancel)
			{
				int delay;
				switch (m_Expansion_EmoteID)
				{
					//! These emotes are temporary poses and don't need to be canceled
					case EmoteConstants.ID_EMOTE_TAUNTKISS:
					case EmoteConstants.ID_EMOTE_TAUNTELBOW:
					case EmoteConstants.ID_EMOTE_THROAT:
					case EmoteConstants.ID_EMOTE_DANCE:
					case EmoteConstants.ID_EMOTE_DABBING:
					case EmoteConstants.ID_EMOTE_CLAP:
					case EmoteConstants.ID_EMOTE_TAUNTTHINK:
					case EmoteConstants.ID_EMOTE_MOVE:
					case EmoteConstants.ID_EMOTE_DOWN:
					case EmoteConstants.ID_EMOTE_COME:
					case EmoteConstants.ID_EMOTE_NOD:
					case EmoteConstants.ID_EMOTE_SHAKE:
					case EmoteConstants.ID_EMOTE_SHRUG:
					case EmoteConstants.ID_EMOTE_VOMIT:
						break;

					//! These emotes are static poses with looping animation
					case EmoteConstants.ID_EMOTE_SOS:
						if (m_Expansion_EmoteAutoCancelDelay)
							delay = m_Expansion_EmoteAutoCancelDelay;
						else
							delay = 4000;
						break;

					//! Assume static pose
					default:
						if (m_Expansion_EmoteAutoCancelDelay)
							delay = m_Expansion_EmoteAutoCancelDelay;
						else
							delay = 2000;
						break;
				}
				if (delay)
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_EmoteManager.ServerRequestEmoteCancel, delay);
			}
		}

		if (m_Expansion_EmoteAutoCancel)
			m_Expansion_EmoteID = 0;
	}

	override vector Expansion_GetHeadingVector()
	{
		return Vector(GetOrientation()[0], 0, 0).AnglesToVector();
	}

	override float Expansion_GetMovementSpeed()
	{
		eAICommandMove cmd = GetCommand_MoveAI();
		if (cmd)
		{
			float speed = cmd.GetCurrentMovementSpeed();
			if (speed > 1.0 && m_WeaponRaised)
				speed = 1.0;
			return speed;
		}

		return 0.0;
	}

	override float Expansion_GetMovementAngle()
	{
		eAICommandMove cmd = GetCommand_MoveAI();
		if (cmd)
			return cmd.GetCurrentMovementAngle();

		return 0.0;
	}

	bool HandleVaulting(float pDt)
	{
		if (m_eAI_PositionIsFinal && Expansion_GetMovementSpeed() == 0.0)
			return false;

		//if (!m_PathFinding.IsVault())
		//{
		//	return false;
		//}

		SHumanCommandClimbSettings hcls = GetDayZPlayerType().CommandClimbSettingsW();
		
		if ( m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE )
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;

		SetOrientation(GetOrientation());
		HumanCommandClimb.DoClimbTest(this, m_ExClimbResult, 0);

		if (m_ExClimbResult.m_bIsClimb || m_ExClimbResult.m_bIsClimbOver)
			return true;

		//! As we are essentially using Zombie pathfinding, we may encounter situations where the path will go through a fence
		//! that Zs would be able to jump (e.g. wall_indfnc_9.p3d) but player AI would not due to HumanCommandClimb.DoClimbTest not letting us.
		//! Use ExpansionClimb.DoClimbTest with alwaysAllowClimb = true instead.
		if (m_PathFinding.m_IsBlocked)
		{
			ExpansionClimb.DoClimbTest(this, m_ExClimbResult, true);
			if (m_ExClimbResult.m_bIsClimb || m_ExClimbResult.m_bIsClimbOver)
				return true;
		}

		return false;
	}

	override bool CanJump()
	{
		//! Following is equivalent to vanilla PlayerBase::CanJump

		if (GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS)
		{	
			return false;
		}
		
		if (!CanConsumeStamina(EStaminaConsumers.JUMP))
			return false;

		//! disables jump when player is significantly injured
		if (m_InjuryHandler && m_InjuryHandler.GetInjuryAnimValue() >= InjuryAnimValues.LVL3)
			return false;
		
		if (IsInFBEmoteState() || m_EmoteManager.m_MenuEmote)
		{
			return false;
		}

		//! Following is roughly equivalent to vanilla DayZPlayerImplement::CanJump

		if (IsFBSymptomPlaying() || IsRestrained() || IsUnconscious())
			return false;
		
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			return false;
		
		HumanItemBehaviorCfg hibcfg = GetItemAccessor().GetItemInHandsBehaviourCfg();
		if (!hibcfg.m_bJumpAllowed)
			return false;
		
		if (!DayZPlayerUtils.PlayerCanChangeStance(this, DayZPlayerConstants.STANCEIDX_ERECT) || !DayZPlayerUtils.PlayerCanChangeStance(this, DayZPlayerConstants.STANCEIDX_RAISEDERECT))
			return false;
		
		return true;
	}

	override bool CanClimb(int climbType, SHumanCommandClimbResult climbRes)
	{
		//! Following is equivalent to vanilla PlayerBase::CanClimb

		if (GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			return false;
		}
			
		if (climbType == 1 && !CanConsumeStamina(EStaminaConsumers.VAULT))
			return false;
		
		if (climbType == 2 && (!CanConsumeStamina(EStaminaConsumers.CLIMB) || GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS))
			return false;

		if (climbType > 0 && m_InjuryHandler && m_InjuryHandler.GetInjuryAnimValue() >= InjuryAnimValues.LVL3)
			return false;

		//! Following is roughly equivalent to vanilla DayZPlayerImplement::CanClimb

		if (IsFBSymptomPlaying() || IsRestrained() || IsUnconscious() || IsInFBEmoteState())
			return false;
		
		if (m_MovementState.IsInProne() || m_MovementState.IsInRaisedProne())
			return false;

		HumanItemBehaviorCfg hibcfg = GetItemAccessor().GetItemInHandsBehaviourCfg();
		if (!hibcfg.m_bJumpAllowed)
			return false;

		if (climbRes)
		{
			IEntity standPointParent;
			vector standPoint;
			vector checkPosition;
			vector checkDirection;

			if (climbRes.m_ClimbOverStandPoint != vector.Zero)
			{
				standPointParent = climbRes.m_ClimbOverStandPointParent;
				standPoint = climbRes.m_ClimbOverStandPoint;
			}
			else if (climbRes.m_ClimbStandPoint != vector.Zero)
			{
				standPointParent = climbRes.m_ClimbStandPointParent;
				standPoint = climbRes.m_ClimbStandPoint;
			}
			else
			{
				checkDirection = GetDirection() * 1.5;
			}

			if (standPoint != vector.Zero)
			{
				vector unitPosition = GetPosition();
				Object standPointObject;
				if (Class.CastTo(standPointObject, standPointParent))
				{
					//! Standpoint parent is object, local coordinates unequal world coordinates
					vector transform[4];
					standPointParent.GetTransform(transform);
					checkPosition = standPoint.Multiply4(transform);
				}
				else
				{
					//! No standpoint parent or part of world, local coordinates equal world coordinates
					checkPosition = standPoint;
				}
				checkPosition[1] = unitPosition[1];
				checkDirection = vector.Direction(unitPosition, checkPosition);
			}

			if (!eAI_IsFallSafe(checkDirection))
				return false;

			if (!eAI_CanClimbOn(climbRes.m_GrabPointParent, climbRes))
				return false;
			if (!eAI_CanClimbOn(climbRes.m_ClimbStandPointParent, climbRes))
				return false;
			if (!eAI_CanClimbOn(climbRes.m_ClimbOverStandPointParent, climbRes))
				return false;
		}

		return true;
	}

	bool eAI_CanClimbOn(IEntity parent, SHumanCommandClimbResult climbRes)
	{
		EntityAI entity;
		if (Class.CastTo(entity, parent) && entity.IsHologram())
			return false;

		Object object;
		if (Class.CastTo(object, parent))
		{ 
			if (object.IsTree() || object.IsBush() || object.IsMan())
				return false;

			BuildingBase building;
			if (Class.CastTo(building, object) && (!climbRes.m_bIsClimb || building.m_eAI_PreventClimb))
				return false;

			if (object.IsTransport() && climbRes.m_bIsClimbOver)
				return false;
		}

		if (EXTrace.AI && parent)
			EXTrace.Print(true, this, "eAI_CanClimbOn " + Debug.GetDebugName(parent));

		return true;
	}

	bool eAI_IsFallSafe(vector checkDirection)
	{
		vector position = GetPosition();
		vector checkPosition = position + checkDirection;
		vector begPos = Vector(checkPosition[0], checkPosition[1], checkPosition[2]);

		checkPosition[1] = GetGame().SurfaceY(checkPosition[0], checkPosition[2]);

		PhxInteractionLayers collisionLayerMask;
		collisionLayerMask |= PhxInteractionLayers.ROADWAY;
		collisionLayerMask |= PhxInteractionLayers.BUILDING;
		collisionLayerMask |= PhxInteractionLayers.FENCE;
		collisionLayerMask |= PhxInteractionLayers.VEHICLE;
		collisionLayerMask |= PhxInteractionLayers.TERRAIN;

		vector hitPosition;

		if (DayZPhysics.RayCastBullet(begPos, checkPosition, collisionLayerMask, this, null, hitPosition, null, null))
			checkPosition = hitPosition;

		float fallHeight = position[1] - checkPosition[1];

		//! https://feedback.bistudio.com/T173348
		bool isFallSafe;
		if (fallHeight <= DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW || (GetHealth01() - Math.InverseLerp(DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW, DayZPlayerImplementFallDamage.HEALTH_HEIGHT_HIGH, fallHeight) >= 0.7))
			isFallSafe = true;

		//EXPrint("position " + position + " checkDirection " + checkDirection + " " + checkDirection.VectorToAngles() + " checkPosition " + checkPosition + " " + isFallSafe);

		return isFallSafe;
	}

	void eAI_JumpOrClimb()
	{
		m_JumpClimb.JumpOrClimb();
	}

	void HandleBuildingDoors(float pDt)
	{
		//if (!m_PathFinding.IsDoor())
			//return;

		vector position = m_ExTransformPlayer[3] + (m_ExTransformPlayer[1] * 1.1);
		vector direction = m_ExTransformPlayer[2];

		float fwdBwd = 1.0;
		if (Math.AbsFloat(Expansion_GetMovementAngle()) > 135.0)
			fwdBwd = -1.0;

		vector p0 = position;
		vector p1 = position + (direction * 1.5 * fwdBwd);

		if (GetWeaponManager().IsRunning())
		{
		#ifdef DIAG
			Expansion_DebugObject(22222, "0 0 0", "ExpansionDebugBox_White");
			Expansion_DebugObject(22223, p1, "ExpansionDebugBox_Black", direction, position);
		#endif
			return;
		}

	#ifdef DIAG
		Expansion_DebugObject(22222, p1, "ExpansionDebugBox_White", direction, position);
		Expansion_DebugObject(22223, "0 0 0", "ExpansionDebugBox_Black");
	#endif

		RaycastRVParams params(p0, p1, this, 0.5);
		array<ref RaycastRVResult> results();
		array<Object> excluded();
		excluded.Insert(this);

		params.with = this;
		params.flags = CollisionFlags.ALLOBJECTS;
		params.type = ObjIntersectView;

		if (!DayZPhysics.RaycastRVProxy(params, results, excluded))
			return;

		int time = GetGame().GetTime();

		foreach (auto result : results)
		{
			BuildingBase building;
			if (!Class.CastTo(building, result.obj))
				continue;

			int doorIndex = building.GetDoorIndex(result.component);
			if (doorIndex == -1)
				continue;

			if (time - building.m_eAI_LastDoorInteractionTime[doorIndex] < 3000)
				continue;

			bool isStuck = false;
			bool isWreck = building.GetType().IndexOf("Land_Wreck_") == 0;  //! Vehicle wreck, excluding heli crashes

			if (building.IsDoorOpen(doorIndex))
			{
				if (!isWreck)
				{
					eAICommandMove move;
					if (Class.CastTo(move, m_eAI_Command))
						isStuck = move.CheckStuck();
					if (!isStuck)
						continue;
				}
			}
			else if (isWreck)
			{
				//! Do not open wreck doors (less chance of getting stuck on them when closed)
				continue;
			}
			else if (building.IsDoorLocked(doorIndex))
			{
				//! TODO: Break through locked doors
				continue;
			}

			/**
			 * TODO: Use FSM events to notify that the door should be opened. 
			 * TODO: Add support for animation action logic to FSM states
			 * TODO: FSM file just for doors. Opening, picking, breaking, closing. Vehicles and Buildings.
			 * 
			 *	GetFSM().Start("OpenDoor")
			 * 
			 * Events are stored in the transitions
			 */

			//! Decrease chance of AI getting stuck between wall and opened door by temporarily stopping before opening
			int speedLimit = m_MovementSpeedLimit;
			int speedLimitThreat = m_MovementSpeedLimitUnderThreat;
			int targetSpeedLimit;
			int delay = 650;
			if (isStuck)
			{
				targetSpeedLimit = 1;
				eAI_ForceSideStep(1.5, null, -180);
				delay = 1500;
			}
			if ((isStuck || !isWreck) && (speedLimit > targetSpeedLimit || speedLimitThreat > targetSpeedLimit))
			{
				SetMovementSpeedLimits(targetSpeedLimit, targetSpeedLimit);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetMovementSpeedLimits, delay, false, speedLimit, speedLimitThreat);
			}

			//! Always close wreck doors (less chance of getting stuck on them when closed)
			if (isStuck || isWreck)
			{
				building.CloseDoor(doorIndex);
			}
			else
			{
				building.OpenDoor(doorIndex);
			}
			
			ActionInteractBaseCB.Cast(AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW,ActionInteractBaseCB));

			building.m_eAI_LastDoorInteractionTime[doorIndex] = time;
			return;
		}
	}
};
