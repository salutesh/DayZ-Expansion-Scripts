/**
 * ExpansionSemiShotgun.cpp
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 * Special thanks to Niphoria#2851 for this file <3
 *
*/

enum ExpansionSemiShotGunAnimState
{
	DEFAULT 			= 0, 	
	OPENED				= 1, 	
	JAMMED				= 2,	
}

enum ExpansionSemiShotGunStableStateID  
{
	UNKNOWN		=  0,
	Empty		=  1,
	Dry			=  2,
	Loaded		=  3,
	Jammed		=  4,
}

class ExpansionSemiShotGunEmpty extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Empty"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Empty"); }
	override int GetCurrentStateID () { return ExpansionSemiShotGunStableStateID.Empty; }
	override bool HasBullet () { return false; }
	override bool IsJammed () { return false; }
}
class ExpansionSemiShotGunDry extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Dry"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Dry"); }
	override int GetCurrentStateID () { return ExpansionSemiShotGunStableStateID.Dry; }
	override bool HasBullet () { return false; }
	override bool IsJammed () { return false; }
	override bool IsBoltOpen () { return true; }
}
class ExpansionSemiShotGunLoaded extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Loaded C"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Loaded C"); }
	override int GetCurrentStateID () { return ExpansionSemiShotGunStableStateID.Loaded; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
}
class ExpansionSemiShotGunJammed extends WeaponStateJammed
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Jammed"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Jammed"); }
	override int GetCurrentStateID () { return ExpansionSemiShotGunStableStateID.Jammed; }
	override bool HasBullet () { return true; }
	override bool IsJammed () { return true; }
	override bool IsBoltOpen () { return true; }
}

class ExpansionSemiShotGun extends Rifle_Base
{
	ref WeaponStateBase Empty;			
	ref WeaponStateBase Dry;
	ref WeaponStateBase Loaded;
	ref WeaponStateBase Jammed;

	override void InitStateMachine ()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSemiShotGun::InitStateMachine - Start");
		#endif
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));  
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));	
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST));	
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE));
		
		
		Empty = 	new ExpansionSemiShotGunEmpty(this, NULL, ExpansionSemiShotGunAnimState.DEFAULT);			
		Dry = 		new ExpansionSemiShotGunDry(this, NULL, ExpansionSemiShotGunAnimState.OPENED);
		Loaded = 	new ExpansionSemiShotGunLoaded(this, NULL, ExpansionSemiShotGunAnimState.DEFAULT);
		Jammed = 	new ExpansionSemiShotGunJammed(this, NULL, ExpansionSemiShotGunAnimState.JAMMED);

		
		WeaponChargingInnerMag		Mech_Empty = 	new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponChargingInnerMag		Mech_Dry = 		new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponChargingInnerMag		Mech_Loaded = 	new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		
		
		LoopedChamberingEjectLast 	Chamber_Empty 		= new LoopedChamberingEjectLast(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChamberingEjectLast 	Chamber_Dry 		= new LoopedChamberingEjectLast(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChamberingEjectLast 	Chamber_Loaded 		= new LoopedChamberingEjectLast(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		
		WeaponStateBase							Unjam_J = 				new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);
		WeaponStateBase							Trigger_Empty = 		new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY); 
		WeaponStateBase							Trigger_Dry = 			new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase 						Trigger_Last = 			new WeaponFireLast(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST);		
		WeaponFireAndChamberFromInnerMagazine 	Trigger_Loaded = 		new WeaponFireAndChamberFromInnerMagazine(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL) ); 
		WeaponStateBase							Trigger_Jammed = 		new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase							Trigger_LoadedJammed = 	new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM);  
		WeaponStateBase 						Unjam_Weapon = 			new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		

		WeaponEventBase _finished_ 		= new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __conload_ 		= new WeaponEventContinuousLoadBulletStart;
		WeaponEventBase __L1B__ 		= new WeaponEventLoad1Bullet;
		WeaponEventBase __Trig__ 		= new WeaponEventTrigger;
		WeaponEventBase __TrigJam_ 		= new WeaponEventTriggerToJam;
		WeaponEventBase __Unjam__ 		= new WeaponEventUnjam;
		WeaponEventBase	__Mechanism__ 	= new WeaponEventMechanism;
		WeaponEventBase _abort_ 		= new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _reloadaction_ 	= new WeaponEventReloadTimeout;
		WeaponEventBase _dry_ 			= new WeaponEventDryFireTimeout;
		
		m_fsm = new WeaponFSM(); 
		

		m_fsm.AddTransition(new WeaponTransition( Empty,				__L1B__,  		Chamber_Empty)); 	
		m_fsm.AddTransition(new WeaponTransition( Chamber_Empty,		_finished_,  	Loaded));			
		m_fsm.AddTransition(new WeaponTransition( Chamber_Empty,		_abort_,  		Empty, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Empty,		_abort_,		Loaded));	
		
		m_fsm.AddTransition(new WeaponTransition( Loaded,				__L1B__,  		Chamber_Loaded, NULL, new GuardNot(new WeaponGuardInnerMagazineFull(this))));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Loaded,		_finished_,  	Loaded));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Loaded,		_abort_,  		Loaded));
		
		m_fsm.AddTransition(new WeaponTransition( Dry,					__L1B__,  		Chamber_Dry, NULL, new GuardNot(new WeaponGuardInnerMagazineFull(this))));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Dry,			_finished_,  	Loaded));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Dry,			_abort_,  		Dry, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_Dry,			_abort_,  		Loaded));
		

		m_fsm.AddTransition(new WeaponTransition( Loaded,				__Mechanism__,	Mech_Loaded));
		m_fsm.AddTransition(new WeaponTransition( Mech_Loaded,			_finished_,		Empty, NULL, new WeaponGuardChamberEmpty(this) )); 
		m_fsm.AddTransition(new WeaponTransition( Mech_Loaded,			_finished_,		Loaded));
		m_fsm.AddTransition(new WeaponTransition( Mech_Loaded,			_abort_,		Empty, NULL, new WeaponGuardChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition( Mech_Loaded,			_abort_,		Loaded));

		m_fsm.AddTransition(new WeaponTransition( Loaded,				__Trig__,		Trigger_Loaded, NULL, new WeaponGuardHasAmmoInnerMagazine(this)));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Loaded,		_finished_,		Loaded));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Loaded,		_reloadaction_,	Loaded));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Loaded,		_abort_,		Loaded));

		
		m_fsm.AddTransition(new WeaponTransition( Loaded,				__Trig__,		Trigger_Last));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Last,			_finished_,		Dry));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Last,			_reloadaction_,	Dry));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Last,			_abort_,		Dry));
		
		
		
		
		

		m_fsm.AddTransition(new WeaponTransition( Dry, 					__Mechanism__,	Mech_Dry)); 
		m_fsm.AddTransition(new WeaponTransition( Mech_Dry,				_finished_,		Empty));
			Mech_Dry.AddTransition(new WeaponTransition( Mech_Dry.m_start,	_abort_,	Empty));
			Mech_Dry.AddTransition(new WeaponTransition( Mech_Dry.m_onCK,	_abort_,	Empty));
		
		m_fsm.AddTransition(new WeaponTransition( Dry,					__Trig__, 		Trigger_Dry));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Dry,			_finished_, 	Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Dry,			_abort_, 		Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Dry,			_reloadaction_, Empty));
		
		m_fsm.AddTransition(new WeaponTransition( Empty,				__Mechanism__,	Mech_Empty));
		m_fsm.AddTransition(new WeaponTransition( Mech_Empty,			_finished_, 	Empty));
		m_fsm.AddTransition(new WeaponTransition( Mech_Empty,			_abort_, 		Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Empty,		_reloadaction_,	Empty));
		
		m_fsm.AddTransition(new WeaponTransition( Empty,				__Trig__, 		Trigger_Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Empty,		_finished_, 	Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Empty,		_dry_,		 	Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Empty,		_abort_, 		Empty));
		m_fsm.AddTransition(new WeaponTransition( Trigger_Empty,		_reloadaction_, Empty));
		

		m_fsm.AddTransition(new WeaponTransition(Loaded,				__TrigJam_, 	Trigger_LoadedJammed)); 
		m_fsm.AddTransition(new WeaponTransition(Trigger_LoadedJammed,	_finished_, 	Jammed ));
		m_fsm.AddTransition(new WeaponTransition(Trigger_LoadedJammed,	_reloadaction_, Jammed ));
		m_fsm.AddTransition(new WeaponTransition(Trigger_LoadedJammed,	_abort_, 		Jammed ));
		
		m_fsm.AddTransition(new WeaponTransition(Jammed,				__Trig__, 		Trigger_Jammed)); 
		m_fsm.AddTransition(new WeaponTransition(Trigger_Jammed,		_finished_, 	Jammed));
		m_fsm.AddTransition(new WeaponTransition(Trigger_Jammed,		_dry_, 			Jammed));
		m_fsm.AddTransition(new WeaponTransition(Trigger_Jammed,		_reloadaction_, Jammed));
		m_fsm.AddTransition(new WeaponTransition(Trigger_Jammed,		_abort_, 		Jammed));
		
		

		m_fsm.AddTransition(new WeaponTransition(Jammed, 				__Unjam__, 		Unjam_Weapon));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Jammed, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Loaded, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Loaded));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Jammed, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Loaded, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Loaded));
		
		
		/*
		m_fsm.AddTransition(new WeaponTransition(Jammed, 				__Unjam__, 		Unjam_Weapon));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Jammed, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Empty, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_finished_, 	Loaded));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Jammed, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Empty, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_Weapon,			_abort_, 		Loaded));*/
		
		
		
		
		m_fsm.SetInitialState(Empty);  //when the weapon spawns .. wich state it should be in

		SelectionBulletHide(); 
		HideMagazine();

		m_fsm.Start();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSemiShotGun::InitStateMachine - End");
		#endif
	}
	
	override bool CanChamberBullet (int muzzleIndex, Magazine mag)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionReflexMRSOptic::CanChamberBullet - Return");
		#endif
		return CanChamberFromMag(muzzleIndex, mag) && !IsInternalMagazineFull(muzzleIndex);
	}
	
	override void SetActions()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionReflexMRSOptic::SetActions - Start");
		#endif
		
		super.SetActions();
		AddAction(FirearmActionLoadMultiBullet);

		RemoveAction(FirearmActionLoadBulletQuick); 
		AddAction(FirearmActionLoadMultiBulletQuick);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionReflexMRSOptic::SetActions - End");
		#endif	
	}
}
		