// Copyright 2021 William Bowers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

class eAIBase extends PlayerBase
{
	const int EAI_COMMANDID_MOVE = 1;

	static bool AI_HANDLEDOORS = true;
	static bool AI_HANDLEVAULTING = true;

	private static autoptr array<eAIBase> s_AllAI = new array<eAIBase>();

	private autoptr eAIFSM m_FSM;

	// Targeting data
	private autoptr array<eAITarget> m_eAI_Targets;
	float m_ThreatClearedTimeout;
	float m_eAI_CurrentThreatToSelf;
	float m_eAI_PreviousThreatToSelf;
	float m_eAI_CurrentThreatToSelfActive;
	float m_eAI_PreviousThreatToSelfActive;

	// Aiming and aim arbitration
	bool m_AimArbitration = false;
	private Man m_CurrentArbiter = null;

	// Command handling
	private ExpansionHumanCommandScript m_eAI_Command;
	private int m_eAI_CurrentCommandID;

	private bool m_eAI_UnconsciousVehicle;

	private Transport m_eAI_Transport;
	private int m_eAI_Transport_SeatIndex;

	private bool m_eAI_Melee;
	private bool m_eAI_MeleeDidHit;
	private float m_eAI_MeleeTimeout;

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

	private vector m_eAI_AimDirection_ModelSpace;
	private vector m_eAI_AimDirectionTarget_ModelSpace;
	private bool m_eAI_AimDirection_Recalculate;

	private eAIAimingState m_AimingState;

	private bool m_MovementSpeedActive;
	private int m_MovementSpeed;
	private int m_MovementSpeedLimit = 3;
	private int m_MovementSpeedLimitUnderThreat = 3;
	private bool m_MovementDirectionActive;
	private float m_MovementDirection;
	private float m_SideStepAngle;

	private bool m_WeaponRaised;
	private bool m_WeaponRaisedPrev;

	private float m_WeaponRaisedTimer;
	private float m_WeaponLowerTimeout;

	ref array<ItemBase> m_Weapons = {};
	ref array<ItemBase> m_MeleeWeapons = {};
	ref array<ItemBase> m_Bandages = {};

	ref map<typename, Magazine> m_eAI_EvaluatedFirearmTypes = new map<typename, Magazine>;

	private float m_eAI_SideStepTimeout;
	private bool m_eAI_LOS;

	private bool m_eAI_UnlimitedReload;

	// Path Finding
	private ref ExpansionPathHandler m_PathFinding;

	private Apple m_DebugTargetApple;
	private vector m_DebugTargetOrientation;

	float m_eAI_UpdateTargetsTick;
	autoptr array<Object> m_eAI_PotentialTargetObjects = new array<Object>();
	int m_eAI_CurrentPotentialTargetIndex;
	PlayerBase m_eAI_PotentialTargetPlayer;
	float m_eAI_UpdateNearTargetsTime;
	int m_eAI_UpdateNearTargetsCount;

	void eAIBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIBase");
#endif

#ifdef CF_DEBUG
		CF_Debug.Create(this);
#endif

		s_AllAI.Insert(this);

		AI_HANDLEVAULTING = GetExpansionSettings().GetAI().Vaulting;

		SetEventMask(EntityEvent.INIT);
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

		m_eAI_Targets = new array<eAITarget>();

		m_AimingProfile = new eAIAimingProfile(this);

		m_eMeleeCombat = new eAIMeleeCombat(this);

		m_MeleeCombat = m_eMeleeCombat;
		m_MeleeFightLogic = new DayZPlayerMeleeFightLogic_LightHeavy(this);

		m_WeaponManager = new eAIWeaponManager(this);
		m_ShockHandler = new eAIShockHandler(this);

		m_PathFinding = new ExpansionPathHandler(this);

		if (GetGame().IsServer())
		{
			SetGroup(eAIGroup.CreateGroup());

			LoadFSM();
		}

		LookAtDirection("0 0 1");
		AimAtDirection("0 0 1");

		//! Vehicles mod will set this in PlayerBase::Init if loaded
		if (!m_ExpansionST)
			m_ExpansionST = new ExpansionHumanST(this);
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

		if (IsAI() && GetGroup())
		{
			GetGroup().RemoveMember(GetGroup().GetIndex(this));
		}
	}

	protected override void EOnInit(IEntity other, int extra)
	{
		OnSelectPlayer();
	}

	override void OnSelectPlayer()
	{
		m_QuickBarBase.updateSlotsCount();

		m_PlayerSelected = true;

		m_WeaponManager.SortMagazineAfterLoad();

		//! add callbacks for ai target system
		SetAITargetCallbacks(new AITargetCallbacksPlayer(this));

		GetSoftSkillsManager().InitSpecialty(GetStatSpecialty().Get());
		GetModifiersManager().SetModifiers(true);

		SetSynchDirty();

		CheckForGag();

		m_eActionManager = new eAIActionManager(this);
		m_ActionManager = m_eActionManager;
	}

	// Used for deciding the best aim arbiter for the AI.
	// TODO: particle system
	Man GetNearestPlayer()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetNearestPlayer");
#endif

		array<Man> players = {};
		GetGame().GetPlayers(players);
		float min = 999999.0;
		float temp;
		Man closest = null;
		foreach (Man p : players)
		{
			temp = vector.DistanceSq(GetPosition(), p.GetPosition());
			if (temp < min)
			{
				min = temp;
				closest = p;
			}
		}
		return closest;
	}

	void StopAimArbitration()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "StopAimArbitration");
#endif

		m_AimingState = eAIAimingState.INACTIVE;
	}

	void UpdateAimArbitration()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "UpdateAimArbitration");
#endif

		m_AimingState = eAIAimingState.ACTIVE;
	}

	bool PlayerIsEnemy(EntityAI other)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "PlayerIsEnemy").Add(other);
#endif

		PlayerBase player = PlayerBase.Cast(other);
		if (!player)
			return true;

		if (player.IsInSafeZone())
			return false;

		if (!GetGroup())
			return true;

		if (player.GetGroup())
		{
			if (player.GetGroup() == GetGroup())
				return false;

			if (player.GetGroup().GetFaction().IsFriendly(GetGroup().GetFaction()))
			{
				if (GetGroup().GetFaction().IsInherited(eAIFactionPassive))
					return player.GetTargetInformation().IsTargetted(GetGroup());
				else
					return false;
			}

			//! If the other player is a guard and we are not a guard, consider them an enemy if WE raise our weapon
			//! (because then THEY will consider us a threat)
			if (player.GetGroup().GetFaction().IsGuard())
				return IsRaised();
		}

		// at this point we know both we and they have groups, and the groups aren't friendly towards each other
		return true;
	}

	int m_MinTimeTillNextFire;
	void TryFireWeapon()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "TryFireWeapon");
#endif

		if (GetGame().GetTime() < m_MinTimeTillNextFire)
			return;
		m_MinTimeTillNextFire = GetGame().GetTime() + 250.0;

		Weapon_Base weapon = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
		if (!weapon)
			return;

		if (GetDayZPlayerInventory().IsProcessing())
			return;
		if (!IsRaised())
			return;
		if (!weapon.CanFire())
			return;
		if (GetWeaponManager().IsRunning())
			return;

		int muzzleIndex = weapon.GetCurrentMuzzle();
		if (!weapon.CanFire(muzzleIndex))
			return;

		if (!m_eAI_LOS)
			return;

		GetWeaponManager().Fire(weapon);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!IsAI() || !GetGroup())
			return;

		if (EXTrace.AI)
		{
			string dmgTypeStr;
			switch (damageType)
			{
				case DT_CLOSE_COMBAT:
					dmgTypeStr = "DT_CLOSE_COMBAT";
					break;
				case DT_FIRE_ARM:
					dmgTypeStr = "DT_FIRE_ARM";
					break;
				case DT_EXPLOSION:
					dmgTypeStr = "DT_EXPLOSION";
					break;
				case DT_CUSTOM:
					dmgTypeStr = "DT_CUSTOM";
					break;
				default:
					dmgTypeStr = "UNKNOWN";
			}

			string msg = "EEHitBy ";
			msg += damageResult.GetDamage(dmgZone, "Health").ToString() + " ";
			msg += damageResult.GetDamage(dmgZone, "Blood").ToString() + " ";
			msg += damageResult.GetDamage(dmgZone, "Shock").ToString() + " ";
			msg += dmgTypeStr + " ";
			msg += source.ToString() + " ";
			msg += component.ToString() + " ";
			msg += dmgZone + " ";
			msg += ammo + " ";
			msg += modelPos.ToString() + " ";
			msg += speedCoef.ToString();
			EXTrace.Print(EXTrace.AI, this, msg);
		}

		ZombieBase zmb;
		if (Class.CastTo(zmb, source))
		{
			if (!zmb.GetTargetInformation().IsTargetted(GetGroup()))
			{
				zmb.GetTargetInformation().AddAI(this);
			}
		}

		PlayerBase player;
		if (Class.CastTo(player, source.GetHierarchyRootPlayer()))
		{
			if (!player.GetTargetInformation().IsTargetted(GetGroup()))
			{
				//! target the attacking player for upto 2 minutes
				player.GetTargetInformation().AddAI(this, 120000);
			}
		}

		AnimalBase animal;
		if (Class.CastTo(animal, source))
		{
			if (!animal.GetTargetInformation().IsTargetted(GetGroup()))
			{
				animal.GetTargetInformation().AddAI(this);
			}
		}
	}

	override bool IsAI()
	{
		return true;
	}

	ExpansionPathHandler GetPathFinding()
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
			if (bandage && bandage.GetHealth() > 0)
			{
				return bandage;
			}
		}

		return null;
	}

	ItemBase GetWeaponToUse(bool requireAmmo = false)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetWeaponToUse").Add(requireAmmo);
#endif

		// very messy :)
		Weapon gun;
		Magazine mag;
		foreach (ItemBase weapon: m_Weapons)
		{
			if (weapon && weapon.GetHealth() > 0)
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
			if (melee && melee.GetHealth() > 0)
			{
				return melee;
			}
		}

		return null;
	}

	//! Unlike GetMagazineToReload, this can be used to check if there is a mag/ammo for a weapon that is not in hands
	EntityAI GetMagazineInInventory(EntityAI weapon)
	{
		auto trace = EXTrace.Start(EXTrace.AI, this, weapon.ToString());

		Weapon_Base weapon_base = Weapon_Base.Cast(weapon);
		eAIWeaponManager weapon_manager = eAIWeaponManager.Cast(GetWeaponManager());
		EntityAI magazine_to_reload;

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

	bool IsInMelee()
	{
		return m_eAI_MeleeTimeout > 0;
	}

	void CreateAimingProfile()
	{
		if (GetGame().IsServer())
			return;

		if (m_AimingProfile)
			return;

		m_AimingProfile = new eAIAimingProfile(this);
	}

	void DestroyAimingProfile()
	{
		if (GetGame().IsServer())
			return;

		if (!m_AimingProfile)
			return;

		delete m_AimingProfile;
	}

	eAIAimingProfile GetAimingProfile()
	{
		return m_AimingProfile;
	}

	array<eAITarget> GetTargets()
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
#ifdef EAI_TRACE
		EXTrace.Print(EXTrace.AI, this, "OnAddTarget " + Debug.GetDebugName(target.GetEntity()) + " - target count " + m_eAI_Targets.Count());
#endif
	}

	void OnRemoveTarget(eAITarget target)
	{
		m_eAI_Targets.RemoveItem(target);
#ifdef EAI_TRACE
		EXTrace.Print(EXTrace.AI, this, "OnRemoveTarget " + Debug.GetDebugName(target.GetEntity()) + " - target count " + m_eAI_Targets.Count());
#endif
	}

	float GetThreatToSelf(bool ignoreLOS = false)
	{
		if (!m_eAI_LOS && !ignoreLOS)
			return m_eAI_CurrentThreatToSelfActive;

		return m_eAI_CurrentThreatToSelf;
	}

	void DetermineThreatToSelf(float pDt)
	{
		m_eAI_PreviousThreatToSelf = m_eAI_CurrentThreatToSelf;
		m_eAI_PreviousThreatToSelfActive = m_eAI_CurrentThreatToSelfActive;

		if (m_eAI_Targets.Count() > 0)
		{
			m_eAI_CurrentThreatToSelf = m_eAI_Targets[0].GetThreat(this);

			//! Make active threat level rise/fall instantly if LOS, rise/fall slowly if no LOS
			if (m_eAI_LOS)
				m_eAI_CurrentThreatToSelfActive = m_eAI_CurrentThreatToSelf;
			else
				m_eAI_CurrentThreatToSelfActive = Math.Lerp(m_eAI_CurrentThreatToSelfActive, 0.200001, pDt * pDt);
		}
		else
		{
			m_eAI_CurrentThreatToSelf = 0.0;
			m_eAI_CurrentThreatToSelfActive = 0.0;
		}
	}

	void ReactToThreatChange(float pDt)
	{
		if (m_eAI_CurrentThreatToSelf >= 0.2)
		{
			if (m_eAI_PreviousThreatToSelf < 0.2)
				EXTrace.Print(EXTrace.AI, this, "current threat to self >= 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");
		}
		else
		{
			if (m_eAI_PreviousThreatToSelf >= 0.2)
				EXTrace.Print(EXTrace.AI, this, "current threat to self < 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");
		}

		if (m_eAI_CurrentThreatToSelf >= 0.4)
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
					if (m_eAI_CurrentThreatToSelf <= 0.2 && !GetExpansionSettings().GetAI().Manners)
					{
						auto hands = GetItemInHands();
						if (GetWeaponManager().IsRunning() || (hands && (hands.IsInherited(BandageDressing) || hands.IsInherited(Rag))))
							return;

						ExpansionState state = m_FSM.GetState();
						if (state && state.GetName() != "Idle" && state.GetName() != "FollowFormation" && state.GetName() != "TraversingWaypoints")
							return;

						int emoteId;
						switch (Math.RandomInt(1, 5))
						{
							//! @note use only emotes that don't need to be canceled for simplicity sake
							//case 0:
								//emoteId = EmoteConstants.ID_EMOTE_TAUNT
								//break;
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
							//! @note dancing takes too long to be viable :-(
							//case 5:
								//emoteId = EmoteConstants.ID_EMOTE_DANCE
								//break;
						}
						//EXPrint(ToString() + " can play emote? " + m_EmoteManager.CanPlayEmote(emoteId));
						m_EmoteManager.PlayEmote(emoteId);
					}
				}
			}
		}

		if (IsRaised() && !GetWeaponManager().IsRunning())
		{
			//! During sidestep, CanRaiseWeapon will return false so we are able to lower weapon instantly
			if (!CanRaiseWeapon())
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
#ifdef EAI_TRACE
		auto trace = EXTrace.Start0(EXTrace.AI, this);

		int ticks;
		int elapsed;
#endif
		// TODO: use particle system instead
		// XXX: I have no clue what the above comment means -lava76

		if (!GetGroup())
			return;

		vector center = GetPosition();

		m_eAI_UpdateTargetsTick += pDt;
		if (m_eAI_CurrentPotentialTargetIndex >= m_eAI_PotentialTargetObjects.Count() && m_eAI_UpdateTargetsTick > Math.RandomFloat(0.1, 0.2))
		{
			m_eAI_UpdateTargetsTick = 0;
			m_eAI_CurrentPotentialTargetIndex = 0;

			//! Get animals/Zs in near range (30 m)

#ifdef EAI_TRACE
			ticks = TickCount(0);
#endif

			array<CargoBase> proxyCargos = new array<CargoBase>();
			GetGame().GetObjectsAtPosition(center, 30.0, m_eAI_PotentialTargetObjects, proxyCargos);

#ifdef EAI_TRACE
			elapsed = TickCount(ticks);
			float time = elapsed / 10000.0;
			m_eAI_UpdateNearTargetsTime += time;
			m_eAI_UpdateNearTargetsCount++;
			float timeAvg = m_eAI_UpdateNearTargetsTime / m_eAI_UpdateNearTargetsCount;

			EXTrace.Add(trace, "objects in near range " + m_eAI_PotentialTargetObjects.Count() + " time (ms) " + time + " timeAvg (ms) " + timeAvg);
#endif
		}

		//! Get other players/AI in extended range (1000 m radius) - check one player per tick (30 players per second)

#ifdef EAI_TRACE
		ticks = TickCount(0);
#endif

		PlayerBase player;
		if (m_eAI_PotentialTargetPlayer)
		{
			player = m_eAI_PotentialTargetPlayer.m_Expansion_NextPlayer;
		}

		if (!player)
		{
			player = s_AllPlayers;
		}

		m_eAI_PotentialTargetPlayer = player;

		Object playerObject = player;
		if (player && player != this && m_eAI_PotentialTargetObjects.Find(playerObject) == -1 && Math.IsPointInCircle(center, 1000, player.GetPosition()))
		{
			m_eAI_PotentialTargetObjects.Insert(playerObject);

#ifdef EAI_TRACE
			elapsed = TickCount(ticks);

			EXTrace.Add(trace, "player/AI in extended range " + player.GetType() + " time (ms) " + (elapsed / 10000.0).ToString());
#endif
		}

		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		ItemBase targetItem;

		float group_count = GetGroup().Count();

		int nonItemTargetCount;

		while (m_eAI_CurrentPotentialTargetIndex < m_eAI_PotentialTargetObjects.Count())
		{
			Object obj = m_eAI_PotentialTargetObjects[m_eAI_CurrentPotentialTargetIndex++];
			if (!obj)
				continue;

			PlayerBase playerThreat;
			if (Class.CastTo(playerThreat, obj))
				if (GetGroup() && GetGroup().IsMember(playerThreat))
					continue;

			//! If the object is an item and we have an entity in hands or the object is not a weapon, ignore it
			if (Class.CastTo(targetItem, obj) && (entityInHands || (!obj.IsWeapon() && !targetItem.Expansion_IsMeleeWeapon()) || GetGroup().GetFaction().IsInherited(eAIFactionPassive)))
				continue;

			if (obj.IsInherited(Building))
				continue;
			if (obj.IsInherited(Transport))
				continue;

			eAITargetInformation target = eAITargetInformation.GetTargetInformation(obj);
			if (!target)
				continue;

			if (!target.IsActive())
				continue;

			if ((targetItem && target.ShouldRemove(this)) || target.ShouldRemove())
				continue;

			int num_ai_in_group_targetting = 0;
			if (target.IsTargetted(GetGroup(), num_ai_in_group_targetting))
			{
				float threatLevel = target.GetThreat(this);
				float frac = (group_count - num_ai_in_group_targetting) / group_count;
				if ((frac * threatLevel) < (1.0 / group_count))
					continue;
			}

			if (!GetGroup().GetFaction().IsInherited(eAIFactionPassive) && (!playerThreat || !playerThreat.GetGroup() || !playerThreat.GetGroup().GetFaction().IsInherited(eAIFactionPassive)))
				target.AddAI(this);

			if (!obj.IsInherited(ItemBase))
				nonItemTargetCount++;

			if (nonItemTargetCount * 2 > group_count)
				break;
		}

		if (m_eAI_CurrentPotentialTargetIndex >= m_eAI_PotentialTargetObjects.Count())
			m_eAI_PotentialTargetObjects.Clear();
	}

	// TODO: Use CF_PriorityQueue<T>
	void PrioritizeTargets()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "PrioritizeTargets");
#endif

		// sorting the targets so the highest the threat is indexed lowest

		for (int i = m_eAI_Targets.Count() - 1; i >= 0; i--)
		{
			if (m_eAI_Targets[i] == null || m_eAI_Targets[i].ShouldRemove(this))
			{
#ifdef EAI_TRACE
				EXTrace.Print(EXTrace.AI, this, "PrioritizeTargets - removing target " + Debug.GetDebugName(m_eAI_Targets[i].GetEntity()));
#endif
				m_eAI_Targets.Remove(i);
			}
		}

//#ifdef EAI_TRACE
		bool swap;
//#endif
		for (i = 0; i < m_eAI_Targets.Count() - 1; i++)
		{
			int min_idx = i;
			for (int j = i + 1; j < m_eAI_Targets.Count(); j++)
			{
				if (m_eAI_Targets[j] && m_eAI_Targets[min_idx] && m_eAI_Targets[j].GetThreat(this) > m_eAI_Targets[min_idx].GetThreat(this))
				{
//#ifdef EAI_TRACE
					swap = true;
//#endif
					min_idx = j;
				}
			}

			if (min_idx != i)
				m_eAI_Targets.SwapItems(min_idx, i);
		}

//#ifdef EAI_TRACE
		if (swap)
			EXTrace.Print(EXTrace.AI, this, "PrioritizeTargets - prioritizing target " + Debug.GetDebugName(m_eAI_Targets[0].GetEntity()));
//#endif
	}

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

	void Notify_Melee()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Notify_Melee");
#endif

		Expansion_GetUp();

		m_eAI_Melee = true;
	}

	/**
	 * @brief Overrides the desired position to generate a new path
	 *
	 * @param pPosition the target position for path finding
	 */
	void OverrideTargetPosition(vector pPosition)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OverrideTargetPosition").Add(pPosition);
#endif

		//! XXX: This is a workaround for AI sometimes not moving despite old movement command still active.
		//! Just restart it to make the AI move again.
		if (m_eAI_MovementCheckTimeout <= 0.0)
		{
			if (pPosition != m_eAI_LastMovementCheckTargetPosition)
			{
				m_eAI_MovementCheckTimeout = 3.0;
				m_eAI_LastMovementCheckTargetPosition = pPosition;
				m_eAI_LastMovementCheckUnitPosition = GetPosition();
			}
		}
		else
		{
			m_eAI_MovementCheckTimeout -= 0.033333;
			if (m_eAI_MovementCheckTimeout <= 0.0)
			{
				vector currentPosition = GetPosition();
				eAICommandMove hcm;
				if (currentPosition == m_eAI_LastMovementCheckUnitPosition && eAICommandMove.CastTo(hcm, m_eAI_Command) && hcm.GetWaypointDistance() >= 0.3)
				{
					EXTrace.Print(EXTrace.AI, this, "MOVE YOU BASTARD " + m_eAI_Command);
					StartCommand_MoveAI();
					//eAI_Unbug("move");
				}

			}
		}
			
		m_PathFinding.SetTarget(pPosition);
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

	void OverrideStance(int pStanceIdx)
	{
		eAICommandMove move = StartCommand_MoveAI();
		move.OverrideStance(pStanceIdx);
	}

	void SetMovementSpeedLimit(int pSpeed, int pSpeedUnderThreat = -1)
	{
		m_MovementSpeedLimit = eAI_GetMovementSpeed(pSpeed);
		if (pSpeedUnderThreat == -1)
			pSpeedUnderThreat = pSpeed;
		m_MovementSpeedLimitUnderThreat = eAI_GetMovementSpeed(pSpeedUnderThreat);
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

	//! XXX: This is a GIANT hack because I cannot figure out how to sync taking item to hands in multiplayer context :-(
	EntityAI Expansion_CloneItemToHands(EntityAI src)
	{
		return Expansion_CloneItemToInventory(src, FindInventoryLocationType.HANDS);
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

		if (!GetInventory().FindFreeLocationFor(src, flags, location))
			return null;

		return Expansion_CloneItemToLocation(src, location);
	}

	EntityAI Expansion_CloneItemToLocation(EntityAI src, InventoryLocation location)
	{
		EntityAI dst = ExpansionItemSpawnHelper.Clone(src, true, location);
		if (dst)
		{
			ItemBase item;
			if (Class.CastTo(item, src))
				eAI_RemoveItem(item);
			GetGame().ObjectDelete(src);
		}

		return dst;
	}

	void eAI_AddItem(ItemBase item)
	{
		if (item.IsWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - gun " + item);
			m_Weapons.Insert(item);
			return;
		}

		if (item.Expansion_IsMeleeWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - melee weapon " + item);
			m_MeleeWeapons.Insert(item);
			return;
		}

		if (item.IsInherited(BandageDressing) || item.IsInherited(Rag))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - bandage/rag " + item);
			m_Bandages.Insert(item);
			return;
		}

		//! Ammo/magazines
		if (item.IsInherited(Magazine))
		{
			//! Force re-evaluation of any gun (loot) targets/guns in inventory
			eAI_EvaluateFirearmTypes();
		}
	}

	void eAI_RemoveItem(ItemBase item)
	{
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

		if (item.IsInherited(BandageDressing) || item.IsInherited(Rag))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - bandage/rag " + item);
			m_Bandages.RemoveItem(item);
			return;
		}

		//! Ammo/magazines
		if (item.IsInherited(Magazine))
		{
			//! Force re-evaluation of any gun (loot) targets/guns in inventory
			eAI_EvaluateFirearmTypes();
		}
	}

	bool eAI_HasWeaponInInventory()
	{
		return m_Weapons.Count() > 0;
	}

	bool eAI_HasAmmoForFirearm(Weapon gun, out Magazine mag, bool checkMagsInInventory = true)
	{
		mag = null;

		int mi = gun.GetCurrentMuzzle();
		if (!gun.IsChamberEmpty(mi) && !gun.IsChamberFiredOut(mi))
			return true;
		if (gun.HasInternalMagazine(mi) && gun.GetInternalMagazineCartridgeCount(mi))
			return true;
		if (Class.CastTo(mag, gun.GetMagazine(mi)) && mag.GetAmmoCount())
			return true;

		bool found = m_eAI_EvaluatedFirearmTypes.Find(gun.Type(), mag);
		bool hasAmmo;

		if (found)
			hasAmmo = mag && mag.GetAmmoCount();

		if (!found || (checkMagsInInventory && !hasAmmo))
		{
			//! GetMagazineInInventory will only return non-empty mags/ammo. EXPENSIVE, use with care.
			Class.CastTo(mag, GetMagazineInInventory(gun));

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

#ifndef SERVER
		for (int i = m_Expansion_DebugShapes.Count() - 1; i >= 0; i--)
			m_Expansion_DebugShapes[i].Destroy();
		m_Expansion_DebugShapes.Clear();
#endif

		if (EXTrace.AI && (pCurrentCommandID != m_eAI_CurrentCommandID || pCurrentCommandFinished))
		{
			if (pCurrentCommandID != m_eAI_CurrentCommandID)
				EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(m_eAI_CurrentCommandID) + " -> " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
			else
				EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
			m_eAI_CurrentCommandID = pCurrentCommandID;
		}

		//! handle death with high priority
		if (HandleDeath(pCurrentCommandID))
		{
			return;
		}

		if (!GetGame().IsServer())
			return;

		int simulationPrecision = 0;

		UpdateTargets(pDt);
		PrioritizeTargets();

		if (m_eAI_Targets.Count() > 0 && m_eAI_CurrentThreatToSelfActive > 0.2)
		{
			eAITarget target = m_eAI_Targets[0];
			if (target.HasInfo())
			{
				m_PathFinding.SetTarget(target.GetPosition(this));
			}
		}

		if (pCurrentCommandID != DayZPlayerConstants.COMMANDID_CLIMB)
			m_PathFinding.OnUpdate(pDt, simulationPrecision);

		GetTransform(m_ExTransformPlayer);

		if (m_eAI_Targets.Count() > 0)
		{
			vector aimPosition = m_eAI_Targets[0].GetPosition(this) + m_eAI_Targets[0].GetAimOffset(this);
			//! @note look position also used for LOS
			LookAtPosition(aimPosition, m_eAI_CurrentThreatToSelfActive >= 0.15);
			if (!m_eAI_LookDirection_Recalculate)
				LookAtDirection("0 0 1");
			AimAtPosition(aimPosition, m_eAI_CurrentThreatToSelfActive >= 0.2);
		}
		else
		{
			if (m_eAI_LookDirection_Recalculate)
				LookAtDirection("0 0 1");

			if (m_eAI_AimDirection_Recalculate)
				AimAtDirection("0 0 1");
		}

		EnforceLOS();

		DetermineThreatToSelf(pDt);
		ReactToThreatChange(pDt);

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

		if (m_eAI_LookDirection_Recalculate)
		{
			m_eAI_LookDirectionTarget_ModelSpace = vector.Direction(GetPosition() + "0 1.5 0", m_eAI_LookPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

		if (m_eAI_AimDirection_Recalculate)
		{
			m_eAI_AimDirectionTarget_ModelSpace = vector.Direction(GetPosition() + "0 1.5 0", m_eAI_AimPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

		HumanInputController hic = GetInputController();
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();

		if (hic)
		{
			bool exitIronSights = false;
			//! @note HandleWeapons also deals with hands lowering/raising if no weapon in hands
			HandleWeapons(pDt, entityInHands, hic, exitIronSights);
		}

		GetDayZPlayerInventory().HandleInventory(pDt);

		if (m_WeaponManager)
			m_WeaponManager.Update(pDt);
		if (m_EmoteManager)
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

		auto nearestPlayer = GetNearestPlayer();
		if (nearestPlayer && m_FSM)
			m_FSM.Update(pDt, simulationPrecision);

		switch (m_AimingState)
		{
		case eAIAimingState.INACTIVE:
			GetAimingProfile().UpdateArbiter(null);
			break;
		case eAIAimingState.ACTIVE:
			GetAimingProfile().UpdateArbiter(nearestPlayer);
			break;
		}

		bool skipScript;

		if (m_ActionManager)
		{
			m_ActionManager.Update(DayZPlayerConstants.COMMANDID_MOVE);

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
			int landType = 0;
			HumanCommandFall fall = GetCommand_Fall();

			if (fall && fall.PhysicsLanded())
			{
				DayZPlayerType type = GetDayZPlayerType();
				NoiseParams npar;

				// land
				m_FallYDiff = m_FallYDiff - GetPosition()[1];
				// CF_Log.Debug(m_FallYDiff);
				if (m_FallYDiff < 0.5)
				{
					landType = HumanCommandFall.LANDTYPE_NONE;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (m_FallYDiff < 1.0)
				{
					landType = HumanCommandFall.LANDTYPE_LIGHT;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandLight();
					AddNoise(npar);
				}
				else if (m_FallYDiff < 2.0)
				{
					landType = HumanCommandFall.LANDTYPE_MEDIUM;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}
				else
				{
					landType = HumanCommandFall.LANDTYPE_HEAVY;
					fall.Land(landType);
					npar = type.GetNoiseParamsLandHeavy();
					AddNoise(npar);
				}

				if (m_FallYDiff >= DayZPlayerImplementFallDamage.FD_DMG_FROM_HEIGHT && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
				{
					this.SpawnDamageDealtEffect();
				}

				m_FallDamage.HandleFallDamage(m_FallYDiff);
				m_JumpClimb.CheckAndFinishJump(landType);
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
			// CF_Log.Error("'COMMANDID_MOVE' was running. This shouldn't happen ever.");
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
					m_eAI_MeleeTimeout = 1.5;

					m_eMeleeCombat.Combo(hcm2);
				}
			}
		}
		else if (m_eAI_Melee)
		{
			m_eAI_Melee = false;
			m_eAI_MeleeTimeout = 1.5;

			m_eMeleeCombat.Start();

			//! Prevent NULL pointers by skipping COMMANDID_SCRIPT handling
			skipScript = true;
		}
		else if (IsInMelee())
		{
			m_eAI_MeleeTimeout -= pDt;
		}

		m_eAI_MeleeDidHit = false;

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT && m_eAI_Command && !skipScript)
		{
			vector lookTargetRelAngles = m_eAI_LookDirectionTarget_ModelSpace.VectorToAngles();

			//! We want to interpolate rel angles for looking! Otherwise, if the conversion to rel angles happens later,
			//! there will be a sudden jump in the unit's head rotation between 180 and -180 due to the way the head animation is set up
			lookTargetRelAngles[0] = ExpansionMath.RelAngle(lookTargetRelAngles[0]);
			lookTargetRelAngles[1] = ExpansionMath.RelAngle(lookTargetRelAngles[1]);

			//TODO: quaternion slerp instead for better, accurate results
			m_eAI_LookRelAngles = vector.Lerp(m_eAI_LookRelAngles, lookTargetRelAngles, pDt * 4);

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
				if (m_eAI_CurrentThreatToSelfActive > 0.4)
					speedLimit = m_MovementSpeedLimitUnderThreat;
				else
					speedLimit = m_MovementSpeedLimit;

				if (m_WeaponRaised)
					speedLimit = Math.Min(speedLimit, 1);

				float turnTarget = GetOrientation()[0];

				if (m_eAI_Targets.Count() > 0)
				{
					if (m_eAI_AimDirection_Recalculate || Math.AbsFloat(m_eAI_LookRelAngles[0]) > 90)
					{
						turnTarget = vector.Direction(GetPosition(), m_eAI_AimPosition_WorldSpace).Normalized().VectorToAngles()[0];
					}
				}

				hcm.SetTurnTarget(turnTarget);

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

	void EnforceLOS()
	{
		//! Prevent core dump on server shutdown
		if (!GetGame())
			return;

		if (m_eAI_SideStepTimeout > 0 || m_MovementDirectionActive)
			return;

		if (!m_eAI_Targets.Count())
			return;

		EntityAI targetEntity = GetTarget().GetEntity();

		if (targetEntity.IsInherited(ItemBase))
		{
			m_eAI_LOS = true;
			return;
		}

		vector begPos = GetBonePositionWS(GetBoneIndexByName("Head"));
		vector endPos = m_eAI_LookPosition_WorldSpace;

		vector contactPos;
		vector contactDir;
		int contactComponent;

		set< Object > results = new set< Object >;
		bool hadLos = m_eAI_LOS;
		m_eAI_LOS = DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, null, this, false, false, ObjIntersectGeom, 0.5);
		if (!m_eAI_LOS && hadLos)
			EXTrace.Print(EXTrace.AI, this, "lost line of sight to target " + targetEntity);
		if (!m_eAI_LOS)
			return;

		PlayerBase player;

		bool sideStep;

		foreach (Object obj: results)
		{
			if (obj.IsTree() || obj.IsBush())
			{
				m_eAI_LOS = false;
				sideStep = true;
				break;
			}

			if (Class.CastTo(player, obj))
			{
				if (!PlayerIsEnemy(player))
				{
					m_eAI_LOS = false;
					sideStep = true;
				}
				//! If player is an enemy, but not the target, we don't care if they get shot when they are in the way
				break;
			}

			//if (obj.IsBuilding())
			//{
			//	m_eAI_LOS = false;
			//	break;
			//}

			if (obj != targetEntity)
			{
				if (!obj.IsInherited(ZombieBase) && !obj.IsInherited(AnimalBase))
					m_eAI_LOS = false;
				//! If object is zombie or animal but not the target, we don't care if they get shot when they are in the way
				break;
			}
		}

		if (m_eAI_LOS != hadLos)
			EXTrace.Print(EXTrace.AI, this, "has line of sight to target (" + targetEntity + ")? " + m_eAI_LOS);

		if (sideStep)
		{
			//! First check if we are roughly facing the target
			//! @note vector.Direction(GetPosition(), GetTarget().GetPosition(this)).Normalized() returns zero vector,
			//! have to first assign target position to variable. Why is this language so INCREDIBLY fucked? :-(
			vector targetPos = GetTarget().GetPosition(this);
			vector targetDirection = vector.Direction(GetPosition(), targetPos).Normalized();
			float dot = vector.Dot(GetDirection(), targetDirection);
			if (dot >= 0.75)
			{
				//! Move, b*tch, get out the way :-)
				m_eAI_SideStepTimeout = 1.5;
				if (Math.RandomIntInclusive(0, 1))
					m_SideStepAngle = 90;
				else
					m_SideStepAngle = -90;
				EXTrace.Print(EXTrace.AI, this, "sidestep " + m_SideStepAngle + " " + obj);
				OverrideMovementDirection(true, m_SideStepAngle);
			}
		}
	}

	override void OnScheduledTick(float deltaTime)
	{
		if (!IsPlayerSelected() || !IsAlive())
			return;
		if (m_ModifiersManager)
			m_ModifiersManager.OnScheduledTick(deltaTime);
		if (m_NotifiersManager)
			m_NotifiersManager.OnScheduledTick();
		//! These send RPC to identity which is NULL for AI and thus affects all actual players HUDs (shows AI values instead of player)
		// if( m_TrasferValues ) m_TrasferValues.OnScheduledTick(deltaTime);
		// if( m_VirtualHud ) m_VirtualHud.OnScheduledTick();
		if (GetBleedingManagerServer())
			GetBleedingManagerServer().OnTick(deltaTime);

		// Check if electric device needs to be unplugged
		ItemBase heldItem = GetItemInHands();
		if (heldItem && heldItem.HasEnergyManager() && heldItem.GetCompEM().IsPlugged())
		{
			// Now we know we are working with an electric device which is plugged into a power source.
			EntityAI placed_entity = heldItem;

			// Unplug the device when the player is too far from the power source.
			placed_entity.GetCompEM().UpdatePlugState();
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
		if (!IsAI())
		{
			super.QuickReloadWeapon(weapon);
			return;
		}

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

			AnimSetBool(m_ExpansionST.m_VAR_Raised, m_WeaponRaised);
		}

		if (m_WeaponRaised)
		{
			m_WeaponRaisedTimer += pDt;

#ifndef SERVER
			vector position;
			vector direction;

			GetAimingProfile().Get(position, direction);

			vector points[2];
			points[0] = position;
			points[1] = position + (direction * 1000.0);
			m_Expansion_DebugShapes.Insert(Shape.CreateLines(COLOR_BLUE, ShapeFlags.VISIBLE, points, 2));
#endif

			//TODO: quaternion slerp instead for better, accurate results
			m_eAI_AimDirection_ModelSpace = vector.Lerp(m_eAI_AimDirection_ModelSpace, m_eAI_AimDirectionTarget_ModelSpace, pDt);		

			vector aimOrientation = m_eAI_AimDirection_ModelSpace.VectorToAngles();

			float dist = vector.Distance(GetPosition() + "0 1.5 0", m_eAI_AimPosition_WorldSpace);
			dist = Math.Clamp(dist, 1.0, 360.0);

			float aimX = ExpansionMath.RelAngle(aimOrientation[0] + (-30.0 / dist));
			float aimY = ExpansionMath.RelAngle(aimOrientation[1]);

			aimX = Math.Clamp(aimX, -90.0, 90.0);
			aimY = Math.Clamp(aimY, -90.0, 90.0);

			AnimSetFloat(m_ExpansionST.m_VAR_AimX, aimX);
			AnimSetFloat(m_ExpansionST.m_VAR_AimY, aimY);
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

		return m_eAI_LOS && m_eAI_SideStepTimeout <= 0;
	}

	// @param true to put weapon up, false to lower
	void RaiseWeapon(bool up)
	{
		m_WeaponRaised = up;
	}

	override bool IsRaised()
	{
		return m_WeaponRaised;
	}

	override bool IsWeaponRaiseCompleted()
	{
		return m_WeaponRaisedTimer > 0.5;
	}

	bool IsTrading()
	{
		return false;
	}

	ActionBase StartAction(typename actionType, ActionTarget target)
	{
		ActionBase action = m_eActionManager.GetAction(actionType);

		m_eActionManager.PerformActionStart(action, target, GetItemInHands());

		return action;
	}

	ActionBase StartActionObject(typename actionType, Object target)
	{
		return StartAction(actionType, new ActionTarget(target, null, -1, vector.Zero, -1.0));
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

	vector GetLookDirection()
	{
		return m_eAI_LookRelAngles.AnglesToVector();
	}

	bool GetLookDirectionRecalculate()
	{
		return m_eAI_LookDirection_Recalculate;
	}

	vector GetAimDirection()
	{
		return m_eAI_AimDirection_ModelSpace;
	}

	vector GetAimPosition()
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
		eAI_DropItemInHands();

		super.OnUnconsciousStart();
	}

	override void OnUnconsciousStop(int pCurrentCommandID)
	{
		super.OnUnconsciousStop(pCurrentCommandID);

		Expansion_GetUp(true);
	}

	void eAI_DropItemInHands()
	{
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands)
			eAI_DropItem(itemInHands);
	}

	void eAI_DropItem(ItemBase item)
	{
		//! XXX: DayZ 1.18 made dropping weapon unreliable. Clone to ground instead.
		//InventoryLocation il_src = new InventoryLocation();
		//InventoryLocation il_dst = new InventoryLocation();

		//item.GetInventory().GetCurrentInventoryLocation(il_src);

		//GameInventory.SetGroundPosByOwner(this, item, il_dst);

		//ServerTakeToDst(il_src, il_dst);

		Expansion_CloneItemToGround(item, GetPosition());
	}

	void Expansion_GetUp(bool force = false)
	{
		if (!force && IsPlayerInStance(DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_RAISEDERECT))
			return;

		EXTrace.Print(EXTrace.AI, this, "Expansion_GetUp " + force);

		//! XXX: Breaks collision box (will still be for prone stance)
		//! Use HumanCommandMove to get up for now which doesn't have this problem.
		//eAICommandMove move = StartCommand_MoveAI();
		//move.GetUp();

		HumanCommandMove cm = StartCommand_Move();
		if (cm)
		{
			cm.ForceStanceUp(DayZPlayerConstants.STANCEIDX_ERECT);
		}
	}

	override vector Expansion_GetHeadingVector()
	{
		return Vector(GetOrientation()[0], 0, 0).AnglesToVector();
	}

	bool HandleVaulting(float pDt)
	{
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
		//ExpansionClimb.DoClimbTest(this, m_ExClimbResult);

		if (!m_ExClimbResult.m_bIsClimb && !m_ExClimbResult.m_bIsClimbOver)
		{
			return false;
		}

		return true;
	}

	override bool CanClimb(int climbType, SHumanCommandClimbResult climbRes)
	{
		if (IsFBSymptomPlaying() || IsRestrained() || IsUnconscious() || IsInFBEmoteState())
			return false;
		
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
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

			if (object.IsBuilding() && !climbRes.m_bIsClimb)
				return false;
		}

		EXTrace.Print(EXTrace.AI, this, "eAI_CanClimbOn " + Debug.GetDebugName(parent));

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

		bool isFallSafe = fallHeight <= 2.5 || (fallHeight <= 3.0 && GetHealth() >= 55.0);

		//EXPrint("position " + position + " checkDirection " + checkDirection + " " + checkDirection.VectorToAngles() + " checkPosition " + checkPosition + " " + isFallSafe);

		return isFallSafe;
	}

	void HandleBuildingDoors(float pDt)
	{
		if (!m_PathFinding.IsDoor())
		{
			return;
		}

		vector position = m_ExTransformPlayer[3] + (m_ExTransformPlayer[1] * 1.1);
		vector direction = m_ExTransformPlayer[2];

		vector p0 = position;
		vector p1 = position + (direction * 1.5);

		RaycastRVParams params(p0, p1, this, 1.0);
		array<ref RaycastRVResult> results();
		array<Object> excluded();
		excluded.Insert(this);

		params.with = this;
		params.flags = CollisionFlags.ALLOBJECTS;
		params.type = ObjIntersectView;

		if (!DayZPhysics.RaycastRVProxy(params, results, excluded))
			return;

		foreach (auto result : results)
		{
			Building building;
			if (!Class.CastTo(building, result.obj))
				continue;

			int doorIndex = building.GetDoorIndex(result.component);
			if (doorIndex == -1)
				continue;

			if (building.IsDoorOpen(doorIndex))
				continue;

			if (building.IsDoorLocked(doorIndex))
				continue;


			/**
			 * TODO: Use FSM events to notify that the door should be opened. 
			 * TODO: Add support for animation action logic to FSM states
			 * TODO: FSM file just for doors. Opening, picking, breaking, closing. Vehicles and Buildings.
			 * 
			 *	GetFSM().Start("OpenDoor")
			 * 
			 * Events are stored in the transitions
			 */

			building.OpenDoor(doorIndex);
			
			ActionInteractBaseCB.Cast(AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW,ActionInteractBaseCB));
			return;
		}
	}
};
