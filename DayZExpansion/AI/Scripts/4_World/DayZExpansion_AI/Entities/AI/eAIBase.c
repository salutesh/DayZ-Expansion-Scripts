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

typedef map<BuildingBase, ref map<int, ref ExpansionLadder>> eAILadders;
typedef map<EntityAI, bool> eAIThreatOverride;
typedef map<string, float> eAIRecentlyDroppedItems;
typedef map<eAITargetInformation, ref eAITarget> eAITargetInformationStates;

class eAIBase: PlayerBase
{
	const int EAI_COMMANDID_MOVE = 1;

	static bool AI_HANDLEDOORS = true;
	static bool AI_HANDLEVAULTING = true;

	static int s_eAI_UnlimitedReload;
	static int s_eAI_UnlimitedReloadAll;

	private static autoptr array<eAIBase> s_AllAI = new array<eAIBase>();
	static float s_eAI_LastCEUpdateTime;
	static ref set<Object> s_eAI_TakenCoverObjects = new set<Object>;

	//! model name w/o .p3d ext, can be partial
	static ref TStringArray s_eAI_PreventClimb = {
		"pole",
		"sign",
		"busstop",
		"stairs",
		"farm_strawstack",
		"misc_walkover",
		"rubble_dirtpile_large",
		"stockyard_oremound",
		"wall_canal_10"
	};

	//! model name w/o .p3d ext, can be partial
	static ref TStringArray s_eAI_PreventClimbOver = {
		"container",
		"tank_small",
		"misc_haybale",
		"misc_woodreserve",
		"misc_advertcolumn",
		"misc_coil",
		"ice_sea"
	};

	static string s_Expansion_SurvivorDisplayName = g_Game.ConfigGetTextOut(CFG_VEHICLESPATH + " SurvivorBase displayName");

	private ref eAICallbacks m_eAI_Callbacks = new eAICallbacks(this);

	protected autoptr eAIFSM m_FSM;
	bool m_eAI_IsFightingFSM;
	bool m_eAI_ShouldTakeCover;
	bool m_eAI_UpdatePotentialCoverObjects;

	//! Fighting FSM variables
	int m_eAI_LastFireTime;
	int m_eAI_TimeBetweenFiring = 10000;
	int m_eAI_TimeBetweenFiringAndGettingUp = 15000;
	int m_eAI_LastEvadeTime;
	float m_eAI_DistanceToTargetSq;

	// Targeting data
	private autoptr array<ref eAITarget> m_eAI_Targets;
	bool m_eAI_TargetChanged;
	ref eAITarget m_eAI_ItemTargetHistory[3];
	typename m_eAI_LastEngagedTargetType;
	int m_eAI_AcuteDangerTargetCount;
	int m_eAI_AcuteDangerPlayerTargetCount;
	float m_eAI_SilentAttackViabilityTime;
#ifdef DIAG_DEVELOPER
	bool m_eAI_PrintCurrentTarget;
#endif
	ref eAITargetInformationStates m_eAI_TargetInformationStates = new eAITargetInformationStates;
	float m_ThreatClearedTimeout;
	float m_eAI_CurrentThreatToSelf;
	float m_eAI_PreviousThreatToSelf;
	float m_eAI_CurrentThreatToSelfActive;
	float m_eAI_PreviousThreatToSelfActive;

	//[eAIAttribute<float>.Register("m_eAI_AccuracyMin")]
	float m_eAI_AccuracyMin;

	//[eAIAttribute<float>.Register("m_eAI_AccuracyMax")]
	float m_eAI_AccuracyMax;

	//[eAIAttribute<float>.Register("m_eAI_ThreatDistanceLimit")]
	float m_eAI_ThreatDistanceLimit;

	//[eAIAttribute<float>.Register("m_eAI_NoiseInvestigationDistanceLimit")]
	float m_eAI_NoiseInvestigationDistanceLimit;

	//[eAIAttribute<float>.Register("m_eAI_DamageMultiplier")]
	float m_eAI_DamageMultiplier;

	bool m_eAI_SyncCurrentTarget;
	int m_eAI_CurrentTarget_NetIDLow;
	int m_eAI_CurrentTarget_NetIDHigh;
	int m_eAI_CurrentTarget_NetIDLowSync;
	int m_eAI_CurrentTarget_NetIDHighSync;
	ref eAINoiseTargetInformation m_eAI_NoiseTargetInfo = new eAINoiseTargetInformation();
	int m_eAI_NoiseTarget;
	private bool m_eAI_HasLOS;
	ref array<ref eAIShot> m_eAI_FiredShots = {};
	float m_eAI_PurgeFiredShotsTick;
	Object m_eAI_HitObject;
	int m_eAI_DiscardedShot_DbgIdx = -8;
	float m_eAI_DbgThreshAngleH;
	float m_eAI_DbgLookAngleH;
	int m_eAI_DbgLOSAngles = -1;

	// Command handling
	ref eAICommandMove m_eAI_CommandMove;
	private int m_eAI_CurrentCommandID;
	float m_eAI_CommandHandlerDT;
	float m_eAI_LOSCheckDT;
	float m_eAI_CommandTime;
	bool m_eAI_DeathHandled;
	bool m_eAI_SkipScript;
	bool m_eAI_IsOnLadder;
	float m_eAI_LadderTime;
	int m_eAI_LadderClimbDirection;  //! 1 = up, -1 = down
	ExpansionLadder m_eAI_Ladder;
	vector m_eAI_LadderEntryPoint;
	vector m_eAI_LadderDirPoint;

	//[eAIAttribute<BuildingBase>.Register("m_eAI_BuildingWithLadder")]
	BuildingBase m_eAI_BuildingWithLadder;
	bool m_eAI_FloorIsBuildingWithLadder;
	bool m_eAI_TargetIsInBuildingWithLadderRadius;
	
	//[eAIAttribute<eAILadders>.Register("m_eAI_Ladders")]
	ref eAILadders m_eAI_Ladders = new eAILadders;

	//[eAIAttribute<int>.Register("m_eAI_LadderLoops")]
	int m_eAI_LadderLoops;

	//[eAIAttribute<BuildingBase>.Register("m_eAI_LastClimbedBuildingWithLadder")]
	BuildingBase m_eAI_LastClimbedBuildingWithLadder;

	//[eAIAttribute<ExpansionLadder>.Register("m_eAI_LastClimbedLadder")]
	ref ExpansionLadder m_eAI_LastClimbedLadder;

	bool m_eAI_PreferLadder;
	bool m_eAI_Recreate;
	bool m_eAI_Recreating;

	private Transport m_eAI_Transport;
	private int m_eAI_Transport_SeatIndex;

	int m_eAI_MeleeTime;

	private ref eAIAimingProfile m_AimingProfile;

	private ref eAIActionManager m_eActionManager;
	ref eAIMeleeCombat m_eMeleeCombat;
	ref eAIMeleeFightLogic_LightHeavy m_eAI_MeleeFightLogic;

	bool m_eAI_FallHasLanded;
	float m_eAI_FallYVelZeroTime;

	private bool m_eAI_IsChangingStance;
	private bool m_eAI_ShouldGetUp = true;
	eAIStance m_eAI_StancePreference = -1;
	eAIStance m_eAI_DefaultStance = eAIStance.STANDING;
	float m_eAI_DefaultLookAngle = -1;

	bool m_eAI_IsRestrained;
	bool m_eAI_IsInventoryVisible;

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
	private vector m_eAI_AimDirectionPrev;

	//! Dynamic FOV (detection angle) depending on distance to target
	static float m_eAI_FOVNear_DistThreshold = 0;
	static float m_eAI_FOVFar_DistThreshold = 125;
	static float m_eAI_FOVNear_HalfAngleH = 120;  //! Half angle! FOV = angle * 2
	static float m_eAI_FOVFar_HalfAngleH = 45;  //! Half angle! FOV = angle * 2
	static float m_eAI_FOVTransitionExponent = 2.0;  //! 1.0 = linear

	private bool m_MovementSpeedActive;
	private int m_MovementSpeed;

	//[eAIAttribute<int>.Register("m_MovementSpeedLimit")]
	private int m_MovementSpeedLimit = 3;

	//[eAIAttribute<int>.Register("m_MovementSpeedLimitUnderThreat")]
	private int m_MovementSpeedLimitUnderThreat = 3;

	//[eAIAttribute<int>.Register("m_eAI_RoamingMovementSpeedLimit")]
	int m_eAI_RoamingMovementSpeedLimit;

	//[eAIAttribute<int>.Register("m_eAI_SpeedLimitPreference")]
	int m_eAI_SpeedLimitPreference = 3;

	bool m_eAI_IsAttachedToMovingParent;

	private bool m_MovementDirectionActive;
	protected bool m_eAI_ResetMovementDirectionActive;
	private float m_MovementDirection;
	private float m_SideStepAngle;
	private bool m_eAI_TurnTargetActive;
	private float m_eAI_TurnTarget;
	bool m_eAI_Halt;
	bool m_eAI_ResetInteractionLayer;
	float m_eAI_PositionTime;
	float m_eAI_BlockedTime;
	float m_eAI_LastDoorInteractionTime;

	float m_eAI_FormationPositionUpdateTime;
	vector m_eAI_FormationPosition;
	vector m_eAI_FormationDirection;

	float m_eAI_FormationDirectionUpdateTime;
	float m_eAI_FormationDirectionNextUpdateTime;
	float m_eAI_MovementSpeedPrev;

	bool m_eAI_HasProjectileWeaponInHands;
	private bool m_WeaponRaised;
	private bool m_WeaponRaisedPrev;

	private float m_WeaponRaisedTimer;
	private float m_WeaponLowerTimeout;

	int m_eAI_FireWeaponOnClient_RPCID;
	int m_eAI_RemoteRecreateWeapon_RPCID;

	ref array<Weapon_Base> m_eAI_Firearms = {};
	ref array<Weapon_Base> m_eAI_Handguns = {};
	ref array<Weapon_Base> m_eAI_Launchers = {};
	ref array<ItemBase> m_eAI_MeleeWeapons = {};
	ref array<ItemBase> m_eAI_Bandages = {};
	ref array<ItemBase> m_eAI_RepairKits = {};
	ref array<Magazine> m_eAI_Magazines = {};
	ref array<ItemBase> m_eAI_Food = {};
	ref array<ItemBase> m_eAI_ItemsToDrop = {};
	ItemBase m_eAI_BandageToUse;
	int m_eAI_MaxFoodCount;

	ref map<typename, Magazine> m_eAI_EvaluatedFirearmTypes = new map<typename, Magazine>;

	int m_eAI_AccessibleCargoSpaceTaken;
	int m_eAI_AccessibleCargoSpaceTotal;

	ref set<Clothing> m_eAI_CargosToTidy = new set<Clothing>;

#ifdef DIAG_DEVELOPER
	EntityAI m_eAI_CompareWeapon;
	EntityAI m_eAI_CurrentWeapon;
#endif

	Object m_eAI_SideStepObject;
	private float m_eAI_SideStepTimeout;
	private bool m_eAI_SideStepCancelOnLOS;
	private float m_eAI_PositionOverrideTimeout;

	//[eAIAttribute<int>.Register("m_eAI_UnlimitedReload")]
	private int m_eAI_UnlimitedReload;

	bool m_eAI_IsUnlimitedReloadAll;

	//[eAIAttribute<float>.Register("m_eAI_SniperProneDistanceThreshold")]
	private float m_eAI_SniperProneDistanceThreshold;

	//[eAIAttribute<int>.Register("m_eAI_LootingBehavior")]
	int m_eAI_LootingBehavior = eAILootingBehavior.DEFAULT;

	// Path Finding
#ifndef EAI_USE_LEGACY_PATHFINDING
	/*private*/ ref ExpansionPathHandler m_PathFinding;
#else
	/*private*/ ref eAIPathFinding m_PathFinding;
#endif
	vector m_eAI_TargetPosition;
	bool m_eAI_TargetPositionIsFinal = true;
	bool m_eAI_PositionIsFinal = true;
	int m_eAI_PrevHCCState = -1;
	float m_eAI_SurfaceY;
	//! @note we track the last entered area and NOT the cluster directly because it is a mutable property of the area
	//! and WILL be recreated whenever dynamic areas get spawned/despawned by CE!
	EffectArea m_eAI_EffectArea;
	int m_eAI_DangerousAreaCount;
	vector m_eAI_DangerousAreaAvoidancePosition;
	ref map<int, float> m_eAI_ProtectionLevels;
	bool m_eAI_IsInDangerByArea;

	private Apple m_DebugTargetApple;
	private vector m_DebugTargetOrientation;

	float m_eAI_UpdateTargetsTick;
	autoptr array<EntityAI> m_eAI_PotentialTargetEntities = new array<EntityAI>();
	ref array<EntityAI> m_eAI_TrackedBodies = {};
	int m_eAI_CurrentPotentialTargetIndex;
	CF_DoublyLinkedNode_WeakRef<PlayerBase> m_eAI_PotentialTargetPlayer;
	float m_eAI_UpdateNearTargetsTime;
	int m_eAI_UpdateNearTargetsCount;
	ref set<Object> m_eAI_PotentialCoverObjects = new set<Object>;
	Object m_eAI_CurrentCoverObject;
	vector m_eAI_CurrentCoverPosition;
	bool m_eAI_IsInCover;
	float m_eAI_MaxFlankingDistance = 200;
	bool m_eAI_EnableFlankingOutsideCombat;
	float m_eAI_FlankAngle;
	float m_eAI_FlankTime;
	float m_eAI_FlankTimeMax;

	int m_Expansion_EmoteID;
	bool m_Expansion_EmoteAutoCancel;
	int m_Expansion_EmoteAutoCancelDelay; //! ms

	int m_eAI_MinTimeTillNextFire;
	int m_eAI_QueuedShots;

	private float m_eAI_UpdateVisibilityTime = 5.0;  //! Force 1st update
	private float m_eAI_UpdateVisibilityTimeThreshold = 5.0;

	float m_Expansion_DaylightVisibility;
	float m_Expansion_Visibility = 0.1;
	float m_Expansion_VisibilityDistThreshold = 90.0;
	float m_eAI_DayNightThreshold = 0.4;  //! below threshold = night, above = day
	int m_eAI_IsDay = -1;  //! -1 = not set (implicit daytime), 0 = night, 1 = day
	float m_eAI_MinVisibility;
	float m_eAI_NightVisibility;
	float m_eAI_FogVisibility;
	float m_eAI_OvercastVisibility;
	float m_eAI_RainVisibility;
	float m_eAI_SnowVisibility;
	float m_eAI_DynVolFogVisibility;
	float m_eAI_ContaminatedAreaVisibility;
	float m_eAI_BaseVisibility;
	float m_eAI_VisibilityLimit;
	ref array<ItemBase> m_Expansion_ActiveVisibilityEnhancers = {};
	bool m_Expansion_TriedTurningOnVisibilityEnhancers;
	bool m_eAI_IsActiveVisibilityEnhancerTemporary;

	string m_eAI_TypeSwitchedOnDuringCombat;

	ref set<Man> m_eAI_InteractingPlayers = new set<Man>;

	//[eAIAttribute<bool>.Register("m_eAI_DespawnOnLoosingAggro")]
	bool m_eAI_DespawnOnLoosingAggro;

	//ref IsObjectObstructedCache m_eAI_IsItemObstructedCache = new IsObjectObstructedCache(vector.Zero, 1);
	//[eAIAttribute<eAIThreatOverride>.Register("m_eAI_ThreatOverride")]
	ref eAIThreatOverride m_eAI_ThreatOverride = new eAIThreatOverride;

	//[eAIAttribute<ItemBase>.Register("m_eAI_LastDroppedItem")]
	ItemBase m_eAI_LastDroppedItem;

	//[eAIAttribute<eAIRecentlyDroppedItems>.Register("m_eAI_RecentlyDroppedItems")]
	ref eAIRecentlyDroppedItems m_eAI_RecentlyDroppedItems = new eAIRecentlyDroppedItems;

	ref Timer m_eAI_ClientUpdateTimer;

	static ref ExpansionSoundSet s_eAI_LoveSound01_SoundSet;
	static ref ExpansionSoundSet s_eAI_LoveSound02_SoundSet;

	int m_eAI_Meme;

	//[eAIAttribute<int>.Register("m_eAI_MemeLevel")]
	int m_eAI_MemeLevel;

	float m_eAI_Lean;
	float m_eAI_LeanTarget;
	
#ifndef DAYZ_1_25	
	vector m_ExTransformPlayer[4];
#endif
	
	ref set<PlayerBase> m_eAI_Spectators = new set<PlayerBase>;

	ref array<ref ExpansionPrefab> m_eAI_LootDropOnDeath;

	void eAIBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIBase");
#endif

#ifdef CF_DEBUG
		CF_Debug.Create(this);
#endif

		s_AllAI.Insert(this);

		if (g_Game.IsServer())
		{
			ExpansionAISettings settings = GetExpansionSettings().GetAI();
			AI_HANDLEVAULTING = settings.Vaulting;
			m_eAI_MemeLevel = settings.MemeLevel;
		}

		SetEventMask(EntityEvent.INIT);

	#ifdef AdmiralsDivingMod
		m_Swimming = new eAIImplementSwimming(this);
	#endif

/*
		if (g_Game.IsClient())
		{
			m_eAI_ClientUpdateTimer = new Timer(CALL_CATEGORY_SYSTEM);
			m_eAI_ClientUpdateTimer.Run(1.0 / 30.0, this, "eAI_ClientUpdate", NULL, true);
		}
*/
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

	static array<eAIBase> eAI_GetAll()
	{
		return s_AllAI;
	}

	//! @note Init is called from vanilla PlayerBase ctor, so runs before eAIBase ctor!
	override void Init()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Init");
#endif

		super.Init();

		m_eAI_Targets = {};

		m_eAI_ProtectionLevels = new map<int, float>;

		m_AimingProfile = new eAIAimingProfile(this);

		m_eMeleeCombat = new eAIMeleeCombat(this);
		m_MeleeCombat = m_eMeleeCombat;

		m_eAI_MeleeFightLogic = new eAIMeleeFightLogic_LightHeavy(this);
		m_MeleeFightLogic = m_eAI_MeleeFightLogic;

		m_WeaponManager = new eAIWeaponManager(this);
		m_ShockHandler = new eAIShockHandler(this);

		if (g_Game.IsServer())
		{
			eAI_SetAccuracy(-1, -1);
			eAI_SetThreatDistanceLimit(-1);
			eAI_SetNoiseInvestigationDistanceLimit(-1);
			eAI_SetMaxFlankingDistance(-1);
			eAI_SetEnableFlankingOutsideCombat(-1);
			eAI_SetDamageMultiplier(-1);
			eAI_SetDamageReceivedMultiplier(-1);
			m_eAI_SniperProneDistanceThreshold = GetExpansionSettings().GetAI().SniperProneDistanceThreshold;

#ifndef EAI_USE_LEGACY_PATHFINDING
			m_PathFinding = new ExpansionPathHandler(this);
#else
			m_PathFinding = new eAIPathFinding(this);
#endif

			LoadFSM();

			s_Expansion_AllPlayers.m_OnRemove.Insert(eAI_OnRemovePlayer);

			GetStatWater().Set(GetStatWater().GetMax());
			GetStatEnergy().Set(GetStatEnergy().GetMax());
		}

		LookAtDirection("0 0 1");
		AimAtDirection("0 0 1");

		//! Vehicles mod will set this in PlayerBase::Init if loaded
		if (!m_ExpansionST)
			m_ExpansionST = new ExpansionHumanST(this);

		if (g_Game.IsServer())
		{
			m_eAI_CommandMove = new eAICommandMove(this, m_ExpansionST, DayZPlayerConstants.STANCEIDX_ERECT);

			eAINoiseSystem.SI_OnNoiseAdded.Insert(eAI_OnNoiseEvent);
		}

		m_Expansion_EnableBonePositionUpdate = true;

		m_Expansion_RPCManager.RegisterServer("RPC_eAI_SetIsInventoryVisible");
		m_eAI_FireWeaponOnClient_RPCID = m_Expansion_RPCManager.RegisterClient("RPC_eAI_FireWeaponOnClient");
		m_eAI_RemoteRecreateWeapon_RPCID = m_Expansion_RPCManager.RegisterServer("RPC_eAI_RemoteRecreateWeapon");
	}

	override void Expansion_Init()
	{
		if (g_Game.IsServer() && !m_eAI_FactionType)
		{
			m_eAI_FactionType = eAIFactionRaiders;
		}

		super.Expansion_Init();

		RegisterNetSyncVariableBool("m_Expansion_CanBeLooted");
		//RegisterNetSyncVariableFloat("m_eAI_AccuracyMin");
		//RegisterNetSyncVariableFloat("m_eAI_AccuracyMax");
		//RegisterNetSyncVariableInt("m_eAI_CurrentTarget_NetIDLow");
		//RegisterNetSyncVariableInt("m_eAI_CurrentTarget_NetIDHigh");
	//#ifdef DIAG_DEVELOPER
		//RegisterNetSyncVariableInt("m_eAI_DbgLOSAngles");
	//#endif
		RegisterNetSyncVariableBool("m_eAI_IsInventoryVisible");

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);

		if (!s_eAI_LoveSound01_SoundSet)
			s_eAI_LoveSound01_SoundSet = ExpansionSoundSet.Register("Expansion_AI_The_Sound_Of_Love_01_SoundSet");
		if (!s_eAI_LoveSound02_SoundSet)
			s_eAI_LoveSound02_SoundSet = ExpansionSoundSet.Register("Expansion_AI_The_Sound_Of_Love_02_SoundSet");
	}

	override void OnPlayerLoaded()
	{
		if (m_Environment)
			m_Environment.Init();
		
		if (g_Game.IsMultiplayer() || g_Game.IsServer())
			Expansion_SynchLifespanVisual();
		
		if (!g_Game.IsDedicatedServer())
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateCorpseStateVisual, 2000, false);
			m_PlayerSoundEventHandler = new PlayerSoundEventHandler(this);
			m_ReplaceSoundEventHandler = new ReplaceSoundEventHandler(this);
		}

		int slotId = InventorySlots.GetSlotIdFromString("Head");
		m_CharactersHead = Head_Default.Cast(GetInventory().FindPlaceholderForSlot(slotId));
		CheckHairClippingOnCharacterLoad();
		UpdateHairSelectionVisibility();
		PreloadDecayTexture();
		
		Weapon_Base wpn = Weapon_Base.Cast(GetItemInHands());
		if (wpn)
		{
			if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE)
			{
				wpn.DelayedValidateAndRepair();
			}
			else
			{
				wpn.ValidateAndRepair();
			}
		}

		m_PlayerLoaded = true;
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

	void eAI_ResetPathfinding()
	{
		m_PathFinding.ResetUnreachable();
		m_eAI_LadderLoops = 0;
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

		if (!g_Game)
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

		if (g_Game.IsServer())
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

		s_eAI_TickSchedulerPlayers.Insert(this);
	#ifdef DIAG_DEVELOPER
		EXPrint(this, "TickScheduler players +1 count=" + s_eAI_TickSchedulerPlayers.Count());
	#endif
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (g_Game.IsServer() && !m_eMeleeCombat.eAI_GetWeapon())
		{
			m_eMeleeCombat.Reset(null, EMeleeHitType.NONE, false);

		#ifdef EXPANSION_AI_MELEEDBG_CHATTY
			ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + " DeferredInit - melee weapon (in hands) null range " + m_eMeleeCombat.eAI_GetRange());
		#endif
		}
	}

	override void EEOnCECreate()
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		super.EEOnCECreate();

		eAI_SetLootingBehavior(eAILootingBehavior.ALL);
		GetGroup().SetWaypointBehaviour(eAIWaypointBehavior.ROAMING);
	}

	override void OnDebugSpawn()
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		eAI_SetLootingBehavior(eAILootingBehavior.ALL);
		GetGroup().SetWaypointBehaviour(eAIWaypointBehavior.ROAMING);
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		int persistentCount = eAIGroup.s_PersistentGroups.Count();

		if (persistentCount > 0)
		{
			int time = g_Game.GetTickTime();

			if (time - s_eAI_LastCEUpdateTime > 3.0)
			{
				int lastPersistIndex = persistentCount - 1;
	
				if (eAIGroup.s_eAI_CurrentPersistIndex > lastPersistIndex)
					eAIGroup.s_eAI_CurrentPersistIndex = lastPersistIndex;
				else if (eAIGroup.s_eAI_CurrentPersistIndex == lastPersistIndex)
					eAIGroup.s_eAI_CurrentPersistIndex = 0;
				else
					eAIGroup.s_eAI_CurrentPersistIndex += 1;

				s_eAI_LastCEUpdateTime = time;
			}
		}

		if (!IsAlive())
			return;

		eAIGroup group = GetGroup();
		if (group && group.m_Persist && group.m_BaseName && group.GetFormationLeader() == this)
		{
			//! Persist one AI group per CE update interval (except if no players online, always persist)

			int idx = eAIGroup.s_PersistentGroups.Find(group);

#ifdef EXTRACE
			auto trace = EXTrace.Start(EXTrace.AI, this, "persistent group '" + group.GetName() + "' index " + idx + " current " + eAIGroup.s_eAI_CurrentPersistIndex);
#endif 
				
			if (idx != eAIGroup.s_eAI_CurrentPersistIndex && PlayerBase.Expansion_GetOnlinePlayersCount() > 0)
				return;

#ifdef EXTRACE
			EXTrace.Add(trace, "PERSIST");
#endif 
			
			group.Save(true);

		//#ifdef DIAG_DEVELOPER
			//ExpansionNotification("EXPANSION AI", "Saved group " + group.GetName()).Info();
		//#endif

		/*
			if (IsAlive())
			{
				string basePath = group.GetStorageDirectory();
				if (FileExist(basePath) || ExpansionStatic.MakeDirectoryRecursive(basePath))
				{
					string path = basePath + m_eAI_GroupMemberID.ToString() + ".bin";
					ExpansionEntityStorageModule.SaveToFile(this, path);
				}
			}
		*/
		}
	}

	override void InsertAgent(int agent, float count = 1)
	{
		switch (agent)
		{
			case eAgents.WOUND_AGENT:
				return;
		}

		super.InsertAgent(agent, count);
	}

	void eAI_Recreate()
	{
		InventoryLocation loc = new InventoryLocation();

		if (GetInventory().GetCurrentInventoryLocation(loc))
		{
			SetPosition(vector.Zero);

			EntityAI clone = ExpansionItemSpawnHelper.Clone(this, true, loc);

			if (clone)
			{
				eAIBase ai;
				if (Class.CastTo(ai, clone))
				{
					eAIGroup group = GetGroup();

					ai.SetGroup(group, true, group.GetIndex(this));

				/*
					foreach (string name, eAIAttributeBase attribute: eAIAttributeBase.s_Attributes)
					{
						attribute.Sync(this, ai);  //! Crashes for some people?!?
					}
				*/

					ai.m_eAI_BuildingWithLadder = m_eAI_BuildingWithLadder;
					ai.m_eAI_LadderLoops = m_eAI_LadderLoops;
					ai.m_eAI_Ladders = m_eAI_Ladders;
					ai.m_eAI_LastClimbedBuildingWithLadder = m_eAI_LastClimbedBuildingWithLadder;
					ai.m_eAI_LastClimbedLadder = m_eAI_LastClimbedLadder;
					ai.eAI_SetSniperProneDistanceThreshold(m_eAI_SniperProneDistanceThreshold);
					ai.Expansion_SetCanBeLooted(m_Expansion_CanBeLooted);
					ai.Expansion_SetFormerGroup(Expansion_GetFormerGroup());
					ai.m_eAI_DespawnOnLoosingAggro = m_eAI_DespawnOnLoosingAggro;
					ai.SetMovementSpeedLimits(m_eAI_SpeedLimitPreference, GetMovementSpeedLimitUnderThreat());
					ai.m_eAI_RoamingMovementSpeedLimit = m_eAI_RoamingMovementSpeedLimit;
					ai.m_eAI_RecentlyDroppedItems = m_eAI_RecentlyDroppedItems;
					ai.m_eAI_LastDroppedItem = m_eAI_LastDroppedItem;
					ai.m_eAI_DamageMultiplier = m_eAI_DamageMultiplier;
					ai.m_eAI_DamageReceivedMultiplier = m_eAI_DamageReceivedMultiplier;
					ai.m_eAI_AccuracyMin = m_eAI_AccuracyMin;
					ai.m_eAI_AccuracyMax = m_eAI_AccuracyMax;
					ai.m_eAI_ThreatOverride = m_eAI_ThreatOverride;
					ai.m_eAI_MemeLevel = m_eAI_MemeLevel;
					ai.m_eAI_NoiseInvestigationDistanceLimit = m_eAI_NoiseInvestigationDistanceLimit;
					ai.eAI_SetUnlimitedReload(m_eAI_UnlimitedReload);
					ai.m_eAI_GroupMemberID = m_eAI_GroupMemberID;
					ai.m_eAI_LootingBehavior = m_eAI_LootingBehavior;
					ai.m_eAI_ThreatDistanceLimit = m_eAI_ThreatDistanceLimit;

				#ifdef JM_COT
					foreach (PlayerBase spectator: m_eAI_Spectators)
					{
						GetDayZGame().GetExpansionGame().SpectateAI(spectator.GetIdentity(), ai, null);
					}
				#endif

					//! Since threat overrides point to same instance on original and cloned AI,
					//! we have to sever this connection before original AI deletion because
					//! threat overrides are cleared on AI destruction
					m_eAI_ThreatOverride = new eAIThreatOverride;

					g_Game.ObjectDelete(this);

					return;
				}
				else
				{
					clone.SetPosition(vector.Zero);
					g_Game.ObjectDelete(clone);
				}
			}

			SetPosition(loc.GetPos());
		}
	}

#ifdef DIAG_DEVELOPER
	ref map<EntityAI, bool> m_eAI_PlayerIsEnemyStatus = new map<EntityAI, bool>;
	void eAI_UpdatePlayerIsEnemyStatus(EntityAI player, bool state, string reason)
	{
		bool currentState;
		if (!m_eAI_PlayerIsEnemyStatus.Find(player, currentState) || state != currentState)
		{
			EXPrint(ToString() + " ::eAI_UpdatePlayerIsEnemyStatus " + GetGroup().GetFaction() + " | " + reason + " | " + player.ToString() + " | is enemy? " + state);
			m_eAI_PlayerIsEnemyStatus[player] = state;
		}
	}
#endif

	/**
	 * @brief check if other player is currently considered an enemy
	 * 
	 * @param other Other player entity
	 * @param track Track movement (will return true also for friendly targets)
	 * @param[out] isPlayerMoving Is other player moving?
	 * @param[out] friendly Are we normally friendly towards other player?
	 * @param[out] targeted Are we currently targeting/targeted by other player? (our threat to them or theirs to us above 0.2)
	 */
	bool PlayerIsEnemy(EntityAI other, bool track = false, out bool isPlayerMoving = false, out bool friendly = false, out bool targeted = false)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "PlayerIsEnemy").Add(other);
#endif

		PlayerBase player;
		if (!Class.CastTo(player, other))
		{
#ifdef DIAG_DEVELOPER
			eAI_UpdatePlayerIsEnemyStatus(player, true, "target not a player");
#endif
			return true;
		}

		if (player.Expansion_GetMovementSpeed() > 0 || player.IsClimbing() || player.IsFalling() || player.IsFighting() || player.IsLeaning())
			isPlayerMoving = true;

		if (GetGroup().GetFaction().IsObserver())
		{
			friendly = true;

			//! Interacting players are always "enemies" to observers (will be looked at)
			if (m_eAI_InteractingPlayers.Find(player) > -1)
				return true;

			//! Look at others if they move
			return isPlayerMoving;
		}

		//! Are we targeting them and aggro?
		if (eAI_GetCachedThreat(player.GetTargetInformation()) > 0.2)
			targeted = true;

		if (player.GetGroup())
		{
			if (player.GetGroup() == GetGroup())
			{
#ifdef DIAG_DEVELOPER
				eAI_UpdatePlayerIsEnemyStatus(player, false, "target has same group");
#endif
				friendly = true;

				if (track && GetExpansionSettings().GetAI().MemeLevel > 0 && isPlayerMoving)
					return true;

				return false;
			}

			//! If we are not targeting them, are they targeting us?
			eAIBase ai;
			float ourThreatToThem;
			if (Class.CastTo(ai, player))
				ourThreatToThem = ai.eAI_GetCachedThreat(GetTargetInformation());
			if (!targeted && !player.GetGroup().GetFaction().IsObserver() && ourThreatToThem > 0.2)
				targeted = true;

			//! Other faction friendly to our faction or this specific AI?
			if (player.GetGroup().GetFaction().IsFriendly(GetGroup().GetFaction()) || player.GetGroup().GetFaction().IsFriendlyEntity(this, player))
			{
#ifdef DIAG_DEVELOPER
				if (eAI_IsPassive())
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "passive | target is friendly " + player.GetGroup().GetFaction());
				else
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "target is friendly " + player.GetGroup().GetFaction());
#endif
				friendly = true;

				if (track && GetExpansionSettings().GetAI().MemeLevel > 0 && isPlayerMoving)
					return true;

				return targeted;
			}

			//! If the other player is a guard and we are not, consider them an enemy if they raised their weapon and
			//! either aren't AI or consider us a threat
			if (player.GetGroup().GetFaction().IsGuard())
			{
				bool hostile;
				//! https://feedback.bistudio.com/T173348
				if ((player.IsRaised() || player.IsFighting()) && (!ai || ourThreatToThem >= 0.4))
					hostile = true;
#ifdef DIAG_DEVELOPER
				eAI_UpdatePlayerIsEnemyStatus(player, hostile, "target is guard");
#endif
				return hostile;
			}
		}
		else
		{
			if (!player.GetIdentity() && !player.CanBeTargetedByAI(this))
			{
				if (track && GetExpansionSettings().GetAI().MemeLevel > 0 && isPlayerMoving)
					return true;

				return targeted;
			}
		}

		//! Our faction fiendly to specific player?
		if (GetGroup().GetFaction().IsFriendlyEntity(other, this))
		{
#ifdef DIAG_DEVELOPER
			if (eAI_IsPassive())
				eAI_UpdatePlayerIsEnemyStatus(player, targeted, "passive | friendly | target has no group");
			else
				eAI_UpdatePlayerIsEnemyStatus(player, targeted, "friendly");
#endif
			friendly = true;

			if (track && GetExpansionSettings().GetAI().MemeLevel > 0 && isPlayerMoving)
				return true;

			return targeted;
		}

#ifdef EXPANSIONMODGROUPS
		if (g_Game.IsServer())
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
#ifdef DIAG_DEVELOPER
					eAI_UpdatePlayerIsEnemyStatus(player, targeted, "target party is leader party");
#endif
					friendly = true;

					if (track && GetExpansionSettings().GetAI().MemeLevel > 0 && isPlayerMoving)
						return true;

					return targeted;
				}
			}
		}
#endif

		// at this point we know both we and they have groups, and the groups aren't friendly towards each other
#ifdef DIAG_DEVELOPER
		if (player.Expansion_HasAdminToolInvisibility())
			eAI_UpdatePlayerIsEnemyStatus(player, false, "target invisible");
		else
			eAI_UpdatePlayerIsEnemyStatus(player, true, "not friendly");
#endif
		return true;
	}

	void TryFireWeapon()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "TryFireWeapon");
#endif

		if (g_Game.GetTime() < m_eAI_MinTimeTillNextFire)
			return;

		Weapon_Base weapon = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
		if (!weapon)
			return;

		if (!eAI_CanFire(weapon))
			return;

		if (!eAI_HasLOS())
			return;

		if (m_eAI_QueuedShots == 0)
		{
			//! Determine firing mode

			int burst;

			switch (weapon.Expansion_GetFireMode())
			{
				case ExpansionFireMode.Burst:
					burst = weapon.GetCurrentModeBurstSize(weapon.GetCurrentMuzzle());
					if (weapon.GetBurstCount() == burst)
						weapon.ResetBurstCount();
					break;

				case ExpansionFireMode.FullAuto:
					Magazine mag;
					if (Class.CastTo(mag, weapon.GetMagazine(weapon.GetCurrentMuzzle())))
						burst = mag.GetAmmoMax() * 0.25;
					break;
			}

			m_eAI_QueuedShots = 1;

			if (burst > 1)
			{
				if (m_eAI_DistanceToTargetSq < 200)
					m_eAI_QueuedShots = Math.Ceil(((200 - m_eAI_DistanceToTargetSq) / 200) * burst);
			}
		}

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "Firing, queued shots: " + m_eAI_QueuedShots);
	#endif

		GetWeaponManager().Fire(weapon);

		if (m_eAI_QueuedShots > 0)
			m_eAI_QueuedShots--;

		if (m_eAI_QueuedShots > 0)
			m_eAI_MinTimeTillNextFire = g_Game.GetTime() + weapon.GetReloadTime(weapon.GetCurrentMuzzle()) * 1000;
		else
			m_eAI_MinTimeTillNextFire = g_Game.GetTime() + Math.RandomIntInclusive(200, 300);
	}

	void eAI_FireWeaponOnClient(int muzzleIndex, int shotID)
	{
	#ifdef SERVER
		auto rpc = ExpansionScriptRPC.Create(m_eAI_FireWeaponOnClient_RPCID);
		//! Pack muzzleIndex and shotID into one integer.
		//! 8 bits should be enough for muzzleIndex and 24 for shotId seems sufficient as well
		//! (no issue either if the latter wraps)
		int packed = ((muzzleIndex & 0xff) << 24) | (shotID & 0xffffff);
		rpc.Write(packed);
		rpc.Expansion_Send(this, true);
	#else
		Weapon_Base weapon;
		if (Class.CastTo(weapon, GetHumanInventory().GetEntityInHands()))
		{
			//! Since muzzleIndex can come from RPC, check it (better safe than sorry, game could CTD if muzzleIndex is invalid otherwise)
			if (muzzleIndex >= weapon.GetMuzzleCount())
				CF.FormatError("Invalid muzzle index %1 for %2", muzzleIndex.ToString(), weapon.ToString());
			else
				weapon.eAI_FireOnClient(muzzleIndex, this, shotID);
		}
		else if (GetExpansionSettings().GetAI().RecreateWeaponNetworkRepresentation)
		{
			EXError.Warn(this, "Couldn't fire weapon (NULL), attempting to recreate network representation");
			auto rpc = ExpansionScriptRPC.Create(m_eAI_RemoteRecreateWeapon_RPCID);
			rpc.Expansion_Send(this, true);
		}
		else
		{
			EXError.Error(this, "Couldn't fire weapon (NULL)");
		}
	#endif
	}

	//! Client
	void RPC_eAI_FireWeaponOnClient(PlayerIdentity sender, ParamsReadContext ctx)
	{
		int packed;
		if (!ctx.Read(packed))
			return;

		int muzzleIndex = (packed >> 24) & 0xff;
		int shotID = packed & 0xffffff;

		eAI_FireWeaponOnClient(muzzleIndex, shotID);
	}

	//! Server
	void RPC_eAI_RemoteRecreateWeapon(PlayerIdentity sender, ParamsReadContext ctx)
	{
		Weapon_Base weapon;
		if (Class.CastTo(weapon, GetHumanInventory().GetEntityInHands()))
		{
			weapon.eAI_RemoteRecreate();
			EXError.Warn(this, "Recreated network representation of " + weapon);
		}
		else
		{
			EXError.Warn(this, "No weapon in hands");
		}
	}

	/**
	 * @brief select firemode based on target, distance, and weapon
	 * 
	 * @note currently only deals with double barrel (i.e. Blaze and BK-43) as there is no real need to ever change firemode on any other gun
	 * because when gun enters AI hands, firemode is set to fullauto (e.g. M4A1) or burst (e.g. M16) if supported.
	 * The firing code in TryFireWeapon deals with the amount of, and time between, shots fired in fullauto and burst modes,
	 * which gives the appearance of semi-auto in those modes if only one shot is queued.
	 */
	void eAI_SelectFireMode(eAITarget target, float distSq, Weapon_Base weapon)
	{
		bool changedFireMode;

		if (weapon.m_Expansion_WeaponInfo.m_FireModes.Contains(ExpansionFireMode.Double))
		{
			if (distSq <= 30 && target.IsPlayer())
				weapon.Expansion_SetFireMode(ExpansionFireMode.Double, changedFireMode);
			else
				weapon.Expansion_SetFireMode(ExpansionFireMode.Single, changedFireMode);

			m_eAI_QueuedShots = 1;  //! @note for firemode double, one queued shot is two projectiles
		}

		if (changedFireMode)
			m_eAI_MinTimeTillNextFire = g_Game.GetTime() + Math.RandomIntInclusive(200, 300);
	}

	bool eAI_CanFire(Weapon_Base weapon)
	{
		if (IsClimbing() || IsFalling() || IsFighting() || IsSwimming())
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

	bool eAI_ShouldBandage()
	{
		if (!IsBleeding())
			return false;

		if (m_eAI_DangerousAreaCount > 0 && m_eAI_IsInDangerByArea)
			return false;
		
		if (m_eAI_CurrentThreatToSelfActive >= 0.4 || m_eAI_IsFightingFSM)
		{
			if (g_Game.GetTickTime() - m_eAI_LastHitTime <= 10)
				return false;

			if (GetHealth01("", "Blood") >= 0.7 && m_eAI_Targets.Count() > 0 && !GetTarget().IsItem() && !GetTarget().IsNoise())
				return false;
		}

		return true;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		m_eAI_FlankTime = 0.0;

		if (source && source.IsDayZCreature() && m_eAI_ThreatOverride.Contains(source))
			eAI_ThreatOverride(source, false);

		//! If we are currently blocking, go out of block so we can start going on offense
		if (m_eAI_MeleeFightLogic.IsInBlock())
			m_eAI_MeleeFightLogic.eAI_EndBlock();
	}

	override void EEHitByRemote(int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		if (!g_Game.IsDedicatedServer())
			super.EEHitByRemote(damageType, source, component, dmgZone, ammo, modelPos);
	#ifdef DIAG_DEVELOPER
		else if (m_MeleeFightLogic.IsInBlock())
			EXError.Warn(this, "EEHitByRemote called on server while in block");
	#endif
	}

	override void EEKilled(Object killer)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + killer);
	#endif

		super.EEKilled(killer);

		if (g_Game.IsServer())
		{
			if (!m_Expansion_EmoteAutoCancel)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_PlayEmote);

			eAIGroup group = GetGroup();
			if (group && group.m_Persist && group.m_BaseName)
				eAI_DeletePersistentFiles();
		}

		if (m_eAI_LootDropOnDeath)
		{
			foreach (ExpansionPrefab lootDropOnDeath: m_eAI_LootDropOnDeath)
			{
				if (lootDropOnDeath.CanSpawn())
					Object obj = lootDropOnDeath.Spawn(ExpansionMath.GetRandomPointInCircle(GetPosition(), 0.5), vector.Zero);
			}
		}
	}

	override protected void eAI_Cleanup(bool autoDeleteGroup = false)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		array<ref eAITargetInformation> targets = {};
		foreach (eAITargetInformation info, eAITarget state: m_eAI_TargetInformationStates)
		{
			if (info)
				targets.Insert(info);
		}

		foreach (eAITargetInformation target: targets)
		{
			target.RemoveAI(this);
		}

		if (m_eAI_TargetInformationStates.Count())
		{
			eAI_DebugTargetInformationStates();

			m_eAI_TargetInformationStates.Clear();
		}

		super.eAI_Cleanup(autoDeleteGroup);

		eAIGroup group = GetGroup();
		if (group)
			group.SetIsInCombat(false);

		if (g_Game.IsServer() && !IsDamageDestroyed())
			s_Expansion_AllPlayers.m_OnRemove.Remove(eAI_OnRemovePlayer);

		if (m_eAI_ClientUpdateTimer && m_eAI_ClientUpdateTimer.IsRunning())
			m_eAI_ClientUpdateTimer.Stop();

		if (g_Game.IsServer())
			eAINoiseSystem.SI_OnNoiseAdded.Remove(eAI_OnNoiseEvent);

		if (m_eAI_CurrentCoverObject)
			s_eAI_TakenCoverObjects.RemoveItem(m_eAI_CurrentCoverObject);
	}

	void eAI_DebugTargetInformationStates()
	{
		int i;

		foreach (eAITargetInformation info, eAITarget state: m_eAI_TargetInformationStates)
		{
			if (info)
				EXPrint(this, "Debug: Not removed during target cleanup " + info.GetDebugName() + " " + state);
			else
				EXPrint(this, "Debug: NULL key in target information at index " + i + " " + state);

			++i;
		}
	}

	override bool IsAI()
	{
		return true;
	}

	override bool Expansion_IsAI()
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
		if (m_eAI_BandageToUse)
		{
			if (m_eAI_BandageToUse.IsDamageDestroyed())
				m_eAI_BandageToUse = null;
			else
				return m_eAI_BandageToUse;
		}

		foreach (ItemBase bandage: m_eAI_Bandages)
		{
			if (bandage && !bandage.IsDamageDestroyed())
			{
				m_eAI_BandageToUse = bandage;
				return bandage;
			}
		}

		return null;
	}

	ItemBase eAI_GetRepairKit(EntityAI entity)
	{
		foreach (ItemBase repairKit: m_eAI_RepairKits)
		{
			if (repairKit && !repairKit.IsDamageDestroyed() && repairKit.Expansion_CanRepair(entity))
				return repairKit;
		}

		eAITarget target = GetTarget();

		if (target && target.GetDistanceSq(true) <= 4.0)
		{
			EntityAI targetEntity = target.GetEntity();
			ItemBase item;
			if (Class.CastTo(item, targetEntity) && item.Expansion_CanRepair(entity))
				return item;
		}

		return null;
	}

	Weapon_Base eAI_GetAnyWeaponToUse(bool requireAmmo = false, bool preferExplosiveAmmo = false)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(EXTrace.AI, this).Add(requireAmmo);
#endif

		Weapon_Base weapon = eAI_GetWeaponToUse(requireAmmo, preferExplosiveAmmo);
		if (!weapon)
			weapon = eAI_GetWeaponToUse(requireAmmo, !preferExplosiveAmmo);

		return weapon;
	}

	Weapon_Base eAI_GetWeaponToUse(bool requireAmmo = false, bool preferExplosiveAmmo = false)
	{
		if (preferExplosiveAmmo)
			return eAI_GetWeaponToUse(m_eAI_Launchers, requireAmmo);

		Weapon_Base weapon = eAI_GetWeaponToUse(m_eAI_Firearms, requireAmmo);
		if (weapon)
			return weapon;

		return eAI_GetWeaponToUse(m_eAI_Handguns, requireAmmo);
	}

	Weapon_Base eAI_GetWeaponToUse(array<Weapon_Base> weapons, bool requireAmmo = false)
	{
		Magazine mag;

		//! Iteration in reverse order so last weapon added is the preferred one
		for (int i = weapons.Count() - 1; i >= 0; --i)
		{
			Weapon_Base weapon = weapons[i];

			if (weapon && !weapon.IsDamageDestroyed())
			{
				if (!requireAmmo)
					return weapon;

				if (eAI_HasAmmoForFirearm(weapon, mag, false))
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

		//! Iteration in reverse order so last melee weapon added is the preferred one
		for (int i = m_eAI_MeleeWeapons.Count() - 1; i >= 0; --i)
		{
			ItemBase melee = m_eAI_MeleeWeapons[i];

			if (melee && !melee.IsDamageDestroyed())
			{
				return melee;
			}
		}

		return null;
	}

	//! Unlike GetMagazineToReload, this can be used to check if there is a mag/ammo for a weapon that is not in hands
	Magazine eAI_GetMagazineToReload(Weapon_Base weapon, bool unlimitedReload = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase, weapon.ToString(), "unlimitedReload " + unlimitedReload);
#endif
		
		eAIWeaponManager weapon_manager = eAIWeaponManager.Cast(GetWeaponManager());

		Magazine ammo_magazine; // magazine
		int last_ammo_magazine_count;
		int ammo_magazine_count;

		Magazine ammo_pile; // ammo pile
		int last_ammo_pile_count;
		int ammo_pile_count;

		// Get all magazines in (player) inventory
		foreach (Magazine magazine: m_eAI_Magazines)
		{
			if (!magazine)
				continue;

			ammo_pile_count = magazine.GetAmmoCount();

			// magazines (get magazine with max ammo count)
			if (weapon_manager.CanAttachMagazine_NoHandsCheck(weapon, magazine) || weapon_manager.CanSwapMagazine_NoHandsCheck(weapon, magazine))
			{
				if (ammo_pile_count > 0 || unlimitedReload)
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
				}
			}
			// bullets (get ammo pile with min ammo count)
			else if (weapon_manager.CanLoadBullet_NoHandsCheck_NoChamberCheck(weapon, magazine))
			{
				if (ammo_pile_count > 0 || unlimitedReload)
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
				}
			}
		#ifdef DIAG_DEVELOPER
			else
			{
				EXTrace.Print(EXTrace.AI, this, "eAI_GetMagazineToReload - can not attach/swap/load " + magazine.GetType());
			}
		#endif
		}

		// prioritize magazine
		Magazine chosen = ammo_pile;

		if (ammo_magazine)
		{
			if (ammo_pile)
			{
				int mi = weapon.GetCurrentMuzzle();
				Magazine currentMag = weapon.GetMagazine(mi);

				//! Prefer larger or same size mag if we have current attached mag
				//! @note ReloadWeaponAI deals with detaching & filling up current attached mag
				//! if its capacity is larger and ammo pile is larger than mag in inventory
				if (!currentMag || currentMag.GetAmmoMax() <= ammo_magazine.GetAmmoMax() || (!unlimitedReload && last_ammo_magazine_count >= last_ammo_pile_count))
					chosen = ammo_magazine;
			}
			else
			{
				chosen = ammo_magazine;
			}
		}

#ifdef DIAG_DEVELOPER
		if (chosen == ammo_magazine)
			EXTrace.Print(EXTrace.AI, this, "eAI_GetMagazineToReload " + ammo_magazine + " ammo count " + last_ammo_magazine_count);
		else
			EXTrace.Print(EXTrace.AI, this, "eAI_GetMagazineToReload " + ammo_pile + " ammo count " + last_ammo_pile_count);
#endif

		return chosen;
	}

	void eAI_SetUnlimitedReload(int unlimitedReload)
	{
		m_eAI_UnlimitedReload = unlimitedReload;

		if (unlimitedReload == eAITargetType.ALL || unlimitedReload == eAITargetType.ALL_OR)
			m_eAI_IsUnlimitedReloadAll = true;
		else
			m_eAI_IsUnlimitedReloadAll = false;
	}

	bool eAI_IsTargetUnlimitedReload()
	{
		if (s_eAI_UnlimitedReloadAll)
			return true;

		if (m_eAI_IsUnlimitedReloadAll)
			return true;

		if (!m_eAI_LastEngagedTargetType)
			return false;

		int unlimitedReload = s_eAI_UnlimitedReload | m_eAI_UnlimitedReload;

		switch (m_eAI_LastEngagedTargetType)
		{
			case eAICreatureTargetInformation:
				if (unlimitedReload & eAITargetType.ANIMAL)
					return true;
				break;

			case eAIZombieTargetInformation:
				if (unlimitedReload & eAITargetType.INFECTED)
					return true;
				break;

			case eAIPlayerTargetInformation:
				if (unlimitedReload & eAITargetType.PLAYER)
					return true;
				break;

			case eAIVehicleTargetInformation:
				if (unlimitedReload & eAITargetType.VEHICLE)
					return true;
				break;
		}

		return false;
	}

	void eAI_SetAccuracy(float accuracyMin, float accuracyMax)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, accuracyMin);
		EXTrace.Add(trace, accuracyMax);
#endif 
		
		if (accuracyMin <= 0)
			accuracyMin = GetExpansionSettings().GetAI().AccuracyMin;

		m_eAI_AccuracyMin = accuracyMin;

		if (accuracyMax <= 0)
			accuracyMax = GetExpansionSettings().GetAI().AccuracyMax;

		m_eAI_AccuracyMax = accuracyMax;

#ifdef EXTRACE
		EXTrace.Add(trace, m_eAI_AccuracyMin);
		EXTrace.Add(trace, m_eAI_AccuracyMax);
#endif 

		SetSynchDirty();
	}

	void eAI_SetThreatDistanceLimit(float distance)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, distance);
#endif 

		if (distance <= 0)
		{
			auto settings = GetExpansionSettings().GetAI();
			distance = settings.ThreatDistanceLimit;

			if (distance <= 0)
			{
				EXError.Error(this, "Invalid ThreatDistanceLimit value " + settings.ThreatDistanceLimit);
				distance = 1000;
			}
		}

		m_eAI_ThreatDistanceLimit = distance;

#ifdef EXTRACE
		EXTrace.Add(trace, distance);
#endif 
	}

	void eAI_SetNoiseInvestigationDistanceLimit(float distance)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, distance);
#endif 

		if (distance <= 0)
			distance = GetExpansionSettings().GetAI().NoiseInvestigationDistanceLimit;

		m_eAI_NoiseInvestigationDistanceLimit = distance;

#ifdef EXTRACE
		EXTrace.Add(trace, distance);
#endif 
	}

	void eAI_SetMaxFlankingDistance(float distance)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, distance);
#endif 

		if (distance <= 0)
			distance = GetExpansionSettings().GetAI().MaxFlankingDistance;

		if (distance > 0)
			m_eAI_MaxFlankingDistance = distance;

#ifdef EXTRACE
		EXTrace.Add(trace, m_eAI_MaxFlankingDistance);
#endif 
	}

	void eAI_SetEnableFlankingOutsideCombat(int option)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, option);
#endif 

		if (option <= 0)
			option = GetExpansionSettings().GetAI().EnableFlankingOutsideCombat;

		if (option > 0)
			m_eAI_EnableFlankingOutsideCombat = true;

#ifdef EXTRACE
		EXTrace.Add(trace, m_eAI_EnableFlankingOutsideCombat);
#endif 
	}

	void eAI_SetDamageMultiplier(float multiplier)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, multiplier);
#endif 

		if (multiplier <= 0)
			multiplier = GetExpansionSettings().GetAI().DamageMultiplier;

		m_eAI_DamageMultiplier = multiplier;

#ifdef EXTRACE
		EXTrace.Add(trace, multiplier);
#endif 
	}

	void eAI_SetDamageReceivedMultiplier(float multiplier)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
		EXTrace.Add(trace, multiplier);
#endif 

		if (multiplier <= 0)
			multiplier = GetExpansionSettings().GetAI().DamageReceivedMultiplier;

		m_eAI_DamageReceivedMultiplier = multiplier;

#ifdef EXTRACE
		EXTrace.Add(trace, multiplier);
#endif 
	}

	void eAI_SetLootingBehavior(int bitmask)
	{
		m_eAI_LootingBehavior = bitmask;

		array<EntityAI> toRemove = {};

		foreach (EntityAI entity, bool state: m_eAI_ThreatOverride)
		{
			if (entity && entity.IsItemBase())
				toRemove.Insert(entity);
		}

		foreach (EntityAI item: toRemove)
		{
			m_eAI_ThreatOverride.Remove(item);
		}
	}
	
	override void SetFallYDiff(float value)
	{
		super.SetFallYDiff(value);

		m_eAI_FallHasLanded = false;
		m_eAI_FallYVelZeroTime = 0;
	}

	//! 1.29 CTD fix for Man::GetEntityInHands used by vanilla
	override ItemBase GetItemInHands()
	{
		HumanInventory humanInventory = GetHumanInventory();
		if (humanInventory)
			return ItemBase.Cast(humanInventory.GetEntityInHands());

		return null;
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

	void eAI_AddTarget(eAITarget target)
	{
		m_eAI_Targets.Insert(target);
		if (m_eAI_Targets.Count() == 1)
			m_eAI_SyncCurrentTarget = true;
#ifdef DIAG_DEVELOPER
		m_eAI_PrintCurrentTarget = true;
		EXTrace.Print(EXTrace.AI, this, "eAI_AddTarget " + target.GetDebugName() + " - found at time " + target.m_FoundAtTime + " - max time " + target.m_MaxTime + " - target count " + m_eAI_Targets.Count());
#endif
	}

	void eAI_RemoveTarget(eAITarget target)
	{
		int removeIndex = m_eAI_Targets.Find(target);
		if (removeIndex >= 0)
		{
			m_eAI_Targets.RemoveOrdered(removeIndex);
			if (removeIndex == 0)
				m_eAI_SyncCurrentTarget = true;
			if (removeIndex < m_eAI_NoiseTarget)
				m_eAI_NoiseTarget--;
		}
		eAI_RemoveTargetInfoState(target.m_Info);
		if (target.IsNoise())
		{
			if (m_eAI_CurrentThreatToSelfActive < 0.4 && !Math.RandomInt(0, 3))
				Expansion_SetEmote(EmoteConstants.ID_EMOTE_SHRUG, true);
			m_eAI_NoiseTargetInfo.SetNoiseParams(null, vector.Zero, 0.0, 0.0, 0.0);
		}
#ifdef DIAG_DEVELOPER
		m_eAI_PrintCurrentTarget = true;
		EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTarget " + target.GetDebugName() + " - time remaining " + (target.m_FoundAtTime + target.m_MaxTime - g_Game.GetTime()) + " - target count " + m_eAI_Targets.Count());
#endif
	}

	void eAI_RemoveTargetInfoState(eAITargetInformation info)
	{
#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTargetInfoState " + info.GetDebugName());
#endif
		m_eAI_TargetInformationStates.Remove(info);
		ItemBase item;
		if (Class.CastTo(item, info.GetEntity()))
		{
			eAI_ThreatOverride(item, false);
		}
	}

	void eAI_UpdateAcuteDangerTargetCount(eAITarget target, int delta)
	{
		switch (target.m_Info.Type())
		{
			case eAICreatureTargetInformation:
			case eAIZombieTargetInformation:
				m_eAI_AcuteDangerTargetCount += delta;
				break;

			case eAIPlayerTargetInformation:
				m_eAI_AcuteDangerPlayerTargetCount += delta;
				break;
		}
	}

	bool eAI_IsLowVitals()
	{
		if (GetHealth01("", "") <= 0.5 || GetHealth01("", "Blood") < 0.7)
			return true;

		return false;
	}

	float GetThreatToSelf(bool ignoreLOS = false)
	{
		if (ignoreLOS)
			return m_eAI_CurrentThreatToSelf;

		return m_eAI_CurrentThreatToSelfActive;
	}

	EntityAI eAI_GetTargetEntity()
	{
		eAITarget target = GetTarget();
		if (target)
			return target.GetEntity();

		return null;
	}

	//! @note all targets except item targets (no state)
	float eAI_GetCachedThreat(eAITargetInformation info, bool ignoreLOS = false)
	{
		eAITarget state;
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

			eAITargetInformationState state = target;
			m_eAI_CurrentThreatToSelf = target.GetThreat();

			if (!state)
			{
				m_eAI_CurrentThreatToSelfActive = m_eAI_CurrentThreatToSelf;
			}
			else
			{
				m_eAI_CurrentThreatToSelfActive = state.m_ThreatLevelActive;
			}
		}
		else
		{
			m_eAI_CurrentThreatToSelf = 0.0;
			m_eAI_CurrentThreatToSelfActive = 0.0;
		}

		if (m_eAI_PreviousThreatToSelf != m_eAI_CurrentThreatToSelf)
			m_eAI_Callbacks.OnThreatLevelChanged(m_eAI_PreviousThreatToSelf, m_eAI_CurrentThreatToSelf);

		if (m_eAI_PreviousThreatToSelfActive != m_eAI_CurrentThreatToSelfActive)
			m_eAI_Callbacks.OnActiveThreatLevelChanged(m_eAI_PreviousThreatToSelfActive, m_eAI_CurrentThreatToSelfActive);
	}

	/*!
	 * @brief Get state for given target info.
	 * New state will be created if none exists.
	 * 
	 * @param[out] created Will be set to true if new state was created.
	 * 
	 * @return eAITarget
	 */
	eAITarget eAI_GetTargetInformationState(eAITargetInformation info, bool initialUpdate = true, out bool created = false)
	{
		eAITarget state;
		if (!m_eAI_TargetInformationStates.Find(info, state))
		{
#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, this, "Adding new target info state for " + info.GetDebugName());
#endif
			return info.GetTargetForAIEx(this, initialUpdate, created);
		}

		return state;
	}

	bool eAI_SkipMelee(string msg, eAITarget state, bool skip = false)
	{
		if ((skip && !state.m_SkipMelee) || (!skip && state.m_SkipMelee))
		{
		#ifdef DIAG_DEVELOPER
			msg = string.Format("[MELEE] %1 (pos=%2) %3 (tgt=%4)", this, ExpansionStatic.VectorToString(GetPosition()), msg, state.GetEntity());
			EXPrint(msg);
			ExpansionStatic.MessageNearPlayers(GetPosition(), 100, msg);
		#endif

			state.m_SkipMelee = skip;

			return true;
		}

		return false;
	}

	void eAI_SetDespawnOnLoosingAggro(bool state)
	{
		m_eAI_DespawnOnLoosingAggro = state;
	}

	void eAI_Despawn()
	{
		Delete();
	}

	TStringArray eAI_DumpState(PlayerIdentity identity, bool includeGroup = true, string indent = string.Empty)
	{
		bool isAdmin;
		if (GetExpansionSettings().GetAI().IsAdmin(identity))
			isAdmin = true;

		TStringArray report = {};

		report.Insert(indent + string.Format("%1", ExpansionStatic.GetDebugInfo(this)));

		if (indent)
		{
			indent.Replace("\\", ".");
			indent.Replace("-", " ");
		}

		report.Insert(indent + string.Format("|- Name %1", GetCachedName()));

		report.Insert(indent + string.Format("|- Position %1", ExpansionStatic.VectorToString(GetPosition(), ExpansionVectorToString.Plain)));

		float visibility = Math.Round(m_Expansion_Visibility * 1000);
		report.Insert(indent + string.Format("|- Visibility %1 m", visibility));
		report.Insert(indent + string.Format("|- Visibility distance threshold %1 m", m_Expansion_VisibilityDistThreshold));
		report.Insert(indent + string.Format("|- Visibility limits", visibility));
		report.Insert(indent + string.Format("|  |- Daylight %1 (threshold %2)", m_Expansion_DaylightVisibility, m_eAI_DayNightThreshold));
		report.Insert(indent + string.Format("|  |- Night %1", m_eAI_NightVisibility));
		report.Insert(indent + string.Format("|  |- Overcast %1", m_eAI_OvercastVisibility));
		report.Insert(indent + string.Format("|  |- Rain %1", m_eAI_RainVisibility));
		report.Insert(indent + string.Format("|  |- Snow %1", m_eAI_SnowVisibility));
		report.Insert(indent + string.Format("|  |- Fog %1", m_eAI_FogVisibility));
		report.Insert(indent + string.Format("|  |- Volumetric fog %1", m_eAI_DynVolFogVisibility));
		report.Insert(indent + string.Format("|  |- Contaminated area %1", m_eAI_ContaminatedAreaVisibility));
		report.Insert(indent + string.Format("|  |- Minimum %1", m_eAI_MinVisibility));
		report.Insert(indent + string.Format("|  |- Maximum %1", m_eAI_VisibilityLimit));
		report.Insert(indent + string.Format("|  \\- Base %1", m_eAI_BaseVisibility));
		report.Insert(indent + string.Format("|- Active visibility enhancers %1", m_Expansion_ActiveVisibilityEnhancers.Count()));
		foreach (ItemBase visEnh: m_Expansion_ActiveVisibilityEnhancers)
		{
			report.Insert(indent + string.Format("|  |- %1 (on %2) %3", ExpansionStatic.GetDebugInfo(visEnh), ExpansionStatic.GetDebugInfo(visEnh.GetHierarchyParent()), visEnh.eAI_GetNightVisibility()));
		}
		if (m_Expansion_ActiveVisibilityEnhancers.Count() > 0)
			eAI_FixupLastReportEntry(report);

		report.Insert(indent + string.Format("|- Stance %1", typename.EnumToString(eAIStance, eAI_GetStance())));

		ItemBase itemInHands = GetItemInHands();

		if (IsRaised())
		{
			if (itemInHands && (itemInHands.IsWeapon() || itemInHands.Expansion_IsMeleeWeapon()))
				report.Insert(indent + "|  \\- Weapon raised");
			else
				report.Insert(indent + "|  \\- Hands raised");
		}

		report.Insert(indent + string.Format("|- Speed limit normal/combat %1/%2", typename.EnumToString(eAIMovementSpeed, m_MovementSpeedLimit), typename.EnumToString(eAIMovementSpeed, m_MovementSpeedLimitUnderThreat)));
		report.Insert(indent + string.Format("|- Current velocity %1 m/s", ExpansionStatic.FloatToString(Expansion_GetActualVelocity().Length())));

		if (includeGroup)
			report.InsertAll(GetGroup().DumpState(identity, false, indent + "|- "));

		bool isGroupLeader;
		if (GetGroup().GetLeader() == this)
			isGroupLeader = true;
		report.Insert(indent + string.Format("|- Is group leader %1", isGroupLeader.ToString()));

		bool isFormLeader;
		if (GetGroup().GetFormationLeader() == this)
			isFormLeader = true;
		report.Insert(indent + string.Format("|- Is formation leader %1", isFormLeader.ToString()));

		report.Insert(indent + string.Format("|- Accuracy min %1%% max %2%%", ExpansionStatic.FloatToString(m_eAI_AccuracyMin * 100), ExpansionStatic.FloatToString(m_eAI_AccuracyMax * 100)));
		report.Insert(indent + string.Format("|- Damage in %1%% out %2%%", m_eAI_DamageReceivedMultiplier * 100, m_eAI_DamageMultiplier * 100));
		report.Insert(indent + string.Format("|- Headshot resistance %1%%", m_eAI_HeadshotResistance * 100));
		report.Insert(indent + string.Format("|- Threat distance limit %1 m", ExpansionStatic.FloatToString(m_eAI_ThreatDistanceLimit)));
		report.Insert(indent + string.Format("|- Noise investigation distance limit %1 m", ExpansionStatic.FloatToString(m_eAI_NoiseInvestigationDistanceLimit)));
		report.Insert(indent + string.Format("|- Sniper prone distance threshold %1 m", ExpansionStatic.FloatToString(m_eAI_SniperProneDistanceThreshold)));

		report.Insert(indent + "|- FOV");
		report.Insert(indent + string.Format("|  |- Near to far FOV transition exponent %1", ExpansionStatic.FormatFloat(m_eAI_FOVTransitionExponent, 4, false, false)));
		report.Insert(indent + string.Format("|  \\- Distance (m)   FOV (°)"));

		if (m_eAI_FOVFar_DistThreshold != m_eAI_FOVNear_DistThreshold && m_eAI_FOVFar_HalfAngleH != m_eAI_FOVNear_HalfAngleH)
		{
			float fovDistRange = m_eAI_FOVFar_DistThreshold - m_eAI_FOVNear_DistThreshold;
			int fovIndexMax = 8;
			float fovDistInc = fovDistRange / fovIndexMax;
			for (int fovIndex = 0; fovIndex <= fovIndexMax; ++fovIndex)
			{
				float fovDist = m_eAI_FOVNear_DistThreshold + fovDistInc * fovIndex;
				float fovHalfAngleH = eAI_CalculateFOVHalfAngleH(fovDist);
				report.Insert(indent + string.Format("|     |- %1    %2",
													 ExpansionString.JustifyLeft(ExpansionStatic.FormatFloat(fovDist, 3, false, false), 8, " "),
													 ExpansionStatic.FormatFloat(fovHalfAngleH * 2, 2, false, false)));
			}
		}
		eAI_FixupLastReportEntry(report);

		report.Insert(indent + "|- Looting behavior");
		TStringArray lootingBehaviors = {};
		ExpansionStatic.BitmaskEnumToString(eAILootingBehavior, m_eAI_LootingBehavior).Split("|", lootingBehaviors);
		foreach (string behavior: lootingBehaviors)
		{
			report.Insert(indent + string.Format("|  |- %1", behavior));
		}
		eAI_FixupLastReportEntry(report);

		report.Insert(indent + "|- Unlimited reload");
		TStringArray unlimitedReload = {};
		ExpansionStatic.BitmaskEnumToString(eAITargetType, m_eAI_UnlimitedReload).Split("|", unlimitedReload);
		foreach (string targetType: unlimitedReload)
		{
			report.Insert(indent + string.Format("|  |- %1", targetType));
		}
		eAI_FixupLastReportEntry(report);

		auto fsmState = GetFSM().GetState();
		if (fsmState)
		{
			auto subFSM = fsmState.GetSubFSM();
			ExpansionState subFSMState;
			if (subFSM)
				subFSMState = subFSM.GetState();
			if (subFSMState)
				report.Insert(indent + string.Format("|- FSM state %1->%2", fsmState.GetName(), subFSMState.GetName()));
			else
				report.Insert(indent + string.Format("|- FSM state %1", fsmState.GetName()));
		}

		int pointIdx = m_PathFinding.m_PointIdx;
		if (m_PathFinding.m_Count >= 2)
			pointIdx += 2;

		report.Insert(indent + string.Format("|- Path progression %1/%2", pointIdx, m_PathFinding.m_Count));

		auto hcm = GetCommand_Move();
		if (hcm)
		{
			report.Insert(indent + string.Format("|  |- Current point %1",
												 ExpansionStatic.VectorToString(m_eAI_CommandMove.GetWaypoint(),
																				ExpansionVectorToString.Plain)));
			report.Insert(indent + string.Format("|  |  \\- 2D distance to AI %1 m", m_eAI_CommandMove.GetWaypointDistance2D()));
		}

		report.Insert(indent + string.Format("|  |- Endpoint %1",
											 ExpansionStatic.VectorToString(m_PathFinding.GetEnd(),
																			ExpansionVectorToString.Plain)));
		report.Insert(indent + string.Format("|  |  \\- 2D distance to AI %1 m",
											 ExpansionMath.Distance2D(GetPosition(), m_PathFinding.GetEnd())));
		report.Insert(indent + string.Format("|  |- Target position %1 (%2)",
											 ExpansionStatic.VectorToString(m_PathFinding.GetTarget(),
																			ExpansionVectorToString.Plain),
											 ExpansionStatic.VectorToString(m_PathFinding.m_TargetPosition,
																			ExpansionVectorToString.Plain)));
		report.Insert(indent + string.Format("|  |  |- 2D distance to AI %1 m",
											 ExpansionMath.Distance2D(GetPosition(), m_PathFinding.GetTarget())));

		bool reachable;
		if (!m_PathFinding.m_IsUnreachable)
			reachable = true;

		bool targetReachable;
		if (!m_PathFinding.m_IsTargetUnreachable)
			targetReachable = true;

		report.Insert(indent + string.Format("|  |  |- Is reachable %1 (%2)", reachable.ToString(), targetReachable.ToString()));
		report.Insert(indent + string.Format("|  |  \\- Is final %1", m_eAI_TargetPositionIsFinal.ToString()));

		report.Insert(indent + string.Format("|  \\- Is AI position final %1", m_eAI_PositionIsFinal.ToString()));

		if (m_eAI_CurrentCoverObject)
		{
			report.Insert(indent + string.Format("|- Current cover object %1", ExpansionStatic.GetDebugInfo(m_eAI_CurrentCoverObject)));
			report.Insert(indent + string.Format("|  |- Position %1",
												 ExpansionStatic.VectorToString(m_eAI_CurrentCoverObject.GetPosition(),
																				ExpansionVectorToString.Plain)));
			report.Insert(indent + string.Format("|  \\- 2D distance to AI %1 m",
												 ExpansionMath.Distance2D(GetPosition(), m_eAI_CurrentCoverObject.GetPosition())));
		}

		if (m_eAI_CurrentCoverPosition != vector.Zero)
		{
			report.Insert(indent + string.Format("|- Current cover position %1",
												 ExpansionStatic.VectorToString(m_eAI_CurrentCoverPosition,
																				ExpansionVectorToString.Plain)));
			report.Insert(indent + string.Format("|  |- 2D distance to AI %1 m", ExpansionMath.Distance2D(GetPosition(), m_eAI_CurrentCoverPosition)));
			report.Insert(indent + string.Format("|  \\- Is AI in cover %1", m_eAI_IsInCover.ToString()));
		}


		vector lookAnglesTarget = m_eAI_LookDirectionTarget_ModelSpace.VectorToAngles();
		string look = string.Format("%1° %2°", ExpansionMath.RelAngle(lookAnglesTarget[0]), ExpansionMath.RelAngle(lookAnglesTarget[1]));
		if (m_eAI_LookDirection_Recalculate)
			look += " (tracking)";
		else
			look += " (static)";

		report.Insert(indent + string.Format("|- Target look H V %1", look));

		vector lookAngles = GetLookDirection().VectorToAngles();
		report.Insert(indent + string.Format("|- Current look H V %1° %2°", ExpansionMath.RelAngle(lookAngles[0]), ExpansionMath.RelAngle(lookAngles[1])));

		vector aimAnglesTarget = m_eAI_AimDirectionTarget_ModelSpace.VectorToAngles();
		string aim = string.Format("%1° %2°", ExpansionMath.RelAngle(aimAnglesTarget[0]), ExpansionMath.RelAngle(aimAnglesTarget[1]));
		if (m_eAI_AimDirection_Recalculate)
			aim += " (tracking)";
		else
			aim += " (static)";

		report.Insert(indent + string.Format("|- Target aim H V %1", aim));

		vector aimAngles = GetAimDirection().VectorToAngles();
		report.Insert(indent + string.Format("|- Current aim H V %1° %2°", ExpansionMath.RelAngle(aimAngles[0]), ExpansionMath.RelAngle(aimAngles[1])));

		report.Insert(indent + string.Format("|- Current absolute threat to AI %1", GetThreatToSelf(true)));
		report.Insert(indent + string.Format("|- Current active threat to AI %1", GetThreatToSelf()));

		report.Insert(indent + string.Format("|- Current acute creature threats to AI %1", m_eAI_AcuteDangerTargetCount));
		report.Insert(indent + string.Format("|- Current acute player threats to AI %1", m_eAI_AcuteDangerPlayerTargetCount));
		report.Insert(indent + string.Format("|- Silent attack viability time %1 s", m_eAI_SilentAttackViabilityTime));

		report.Insert(indent + string.Format("|- Tracked/stateful targets %1/%2", m_eAI_Targets.Count(), m_eAI_TargetInformationStates.Count()));

		array<ref eAITarget> targets = {};
		ExpansionArray<eAITarget>.RefCopy(m_eAI_Targets, targets);

		foreach (eAITargetInformation info, eAITarget state: m_eAI_TargetInformationStates)
		{
			bool tracked = state.m_IsTracked;

			if (!tracked)
				targets.Insert(state);
		}

		foreach (int i, eAITarget target: targets)
		{
			if (!target)
				continue;

			DayZPlayerImplement player = null;
			ItemBase item = null;

			string targetInfo = target.m_Info.ClassName().Substring(3, target.m_Info.ClassName().Length() - 20);
			if (target.GetEntity())
			{
				if (Class.CastTo(player, target.GetEntity()) && player.GetIdentity())
					targetInfo += " " + player.GetIdentity().GetName();
				else
					targetInfo += " " + ExpansionStatic.GetDebugInfo(target.GetEntity());

				if (!target.m_IsTracked)
					targetInfo += " (untracked)";

				Class.CastTo(item, target.GetEntity());
			}

			report.Insert(indent + string.Format("|- Target %1 %2", i, targetInfo));

			if (player)
			{
				if (player.GetGroup())
					//report.InsertAll(player.GetGroup().DumpState(identity, false, indent + "|  |- "));
					report.Insert(indent + "|  |- " + string.Format("Group ID %1 %2", player.GetGroup().GetID(), player.GetGroup().GetName()));

				bool moving = false;
				bool friendly = false;
				bool targeted = false;
				bool enemy = PlayerIsEnemy(player, false, moving, friendly, targeted);
				string standing;
				if (!enemy || (friendly && !targeted))
					standing = "neutral";
				else
					standing = "enemy";
				report.Insert(indent + string.Format("|  |- Is seen as %1", standing));
				if (isAdmin)
					report.Insert(indent + string.Format("|  |- Is moving %1", moving.ToString()));
				report.Insert(indent + string.Format("|  |- Can be friendly %1", friendly.ToString()));
				report.Insert(indent + string.Format("|  |- Is targeted %1", targeted.ToString()));
			}
			else if (item)
			{
				bool hasFreeInvLoc = eAI_FindFreeInventoryLocationFor(item);
				report.Insert(indent + string.Format("|  |- Fits in AI inventory %1", hasFreeInvLoc.ToString()));
				bool isIgnoredItem = eAI_GetThreatOverride(item);
				report.Insert(indent + string.Format("|  |- Is ignored item %1", isIgnoredItem.ToString()));
			}

			eAITargetInformationState targetInfoState = target;
			report.Insert(indent + string.Format("|  |- Absolute threat to AI %1", target.GetThreat()));

			if (targetInfoState)
			{
				report.Insert(indent + string.Format("|  |- Active threat to AI %1", targetInfoState.m_ThreatLevelActive));

				string losState = targetInfoState.m_LOS.ToString();
				if (i > 0)
					losState += " (stale)";

				report.Insert(indent + string.Format("|  |- LOS state %1", losState));
			}

			if (isAdmin)
			{
			#ifdef DIAG_DEVELOPER
				if (targetInfoState)
				{
					report.Insert(indent + string.Format("|  |- LOS ray hit position %1", ExpansionStatic.VectorToString(targetInfoState.m_LOSRaycastHitPosition, ExpansionVectorToString.Plain)));
					report.Insert(indent + string.Format("|  |- LOS ray hit object %1", ExpansionStatic.GetDebugInfo(targetInfoState.m_LOSRaycastHitObject)));
				}
			#endif

				report.Insert(indent + string.Format("|  |- Actual position %1", ExpansionStatic.VectorToString(target.GetPosition(true), ExpansionVectorToString.Plain)));
				report.Insert(indent + string.Format("|  |- Actual distance to AI %1 m", target.GetDistance(true)));

				if (targetInfoState)
				{
					report.Insert(indent + string.Format("|  |- Search position %1", ExpansionStatic.VectorToString(target.GetPosition(), ExpansionVectorToString.Plain)));
					report.Insert(indent + string.Format("|  |- Search distance to AI %1 m", target.GetDistance()));
				}
			}

			if (target.m_IsTracked)
			{
				string foundAtTime = ExpansionStatic.FormatTime(GetDayZGame().ExpansionGetStartTimestamp(true) + target.m_FoundAtTime * 0.001, true, true, true);
				report.Insert(indent + string.Format("|  |- Found at time %1 UTC", foundAtTime));
				report.Insert(indent + string.Format("|  \\- Max time %1s", target.m_MaxTime * 0.001));
			}
			else
			{
				eAI_FixupLastReportEntry(report);
			}
		}

		string water = ExpansionStatic.FormatFloat(GetStatWater().Get() / GetStatWater().GetMax() * 100, 3, false, false);
		string energy = ExpansionStatic.FormatFloat(GetStatEnergy().Get() / GetStatEnergy().GetMax() * 100, 4, false, false);
		string heatComfort = ExpansionStatic.FormatFloat(GetStatHeatComfort().Get() / GetStatHeatComfort().GetMax() * 100, 1, false, false);
		report.Insert(indent + string.Format("|- Water %1%% Energy %2%% Heat %3%%", water, energy, heatComfort));

		string health = ExpansionStatic.FormatFloat(GetHealth01() * 100, 1, false, false);
		string blood = ExpansionStatic.FormatFloat(GetHealth01("", "Blood") * 100, 1, false, false);
		string shock = ExpansionStatic.FormatFloat(GetHealth01("", "Shock") * 100, 1, false, false);
		report.Insert(indent + string.Format("|- Health %1%% Blood %2%% Shock %3%%", health, blood, shock));

		if (IsUnconscious())
			report.Insert(indent + "|- Unconscious");

		if (IsBleeding())
		{
			report.Insert(indent + string.Format("|- Bleeding %1", GetBleedingSourceCount()));

			foreach (int bleedingBit, BleedingSource bleedingSource: m_BleedingManagerServer.m_BleedingSources)
			{
				float blood_scale = Math.InverseLerp(PlayerConstants.BLOOD_THRESHOLD_FATAL, PlayerConstants.BLEEDING_LOW_PRESSURE_BLOOD, GetHealth("GlobalHealth", "Blood"));
				blood_scale = Math.Clamp(blood_scale, PlayerConstants.BLEEDING_LOW_PRESSURE_MIN_MOD, 1);
				float flow = bleedingSource.m_FlowModifier;
				switch (bleedingSource.m_Type)
				{
					case eBleedingSourceType.CONTAMINATED:
						flow *= PlayerConstants.BLEEDING_SOURCE_BURN_MODIFIER;
						break;
				}
				int bloodLoss = Math.Round(-PlayerConstants.BLEEDING_SOURCE_BLOODLOSS_PER_SEC * blood_scale * flow);
				report.Insert(indent + string.Format("|  |- %1 %2 ml/s duration %3/%4 s %5", bleedingSource.m_Bone, bloodLoss, ExpansionStatic.FormatFloat(bleedingSource.m_ActiveTime, 3, false, false), bleedingSource.m_MaxTime, typename.EnumToString(eBleedingSourceType, bleedingSource.m_Type)));
			}

			eAI_FixupLastReportEntry(report);
		}

		if (GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			report.Insert(indent + "|- Broken legs");

		if (GetAgents() > 0)
		{
			int totalAgents = m_AgentPool.m_VirusPool.Count();
			report.Insert(indent + string.Format("|- Pathogens %1", totalAgents));

			foreach (int agentId, float agentCount: m_AgentPool.m_VirusPool)
			{
				string agentName = typename.EnumToString(eAgents, agentId);
				int agentMax = PluginTransmissionAgents.GetAgentMaxCount(agentId);
				int agentCountPrecision = Math.Max(5 - agentMax.ToString().Length(), 1);

				EStatLevels immunityLevel = GetImmunityLevel();
				EStatLevels agentPotency = m_AgentPool.m_PluginTransmissionAgents.GetAgentPotencyEx(agentId, this);

				float growDelta;
				if (agentPotency <= immunityLevel)
				{
					//! Agent can grow unless prevented by one of the active medical drugs
					if (Expansion_CanAgentGrow(agentId))
						growDelta = m_AgentPool.m_PluginTransmissionAgents.GetAgentInvasibilityEx(agentId, this);
					else
						growDelta = 0;
				}
				else
				{
					growDelta = -m_AgentPool.m_PluginTransmissionAgents.GetAgentDieOffSpeedEx(agentId, this);
				}

				string deltaIndicator;
				if (growDelta > 0 && agentCount < agentMax)
					deltaIndicator = "(+)";
				else if (growDelta < 0)
					deltaIndicator = "(-)";
				else
					deltaIndicator = "(=)";

				report.Insert(indent + string.Format("|  |- %1 %2/%3 %4", agentName, ExpansionStatic.FormatFloat(agentCount, agentCountPrecision, false, false), agentMax, deltaIndicator));
			}

			eAI_FixupLastReportEntry(report);
		}

		eAI_SymptomReport(m_SymptomManager.m_SymptomQueuePrimary, report, "Primary symptoms", indent);

		array<ref SymptomBase> secondarySymptoms = {};

		foreach (SymptomBase symptom: m_SymptomManager.m_SymptomQueueSecondary)
		{
			if (symptom.GetType() == SymptomIDs.SYMPTOM_BLOODLOSS)  //! Always active, ignore. See SymptomManager::AutoactivateSymptoms
				continue;

			secondarySymptoms.Insert(symptom);
		}

		eAI_SymptomReport(secondarySymptoms, report, "Secondary symptoms", indent);

		if (m_MedicalDrugsActive)
		{
			TStringArray medicalDrugs = {};
			ExpansionStatic.BitmaskEnumToString(EMedicalDrugsType, m_MedicalDrugsActive).Split("|", medicalDrugs);

			report.Insert(indent + "|- Active medication " + medicalDrugs.Count());

			foreach (string medicalDrug: medicalDrugs)
			{
				report.Insert(indent + string.Format("|  |- %1", medicalDrug));
			}

			eAI_FixupLastReportEntry(report);
		}

		TIntArray ignoreModifiers = {
			eModifiers.MDF_TEMPERATURE,
			eModifiers.MDF_HUNGER,
			eModifiers.MDF_THIRST,
			eModifiers.MDF_HEALTH,
			eModifiers.MDF_STOMACH,
			eModifiers.MDF_IMMUNE_SYSTEM,
			eModifiers.MDF_SHOCK,
			eModifiers.MDF_TOXICITY,
			eModifiers.MDF_BREATH_VAPOUR
		};

		if (IsUnconscious())
			ignoreModifiers.Insert(eModifiers.MDF_UNCONSCIOUSNESS);

		if (GetBrokenLegs())
			ignoreModifiers.Insert(eModifiers.MDF_BROKEN_LEGS);

		array<ModifierBase> activeModifiers = {};

		foreach (ModifierBase modifier: m_ModifiersManager.m_ModifierListArray)
		{
			if (ignoreModifiers.Find(modifier.GetModifierID()) > -1)
				continue;

			if (modifier.IsActive())
			{
				if (modifier.GetModifierID() == eModifiers.MDF_BLOOD_REGEN && GetHealth01("", "Blood") == 1.0)
					continue;

				if (modifier.GetModifierID() == eModifiers.MDF_HEALTH_REGEN && GetHealth01() == 1.0)
					continue;

				activeModifiers.Insert(modifier);
			}
		}

		if (activeModifiers.Count() > 0)
		{
			report.Insert(indent + "|- Active modifiers " + activeModifiers.Count());

			foreach (ModifierBase activeModifier: activeModifiers)
			{
				string modifierActiveTime =  ExpansionStatic.FormatFloat(activeModifier.GetAttachedTime(), 3, false, false);
				report.Insert(indent + string.Format("|  |- %1 duration %2 s", activeModifier.ClassName(), modifierActiveTime));
			}

			eAI_FixupLastReportEntry(report);
		}

		string itemName;
		if (itemInHands)
			itemName = ExpansionStatic.GetDebugInfo(itemInHands);
		else
			itemName = "NONE";
		report.Insert(indent + string.Format("|- Item in hands %1", itemName));

		eAI_WeaponReport(m_eAI_Firearms, report, indent);
		eAI_WeaponReport(m_eAI_Handguns, report, indent);
		eAI_WeaponReport(m_eAI_Launchers, report, indent);

		report.Insert(indent + string.Format("|- Melee weapons %1", m_eAI_MeleeWeapons.Count()));
		foreach (ItemBase meleeWeapon: m_eAI_MeleeWeapons)
		{
			report.Insert(indent + string.Format("|  |- %1", ExpansionStatic.GetDebugInfo(meleeWeapon)));
		}
		if (m_eAI_MeleeWeapons.Count())
			eAI_FixupLastReportEntry(report);

		report.Insert(indent + string.Format("|- Firearm types and ammo/mags to reload %1", m_eAI_EvaluatedFirearmTypes.Count()));
		foreach (typename weaponType, Magazine magToReload: m_eAI_EvaluatedFirearmTypes)
		{
			report.Insert(indent + string.Format("|  |- %1 -> %2", weaponType, ExpansionStatic.GetDebugInfo(magToReload)));
		}
		if (m_eAI_EvaluatedFirearmTypes.Count())
			eAI_FixupLastReportEntry(report);

		report.Insert(indent + string.Format("|- Ammo/mags %1", m_eAI_Magazines.Count()));
		foreach (Magazine candidateMag: m_eAI_Magazines)
		{
			report.Insert(indent + string.Format("|  |- %1 (%2/%3)", ExpansionStatic.GetDebugInfo(candidateMag), candidateMag.GetAmmoCount(), candidateMag.GetAmmoMax()));
		}
		if (m_eAI_Magazines.Count())
			eAI_FixupLastReportEntry(report);

		report.Insert(indent + string.Format("|- Food %1/%2", m_eAI_Food.Count(), m_eAI_MaxFoodCount));
		foreach (ItemBase food: m_eAI_Food)
		{
			report.Insert(indent + string.Format(".  |- %1", ExpansionStatic.GetDebugInfo(food)));
		}
		if (m_eAI_Food.Count())
			eAI_FixupLastReportEntry(report);

		report.Insert(indent + string.Format("|- Cargo space taken %1/%2", m_eAI_AccessibleCargoSpaceTaken, m_eAI_AccessibleCargoSpaceTotal));

		array<EntityAI> cargoItems = MiscGameplayFunctions.Expansion_GetCargoItems(this);
		report.Insert(indent + string.Format("\\- Items in cargo %1", cargoItems.Count()));

		Magazine mag;
		ItemBase cargoItemBase;
		foreach (EntityAI cargoItem: cargoItems)
		{
			if (Class.CastTo(mag, cargoItem))
				report.Insert(indent + string.Format(".  |- %1 (%2/%3)", ExpansionStatic.GetDebugInfo(cargoItem), mag.GetAmmoCount(), mag.GetAmmoMax()));
			else if (Class.CastTo(cargoItemBase, cargoItem) && cargoItemBase.HasQuantity())
			{
				if (cargoItemBase.Expansion_IsStackable())
					report.Insert(indent + string.Format(".  |- %1 (%2/%3)", ExpansionStatic.GetDebugInfo(cargoItem), cargoItem.GetQuantity(), cargoItem.GetQuantityMax()));
				else
					report.Insert(indent + string.Format(".  |- %1 (%2%%)", ExpansionStatic.GetDebugInfo(cargoItem), cargoItem.GetQuantityNormalized() * 100));
			}
			else
				report.Insert(indent + string.Format(".  |- %1", ExpansionStatic.GetDebugInfo(cargoItem)));
		}
		if (cargoItems.Count())
			eAI_FixupLastReportEntry(report);

		return report;
	}

	void eAI_SymptomReport(array<ref SymptomBase> symptoms, TStringArray report, string label, string indent = string.Empty)
	{
		int symptomCount = symptoms.Count();
		if (symptomCount > 0)
		{
			report.Insert(indent + string.Format("|- %1 %2", label, symptomCount));

			foreach (SymptomBase symptom: symptoms)
			{
				if (symptom.IsActivated())
				{
					if (symptom.m_Duration > 0)
						report.Insert(indent + string.Format("|  |- %1 duration %2/%3 s", symptom.ClassName(), ExpansionStatic.FormatFloat(symptom.m_ActivatedTime, 3, false, false), symptom.m_Duration));
					else
						report.Insert(indent + string.Format("|  |- %1 duration %2 s", symptom.ClassName(), ExpansionStatic.FormatFloat(symptom.m_ActivatedTime, 3, false, false)));
				}
				else
				{
					report.Insert(indent + string.Format("|  |- %1 (inactive)", symptom.ClassName()));
				}
			}

			eAI_FixupLastReportEntry(report);
		}
	}

	void eAI_WeaponReport(array<Weapon_Base> weapons, TStringArray report, string indent = string.Empty)
	{
		foreach (Weapon_Base weapon: weapons)
		{
			report.Insert(indent + string.Format("|- Weapon %1", ExpansionStatic.GetDebugInfo(weapon)));

			int mi = weapon.GetCurrentMuzzle();

			report.Insert(indent + string.Format("|  |- Is chambered %1", weapon.Expansion_IsChambered(mi).ToString()));

			Magazine attachedMag = null;
			int ammoCount = weapon.Expansion_GetMagazineAmmoCount(mi, attachedMag);

			if (attachedMag)
				report.Insert(indent + string.Format("|  |- Has attached mag %1 (%2/%3)", ExpansionStatic.GetDebugInfo(attachedMag), ammoCount, attachedMag.GetAmmoMax()));
			else if (weapon.GetMagazineTypeCount(mi) > 0)
				report.Insert(indent + "|  |- Has no attached mag");
			else if (weapon.HasInternalMagazine(mi))
				report.Insert(indent + string.Format("|  |- Has internal mag (%1/%2)", ammoCount, weapon.GetInternalMagazineMaxCartridgeCount(mi)));

			Magazine mag = null;
			if (!m_eAI_EvaluatedFirearmTypes.Find(weapon.Type(), mag))
				report.Insert(indent + "|  \\- Ammo/mag to reload not evaluated");
			else if (!mag)
				report.Insert(indent + "|  \\- Has no ammo/mag to reload");
			else
				report.Insert(indent + string.Format("|  \\- Has ammo/mag to reload %1 (%2/%3)", ExpansionStatic.GetDebugInfo(mag), mag.GetAmmoCount(), mag.GetAmmoMax()));
		}
	}

	void eAI_FixupLastReportEntry(TStringArray report)
	{
		int index = report.Count() - 1;
		string last = report[index];
		last.Replace("|-", "\\-");
		report[index] = last;
	}

	void ReactToThreatChange(float pDt, EntityAI entityInHands = null)
	{
		if (m_eAI_CurrentThreatToSelf >= 0.2)
		{
			if (m_eAI_PreviousThreatToSelf < 0.2)
				EXTrace.Print(EXTrace.AI, this, "current threat to self >= 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");

			if (!m_eAI_IsDay && m_eAI_CurrentThreatToSelfActive >= 0.4 && m_eAI_PreviousThreatToSelfActive < 0.4 && !m_Expansion_ActiveVisibilityEnhancers.Count())
			{
				Expansion_TryTurningOnAnyLightsOrNVGEx(false, true);  //! Switch on lights at night (skip NVG)
				if (m_Expansion_ActiveVisibilityEnhancers.Count())
				{
					m_eAI_IsActiveVisibilityEnhancerTemporary = true;
					Expansion_UpdateVisibility();
				}
			}
		}
		else
		{
			if (m_eAI_PreviousThreatToSelf >= 0.2)
			{
				EXTrace.Print(EXTrace.AI, this, "current threat to self < 0.2 (active " + m_eAI_CurrentThreatToSelfActive + ")");

				if (!m_eAI_IsDay && m_eAI_IsActiveVisibilityEnhancerTemporary)
				{
					Expansion_TryTurningOffAnyLightsOrNVG(true);  //! Switch off any lights (skip NVG)
					m_eAI_IsActiveVisibilityEnhancerTemporary = false;
					Expansion_UpdateVisibility();
				}
			}
		}

		if (m_eAI_CurrentThreatToSelfActive >= 0.4)
		{
			if (m_eAI_PreviousThreatToSelfActive < 0.4)
				EXTrace.Print(EXTrace.AI, this, "current active threat to self >= 0.4 (" + m_eAI_CurrentThreatToSelfActive + ")");

			if (m_ThreatClearedTimeout <= 0)
				m_ThreatClearedTimeout = Math.RandomFloat(1, 3);
		}
		else
		{
			if (m_eAI_PreviousThreatToSelfActive >= 0.4)
				EXTrace.Print(EXTrace.AI, this, "current active threat to self < 0.4 (" + m_eAI_CurrentThreatToSelfActive + ")");

			if (!m_eAI_Meme && m_eAI_CurrentThreatToSelfActive > 0.15 && m_eAI_PreviousThreatToSelfActive < 0.15 && !Math.RandomInt(0, 3))
				m_eAI_Meme = Math.RandomInt(2, 4);

			if (m_ThreatClearedTimeout > 0)
			{
				m_ThreatClearedTimeout -= pDt;
				if (m_ThreatClearedTimeout <= 0)
				{
					if (m_eAI_DespawnOnLoosingAggro)
					{
						eAI_Despawn();
					}
					else if (m_eAI_CurrentThreatToSelf <= 0.2 && !GetExpansionSettings().GetAI().Manners)
					{
						int maxEmote;

						if (Expansion_GetMovementSpeed() == 0.0 && Math.RandomInt(0, 101) <= 10)  //! 10% chance to dance
							maxEmote = 5;
						else
							maxEmote = 4;

						int emoteId;
						switch (Math.RandomIntInclusive(1, maxEmote))
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
			else if (m_eAI_PreviousThreatToSelf < 0.15 && m_eAI_CurrentThreatToSelf >= 0.15 && m_eAI_CurrentThreatToSelf < 0.2 && !m_Expansion_EmoteID && Math.RandomFloat(0.0, 1.0) < 0.05)
			{
				eAI_RandomGreeting(true);
			}
		}

		if (IsRaised() && !GetWeaponManager().IsRunning())
		{
			//! CanRaiseWeapon will return false if getting out of way of vehicle so we are able to lower weapon instantly to move faster
			if (!CanRaiseWeapon())
			{
				RaiseWeapon(false);
			}
			else if (m_eAI_CurrentThreatToSelfActive < 0.2 && !m_eAI_IsInCover)
			{
				if (!entityInHands || !entityInHands.IsWeapon())
				{
					m_WeaponLowerTimeout = 0.0;
					RaiseWeapon(false);
				}
				else
				{
					if (m_eAI_PreviousThreatToSelfActive >= 0.2)
						m_WeaponLowerTimeout = Math.RandomFloat(15.0, 30.0);

					m_WeaponLowerTimeout -= pDt;
			
					if (m_WeaponLowerTimeout <= 0)
						RaiseWeapon(false);
				}
			}
		}
	}

	void UpdateTargets(float pDt, EntityAI entityInHands = null)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(03a) -> UpdateTargets");
#endif

#ifdef EAI_TRACE
		int ticks;
		int elapsed;
#endif
		// TODO: use particle system instead
		// XXX: I have no clue what the above comment means -lava76

		eAIGroup group = GetGroup();
		if (!group)  //! This should never happen
			return;

		if (eAI_IsPassive())
			return;

		int i;

		eAIFaction faction = group.GetFaction();

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

			bool updateCoverObjects;

			if (m_eAI_ShouldTakeCover)
				updateCoverObjects = true;
			else if (group.GetWaypointBehaviour() == eAIWaypointBehavior.ROAMING && group.GetFormationLeader() == this)
				updateCoverObjects = true;

			int flags = QueryFlags.DYNAMIC;
			if (updateCoverObjects && (m_eAI_UpdateNearTargetsCount % 33 == 0 || m_eAI_UpdatePotentialCoverObjects))
			{
				m_eAI_PotentialCoverObjects.Clear();
				//flags |= QueryFlags.STATIC;
				DayZPlayerUtils.PhysicsGetEntitiesInBox(min, max, m_eAI_PotentialTargetEntities);
				m_eAI_UpdatePotentialCoverObjects = false;
				for (i = m_eAI_PotentialTargetEntities.Count() - 1; i >= 0; i--)
				{
					EntityAI potentialCoverObj = m_eAI_PotentialTargetEntities[i];

					bool isPotentialCoverObject = false;

					if (potentialCoverObj.IsBuilding())
					{
						string type = potentialCoverObj.GetType();
						if ((type.IndexOf("Land_") == 0 || type.IndexOf("Wreck_") == 0) && type.IndexOf("Gate") == -1)
						{
						#ifdef DIAG_DEVELOPER
							EXTrace.Print(EXTrace.AI, this, "Potential cover object (building): " + type +  " " + potentialCoverObj);
						#endif
							isPotentialCoverObject = true;
						}
					}
					else if (!m_eAI_ShouldTakeCover)
					{
						//! Ignore all non-building objects if not seeking cover
						continue;
					}
					else if (potentialCoverObj.IsRock())
					{
						//! @note rocks are not picked up by SceneGetEntitiesInBox even with QueryFlags.STATIC
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, "Potential cover object (rock): " + potentialCoverObj);
					#endif
						isPotentialCoverObject = true;
					}
					else if (potentialCoverObj.IsTree())
					{
						//! @note trees are not picked up by SceneGetEntitiesInBox even with QueryFlags.STATIC
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, "Potential cover object (tree): " + potentialCoverObj);
					#endif
						isPotentialCoverObject = true;
					}
					else if (potentialCoverObj.IsTransport())
					{
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, "Potential cover object (transport): " + potentialCoverObj);
					#endif
						isPotentialCoverObject = true;
					}

					if (isPotentialCoverObject)
					{
						m_eAI_PotentialCoverObjects.Insert(potentialCoverObj);
						//! Remove from potential target entities
						m_eAI_PotentialTargetEntities.Remove(i);
					}
				}
			}
			else
			{
				DayZPlayerUtils.SceneGetEntitiesInBox(min, max, m_eAI_PotentialTargetEntities, flags);
			}

			m_eAI_UpdateNearTargetsCount++;

#ifdef EAI_TRACE
			elapsed = TickCount(ticks);
			float time = elapsed / 10000.0;
			m_eAI_UpdateNearTargetsTime += time;
			float timeAvg = m_eAI_UpdateNearTargetsTime / m_eAI_UpdateNearTargetsCount;

			EXTrace.Add(trace, "objects in near range " + m_eAI_PotentialTargetEntities.Count() + " time (ms) " + time + " timeAvg (ms) " + timeAvg);
#endif

			eAI_PurgeThreatOverride();
		}

		//! Get other players/AI in extended range (1000 m radius) - check one player per tick (30 players per second)

#ifdef EAI_TRACE
		ticks = TickCount(0);
#endif

		if (!faction.IsObserver())  //! Observers only react to near players
		{
			if (!m_eAI_PotentialTargetPlayer)
				m_eAI_PotentialTargetPlayer = s_Expansion_AllPlayers.m_Head;

			PlayerBase player = m_eAI_PotentialTargetPlayer.m_Value;

			EntityAI playerEntity = player;
			if (player && player != this && m_eAI_PotentialTargetEntities.Find(playerEntity) == -1 && Math.IsPointInCircle(center, eAI_GetVisibilityDistanceLimit(player), player.GetPosition()))
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
		eAIBase ai;
		ItemBase targetItem;
		Weapon_Base targetWeapon;
		Magazine mag;
		Edible_Base food;

		Weapon_Base fireArm;
		ItemBase itemInHands;
		Class.CastTo(itemInHands, entityInHands);
		bool hasFirearmWithAmmo;
		bool hasMeleeWeapon;

		if (!Class.CastTo(fireArm, entityInHands))
			fireArm = eAI_GetAnyWeaponToUse(false);

		if (fireArm)
		{
			if (fireArm.Expansion_HasAmmo())
				hasFirearmWithAmmo = true;
		}
		else if ((itemInHands && itemInHands.Expansion_IsMeleeWeapon()) || GetMeleeWeaponToUse())
		{
			hasMeleeWeapon = true;
		}

		float group_count = group.Count();
		bool playerIsEnemy;

		while (m_eAI_CurrentPotentialTargetIndex < m_eAI_PotentialTargetEntities.Count())
		{
			EntityAI entity = m_eAI_PotentialTargetEntities[m_eAI_CurrentPotentialTargetIndex++];
			if (!entity)
				continue;

			if (Class.CastTo(playerThreat, entity))
			{
				if (playerThreat == this)
					continue;
				if (playerThreat.eAI_IsPassive())
					continue;
				if (!playerThreat.IsPlayerLoaded())
					continue;
				playerIsEnemy = PlayerIsEnemy(playerThreat, true);
				if (!playerThreat.IsAlive() || (playerThreat.IsUnconscious() && playerThreat.IsAI() && playerIsEnemy))
				{
					if (m_eAI_TrackedBodies.Find(entity) > -1)
						continue;

					m_eAI_TrackedBodies.Insert(entity);

					if (Class.CastTo(ai, playerThreat))
					{
						//! Allow to loot anything from dead/uncon AI

						ExpansionArrayTools<Weapon_Base, EntityAI>.InsertAll_UpCast(ai.m_eAI_Firearms, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<Weapon_Base, EntityAI>.InsertAll_UpCast(ai.m_eAI_Handguns, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<Weapon_Base, EntityAI>.InsertAll_UpCast(ai.m_eAI_Launchers, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<ItemBase, EntityAI>.InsertAll_UpCast(ai.m_eAI_MeleeWeapons, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<ItemBase, EntityAI>.InsertAll_UpCast(ai.m_eAI_Bandages, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<Magazine, EntityAI>.InsertAll_UpCast(ai.m_eAI_Magazines, m_eAI_PotentialTargetEntities);
						ExpansionArrayTools<ItemBase, EntityAI>.InsertAll_UpCast(ai.m_eAI_RepairKits, m_eAI_PotentialTargetEntities);

						for (i = 0; i < ai.GetInventory().AttachmentCount(); i++)
						{
							EntityAI attachment = ai.GetInventory().GetAttachmentFromIndex(i);
							if (attachment && attachment.IsClothing())
								m_eAI_PotentialTargetEntities.Insert(attachment);
						}
					}
					else if (group.m_Persist && group.m_BaseName)
					{
						//! Allow to loot shoulder/melee slots from dead players if this AI is in a persistent group

						EntityAI shoulder = playerThreat.FindAttachmentBySlotName("Shoulder");
						if (shoulder)
							m_eAI_PotentialTargetEntities.Insert(shoulder);

						EntityAI melee = playerThreat.FindAttachmentBySlotName("Melee");
						if (melee)
							m_eAI_PotentialTargetEntities.Insert(melee);
					}

					continue;
				}
				if (!playerIsEnemy)
					continue;
			}
			else if (entity.IsInherited(DayZPlayerImplement))
				continue;  //! Ignore non PlayerBase NPCs
			else if (Class.CastTo(targetItem, entity))
			{
				//! If the object is an item, ignore it if any of the following conditions are met.
				//! Note these should be roughly in sync with what's in eAIItemTargetInformation::CalculateThreat,
				//! but the latter can be more specific

				if (targetItem.IsDamageDestroyed() || targetItem.IsSetForDeletion() || eAI_GetThreatOverride(targetItem))
					continue;

				if (targetItem.Expansion_IsDanger())
				{
					if (!Expansion_CanBeDamaged())
						continue;
				}
				else
				{
					//! Check if we're interested in the item

					if (targetItem.IsWeapon())
					{
						if (eAI_CanLootWeapon(targetItem, faction))
						{
							//! Flaregun is simply too low damage to be of any use, we're better off even with melee.
							//! Also, it causes NULL ptrs if fired on server side.
							//! TODO: Maybe have a list of excludes?
							if (targetItem.IsInherited(Flaregun))
								continue;

							if (eAI_WasItemRecentlyDropped(targetItem))
							{
								EXPrint(this, "Ignoring weapon because it was recently dropped: " + targetItem + " (type=" + targetItem.GetType() + " health=" + targetItem.GetHealth() + ")");
								eAI_ThreatOverride(targetItem, true);  //! Ignore by overriding threat so we don't have to do above checks again
								continue;
							}

							if (hasFirearmWithAmmo)
							{
								if ((m_eAI_LootingBehavior & eAILootingBehavior.UPGRADE) == 0)
									continue;

								if (!itemInHands || !itemInHands.IsWeapon())
									continue;

								if (!eAI_WeaponSelection(itemInHands, targetItem))
									continue;
							}
							else if (targetItem.Expansion_GetDPS() <= 0)
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					else if (targetItem.Expansion_IsMeleeWeapon())
					{
						if (eAI_CanLootMeleeWeapon(targetItem, faction))
						{
							if (eAI_WasItemRecentlyDropped(targetItem))
							{
								EXPrint(this, "Ignoring melee weapon because it was recently dropped: " + targetItem + " (type=" + targetItem.GetType() + " health=" + targetItem.GetHealth() + ")");
								eAI_ThreatOverride(targetItem, true);  //! Ignore by overriding threat so we don't have to do above checks again
								continue;
							}

							if (hasFirearmWithAmmo)
								continue;

							if (hasMeleeWeapon)
							{
								if ((m_eAI_LootingBehavior & eAILootingBehavior.UPGRADE) == 0)
									continue;

								if (!itemInHands || !itemInHands.Expansion_IsMeleeWeapon())
									continue;

								if (itemInHands.Expansion_CompareDPS(targetItem) >= 0)
									continue;
							}
						}
						else
						{
							continue;
						}
					}
					else if (targetItem.IsMagazine())
					{
						if (faction.IsWeaponPickupEnabled() && (!targetItem.IsAmmoPile() || targetItem.Expansion_GetHealthDamage() > 0))
						{
							if ((m_eAI_LootingBehavior & eAILootingBehavior.WEAPONS_FIREARMS) == 0)
							{
								if ((m_eAI_LootingBehavior & eAILootingBehavior.WEAPONS_LAUNCHERS) == 0)
									continue;
							}

							if (!fireArm || !Class.CastTo(mag, targetItem) || mag.GetAmmoCount() == 0)
								continue;
						}
						else
						{
							continue;
						}
					}
					else if (targetItem.Expansion_CanBeUsedToBandage())
					{
						if (!eAI_ShouldPickupBandage(targetItem))
							continue;
					}
					else if (targetItem.IsInherited(WeaponCleaningKit))
					{
						if (!faction.IsWeaponPickupEnabled() || !fireArm || m_eAI_RepairKits.Count() > 0)
							continue;
					}
					else if (targetItem.IsClothing())
					{
						if ((m_eAI_LootingBehavior & eAILootingBehavior.CLOTHING) == 0)
							continue;

						//! Ignore hockey mask since it blocks certain helmets from being worn and due to its armor value
						//! will only be swapped out for a different mask when it's ruined
						//! (hockey mask is the only vanilla mask with armor)
						if (targetItem.IsInherited(HockeyMask))
							continue;

						TStringArray inventorySlots = targetItem.Expansion_GetInventorySlots();
						bool canWear = false;
						ItemBase slotGear;
						ItemBase currentlyWornGear;
						int slotID = 0;
						bool preferLargerCargo = false;

						foreach (string slot: inventorySlots)
						{
							slotID = InventorySlots.GetSlotIdFromString(slot);
							if (GetInventory().HasAttachmentSlot(slotID))
							{
								switch (slotID)
								{
									case InventorySlots.BODY:
									case InventorySlots.BACK:
									case InventorySlots.HIPS:
									case InventorySlots.LEGS:
										preferLargerCargo = true;
										break;
								}

								if (!Class.CastTo(slotGear, GetInventory().FindAttachment(slotID)))
								{
									//! Found empty slot

									if (!eAI_ClothingLootingBehaviorCheck_Slot(slot, targetItem))
										continue;

									canWear = true;
									currentlyWornGear = null;  //! null any gear that was found in another slot

									break;
								}
								else if (eAI_ClothingLootingBehaviorCheck_Slot(slot, targetItem))
								{
									//! Found taken slot, potential upgrade

									canWear = true;
									currentlyWornGear = slotGear;
								}
							}
						}

						if (!canWear || !eAI_ClothingSelection(currentlyWornGear, targetItem, preferLargerCargo))
						{
							eAI_ThreatOverride(targetItem, true);  //! Ignore by overriding threat so we don't have to do above checks again
							continue;
						}
					}
					else if (Class.CastTo(food, targetItem))
					{
						if (!eAI_ShouldProcureFood())
							continue;

						if (targetItem.IsCorpse() && m_eAI_MeleeWeapons.Count() == 0)
							continue;

						if (!targetItem.IsFood())
							continue;

						if (food.GetFoodStage() && food.GetFoodStageType() == FoodStageType.ROTTEN)
							continue;
					}
					else
					{
						//! Ignore everything else
						continue;
					}
				}
			}
			else if (entity.IsBuilding())
				continue;
			else if (entity.IsInherited(Transport))
				continue;
			else if (faction.IsFriendlyEntity(entity, this))
				continue;

			if (!eAI_ProcessTarget(entity, group))
				continue;

			if (m_eAI_Targets.Count() * 2 > group_count)
				break;
		}

		if (m_eAI_CurrentPotentialTargetIndex >= m_eAI_PotentialTargetEntities.Count())
		{
			m_eAI_PotentialTargetEntities.Clear();
			m_eAI_TrackedBodies.Clear();
		}

		//! Since item target info states are only removed if the item is deleted or picked up,
		//! we periodically purge states to avoid accumulating a lot of stale states over time

		float timestamp = g_Game.GetTickTime();

		i = 0;

		foreach (eAITargetInformation info, eAITarget state: m_eAI_TargetInformationStates)
		{
			if (!info)
			{
				//! This shouldn't be possible to happen, but let's see...
				EXPrint(this, "Removing target info state without info: #" + i + " " + state);
				m_eAI_TargetInformationStates.RemoveElement(i);
				break;
			}
			//! Purge if out of range
			else if (info.IsItem() && !Math.IsPointInCircle(center, 500.0, info.GetPosition()))
			////! Purge if last threat level update was longer than 120 s ago
			//else if (info.IsItem() && timestamp - state.m_ThreatLevelUpdateTimestamp > 120.0)
			{
				eAI_RemoveTargetInfoState(info);
				break;
			}

			i++;
			if (i == 50)
				break;
		}
	}

	bool eAI_ProcessTarget(EntityAI entity, eAIGroup group, out eAITargetInformation info = null)
	{
		info = eAITargetInformation.GetTargetInformation(entity);
		if (!info)
			return false;

		if (!info.IsActive())
		{
			if (entity.IsZombie())
			{
				if (m_eAI_TrackedBodies.Find(entity) == -1)
				{
					m_eAI_TrackedBodies.Insert(entity);

					array<EntityAI> cargoItems = MiscGameplayFunctions.Expansion_GetCargoItems(entity, true);
					foreach (EntityAI cargoItem: cargoItems)
					{
						m_eAI_PotentialTargetEntities.Insert(cargoItem);
					}
				}
			}

			return false;
		}

		if (entity.IsItemBase() || entity.IsZombie() || entity.IsAnimal())
		{
			if (info.ShouldRemove(this))
				return false;
		}
		else if (info.ShouldRemove())
		{
			return false;
		}

		int num_ai_in_group_tracking = 0;
		if (m_eAI_Targets.Count() > 0 && m_eAI_CurrentThreatToSelf >= 0.4 && info.IsTrackedBy(group, num_ai_in_group_tracking))
		{
			int group_count = group.Count();
			int num_ai_in_group_not_tracking = group_count - num_ai_in_group_tracking;
			if (!num_ai_in_group_not_tracking)
				return false;
			float threatLevel = info.GetThreat(this);
			float frac = num_ai_in_group_not_tracking / group_count;
			if ((frac * threatLevel) < (1.0 / group_count))
				return false;
		}

		info.AddAI(this);

		return true;
	}

	void eAI_OnRemovePlayer(CF_DoublyLinkedNode_WeakRef<PlayerBase> node)
	{
		if (node == m_eAI_PotentialTargetPlayer)
			m_eAI_PotentialTargetPlayer = node.m_Next;
	}

	/**
	 * @brief select between current and compared weapon.
	 * 
	 * @return true if compared weapon was selected
	 */
	bool eAI_WeaponSelection(ItemBase currentItem, ItemBase compareItem)
	{
		Weapon_Base currentWeapon;
		Weapon_Base compareWeapon;

		if (!Class.CastTo(compareWeapon, compareItem))
			return false;  //! If compared item is not weapon, prefer current item (even if null)

		float compareDPS = eAI_GetCurrentDPS(compareWeapon);

		//! If compared weapon DPS is equal or below 10 and doesn't shoot explosive ammo, we're better off with melee even
		if (compareDPS <= 10.0 && !compareWeapon.ShootsExplosiveAmmo())
			return false;

		if (!Class.CastTo(currentWeapon, currentItem))
			return true;  //! If current item is not weapon, prefer compared item (weapon, not null)

		ExpansionWeaponType currentWeaponType = currentWeapon.Expansion_GetWeaponType();
		ExpansionWeaponType compareWeaponType = compareWeapon.Expansion_GetWeaponType();

		if (compareWeaponType == ExpansionWeaponType.Handgun && currentWeaponType != ExpansionWeaponType.Handgun)
			return false;  //! Don't replace non-handgun with handgun

		if (currentWeaponType == ExpansionWeaponType.Marksman && currentWeapon.GetAttachedOptics())
		{
			if (compareWeaponType == ExpansionWeaponType.Marksman)
			{
				if (!compareWeapon.GetAttachedOptics())
					return false; //! Don't replace marksman rifle w/ optics with marksman rifle w/o optics
			}
			else if (compareWeapon.m_Expansion_WeaponInfo.m_AvgDmg <= currentWeapon.m_Expansion_WeaponInfo.m_AvgDmg)
			{
				return false; //! Don't replace marksman rifle w/ optics with non-marksman weapon unless other weapon avg dmg per shot is higher
			}
		}
		else if (compareWeapon.IsInherited(RifleSingleShot_Base) && !currentWeapon.IsInherited(RifleSingleShot_Base))
		{
			return false;  //! Don't replace non-single-shot rifle with single-shot-rifle
		}

		float currentDPS = eAI_GetCurrentDPS(currentWeapon);

	#ifdef DIAG_DEVELOPER
		string curWpnInfo;
		string cmpWpnInfo;
	#endif

		if (compareDPS == currentDPS)
		{
			//! If DPS is equal, only select other weapon if it has more health and equal or more atts, or equal health and more atts

			//! @note higher health level = worse! 0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined
			int currentHealthLv = currentWeapon.GetHealthLevel();
			int compareHealthLv = compareWeapon.GetHealthLevel();
			int currentAttCount = currentWeapon.GetInventory().AttachmentCount();
			int compareAttCount = compareWeapon.GetInventory().AttachmentCount();

			if ((currentHealthLv > compareHealthLv && currentAttCount <= compareAttCount) || (currentHealthLv == compareHealthLv && currentAttCount < compareAttCount))
			{
			#ifdef DIAG_DEVELOPER
				if (m_eAI_CompareWeapon != compareWeapon || m_eAI_CurrentWeapon != currentWeapon)
				{
					m_eAI_CompareWeapon = compareWeapon;
					m_eAI_CurrentWeapon = currentWeapon;

					curWpnInfo = string.Format("%1 type=%2 dmg=%3 dps=%4 health=%5 attCount=%6", ExpansionStatic.GetHierarchyInfo(currentWeapon), typename.EnumToString(ExpansionWeaponType, currentWeaponType), currentWeapon.m_Expansion_WeaponInfo.m_AvgDmg, currentWeapon.Expansion_GetDPS(), currentWeapon.GetHealth(), currentWeapon.GetInventory().AttachmentCount());

					cmpWpnInfo = string.Format("%1 type=%2 dmg=%3 dps=%4 health=%5 attCount=%6", ExpansionStatic.GetHierarchyInfo(compareWeapon), typename.EnumToString(ExpansionWeaponType, compareWeaponType), compareWeapon.m_Expansion_WeaponInfo.m_AvgDmg, compareWeapon.Expansion_GetDPS(), compareWeapon.GetHealth(), compareWeapon.GetInventory().AttachmentCount());

					EXTrace.Print(EXTrace.AI, this, string.Format("eAI_WeaponSelection cur=%1 cmp=%2 curDPS == cmpDPS", curWpnInfo, cmpWpnInfo));
				}
			#endif
				return true;
			}
		}
		else if (compareDPS > currentDPS)
		{
		#ifdef DIAG_DEVELOPER
			if (m_eAI_CompareWeapon != compareWeapon || m_eAI_CurrentWeapon != currentWeapon)
			{
				m_eAI_CompareWeapon = compareWeapon;
				m_eAI_CurrentWeapon = currentWeapon;

				curWpnInfo = string.Format("%1 type=%2 dmg=%3 dps=%4 health=%5 attCount=%6", ExpansionStatic.GetHierarchyInfo(currentWeapon), typename.EnumToString(ExpansionWeaponType, currentWeaponType), currentWeapon.m_Expansion_WeaponInfo.m_AvgDmg, currentWeapon.Expansion_GetDPS(), currentWeapon.GetHealth(), currentWeapon.GetInventory().AttachmentCount());

				cmpWpnInfo = string.Format("%1 type=%2 dmg=%3 dps=%4 health=%5 attCount=%6", ExpansionStatic.GetHierarchyInfo(compareWeapon), typename.EnumToString(ExpansionWeaponType, compareWeaponType), compareWeapon.m_Expansion_WeaponInfo.m_AvgDmg, compareWeapon.Expansion_GetDPS(), compareWeapon.GetHealth(), compareWeapon.GetInventory().AttachmentCount());

				EXTrace.Print(EXTrace.AI, this, string.Format("eAI_WeaponSelection cur=%1 cmp=%2 cmpDPS > curDPS", curWpnInfo, cmpWpnInfo));
			}
		#endif
			return true;
		}

		return false;
	}

	bool eAI_ClothingSelection(ItemBase currentlyWornGear, ItemBase targetItem, bool preferLargerCargo = false)
	{
		if (!currentlyWornGear)
		{
			if (!eAI_WasItemRecentlyDropped(targetItem))
			{
				return true;
			}
			else
			{
				EXPrint(this, "Ignoring item because it was recently dropped: " + targetItem + " (type=" + targetItem.GetType() + " health=" + targetItem.GetHealth() + ")");
				return false;
			}
		}

		if (targetItem.IsInherited(NVGHeadstrap) && targetItem.GetInventory().AttachmentCount() == 0)
			return false;

		if (!currentlyWornGear.IsInherited(HelmetBase) || targetItem.IsInherited(HelmetBase))
		{
			if (eAI_ClothingLootingBehaviorCheck_Selection(currentlyWornGear, targetItem))
			{
				//! Is target item better than what we currently have in some capacity?

				if (preferLargerCargo)
				{
					int curCargoSize = currentlyWornGear.eAI_GetCargoSize();
					int tgtCargoSize = targetItem.eAI_GetCargoSize();

					//! Prefer larger cargo space which usually also reflects the other attributes of clothing
					//! (military clothing usually has the largest cargo capacity and best protection)
					if (tgtCargoSize < curCargoSize)
						return false;
					else if (tgtCargoSize > curCargoSize)
						return true;
				}

				//! Since destroyed clothing provides no protection whatsoever, always prefer target item if same or bigger cargo size,
				//! even if recently dropped
				if (currentlyWornGear.IsDamageDestroyed())
					return true;

				string curGearArmorPath = "CfgVehicles " + currentlyWornGear.GetType() + " DamageSystem GlobalArmor";
				string tgtGearArmorPath = "CfgVehicles " + targetItem.GetType() + " DamageSystem GlobalArmor";
				string curGearDmgPath;
				string tgtGearDmgPath;

				//! Always prefer comparing to current gear projectile health damage even if target item doesn't have any,
				//! That way we don't downgrade from an item that has projectile armor to one that doesn't
				if (g_Game.ConfigIsExisting(curGearArmorPath + " Projectile"))
				{
					curGearDmgPath = curGearArmorPath + " Projectile Health damage";
					tgtGearDmgPath = tgtGearArmorPath + " Projectile Health damage";
				}
				else
				{
					curGearDmgPath = curGearArmorPath + " Melee Health damage";
					tgtGearDmgPath = tgtGearArmorPath + " Melee Health damage";
				}

				float curGearDmg = 1.0;
				float tgtGearDmg = 1.0;

				if (g_Game.ConfigIsExisting(curGearDmgPath))
				{
					curGearDmg = g_Game.ConfigGetFloat(curGearDmgPath);
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, this, "Current gear " + currentlyWornGear.GetType() + " armor dmg " + curGearDmg);
				#endif
				}

				if (g_Game.ConfigIsExisting(tgtGearDmgPath))
				{
					tgtGearDmg = g_Game.ConfigGetFloat(tgtGearDmgPath);
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, this, "Target gear " + targetItem.GetType() + " armor dmg " + tgtGearDmg);
				#endif
				}

				//! @note higher health level = worse! 0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined
				if (tgtGearDmg < curGearDmg || (tgtGearDmg == curGearDmg && targetItem.GetHealthLevel() < currentlyWornGear.GetHealthLevel()))
				{
					if (!eAI_WasItemRecentlyDropped(targetItem))
						return true;
				#ifdef DIAG_DEVELOPER
					else
						EXPrint(this, "Ignoring item for swap because it was recently dropped: " + targetItem + " (type=" + targetItem.GetType() + " health=" + targetItem.GetHealth() + ")");
				#endif
				}
			}
		}

		return false;
	}

	bool eAI_ClothingLootCheck(string slot, ItemBase item)
	{
		EXError.Error(this, "DEPRECATED - use eAI_ClothingLootingBehaviorCheck_Slot");
		return eAI_ClothingLootingBehaviorCheck_Slot(slot, item);
	}

	/**
	 * @brief Check if looting behavior allows respective slot
	 * 
	 * @param slot
	 * @param item
	 */
	bool eAI_ClothingLootingBehaviorCheck_Slot(string slot, ItemBase item)
	{
		slot.ToUpper();

		int behavior;

		switch (slot)
		{
			case "BACK":
				int itemWidth;
				int itemHeight;

				g_Game.GetInventoryItemSize(item, itemWidth, itemHeight);

				int itemSize = itemWidth * itemHeight;

				vector minMax[2];
				if (item.GetCollisionBox(minMax))
				{
					float w = minMax[1][0] - minMax[0][0];
					float h = minMax[1][1] - minMax[0][1];
					float d = minMax[1][2] - minMax[0][2];

					float max = Math.Max(Math.Max(w, h), d);

					if (itemSize >= 30 && w > 0.3 && max > 0.55 && d > 0.2)
						behavior = eAILootingBehavior.CLOTHING_BACK_LARGE;
					else if (itemSize > 20 && w > 0.3 && h > 0.39 && d > 0.2)
						behavior = eAILootingBehavior.CLOTHING_BACK_MEDIUM;
					else
						behavior = eAILootingBehavior.CLOTHING_BACK_SMALL;
				}
				else
				{
					if (itemSize >= 30)  //! e.g. ghillie suit
						behavior = eAILootingBehavior.CLOTHING_BACK_LARGE;
					else if (itemSize >= 16)  //! e.g. ghillie cloak
						behavior = eAILootingBehavior.CLOTHING_BACK_MEDIUM;
					else  //! e.g. ghillie shrug
						behavior = eAILootingBehavior.CLOTHING_BACK_SMALL;
				}

				break;

			default:
				behavior = typename.StringToEnum(eAILootingBehavior, "CLOTHING_" + slot);
				break;
		}

		if (m_eAI_LootingBehavior & behavior)
			return true;

		return false;
	}

	/**
	 * @brief Check if looting behavior generally allows identical or similar clothing
	 */
	bool eAI_ClothingLootingBehaviorCheck_Similarity()
	{
		if (m_eAI_LootingBehavior & (eAILootingBehavior.CLOTHING_IDENTICAL | eAILootingBehavior.CLOTHING_SIMILAR))
			return true;

		return false;
	}

	bool eAI_ClothingLootingBehaviorCheck_Selection(ItemBase currentlyWornGear, ItemBase targetItem)
	{
		//! @note similarity takes precedence and overrides upgrade!
		if (eAI_ClothingLootingBehaviorCheck_Similarity())
			return eAI_ClothingSimilarityCheck(currentlyWornGear, targetItem);
		else if (m_eAI_LootingBehavior & eAILootingBehavior.UPGRADE)
			return true;

		return false;
	}

	/**
	 * @brief Check if clothing item is identical (same type) or similar (same base type, e.g. TShirt_ColorBase) to currently worn gear
	 */
	bool eAI_ClothingSimilarityCheck(ItemBase currentlyWornGear, ItemBase targetItem)
	{
		if (m_eAI_LootingBehavior & eAILootingBehavior.CLOTHING_IDENTICAL)
		{
			if (targetItem.GetType() == currentlyWornGear.GetType())
				return true;
		}
		else if (m_eAI_LootingBehavior & eAILootingBehavior.CLOTHING_SIMILAR)
		{
			string baseName;
			if (g_Game.ConfigGetBaseName(CFG_VEHICLESPATH + " " + currentlyWornGear.GetType(), baseName))
			{
				if (ExpansionString.EndsWith(baseName, "_ColorBase") && ExpansionStatic.Is(targetItem, baseName))
					return true;
			}
		}

		return false;
	}

	/**
	 * @brief forget recently dropped item
	 * 
	 * @return true if item was previously remembered
	 * 
	 * @note items are remembered by their type name and position (fractional digits rounded to two decimals)
	 */
	bool eAI_ForgetRecentlyDroppedItem(ItemBase item)
	{
		string key = string.Format("%1@%2", item.GetType(), ExpansionStatic.VectorToString(item.GetPosition()));
		if (m_eAI_RecentlyDroppedItems.Contains(key))
		{
			m_eAI_RecentlyDroppedItems.Remove(key);
			return true;
		}

		return false;
	}

	/**
	 * @brief forget items dropped more than 5 minutes ago
	 * 
	 * @return number of stale items removed from memory
	 */
	int eAI_ForgetStaleRecentlyDroppedItems()
	{
		float time = g_Game.GetTickTime();

		TStringArray staleRecentlyDroppedItems = {};

		foreach (string key, float timeDropped: m_eAI_RecentlyDroppedItems)
		{
			if (time - timeDropped > 300.0)
				staleRecentlyDroppedItems.Insert(key);
		}

		foreach (string toRemove: staleRecentlyDroppedItems)
		{
			m_eAI_RecentlyDroppedItems.Remove(toRemove);
		}

		return staleRecentlyDroppedItems.Count();
	}

	/**
	 * @brief remember recently dropped item
	 * 
	 * @note items are remembered by their type name and position (fractional digits rounded to two decimals)
	 */
	void eAI_RememberRecentlyDroppedItem(ItemBase item)
	{
		eAI_ForgetStaleRecentlyDroppedItems();

		string key = string.Format("%1@%2", item.GetType(), ExpansionStatic.VectorToString(item.GetPosition()));
		m_eAI_RecentlyDroppedItems[key] = g_Game.GetTickTime();

		EXTrace.Print(EXTrace.AI, this, "Recently dropped items: " + m_eAI_RecentlyDroppedItems.Count());
	}

	/**
	 * @brief check if item was recently dropped
	 * 
	 * @return true if item was dropped 5 minutes ago or less
	 * 
	 * @note items are remembered by their type name and position (fractional digits rounded to two decimals)
	 */
	bool eAI_WasItemRecentlyDropped(ItemBase item)
	{
		string key = string.Format("%1@%2", item.GetType(), ExpansionStatic.VectorToString(item.GetPosition()));
		float timeDropped;
		if (m_eAI_RecentlyDroppedItems.Find(key, timeDropped) && g_Game.GetTickTime() - timeDropped <= 300.0)
			return true;

		return false;
	}

	override void SetGroup(eAIGroup group, bool autoDeleteFormerGroupIfEmpty = true, int groupMemberIndex = -1)
	{
		if (!group)
		{
			Error("Group cannot be NULL");
			return;
		}

		if (GetGroup() == group)
			return;

		//! Since target information is tracked per-group, need to clear if group changed.
		//! m_eAI_Targets can be NULL at this point because it's set in Init which is called from vanilla PlayerBase ctor,
		//! but group is assigned in DayZPlayerImplement ctor
		if (m_eAI_Targets)
		{
			for (int i = m_eAI_Targets.Count() - 1; i >= 0; i--)
			{
				eAITarget target = m_eAI_Targets[i];
				target.Remove();
			}
		}

		super.SetGroup(group, autoDeleteFormerGroupIfEmpty, groupMemberIndex);
	}

	bool eAI_RemoveTargets()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAI_RemoveTargets");
#endif

#ifdef EXTRACE_DIAG
		auto hitch = new EXHitch(ToString() + "::eAI_RemoveTargets ", 20000);
#endif

		int count = m_eAI_Targets.Count();

		for (int i = count - 1; i >= 0; i--)
		{
			eAITarget target = m_eAI_Targets[i];
			if (target.ShouldRemove())
			{
#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTargets - removing target " + target.GetDebugName());
#endif
				target.Remove();
			}
		}

#ifdef DIAG_DEVELOPER
		if (count > 0 && m_eAI_Targets.Count() == 0)
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveTargets - no more targets");
#endif

		return count != m_eAI_Targets.Count();
	}

	void eAI_PrioritizeTargets()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(03b) -> eAI_PrioritizeTargets");
#endif

		//! find the target with the highest threat level, no sorting
		//! (sorting performance would be abysmal if there are many targets due to processing n*(n/2)-n/2 targets when comparing threat levels)

		float threat;
		float max_threat = 0.099999;  //! 0.1 is the 'look at' threshold
		float noise_max_threat;
		int max_threat_idx;

		m_eAI_NoiseTarget = 0;
		m_eAI_AcuteDangerTargetCount = 0;
		m_eAI_AcuteDangerPlayerTargetCount = 0;

		foreach (int i, eAITarget target: m_eAI_Targets)
		{
			threat = target.GetThreat();
			if (threat > max_threat)
			{
				max_threat_idx = i;
				max_threat = threat;
			}
			else if (target.IsNoise() && !IsUnconscious())
			{
				m_eAI_NoiseTarget = i;
			}

			if (threat >= 0.4 && target.IsAcuteDanger())
				eAI_UpdateAcuteDangerTargetCount(target, 1);

		#ifdef DIAG_DEVELOPER
			if (m_eAI_PrintCurrentTarget)
				EXTrace.Print(EXTrace.AI, this, "eAI_PrioritizeTargets - target " + i + " " + target.GetDebugName() + " threat lvl " + threat);
		#endif
		}

		m_eAI_TargetChanged = false;

		if (max_threat_idx > 0)
		{
			eAITarget selectedTarget = m_eAI_Targets[max_threat_idx];

			if (selectedTarget.IsItem() && selectedTarget != m_eAI_ItemTargetHistory[0])
			{
				//! Avoid switching between same two item targets in alternating fashion as it can cause AI
				//! to move between these two items endlessly when it doesn't get close enough for pickup
				if (selectedTarget == m_eAI_ItemTargetHistory[1] && m_eAI_ItemTargetHistory[0] == m_eAI_ItemTargetHistory[2])
				{
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, this, "selectedTarget = " + selectedTarget);
					EXTrace.Print(EXTrace.AI, this, "m_eAI_ItemTargetHistory[0] = " + m_eAI_ItemTargetHistory[0]);
				#endif
					return;
				}

				m_eAI_ItemTargetHistory[2] = m_eAI_ItemTargetHistory[1];
				m_eAI_ItemTargetHistory[1] = m_eAI_ItemTargetHistory[0];
				m_eAI_ItemTargetHistory[0] = selectedTarget;

			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.AI, this, "selectedTarget = " + selectedTarget);
				EXTrace.Print(EXTrace.AI, this, "m_eAI_ItemTargetHistory[1] = " + m_eAI_ItemTargetHistory[1]);
				EXTrace.Print(EXTrace.AI, this, "m_eAI_ItemTargetHistory[2] = " + m_eAI_ItemTargetHistory[2]);
			#endif
			}

			eAITarget previousTarget = m_eAI_Targets[0];
			m_eAI_Targets[0] = selectedTarget;
			if (m_eAI_NoiseTarget == max_threat_idx)
				m_eAI_NoiseTarget = 0;
			else if (previousTarget.IsNoise() && !IsUnconscious())
				m_eAI_NoiseTarget = max_threat_idx;
			m_eAI_Targets[max_threat_idx] = previousTarget;
			m_eAI_TargetChanged = true;
			m_eAI_SyncCurrentTarget = true;
#ifdef DIAG_DEVELOPER
			m_eAI_PrintCurrentTarget = true;
		}

		if (m_eAI_PrintCurrentTarget && m_eAI_Targets[0])
		{
			m_eAI_PrintCurrentTarget = false;
			EXTrace.Print(EXTrace.AI, this, "eAI_PrioritizeTargets - prioritizing target " + max_threat_idx + " " + m_eAI_Targets[0].GetDebugName() + " threat lvl " + max_threat);
#endif
		}
	}

	void eAI_SyncCurrentTarget()
	{
		m_eAI_SyncCurrentTarget = false;

	#ifdef SERVER
		eAITarget target = GetTarget();

		EntityAI targetEntity;
		if (target)
			targetEntity = target.GetEntity();

		if (targetEntity && !targetEntity.IsItemBase())
		{
			targetEntity.GetNetworkID(m_eAI_CurrentTarget_NetIDLow, m_eAI_CurrentTarget_NetIDHigh);
		}
		else
		{
			m_eAI_CurrentTarget_NetIDLow = 0;
			m_eAI_CurrentTarget_NetIDHigh = 0;
		}

		SetSynchDirty();
	#endif
	}

	void eAI_UpdateCurrentTarget_Client()
	{
		eAITarget target = GetTarget();
		if (target)
			target.Remove();
		if (m_eAI_CurrentTarget_NetIDLow == 0 && m_eAI_CurrentTarget_NetIDHigh == 0)
			return;
		Object entity = g_Game.GetObjectByNetworkId(m_eAI_CurrentTarget_NetIDLow, m_eAI_CurrentTarget_NetIDHigh);
		if (!entity)
			return;
		eAITargetInformation info = eAITargetInformation.GetTargetInformation(entity);
		if (!info)
			return;
		target = info.AddAI(this);
		
		EXTrace.Print(EXTrace.AI, this, "Prioritizing target " + info.GetDebugName());
	}

	void eAI_OnNoiseEvent(EntityAI source, vector position, float lifetime, eAINoiseParams params, float strengthMultiplier)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "eAI_OnNoiseEvent");
#endif

		if (m_eAI_NoiseInvestigationDistanceLimit <= 0)
			return;

		if (IsUnconscious())
			return;

		float strength = params.m_Strength * strengthMultiplier;
		if (strength <= 0)
			return;

		EntityAI root;
		if (source)
		{
			root = source.GetHierarchyRoot();

			if (root == this)
				return;

			if (m_eAI_CurrentThreatToSelfActive >= 0.4 && m_eAI_HasLOS)
			{
				eAITarget currentTarget = GetTarget();
				if (currentTarget && currentTarget.GetEntity() == root)
					return;  //! Already targeting the source entity and have line of sight
			}
		}

		//! Friendly checks. Not using PlayerIsEnemy here is intentional.
		//! We ignore noises made by friendlies.
		DayZPlayerImplement player;
		eAIGroup ourGroup = GetGroup();
		eAIFaction faction = ourGroup.GetFaction();
		eAIPlayerTargetInformation info;
		if (Class.CastTo(player, root))
		{
			if (params.m_Type != eAINoiseType.BULLETIMPACT)
			{
				eAIGroup theirGroup = player.GetGroup();
				if (theirGroup)
				{
					if (theirGroup == ourGroup || theirGroup.GetFaction().IsFriendly(faction) || theirGroup.GetFaction().IsFriendlyEntity(this))
						return;
				}
			}

			info = player.GetTargetInformation();

			//! We ignore the noise if noise source is a player that we have line of sight to
			if (eAI_HasLOS(info))
				return;
		}

		if (source && position == vector.Zero)
			position = source.GetPosition();

		float strengthSq = strength * strength;
		float distSq = vector.DistanceSq(GetPosition(), position);
		if (distSq > strengthSq)
			return;

		//! If noise was bullet impact and source root is player, add/update player as target
		if (params.m_Type == eAINoiseType.BULLETIMPACT && info)
		{
			bool created;
			eAITarget target = info.AddAI(this, -1, true, created);

			//! If target threat level is above 0.2, player is enemy
			if (target.m_ThreatLevel > 0.2)
			{
				float threat = 1.0;

				//! Add for searching if desired threat is above active threat
				if (threat > target.m_ThreatLevelActive)
				{
					//! If m_SearchOnLOSLost is false, this means we never had LOS to this target,
					//! so we set initial position to noise position for searching
					if (!target.m_SearchOnLOSLost)
					{
						target.SetInitial(threat, position);
						target.m_SearchOnLOSLost = true;
					}
					else
					{
						target.SetThreat(threat);
					}
				}

				if (!created)
					target.Update();

				return;
			}
		}

		float distance = Math.Sqrt(distSq);

		//! Now, convert strength (range) to threat.
		//! @note noise threat is capped at 0.4 so entity targets can take precedence.
		float threatLevel;
		if (eAI_IsPassive() || faction.IsObserver() || faction.IsGuard())
		{
			threatLevel = ExpansionMath.PowerConversion(0.5, Math.Max(strength, 30.0), distance, 0.152, 0.1, 0.1);
		}
		else
		{
			/*
			if (root && !root.IsItemBase())
			{
				eAITargetInformation info;
				eAI_ProcessTarget(root, ourGroup, info);
				eAITargetInformationState state = eAI_GetTargetInformationState(info);
				if (state.m_ThreatLevel >= 0.4)
				{
					state.m_ThreatLevelActive = state.m_ThreatLevel;
					state.m_SearchPosition = ExpansionMath.GetRandomPointInCircle(position, distance * 0.05);
					state.m_SearchDirection = vector.Direction(GetPosition(), state.m_SearchPosition);
					return;
				}
			}
			*/

			float distMin;
			float threatLevelMax;

			if (params.m_Type == eAINoiseType.BULLETIMPACT)
			{
				distMin = 0.5;
				threatLevelMax = 0.2;
			}
			else
			{
				distMin = Math.Min(strength, m_eAI_NoiseInvestigationDistanceLimit);
				threatLevelMax = 0.4;
			}

			threatLevel = ExpansionMath.LinearConversion(distMin, strength * 1.1, distance, threatLevelMax, 0.1024);
		}

		//! Update noise target info
		if (threatLevel >= m_eAI_NoiseTargetInfo.GetThreat())
		{
			position = ExpansionMath.GetRandomPointInCircle(position, distance * 0.05);

			if (threatLevel >= 0.4)
				lifetime = ExpansionMath.LinearConversion(0.0, 500.0, distance, 3.0, 240.0);  //! Leave enough time to run there and check it out
			else
				lifetime = Math.RandomFloat(2.0, 4.0);  //! Just look briefly

			m_eAI_NoiseTargetInfo.SetNoiseParams(source, position, strength, lifetime, threatLevel);

			//! 2.915452 = 1000 / (speed of sound = 343 m/s)
			//! avg human reaction time to sound = 170 ms
			int delay = distance * 2.915452 + 170;
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_AddNoiseTarget, delay, false, threatLevel);
		}

	#ifdef DIAG_DEVELOPER
		string fmt = "::eAI_OnNoiseEvent %1 %2 %3 m %4 %5 s %6 x %7 %8 %9";
		string pos = position.ToString();
		string type = typename.EnumToString(eAINoiseType, params.m_Type);
		float paramStrength = params.m_Strength;
		string msg = string.Format(fmt, source, pos, distance, threatLevel, lifetime, paramStrength, strengthMultiplier, params.m_Path, type);

		EXTrace.Print(EXTrace.AI, this, msg);

		if (params.m_Type == eAINoiseType.BULLETIMPACT)
			ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + msg);
	#endif
	}

	void eAI_AddNoiseTarget(float threatLevel)
	{
		if (IsDamageDestroyed())
			return;

		if (threatLevel >= m_eAI_NoiseTargetInfo.GetThreat())
		{
			int maxTime = m_eAI_NoiseTargetInfo.GetLifetime() * 1000;
			bool created;

			eAITarget state = m_eAI_NoiseTargetInfo.AddAI(this, maxTime, true, created);

			if (!created)
			{
				state.UpdatePosition(true);
				state.UpdateFoundAtTime();
			}
		}
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_eAI_CurrentTarget_NetIDLowSync != m_eAI_CurrentTarget_NetIDLow || m_eAI_CurrentTarget_NetIDHighSync != m_eAI_CurrentTarget_NetIDHigh)
		{
			m_eAI_CurrentTarget_NetIDLowSync = m_eAI_CurrentTarget_NetIDLow;
			m_eAI_CurrentTarget_NetIDHighSync = m_eAI_CurrentTarget_NetIDHigh;
			eAI_UpdateCurrentTarget_Client();
		}
	}

	override void OnInputForRemote(ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputForRemote - cannot read input type");
			return;
		}
		
		switch (userDataType)
		{
			case INPUT_UDT_WEAPON_REMOTE_EVENT:
				if (GetHumanInventory().GetEntityInHands())
					GetDayZPlayerInventory().OnEventForRemoteWeapon(ctx);
				break;
			case INPUT_UDT_HAND_REMOTE_EVENT:
				GetDayZPlayerInventory().OnHandEventForRemote(ctx);
				break;
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnInputUserDataForRemote(ctx);
				break;
			default:
				Error("OnInputForRemote - unknown userDataType=" + userDataType);
				break;
		}
	}

	HumanCommandMove GetCommand_MoveAI()
	{
		EXError.ErrorOnce(this, "DEPRECATED, use GetCommand_Move()");
		return GetCommand_Move();
	}

	HumanCommandMove StartCommand_MoveAI()
	{
		int stance = -1;
		if (GetCommand_Unconscious() && !IsSwimming() && m_LastCommandBeforeUnconscious != DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			stance = DayZPlayerConstants.STANCEIDX_PRONE;
		}
		else if (GetCommand_Move() || GetCommand_Action() || GetCommand_Damage())
		{
			stance = m_MovementState.m_iStanceIdx;
			if (m_MovementState.IsRaised())
				stance -= DayZPlayerConstants.STANCEIDX_RAISED;
		}
		EXTrace.Print(EXTrace.AI, this, "StartCommand_MoveAI " + typename.EnumToString(eAIStance, stance) + " (" + stance + ")");
		HumanCommandMove cmd = StartCommand_Move();
		if (stance != -1)
			m_eAI_CommandMove.SetStance(stance);
		if (m_WeaponRaised)
			AnimSetBool(m_ExpansionST.m_VAR_Raised, m_WeaponRaised);
		return cmd;
	}

	HumanCommandVehicle GetCommand_VehicleAI()
	{
		EXError.ErrorOnce(this, "DEPRECATED, use GetCommand_Vehicle()");
		return GetCommand_Vehicle();
	}

	HumanCommandVehicle StartCommand_VehicleAI(Transport vehicle, int seatIdx, int seat_anim, bool fromUnconscious = false)
	{
		EXError.ErrorOnce(this, "DEPRECATED, use StartCommand_Vehicle()");
		return StartCommand_Vehicle(vehicle, seatIdx, seat_anim, fromUnconscious);
	}

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

	void Notify_Melee(bool melee = true)
	{
		if (melee && !m_eAI_MeleeFightLogic.m_eAI_Melee)
		{
			Expansion_GetUp();

			if (!IsFighting() && GetExpansionSettings().GetAI().MemeLevel > 9000)
				s_eAI_LoveSound02_SoundSet.Play(this);
		}

		m_eAI_MeleeFightLogic.m_eAI_Melee = melee;
	}

	override void OnCommandMelee2Start()
	{
		super.OnCommandMelee2Start();
		EXTrace.Print(EXTrace.AI, this, "OnCommandMelee2Start");
		m_eAI_MeleeTime = g_Game.GetTime();
	}

	override void EndFighting()
	{
		super.EndFighting();
		EXTrace.Print(EXTrace.AI, this, "EndFighting - melee time " + (g_Game.GetTime() - m_eAI_MeleeTime) + " ms");
	}

	override void OnCommandLadderFinish()
	{
		super.OnCommandLadderFinish();

		m_PathFinding.ForceRecalculate(true);
	}

	bool eAI_CanEnableSwimming()
	{
		//! @note order matters! To preserve intent, conditions that return false need to come first,
		//! then those that return true followed by final return false

		if (IsSwimming())
			return false;

		if (IsRestrained())
			return false;

		if (m_PathFinding.m_IsSwimmingEnabled)
			return false;

		if (IsBleeding() && m_eAI_DangerousAreaCount == 0)
			return false;

		if (m_eAI_EffectArea)
			return true;

		if (GetCurrentWaterLevel() >= -0.5 || m_eAI_SurfaceY < g_Game.SurfaceGetSeaLevelMin())
		{
			if (GetThreatToSelf() < 0.2)
				return true;
		}

		return false;
	}

	override void OnCommandSwimStart()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 
		
		ItemBase itemInHands = GetItemInHands();

		//! Switch off hand item when starting to swim
		if (itemInHands && itemInHands.Expansion_TryTurningOffAnyLightOrNVG(true))
			itemInHands = GetItemInHands();  //! In case switching off hands item replaces it with something else

		if (itemInHands)
		{
			if (itemInHands.IsHeavyBehaviour())
			{
				TryHideItemInHands(false);
				eAI_DropItemInHandsImpl(true);
			}
			else
			{
				TryHideItemInHands(true);
			}
			itemInHands.OnItemInHandsPlayerSwimStart(this);
		}
		m_AnimCommandStarting = HumanMoveCommandID.CommandSwim;
		
		AbortWeaponEvent();
		GetWeaponManager().DelayedRefreshAnimationState(10);
		RequestHandAnimationStateRefresh();

		if (!g_Game.IsServer())
			return;

		bool wasSwimmingEnabled = m_PathFinding.m_IsSwimmingEnabled;

		if (GetGroup().GetFormationLeader() == this)
			GetGroup().EnableSwimming(true);  //! Enable swimming for the whole group so they can follow the leader
		else
			m_PathFinding.EnableSwimming(true);

		if (wasSwimmingEnabled && !m_Swimming.m_bWasSwimming)
			m_PathFinding.ForceRecalculate(true);
	}

	override void OnCommandSwimFinish()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 
		
		TryHideItemInHands(false, true);
		
		GetWeaponManager().RefreshAnimationState();

		//auto leader = GetGroup().GetFormationLeader();
		//if (leader == this)
			//GetGroup().EnableSwimming(false);  //! Disable swimming for the whole group (unless they are currently swimming)
		//else if (!leader.IsSwimming())
			//m_PathFinding.EnableSwimming(false);
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (g_Game.IsServer())
			eAI_UpdateProtectionLevels(item, 1, slot_name);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (g_Game.IsServer())
			eAI_UpdateProtectionLevels(item, -1, slot_name);
	}

	override void EEItemIntoHands(EntityAI item)
	{
		super.EEItemIntoHands(item);

		//! Set melee weapon so we can use GetRange()
		m_eMeleeCombat.Reset(InventoryItem.Cast(item), EMeleeHitType.NONE, false);

	#ifdef EXPANSION_AI_MELEEDBG_CHATTY
		ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + " EEItemIntoHands " + item + " melee range " + m_eMeleeCombat.eAI_GetRange());
	#endif

		Weapon_Base weapon;
		if (Class.CastTo(weapon, item))
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(weapon.eAI_SetFireModeAuto);

		if (g_Game.IsServer() && item.IsInherited(RestrainingToolLocked))
		{
			if (AI_HANDLEDOORS)
				m_PathFinding.AllowClosedDoors(false, 0.0);

			if (AI_HANDLEVAULTING)
				m_PathFinding.SetAllowJumpClimb(false, 0.0);

			m_PathFinding.EnableSwimming(false);
		}
	}

	override void EEItemOutOfHands(EntityAI item)
	{
		super.EEItemOutOfHands(item);

		if (!ToDelete() && m_eMeleeCombat.eAI_GetWeapon() == item)
		{
			//! Set melee so we can use GetRange()
			m_eMeleeCombat.Reset(null, EMeleeHitType.NONE, false);

		#ifdef EXPANSION_AI_MELEEDBG_CHATTY
			ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + " EEItemOutOfHands " + item + " melee range (fists) " + m_eMeleeCombat.eAI_GetRange());
		#endif
		}

		if (g_Game.IsServer() && item.IsInherited(RestrainingToolLocked))
		{
			if (AI_HANDLEDOORS)
				m_PathFinding.AllowClosedDoors(true, 0.0);

			if (AI_HANDLEVAULTING)
				m_PathFinding.SetAllowJumpClimb(true, 0.0);
		}
	}

	void eAI_UpdateProtectionLevels(EntityAI entity, int value, string slotName = string.Empty)
	{
		if (slotName)
		{
			if (!GetBleedingManagerServer())  //! Vanilla deletes BleedingManagerServer in EEKilled
				return;

			set<int> slotIDs = GetBleedingManagerServer().GetBleedingSourcesLocations();
			int slotID = InventorySlots.GetSlotIdFromString(slotName);
			if (slotIDs.Find(slotID) == -1)
				return;
		}

		ItemBase item;
		if (Class.CastTo(item, entity))
		{
			eAI_UpdateProtectionLevel(DEF_BIOLOGICAL, item, value);
			eAI_UpdateProtectionLevel(DEF_CHEMICAL, item, value);
		}
	}

	void eAI_UpdateProtectionLevel(int type, ItemBase item, int value)
	{
		float level = item.GetProtectionLevel(type, true) * value;

		m_eAI_ProtectionLevels[type] = m_eAI_ProtectionLevels[type] + level;

	#ifdef DIAG_DEVELOPER
		if (level != 0)
		{
			if (type == DEF_CHEMICAL)
				EXTrace.Print(EXTrace.AI, this, "Chemical protection level: " + m_eAI_ProtectionLevels[type]);
			else
				EXTrace.Print(EXTrace.AI, this, "Biological protection level: " + m_eAI_ProtectionLevels[type]);
		}
	#endif
	}

	void eAI_UpdateProtectionLevels()
	{
		if (!GetBleedingManagerServer())  //! Vanilla deletes BleedingManagerServer in EEKilled
			return;

		set<int> slotIDs = GetBleedingManagerServer().GetBleedingSourcesLocations();

		m_eAI_ProtectionLevels[DEF_BIOLOGICAL] = 0;
		m_eAI_ProtectionLevels[DEF_CHEMICAL] = 0;

		foreach (int slotID: slotIDs)
		{
			EntityAI attachment = GetInventory().FindAttachment(slotID);

			eAI_UpdateProtectionLevels(attachment, 1);
		}
	}

	override void Expansion_OnDangerousAreaEnterServer(EffectArea area, Trigger trigger)
	{
	#ifdef DIAG_DEVELOPER
		ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaEnterServer " + area + " " + area.m_Position + " " + area.m_Radius);
	#endif

		m_eAI_DangerousAreaCount++;

		if (!m_eAI_EffectArea || area.m_Expansion_MergedCluster != m_eAI_EffectArea.m_Expansion_MergedCluster)
		{
		#ifdef DIAG_DEVELOPER
			ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaEnterServer " + area.m_Expansion_MergedCluster + " " + area.m_Expansion_MergedCluster.m_Position + " " + area.m_Expansion_MergedCluster.m_Radius);
		#endif

			ExpansionEffectAreaMergedCluster cluster = area.m_Expansion_MergedCluster;
			EffectArea closestArea;
			vector position = GetPosition();
			vector target = m_PathFinding.GetTarget();
			vector left = target;
			vector right = target;

			cluster.FindClosestPointOutsideCluster(position, left, 1.0, true, closestArea);
			cluster.FindClosestPointOutsideCluster(position, right, -1.0, true, closestArea);

			float distToTargetLeft = vector.DistanceSq(left, target);
			float distToTargetRight = vector.DistanceSq(right, target);

			if (distToTargetLeft < distToTargetRight)
			{
				GetGroup().m_DangerousAreaAvoidanceDirection = 1.0;

			#ifdef DIAG_DEVELOPER
				ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaEnterServer avoid left");
			#endif
			}
			else if (distToTargetLeft > distToTargetRight)
			{
				GetGroup().m_DangerousAreaAvoidanceDirection = -1.0;

			#ifdef DIAG_DEVELOPER
				ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaEnterServer avoid right");
			#endif
			}
			else
			{
				GetGroup().RandomizeDangerousAreaAvoidanceDirection();

			#ifdef DIAG_DEVELOPER
				ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaEnterServer avoid random");
			#endif
			}
		}

		m_eAI_EffectArea = area;
	}

	override void Expansion_OnDangerousAreaExitServer(EffectArea area, Trigger trigger)
	{
	#ifdef DIAG_DEVELOPER
		ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " Expansion_OnDangerousAreaExitServer " + area + " " + area.m_Position + " " + area.m_Radius);
	#endif

		m_eAI_DangerousAreaCount--;
		if (m_eAI_DangerousAreaCount < 0)
			m_eAI_DangerousAreaCount = 0;
	}

	void eAI_ForgetDangerousAreaCluster()
	{
		if (m_eAI_DangerousAreaCount == 0)
		{
		#ifdef DIAG_DEVELOPER
			auto cluster = m_eAI_EffectArea.m_Expansion_MergedCluster;
			if (cluster)
				ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " eAI_ForgetDangerousAreaCluster " + cluster + " " + cluster.m_Position + " " + cluster.m_Radius);
			else
				ExpansionStatic.MessageNearPlayers(GetPosition(), 1000, ToString() + " eAI_ForgetDangerousAreaCluster");
		#endif

			m_eAI_EffectArea = null;
		}
	}

	/**
	 * @brief Overrides the desired position to generate a new path
	 *
	 * @param pPosition the target position for path finding
	 */
	void OverrideTargetPosition(vector pPosition, bool isFinal = true, float maxDistance = 1.0, bool allowJumpClimb = true)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OverrideTargetPosition").Add(pPosition);
#endif

		vector oPos = pPosition;

		vector position = m_ExTransformPlayer[3];

		if (m_eAI_Ladder && eAI_CheckShouldUseBuildingWithLadder(pPosition))
		{
			if (m_eAI_IsOnLadder)
			{
				//! Allow changing direction (up/down) while climbing ladder if target position changes
				//! Currently problematic if there is more than one ladder e.g. if we need to go up one and down the next to get to target,
				//! so we limit to buildings with just one ladder
				//! DISABLED for now, since pPosition is NOT suitable to check whether we need to go down or not
			/*
				if (m_eAI_BuildingWithLadder.Expansion_GetLaddersCount() == 1)
				{
					vector modelTgtPos = m_eAI_BuildingWithLadder.WorldToModel(pPosition);
					vector btm = m_eAI_Ladder.m_Con[0];
					vector top = m_eAI_Ladder.m_Con[1];
					float middle = btm[1] + (top[1] - btm[1]) * 0.5;
					if (modelTgtPos[1] > middle)
						m_eAI_LadderClimbDirection = 1;
					else if (modelTgtPos[1] < middle)
						m_eAI_LadderClimbDirection = -1;
				}
			*/

				Expansion_DebugObject(740038, vector.Zero, "ExpansionDebugBox_Yellow");

				return;
			}
			else
			{
				pPosition = m_eAI_LadderEntryPoint;

				Expansion_DebugObject(740038, pPosition, "ExpansionDebugBox_Yellow", vector.Direction(position, pPosition), position);
			}
		}
		else if (eAI_CheckShouldClimbLadderToReachPosition(pPosition))
		{
			//! Traverse ladders by a process of elimination. This means we won't necessarily be taking the optimal path,
			//! but makes it unlikely to get stuck permanently.
			//! Ladders that have been used or are not reachable are removed from the pool if there is more than one ladder,
			//! when count reaches zero, all ladders are added back into the pool.

			map<int, ref ExpansionLadder> ladders;
			if (!m_eAI_Ladders.Find(m_eAI_BuildingWithLadder, ladders))
			{
				ladders = new map<int, ref ExpansionLadder>;
				m_eAI_Ladders[m_eAI_BuildingWithLadder] = ladders;
			}

			if (ladders.Count() == 0)
			{
				ExpansionUtil2<int, ExpansionLadder>.RefCopy(m_eAI_BuildingWithLadder.m_Expansion_Ladders, ladders);
				m_eAI_LadderLoops++;
			#ifdef DIAG_DEVELOPER
				string msg = "Ladder loops " + m_eAI_LadderLoops;
				EXTrace.Print(EXTrace.AI, this, msg);
				ExpansionStatic.MessageNearPlayers(position, 100, ToString() + " " + msg);
			#endif
			}

			TIntArray distancesSqBtm = {};
			TIntArray distancesSqTop = {};

			map<int, ref ExpansionLadder> laddersByDistSqBtm = new map<int, ref ExpansionLadder>;
			map<int, ref ExpansionLadder> laddersByDistSqTop = new map<int, ref ExpansionLadder>;

			vector modelPos = m_eAI_BuildingWithLadder.WorldToModel(position);

			float lowestLadderBtm = float.MAX;
			//float lowestLadderTop = float.MAX;

			foreach (int ladderIndex, ExpansionLadder ladder: ladders)
			{
				vector btm = ladder.m_Con[0];
				vector top = ladder.m_Con[1];

				lowestLadderBtm = Math.Min(btm[1], lowestLadderBtm);
				//lowestLadderTop = Math.Min(top[1], lowestLadderTop);

				//! @note the Y difference is used for weighting
				int distSqBtm = ExpansionMath.Distance2DSq(modelPos, btm) * Math.AbsFloat(modelPos[1] - btm[1]);
				int distSqTop = ExpansionMath.Distance2DSq(modelPos, top) * Math.AbsFloat(modelPos[1] - top[1]);

				distancesSqBtm.Insert(distSqBtm);
				distancesSqTop.Insert(distSqTop);

				laddersByDistSqBtm[distSqBtm] = ladder;
				laddersByDistSqTop[distSqTop] = ladder;
			}

			vector modelTgtPos = m_eAI_BuildingWithLadder.WorldToModel(pPosition);

			if (modelTgtPos[1] < lowestLadderBtm || !m_eAI_TargetIsInBuildingWithLadderRadius)
			{
				if (!m_eAI_FloorIsBuildingWithLadder || (Math.AbsFloat(modelTgtPos[1] - modelPos[1]) < 0.5 && modelPos[1] < lowestLadderBtm && m_eAI_LastClimbedBuildingWithLadder != m_eAI_BuildingWithLadder))
					m_eAI_PreferLadder = false;
			}

			if (m_eAI_PreferLadder)
			{
				distancesSqBtm.Sort();
				distancesSqTop.Sort();

				int closestBtm = distancesSqBtm[0];
				int closestTop = distancesSqTop[0];

				ExpansionLadder closestLadder;
				vector entryPoint;
				vector dirPoint;
				int climbDir;

				if (closestBtm < closestTop)
				{
					//! Go to btm pos
					closestLadder = laddersByDistSqBtm[closestBtm];
					entryPoint = m_eAI_BuildingWithLadder.ModelToWorld(closestLadder.m_Con[0]);
					dirPoint = m_eAI_BuildingWithLadder.ModelToWorld(closestLadder.m_ConDir[0]);
					climbDir = 1;
				}
				else
				{
					//! Go to top pos
					closestLadder = laddersByDistSqTop[closestTop];
					entryPoint = m_eAI_BuildingWithLadder.ModelToWorld(closestLadder.m_Con[1]);
					dirPoint = m_eAI_BuildingWithLadder.ModelToWorld(closestLadder.m_ConDir[1]);
					climbDir = -1;
				}

				if (g_Game.GetWaterDepth(m_eAI_LadderEntryPoint) < 1.0 || eAI_IsInLadderRadius(entryPoint))
				{
					//! Only use ladder if entry point is not underwater or we are in radius

					m_eAI_Ladder = closestLadder;
					m_eAI_LadderEntryPoint = entryPoint;
					m_eAI_LadderDirPoint = dirPoint;
					m_eAI_LadderClimbDirection = climbDir;

					pPosition = m_eAI_LadderEntryPoint;

					Expansion_DebugObject(740038, pPosition, "ExpansionDebugBox_Yellow", vector.Direction(position, pPosition), position);
				}
				else
				{
					Expansion_DebugObject(740038, vector.Zero, "ExpansionDebugBox_Yellow");
				}
			}
		}
		else
		{
			m_eAI_Ladder = null;

		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(740038, vector.Zero, "ExpansionDebugBox_Yellow");
		#endif
		}

		//! Try and go around any contaminated area if not fuly protected by following the outer edge
		if (!m_eAI_Ladder && m_eAI_IsInDangerByArea)
		{
			ExpansionEffectAreaMergedCluster cluster = m_eAI_EffectArea.m_Expansion_MergedCluster;
			EffectArea closestArea;
			float tolerance = Math.Max(Math.Min(m_eAI_EffectArea.m_Radius * 0.1, 15.0), 3.0);

		#ifdef DIAG_DEVELOPER
			Object dbgObj;
			vector dir;
		#endif

			if (m_eAI_DangerousAreaAvoidancePosition != vector.Zero && !Math.IsPointInCircle(m_eAI_DangerousAreaAvoidancePosition, 2.0, position) && !m_PathFinding.m_IsTargetUnreachable && Math.IsPointInCircle(m_eAI_TargetPosition, tolerance, pPosition))
			{
				pPosition = m_eAI_DangerousAreaAvoidancePosition;
				isFinal = false;
			}
			else if (cluster.FindClosestPointOutsideCluster(position, pPosition, GetGroup().m_DangerousAreaAvoidanceDirection, true, closestArea))
			{
				pPosition = ExpansionStatic.GetSurfaceRoadPosition(pPosition, RoadSurfaceDetection.CLOSEST);

				float waterDepth = g_Game.GetWaterDepth(pPosition);
				if (waterDepth > 0)
					pPosition[1] = pPosition[1] + waterDepth;

			#ifdef DIAG_DEVELOPER
				dir = pPosition - position;
				dir[1] = 0;
				Expansion_DebugObject(3142, pPosition, "ExpansionDebugNoticeMe_Cyan", dir);
				//Expansion_DebugObject(3141, vector.Zero, "ExpansionDebugNoticeMe_Black");
				dbgObj = Expansion_DebugObject(3143 + cluster.m_Areas.Count(), closestArea.m_Position, "ExpansionDebugNoticeMe_White", vector.Zero, pPosition, 300.0, ShapeFlags.NOZBUFFER);
				if (dbgObj)
				{
					dir = closestArea.m_Position - pPosition;
					dir[1] = 0;
					dbgObj.SetOrientation(dir.VectorToAngles());
				}
				dbgObj = Expansion_DebugObject(3143, closestArea.m_Position - "0 1.5 0", "ExpansionDebugNoticeMe_Yellow", vector.Zero, position, 300.0, ShapeFlags.NOZBUFFER);
				if (dbgObj)
				{
					dir = closestArea.m_Position - position;
					dir[1] = 0;
					dbgObj.SetOrientation(dir.VectorToAngles());
				}
			#endif

				//! If the original target position is inside the closest area, stop unless we are attempting to flank
				if (m_eAI_DangerousAreaCount == 0 && Math.IsPointInCircle(closestArea.m_Position, closestArea.m_Radius + 3.0, oPos) && !m_eAI_ShouldTakeCover)
				{
					isFinal = true;
					m_PathFinding.m_IsTargetUnreachable = true;
					m_PathFinding.m_IsUnreachable = true;
					pPosition = oPos;
				}
				else
				{
					m_eAI_DangerousAreaAvoidancePosition = pPosition;
					isFinal = false;

					m_PathFinding.ForceRecalculate(true);
				}
			}
			else
			{
				m_eAI_DangerousAreaAvoidancePosition = vector.Zero;

			#ifdef DIAG_DEVELOPER
				Expansion_DebugObject(3142, vector.Zero, "ExpansionDebugNoticeMe_Cyan");
				//dir = pPosition - position;
				//dir[1] = 0;
				//player.Expansion_DebugObject(3141, pPosition - "0 1.5 0", "ExpansionDebugNoticeMe_Black", dir, position);
				Expansion_DebugObject(3143 + cluster.m_Areas.Count(), vector.Zero, "ExpansionDebugNoticeMe_White");
			#endif

				if (!Math.IsPointInCircle(cluster.m_Position, cluster.m_Radius + 100.0, position))
					eAI_ForgetDangerousAreaCluster();
			}
		}

#ifndef EAI_USE_LEGACY_PATHFINDING
		m_PathFinding.SetTarget(pPosition, maxDistance, allowJumpClimb);
#else
		m_PathFinding.OverridePosition(pPosition);
#endif

		m_eAI_TargetPosition = oPos;
		m_eAI_TargetPositionIsFinal = isFinal;
	}

	bool eAI_CheckShouldClimbLadderToReachPosition(vector targetPos)
	{
		if (m_eAI_IsOnLadder)
			return true;

		if (m_eAI_IsFightingFSM && m_eAI_AcuteDangerPlayerTargetCount > 0)
			return false;

		vector position = m_ExTransformPlayer[3];

		if (!m_eAI_BuildingWithLadder)
		{
			if (Math.AbsFloat(targetPos[1] - position[1]) <= 1.5)
				return false;
		}
		else if (!m_PathFinding.m_IsUnreachable && !Math.IsPointInCircle(m_PathFinding.GetEnd(), 0.55, position))
		{
			return false;
		}

		IEntity floor = PhysicsGetFloorEntity();
		BuildingBase building;
		if (Class.CastTo(building, floor) && building != m_eAI_BuildingWithLadder && building.Expansion_GetLaddersCount() > 0 && !eAI_IsExcludedBuildingWithLadder(building))
		{
		#ifdef DIAG_DEVELOPER
			string msg = "Physics floor entity " + building.GetType() + " is building with ladder";
			if (m_eAI_BuildingWithLadder)
				msg += " and different from " + m_eAI_BuildingWithLadder.GetType();
			EXTrace.Print(EXTrace.AI, this, msg);
			ExpansionStatic.MessageNearPlayers(position, 100, ToString() + " " + msg);
		#endif

			m_eAI_BuildingWithLadder = building;
			m_eAI_LadderLoops = 0;
		}
		else if (!m_eAI_BuildingWithLadder)
		{
			return false;
		}

		if (building == m_eAI_BuildingWithLadder)
			m_eAI_FloorIsBuildingWithLadder = true;
		else
			m_eAI_FloorIsBuildingWithLadder = false;

		if (!eAI_CheckShouldUseBuildingWithLadder(targetPos))
			return false;

		if (m_PathFinding.m_IsTargetUnreachable)
			m_eAI_PreferLadder = true;

		return m_eAI_PreferLadder;
	}

	bool eAI_IsExcludedBuildingWithLadder(Object building)
	{
		string type = building.GetType();

		switch (type)
		{
			case "Land_Pier_Crane2_Base":
				if (m_eAI_LastClimbedLadder)
				{
					if (m_eAI_LastClimbedBuildingWithLadder == building && m_eAI_LastClimbedLadder.m_Index == 2)
					{
						//! Allow transition to top half of crane
						return true;
					}
				}
				break;

			case "Land_Pier_Crane2_Top":
				if (m_eAI_LastClimbedLadder)
				{
					if (m_eAI_LastClimbedBuildingWithLadder == building && m_eAI_LastClimbedLadder.m_Index == 3)
					{
						//! Allow transition to btm half of crane
						return true;
					}
				}
				break;

			case "Land_Pier_Crane_A":
				if (m_eAI_LastClimbedLadder)
				{
					if (m_eAI_LastClimbedBuildingWithLadder == building && m_eAI_LastClimbedLadder.m_Index == 2)
					{
						//! Allow transition to top half of crane
						return true;
					}
				}
				break;

			case "Land_Pier_Crane_B":
				if (m_eAI_LastClimbedLadder)
				{
					if (m_eAI_LastClimbedBuildingWithLadder == building && m_eAI_LastClimbedLadder.m_Index == 1)
					{
						//! Allow transition to btm half of crane
						return true;
					}
				}
				break;

			case "Land_Geoplant_PipeHall":
				//! Incorrect navmesh on top of accessory towers
				return true;

			case "Land_Geoplant_CoolingStack":
				//! Ladder unreachable via navmesh
				return true;
		}

		return false;
	}

	bool eAI_CheckShouldUseBuildingWithLadder(vector targetPos)
	{
		vector center = m_eAI_BuildingWithLadder.GetPosition();
		float radius = ExpansionStatic.GetBoundingRadius(m_eAI_BuildingWithLadder);
		vector position = m_ExTransformPlayer[3];

	#ifdef DIAG_DEVELOPER
		string msg;
	#endif

		if (Math.IsPointInCircle(center, radius, targetPos))
			m_eAI_TargetIsInBuildingWithLadderRadius = true;
		else
			m_eAI_TargetIsInBuildingWithLadderRadius = false;

		//! If neither target position nor player position is in radius of building, ignore building and ladders
		if (((!m_eAI_TargetIsInBuildingWithLadderRadius || m_eAI_DangerousAreaAvoidancePosition != vector.Zero) && !Math.IsPointInCircle(center, radius, position)))
		{
		#ifdef DIAG_DEVELOPER
			msg = "Target position or AI no longer in radius of " + m_eAI_BuildingWithLadder.GetType();
			EXTrace.Print(EXTrace.AI, this, msg);
			ExpansionStatic.MessageNearPlayers(position, 100, ToString() + " " + msg);
		#endif

			m_eAI_BuildingWithLadder = null;
			m_eAI_Ladder = null;
			m_eAI_PreferLadder = false;
			m_eAI_LadderLoops = 0;
			return false;
		}

		if (m_eAI_LadderLoops == 3)
		{
		#ifdef DIAG_DEVELOPER
			if (m_eAI_Ladder)
			{
				msg = "Exceeded maximum ladder loops";
				EXTrace.Print(EXTrace.AI, this, msg);
				ExpansionStatic.MessageNearPlayers(position, 100, ToString() + " " + msg);
			}
		#endif

			m_eAI_Ladder = null;
			m_eAI_PreferLadder = false;

			return false;
		}

		return true;
	}

	bool eAI_CanReachLadderEntryPoint()
	{
		vector begPos = m_ExTransformPlayer[3];

		begPos[1] = begPos[1] + 1.1;

		PhxInteractionLayers collisionLayerMask;
		collisionLayerMask |= PhxInteractionLayers.ROADWAY;
		collisionLayerMask |= PhxInteractionLayers.BUILDING;
		collisionLayerMask |= PhxInteractionLayers.DOOR;
		collisionLayerMask |= PhxInteractionLayers.FENCE;
		collisionLayerMask |= PhxInteractionLayers.VEHICLE;
		collisionLayerMask |= PhxInteractionLayers.TERRAIN;
		collisionLayerMask |= PhxInteractionLayers.ITEM_LARGE;

		vector contactPos;
		vector contactDir;
		Object hitObj;
		float hitFraction;

		//! Have to use RayCastBullet/SphereCastBullet because raycastRV can not ignore water so won't work as intended while swimming
		if (DayZPhysics.SphereCastBullet(begPos, m_eAI_LadderEntryPoint, 0.1, collisionLayerMask, this, hitObj, contactPos, contactDir, hitFraction))
		{
		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(740039, contactPos, "ExpansionDebugBox_Cyan", vector.Direction(begPos, contactPos), begPos);
		#endif

			vector dir = vector.Direction(m_eAI_LadderDirPoint, m_eAI_LadderEntryPoint).Normalized();
			vector perpend = dir.Perpend();
			vector min = m_eAI_LadderEntryPoint - perpend * 0.35 - dir * 0.45;
			vector max = m_eAI_LadderEntryPoint + perpend * 0.35 + dir * 0.15;

		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(740040, min, "ExpansionDebugBox_White", vector.Direction(m_eAI_LadderEntryPoint, m_eAI_LadderDirPoint));
			Expansion_DebugObject(740041, max, "ExpansionDebugBox_White", vector.Direction(m_eAI_LadderEntryPoint, m_eAI_LadderDirPoint));
		#endif

			if (Math.IsPointInRotatedRectangle(min, max, 0.6, contactPos))
				return true;

			return false;
		}

		return true;
	}

	bool eAI_IsCloseToLadderEntryPoint(float maxDist = 2.282542)
	{
		vector begPos = m_ExTransformPlayer[3];

		if (m_eAI_LadderEntryPoint[1] > begPos[1])
			begPos[1] = begPos[1] + 1.1;

		if (vector.DistanceSq(begPos, m_eAI_LadderEntryPoint) < maxDist * maxDist)
		{
		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(740043, vector.Zero, "ExpansionDebugBox_Orange");
		#endif
			return true;
		}

	#ifdef DIAG_DEVELOPER
		Expansion_DebugObject(740043, begPos, "ExpansionDebugBox_Orange", vector.Direction(begPos, m_eAI_LadderEntryPoint), m_eAI_LadderEntryPoint);
	#endif

		return false;
	}

	bool eAI_IsInLadderRadius(vector entryPoint)
	{
		vector begPos = m_ExTransformPlayer[3];

		if (Math.IsPointInCircle(entryPoint, UAMaxDistances.LADDERS, begPos))
		{
		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(740044, vector.Zero, "ExpansionDebugBox_Red");
		#endif
			return true;
		}

	#ifdef DIAG_DEVELOPER
		Expansion_DebugObject(740044, begPos, "ExpansionDebugBox_Red", vector.Direction(begPos, entryPoint), entryPoint);
	#endif

		return false;
	}

	void eAI_CheckIsInCover()
	{
		if (m_eAI_CurrentCoverObject && Math.IsPointInCircle(GetPosition(), 0.55, m_eAI_CurrentCoverPosition))
		{
			if (!m_eAI_IsInCover)
				m_eAI_Callbacks.OnEnterCover(m_eAI_CurrentCoverObject);
			m_eAI_IsInCover = true;
		}
		else
		{
			if (m_eAI_IsInCover)
				m_eAI_Callbacks.OnLeaveCover(m_eAI_CurrentCoverObject);
			m_eAI_IsInCover = false;
		}
	}

	//! @note in danger by area doesn't necessarily mean currently in area
	void eAI_CheckIsInDangerByArea()
	{
		if (m_eAI_EffectArea && m_eAI_EffectArea.eAI_IsDangerousToAI(this))
			m_eAI_IsInDangerByArea = true;
		else
			m_eAI_IsInDangerByArea = false;
	}

	void eAI_CheckIsAttachedToMovingParent()
	{
		IEntity parent = Expansion_GetParent();

		if (parent && GetVelocity(parent).LengthSq() > 0.3)
		{
		#ifdef DIAG_DEVELOPER
			if (!m_eAI_IsAttachedToMovingParent)
				ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + " attached to moving parent " + parent);
		#endif
			m_eAI_IsAttachedToMovingParent = true;
		}
		else
		{
		#ifdef DIAG_DEVELOPER
			if (m_eAI_IsAttachedToMovingParent)
				ExpansionStatic.MessageNearPlayers(GetPosition(), 100, ToString() + " No longer attached or parent is no longer moving");
		#endif
			m_eAI_IsAttachedToMovingParent = false;
		}

	}

	void OverrideTargetPosition(eAITarget target, bool isFinal = true)
	{
		if (m_eAI_IsOnLadder)
			return;

		vector pos;
		vector dir = target.GetDirection();

		float dist = dir.Length();
		float minDist = target.GetMinDistance(dist);
		bool allowJumpClimb = true;
		bool keepMinDistToTarget;
		bool cannotMelee;
		bool flank;

		if (minDist)
		{
			//! Return early if we are already overriding position and not yet near endpoint
			if (!eAI_ShouldUpdatePosition())
			{
				if (m_eAI_IsInDangerByArea)
					OverrideTargetPosition(m_eAI_TargetPosition, m_eAI_TargetPositionIsFinal, 1.0, m_PathFinding.m_AllowJumpClimb);
				return;
			}

			keepMinDistToTarget = true;
			//EXTrace.Print(EXTrace.AI, this, "Positioning - min dist " + minDist);
		}
		else
		{
			if (!IsRaised() && target.IsEntity() && !target.CanMeleeIfClose())
				cannotMelee = true;

			//! While weapon is raised and we have LOS or action/weapon manager is running (e.g. reload/unjam), reposition/seek cover
			if ((IsRaised() && target.m_LOS) || GetActionManager().GetRunningAction() || GetWeaponManager().IsRunning() || cannotMelee)
			{
				allowJumpClimb = false;
				//EXTrace.Print(EXTrace.AI, this, "Positioning - flank time reset " + m_eAI_FlankTime);
				m_eAI_FlankTime = 0.0;
				m_eAI_FlankTimeMax = 0.0;
			}
			else if (eAI_IsInFlankRange(dist) && !eAI_IsDangerousAltitude())
			{
				if (m_eAI_FlankTime <= 0.0)
				{
					if (Math.RandomInt(0, 2))
						m_eAI_FlankAngle = Math.RandomFloat(56.25, 67.5);
					else
						m_eAI_FlankAngle = -Math.RandomFloat(56.25, 67.5);

					m_eAI_FlankTimeMax = Math.RandomFloat(dist * 0.3, dist * 0.6);
					//EXTrace.Print(EXTrace.AI, this, "Positioning - flank angle " + m_eAI_FlankAngle + ", time max " + m_eAI_FlankTimeMax);
				}

				m_eAI_FlankTime += m_dT;

				//! Return early if we are already overriding position and not yet near endpoint and not in cover
				if (m_eAI_FlankTimeMax > 0.0 && !eAI_ShouldUpdatePosition() && !m_eAI_IsInCover)
				{
					//EXTrace.Print(EXTrace.AI, this, "Positioning - flanking, not updating pos tmax=" + m_eAI_FlankTime + " timeout=" + m_eAI_PositionOverrideTimeout + " inCover=" + m_eAI_IsInCover);
					if (m_eAI_IsInDangerByArea)
						OverrideTargetPosition(m_eAI_TargetPosition, m_eAI_TargetPositionIsFinal, 1.0, m_PathFinding.m_AllowJumpClimb);
					return;
				}

				//! Flank time is based on a random speed estimate.
				//! Sprint = 6.66 m/s avg or factor 0.3, half speed = 3.33 m/s avg or factor 0.6 (lower speed = more time)
				if (m_eAI_FlankTime >= m_eAI_FlankTimeMax)
				{
					//EXTrace.Print(EXTrace.AI, this, "Positioning - flank time exceeded " + m_eAI_FlankTime);

					if ((m_eAI_FlankTime >= m_eAI_FlankTimeMax * 2 && Math.RandomInt(0, 3000) < 1) || m_PathFinding.m_IsUnreachable)
					{
						m_eAI_FlankTime = 0.0;
						Expansion_GetUp();
						if (m_eAI_IsInDangerByArea)
							OverrideTargetPosition(m_eAI_TargetPosition, m_eAI_TargetPositionIsFinal, 1.0, m_PathFinding.m_AllowJumpClimb);
						return;
					}

					if (m_eAI_IsInCover)
					{
						if (m_eAI_CurrentCoverObject && m_eAI_CurrentCoverObject.IsRock())
							OverrideStance(DayZPlayerConstants.STANCEIDX_PRONE);
						else if (eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && m_eAI_StancePreference != DayZPlayerConstants.STANCEIDX_PRONE)
							OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);

						EntityAI hands = GetHumanInventory().GetEntityInHands();
						if (hands && hands.IsWeapon())
							RaiseWeapon(true);

						m_eAI_TargetPositionIsFinal = true;

						return;
					}
				}

				flank = true;
				isFinal = false;
				Expansion_GetUp();
				//EXTrace.Print(EXTrace.AI, this, "Positioning - flanking " + m_eAI_FlankTime);
			}
			else
			{
				//EXTrace.Print(EXTrace.AI, this, "Positioning - dist " + dist);
				m_eAI_FlankTime = 0.0;  //! Allow switch to search if flanking previously
			}
		}

		Weapon_Base weapon;
		if (Class.CastTo(weapon, GetHumanInventory().GetEntityInHands()))
		{
			if (weapon.ShootsExplosiveAmmo())
			{
				minDist = Math.Max(weapon.Expansion_GetMinSafeFiringDistance(), minDist);
				keepMinDistToTarget = true;
			}
		}

		if (!keepMinDistToTarget && !allowJumpClimb)
		{
			if (dist < 3.0 && !cannotMelee)
			{
				//! If the distance to target is already less than 3 m, don't take cover, just try to keep the distance
				minDist = 3.0;  //! Potentially backpedal
			}
			else
			{
				//! Return early if we are already overriding position and not yet near endpoint
				if (!eAI_ShouldUpdatePosition())
				{
					if (m_eAI_IsInDangerByArea)
						OverrideTargetPosition(m_eAI_TargetPosition, m_eAI_TargetPositionIsFinal, 1.0, m_PathFinding.m_AllowJumpClimb);
					return;
				}

				if (weapon)
				{
					//! Determine min distance range based on zeroing range.
					//! Final min distance will be chosen to be lower or equal to this range.

					ItemOptics optics = weapon.GetAttachedOptics();

					if (optics)
						minDist = optics.GetZeroingDistanceZoomMax();
					else
						minDist = weapon.GetZeroingDistanceZoomMax(weapon.GetCurrentMuzzle()) * 0.3;

					if (minDist > 1000)
						minDist = 1000;
				}

				//! Stay at current distance if lower than min distance range
				//! (roughly, may take cover or reposition to random point if no cover available)
				if (dist <= minDist)
					minDist = dist;
				else if (cannotMelee)
					minDist = 30.0;
			}

			//EXTrace.Print(EXTrace.AI, this, "Positioning - no jump/climb, min dist " + minDist);
		}

		Object currentCoverObj = m_eAI_CurrentCoverObject;

		if (minDist || flank)
		{
			pos = GetPosition();

			if (flank)
			{
				vector ori = dir.VectorToAngles();

				ori[0] = ExpansionMath.RelAngle(ori[0] + m_eAI_FlankAngle);

				dir = ori.AnglesToVector();
			}

			if (m_eAI_CurrentCoverObject)
			{
				s_eAI_TakenCoverObjects.RemoveItem(m_eAI_CurrentCoverObject);
				m_eAI_CurrentCoverObject = null;
			}

			if (m_eAI_PotentialCoverObjects.Count() && (!target.IsMechanicalTrap() || target.IsExplosive()))
			{
				vector aimDirectionTarget = GetAimDirectionTarget();
				vector forwardPos = GetPosition();
				
				vector toObjDir;
				vector toObjDirNorm;
				float dot;
				int coverObjKey;
				float distSq = dist * dist;
				float objDistSq;

				vector targetPos;
				float minDistSq;
				float objToTargetDistSq;

				if (keepMinDistToTarget)
				{
					targetPos = target.GetPosition(true);
					minDistSq = minDist * minDist;
				}

				TIntArray distances = {};
				map<int, Object> potentialCoverObjs = new map<int, Object>;

				foreach (Object obj: m_eAI_PotentialCoverObjects)
				{
					if (!obj)
						continue;

					//! Don't try to find cover behind objects with active physics
					if (dBodyIsActive(obj))
						continue;

					if (flank)
					{
						if (obj == currentCoverObj)
							continue;
					}

					if (s_eAI_TakenCoverObjects.Find(obj) > -1)
						continue;

					toObjDir = vector.Direction(forwardPos, obj.GetPosition());
					toObjDirNorm = toObjDir.Normalized();

					objDistSq = toObjDir.LengthSq();

					if (flank)
					{
						dot = vector.Dot(dir, toObjDirNorm);

						//! Select object if in direction of flank angle
						if (dot < 0.75)
							continue;
					}
					else
					{
						dot = vector.Dot(aimDirectionTarget, toObjDirNorm);

						//! Select object if closer than target or if opposite of AI aim direction target
						if ((keepMinDistToTarget || distSq <= objDistSq) && dot >= 0.0)
							continue;
					}

					if (keepMinDistToTarget)
						objToTargetDistSq = vector.DistanceSq(obj.GetPosition(), targetPos);

					if (!keepMinDistToTarget || objToTargetDistSq > minDistSq)
					{
						coverObjKey = Math.Round(objDistSq);
						distances.Insert(coverObjKey);
						potentialCoverObjs[coverObjKey] = obj;
					}
				}

				if (distances.Count())
				{
					//! Select closest potential cover
					distances.Sort();

					coverObjKey = distances[0];

					m_eAI_CurrentCoverObject = potentialCoverObjs[coverObjKey];
					//m_eAI_PotentialCoverObjects.Clear();
					//m_eAI_PotentialCoverObjects.Insert(m_eAI_CurrentCoverObject);
					s_eAI_TakenCoverObjects.Insert(m_eAI_CurrentCoverObject);

				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, this, "New cover object selected: " + m_eAI_CurrentCoverObject + " " + m_eAI_CurrentCoverObject.GetPosition());
				#endif

					BuildingBase building;
					bool houseWithDoors;
					float extension;
					vector minMax[2];
					if (m_eAI_CurrentCoverObject.IsTree())
					{
						extension = 2.0;
					}
					else if (Class.CastTo(building, m_eAI_CurrentCoverObject) && building.Expansion_IsEnterable())
					{
						houseWithDoors = true;
					}
					else if (m_eAI_CurrentCoverObject.GetCollisionBox(minMax))
					{
						minMax[0][1] = 0.0;
						minMax[1][1] = 0.0;
						extension = vector.Distance(minMax[0], minMax[1]) * 0.5 + 1.0;
					}
					else
					{
						extension = m_eAI_CurrentCoverObject.ClippingInfo(minMax);
					}

					vector objPos = m_eAI_CurrentCoverObject.GetPosition();

					if (houseWithDoors)
					{
						pos = objPos;
					}
					else
					{
						objPos[1] = m_eAI_AimPosition_WorldSpace[1];
						vector objToTargetDir = vector.Direction(objPos, m_eAI_AimPosition_WorldSpace);
						vector objToTargetDirNorm = objToTargetDir.Normalized();
						pos = objPos - objToTargetDirNorm * extension;
					}

					if (flank)
					{
						m_eAI_FlankTimeMax = Math.RandomFloat(dist * 0.3, dist * 0.6);
						m_eAI_PositionOverrideTimeout = m_eAI_FlankTimeMax;
					}
					else
					{
						m_eAI_PositionOverrideTimeout = 3.0;
					}
				}
			}

			if (!m_eAI_CurrentCoverObject)
			{
				//EXTrace.Print(EXTrace.AI, this, "Positioning - no cover obj");

				if (dist != minDist)
				{
					//EXTrace.Print(EXTrace.AI, this, "Positioning - dist != min dist, AI pos=" + pos);

					pos = pos + dir.Normalized() * (dist - minDist * Math.RandomFloat(1.0, 2.0));
				}

				if (flank)
				{
					m_eAI_FlankTimeMax = Math.RandomFloat(dist * 0.3, dist * 0.6);
					m_eAI_PositionOverrideTimeout = m_eAI_FlankTimeMax;
				}
				else
				{
					if (keepMinDistToTarget)
					{
						m_eAI_PositionOverrideTimeout = 3.0;
					}
					else
					{
						if (minDist > 3.0)
							pos = ExpansionMath.GetRandomPointInCircle(pos, minDist);

						m_eAI_PositionOverrideTimeout = Math.RandomFloat(5.0, 10.0);
					}
				}
			}
		}
		else
		{
			pos = target.GetPosition();
		}

		//EXTrace.Print(EXTrace.AI, this, "Positioning - pos " + pos + " dist " + dist + " minDist " + minDist + " cover obj " + m_eAI_CurrentCoverObject);
		OverrideTargetPosition(pos, isFinal, 1.0, allowJumpClimb);

		if ((minDist || flank) && m_eAI_CurrentCoverObject && m_eAI_CurrentCoverObject != currentCoverObj)
		{
			m_PathFinding.OnUpdate(m_dT, -1.0);
			m_eAI_CurrentCoverPosition = m_PathFinding.GetEnd();
		#ifdef DIAG_DEVELOPER
			//EXTrace.Print(EXTrace.AI, this, "Positioning - cover obj " + m_eAI_CurrentCoverObject + " pos " + m_eAI_CurrentCoverObject.GetPosition() + " cover pos " + m_eAI_CurrentCoverPosition + " dist to cover pos " + vector.Distance(m_eAI_CurrentCoverObject.GetPosition(), m_eAI_CurrentCoverPosition));
			Expansion_DebugObject_Deferred(20, m_eAI_CurrentCoverPosition, "ExpansionDebugNoticeMe", GetDirection());
		}
		else if (!m_eAI_CurrentCoverObject)
		{
			Expansion_DebugObject_Deferred(20, "0 0 0", "ExpansionDebugNoticeMe");
		#endif
		}
	}

	bool eAI_ShouldUpdatePosition()
	{
		if (m_eAI_PositionOverrideTimeout > 0 && (!Math.IsPointInCircle(GetPosition(), 0.55, m_PathFinding.GetEnd()) || m_eAI_IsInCover))
			return false;

		return true;
	}

	bool eAI_IsInFlankRange(eAITarget target)
	{
		return eAI_IsInFlankRange(target.GetDistance());
	}

	bool eAI_IsInFlankRange(float dist)
	{
		if (dist > 5.0 && dist <= m_eAI_MaxFlankingDistance)
			return true;

		return false;
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
		if (IsClimbing() || IsFalling() || IsFighting() || IsSwimming() || IsUnconscious())
			return false;

		auto move = GetCommand_Move();

		if (!move && force)
			move = StartCommand_MoveAI();

		if (move)
		{
			//int stanceIdx = m_eAI_CommandMove.GetStance();
			int stanceIdx = eAI_GetStance();
			int pStanceIdxOrig = pStanceIdx;

			if (pStanceIdx < m_eAI_DefaultStance)
			{
				if (g_Game.GetTickTime() - m_eAI_LastHitTime > 10.0 && !m_FSM.IsInState("Melee") && GetCurrentWaterLevel() < 0.3)
					pStanceIdx = m_eAI_DefaultStance;
			}

			if (m_eAI_CommandMove.OverrideStance(pStanceIdx, force))
			{
				if (pStanceIdxOrig == DayZPlayerConstants.STANCEIDX_ERECT)
					m_eAI_ShouldGetUp = true;
				else
					m_eAI_ShouldGetUp = false;

				if (EXTrace.AI)
					EXTrace.Print(true, this, "OverrideStance " + typename.EnumToString(eAIStance, stanceIdx) + " (" + stanceIdx + ") -> " + typename.EnumToString(eAIStance, pStanceIdx) + " (" + pStanceIdx + ") " + force);

				return true;
			}
		}

		return false;
	}
	
	bool eAI_IsChangingStance()
	{
		auto cmd = GetCommand_Move();
		return cmd && m_eAI_CommandMove.IsChangingStance();
	}

	bool eAI_ShouldGetUp()
	{
		return m_eAI_ShouldGetUp;
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
		if (distThreshSq > 0.0 && distToTargetSq > distThreshSq && g_Game.GetTickTime() - m_eAI_LastHitTime > 10.0)
		{
			vector targetDirection = vector.Direction(GetPosition(), m_eAI_AimPosition_WorldSpace).Normalized();
			ItemOptics optics;
			if (vector.Dot(GetDirection(), targetDirection) > 0.9 && Class.CastTo(optics, gun.GetAttachedOptics()) && optics.GetZeroingDistanceZoomMax() >= 800)
				return OverrideStance(DayZPlayerConstants.STANCEIDX_PRONE);
		}

		if (m_eAI_IsInCover)
			return false;

		return OverrideStance(DayZPlayerConstants.STANCEIDX_ERECT);
	}

	bool eAI_AdjustStance(int lastFireTime, int timeSinceLastFire, int timeBetweenFiringAndGettingUp)
	{
		bool getUp;

		if (lastFireTime > 0 && timeSinceLastFire > timeBetweenFiringAndGettingUp && !m_eAI_IsInCover)
		{
			getUp = Expansion_GetUp();
			if (getUp && EXTrace.AI)
				EXTrace.Print(true, this, "eAI_AdjustStance " + timeSinceLastFire + " > " + timeBetweenFiringAndGettingUp);
		}

		return getUp;
	}

	/**
	 * @brief Return stance in range DayZPlayerConstants.STANCEIDX_RAISED..STANCEIDX_PRONE
	 * 
	 * @note if raised, STANCEIDX_RAISED is subtracted, so the return value is guaranteed to be in aforementioned range
	 */
	int eAI_GetStance()
	{
		int stance = m_MovementState.m_iStanceIdx;

		if (stance >= DayZPlayerConstants.STANCEIDX_RAISED)
			stance -= DayZPlayerConstants.STANCEIDX_RAISED;

		return stance;
	}

	void SetMovementSpeedLimit(int pSpeed, bool roaming = false)
	{
		m_MovementSpeedLimit = eAI_GetMovementSpeed(pSpeed);

		if (roaming)
			m_eAI_RoamingMovementSpeedLimit = m_MovementSpeedLimit;
	}

	void SetMovementSpeedLimits(int pSpeed, int pSpeedUnderThreat = -1)
	{
		eAI_SetSpeedLimitPreference(pSpeed);
		if (pSpeedUnderThreat == -1)
			pSpeedUnderThreat = pSpeed;
		m_MovementSpeedLimitUnderThreat = eAI_GetMovementSpeed(pSpeedUnderThreat);
	}

	/**
	 * @brief set best speed limit based on member state (broken legs etc)
	 */
	void eAI_SetBestSpeedLimit(int speedLimit)
	{
		if (speedLimit <= 1)
			return;

		eAIGroup group = GetGroup();
		eAIBase ai;

		if (group.GetWaypointBehaviour() == eAIWaypointBehavior.ROAMING && speedLimit < m_eAI_RoamingMovementSpeedLimit)
			speedLimit = m_eAI_RoamingMovementSpeedLimit;

		for (int i = 0; i < group.Count(); i++)
		{
			if (Class.CastTo(ai, group.GetMember(i)) && ai != this)
			{
				if (ai.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS || (!IsSwimming() && ai.IsSwimming()))
				{
					if (m_MovementSpeedLimit > 1)
						SetMovementSpeedLimit(1);

					return;
				}

				//! If any other AI in group is more than 30 m from leader, slow down enough for them to be able to catch up
				if (speedLimit > 2 && vector.DistanceSq(GetPosition(), ai.GetPosition()) > 900)
				{
					if (m_MovementSpeedLimit > 2)
						SetMovementSpeedLimit(2);

					return;
				}
			}
		}

		if (m_MovementSpeedLimit < speedLimit)
			SetMovementSpeedLimit(speedLimit);
	}

	void eAI_SetSpeedLimitPreference(int speedLimit)
	{
		SetMovementSpeedLimit(speedLimit);
		m_eAI_SpeedLimitPreference = m_MovementSpeedLimit;
	}

	int GetMovementSpeedLimit()
	{
		return m_MovementSpeedLimit;
	}

	int GetMovementSpeedLimitUnderThreat()
	{
		return m_MovementSpeedLimitUnderThreat;
	}

	float eAI_GetCurrentMovementSpeedLimit()
	{
		auto cmd = GetCommand_Move();
		if (cmd)
			return m_eAI_CommandMove.GetSpeedLimit();

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
		Class.CastTo(m_DebugTargetApple, g_Game.CreateObject("Apple", GetPosition(), true));
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
			flags = FindInventoryLocationType.ATTACHMENT | FindInventoryLocationType.CARGO;

		if (!GetInventory().FindFirstFreeLocationForNewEntity(src.GetType(), flags, location))
			return null;

		return Expansion_CloneItemToLocation(src, location);
	}

	EntityAI Expansion_CloneItemToLocation(EntityAI src, InventoryLocation location)
	{
		if (!src || src.IsSetForDeletion())
			return null;

		InventoryLocation il_src = new InventoryLocation();

		src.GetInventory().GetCurrentInventoryLocation(il_src);

		if (il_src.GetType() == InventoryLocationType.HANDS)
		{
			if (GetWeaponManager().IsRunning())
				GetWeaponManager().OnWeaponActionEnd();  //! Prevent getting stuck in running state
		}

		if (location.GetType() == InventoryLocationType.HANDS)
		{
			//! Forcing switch to HumanCommandMove before taking to hands,
			//! and hiding/showing item in hands after, unbreaks hand anim state
			if (!GetCommand_Move())
				StartCommand_Move();
			else
				m_eAI_CommandTime = 0.0;
		}

		//! Needs to be 'lootable' while in AI inventory, else AI won't be able to properly interact with item
		//! (e.g. won't be able to detach/attach mag to firearm)
		EntityAI dst = ExpansionItemSpawnHelper.Clone(src, true, location, false);
		if (dst)
		{
			ItemBase item;
			ItemBase dstItem;
			if (Class.CastTo(dstItem, dst) && Class.CastTo(item, src))
				dstItem.m_Expansion_IsOwnerPlayer = item.m_Expansion_IsOwnerPlayer;

		#ifdef JM_COT
			set<PlayerBase> spectators;
			if (src.GetHierarchyRootPlayer() != this && dstItem)
			{
				eAIGroup group = GetGroup();
				eAIBase ai;
				for (int i = 0; i < group.Count(); i++)
				{
					if (Class.CastTo(ai, group.GetMember(i)) && ai.m_eAI_Spectators.Count() > 0)
					{
						spectators = ai.m_eAI_Spectators;
						break;
					}
				}
			}
		#endif

			g_Game.ObjectDelete(src);

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

		#ifdef JM_COT
			if (spectators)
			{
				string itemName = dstItem.Expansion_GetDisplayNameRaw();

				foreach (PlayerBase spectator: spectators)
				{
					if (spectator)
						ExpansionNotification(GetCachedName(), itemName, "Open Hand", ARGB(255, 248, 228, 51), 1.0, ExpansionNotificationType.ACTIVITY).Create(spectator.GetIdentity());
				}
			}
		#endif
		}

		return dst;
	}	
	
	override string GetCachedName()
	{
		m_CachedPlayerName = GetDisplayName();

		if (m_CachedPlayerName == s_Expansion_SurvivorDisplayName)
			m_CachedPlayerName = Expansion_GetSurvivorName();

		return m_CachedPlayerName;
	}

	//! @note INTERNAL USE ONLY
	void eAI_OnInventoryEnter(ItemBase item)
	{
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(eAI_AddItem, item);  //! Add in next tick to make sure we're not instantly adding items created by ObjectCreate(Ex) and similar methods where item setup (quantity etc) is performed in script after creation
	}

	//! @note INTERNAL USE ONLY
	void eAI_OnInventoryExit(ItemBase item)
	{
		eAI_RemoveItem(item);
	}

	//! @note INTERNAL USE ONLY
	void eAI_OnItemDestroyed(ItemBase item)
	{
		EXTrace.Print(EXTrace.AI, this, "eAI_OnItemDestroyed " + item);
		if (!item.GetInventory().GetCargo())
		{
			eAI_RemoveItem(item);
			m_eAI_ItemsToDrop.Insert(item);
		}
	}

	void eAI_OnCargoEnter(EntityAI parent, ItemBase item)
	{
	}

	void eAI_OnCargoExit(EntityAI parent, ItemBase item)
	{
		if (!parent.GetInventory().IsInCargo())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_OnCargoExit " + parent + " " + item);
			m_eAI_AccessibleCargoSpaceTaken -= item.Expansion_GetItemSize();
			m_eAI_MaxFoodCount = Math.Ceil((m_eAI_AccessibleCargoSpaceTotal - m_eAI_AccessibleCargoSpaceTaken) * 0.05);
		}
	}

#ifdef DIAG_DEVELOPER
	bool m_eAI_AccessibleCargoSpaceDbg;
#endif

	//! @note INTERNAL USE ONLY
	private void eAI_AddItem(ItemBase item)
	{
		//! @note since call to eAI_AddItem is deferred, we need to do these checks here
		if (!item || item.GetHierarchyRootPlayer() != this || item.IsDamageDestroyed())
			return;

		if (item.GetInventory().IsInCargo() && item.GetHierarchyParent().IsClothing() && !item.GetHierarchyParent().GetInventory().IsInCargo())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - cargo " + item);
			m_eAI_AccessibleCargoSpaceTaken += item.Expansion_GetItemSize();

		#ifdef DIAG_DEVELOPER
			if (m_eAI_AccessibleCargoSpaceTaken > m_eAI_AccessibleCargoSpaceTotal)
			{
				vector itemSize = item.ConfigGetVector("itemSize");
				string warning = string.Format("m_eAI_AccessibleCargoSpaceTaken(%1) > m_eAI_AccessibleCargoSpaceTotal(%2) | item %3 %4x%5 | parent %6 | initialized %7", m_eAI_AccessibleCargoSpaceTaken, m_eAI_AccessibleCargoSpaceTotal, item, itemSize[0], itemSize[1], item.GetHierarchyParent(), IsInitialized().ToString());
				EXError.Warn(this, warning, {});
				m_eAI_AccessibleCargoSpaceDbg = true;
			}
		#endif

			m_eAI_MaxFoodCount = Math.Ceil((m_eAI_AccessibleCargoSpaceTotal - m_eAI_AccessibleCargoSpaceTaken) * 0.05);

			if (IsInitialized() && m_eAI_MaxFoodCount >= 0)
			{
				while (m_eAI_Food.Count() > m_eAI_MaxFoodCount)
				{
					m_eAI_ItemsToDrop.Insert(m_eAI_Food[0]);
					m_eAI_Food.Remove(0);
				}
			}
		}

		Weapon_Base weapon;
		if (Class.CastTo(weapon, item))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - weapon " + item);
			if (item.ShootsExplosiveAmmo())
			{
				m_eAI_Launchers.Insert(weapon);
			}
			else
			{
				if (item.IsKindOf("Pistol_Base"))
					m_eAI_Handguns.Insert(weapon);
				else
					m_eAI_Firearms.Insert(weapon);
			}
			return;
		}

		//! When item is cloned to a shoulder slot or similar, GetInventory().IsAttachment() is true, but GetHierarchyRootPlayer() is NULL
		//! GetHierarchyRoot() is the item itself in that case
		if (item.Expansion_IsMeleeWeapon() && (!item.GetInventory().IsAttachment() || !item.GetHierarchyRootPlayer() || item.GetHierarchyRootPlayer() == this))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - melee weapon " + item);
			m_eAI_MeleeWeapons.Insert(item);
			return;
		}

		if (item.Expansion_CanBeUsedToBandage())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - bandage/rag " + item);
			m_eAI_Bandages.Insert(item);
			return;
		}

		if (item.IsInherited(WeaponCleaningKit))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - repair kit " + item);
			m_eAI_RepairKits.Insert(item);
			return;
		}

		Edible_Base food;
		if (Class.CastTo(food, item) && !item.IsCorpse() && item.IsFood())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - food " + item);
			if (food.GetFoodStage() && food.GetFoodStageType() == FoodStageType.ROTTEN)
				m_eAI_ItemsToDrop.Insert(item);
			else
				m_eAI_Food.Insert(item);
			return;
		}

		if (item.IsClothing() && !item.GetInventory().IsInCargo())
		{
			int w, h;
			CargoBase cargo = item.GetInventory().GetCargo();
			if (cargo)
			{
				w = cargo.GetWidth();
				h = cargo.GetHeight();

			#ifdef DIAG_DEVELOPER
				if (m_eAI_AccessibleCargoSpaceTaken > m_eAI_AccessibleCargoSpaceTotal || m_eAI_AccessibleCargoSpaceDbg)
				{
					string cmp;

					if (m_eAI_AccessibleCargoSpaceTaken > m_eAI_AccessibleCargoSpaceTotal + w * h)
					{
						cmp = ">";
					}
					else
					{
						cmp = "<=";
						m_eAI_AccessibleCargoSpaceDbg = false;
					}

					EXError.Warn(this, string.Format("m_eAI_AccessibleCargoSpaceTaken(%1) %2 m_eAI_AccessibleCargoSpaceTotal(%3) | clothing %4 cargo size %5x%6 | initialized %7", m_eAI_AccessibleCargoSpaceTaken, cmp, m_eAI_AccessibleCargoSpaceTotal + w * h, item, w, h, IsInitialized().ToString()), {});
				}
			#endif

				m_eAI_AccessibleCargoSpaceTotal += w * h;

				m_eAI_MaxFoodCount = Math.Ceil((m_eAI_AccessibleCargoSpaceTotal - m_eAI_AccessibleCargoSpaceTaken) * 0.05);
			}
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - clothing " + item + " " + w + "x" + h);
			return;
		}

		//! Ammo/magazines
		Magazine mag;
		if (Class.CastTo(mag, item) && !item.GetInventory().IsAttachment())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_AddItem - mag " + item);
			eAI_AddMag(mag, true);
		}
	}

	//! @note INTERNAL USE ONLY
	void eAI_OnMagAttached(EntityAI parent, int slot_id, Magazine mag)
	{
		//! If mag is attached to weapon, remove from available mags
		if (parent.IsWeapon())
			eAI_RemoveMag(mag);
	}

	//! @note INTERNAL USE ONLY
	void eAI_OnMagDetached(EntityAI parent, int slot_id, Magazine mag)
	{
		//! If mag is detached from weapon and still on player, add to available mags
		if (parent.IsWeapon() && mag.GetHierarchyRootPlayer() == this && !mag.IsDamageDestroyed())
			eAI_AddMag(mag, false, false);
	}

	//! @note INTERNAL USE ONLY
	private void eAI_AddMag(Magazine mag, bool fillAnyCompatibleMag = false, bool forceEvaluateFirearmTypes = true)
	{
		EXTrace.Print(EXTrace.AI, this, "eAI_AddMag - " + mag + " " + mag.GetType() + " ammo count " + mag.GetAmmoCount());

		if (mag.GetAmmoCount() == 0 && eAI_GetMagazineTypeCount(mag.GetType()) > 1)
		{
			EXTrace.Print(EXTrace.AI, this, "Deleting empty duplicate " +  mag);
			g_Game.ObjectDelete(mag);
			return;
		}

		//! If current weapon doesn't have internal mag, fill any compatible mag in inventory from mag entering inv
		//! (and delete afterwards if empty)
		Weapon_Base weapon;
		if (fillAnyCompatibleMag && Class.CastTo(weapon, GetHumanInventory().GetEntityInHands()) && !weapon.HasInternalMagazine(weapon.GetCurrentMuzzle()))
			eAI_FillAnyCompatibleMag(weapon, mag, true);

		if (mag && !mag.IsSetForDeletion())  //! eAI_FillMag may delete mag if empty
			m_eAI_Magazines.Insert(mag);

		//! Force re-evaluation of any gun/mag (loot) targets and guns/mags in inventory
		eAI_EvaluateFirearmTypes(forceEvaluateFirearmTypes);
	}

	//! @note INTERNAL USE ONLY
	private void eAI_RemoveItem(ItemBase item)
	{
		eAI_RemoveActiveVisibilityEnhancer(item);

		if (item.GetInventory().IsInCargo() && item.GetHierarchyParent().IsClothing() && !item.GetHierarchyParent().GetInventory().IsInCargo())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - cargo " + item);
			m_eAI_AccessibleCargoSpaceTaken -= item.Expansion_GetItemSize();
			m_eAI_MaxFoodCount = Math.Ceil((m_eAI_AccessibleCargoSpaceTotal - m_eAI_AccessibleCargoSpaceTaken) * 0.05);
		}

		m_eAI_ItemsToDrop.RemoveItemUnOrdered(item);

		Weapon_Base weapon;
		if (Class.CastTo(weapon, item))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - gun " + item);
			if (item.ShootsExplosiveAmmo())
			{
				m_eAI_Launchers.RemoveItem(weapon);
			}
			else
			{
				if (item.IsKindOf("Pistol_Base"))
					m_eAI_Handguns.RemoveItem(weapon);
				else
					m_eAI_Firearms.RemoveItem(weapon);
			}
			m_eAI_EvaluatedFirearmTypes.Remove(item.Type());
			return;
		}

		if (item.Expansion_IsMeleeWeapon())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - melee weapon " + item);
			m_eAI_MeleeWeapons.RemoveItem(item);
			return;
		}

		if (item.Expansion_CanBeUsedToBandage())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - bandage/rag " + item);
			m_eAI_Bandages.RemoveItem(item);
			if (item == m_eAI_BandageToUse)
				m_eAI_BandageToUse = null;
			return;
		}

		if (item.IsInherited(WeaponCleaningKit))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - repair kit " + item);
			m_eAI_RepairKits.RemoveItem(item);
			return;
		}

		Edible_Base food;
		if (Class.CastTo(food, item) && !item.IsCorpse() && item.IsFood())
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - food " + item);
			if (!food.GetFoodStage() || food.GetFoodStageType() != FoodStageType.ROTTEN)
				m_eAI_Food.RemoveItemUnOrdered(item);
			return;
		}

		if (item.IsClothing() && !item.GetInventory().IsInCargo())
		{
			int w, h;
			CargoBase cargo = item.GetInventory().GetCargo();
			if (cargo)
			{
				w = cargo.GetWidth();
				h = cargo.GetHeight();
				m_eAI_AccessibleCargoSpaceTotal -= w * h;
			}
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - clothing " + item + " " + w + "x" + h);
			return;
		}

		//! Ammo/magazines
		Magazine mag;
		if (Class.CastTo(mag, item))
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_RemoveItem - mag " + item);
			eAI_RemoveMag(mag, true);
		}
	}

	//! @note INTERNAL USE ONLY
	private void eAI_RemoveMag(Magazine mag, bool checkAttached = false)
	{
		int removeIndex = m_eAI_Magazines.Find(mag);
		if (removeIndex >= 0)
		{
			m_eAI_Magazines.RemoveOrdered(removeIndex);
			//! Warn if removed mag is attached to weapon
			if (checkAttached)
			{
				InventoryLocation lcn = new InventoryLocation();
				mag.GetInventory().GetCurrentInventoryLocation(lcn);
				if (lcn.GetType() == InventoryLocationType.ATTACHMENT)
				{
					EntityAI parent = lcn.GetParent();
					if (parent && parent.IsWeapon())
						EXError.Warn(this, "Removed mag is attached to weapon but was tracked as available for reload " + ExpansionStatic.GetDebugInfo(mag) + ", parent " + ExpansionStatic.GetDebugInfo(parent));
				}
			}
		}

		EXTrace.Print(EXTrace.AI, this, "eAI_RemoveMag - " + mag + " index " + removeIndex + " remaining " + m_eAI_Magazines.Count());

		for (int i = m_eAI_Magazines.Count() - 1; i >= 0; i--)
		{
			mag = m_eAI_Magazines[i];
			if (!mag)
			{
				EXError.Warn(this, "NULL entry found in magazines tracked for reload at index " + i);
				m_eAI_Magazines.Remove(i);
			}
		}

		//! Force re-evaluation of any gun (loot) targets/guns in inventory
		eAI_EvaluateFirearmTypes();
	}

	void eAI_OnFoodExpired(ItemBase food)
	{
		int index = m_eAI_Food.Find(food);

		if (index >= 0)
		{
			m_eAI_Food.Remove(index);
			m_eAI_ItemsToDrop.Insert(food);
		}
	}

	float eAI_GetCurrentDPS(Weapon_Base weapon)
	{
		int muzzleIndex = weapon.GetCurrentMuzzle();

		if (eAI_HasMagazineForWeapon(weapon, muzzleIndex))
			return weapon.Expansion_GetDPS();

		return weapon.Expansion_GetAvgDmgPerShot();
	}

	bool eAI_HasMagazineForWeapon(Weapon_Base weapon, int muzzleIndex)
	{
		if (weapon.HasInternalMagazine(muzzleIndex) || weapon.GetMagazine(muzzleIndex))
			return true;

		typename type = weapon.Type();
		Magazine evaluated = m_eAI_EvaluatedFirearmTypes[type];
		if (evaluated && !evaluated.IsAmmoPile())
			return true;

		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.IsAmmoPile())
				continue;

			if (weapon.TestAttachMagazine(muzzleIndex, mag, false, true))
			{
				m_eAI_EvaluatedFirearmTypes[type] = mag;
				return true;
			}
		}

		return false;
	}

	bool eAI_HasMagazineType(string type)
	{
		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.GetType() == type)
				return true;
		}

		return false;
	}

	bool eAI_HasMagazineTypeWithAmmo(string type)
	{
		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.GetType() == type && mag.GetAmmoCount() > 0)
				return true;
		}

		return false;
	}

	bool eAI_HasWeaponForMagazine(Magazine mag)
	{
		if (eAI_HasWeaponForMagazine(m_eAI_Firearms, mag))
			return true;

		if (eAI_HasWeaponForMagazine(m_eAI_Handguns, mag))
			return true;

		if (eAI_HasWeaponForMagazine(m_eAI_Launchers, mag))
			return true;

		return false;
	}

	bool eAI_HasWeaponForMagazine(array<Weapon_Base> weapons, Magazine mag)
	{
		foreach (Weapon_Base weapon: weapons)
		{
			if (weapon.Expansion_IsCompatibleMag(weapon.GetCurrentMuzzle(), mag))
				return true;
		}

		return false;
	}

	int eAI_GetMagazineTypeAmmoCount(string type)
	{
		int ammoCount;

		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.GetType() == type)
				ammoCount += mag.GetAmmoCount();
		}

		return ammoCount;
	}

	int eAI_GetMagazineTypeCount(string type)
	{
		int count;

		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.GetType() == type)
				count++;
		}

		return count;
	}

	int eAI_GetMagazineTypeWithAmmoCount(string type)
	{
		int count;

		foreach (Magazine mag: m_eAI_Magazines)
		{
			if (!mag)
				continue;

			if (mag.GetType() == type && mag.GetAmmoCount() > 0)
				count++;
		}

		return count;
	}

	bool eAI_HasWeaponInInventory(bool requireAmmo = false)
	{
		if (m_eAI_Firearms.Count() == 0 && m_eAI_Handguns.Count() == 0 && m_eAI_Launchers.Count() == 0)
			return false;

		if (requireAmmo && !eAI_GetAnyWeaponToUse(true))
			return false;

		return true;
	}

	bool eAI_HasFirearm(bool requireAmmo = false)
	{
		if (m_eAI_Launchers.Count() == 0)
			return false;

		if (requireAmmo && !eAI_GetWeaponToUse(true, false))
			return false;

		return true;
	}

	bool eAI_HasLauncher(bool requireAmmo = false)
	{
		if (m_eAI_Launchers.Count() == 0)
			return false;

		if (requireAmmo && !eAI_GetWeaponToUse(true, true))
			return false;

		return true;
	}

	/**
	 * @brief check if we have ammo for firearm
	 * 
	 * @param gun
	 * @param [out] mag            Magazine that contains ammo (may be attached to this gun) for guns that don't have internal mag,
	 *                             or ammo pile
	 * @param checkMagsInInventory If true, check mags/ammo in inventory even if gun is chambered or internal/attached mag is not empty
	 *                             If false, only check mags/ammo in inventory if gun is not chambered and internal/attached mag is empty
	 * 
	 * @return                     true if we have ammo
	 */
	bool eAI_HasAmmoForFirearm(Weapon_Base gun, out Magazine mag, bool checkMagsInInventory = true)
	{
		if (!checkMagsInInventory && gun.Expansion_HasAmmo(mag))
			return true;

		typename type = gun.Type();

		bool found = m_eAI_EvaluatedFirearmTypes.Find(type, mag);

		bool unlimitedReload;

		if (found && mag)
		{
			int ammoCount = mag.GetAmmoCount();

			unlimitedReload = eAI_IsTargetUnlimitedReload();

			if (ammoCount > 0 || unlimitedReload)
				return true;
		}

		//! Mag not found or has no ammo and no unlimited reload
		if (!found || mag)
		{
			if (!found)
				unlimitedReload = eAI_IsTargetUnlimitedReload();

			//! eAI_GetMagazineToReload will only return non-empty mags/ammo (unless unlimited reload) or null. EXPENSIVE, use with care.
			mag = eAI_GetMagazineToReload(gun, unlimitedReload);

			EXTrace.Print(EXTrace.AI, this, "eAI_HasAmmoForFirearm - inserting " + type + " " + mag);
			m_eAI_EvaluatedFirearmTypes[type] = mag;

			return mag != null;
		}

		return false;
	}

	void eAI_EvaluateFirearmTypes(bool force = false)
	{
		TTypenameArray toRemove();

		foreach (typename type, Magazine mag: m_eAI_EvaluatedFirearmTypes)
		{
			if (force || !mag || mag.GetHierarchyRootPlayer() != this || mag.GetHierarchyParent().IsWeapon())
				toRemove.Insert(type);
		}

		foreach (typename removeType: toRemove)
		{
			EXTrace.Print(EXTrace.AI, this, "eAI_EvaluateFirearmTypes - removing " + removeType + " " + m_eAI_EvaluatedFirearmTypes[removeType]);
			m_eAI_EvaluatedFirearmTypes.Remove(removeType);

			if (eAI_HasAmmoForFirearmType(m_eAI_Firearms, removeType))
				continue;
			if (eAI_HasAmmoForFirearmType(m_eAI_Handguns, removeType))
				continue;
			if (eAI_HasAmmoForFirearmType(m_eAI_Launchers, removeType))
				continue;
		}
	}

	bool eAI_HasAmmoForFirearmType(array<Weapon_Base> weapons, typename type)
	{
		Magazine mag;
		foreach (Weapon_Base weapon: weapons)
		{
			if (weapon.Type() == type)
			{
				if (eAI_HasAmmoForFirearm(weapon, mag, true))
					return true;

				//! When eAI_HasAmmoForFirearm returns false, it has inserted a NULL mag entry for the weapon type,
				//! which we need to remove again so the next call to eAI_HasAmmoForFirearm with the same type
				//! checks mags in inventory
				m_eAI_EvaluatedFirearmTypes.Remove(type);
				break;
			}
		}

		return false;
	}

	bool eAI_ShouldPreferExplosiveAmmo()
	{
		eAITarget target = GetTarget();

		if (target)
		{
			if (target.IsPlayer() && target.GetParent() && target.GetCachedThreat(true) > 0.2)
				return true;
		}

		return false;
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
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(06) -> eAI_HandleAiming");
	#endif

		auto vehCmd = GetCommand_Vehicle();
		if (vehCmd && (vehCmd.IsGettingIn() || vehCmd.IsGettingOut()))
			return false;

		eAITarget target = m_eAI_Targets[0];

		bool isServer = g_Game.IsServer();
		bool lookAim;
		bool lookDirectionRecalculate;
		bool aimDirectionRecalculate;

		//! Aim from, not to! Has to be "neck" because that's how player model holds gun, everything else will break aiming
		vector neck = GetBonePositionWS(GetBoneIndexByName("neck"));

		if (target)
		{
			//! If we have LOS, or active threat level is equal to or above 0.4 (meaning we had LOS before), or target is noise,
			//! look/aim in (last known) target direction
			if (hasLOS || (m_eAI_CurrentThreatToSelfActive >= 0.4 && !target.IsItem()) || target.IsNoise())
				lookAim = true;

			//! If we don't have LOS, are in cover and in a building with doors, then look in direction of previous waypoint,
			//! which is guaranteed to be the direction of a door
			BuildingBase building;
			bool houseWithDoors;
			vector prevWaypoint;
			if (!hasLOS && m_eAI_IsInCover && Class.CastTo(building, m_eAI_CurrentCoverObject) && building.Expansion_IsEnterable())
			{
				prevWaypoint = m_PathFinding.m_PrevPoint;
				if (ExpansionMath.Distance2DSq(GetPosition(), prevWaypoint) > 0.0001)
				{
					houseWithDoors = true;
					lookAim = true;
				}
			}

			vector aimPosition;

			if (houseWithDoors)
				aimPosition = prevWaypoint + (neck - GetPosition());
			else
				aimPosition = target.GetPosition(!isServer) + target.GetAimOffset();

			if ((!isServer || (IsRaised() && m_eAI_CurrentThreatToSelfActive > 0.152) || (!IsRaised() && m_eAI_CurrentThreatToSelfActive > 0.15) || houseWithDoors) && lookAim)
				aimDirectionRecalculate = true;

			if (isServer)
			{
				if (aimDirectionRecalculate || (!IsRaised() && (m_eAI_CurrentThreatToSelfActive > 0.1 || houseWithDoors) && lookAim))
					lookDirectionRecalculate = true;

				LookAtPosition(aimPosition, lookDirectionRecalculate);
			}

			AimAtPosition(aimPosition, aimDirectionRecalculate);
		}

		if (!target || (!lookDirectionRecalculate && !aimDirectionRecalculate))
		{
			if (!isServer || !eAI_UpdateLookDirectionPreference())
			{
				if (isServer && m_eAI_LookDirection_Recalculate)
					LookAtDirection("0 0 1");

				if (m_eAI_AimDirection_Recalculate)
					AimAtDirection("0 0 1");
			}
		}

		if (isServer && m_eAI_LookDirection_Recalculate)
		{
			m_eAI_LookDirectionTarget_ModelSpace = vector.Direction(neck, m_eAI_LookPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

		if (m_eAI_AimDirection_Recalculate)
		{
			m_eAI_AimDirectionTarget_ModelSpace = vector.Direction(neck, m_eAI_AimPosition_WorldSpace).Normalized().InvMultiply3(m_ExTransformPlayer);
		}

	#ifdef DIAG_DEVELOPER
	#ifdef SERVER
		vector dir = GetLookDirection();
		dir[1] = 0;
		vector fwd = neck + dir;
		Expansion_DebugObject(-100, fwd, "ExpansionDebugSphereSmall_Blue", vector.Zero, neck);
		dir = Vector(m_eAI_LookRelAngles[0] - m_eAI_DbgThreshAngleH, 0, 0).AnglesToVector().Multiply3(m_ExTransformPlayer);
		dir[1] = 0;
		vector left = neck + dir;
		Expansion_DebugObject(-101, left, "ExpansionDebugSphereSmall", vector.Zero, neck);
		dir = Vector(m_eAI_LookRelAngles[0] + m_eAI_DbgThreshAngleH, 0, 0).AnglesToVector().Multiply3(m_ExTransformPlayer);
		dir[1] = 0;
		vector right = neck + dir;
		Expansion_DebugObject(-102, right, "ExpansionDebugSphereSmall", vector.Zero, neck);
/*
	#else
		if (m_eAI_DbgLOSAngles != -1)
		{
			float decodedThreshAngleH = (int)(m_eAI_DbgLOSAngles & 0x0000ffff);
			decodedThreshAngleH = decodedThreshAngleH / 65535 * 360;

			float decodedLookAngleH = (int)((m_eAI_DbgLOSAngles & 0xffff0000) >> 16);
			decodedLookAngleH = decodedLookAngleH / 65535 * 360;

			//! @note doesn't really work well with half angles >= 90 deg
			Debug.DrawCone(neck, 1.0, decodedThreshAngleH * Math.DEG2RAD, -decodedLookAngleH * Math.DEG2RAD + Math.PI_HALF, Colors.GREEN, ShapeFlags.ONCE);
		}
*/
	#endif
	#endif

		if (target)
			return true;

		return false;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (!g_Game)
			return;

	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler");
	#endif

	/*
		if (GetIdentity())
		{
			super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);
			return;
		}
	*/

		// CarScript car;
		// if (Class.CastTo(car, GetParent()))
		//{
		//	car.Control(pDt);
		// }

		int i;

		if (pCurrentCommandID != m_eAI_CurrentCommandID)
		{
		#ifdef DIAG_DEVELOPER
			if (EXTrace.AI)
				EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(m_eAI_CurrentCommandID) + " -> " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
		#endif

			m_eAI_CurrentCommandID = pCurrentCommandID;
			m_eAI_CommandTime = 0.0;
		}
		else if (pCurrentCommandFinished && EXTrace.AI)
		{
			EXTrace.Print(true, this, "CommandHandler " + Expansion_CommandIDToString(pCurrentCommandID) + " finished " + pCurrentCommandFinished);
		}

		m_eAI_CommandTime += pDt;

		m_eAI_SkipScript = false;

	#ifdef EXTRACE_DIAG
		auto trace2 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(02) -> super");
	#endif

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);

	#ifdef EXTRACE_DIAG
		trace2 = null;
	#endif

		//! @note CommandHandler super may have started different command!
		//! Use pCurrentCommandID/pCurrentCommandFinished only to check for previous command,
		//! and actualCommandID for actual currently running command!
		
		//! <--------------------------------------------------------------------------------

		GetMovementState(m_MovementState);

		int actualCommandID = m_MovementState.m_CommandTypeId;

		if (actualCommandID != pCurrentCommandID)
		{
		#ifdef DIAG_DEVELOPER
			if (EXTrace.AI)
				EXTrace.Print(true, this, "CommandHandler super " + Expansion_CommandIDToString(pCurrentCommandID) + " -> " + Expansion_CommandIDToString(actualCommandID));
		#endif

			m_eAI_CurrentCommandID = actualCommandID;
			m_eAI_CommandTime = 0.0;
		}

		//! -------------------------------------------------------------------------------/>

		if (m_eAI_DeathHandled)
			return;

		if (!g_Game.IsServer())  //! @note ComandHandler will not run on MP client for AI anyway
			return;

		if (m_eAI_Recreate)
		{
			if (!m_eAI_Recreating)
			{
				m_eAI_Recreating = true;
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_Recreate, 34, false);
			}

			return;
		}

	//#ifdef EXTRACE_DIAG
		//auto trace3 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(03)");
	//#endif

		if (m_eAI_CommandHandlerDT >= 0.12)
			m_eAI_CommandHandlerDT = 0;
		m_eAI_CommandHandlerDT += pDt;

		if (m_eAI_LOSCheckDT >= 0.15)
			m_eAI_LOSCheckDT = 0;
		m_eAI_LOSCheckDT += pDt;

		int simulationPrecision = 0;

		GetTransform(m_ExTransformPlayer);

		vector playerPosition = m_ExTransformPlayer[3];
		m_eAI_SurfaceY = g_Game.SurfaceY(playerPosition[0], playerPosition[2]);

		if (playerPosition[1] < m_eAI_SurfaceY - 0.5)
		{
			playerPosition[1] = m_eAI_SurfaceY;
			EXTrace.Print(EXTrace.AI, this, "below surface - teleporting from " + m_ExTransformPlayer[3] + " to " + playerPosition);
			SetPosition(playerPosition);
			return;
		}

		//! Needed because vanilla only checks for INSTANCETYPE_SERVER
		GetPlayerSoundManagerServer().Update();
		ShockRefill(pDt);
		FreezeCheck();

		eAI_CheckIsInCover();
		eAI_CheckIsInDangerByArea();
		eAI_CheckIsAttachedToMovingParent();

		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();

		if (entityInHands && entityInHands.IsWeapon())
			m_eAI_HasProjectileWeaponInHands = true;
		else
			m_eAI_HasProjectileWeaponInHands = false;

	//#ifdef EXTRACE_DIAG
		//trace3 = null;
	//#endif

		eAITarget previousTarget = m_eAI_Targets[0];
		UpdateTargets(pDt, entityInHands);
		if (eAI_RemoveTargets() || m_eAI_UpdateTargetsTick == 0)
			eAI_PrioritizeTargets();
		//if (m_eAI_SyncCurrentTarget)
			//eAI_SyncCurrentTarget();

		bool hadLOS = m_eAI_HasLOS;
		m_eAI_HasLOS = EnforceLOS();

	//#ifdef EXTRACE_DIAG
		//auto trace5 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(05)");
	//#endif

	#ifdef DIAG_DEVELOPER
		if (!m_eAI_Targets.Count())
		{
			Expansion_DeleteDebugObject(18);
			Expansion_DeleteDebugObject(19);
		}
	#endif

		eAITarget currentTarget;
		if (!m_eAI_HasLOS && m_eAI_NoiseTarget > 0)
		{
			currentTarget = m_eAI_Targets[0];
			m_eAI_Targets[0] = m_eAI_Targets[m_eAI_NoiseTarget];
			m_eAI_Targets[m_eAI_NoiseTarget] = currentTarget;
#ifdef DIAG_DEVELOPER
			float noiseThreatLevelActive = m_eAI_Targets[0].m_ThreatLevelActive;
			EXTrace.Print(EXTrace.AI, this, "eAI_PrioritizeTargets - prioritizing noise target " + m_eAI_NoiseTarget + " " + m_eAI_Targets[0].GetDebugName() + " threat lvl " + m_eAI_CurrentThreatToSelfActive + " -> " + noiseThreatLevelActive);
#endif
			m_eAI_NoiseTarget = 0;
		}

		currentTarget = m_eAI_Targets[0];

		if (!currentTarget)
		{
			if (previousTarget)
				m_eAI_Callbacks.OnNoMoreTargets();
		}
		else if (currentTarget != previousTarget)
		{
			m_eAI_Callbacks.OnTargetSelected(currentTarget);
		}

		if (m_eAI_HasLOS != hadLOS)
		{
			if (m_eAI_HasLOS)
				m_eAI_Callbacks.OnLOS(currentTarget);
			else
				m_eAI_Callbacks.OnLOSLost(currentTarget);
		}

		DetermineThreatToSelf(pDt);

		if (!IsUnconscious() && !IsRestrained())
			ReactToThreatChange(pDt, entityInHands);

		if (m_eAI_HasLOS && m_eAI_CurrentThreatToSelfActive > 0.1 && m_eAI_AcuteDangerTargetCount <= 1 && m_eAI_AcuteDangerPlayerTargetCount == 0)
			m_eAI_SilentAttackViabilityTime += pDt;  //! Prefer melee if gun not silenced
		else
			m_eAI_SilentAttackViabilityTime = 0;

	//#ifdef EXTRACE_DIAG
		//trace5 = null;
	//#endif

		if (actualCommandID != DayZPlayerConstants.COMMANDID_LADDER)
		{
			eAI_HandleAiming(pDt, m_eAI_HasLOS);

			if (actualCommandID != DayZPlayerConstants.COMMANDID_CLIMB && !m_eAI_IsAttachedToMovingParent && !m_FSM.IsInState("Idle"))
				m_PathFinding.OnUpdate(pDt, simulationPrecision);
		#ifdef DIAG_DEVELOPER
			else
				m_PathFinding.DrawDebug();
		#endif
		}
		else
		{
			LookAtDirection("0 0 1");
			AimAtDirection("0 0 1");
		}

		if (m_eAI_ResetMovementDirectionActive)
		{
			m_MovementDirectionActive = false;
			m_eAI_ResetMovementDirectionActive = false;
		}

		if (m_eAI_SideStepTimeout > 0)
		{
			if (m_eAI_HasLOS && m_eAI_SideStepCancelOnLOS)
				m_eAI_SideStepTimeout = 0.0;
			else
				m_eAI_SideStepTimeout -= pDt;

			if (m_eAI_SideStepTimeout <= 0)
			{
				eAI_ResetSidestep();
			}
		}

		if (m_eAI_PositionOverrideTimeout > 0)
			m_eAI_PositionOverrideTimeout -= pDt;

		HumanInputController hic = GetInputController();

		if (hic && !pCurrentCommandFinished)
		{
			//HumanCommandVehicle vehicleCmd = GetCommand_Vehicle();
			//if (!vehicleCmd || !vehicleCmd.IsGettingIn())
			//{
				bool exitIronSights = false;
				//! @note HandleWeapons also deals with hands lowering/raising if no weapon in hands
				eAI_HandleWeapons(pDt, entityInHands, hic, exitIronSights);
			//}
		}

	//#ifdef EXTRACE_DIAG
		//auto trace10 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(10)");
	//#endif

		m_eAI_PurgeFiredShotsTick += pDt;
		if (m_eAI_PurgeFiredShotsTick > 1.0)
		{
			m_eAI_PurgeFiredShotsTick = 0;

			float time = g_Game.GetTickTime();

		#ifdef DIAG_DEVELOPER
			bool dbgObjEnabled = s_Expansion_DebugObjects_Enabled;
			s_Expansion_DebugObjects_Enabled = s_eAI_DebugDamage;
		#endif

			for (i = m_eAI_FiredShots.Count() - 1; i >= 0; i--)
			{
				eAIShot shot = m_eAI_FiredShots[i];
				float elapsed = time - shot.m_Time;
				//! If shot has been processed or flight time exceeds 6 seconds (DayZ max), remove shot
				if ((shot.m_ProcessedTime && time - shot.m_ProcessedTime > 0.005) || elapsed > 6.0)
				{
				#ifdef DIAG_DEVELOPER
					if (!shot.m_ProcessedTime)
					{
						EXTrace.Print(EXTrace.AI, this, "Discarding unprocessed " + shot.GetInfo());
						vector dir = Vector(shot.m_Direction[0], 0, shot.m_Direction[2]);
						Expansion_DebugObject(m_eAI_DiscardedShot_DbgIdx--, shot.m_HitPosition - "0 1.5 0", "ExpansionDebugNoticeMe_Orange", dir, vector.Zero, 5);
						Expansion_DebugObject(m_eAI_DiscardedShot_DbgIdx--, shot.m_HitPosition, "ExpansionDebugBox_Orange", dir, vector.Zero, 5);

					#ifdef EXPANSION_AI_DMGDEBUG_CHATTY
						ExpansionStatic.MessageNearPlayers(shot.m_HitPosition, 100.0, "[" + ExpansionStatic.FormatFloat(shot.m_Time, 3, false) + "] timeout " + shot.m_HitObject + " travel " + ExpansionStatic.FormatFloat(shot.m_TravelTime, 4, false) + " elapsed " + ExpansionStatic.FormatFloat(elapsed, 4, false));
					#endif
					}
				#endif
					m_eAI_FiredShots.Remove(i);
				}
			}

		#ifdef DIAG_DEVELOPER
			s_Expansion_DebugObjects_Enabled = dbgObjEnabled;
		#endif
		}

	#ifdef EXTRACE_DIAG
		//trace10 = null;
		auto trace11 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(11) -> eAIFSM::Update");
	#endif

		//! Do FSM update only after current command has been running for at least one command handler tick, else initial gun holding will look scuffed
		if (m_FSM && m_eAI_CommandTime > pDt)
			m_FSM.Update(pDt, simulationPrecision);

	#ifdef EXTRACE_DIAG
		trace11 = null;
		//auto trace12 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(12)");
	#endif

		if (m_ActionManager)
		{
			if (actualCommandID != DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				if (m_ShouldBeUnconscious || m_IsUnconscious)
					m_eAI_SkipScript = true;
			}
		}

		//! For the following if-else cascade, order matters!
		if (pCurrentCommandFinished)
		{
			if (m_eAI_IsOnLadder)
			{
				//! Finished climbing ladder
				m_eAI_IsOnLadder = false;
				m_eAI_Ladder = null;
				m_eAI_PreferLadder = false;
				//if (m_eAI_LadderClimbDirection < 0)
					//m_eAI_Recreate = true;  //! Hack fix for vanilla bug with spinning while falling (each time) after climbing ladder
				m_eAI_LadderLoops = 0;
			}

			return;
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			//! Nothing
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_LADDER)
		{
			m_eAI_LadderTime += pDt;
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_CLIMB)
		{
			HumanCommandClimb hcc = GetCommand_Climb();

			if (hcc)
			{
				int hccState = hcc.GetState();
				
				/**
				 * @note
				 * For vaulting: MOVE -> TAKEOFF -> ONTOP -> FALLING -> FINISH
				 * For climbing: MOVE -> TAKEOFF -> ONTOP
				 */

				if (hccState != m_eAI_PrevHCCState)
				{
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, this, "CommandClimb -> " + typename.EnumToString(ClimbStates, hccState) + "(" + hccState + ")");
				#endif
					m_eAI_PrevHCCState = hccState;

					if (hccState == ClimbStates.STATE_ONTOP)
					{
						float dist2DSq = ExpansionMath.Distance2DSq(playerPosition, m_PathFinding.GetNextPoint());
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, "pos " + playerPosition + " next waypoint " + m_PathFinding.GetNextPoint() + " dist2DSq " + dist2DSq);
					#endif
						if (dist2DSq < 4.0)
						{
						#ifdef DIAG_DEVELOPER
							Expansion_DebugObject(-1000, m_PathFinding.GetNextPoint(), "ExpansionDebugNoticeMe_Blue", GetDirection());
						#endif
							m_PathFinding.UpdateNext(true);
						}
					}
				}
			}
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_ACTION)
		{
			//! Nothing
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
		{
			//! Nothing
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_DAMAGE)
		{
			//! Nothing
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_FALL)
		{
			m_eAI_Ladder = null;
			m_eAI_LadderLoops = 0;

			if (m_eAI_IsOnLadder)
			{
				m_eAI_IsOnLadder = false;
				//m_eAI_Recreate = true;  //! Hack fix for vanilla bug with spinning while falling (each time) after climbing ladder
			}

			return;
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			//! Nothing
		}
		else if (m_eAI_Transport)
		{
			int seat_anim_type = m_eAI_Transport.GetSeatAnimationType(m_eAI_Transport_SeatIndex);
			auto vehCommand = StartCommand_Vehicle(m_eAI_Transport, m_eAI_Transport_SeatIndex, seat_anim_type);
			vehCommand.SetVehicleType(m_eAI_Transport.GetAnimInstance());

			m_eAI_Transport = null;
			return;
		}
		else if (m_eAI_Ladder && m_eAI_BuildingWithLadder && m_eAI_CommandTime > 1.0)
		{
			if (eAI_IsInLadderRadius(m_eAI_LadderEntryPoint) && Math.IsPointInCircle(m_PathFinding.GetEnd(), 0.55, playerPosition) && !eAI_IsChangingStance())
			{
				bool ladderUnreachable;

				if (eAI_IsCloseToLadderEntryPoint() && eAI_CanReachLadderEntryPoint())
				{
					m_eAI_LastClimbedLadder = m_eAI_Ladder;
					m_eAI_LastClimbedBuildingWithLadder = m_eAI_BuildingWithLadder;
					m_eAI_IsOnLadder = true;
					m_eAI_LadderTime = 0;
					SetClimbingLadderType(m_eAI_Ladder.m_Type);
					eAI_ResetRaised();
					StartCommand_Ladder(m_eAI_BuildingWithLadder, m_eAI_Ladder.m_Index);
					m_eAI_CommandMove.ResetIsBlockedByBuildingWithLadder();
					m_eAI_SkipScript = true;
				}
				else
				{
					ladderUnreachable = true;
					m_PathFinding.m_IsUnreachable = true;
				}

				if (m_eAI_BuildingWithLadder.Expansion_GetLaddersCount() > 1)
					m_eAI_Ladders[m_eAI_BuildingWithLadder].Remove(m_eAI_Ladder.m_Index);

				if (ladderUnreachable)
					m_eAI_Ladder = null;
			}
		}
		else if (actualCommandID == DayZPlayerConstants.COMMANDID_SWIM)
		{
			//! Nothing
		}

		vector lookTargetRelAngles = m_eAI_LookDirectionTarget_ModelSpace.VectorToAngles();
		vector aimTargetRelAngles = m_eAI_AimDirectionTarget_ModelSpace.VectorToAngles();

		//! We want to interpolate rel angles for looking! Otherwise, if the conversion to rel angles happens later,
		//! there will be a sudden jump in the unit's head rotation between 180 and -180 due to the way the head animation is set up
		lookTargetRelAngles[0] = ExpansionMath.RelAngle(lookTargetRelAngles[0]);
		lookTargetRelAngles[1] = ExpansionMath.RelAngle(lookTargetRelAngles[1]);

		lookTargetRelAngles[1] = Math.Clamp(lookTargetRelAngles[1], -85.0, 85.0);  //! Valid range is [-85, 85]

		//TODO: quaternion slerp instead for better, accurate results
		m_eAI_LookRelAngles = ExpansionMath.InterpolateAngles(m_eAI_LookRelAngles, lookTargetRelAngles, pDt, Math.RandomFloat(3.0, 5.0), Math.RandomFloat(1.0, 3.0));

		float lookLR = m_eAI_LookRelAngles[0];
		float lookUD = m_eAI_LookRelAngles[1];

		if (IsRaised())
		{
			//! Need to adjust look direction when aiming
			lookLR =  ExpansionMath.RelAngle(lookLR - m_eAI_AimRelAngles[0]);
			if (entityInHands && entityInHands.IsWeapon())
				lookUD = 0;
		}

		m_eAI_CommandMove.SetLookAnglesRel(lookLR, lookUD);

	//#ifdef EXTRACE_DIAG
		//trace12 = null;
	//#endif

		if (!m_eAI_SkipScript && eAI_IsLocomotionCmd(actualCommandID))
		{
			eAICommandMove ecm;

			int performCommand;
			//if (Class.CastTo(ecm, ehcs))
			if (actualCommandID != DayZPlayerConstants.COMMANDID_VEHICLE)
				performCommand = EAI_COMMANDID_MOVE;

			ecm = m_eAI_CommandMove;

			switch (performCommand)
			{
			case EAI_COMMANDID_MOVE:
				if (m_eAI_IsOnLadder)
				{
					HumanCommandLadder hcl = GetCommand_Ladder();
					if (!hcl)
					{
						m_eAI_IsOnLadder = false;
						m_eAI_Ladder = null;
						m_eAI_PreferLadder = false;
						m_eAI_LadderLoops = 0;
					}
					else if (hcl.CanExit() && m_eAI_LadderTime > 2.0)
					{
						hcl.Exit();
					}
				}
				else if (m_eAI_CommandHandlerDT < 0.12)
				{
					//! Do nothing
				}
				else if (AI_HANDLEDOORS && HandleBuildingDoors(ecm, pDt))
				{
					break;
				}
				else if (AI_HANDLEVAULTING && HandleVaulting(ecm, pDt))
				{
					if (m_JumpClimb.Expansion_Climb())
						break;
					
					//! If we didn't vault or climb, find way around obstacle, but only if path is not blocked physically
					//! (path blocked physically is only set on pathfinding if navmesh isn't blocked,
					//! so in that case we won't find an alternative path)
					if (m_PathFinding.m_AllowJumpClimb && !m_PathFinding.m_IsBlockedPhysically && !IsSwimming())
						m_PathFinding.SetAllowJumpClimb(false, 15.0);
				}
				else if (m_PathFinding.m_IsJumpClimb && m_PathFinding.m_AllowJumpClimb && !m_PathFinding.m_IsBlockedPhysically && ecm.IsBlocked() && m_eAI_BlockedTime > pDt && !IsSwimming())
				{
					//! If we are still blocked, find way around obstacle
					//! (need to be blocked for at least one commandhandler tick
					//! else it messes with climbing after door interaction)
					m_PathFinding.SetAllowJumpClimb(false, 15.0);
				}
				
				float speedLimit;
				if (m_eAI_Halt)
					speedLimit = 0.0;
				else if (eAI_IsSideSteppingVehicle())
					speedLimit = 3.0;
				else if (m_eAI_CurrentThreatToSelfActive >= 0.4 || m_FSM.IsInState("Flank"))
					speedLimit = m_MovementSpeedLimitUnderThreat;
				else
					speedLimit = m_MovementSpeedLimit;

				if (m_WeaponRaised)
					speedLimit = Math.Min(speedLimit, 2);

				float turnTarget;
				bool setTurnTarget;

				if (m_eAI_TurnTargetActive)
				{
					turnTarget = m_eAI_TurnTarget;
					setTurnTarget = true;
				}
				else if (m_eAI_AimDirection_Recalculate || Math.AbsFloat(m_eAI_AimRelAngles[0]) > 90)
				{
					turnTarget = GetAimDirectionTarget().VectorToAngles()[0];
					setTurnTarget = true;

					if (!m_eAI_LookDirection_Recalculate)
					{
						lookTargetRelAngles[0] = 0.0;
						m_eAI_LookDirectionTarget_ModelSpace = lookTargetRelAngles.AnglesToVector();
					}

					if (!m_eAI_AimDirection_Recalculate)
					{
						aimTargetRelAngles[0] = 0.0;
						m_eAI_AimDirectionTarget_ModelSpace = aimTargetRelAngles.AnglesToVector();
					}
				}
				else if (IsInherited(eAINPCBase) && ecm.GetCurrentMovementSpeed() == 0.0)
				{
					turnTarget = GetOrientation()[0];
					setTurnTarget = true;
				}
				else if (speedLimit == 0 && m_eAI_DefaultLookAngle != 0)
				{
					turnTarget = m_eAI_DefaultLookAngle;
					setTurnTarget = true;
				}

				if (setTurnTarget)
					ecm.SetTurnTarget(turnTarget, m_eAI_TurnTargetActive);

				if (GetInputController().LimitsIsSprintDisabled())
				{
					speedLimit = Math.Min(speedLimit, 2);
				}

				if (!m_MovementDirectionActive)
					m_MovementDirection = 0;

				ecm.SetSpeedOverrider(m_MovementSpeedActive);
				ecm.SetTargetSpeed(m_MovementSpeed);
				ecm.SetSpeedLimit(speedLimit);
				ecm.SetTargetDirection(m_MovementDirection, m_MovementDirectionActive);

				if (ecm.GetCurrentMovementSpeed() >= 0.5 && m_eAI_PositionTime > 4.98 && Expansion_IsAnimationIdle())
				{
				#ifdef DIAG_DEVELOPER
					string pos = ExpansionStatic.VectorToString(playerPosition);
					string msg = "Movement speed " + ecm.GetCurrentMovementSpeed() + " but not moving for " + m_eAI_PositionTime + " s " + Debug.GetDebugName(this) + " (pos=" + pos + "), item in hands " + ExpansionStatic.GetDebugInfo(GetItemInHands());

					EXPrint(this, msg);

					msg.Replace("<", "‹");
					msg.Replace(">", "›");
					ExpansionNotification("NOT MOVING", msg).Error();
				#endif

					Object blockingObject = m_eAI_CommandMove.GetBlockingObject();
					if ((blockingObject && !blockingObject.IsPlainObject() && !blockingObject.IsRock()) || m_PathFinding.m_PathGlueIdx != -1)
					{
						vector waypoint = m_eAI_CommandMove.GetWaypoint();
						if (waypoint != playerPosition && waypoint != vector.Zero)
						{
							m_eAI_PositionTime = 0;
							SetPosition(waypoint);  //! OH GOD THE HACKS

						#ifdef DIAG_DEVELOPER
							pos = ExpansionStatic.VectorToString(waypoint);
							float dist = vector.Distance(playerPosition, waypoint);
							msg = "Teleporting " + Debug.GetDebugName(this) + " to " + pos + " (distance " + dist + " m)";

							EXPrint(this, msg);

							msg.Replace("<", "‹");
							msg.Replace(">", "›");
							ExpansionNotification("TELEPORTING", msg).Error();
						#endif
						}
						else
						{
							m_eAI_PositionTime = 0;
							eAI_ResetPathfinding();
						}
					}
					else
					{
						m_eAI_PositionTime = 0;
						eAI_ResetPathfinding();
					}
				}

				break;
			default:
			#ifdef DIAG_DEVELOPER
				Expansion_DebugObject(22222, "0 0 0", "ExpansionDebugSphereSmall_White");
				Expansion_DebugObject(22223, "0 0 0", "ExpansionDebugSphereSmall_Black");
			#endif
				break;
			}

			m_eAI_CommandMove.PreAnimUpdate(pDt);
		}
	}

	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//if (!GetIdentity())
			return false;

		//return super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//if (!GetIdentity())
			return false;

		//return super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}
	
	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//if (!GetIdentity())
			return false;

		//return super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	bool eAI_IsLocomotionCmd(int cmdID)
	{
		switch (cmdID)
		{
			case DayZPlayerConstants.COMMANDID_MELEE2:
			case DayZPlayerConstants.COMMANDID_MOVE:
			case DayZPlayerConstants.COMMANDID_LADDER:
			case DayZPlayerConstants.COMMANDID_SWIM:
			case DayZPlayerConstants.COMMANDID_VEHICLE:
				return true;
		}

		return false;
	}

	bool eAI_ShouldStartFalling()
	{
		vector position = m_ExTransformPlayer[3];
		if (!IsSwimming() && position[1] - ExpansionStatic.GetSurfaceRoadY3D(position[0], position[1], position[2], RoadSurfaceDetection.UNDER) >= 0.5)
			return true;

		return false;
	}

	bool eAI_ShouldStartSwimming(bool checkSwimming = true)
	{
		vector wl;

		if ((!checkSwimming || !IsSwimming()) && DayZPlayerUtils.CheckWaterLevel(this, wl) == EWaterLevels.LEVEL_SWIM_START)
			return true;

		return false;
	}

	bool eAI_UpdateLookDirectionPreference()
	{
		if (!GetGroup() || IsUnconscious())
			return false;
		
		float time = g_Game.GetTickTime();
		bool force;

		if (m_eAI_FormationDirectionUpdateTime == 0.0)
			force = true;

		float speed = Expansion_GetMovementSpeed();
		if (m_eAI_MovementSpeedPrev == 0.0 && speed > 0.0)
			force = true;

		if (!force && time - m_eAI_FormationDirectionUpdateTime < m_eAI_FormationDirectionNextUpdateTime)
			return true;

		int count = GetGroup().Count();
		
		m_eAI_MovementSpeedPrev = speed;
		float f = 4.0 - speed;
		if (speed > 0.0)
			m_eAI_FormationDirectionNextUpdateTime = Math.RandomFloat(0.5, 0.333333 * f);
		else
			m_eAI_FormationDirectionNextUpdateTime = Math.RandomFloat(2.0, 6.666666 * f);
		m_eAI_FormationDirectionUpdateTime = time;
		vector pos, ori, dir;
		float angle;
		bool isDir;
		bool isDirWS;

		auto hcv = GetCommand_Vehicle();
		auto hcm = GetCommand_Move();

		if (speed > 0 || !hcm)
		{
			isDir = true;
			if (!hcv)
			{
				Object blockingObject = m_eAI_CommandMove.GetBlockingObject();

				if (blockingObject)
				{
					pos = blockingObject.GetPosition();
					pos[1] = GetPosition()[1];
				}
				else
				{
					pos = m_eAI_CommandMove.GetWaypoint() + GetDirection();
				}

				dir = vector.Direction(GetPosition(), pos);
				if (dir.LengthSq() > 0.01)
				{
					angle = ExpansionMath.AngleDiff2(GetOrientation()[0], dir.VectorToAngles()[0]);
					ori[0] = Math.Clamp(angle, -45.0, 45.0);

					//! We use the up/down angle of the path segment direction because direction from AI to waypoint
					//! may otherwise be steeper than intended as AI position is on surface while waypoint is on navmesh,
					//! which can lead to a significant height difference.
					angle = ExpansionMath.RelAngle(m_PathFinding.m_PathSegmentDirection.VectorToAngles()[1]);
					ori[1] = Math.Clamp(angle, -45.0, 45.0);

					if (speed >= 2)
						ori[1] = ori[1] + 8 * speed;  //! The faster we go, the more we lean forward, compensate this by looking up slightly

					//EXTrace.Print(EXTrace.AI, this, "look u/d to wp " + ori[1]);
				}
				else
				{
					return true;
				}
			}
			else
			{
				ori[0] = Math.RandomFloat(-22.0 * f, 22.0 * f);
			}
		}
		else if (m_PathFinding.GetRemainingCount() == 2 && m_PathFinding.m_IsUnreachable && !Math.IsPointInCircle(m_PathFinding.GetEnd(), 0.55, m_ExTransformPlayer[3]) && !m_FSM.IsInState("Idle"))
		{
			//! Look and aim towards current waypoint so we turn in direction we can go.
			//! This helps with movement towards ladder or when unreachable due to fallheight check
			pos = m_PathFinding.GetCurrentPoint();
			pos[1] = GetBonePositionWS(GetBoneIndexByName("neck"))[1];
		}
		else if (m_PathFinding.GetRemainingCount() > 2 && m_PathFinding.m_IsUnreachable && !m_FSM.IsInState("Idle"))
		{
			//! Look and aim towards next waypoint so we turn in direction we can go.
			//! This helps with movement towards ladder or when unreachable due to fallheight check
			pos = m_PathFinding.GetNextPoint();
			pos[1] = GetBonePositionWS(GetBoneIndexByName("neck"))[1];
		}
		else if (m_eAI_DefaultLookAngle != 0)
		{
			ori[0] = m_eAI_DefaultLookAngle;
			isDir = true;
			isDirWS = true;
		}
		else
		{
			//! Roll dice for a random look at behaviour
			int diceRoll = Math.RandomIntInclusive(1, 100);

			//! 4% to look at another member and Emote if weapon not raised (we don't want to aim gun at friendly)
			if (diceRoll < 4 && count > 1 && !IsRaised())
			{
				pos = GetGroup().GetRandomMemberExcluding(this).GetPosition() + "0 1.5 0";
				ori = vector.Direction(GetPosition() + "0 1.5 0", pos).VectorToAngles();

				if (Math.AbsFloat(ori[1]) > 45.0)
					return true;  //! Don't look up or down at steep angles

				angle = ExpansionMath.AngleDiff2(GetOrientation()[0], ori[0]);

				if (!m_Expansion_EmoteID && angle > -30 && angle < 30)
				{
					isDir = true;
					ori[0] = angle;

					eAI_RandomGreeting();
				}
			}
			//! 12% to look at another member if weapon not raised (we don't want to aim gun at friendly)
			else if (diceRoll < 16 && count > 1 && !IsRaised())
			{
				pos = GetGroup().GetRandomMemberExcluding(this).GetPosition() + "0 1.5 0";
				ori = vector.Direction(GetPosition() + "0 1.5 0", pos).VectorToAngles();

				if (Math.AbsFloat(ori[1]) > 45.0)
					return true;  //! Don't look up or down at steep angles

				angle = ExpansionMath.AngleDiff2(GetOrientation()[0], ori[0]);

				if (angle < -80 || angle > 80)
				{
					isDir = true;
					ori[0] = angle;
				}
				else
				{
					pos[1] = pos[1] + Math.RandomFloat(0.1, 0.2);
				}
			}
			//! 84% to look in formation direction, ahead or around (100% if no other group members)
			else
			{
				isDir = true;

				//! 41% to look in formation direction (56% if no other group members)
				if (diceRoll < 57)
				{
					ori[0] = Math.RandomFloat(-54.0, 54.0);

					if (count > 1 && GetGroup().GetFormationState() == eAIGroupFormationState.IN)
					{
						isDirWS = true;

						vector formPos = GetGroup().GetFormation().ToWorld(m_eAI_FormationPosition);
						dir = vector.Direction(formPos, GetGroup().GetFormation().ToWorld(m_eAI_FormationPosition + m_eAI_FormationDirection));
						ori[0] = ori[0] + dir.VectorToAngles()[0];

					#ifdef DIAG_DEVELOPER
						Expansion_DebugObject(987654321, formPos + "0 1.5 0" + dir, "ExpansionDebugSphereSmall_Orange", dir, formPos + "0 1.5 0");
					#endif
					}

					if (!m_Expansion_EmoteID && Math.RandomFloat(0.0, 1.0) < 0.05)
						Expansion_SetEmote(EmoteConstants.ID_EMOTE_WATCHING, true);
				}
				//! 43% to look ahead if NPC or around if not NPC
				else if (IsInherited(eAINPCBase))
				{
					ori[0] = Math.RandomFloat(-72.0, 72.0);
				}
				else
				{
					ori[0] = Math.RandomFloat(-144.0, 144.0);
				}
			}
		}

		if (isDir)
		{
			if (IsRaised())
				ori[1] = Math.RandomFloat(-3.0, 0.0);
			else if ((hcm && speed == 0) || hcv)
				ori[1] = Math.RandomFloat(-36.0, 18.0);

			dir = ori.AnglesToVector();

			if (isDirWS)
				dir = dir.InvMultiply3(m_ExTransformPlayer);

			LookAtDirection(dir);
			AimAtDirection(dir);
		}
		else
		{
			LookAtPosition(pos, true);
			AimAtPosition(pos, true);
		}

		return true;
	}

	void eAI_RandomGreeting(bool excludeWatching = false)
	{
		bool manners = GetExpansionSettings().GetAI().Manners;

		int emoteId;
		int min;
		int max;

		if (manners)
			min = 4;

		if (Expansion_GetMovementSpeed() > 0.0)
			max = 8;
		else
			max = 9;  //! Only include salute if not moving

		switch (Math.RandomIntInclusive(min, max))
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
				emoteId = EmoteConstants.ID_EMOTE_TAUNTTHINK;
				break;
			case 4:
				emoteId = EmoteConstants.ID_EMOTE_GREETING;
				break;
			case 5:
				emoteId = EmoteConstants.ID_EMOTE_TAUNTKISS;
				break;
			case 6:
				emoteId = EmoteConstants.ID_EMOTE_POINT;
				break;
			case 7:
				if (excludeWatching)
					return;
				emoteId = EmoteConstants.ID_EMOTE_WATCHING;
				break;
			case 8:
				emoteId = EmoteConstants.ID_EMOTE_HEART;
				break;
			case 9:
				emoteId = EmoteConstants.ID_EMOTE_SALUTE;
				break;
		}

		Expansion_SetEmote(emoteId, true);
	}

	bool EnforceLOS()
	{
		//! Prevent core dump on server shutdown
		if (!g_Game)
			return false;

		if (!m_eAI_Targets.Count())
		{
			return false;
		}

		auto target = GetTarget();

		EntityAI targetEntity = target.GetEntity();

		if (!targetEntity)
		{
			return false;
		}

		eAITargetInformationState state = target;

		if (m_eAI_LOSCheckDT < 0.15 && !m_eAI_TargetChanged)
			return state.m_LOS;

		PlayerBase targetPlayer;
		if (Class.CastTo(targetPlayer, targetEntity) && targetPlayer.Expansion_HasAdminToolInvisibility())
		{
#ifdef DIAG_DEVELOPER
			eAI_UpdatePlayerIsEnemyStatus(targetPlayer, false, "target invisible");
#endif
			state.m_LOS = false;
			return false;
		}

		if (IsUnconscious())
		{
			state.m_LOS = false;
			return false;
		}

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(04) -> EnforceLOS");
#endif

		string boneName;
		if (IsRaised())
			boneName = "Neck";
		else
			boneName = "Head";

		vector begPos = GetBonePositionWS(GetBoneIndexByName(boneName));
		vector aimOffset = target.GetAimOffset();
		vector dir = vector.Direction(begPos, target.GetPosition(true) + aimOffset);
		float dist = eAI_GetVisibilityDistance(dir.Length(), target);
		dir.Normalize();
		//! Extend LOS ray by some amount because some targets like doors have inaccurate position and ray would not hit otherwise
		vector endPos = begPos + dir * dist + dir * 0.5;

		Object parent;

		if (targetPlayer)
		{
			//! If targeting a player, and player is not in vehicle, or vehicle engine is not on,
			//! or vehicle is farther away than 150 m and not a helicopter, check if we are facing target
			//! (look direction, not movement direction)
			ExpansionVehicle vehicle;
			if (!Class.CastTo(parent, targetPlayer.Expansion_GetParent()) || !ExpansionVehicle.Get(vehicle, parent) || !vehicle.EngineIsOn() || (dist > 150 && !vehicle.IsHelicopter()))
			{
				vector toTargetAngles = dir.VectorToAngles();
				float toTargetAngleH = toTargetAngles[0];
				//float toTargetAngleV = toTargetAngles[1];
				vector lookAngles = GetLookDirection().VectorToAngles();
				float lookAngleH = lookAngles[0];
				//float lookAngleV = lookAngles[1];
				float angleDiffH = ExpansionMath.AngleDiff2(toTargetAngleH, lookAngleH);
				//float angleDiffV = ExpansionMath.AngleDiff2(toTargetAngleV, lookAngleV);
				float threshAngleH = eAI_CalculateFOVHalfAngleH(dist);
			#ifdef DIAG_DEVELOPER
				m_eAI_DbgThreshAngleH = threshAngleH;
				m_eAI_DbgLookAngleH = lookAngleH;
				//int encodedThreshAngleH = threshAngleH / 360.0 * 65535;
				//int encodedLookAngleH = lookAngleH / 360.0 * 65535;
				//encodedLookAngleH = encodedLookAngleH << 16;
				//m_eAI_DbgLOSAngles = encodedThreshAngleH | encodedLookAngleH;
			#endif
				//if (Math.AbsFloat(angleDiffH) > threshH || Math.AbsFloat(angleDiffV) > threshV)  //! Player is outside AI FOV
				if (Math.AbsFloat(angleDiffH) > threshAngleH)  //! Player is outside AI FOV
				{
				#ifdef DIAG_DEVELOPER
					Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugSphereSmall");
					Expansion_DebugObject_Deferred(19, endPos, "ExpansionDebugSphereSmall_Red", dir, begPos);
				#endif
					
					state.m_LOS = false;
					return false;
				}
			}
		}

		bool isItemTarget = targetEntity.IsItemBase();

		vector contactPos;
		vector contactDir;
		int contactComponent;

		set< Object > results = new set< Object >;
		float radius;
	#ifdef EXPANSION_AI_ITEM_TARGET_REQUIRE_LOS
		if (isItemTarget)
			radius = 0.15;
		else
	#endif
			radius = 0.05;
		bool hadLOS = state.m_LOS;
		state.m_LOS = DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, null, this, false, false, ObjIntersectView, radius);
		if (!state.m_LOS)
		{
			if (hadLOS && m_eAI_IsFightingFSM)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_UpdateLeanTarget, 1000, false);

		#ifdef DIAG_DEVELOPER
			if (hadLOS)
			{
				EXTrace.Print(EXTrace.AI, this, "lost line of sight to target " + targetEntity);
				EXTrace.Print(EXTrace.AI, this, "aim offset " + aimOffset + " end pos " + endPos);
			}

			state.m_LOSRaycastHitObject = null;
			state.m_LOSRaycastHitPosition = vector.Zero;

			Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugSphereSmall");
			Expansion_DebugObject_Deferred(19, "0 0 0", "ExpansionDebugSphereSmall_Red");
		#endif
			return false;
		}

		//! Reset state for further checks
		state.m_LOS = false;

		bool isCreatureTarget = targetEntity.IsDayZCreature();

		if (results.Count() && results[0].IsBuilding())
		{
			//! Check again in case view is "blocked" by a window/other see-through object
			//! TODO: If a player is behind an Expansion BaseBuilding wall, the AI will "see" the player if he is standing next to a window position
			//! (regardless if the wall actually has windows or not or if they are closed) due to the wall's firegeo and use of ObjIntersectFire
			results.Clear();
			//Expansion_DebugObject_Deferred(1819, contactPos, "ExpansionDebugSphereSmall_Red", contactDir);
			DayZPhysics.RaycastRV(contactPos - dir * 0.1, endPos, contactPos, contactDir, contactComponent, results, null, this, false, false, ObjIntersectFire, radius);
		}

		//float targetDistSq = vector.DistanceSq(begPos, endPos);
		float contactToTargetDistSq = vector.DistanceSq(contactPos, endPos);

		DayZPlayerImplement player;

		bool sideStep;

		Object hitObj;
		EntityAI hitEntity;
		EntityAI hitEntityRoot;
		bool isCreatureHit;
		foreach (Object obj: results)
		{
			hitObj = obj;

			if (Class.CastTo(hitEntity, obj))
			{
				hitEntityRoot = hitEntity.GetHierarchyRoot();
				isCreatureHit = hitEntityRoot.IsDayZCreature();
				obj = hitEntityRoot;
			}

			//! Tree with player more than 2 m away from contact pos
			if (obj.IsTree() && contactToTargetDistSq > 4 && !state.m_SearchPositionUpdateCount)
			{
				if (!isItemTarget)
					sideStep = state.m_ThreatLevelActive >= 0.4;
				break;
			}

			//if (obj.IsBuilding())
			//{
			//	state.m_LOS = false;
			//	break;
			//}

			if (obj == targetEntity)
			{
				//! Right on target
				state.m_LOS = true;
			}
			else if (obj == parent)
			{
				state.m_LOS = true;
			}
			else
			{
				if (Class.CastTo(player, obj))
				{
					if (isItemTarget && !player.IsAlive() && targetEntity.GetHierarchyRootPlayer() == player)
					{
						//! Target is item and item is in player inventory
						state.m_LOS = true;
					}
					else if (!isItemTarget && eAI_GetCachedThreat(player.GetTargetInformation()) < 0.2)
					{
						sideStep = m_eAI_IsFightingFSM;  //! Sidestep if we are in fighting FSM
					}
				}
				else if (isCreatureTarget && isCreatureHit)
				{
					//! If object is zombie or animal but not the target or its parent, we don't care if they get shot when they are in the way
					state.m_LOS = true;
				}
				else if (((obj.IsTree() || obj.IsBush()) && contactToTargetDistSq <= 4 && state.m_ThreatLevelActive >= 0.4) || contactToTargetDistSq <= 0.0625)
				{
					//! If object is tree/bush but not the target or its parent, we don't care if they get shot when they are in the way
					state.m_LOS = true;
				}
			}

			break;
		}

		if (state.m_LOS)
			state.m_SearchOnLOSLost = true;

		if (state.m_LOS && !hadLOS)
		{
			m_eAI_PositionOverrideTimeout = 0.0;
			if (m_eAI_LeanTarget)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_ResetLeanTarget, 1000, false);
		}
		else if (hadLOS && !state.m_LOS && m_eAI_IsFightingFSM)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_UpdateLeanTarget, 1000, false);
		}

	#ifdef DIAG_DEVELOPER
		if (state.m_LOS != hadLOS)
		{
			EXTrace.Print(EXTrace.AI, this, "has line of sight to target (" + targetEntity + ")? " + state.m_LOS);
			EXTrace.Print(EXTrace.AI, this, "aim offset " + aimOffset + " end pos " + endPos);
		}

		state.m_LOSRaycastHitObject = hitObj;
		state.m_LOSRaycastHitPosition = contactPos;

		if (state.m_LOS)
		{
			Expansion_DebugObject_Deferred(18, contactPos, "ExpansionDebugSphereSmall", dir, begPos);
			Expansion_DebugObject_Deferred(19, "0 0 0", "ExpansionDebugSphereSmall_Red");
		}
		else
		{
			Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugSphereSmall");
			Expansion_DebugObject_Deferred(19, contactPos, "ExpansionDebugSphereSmall_Red", dir, begPos);
		}
	#endif

		if (sideStep)
		{
			if (m_eAI_SideStepTimeout > 0 || m_MovementDirectionActive)
				return state.m_LOS;

			//! First check if we are roughly moving in target direction
			//! @note vector.Direction(GetPosition(), GetTarget().GetPosition()).Normalized() returns zero vector,
			//! have to first assign target position to variable. Why is this language so INCREDIBLY fucked? :-(
			vector targetPos = state.m_SearchPosition;
			vector targetDirection = vector.Direction(GetPosition(), targetPos).Normalized();
			float dot = vector.Dot(GetDirection(), targetDirection);
			if (dot >= 0.75)
			{
				eAI_ForceSideStep(1.5, hitObj, 0.0, false, true);
			}
		}

		return state.m_LOS;
	}

	float eAI_CalculateFOVHalfAngleH(float dist)
	{
		float nearHalfAngleH = Math.Max(m_eAI_FOVNear_HalfAngleH, 95);
		float farHalfAngleH = Math.Max(m_eAI_FOVFar_HalfAngleH, 45);

		if (farHalfAngleH == nearHalfAngleH || dist <= m_eAI_FOVNear_DistThreshold)
		{
			return nearHalfAngleH;
		}
		else
		{
			float farDistThresh = Math.Max(m_eAI_FOVFar_DistThreshold, 60);

			if (dist < farDistThresh)
			{
				float range = farDistThresh - m_eAI_FOVNear_DistThreshold;
				float t = Easing.EaseInOutPow(dist / range, m_eAI_FOVTransitionExponent);
				return ExpansionMath.LinearConversion(0, 1, t, nearHalfAngleH, farHalfAngleH);
			}
		}

		return farHalfAngleH;
	}

	//! @note when leaning, aim animation direction needs to be adjusted for character rotation, see HandleWeapons
	void eAI_UpdateLeanTarget()
	{
		eAITarget target = GetTarget();
		if (!target)
			return;

		vector p1 = GetPosition();
		vector p2 = p1 + m_PathFinding.m_PathSegmentDirection;
		vector p = target.GetPosition();

		if (vector.DistanceSq(p1, p) < vector.DistanceSq(p2, p))
		{
			//! Moving away from target
			p1 = p2;
			p2 = GetPosition();
		}

		if (ExpansionMath.Side(p1, p2, p) < 0)
			m_eAI_LeanTarget = -1.0;  //! target is on the right, lean left
		else
			m_eAI_LeanTarget = 1.0;  //! target is on the left, lean right

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_ResetLeanTarget, Math.RandomInt(1000, 2000), false);
	}

	void eAI_ResetLeanTarget()
	{
		m_eAI_LeanTarget = 0.0;
	}

	void eAI_ForceSideStep(float duration, Object obj = null, float angle = 0.0, bool allowBackPedaling = true, bool sideStepCancelOnLOS = false)
	{
		bool blockedLeft;
		bool blockedRight;

		if (angle == 0.0)
		{
			auto cmd = GetCommand_Move();
			if (cmd)
			{
				blockedLeft = m_eAI_CommandMove.CheckBlockedLeft();
				blockedRight = m_eAI_CommandMove.CheckBlockedRight();
			}

			vector transform[4];
			GetTransform(transform);

			if (!blockedLeft)
				blockedLeft = m_PathFinding.IsBlocked(transform[3], transform[3] + (-0.5 * transform[0]));

			if (!blockedRight)
				blockedRight = m_PathFinding.IsBlocked(transform[3], transform[3] + (0.5 * transform[0]));

		#ifdef DIAG_DEVELOPER
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
		m_eAI_SideStepObject = obj;
		m_eAI_SideStepTimeout = duration;
		m_eAI_SideStepCancelOnLOS = sideStepCancelOnLOS;

		if (m_EmoteManager.IsEmotePlaying())
			m_EmoteManager.ServerRequestEmoteCancel();

		EXTrace.Print(EXTrace.AI, this, "sidestep " + m_SideStepAngle + " " + obj);
		OverrideMovementDirection(true, m_SideStepAngle);
	}

	void eAI_CancelSidestep()
	{
		m_eAI_SideStepTimeout = 0.0;
		eAI_ResetSidestep();
	}

	protected void eAI_ResetSidestep()
	{
		EXTrace.Print(EXTrace.AI, this, "sidestep 0");

		m_SideStepAngle = 0.0;
		m_eAI_SideStepCancelOnLOS = false;
		m_eAI_SideStepObject = null;

		OverrideMovementDirection(true, 0.0);
		m_eAI_ResetMovementDirectionActive = true;  //! Next call to cmdhandler

		m_PathFinding.StopOverride();
	}

	bool eAI_IsSideStepping()
	{
		return m_eAI_SideStepTimeout > 0.0;
	}

	float eAI_GetSideStepTimeout()
	{
		return m_eAI_SideStepTimeout;
	}

	override bool eAI_IsSideSteppingObstacles()
	{
		auto cmd = GetCommand_Move();
		return cmd && m_eAI_CommandMove.IsSideSteppingObstacles();
	}

	bool eAI_IsSideSteppingVehicle()
	{
		if (m_eAI_SideStepObject && m_eAI_SideStepObject.IsTransport())
			return true;

		return false;
	}

	bool eAI_IsUnreachable(float minDistance, vector position)
	{
		vector aiPosition = GetPosition();

		if (Math.AbsFloat(position[1] - aiPosition[1]) > minDistance && Math.IsPointInCircle(position, minDistance, aiPosition))
			return true;

		return false;
	}

	bool eAI_ShouldUseSurfaceUnderWaterForPathFinding()
	{
		if (m_eAI_SurfaceY < g_Game.SurfaceGetSeaLevelMin())
		{
			if (IsSwimming())
				return true;

			IEntity floor = PhysicsGetFloorEntity();

			if (eAI_IsSeaIce(floor))
				return true;
		}

		return false;
	}

	bool eAI_IsSeaIce(IEntity entity)
	{
		if (entity)
		{
			string name = entity.GetDebugName();

			name.ToLower();

			//! Sakhal ice floes
			if (name.Contains(": ice_sea"))
				return true;
		}

		return false;
	}

	void eAI_ThreatOverride(EntityAI entity, bool state)
	{
	#ifdef EXTRACE_DIAG
		//EXTrace.Print(EXTrace.AI, this, "eAI_ThreatOverride " + entity + " " + state);
		auto trace = EXTrace.StartStack(EXTrace.AI, this, entity.ToString() + " " + state);
	#endif

		if (state)
			m_eAI_ThreatOverride[entity] = true;
		else
			m_eAI_ThreatOverride.Remove(entity);
	}

	bool eAI_GetThreatOverride(EntityAI entity)
	{
		return m_eAI_ThreatOverride[entity];
	}

	/**
	 * @brief purge no longer existing or far (distance > 1000 m) entities from threat overrides
	 */
	void eAI_PurgeThreatOverride()
	{
		int currentCount = m_eAI_ThreatOverride.Count();

		if (currentCount < 10)
			return;

	#ifdef DIAG_DEVELOPER
		auto timeIt = new EXTimeIt();
	#endif

		array<EntityAI> threats = {};

		foreach (EntityAI threat, bool state: m_eAI_ThreatOverride)
		{
			if (threat)
				threats.Insert(threat);
		}

		int nullCount = currentCount - threats.Count();

		if (nullCount > 0)
		{
		#ifdef DIAG_DEVELOPER
			int farCount;
		#endif

			vector position = GetPosition();

			m_eAI_ThreatOverride.Clear();

			foreach (EntityAI candidate: threats)
			{
				if (Math.IsPointInCircle(position, 1000.0, candidate.GetPosition()))
					m_eAI_ThreatOverride[candidate] = true;
			#ifdef DIAG_DEVELOPER
				else
					farCount++;
			#endif
			}

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, this, string.Format("eAI_PurgeThreatOverride - purged %1 NULL and %2 far threat overrides in %3 ms", nullCount, farCount, timeIt.GetElapsedMS()));
		#endif
		}
	}

	bool eAI_HasLOS()
	{
		return m_eAI_HasLOS;
	}

	/**
	 * @brief Return whether or not we have LOS to this target
	 * 
	 * @note WARNING: When not passing in current target, returns cached (last calculated) LOS info which may be stale!
	 * Use with care!
	 */
	bool eAI_HasLOS(eAITargetInformation info)
	{
		if (!info.IsEntity())
			return false;

		eAITarget target = info.GetTargetForAI(this);
		if (target)
			return target.m_LOS;

		return false;
	}

	void eAI_SetIsFightingFSM(bool state)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "SetIsFightingFSM " + state);
	#endif
		m_eAI_IsFightingFSM = state;

		eAITarget target = GetTarget();
		if (!state || !target || !target.IsItem())
		{
			GetGroup().SetIsInCombat(state);

			eAI_SetShouldTakeCover(state);
		}

		if (m_eAI_IsFightingFSM && GetExpansionSettings().GetAI().MemeLevel > 9000)
			eAI_PlayRandomLoveSound();
	}

	void eAI_SetShouldTakeCover(bool state)
	{
		if (state)
		{
			m_eAI_ShouldTakeCover = true;
			m_eAI_UpdatePotentialCoverObjects = true;
		}
		else
		{
			m_eAI_ShouldTakeCover = false;
		}
	}

	void eAI_LeaveCover()
	{
		if (m_eAI_CurrentCoverObject)
		{
			s_eAI_TakenCoverObjects.RemoveItem(m_eAI_CurrentCoverObject);
			m_eAI_CurrentCoverObject = null;
		}
	}

	void eAI_PlayRandomLoveSound()
	{
		if (Math.RandomInt(0, 2))
			s_eAI_LoveSound01_SoundSet.Play(this);
		else
			s_eAI_LoveSound02_SoundSet.Play(this);
	}

	override void OnScheduledTick(float deltaTime)
	{
	//#ifdef EXTRACE_DIAG
		//auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick");
	//#endif

		if (!IsPlayerSelected() || !IsAlive())
			return;

	#ifdef EXTRACE_DIAG
		auto trace2 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick(2) -> ModifiersManager::OnScheduledTick");
	#endif

		if (m_ModifiersManager)
			m_ModifiersManager.OnScheduledTick(deltaTime);

	#ifdef EXTRACE_DIAG
		trace2 = null;
	#endif

		//if (m_NotifiersManager)
			//m_NotifiersManager.OnScheduledTick();
		//! These send RPC to identity which is NULL for AI and thus affects all actual players HUDs (shows AI values instead of player)
		// if( m_TrasferValues )
			//m_TrasferValues.OnScheduledTick(deltaTime);
		// if( m_VirtualHud )
			//m_VirtualHud.OnScheduledTick();

	//#ifdef EXTRACE_DIAG
		//auto trace3 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick(3) -> BleedingSourcesManagerServer::OnTick");
	//#endif

		if (GetBleedingManagerServer())
			GetBleedingManagerServer().OnTick(deltaTime);

	//#ifdef EXTRACE_DIAG
		//trace3 = null;
	//#endif

		if (m_Environment)
		{
		#ifdef EXTRACE_DIAG
			auto trace4 = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick(4) -> Environment::Update");
		#endif

			m_Environment.Update(deltaTime);

		#ifdef EXTRACE_DIAG
			trace4 = null;
		#endif
		}

		// Check if electric device needs to be unplugged
		ItemBase heldItem = GetItemInHands();
		if (heldItem && heldItem.HasEnergyManager() && heldItem.GetCompEM().IsPlugged())
		{
			// Now we know we are working with an electric device which is plugged into a power source.
			EntityAI placed_entity = heldItem;

			// Unplug the device when the player is too far from the power source.
			placed_entity.GetCompEM().UpdatePlugState();
		}

		m_eAI_UpdateVisibilityTime += deltaTime;
		if (m_eAI_UpdateVisibilityTime > m_eAI_UpdateVisibilityTimeThreshold)
		{
			m_eAI_UpdateVisibilityTime = 0;
			Expansion_UpdateVisibility();
		}

	}

	// We should integrate this into ReloadWeapon
	void ReloadWeaponAI(EntityAI weapon, EntityAI magazine)
	{
#ifdef EXTRACE
		EXTrace trace;
#endif

		m_eAI_PositionOverrideTimeout = 0;  //! Reset so we prioritize taking cover during reload

		// CF_Log.Debug(this.ToString() + "(DayZPlayerInstanceType." + GetInstanceType().ToString() + ") is trying to reload " + magazine.ToString() + " into " + weapon.ToString());
		eAIActionManager mngr_ai;
		CastTo(mngr_ai, GetActionManager());

		if (mngr_ai && FirearmActionLoadMultiBulletRadial.Cast(mngr_ai.GetRunningAction()))
		{
#ifdef EXTRACE
			trace = EXTrace.Start0(EXTrace.AI, this, "Interrupting current reload action for " + weapon);
#endif
			mngr_ai.Interrupt();
		}
		else if (GetHumanInventory().GetEntityInHands() != magazine)
		{
			Weapon_Base wpn;
			Magazine mag;
			Class.CastTo(wpn, weapon);
			Class.CastTo(mag, magazine);
			int mi = wpn.GetCurrentMuzzle();
			Magazine currentMag = wpn.GetMagazine(mi);
			EXTrace.Print(EXTrace.AI, this, "ReloadWeaponAI - wpn " + wpn + " attached mag " + currentMag + " internal mag cartridge count " + wpn.GetInternalMagazineCartridgeCount(mi) + " - reload from " + mag);
			

			if (mag && mag.GetAmmoCount() < mag.GetAmmoMax() && eAI_IsTargetUnlimitedReload())
			{
				mag.ServerSetAmmoMax();
				mag.SetSynchDirty();
			}

			if (GetWeaponManager().CanUnjam(wpn))
			{
#ifdef EXTRACE
				//! NEVER unjam while reloading, unjamming should ALWAYS be handled by unjamming state in FSM so that weapon manager timeout can be dealt with
				trace = EXTrace.Start0(EXTrace.AI, this, "Not reloading jammed " + wpn);
#endif
				return;
			}
			else if (wpn.IsChamberFiredOut(mi) && wpn.GetInternalMagazineCartridgeCount(mi) > 0 && GetWeaponManager().CanEjectBullet(wpn))
			{
#ifdef EXTRACE
				trace = EXTrace.Start0(EXTrace.AI, this, "Ejecting bullet and chambering from internal mag " + wpn);
#endif
				GetWeaponManager().EjectBullet();
				return;
			}
			else if (GetWeaponManager().CanAttachMagazine(wpn, mag))
			{
#ifdef EXTRACE
				trace = EXTrace.Start0(EXTrace.AI, this, "Attaching mag to " + wpn + " " + mag);
#endif
				GetWeaponManager().AttachMagazine(mag);
				return;
			}
			else if (GetWeaponManager().CanSwapMagazine(wpn, mag))
			{
#ifdef EXTRACE
				trace = EXTrace.Start0(EXTrace.AI, this, "Swapping mag " + wpn + " " + mag);
#endif
				GetWeaponManager().SwapMagazine(mag);
				return;
			}
			else if (mag && currentMag && eAI_ShouldDetachMag(currentMag, mag) && GetWeaponManager().CanDetachMagazine(wpn, currentMag))
			{
				InventoryLocation il = new InventoryLocation;

				if (mag.IsAmmoPile() && eAI_GetMagTempDetachLocation(currentMag, il))
				{
#ifdef EXTRACE
					trace = EXTrace.Start0(EXTrace.AI, this, "Detaching mag " + wpn + " " + currentMag);
#endif

					//! TODO: Proper fill up of current mag from ammo pile after detach (put gun away, take mag to hand, fill mag, reattach to wpn)
					eAI_FillMag(currentMag, mag, true);

					GetWeaponManager().DetachMagazine(il);

					//! Force re-evaluation of mag to use for reloading
					typename removeType = wpn.Type();
					EXTrace.Print(EXTrace.AI, this, "ReloadWeaponAI - removing " + removeType + " " + m_eAI_EvaluatedFirearmTypes[removeType]);
					m_eAI_EvaluatedFirearmTypes.Remove(removeType);
					return;
				}
				else if (!mag.IsAmmoPile())
				{
#ifdef EXTRACE
					trace = EXTrace.Start0(EXTrace.AI, this, "Attached mag doesn't fit in inventory, dropping " + wpn + " " + currentMag);
#endif

					//! Could also use PredictiveDropEntity, but let's be explicit for maximum control
					vector m4[4];
					Math3D.MatrixIdentity4(m4);
					
					//! We don't care if a valid transform couldn't be found, we just want to preferably use it instead of placing on the player
					GameInventory.PrepareDropEntityPos(this, currentMag, m4, false, GameConstants.INVENTORY_ENTITY_DROP_OVERLAP_DEPTH);
					il.SetGround(currentMag, m4);

					GetWeaponManager().DetachMagazine(il);
					return;
				}
				else
				{
#ifdef EXTRACE
					trace = EXTrace.Start0(EXTrace.AI, this, "Attached mag doesn't fit in inventory " + wpn + " " + currentMag);
#endif

					//! Inventory is full, drop sth we don't need
					foreach (Magazine othermag: m_eAI_Magazines)
					{
						if (!othermag)
							continue;

						if (!eAI_HasWeaponForMagazine(othermag))
						{
							eAI_DropItemImpl(othermag);
							break;
						}
					}
				}

				//! If mag to reload from is ammo pile but current mag doesn't fit in inventory,
				//! we fall through here to load single bullet or eject casing
			}

			if (GetWeaponManager().CanLoadBullet(wpn, mag))
			{
#ifdef EXTRACE
				trace = EXTrace.Start0(EXTrace.AI, this, "Loading bullet " + wpn + " " + mag);
#endif
				// GetWeaponManager().LoadMultiBullet(mag);

				if (mag.IsAmmoPile() && eAI_IsTargetUnlimitedReload() && mag.GetAmmoMax() == 1)
					GetInventory().CreateInInventory(mag.GetType());

				ActionTarget atrg = new ActionTarget(mag, this, -1, vector.Zero, -1.0);
				if (mngr_ai && !mngr_ai.GetRunningAction() && mngr_ai.GetAction(FirearmActionLoadMultiBulletRadial).Can(this, atrg, wpn))
					mngr_ai.PerformActionStart(mngr_ai.GetAction(FirearmActionLoadMultiBulletRadial), atrg, wpn);
			}
			else if ((wpn.IsChamberEmpty(mi) || wpn.IsChamberFiredOut(mi)) && GetWeaponManager().CanEjectBullet(wpn))
			{
#ifdef EXTRACE
				//! @note WeaponManager::EjectBullet also deals with loading bullet from attached or internal mag into chamber
				trace = EXTrace.Start0(EXTrace.AI, this, "Ejecting bullet/cycling action " + wpn + " chamber empty? " + wpn.IsChamberEmpty(mi) + " fired out? " + wpn.IsChamberFiredOut(mi));
#endif
				GetWeaponManager().EjectBullet();
			}
			else
			{
				if (mag)
					EXError.Error(this, "Can't reload " + wpn + " from " + mag + " (" + mag.GetType() + ") - not implemented");
				else
					EXError.Error(this, "Can't reload " + wpn + " - not implemented");
			}
		}
	}

	bool eAI_GetMagTempDetachLocation(Magazine currentMag, InventoryLocation il)
	{
		if (GetInventory().FindFreeLocationFor(currentMag, FindInventoryLocationType.CARGO, il))
			return true;

		if (!GetInventory().FindAttachment(InventorySlots.LEFTHAND))
		{
			il.SetAttachment(this, currentMag, InventorySlots.LEFTHAND);
			return true;
		}

		return false;
	}

	bool eAI_ShouldDetachMag(Magazine currentMag, Magazine mag)
	{
		if (currentMag.GetAmmoCount() == 0)
			return true;

		if (mag != currentMag)
			return true;

		return false;
	}

	bool eAI_IsSafeToFillMag()
	{
		if (m_eAI_CurrentThreatToSelfActive >= 0.4)
			return false;

		if (g_Game.GetTime() - m_eAI_LastFireTime < Math.RandomFloat(m_eAI_TimeBetweenFiring, m_eAI_TimeBetweenFiringAndGettingUp))
			return false;

		return true;
	}

	/**
	 * @brief Fill mag from ammo source
	 * 
	 * @param magToFill
	 * @param ammoSource
	 * @param deleteSourceIfDepleted If ammo source is depleted during this operation, it is deleted
	 * 
	 * @return number of transferred cartridges
	 */
	int eAI_FillMag(Magazine magToFill, Magazine ammoSource, bool deleteSourceIfDepleted = false)
	{
		int ammoCount = ammoSource.GetAmmoCount();
		float totalDamage;
		int reloadAmmo = MiscGameplayFunctions.Expansion_TransferCartridges(ammoSource, magToFill, ammoCount, false, totalDamage);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "Filled mag " + magToFill + " with " + reloadAmmo + " cartridges from " + ammoSource);
	#endif

		ammoCount -= reloadAmmo;
		if (ammoCount == 0 && deleteSourceIfDepleted && ammoSource && !ammoSource.IsSetForDeletion())
			g_Game.ObjectDelete(ammoSource);

		return reloadAmmo;
	}

	/**
	 * @brief Fill any non-full mag in inventory which is compatible with weapon from ammo source
	 * 
	 * @param weapon
	 * @param ammoSource
	 * @param deleteSourceIfDepleted If ammo source is depleted during this operation, it is deleted
	 * 
	 * @note If ammo source is a mag and its health is higher and ammo max is higher or equal than mag in inv,
	 * the operation is swapped and ammo source is filled from mag
	 * 
	 * @return number of transferred cartridges
	 */
	int eAI_FillAnyCompatibleMag(Weapon_Base weapon, Magazine ammoSource, bool deleteSourceIfDepleted = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 
		
		int numberOfTransferredCartridges;

		foreach (Magazine magToFill: m_eAI_Magazines)
		{
			if (!magToFill)
				continue;

			if (magToFill.IsAmmoPile())
				continue;

			if (magToFill.IsFullQuantity())
				continue;

			if (!magToFill.IsCompatiableAmmo(ammoSource))
				continue;

			if (!ammoSource.IsAmmoPile() && ammoSource.GetHealthLevel() < magToFill.GetHealthLevel() && ammoSource.GetAmmoMax() >= magToFill.GetAmmoMax())
			{
				if (weapon.TestAttachMagazine(weapon.GetCurrentMuzzle(), ammoSource, false, true))
				{
					numberOfTransferredCartridges += eAI_FillMag(ammoSource, magToFill, deleteSourceIfDepleted);
					if (!magToFill || magToFill.IsSetForDeletion() || magToFill.GetAmmoCount() == 0)
						break;

					continue;
				}
			}

			if (weapon.TestAttachMagazine(weapon.GetCurrentMuzzle(), magToFill, false, true))
			{
				numberOfTransferredCartridges += eAI_FillMag(magToFill, ammoSource, deleteSourceIfDepleted);
				if (!ammoSource || ammoSource.IsSetForDeletion() || ammoSource.GetAmmoCount() == 0)
					break;
			}
		}

		return numberOfTransferredCartridges;
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

	void Expansion_OnAIUpdate()
	{
		Error("DEPRECATED");
	}

	void eAI_ForceVisibilityUpdate()
	{
		m_eAI_UpdateVisibilityTime = m_eAI_UpdateVisibilityTimeThreshold;
	}

	void Expansion_TryTurningOnAnyLightsOrNVG(out float nightVisibility, bool skipNonNVG = false, bool skipNVG = false)
	{
		EXError.Error(this, "DEPRECATED, use Expansion_TryTurningOnAnyLightsOrNVGEx(bool skipNonNVG, ...)");
		Expansion_TryTurningOnAnyLightsOrNVGEx(skipNonNVG, skipNVG);
	}

	void Expansion_TryTurningOnAnyLightsOrNVGEx(bool skipNonNVG = false, bool skipNVG = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
#endif 
		
		if (IsUnconscious() || IsRestrained())
			return;

		array<ItemBase> items = {};

		ItemBase headGear = GetItemOnHead();
		if (headGear)
			items.Insert(headGear);

		ItemBase eyeWear =  GetItemOnSlot("Eyewear");
		if (eyeWear)
			items.Insert(eyeWear);

		ItemBase handItem = GetItemInHands();
		if (handItem && !GetItemAccessor().IsItemInHandsHidden())
			items.Insert(handItem);

		array<ItemBase> optics = {};
		array<ItemBase> lights = {};

		foreach (ItemBase item: items)
		{
			ItemBase visEnh = item.Expansion_GetAnyLightOrNVG(skipNonNVG, skipNVG);

			if (visEnh)
			{
				//! Prefer night vision over lights
				//! @note both weapon optics as well as NVGoggles inherit from ItemOptics
				if (visEnh.IsInherited(ItemOptics))
					optics.Insert(visEnh);
				else
					lights.Insert(visEnh);
			}
		}

		m_Expansion_ActiveVisibilityEnhancers.Clear();

		foreach (ItemBase optic: optics)
		{
			//! If optic can be switched on, will add itself to vis enhancers in OnSwitchedOn, else have to add it explicitly
			if (!optic.Expansion_TryTurningOn())
				eAI_AddActiveVisibilityEnhancer(optic);
		}

		if (m_Expansion_ActiveVisibilityEnhancers.Count() == 0)
		{
			foreach (ItemBase light: lights)
			{
				//! If light can be switched on, will add itself to vis enhancers in OnSwitchedOn, else have to add it explicitly
				if (!light.Expansion_TryTurningOn())
					eAI_AddActiveVisibilityEnhancer(light);
			}
		}

		m_Expansion_TriedTurningOnVisibilityEnhancers = true;

		eAI_UpdateNightVisibility();
	}

	void Expansion_TryTurningOffAnyLightsOrNVG(bool skipNVG = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, eAIBase);
#endif 
		
		if (IsUnconscious() || IsRestrained())
			return;

		for (int i = m_Expansion_ActiveVisibilityEnhancers.Count() - 1; i >= 0; --i)
		{
			ItemBase visEnhActive = m_Expansion_ActiveVisibilityEnhancers[i];

			//! @note both weapon optics as well as NVGoggles inherit from ItemOptics
			if (!skipNVG || !visEnhActive.IsInherited(ItemOptics))
			{
				if (!visEnhActive.Expansion_TryTurningOff())
					eAI_RemoveActiveVisibilityEnhancerAtIndex(i);
			}
		}

		eAI_UpdateNightVisibility();
	}

	void eAI_AddActiveVisibilityEnhancer(ItemBase visEnh)
	{
		int index = m_Expansion_ActiveVisibilityEnhancers.Insert(visEnh);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "Added active visibility enhancer " + visEnh + " at index " + index);
	#endif
	}

	void eAI_RemoveActiveVisibilityEnhancer(ItemBase visEnh)
	{
		int index = m_Expansion_ActiveVisibilityEnhancers.Find(visEnh);

		if (index >= 0)
			eAI_RemoveActiveVisibilityEnhancerAtIndex(index);
	}

	void eAI_RemoveActiveVisibilityEnhancerAtIndex(int index)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, "Removing active visibility enhancer " + m_Expansion_ActiveVisibilityEnhancers[index] + " at index " + index);
	#endif

		m_Expansion_ActiveVisibilityEnhancers.Remove(index);
	}

	void eAI_UpdateNightVisibility()
	{
	#ifdef DIAG_DEVELOPER
		float nightVisibility = m_eAI_NightVisibility;
		bool isLit = m_eAI_IsLit;
	#endif

		m_eAI_NightVisibility = m_eAI_MinVisibility;
		m_eAI_IsLit = false;

		foreach (ItemBase visEnhActive: m_Expansion_ActiveVisibilityEnhancers)
		{
			float visEnhNightVisibility = visEnhActive.eAI_GetNightVisibility();

			if (visEnhNightVisibility > 0 && visEnhNightVisibility <= m_eAI_MinVisibility)
				visEnhNightVisibility += m_eAI_MinVisibility;

			if (visEnhNightVisibility > m_eAI_NightVisibility)
				m_eAI_NightVisibility = visEnhNightVisibility;

			if (!visEnhActive.IsInherited(ItemOptics))
				m_eAI_IsLit = true;
		}

	#ifdef DIAG_DEVELOPER
		if (m_eAI_NightVisibility != nightVisibility)
			EXTrace.Print(EXTrace.AI, this, "nighttime base visibility " + m_eAI_NightVisibility);

		if (m_eAI_IsLit != isLit)
			EXTrace.Print(EXTrace.AI, this, "lit " + m_eAI_IsLit);
	#endif
	}

	void Expansion_UpdateVisibility(bool force = false)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick(5) -> Expansion_UpdateVisibility");
#endif

		m_Expansion_DaylightVisibility = m_Environment.Expansion_GetDaylightVisibility();

		m_Environment.Expansion_GetWeatherVisibility(m_eAI_FogVisibility, m_eAI_OvercastVisibility, m_eAI_RainVisibility, m_eAI_SnowVisibility);

		if (!m_eAI_FogVisibility)
		{
			EXError.Error(this, "Fog visibility is zero!");
			m_eAI_FogVisibility = 1.0;
		}

		if (!m_eAI_OvercastVisibility)
		{
			EXError.Error(this, "Overcast visibility is zero!");
			m_eAI_OvercastVisibility = 1.0;
		}

		if (!m_eAI_RainVisibility)
		{
			EXError.Error(this, "Rain visibility is zero!");
			m_eAI_RainVisibility = 1.0;
		}

		if (!m_eAI_SnowVisibility)
		{
			EXError.Error(this, "Snow visibility is zero!");
			m_eAI_SnowVisibility = 1.0;
		}

		m_eAI_DynVolFogVisibility = 0.001 + m_Environment.Expansion_GetDynVolFogVisibility() * 0.999;

		float minVisibility;

		int lightingConfig = CfgGameplayHandler.GetLightingConfig();
		auto settings = GetExpansionSettings().GetAI();
		float minVisibilityMeters;
		if (settings.LightingConfigMinNightVisibilityMeters.Find(lightingConfig, minVisibilityMeters))
		{
			minVisibility = minVisibilityMeters * 0.001;

			if (minVisibility <= 0 || minVisibility > 1)
			{
				EXError.ErrorOnce(this, "Invalid LightingConfigMinNightVisibilityMeters value " + minVisibilityMeters);
				minVisibility = settings.GetDefaultMinVisibility(lightingConfig);
			}
		}
		else
		{
			minVisibility = settings.GetDefaultMinVisibility(lightingConfig);
		}

		m_eAI_MinVisibility = minVisibility;

		bool isDay;

		if (m_Expansion_DaylightVisibility < m_eAI_DayNightThreshold)
			isDay = false;
		else
			isDay = true;

		if (force || isDay != m_eAI_IsDay)
		{
			m_eAI_IsDay = isDay;
			m_Expansion_TriedTurningOnVisibilityEnhancers = false;
		}

		if (!isDay)
		{
			if (!m_Expansion_TriedTurningOnVisibilityEnhancers)
			{
				//! Try switching on any lights or NVG at night
				Expansion_TryTurningOnAnyLightsOrNVGEx();
				m_eAI_IsActiveVisibilityEnhancerTemporary = false;
			}
			else
			{
				eAI_UpdateNightVisibility();
			}

			if (m_eAI_NightVisibility > minVisibility)
				minVisibility = m_eAI_NightVisibility;
		}
		else if (m_Expansion_ActiveVisibilityEnhancers.Count())
		{
			Expansion_TryTurningOffAnyLightsOrNVG();
		}

		m_eAI_BaseVisibility = minVisibility + m_Expansion_DaylightVisibility * (1 - minVisibility);

		if (!m_eAI_BaseVisibility)
		{
			EXError.Error(this, "Base visibility is zero!");
			m_eAI_BaseVisibility = 1.0;
		}

		m_eAI_VisibilityLimit = m_eAI_ThreatDistanceLimit * 0.001;
		if (!m_eAI_VisibilityLimit)
		{
			EXError.Error(this, "Visibility limit is zero! Threat distance limit: " + m_eAI_ThreatDistanceLimit);
			m_eAI_VisibilityLimit = 1.0;
		}

		//! Limit visibility in contaminated areas due to gas clouds
		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_AREAEXPOSURE))
			m_eAI_ContaminatedAreaVisibility = 0.2;  //! 200 m
		else
			m_eAI_ContaminatedAreaVisibility = 1.0;

		//! Final visibility
		TFloatArray visibilities = {
			m_eAI_FogVisibility,
			m_eAI_OvercastVisibility,
			m_eAI_RainVisibility,
			m_eAI_SnowVisibility,
			m_eAI_DynVolFogVisibility,
			m_eAI_ContaminatedAreaVisibility,
			m_eAI_BaseVisibility,
			m_eAI_VisibilityLimit
		};

		m_Expansion_Visibility = ExpansionMath.Min(visibilities);

		m_Expansion_VisibilityDistThreshold = 900 * m_Expansion_Visibility;
	}

	float Expansion_GetVisibility(float distance)
	{
		EXError.ErrorOnce(this, "DEPRECATED, use eAI_GetVisibility");

		return eAI_GetVisibility(distance, GetTarget());
	}

	float eAI_GetThreatDistanceFactor(float distance)
	{
		float threatDistanceThreshold = 900;

		if (distance > threatDistanceThreshold)
			return ExpansionMath.PowerConversion(1100, threatDistanceThreshold, distance, 0.0, 1.0, 2.0);

		return 1.0;
	}

	float eAI_GetVisibility(float distance, eAITarget target)
	{
		if (distance > m_Expansion_VisibilityDistThreshold)
		{
			if (m_Expansion_Visibility <= m_eAI_NightVisibility)
			{
				if (target.IsLit())
					return ExpansionMath.PowerConversion(250, 200, distance, 0.0, 1.0, 2.0);
			}

			return ExpansionMath.PowerConversion(1100 * m_Expansion_Visibility, m_Expansion_VisibilityDistThreshold, distance, 0.0, 1.0, 2.0);
		}

		return 1.0;
	}

	float eAI_GetVisibilityDistance(float distance, eAITarget target)
	{
		if (distance > m_Expansion_VisibilityDistThreshold)
		{
			if (m_Expansion_Visibility <= m_eAI_NightVisibility)
			{
				if (target.IsLit())
					return Math.Min(distance, Math.Min(m_eAI_ThreatDistanceLimit, 250));
			}

			return Math.Min(distance, 1100 * m_Expansion_Visibility);
		}

		return distance;
	}

	float eAI_GetVisibilityDistanceLimit(PlayerBase player)
	{
		if (m_Expansion_Visibility <= m_eAI_NightVisibility)
		{
			if (player.eAI_IsLit())
				return Math.Min(m_eAI_ThreatDistanceLimit, 250);
		}

		return 1100 * m_Expansion_Visibility;
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

		//if (GetIdentity())
		//{
			//super.HandleWeapons(pDt, pInHands, pInputs, pExitIronSights);
			//return;
		//}

		GetDayZPlayerInventory().HandleWeaponEvents(pDt, pExitIronSights);
	}

	void eAI_HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(08) -> eAI_HandleWeapons");
#endif

		HumanCommandWeapons hcw = GetCommandModifier_Weapons();

		Weapon_Base weapon;
		Class.CastTo(weapon, pInHands);

		if (m_WeaponRaised != m_WeaponRaisedPrev)
		{
			m_WeaponRaisedPrev = m_WeaponRaised;
			m_WeaponRaisedTimer = 0.0;

			if (g_Game.IsServer())
			{
				AnimSetBool(m_ExpansionST.m_VAR_Raised, m_WeaponRaised);
				if (!m_WeaponRaised)
				{
					//AnimSetBool(m_ExpansionST.m_VAR_ADS, false);
					if (hcw)
						hcw.SetADS(false);
				}
			}
		}

		if (m_WeaponRaised || g_Game.IsClient())
		{
			if (g_Game.IsServer())
				m_WeaponRaisedTimer += pDt;

			vector aimTargetRelAngles = m_eAI_AimDirectionTarget_ModelSpace.VectorToAngles();

			bool instant;

			if (g_Game.IsServer() && m_eAI_AimDirectionPrev != vector.Zero)
			{
				auto cmd = GetCommand_Move();
				if (cmd)
				{
					//! Adjust aim when turning
					float diff = ExpansionMath.AngleDiff2(m_eAI_AimDirectionPrev.VectorToAngles()[0], GetAimDirectionTarget().VectorToAngles()[0]);
					float diffAbs = Math.AbsFloat(diff);
					if (diffAbs <= 10.0)
					{
						instant = true;
					}
				}
			}

			//! We want to interpolate rel angles for aiming! Otherwise, if the conversion to rel angles happens later,
			//! there will be a sudden jump in the unit's rotation between 180 and -180 due to the way the animation is set up
			aimTargetRelAngles[0] = ExpansionMath.RelAngle(aimTargetRelAngles[0]);
			aimTargetRelAngles[1] = ExpansionMath.RelAngle(aimTargetRelAngles[1]);

			aimTargetRelAngles[1] = Math.Clamp(aimTargetRelAngles[1], -85.0, 85.0);  //! Valid range is [-85, 85]

			//TODO: quaternion slerp instead for better, accurate results
			m_eAI_AimRelAngles = ExpansionMath.InterpolateAngles(m_eAI_AimRelAngles, aimTargetRelAngles, pDt, Math.RandomFloat(3.0, 5.0), Math.RandomFloat(1.0, 3.0));
			if (instant)
				m_eAI_AimRelAngles[0] = aimTargetRelAngles[0];

			GetAimingProfile().Update();
			vector direction = GetAimingProfile().GetAimDirection();
			vector orientation = direction.VectorToAngles();
			m_eAI_AimRelAngleLR = orientation[0];
			m_eAI_AimRelAngleUD = orientation[1];

		/*
		#ifndef SERVER
		#ifdef DIAG_DEVELOPER
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
				float distClamped = Math.Clamp(dist, 1.0, 360.0);

				float aimX = ExpansionMath.RelAngle(ExpansionMath.AbsAngle(m_eAI_AimRelAngles[0]) + (-15.0 / distClamped));
				float aimY = m_eAI_AimRelAngles[1];

				//! When leaning, aim animation direction needs to be adjusted for character rotation
				if (m_eAI_Lean)
					aimY += Math.Clamp(aimX, -45.0, 45.0) * Math.Clamp(m_eAI_Lean, -1.0, 1.0);

				AnimSetFloat(m_ExpansionST.m_VAR_AimX, aimX);
				AnimSetFloat(m_ExpansionST.m_VAR_AimY, aimY);

				bool ads;
				if (weapon && !GetWeaponManager().IsRunning() && Math.AbsFloat(ExpansionMath.AngleDiff2(m_eAI_AimRelAngles[0], aimTargetRelAngles[0])) < 45.0)
				{
					ItemOptics optics = weapon.GetAttachedOptics();

					float zeroingDistanceZoomMin;

					if (optics)
						zeroingDistanceZoomMin = optics.GetZeroingDistanceZoomMin();
					else
						zeroingDistanceZoomMin = weapon.GetZeroingDistanceZoomMin(weapon.GetCurrentMuzzle());

					if (zeroingDistanceZoomMin * 0.3 < dist)
						ads = true;
				}

				//AnimSetBool(m_ExpansionST.m_VAR_ADS, ads);
				if (hcw)
					hcw.SetADS(ads);
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

		m_eAI_AimDirectionPrev = GetAimDirection();
	}

	override void HandleOptic(notnull ItemOptics optic, bool inHands, HumanInputController pInputs, out bool pExitOptics)
	{
		//if (GetIdentity())
			//super.HandleOptic(optic, inHands, pInputs, pExitOptics);
	}

	override void HandleADS()
	{
		//if (GetIdentity())
			//super.HandleADS();
	}

	override void HandleView()
	{
		//if (GetIdentity())
			//super.HandleView();
	}

	override bool HandleDeath(int pCurrentCommandID)
	{
		if (super.HandleDeath(pCurrentCommandID))
		{
			m_eAI_DeathHandled = true;
			return true;
		}

		return false;
	}

	override bool HandleDamageHit(int pCurrentCommandID)
	{
		if (super.HandleDamageHit(pCurrentCommandID))
		{
			m_eAI_SkipScript = true;
			return true;
		}

		return false;
	}

	// As with many things we do, this is an almagomation of the client and server code
	override void CheckLiftWeapon()
	{
		//if (GetIdentity())
		//{
			//super.CheckLiftWeapon();
			//return;
		//}

		if (!g_Game.IsServer())
			return;

		Weapon_Base weap;
		if (Weapon_Base.CastTo(weap, GetItemInHands()))
		{
			m_LiftWeapon_player = weap.LiftWeaponCheck(this);
		}
	}

	override bool CanConsumeStamina(EStaminaConsumers consumer)
	{
		switch (consumer)
		{
			//! Don't consume stamina for melee against creatures/Zs
			case EStaminaConsumers.MELEE_HEAVY:
			case EStaminaConsumers.MELEE_EVADE:
				eAITarget target = GetTarget();  //! The current prioritized target
				if (target && (target.IsCreature() || target.IsZombie()))
					return true;
				break;
		}

		return super.CanConsumeStamina(consumer);
	}

	//! @note this is marked as deprecated in vanilla but still used by vanilla melee fight logic...
	override void DepleteStamina(EStaminaModifiers modifier, float dT = -1)
	{
		switch (modifier)
		{
			//! Don't consume stamina for melee against creatures/Zs
			case EStaminaModifiers.MELEE_LIGHT:
			case EStaminaModifiers.MELEE_HEAVY:
			case EStaminaModifiers.MELEE_EVADE:
				EntityAI targetEntity = m_MeleeCombat.GetTargetEntity();  //! The actual melee target
				if (targetEntity && targetEntity.IsDayZCreature())
					return;
				break;
		}

		super.DepleteStamina(modifier, dT);
	}

	bool CanRaiseWeapon()
	{
		if (IsClimbing() || IsFalling() || IsSwimming() || m_eAI_IsOnLadder)
			return false;

		if (eAI_IsSideSteppingVehicle())
			return false;

		return true;
	}

	// @param true to put weapon up, false to lower
	void RaiseWeapon(bool up = true)
	{
		m_WeaponRaised = up;
		if (!up)
			m_eAI_QueuedShots = 0;
	}

	void eAI_ResetRaised()
	{
		if (g_Game.IsServer())
		{
			m_WeaponRaised = false;
			m_WeaponRaisedPrev = false;
			m_WeaponRaisedTimer = 0.0;
			AnimSetBool(m_ExpansionST.m_VAR_Raised, false);
		}
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
		for (int i = m_eAI_InteractingPlayers.Count() - 1; i >= 0; i--)
		{
			Man player = m_eAI_InteractingPlayers[i];
			if (player && player.IsAlive() && !player.IsUnconscious())
				return true;
			else
				m_eAI_InteractingPlayers.Remove(i);
		}

		return false;
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(eAIActionTakeItemToHands, InputActionMap);
		AddAction(eAIActionTakeItem, InputActionMap);
		AddAction(eAIActionDropItem, InputActionMap);
		AddAction(eAIActionOpenDoorsDummy, InputActionMap);
		AddAction(eAIActionCloseDoorsDummy, InputActionMap);
	}

	ActionBase StartAction(typename actionType, ActionTarget target, ItemBase mainItem = null)
	{
		ActionBase action = m_eActionManager.GetAction(actionType);

		if (!mainItem)
			mainItem = GetItemInHands();

		m_eActionManager.PerformActionStart(action, target, mainItem);

		m_eAI_PositionOverrideTimeout = 0;  //! Reset so we prioritize taking cover during actions

		return action;
	}

	ActionBase StartActionObject(typename actionType, Object target, ItemBase mainItem = null)
	{
		ActionTarget actionTgt = new ActionTarget(target, null, -1, vector.Zero, -1.0);
		return StartAction(actionType, actionTgt, mainItem);
	}

	override void SetRestrained(bool is_restrained)
	{
		//! Set this flag to signify if we're not fake restrained (server only)
		m_eAI_IsRestrained = is_restrained;

		if (is_restrained)
		{
			m_eAI_IsInventoryVisible = false;

			if (GetEmoteManager().IsEmotePlaying())
				GetEmoteManager().ServerRequestEmoteCancel();
		}

		super.SetRestrained(is_restrained);

		m_eAI_InteractingPlayers.Clear();
	}
	
	override bool IsRestrained()
	{
		if (!super.IsRestrained())
			return false;

		if (!eAI_IsInventoryVisible())
			return false;  //! Don't show "unrestrain" action if we're fake restrained while a player accesses AI inventory

		return true;
	}

	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;

		if (!eAI_IsInventoryVisible())
			return false;

		return m_Expansion_CanBeLooted;
	}
	
	void eAI_SetIsInventoryVisible(bool visible, notnull PlayerBase player)
	{
		if (g_Game.IsClient())
		{
			auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_eAI_SetIsInventoryVisible");
			rpc.Write(visible);
			rpc.Expansion_Send(true);
		}
		else
		{
			if (!visible)
				eAI_RemoveInteractingPlayer(player);

			if (m_eAI_InteractingPlayers.Count() == 0)
			{
				//! Fake restrain to make inventory accessible
				//! Hacky, but works
				m_IsRestrained = visible;
				m_eAI_IsInventoryVisible = visible;
				SetSynchDirty();
			}

			if (visible)
				eAI_AddInteractingPlayer(player);
		}
	}

	bool eAI_IsInventoryVisible()
	{
		if (!g_Game.IsDedicatedServer() && m_eAI_IsInventoryVisible)
		{
			//! Prevent other players not in group from seeing AI inventory
			PlayerBase player;
			if (Class.CastTo(player, g_Game.GetPlayer()) && player.GetGroup() != GetGroup())
				return false;
		}

		return true;
	}

	void RPC_eAI_SetIsInventoryVisible(PlayerIdentity sender, ParamsReadContext ctx)
	{
		PlayerBase player;
		bool visible;
		if (Class.CastTo(player, sender.GetPlayer()) && player.GetGroup() == GetGroup() && ctx.Read(visible))
		{
			if (!m_eAI_IsRestrained && m_Expansion_CanBeLooted)
				eAI_SetIsInventoryVisible(visible, player);
		}
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

	vector GetAimDirectionTarget()
	{
		return m_eAI_AimDirectionTarget_ModelSpace.Multiply3(m_ExTransformPlayer);
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

	vector Expansion_GetAimDirectionClient()
	{
		return super.Expansion_GetAimDirection();
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
		if (!IsAlive())
		{
			return false;
		}
		
		GetMovementState(m_MovementState);

		float angle;
		
		switch (m_MovementState.m_CommandTypeId)
		{
			//! Vanilla
			case DayZPlayerConstants.COMMANDID_VEHICLE:
				HumanCommandVehicle hmv = GetCommand_Vehicle();
				if (hmv.IsGettingOut() || hmv.IsGettingIn())
				{
					return DayZPlayerImplementHeading.NoHeading(pDt, pModel, m_fLastHeadingDiff);
				}

				m_fLastHeadingDiff = 0;
				return false;

			case DayZPlayerConstants.COMMANDID_UNCONSCIOUS:
				//! Vanilla
				HumanCommandUnconscious	hcu = GetCommand_Unconscious();
				if (!hcu.IsWakingUp())
				{
					m_fLastHeadingDiff = 0;
					//pModel.m_iCamMode = DayZPlayerConstants.CAMERAMODE_HEAD;
					return false;
				}
				//! Needed to prevent AI spinning in place while waking up in vehicle
				else if (m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE)
				{
					m_fLastHeadingDiff = 0;
					return false;
				}

				break;

			//case DayZPlayerConstants.COMMANDID_ACTION:
			//! Vanilla
			case DayZPlayerConstants.COMMANDID_CLIMB:
			case DayZPlayerConstants.COMMANDID_LADDER:
				m_fLastHeadingDiff = 0;
				return false;

			case DayZPlayerConstants.COMMANDID_MOVE:
				//! Vanilla
				HumanCommandMove hcm = GetCommand_Move();
				if (hcm.IsStandingFromBack())
				{
					m_fLastHeadingDiff = 0;
					return false;
				}

				//! Needed to place AI in correct orientation while moving
				//if (!PhysicsIsFalling(false))
				//{
					m_fLastHeadingDiff = 0;
					if (Expansion_GetParent())
						return false;
					angle = GetOrientation()[0] * Math.DEG2RAD;
					pModel.m_fHeadingAngle = angle;
					pModel.m_fOrientationAngle = angle;
					return true;
				//}
			//#ifdef DIAG_DEVELOPER
				//else
				//{
					//EXTrace.Print(EXTrace.AI, this, "HeadingModel PhysicsIsFalling " + Expansion_CommandIDToString(m_MovementState.m_CommandTypeId) + " headingAngle=" + pModel.m_fHeadingAngle + " orientationAngle=" + pModel.m_fOrientationAngle);
				//}
			//#endif

				//break;

			case DayZPlayerConstants.COMMANDID_SWIM:
				//! Needed to place AI in correct orientation while moving
				if (m_MovementState.m_iMovement > 0)
				{
					angle = GetOrientation()[0] * Math.DEG2RAD;
					pModel.m_fHeadingAngle = angle;
					pModel.m_fOrientationAngle = angle;
					m_fLastHeadingDiff = 0;
					return true;
				}

				//! Needed to prevent AI from spinning in place after going down ladder
				pModel.m_fOrientationAngle = pModel.m_fHeadingAngle;
				SetOrientation(Vector(pModel.m_fHeadingAngle * Math.RAD2DEG, 0, 0));
				m_fLastHeadingDiff = 0;
				return true;

			//! Needed to prevent AI from spinning in place after going down ladder
			case DayZPlayerConstants.COMMANDID_MELEE:
			case DayZPlayerConstants.COMMANDID_MELEE2:
			case DayZPlayerConstants.COMMANDID_FALL:
				pModel.m_fOrientationAngle = pModel.m_fHeadingAngle;
				SetOrientation(Vector(pModel.m_fHeadingAngle * Math.RAD2DEG, 0, 0));
				m_fLastHeadingDiff = 0;
				return false;
		}
		
	/*
#ifdef DEVELOPER
		int	actMenuValue = DiagMenu.GetEngineValue(DayZPlayerConstants.DEBUG_TURNSLIDE);
		if (actMenuValue != 0)
		{
			//! this clamps heading 
			return DayZPlayerImplementHeading.ClampHeading(pDt, pModel, m_fLastHeadingDiff);
		}
#endif
		
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		
		HumanItemAccessor hia = GetItemAccessor();
		HumanItemBehaviorCfg hibcfg = hia.GetItemInHandsBehaviourCfg();
		if (hibcfg && hibcfg.m_StanceRotation[m_MovementState.m_iStanceIdx] == DayZPlayerConstants.ROTATION_DISABLE)
		{
			return DayZPlayerImplementHeading.NoHeading(pDt, pModel, m_fLastHeadingDiff);
		}
		if (IsUnconscious() || (GetCommand_Move() && GetCommand_Move().IsLeavingUncon()))
		{
			return DayZPlayerImplementHeading.NoHeading(pDt, pModel, m_fLastHeadingDiff);
		}

		return DayZPlayerImplementHeading.RotateOrient(pDt, pModel, m_fLastHeadingDiff);
	*/

		return DayZPlayerImplementHeading.NoHeading(pDt, pModel, m_fLastHeadingDiff);
	}

	//! Hack to make AI responsive again when weapon manager or action bugs out.
	//! Starts the damage hit animation (fullbody forward) w/o actually applying any damage, afterwards it should be fine again.
	//! @note (for documentation purposes) other tried ands tested workarounds that are less desirable:
	//! - Send AI uncon and wake it up again (more awkward and takes more time)
	//! - Let AI fall a short distance (unreliable in fixing the bug)
	void eAI_Unbug(string what)
	{
		string pos = ExpansionStatic.VectorToString(GetPosition());
		string msg = "Action failed for " + Debug.GetDebugName(this) + " (pos=" + pos + ") - " + what + " - item in hands " + ExpansionStatic.GetDebugInfo(GetItemInHands());
		EXTrace.Print(true, this, msg);

#ifdef DIAG_DEVELOPER
		msg.Replace("<", "‹");
		msg.Replace(">", "›");
		ExpansionNotification("ACTION FAILED", msg).Error();
#endif

		EXTrace.Print(true, this, "Applying SMACK OF GOD (='-')-o )'~')");
		StartCommand_Damage(1, 180);
	}

	override void OnUnconsciousStart()
	{
		eAI_ResetRaised();

		if (!IsRestrained())
			eAI_DropItemInHandsImpl(false, false, false);

		super.OnUnconsciousStart();

		//! Needed because vanilla only checks for INSTANCETYPE_SERVER

		//! <--------------------------------------------------------------------------------

		SetSynchDirty();

		if (m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			if (m_TransportCache)
				m_TransportCache.MarkCrewMemberUnconscious(m_TransportCache.CrewMemberIndex(this));
		}

		if (m_AdminLog)
			m_AdminLog.UnconStart(this);

		if (GetMeleeFightLogic())
			GetMeleeFightLogic().SetBlock(false);

		//! -------------------------------------------------------------------------------/>
	}

	override void OnUnconsciousStop(int pCurrentCommandID)
	{
		super.OnUnconsciousStop(pCurrentCommandID);

		//! Needed because vanilla only checks for INSTANCETYPE_SERVER
		if (m_AdminLog)
			m_AdminLog.UnconStop(this);
	}

	bool eAI_DropItemInHands(bool useAction = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 
		
		ItemBase itemInHands = GetItemInHands();
		if (itemInHands)
			return eAI_DropItem(itemInHands, useAction);

		return false;
	}

	bool eAI_DropItemInHandsImpl(bool overrideThreat = false, bool switchOff = true, bool remember = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 

		ItemBase itemInHands = GetItemInHands();
		if (itemInHands)
			return eAI_DropItemImpl(itemInHands, overrideThreat, switchOff, remember);

		return false;
	}

	bool eAI_DropItem(ItemBase item, bool useAction = true, bool switchOff = true, bool remember = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		if (!useAction || item != GetItemInHands())
			return eAI_DropItemImpl(item, eAI_GetThreatOverride(item), switchOff, remember);
		else if (StartAction(eAIActionDropItem, null, item))
			return true;

		return false;
	}

	bool eAI_DropItemImpl(ItemBase item, bool overrideThreat = false, bool switchOff = true, bool remember = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		InventoryLocation il_dst = new InventoryLocation();

		GameInventory.SetGroundPosByOwner(this, item, il_dst);

		if (switchOff)
			item.Expansion_TryTurningOffAnyLightOrNVG();

		//! 1) Remove from active visibility enhancers if present will happen in eAI_RemoveItem when item leaves inventory

		bool result;

		if (item != GetItemInHands())
		{
			if (eAI_TakeItemToLocation(item, il_dst))
			{
				result = true;

				if (Class.CastTo(m_eAI_LastDroppedItem, item))
					eAI_ThreatOverride(m_eAI_LastDroppedItem, overrideThreat);

				item.SetLifetime(300);
			}
		}
		else
		{
			EntityAI dst = Expansion_CloneItemToLocation(item, il_dst);
			if (dst)
			{
				result = true;

				if (Class.CastTo(m_eAI_LastDroppedItem, dst))
					eAI_ThreatOverride(m_eAI_LastDroppedItem, overrideThreat);

				dst.SetLifetime(300);
			}
		}

		//! 2) Update visibility regardless of result because of 1)
		Expansion_UpdateVisibility(true);

		if (result && remember)
			eAI_RememberRecentlyDroppedItem(m_eAI_LastDroppedItem);

		return result;
	}

	bool eAI_TakeItemToHands(ItemBase item, bool useAction = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		if (item.GetHierarchyRootPlayer() == this || !useAction)
			return eAI_TakeItemToHandsImpl(item);
		else if (StartActionObject(eAIActionTakeItemToHands, item))
			return true;

		return false;
	}

	bool eAI_TakeItemToHandsImpl(ItemBase item)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		if (!item)
			return false;

		if (item.GetHierarchyRoot() != this)  //! Item not in AI inventory, so we are looting
			eAI_UpdateVisitedBuildings();

		EntityAI hands = GetHumanInventory().GetEntityInHands();
		if (hands)
		{
		#ifdef DIAG_DEVELOPER
			EXError.Error(this, string.Format("Cannot take %1 to hands - current hands entity: %2", item, hands));
		#endif
			return false;
		}

		InventoryLocation il_dst = new InventoryLocation;
		il_dst.SetHands(this, item);

		if (il_dst.IsValid())
		{
			if (!item.IsTakeable() || item.Expansion_IsInventoryLocked())
				item.Expansion_SetLootable(true);

		#ifdef EXPANSIONMODAI_TAKETOLOCATION
			hands = eAI_TakeItemToLocation(item, il_dst);
		#else
			hands = Expansion_CloneItemToLocation(item, il_dst);
		#endif
			if (hands)
			{
				if (m_eAI_Targets.Count() > 1 && GetExpansionSettings().GetAI().MemeLevel > 9000)
					eAI_PlayRandomLoveSound();

				m_eAI_PositionOverrideTimeout = 0;  //! Reset so we prioritize taking cover during actions

				Expansion_UpdateVisibility(true);

				return true;
			}
		}

	#ifdef DIAG_DEVELOPER
		InventoryLocation il_src = new InventoryLocation;
		item.GetInventory().GetCurrentInventoryLocation(il_src);
		EXError.Error(this, string.Format("Cannot take %1 to hands - src %2 dst %3", item, ExpansionStatic.DumpToString(il_src), ExpansionStatic.DumpToString(il_dst)));
	#endif

		//! If we couldn't take, make sure we don't try again
		if (item && !item.IsSetForDeletion())
			eAI_ThreatOverride(item, true);

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
			flags = FindInventoryLocationType.ATTACHMENT | FindInventoryLocationType.CARGO;

		il_dst = new InventoryLocation();

		if (GetInventory().FindFreeLocationFor(item, flags, il_dst))
		{
			if (GameInventory.LocationCanAddEntity(il_dst))
				return true;

			il_dst.Reset();  //! Invalidate location
		}

		return false;
	}

	bool eAI_TakeItemToInventory(ItemBase item, bool useAction = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		if (!item)
			return false;

		if (item == GetItemInHands() || !useAction)
		{
			if (eAI_TakeItemToInventoryImpl(item))
				return true;
		}
		else if (StartActionObject(eAIActionTakeItem, item))
		{
			return true;
		}

		return false;
	}

	bool eAI_CanLootWeapon(ItemBase weapon, eAIFaction faction)
	{
		if (weapon.m_Expansion_PreviousOwner == this)
			return true;

		if (faction.IsWeaponPickupEnabled())
		{
			if (weapon.ShootsExplosiveAmmo())
			{
				if (m_eAI_LootingBehavior & eAILootingBehavior.WEAPONS_LAUNCHERS)
					return true;
			}
			else if (m_eAI_LootingBehavior & eAILootingBehavior.WEAPONS_FIREARMS)
			{
				return true;
			}
		}

		return false;
	}

	bool eAI_CanLootMeleeWeapon(ItemBase weapon, eAIFaction faction)
	{
		if (weapon.m_Expansion_PreviousOwner == this)
			return true;

		if (faction.IsWeaponPickupEnabled() && (m_eAI_LootingBehavior & eAILootingBehavior.WEAPONS_MELEE))
			return true;

		return false;
	}

	bool eAI_ShouldPickupBandage(ItemBase item)
	{
		int bandages = m_eAI_Bandages.Count();

		if ((IsBleeding() && bandages == 0) || (!IsBleeding() && (m_eAI_LootingBehavior & eAILootingBehavior.BANDAGES) && bandages < 3))
			return true;

		return false;
	}

	bool eAI_ShouldTreatItemAsBandage(ItemBase item)
	{
		if (item.Expansion_CanBeUsedToBandage())
		{
			if ((m_eAI_LootingBehavior & eAILootingBehavior.CLOTHING) == 0)
				return true;

			if ((m_eAI_LootingBehavior & eAILootingBehavior.BANDAGES) && (m_eAI_Bandages.Count() < 3 || item == GetItemInHands()))
				return true;
		}

		return false;
	}

	bool eAI_ShouldProcureFood()
	{
		if ((m_eAI_LootingBehavior & eAILootingBehavior.FOOD) && m_eAI_Food.Count() < m_eAI_MaxFoodCount)
			return true;

		return false;
	}

	bool eAI_TakeItemToInventoryImpl(ItemBase item, FindInventoryLocationType flags = 0, bool threatOverrideOnFailure = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + item);
#endif 

		if (!item)
			return false;

		if (item.GetHierarchyRoot() != this)  //! Item not in AI inventory, so we are looting
			eAI_UpdateVisitedBuildings();

		InventoryLocation il_dst;

		ItemBase currentlyWornGear;
		bool transferCargo;

		if (eAI_ShouldTreatItemAsBandage(item))
		{
			flags = FindInventoryLocationType.CARGO;
		}
		else if (item.IsClothing())
		{
			TStringArray inventorySlots = item.Expansion_GetInventorySlots();

			foreach (string slot: inventorySlots)
			{
				int slotId = InventorySlots.GetSlotIdFromString(slot);
				if (GetInventory().HasAttachmentSlot(slotId))
				{
					if (!eAI_ClothingLootingBehaviorCheck_Slot(slot, item))
						continue;

					if (!Class.CastTo(currentlyWornGear, GetInventory().FindAttachment(slotId)))
					{
						//! Found empty slot
						currentlyWornGear = null;  //! null any gear that was found in another slot
						il_dst = new InventoryLocation();
						il_dst.SetAttachment(this, item, slotId);
						break;
					}
					else if (!eAI_ClothingLootingBehaviorCheck_Selection(currentlyWornGear, item))
					{
						currentlyWornGear = null;
					}
				}
			}

			if (currentlyWornGear)
			{
				il_dst = new InventoryLocation();

				if (currentlyWornGear.GetInventory().GetCurrentInventoryLocation(il_dst) && eAI_DropItemImpl(currentlyWornGear, true) && CanReceiveAttachment(item, il_dst.GetSlot()))
				{
					//! Drop whole cargo of target item before taking
					item.Expansion_DropAllCargo();

					il_dst.SetItem(item);

					EXTrace.Print(EXTrace.AI, this, "Swapping " + currentlyWornGear + " for " + item);

					transferCargo = true;
				}
				else if (il_dst.IsValid())
				{
					il_dst.Reset();  //! Invalidate location
				}
			}
			else if (!il_dst)
			{
				eAI_ThreatOverride(item, true);
				return false;
			}
		}

		if (!il_dst)
			eAI_FindFreeInventoryLocationFor(item, flags, il_dst);

		bool result;

		if (il_dst.IsValid())
		{
			if (!item.IsTakeable() || item.Expansion_IsInventoryLocked())
				item.Expansion_SetLootable(true);

			item.Expansion_TryTurningOffAnyLightOrNVG();

			//! 1) Remove from active visibility enhancers if present
			eAI_RemoveActiveVisibilityEnhancer(item);

		#ifdef EXPANSIONMODAI_TAKETOLOCATION
			if (eAI_TakeItemToLocation(item, il_dst))
			{
				result = true;

				if (transferCargo && !MiscGameplayFunctions.Expansion_MoveCargo(currentlyWornGear, item))
					currentlyWornGear.Expansion_DropAllCargo(300);
			}
		#else
			EntityAI clone = Expansion_CloneItemToLocation(item, il_dst);
			if (clone)
			{
				result = true;

				if (transferCargo && !MiscGameplayFunctions.Expansion_MoveCargo(currentlyWornGear, clone))
					currentlyWornGear.Expansion_DropAllCargo(300);
			}
		#endif

			//! 2) Update visibility regardless of result because of 1)
			Expansion_UpdateVisibility(true);
		}

		if (!result)
		{
			//! If we couldn't take to inventory, make sure we don't try again
			if (threatOverrideOnFailure && !item.IsSetForDeletion())
				eAI_ThreatOverride(item, true);

			if (currentlyWornGear)
			{
				eAI_ThreatOverride(currentlyWornGear, false);  //! Enable taking what we dropped earlier
				eAI_ForgetRecentlyDroppedItem(currentlyWornGear);
			}
		}

		return result;
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
				if (m_WeaponManager.IsRunning())
					m_WeaponManager.OnWeaponActionEnd();  //! Prevent getting stuck in running state

				//! Forcing switch to HumanCommandMove before taking from hands,
				//! and hiding item in hands before, unbreaks hand anim state
				GetItemAccessor().HideItemInHands(true);
			}

			if (il_dst.GetType() == InventoryLocationType.HANDS)
			{
				//! Needs to be 'lootable' while in AI inventory, else AI won't be able to properly interact with item
				//! (e.g. won't be able to detach/attach mag to firearm)
				ExpansionItemBaseModule.SetLootable(item, true);
			}
		}

		if (g_Game.IsMultiplayer())
			g_Game.RemoteObjectTreeDelete(item);

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
					if (!m_Expansion_CanBeLooted && !itemBs.m_Expansion_IsOwnerPlayer)
					{
						ExpansionItemBaseModule.SetLootable(item, false);
						item.SetLifetimeMax(120);  //! Make sure it despawns quickly when left alone
					}
				}
			}
		}

		if (g_Game.IsMultiplayer())
			g_Game.RemoteObjectTreeCreate(item);

		return result;
	}

	bool eAI_TakeItemToInventoryDropShoulderImpl(ItemBase item, bool threatOverrideOnFailureToTakeToInv = true)
	{
		if (item.IsWeapon())
		{
			array<Weapon_Base> weapons;
			if (item.IsKindOf("Pistol_Base"))
				weapons = m_eAI_Handguns;
			else if (item.ShootsExplosiveAmmo())
				weapons = m_eAI_Launchers;
			else
				weapons = m_eAI_Firearms;

			foreach (Weapon_Base weapon: weapons)
			{
				if (weapon.IsDamageDestroyed())
				{
					eAI_DropItemImpl(weapon);
					break;
				}

				if (item != weapon && item.GetType() == weapon.GetType())
				{
					//! We already have a weapon of that type in inventory

					if (weapon.GetHealthLevel() > item.GetHealthLevel())
					{
						//! Existing weapon is in worse condition, drop it
						eAI_DropItemImpl(weapon, true);
						break;
					}

					//! Existing weapon is in equal or better condition, don't take item
					eAI_ThreatOverride(item, true);
					return false;
				}
			}
		}

		if (!eAI_TakeItemToInventoryImpl(item, 0, threatOverrideOnFailureToTakeToInv))
		{
			//! If we failed to take item to inv, item is weapon and both shoulder and melee slots are taken,
			//! drop one to make space for item and try again
			if (item.IsWeapon() && item.Expansion_HasInventorySlot((TStringArray){"Shoulder", "Melee"}))
			{
				ItemBase shoulder;
				ItemBase melee;

				if (Class.CastTo(shoulder, FindAttachmentBySlotName("Shoulder")) && Class.CastTo(melee, FindAttachmentBySlotName("Melee")))
				{
					ItemBase other;

					if (eAI_WeaponSelection(melee, shoulder))
						other = melee;
					else
						other = shoulder;

					if (eAI_DropIfLessDPS(item, other))
						return eAI_TakeItemToInventoryImpl(item);
				}
			}

			return false;
		}

		return true;
	}

	/**
	 * @brief drop other item if not a weapon or less DPS than item
	 */
	bool eAI_DropIfLessDPS(ItemBase item, ItemBase other)
	{
		if ((!other.IsWeapon() || !eAI_WeaponSelection(item, other)) && eAI_DropItemImpl(other))
		{
			eAI_ThreatOverride(other, false);
			return true;
		}

		return false;
	}

	bool eAI_IsItemObstructed(ItemBase item)
	{
		vector begPos;
		MiscGameplayFunctions.GetHeadBonePos(this, begPos);
		vector endPos = item.GetCenter();

		//! @note cannot use IsObjectObstructedFilterEx
		//! (uses vanilla ItemBase::CanObstruct which will always return true on server as it is only meant to be called on client because it relies on g_Game.GetPlayer)
		//m_eAI_IsItemObstructedCache.ClearCache();
		//m_eAI_IsItemObstructedCache.RaycastStart = begPos;
		//m_eAI_IsItemObstructedCache.ObjectCenterPos = endPos;

		//if (MiscGameplayFunctions.IsObjectObstructedFilterEx(item, m_eAI_IsItemObstructedCache, this))
			//return true;

		vector contactPos, contactDir;
		int contactComponent;
		set<Object> results = new set<Object>;

		if (DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, item, this, false, false, ObjIntersectFire, 0.0, CollisionFlags.ALLOBJECTS))
		{
			if (results.Count() == 0)
				return true;

			EntityAI entity;

			foreach (Object obj: results)
			{
				if (Class.CastTo(entity, obj))
					obj = entity.GetHierarchyRoot();

				if (ExpansionStatic.CanObstruct(obj))
					return true;
			}
		}

		return false;
	}

	bool Expansion_GetUp(bool force = false)
	{
		if (!force)
		{
			int stanceMask;
			eAIStance defaultStance;

			if (m_eAI_DefaultStance != eAIStance.STANDING)
			{
				if (g_Game.GetTickTime() - m_eAI_LastHitTime > 10.0 && !m_FSM.IsInState("Melee") && GetCurrentWaterLevel() < 0.3)
					defaultStance = m_eAI_DefaultStance;
			}

			switch (defaultStance)
			{
				case eAIStance.CROUCHED:
					stanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_RAISEDCROUCH;
					break;

				case eAIStance.PRONE:
					stanceMask = DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
					break;

				default:
					stanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_RAISEDERECT;
					break;
			}

			if (IsPlayerInStance(stanceMask))
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
					g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_EmoteManager.ServerRequestEmoteCancel, delay);
			}
		}

		if (m_Expansion_EmoteAutoCancel)
			m_Expansion_EmoteID = 0;
	}

	//! Radians!
	override float Expansion_GetHeadingAngle()
	{
		//! XXX: Should we sync m_eAI_AimRelAngles[0] to client? Seems overkill though
		//return (m_eAI_AimRelAngles[0] - GetOrientation()[0]) * Math.DEG2RAD;
		return -GetOrientation()[0] * Math.DEG2RAD;
	}

	override vector Expansion_GetHeadingVector()
	{
		vector dir = vector.Zero;
		//! XXX: Should we sync m_eAI_AimRelAngles[0] to client? Seems overkill though
		//float headingAngle = (m_eAI_AimRelAngles[0] - GetOrientation()[0]) * Math.DEG2RAD;
		float headingAngle = -GetOrientation()[0] * Math.DEG2RAD;
		dir[0] = Math.Cos(headingAngle + Math.PI_HALF);
		dir[2] = Math.Sin(headingAngle + Math.PI_HALF);
		return dir.Normalized();
	}

	override float Expansion_GetMovementSpeed()
	{
		auto cmd = GetCommand_Move();
		if (cmd)
		{
			float speed = m_eAI_CommandMove.GetCurrentMovementSpeed();
			if (speed > 2.0 && m_WeaponRaised)
				speed = 2.0;
			return speed;
		}

		return 0.0;
	}

	override float Expansion_GetMovementAngle()
	{
		auto cmd = GetCommand_Move();
		if (cmd)
			return m_eAI_CommandMove.GetCurrentMovementAngle();

		return 0.0;
	}

	float Expansion_GetTargetMovementAngle()
	{
		auto cmd = GetCommand_Move();
		if (cmd)
			return m_eAI_CommandMove.GetTargetDirection();

		return 0.0;
	}

	bool HandleVaulting(eAICommandMove hcm, float pDt)
	{
		//if (!m_PathFinding.IsVault())
		//{
		//	return false;
		//}

		if (IsRestrained())
			return false;

		if (m_eAI_PositionIsFinal && Math.Round(Expansion_GetMovementSpeed()) == 0.0)
			return false;

		if (!IsSwimming() && !m_PathFinding.m_AllowJumpClimb && !m_PathFinding.m_IsBlockedPhysically)
			return false;

		if (Math.AbsFloat(ExpansionMath.AngleDiff2(GetOrientation()[0], m_PathFinding.m_PathSegmentDirection.VectorToAngles()[0])) > 45.0)
			return false;

		if (m_WeaponManager.IsRunning())
			return false;

		Object blockingObject = hcm.GetBlockingObject();

		bool isBlockingItem;
		bool climbFloatingItem;
		bool isBlockingVehicle;

		//! Offset is because AI, like players, can just run over small height differences unless walking slowly
		//! 0.49 instead of 0.5 because of platform2_wall.p3d (e.g. at <1057.06, 157.539, 6692.51> in Camp Metalurg on Chernarus)
		float offset = 0.49;

		//! Superjanky but allows climbing floating items with collision, e.g. Expansion basebuilding floors,
		//! while preventing unwanted climbing on other items.
		if (blockingObject)
		{
			if (blockingObject.IsTransport())
			{
				isBlockingVehicle = true;
			}
			else if (blockingObject.IsInventoryItem() && ExpansionStatic.CanObstruct(blockingObject))
			{
				isBlockingItem = true;

				if (GetPosition()[1] + offset < blockingObject.GetPosition()[1] || IsSwimming() || m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK)
					climbFloatingItem = true;
			}
		}

		//! If blocking object is NOT a floating item that should be climbed/jumped and is NOT a vehicle...
		if (!climbFloatingItem && !isBlockingVehicle)
		{
			//! ...don't allow jump/climb if blocking object is an item
			if (isBlockingItem)
				return false;

			//! If pathfinding doesn't indicate jump/climb...
			if (!m_PathFinding.m_IsJumpClimb)
			{
				//! ...don't allow jump/climb if movement NOT blocked
				if (!hcm.IsBlocked())
					return false;

				//! ...don't allow jump/climb if NOT on inverse path and NOT using underwater surface for pathfinding
				//! and NOT blocked by sea ice
				if (m_PathFinding.m_PathGlueIdx == -1 && !eAI_ShouldUseSurfaceUnderWaterForPathFinding())
				{
					//! @note if not using underwater surface for pathfinding, we're also not swimming but might be in shallow water
					if (!eAI_IsSeaIce(blockingObject))
						return false;
				}
			}
		}

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(14) -> HandleVaulting");
#endif

		SHumanCommandClimbSettings hcls = GetDayZPlayerType().CommandClimbSettingsW();
		
		if ( m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE )
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;
		
		if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_SWIM)
			hcls.m_fBackwardsCheckDist = 0.35;
		else
			hcls.m_fBackwardsCheckDist = 0;

		SetOrientation(GetOrientation());
		HumanCommandClimb.DoClimbTest(this, m_ExClimbResult, 0);

		if (m_ExClimbResult.m_bIsClimb || m_ExClimbResult.m_bIsClimbOver)
		{
			//! AI, like players, can just run over small height differences unless walking slowly (avoids awkwardly climbing stairs)
			//! @note hcls.m_fFwMinHeight cannot be used for this since it is too high (0.7)
			//! @note this check is only needed for vanilla DoClimbTest, ExpansionClimb::DoClimbTest checks height internally
			if ((m_ExClimbResult.m_fClimbHeight < offset && !IsSwimming() && m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_WALK) || m_ExClimbResult.m_fClimbHeight > hcls.m_fFwMaxHeight)
				return false;

			return true;
		}

		//! As we are essentially using Zombie pathfinding, we may encounter situations where the path will go through a fence
		//! that Zs would be able to jump (e.g. wall_indfnc_9.p3d) but player AI would not due to HumanCommandClimb.DoClimbTest not letting us.
		//! Use ExpansionClimb.DoClimbTest with alwaysAllowClimb = true instead.
		if (m_PathFinding.m_DoClimbTestEx)
		{
			ExpansionClimb.DoClimbTest(this, m_ExClimbResult, true);
			if (m_ExClimbResult.m_bIsClimb || m_ExClimbResult.m_bIsClimbOver)
				return true;
		}

		return false;
	}

	override bool ProcessJumpOrClimb(float pDt, int pCurrentCommandID)
	{
		//if (GetIdentity())
			//return super.ProcessJumpOrClimb(pDt, pCurrentCommandID);

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
		//! Following is roughly equivalent to vanilla PlayerBase::CanClimb

		if (GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			return false;
			
		if (climbType == 1 && !CanConsumeStamina(EStaminaConsumers.VAULT))
			return false;	
		
		//! vault from water allowed - only check if not swimming
		if (!IsSwimming())
		{
			if (climbType == 2 && (!CanConsumeStamina(EStaminaConsumers.CLIMB) || GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS))
				return false;

			if (climbType > 0 && m_InjuryHandler && m_InjuryHandler.GetInjuryAnimValue() >= InjuryAnimValues.LVL3)
				return false;
		}

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
			if (!eAI_CanClimbOn(climbRes.m_GrabPointParent, climbRes))
				return false;
			if (!eAI_CanClimbOn(climbRes.m_ClimbStandPointParent, climbRes))
				return false;
			if (!eAI_CanClimbOn(climbRes.m_ClimbOverStandPointParent, climbRes))
				return false;

			IEntity standPointParent;
			vector standPoint;
			vector checkPosition;
			vector checkDirection;
			vector unitPosition = GetPosition();

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

			vector surfacePosition = ExpansionStatic.GetSurfaceRoadPosition(unitPosition + checkDirection);
			//! Swim start water level = 1.5 m, see DayZPlayerUtils::CheckWaterLevel
			if (!m_PathFinding.m_IsSwimmingEnabled && g_Game.GetWaterDepth(surfacePosition) > 1.5)
			{
			#ifdef DIAG_DEVELOPER
				Expansion_DebugObject(1344, unitPosition + checkDirection, "ExpansionDebugRodBig_Blue", checkDirection, surfacePosition);
			#endif
				return false;
			}
		#ifdef DIAG_DEVELOPER
			else
			{
				Expansion_DebugObject(1344, "0 0 0", "ExpansionDebugRodBig_Blue");
			}
		#endif

			if (!IsSwimming() || climbRes.m_bIsClimbOver)
			{
				float heightThresh;

				if (climbRes.m_bIsClimbOver)
					heightThresh = 1.0;  //! should prevent unwanted vaulting of stair-rails
				else
					heightThresh = DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW - climbRes.m_fClimbHeight;

				if (!eAI_IsFallSafe(checkDirection, false, heightThresh, false))
					return false;
			}
		}

		return true;
	}

	bool eAI_CanClimbOn(IEntity parent, SHumanCommandClimbResult climbRes)
	{
		EntityAI entity;
		if (Class.CastTo(entity, parent) && entity.IsHologram())
			return false;

		Object object;
		bool isPathEndPointCollidingObject;
		if (Class.CastTo(object, parent) && ExpansionStatic.IsColliding(object, m_PathFinding.GetEnd()))
			isPathEndPointCollidingObject = true;

		if ((m_WeaponRaised || (m_eAI_IsFightingFSM && !IsSwimming())) && !isPathEndPointCollidingObject && !climbRes.m_bIsClimbOver && !m_PathFinding.m_IsBlockedPhysically)
			return false;

		if (object)
		{ 
			if (object.IsTree() || object.IsBush() || object.IsMan())
				return false;

			BuildingBase building;
			if (Class.CastTo(building, object))
			{
				if (building.m_eAI_PreventClimb || building.IsInherited(ExpansionDebugObject))
					return false;

				if (climbRes.m_bIsClimbOver)
				{
					//! This should prevent AI from vaulting open doors...
					if (climbRes.m_fClimbHeight >= 1.7)
						return false;

					//! Don't allow vaulting unenterable building unless path endpoint is within building or we are within building
					if (!building.Expansion_IsEnterable() && !isPathEndPointCollidingObject && !ExpansionStatic.IsColliding(object, GetPosition()))
					{
						auto move = GetCommand_Move();
						if (!move || !m_eAI_CommandMove.IsBlocked())
						{
							if (EXTrace.AI)
								EXTrace.Print(true, this, "eAI_CanClimbOn false " + Debug.GetDebugName(parent) + " is scenery? " + object.IsScenery() + " is plain? " + object.IsPlainObject());

							return false;
						}
					}
				}
			}
			else if (object.IsTransport() || object.IsInventoryItem())
			{
				if (climbRes.m_bIsClimbOver && !isPathEndPointCollidingObject && !ExpansionStatic.IsColliding(object, GetPosition()))
					return false;
			}
			else if (!object.IsScenery())
			{
				//! This should prevent AI from vaulting open gates unless blocked...
				if (climbRes.m_bIsClimbOver && climbRes.m_fClimbHeight >= 1.7 && (!m_PathFinding.m_IsBlocked || !m_PathFinding.m_IsJumpClimb))
					return false;

				string debugName = object.GetDebugName();
				debugName.ToLower();

				//! Things we should never attempt to vault
				if (climbRes.m_bIsClimbOver && ExpansionString.ContainsAny(debugName, s_eAI_PreventClimbOver))
				{
					if (EXTrace.AI)
						EXTrace.Print(true, this, "eAI_CanClimbOn false " + Debug.GetDebugName(parent) + " is scenery? " + object.IsScenery() + " is plain? " + object.IsPlainObject());

					return false;
				}

				if (ExpansionString.ContainsAny(debugName, s_eAI_PreventClimb))
				{
					if (EXTrace.AI)
						EXTrace.Print(true, this, "eAI_CanClimbOn false " + Debug.GetDebugName(parent) + " is scenery? " + object.IsScenery() + " is plain? " + object.IsPlainObject());

					return false;
				}
			}
		}

		if (EXTrace.AI && parent)
			EXTrace.Print(true, this, "eAI_CanClimbOn " + Debug.GetDebugName(parent) + " is scenery? " + object.IsScenery() + " is plain? " + object.IsPlainObject());

		return true;
	}

	bool eAI_IsFallSafe(vector checkDirection, bool checkBlocking = true, float heightThresh = 0, bool checkHealth = true, int dbgIndex = 1337)
	{
		if (heightThresh == 0)
			heightThresh = DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW;

		vector position = GetPosition();
		vector checkPosition = position + checkDirection;
		
		vector hitPosition;
		vector hitNormal;
		int contactComponent;
		set<Object> results = new set<Object>;

		//! Is there a physical object blocking check direction (e.g. a wall)?
		//! @note vertical offset of 0.76 m and radius of 0.26 m is chosen so that it can still hit the railing of various buildings
		//! (e.g. Land_Pier_Crane2_Base and Land_Factory_Small), else AI could get stuck due to unsafe fallheight
		if (checkBlocking && DayZPhysics.RaycastRV(checkPosition + "0 0.76 0", position + "0 0.76 0", hitPosition, hitNormal, contactComponent, results, null, this, false, false, ObjIntersectGeom, 0.26))
		{
			//vector fromHit = hitPosition - checkDirection.Normalized() * 0.2;

			////! Is there a physical object blocking up direction from hit pos (e.g. a ceiling)?
			//if (DayZPhysics.RaycastRV(fromHit, fromHit + "0 3.5 0", hitPosition, hitNormal, contactComponent, results, null, this, false, false, ObjIntersectGeom))
				return true;
		}
	#ifdef DIAG_DEVELOPER
		else if (checkBlocking)
		{
			Expansion_DebugObject(dbgIndex + 3, checkPosition + "0 0.76 0", "ExpansionDebugSphereSmall_Red", vector.Zero, position + "0 0.76 0");
		}
	#endif

		//! @note the 1.5m vertical offset has been added for the top floor of Land_HouseBlock_2F3, else surface won't be detected properly
		//! (doesn't matter if via SurfaceRoad3D or raycast)
		float offsetY = 1.5;

/*
		vector begPos = Vector(checkPosition[0], checkPosition[1] + offsetY, checkPosition[2]);
*/

		checkPosition[1] = ExpansionStatic.GetSurfaceRoadY3D(checkPosition[0], checkPosition[1] + offsetY, checkPosition[2], RoadSurfaceDetection.UNDER);

		float fallHeight = position[1] - checkPosition[1];
/*
		if (fallHeight > DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
		{
			PhxInteractionLayers collisionLayerMask;
			collisionLayerMask |= PhxInteractionLayers.ROADWAY;
			collisionLayerMask |= PhxInteractionLayers.BUILDING;
			collisionLayerMask |= PhxInteractionLayers.FENCE;
			collisionLayerMask |= PhxInteractionLayers.VEHICLE;
			collisionLayerMask |= PhxInteractionLayers.TERRAIN;

			Object hitObject;
		#ifdef DIAG_DEVELOPER
			string type;
		#endif

			if (DayZPhysics.RayCastBullet(begPos, checkPosition, collisionLayerMask, this, hitObject, hitPosition, null, null))
			{
				checkPosition = hitPosition;

				fallHeight = position[1] - checkPosition[1];

			#ifdef DIAG_DEVELOPER
				if (fallHeight > DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
				{
					if (hitObject)
						type = hitObject.GetType();
					EXTrace.Print(EXTrace.AI, this, "eAI_IsFallSafe RayCastBullet hitObject " + hitObject + " " + type + " " + hitPosition);
				}
			#endif
			}

			if (fallHeight > DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
			{
				results = new set<Object>;

				if (DayZPhysics.RaycastRV(begPos, checkPosition, hitPosition, hitNormal, contactComponent, results, null, this, false, false, ObjIntersectFire, 0.0))
				{
					checkPosition = hitPosition;

					fallHeight = position[1] - checkPosition[1];

				#ifdef DIAG_DEVELOPER
					if (fallHeight > DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
					{
						hitObject = results[0];
						if (hitObject)
							type = hitObject.GetType();
						EXTrace.Print(EXTrace.AI, this, "eAI_IsFallSafe RaycastRV hitObject " + hitObject + " " + type + " " + hitPosition);
					}
				#endif
				}
			}
		}
*/
		bool isFallSafe;

		vector waterCheckPosition = checkPosition;
		float waterDepth = g_Game.GetWaterDepth(checkPosition);
		if (waterDepth > 0.0)
		{
			waterCheckPosition[1] = waterCheckPosition[1] + waterDepth;
			fallHeight -= waterDepth;
		}
		
		if (waterDepth > 1.5 && ExpansionStatic.SurfaceIsWater(waterCheckPosition))
			isFallSafe = m_PathFinding.m_IsSwimmingEnabled;  //! Falling into water that is deep enough for swimming is safe if swimming enabled
		else if (fallHeight <= heightThresh || (checkHealth && GetHealth01() - Math.InverseLerp(heightThresh, DayZPlayerImplementFallDamage.HEALTH_HEIGHT_HIGH, fallHeight) >= 0.90))
			isFallSafe = true;

	#ifdef DIAG_DEVELOPER
		//EXTrace.Print(EXTrace.AI, this, "eAI_IsFallSafe position " + ExpansionStatic.VectorToString(position, ExpansionVectorToString.Plain) + " checkDirection " + ExpansionStatic.VectorToString(checkDirection, ExpansionVectorToString.Plain) + " " + ExpansionStatic.VectorToString(checkDirection.VectorToAngles(), ExpansionVectorToString.Plain) + " checkPosition " + ExpansionStatic.VectorToString(checkPosition, ExpansionVectorToString.Plain) + " fallHeight " + fallHeight + " safe " + isFallSafe);
		vector arrowPos = checkPosition;
		arrowPos[1] = position[1] + 0.3;
		vector arrowOri = checkDirection.VectorToAngles();
		arrowOri[1] = -90.0;
		if (isFallSafe)
		{
			Expansion_DebugObject(dbgIndex, arrowPos, "ExpansionDebugArrow", arrowOri.AnglesToVector());
			Expansion_DebugObject(dbgIndex + 1, "0 0 0", "ExpansionDebugArrow_Red");
			Expansion_DebugObject(dbgIndex + 2, "0 0 0", "ExpansionDebugArrow_Red");
		}
		else
		{
			Expansion_DebugObject(dbgIndex, "0 0 0", "ExpansionDebugArrow");
			Expansion_DebugObject(dbgIndex + 1, arrowPos, "ExpansionDebugArrow_Red", arrowOri.AnglesToVector());
			arrowPos[1] = checkPosition[1] + 0.3;
			Expansion_DebugObject(dbgIndex + 2, arrowPos, "ExpansionDebugArrow_Red", arrowOri.AnglesToVector());
		}
	#endif

		return isFallSafe;
	}

	bool eAI_IsDangerousAltitude()
	{
		if (IsSwimming())
			return false;

		vector position = m_ExTransformPlayer[3];
		float fallHeight = position[1] - m_eAI_SurfaceY;

		if (fallHeight < DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
			return false;

		if (m_PathFinding.m_IsSwimmingEnabled)
		{
			vector surfacePosition = Vector(position[0], m_eAI_SurfaceY, position[2]);
			float waterDepth = Math.Max(g_Game.GetWaterDepth(surfacePosition), 0.0);
			if (fallHeight - waterDepth < DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW)
				return false;
		}

		return true;
	}

	void eAI_JumpOrClimb()
	{
		m_JumpClimb.JumpOrClimb();
	}

	bool eAI_IsLargeGate(Object obj)
	{
		string type = obj.GetType();

		type.ToLower();

		if (type.IndexOf("gate") > -1)
		{
			vector minMax[2];
			if (obj.GetCollisionBox(minMax) && vector.DistanceSq(minMax[0], minMax[1]) > 9.0)
				return true;
		}

		return false;
	}

	bool HandleBuildingDoors(eAICommandMove hcm, float pDt)
	{
		//if (!m_PathFinding.IsDoor())
			//return;

		if (IsRestrained())
			return false;

		float speed = hcm.GetCurrentMovementSpeed();
		if (m_eAI_PositionIsFinal && Math.Round(speed) == 0.0)
			return false;

		int time = g_Game.GetTime();

		if (time - m_eAI_LastDoorInteractionTime < 1000)
			return false;

		BuildingBase cover;
		if (!hcm.IsBlockedByBuildingWithDoor() && (!m_eAI_CurrentCoverObject || !Class.CastTo(cover, m_eAI_CurrentCoverObject) || cover.Expansion_IsEnterable()))
			return false;

		vector position = m_ExTransformPlayer[3] + (m_ExTransformPlayer[1] * 1.1);
		vector direction = m_ExTransformPlayer[2];

		float fwdBwd = 1.0;
		if (Math.AbsFloat(Expansion_GetMovementAngle()) > 135.0)
			fwdBwd = -1.0;

		vector p0 = position;
		vector p1 = position + (direction * 1.5 * Math.Clamp(speed, 1.0, 1.333) * fwdBwd);

		if (GetWeaponManager().IsRunning())
		{
		#ifdef DIAG_DEVELOPER
			Expansion_DebugObject(22222, "0 0 0", "ExpansionDebugSphereSmall_White");
			Expansion_DebugObject(22223, p1, "ExpansionDebugSphereSmall_Black", direction, position);
		#endif
			return false;
		}

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "CommandHandler(13) -> HandleBuildingDoors");
#endif

	#ifdef DIAG_DEVELOPER
		Expansion_DebugObject(22222, p1, "ExpansionDebugSphereSmall_White", direction, position);
		Expansion_DebugObject(22223, "0 0 0", "ExpansionDebugSphereSmall_Black");
	#endif

		RaycastRVParams params(p0, p1, this, 0.5);
		array<ref RaycastRVResult> results();
		array<Object> excluded();
		excluded.Insert(this);

		params.with = this;
		params.flags = CollisionFlags.ALLOBJECTS;
		params.type = ObjIntersectView;

		if (!DayZPhysics.RaycastRVProxy(params, results, excluded))
			return false;

		int inactiveDuration;  //! ms

		foreach (auto result : results)
		{
			BuildingBase building;
			if (!Class.CastTo(building, result.obj))
				continue;

			int doorIndex = building.GetDoorIndex(result.component);
			if (doorIndex == -1)
			{
				//! Why has everything to be a special case in vanilla >:(
				//! GetDoorIndex can not be used, and these special doors are not included in GetDoorCount either
				if (building.Expansion_IsUndergroundEntrance() && building.GetActionComponentName(result.component).IndexOf("door") == 0)
				{
					Land_Underground_EntranceBase entrance;
					//! CanManipulate == true means door is closed
					if (Class.CastTo(entrance, building) && entrance.CanManipulate())
					{
						if (m_PathFinding.m_IsBlocked)
						{
							m_PathFinding.m_IsUnreachable = true;
							m_PathFinding.m_IsTargetUnreachable = true;
						}

						if (m_PathFinding.m_AllowClosedDoors)
							m_PathFinding.AllowClosedDoors(false, 15.0);
					}

					break;
				}

				continue;
			}

			bool isDoorOpen = building.IsDoorOpen(doorIndex);
			bool isEnterable = building.Expansion_IsEnterable();

			bool canInteract = true;

			if (isDoorOpen)
			{
				if (isEnterable)
				{
					Object blockingObject = hcm.GetBlockingObject();
					if (blockingObject || !hcm.CheckBlocked())
					{
						//! If we're blocked less than 3 seconds, and there's enough free space to move fwd/left/right,
						//! and speed limit is zero or movement speed is zero or velocity is higher than 0.001 m/s, ignore opened door
						if (m_eAI_BlockedTime < 3.0 && (CheckFreeSpace(vector.Forward, 0.3, false) || CheckFreeSpace(vector.Aside, 0.5, false) || CheckFreeSpace(-vector.Aside, 0.5, false)) && (m_MovementSpeedLimit == 0 || Math.Floor(speed) == 0.0 || m_eAI_PositionTime < 1.5))
						{
							if (hcm.IsBlocked() && m_eAI_BlockedTime > 0.5)
							{
								m_PathFinding.ForceRecalculate(true);
								break;
							}
							else if (m_eAI_BlockedTime < 4.0 - speed)
							{
								break;
							}
						}
					}
				}

				if (!building.CanDoorBeClosed(doorIndex))
				{
					break;
				}
			}
			else if (!isEnterable)
			{
				//! Do not open doors on structures that cannot be entered, e.g. wrecks (less chance of getting stuck on them when closed)
				break;
			}
			//! @note vanilla Building::CanDoorBeOpened is implemented awkardly: If you pass `true` in the 2nd argument (`checkIfLocked`),
			//! then it will return false if the door is closed and locked.
			//! But if you pass in `false` for `checkIfLocked` (which is also the default), then it will return false if the door is
			//! closed and *not* locked, which is unexpected.
			//! The only reason we use it to begin with is because mods can override it.
			else if (building.IsDoorLocked(doorIndex) && building.CanDoorBeOpened(doorIndex, false))
			{
				bool canHitToUnlock = building.GetAllowDamage();

			#ifdef EXPANSIONMOD
				if (GetExpansionSettings().GetGeneral().DisableShootToUnlock)
					canHitToUnlock = false;
			#endif

				if (canHitToUnlock && (building.GetLockCompatibilityType(doorIndex) & (1 << EBuildingLockType.LOCKPICK)))
				{
					//auto info = building.eAI_GetDoorTargetInformation(doorIndex, result.pos);
					auto info = building.eAI_GetDoorTargetInformation(doorIndex, building.GetDoorSoundPos(doorIndex));
					info.AddAI(this);
					break;
				}
				else
				{
					//! If door cannot be opened and path is blocked, stop moving
					if (m_PathFinding.m_IsBlocked)
					{
						m_PathFinding.m_IsUnreachable = true;
						m_PathFinding.m_IsTargetUnreachable = true;
					}

					canInteract = false;
				}
			}
			else if (!building.CanDoorBeOpened(doorIndex, true))
			{
				//! If door cannot be opened and path is blocked, stop moving
				if (m_PathFinding.m_IsBlocked)
					m_PathFinding.m_IsUnreachable = true;

				canInteract = false;
			}

			int timeTresh = building.eAI_GetDoorAnimationTime(doorIndex) * 1000;
			if (time - building.m_eAI_LastDoorInteractionTime[doorIndex] < timeTresh + 500)
				break;

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
			if (!isDoorOpen && !m_eAI_Halt)
			{
				eAI_SetHalt(true);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_SetHalt, timeTresh * 0.65, false, false);
			}

			//! Prevent AI getting pushed by opening/closing door by temporarily disabling collision
			/*
			PhxInteractionLayers layer = dBodyGetInteractionLayer(this);
			if (layer != PhxInteractionLayers.AI && !m_eAI_ResetInteractionLayer)
			{
				m_eAI_ResetInteractionLayer = true;

				if (m_eAI_Halt)
					inactiveDuration = Math.Min(timeTresh, 1000);
				else
					inactiveDuration = ExpansionMath.PowerConversion(1, 3, m_MovementSpeedLimit, Math.Min(timeTresh, 1000), timeTresh * 0.2, 2.6);

				dBodySetInteractionLayer(this, PhxInteractionLayers.AI);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_dBodySetInteractionLayer, inactiveDuration, false, layer);
			}
			*/

			if (isDoorOpen)
			{
				StartActionObject(eAIActionCloseDoorsDummy, building);
				building.CloseDoor(doorIndex);
			}
			else
			{
				StartActionObject(eAIActionOpenDoorsDummy, building);
				//! @note if the door cannot be opened because it is locked,
				//! the game will prevent opening and play the usual "doorknob rattle" sound
				if (canInteract || building.IsDoorLocked(doorIndex))
					building.OpenDoor(doorIndex);
			}

			if (canInteract)
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_PathFinding.ForceRecalculate, 34, false, true);
			}
			else
			{
				if (m_PathFinding.m_AllowClosedDoors)
					m_PathFinding.AllowClosedDoors(false, 15.0);
			}

			building.m_eAI_LastDoorInteractionTime[doorIndex] = time;  //! To prevent any AI from interacting with this specific door (cooldown)
			m_eAI_LastDoorInteractionTime = time;  //! To prevent this AI from interacting with any door (cooldown)
			return canInteract;
		}

		return false;
	}

	void eAI_SetHalt(bool halt)
	{
		m_eAI_Halt = halt;
	}

	void eAI_dBodySetInteractionLayer(PhxInteractionLayers layer)
	{
		m_eAI_ResetInteractionLayer = false;

		if (!IsDamageDestroyed())
			dBodySetInteractionLayer(this, layer);
	}

	void eAI_UpdateVisitedBuildings()
	{
		if (GetGroup().GetWaypointBehaviour() == eAIWaypointBehavior.ROAMING && GetGroup().GetFormationLeader() == this)
		{
			vector begPos = GetPosition() - "0 0.1 0";  //! Start raycast slightly under ground in case building has no roof
			//! Raycast up instead of down because not all buildings have floors (e.g. sheds)
			//! Raycast height is chosen so that it can still hit ceiling of Land_Cementworks_Hall2 from floor
			vector endPos = GetPosition() + "0 11 0";
			vector hitPosition;
			vector hitNormal;
			int contactComponent;
			set<Object> results = new set<Object>;

			if (DayZPhysics.RaycastRV(begPos, endPos, hitPosition, hitNormal, contactComponent, results, null, this, false, false, ObjIntersectGeom, 0.0, CollisionFlags.ALLOBJECTS))
			{
				BuildingBase building;
				foreach (Object obj: results)
				{
					if (Class.CastTo(building, obj))
					{
						GetGroup().m_VisitedBuildings.Insert(building);
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, "Looted " + ExpansionStatic.GetDebugInfo(building));
					#endif
						break;
					}
				}
			}
		}
	}
};
