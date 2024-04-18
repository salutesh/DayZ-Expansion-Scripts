/**
 * ExpansionLauncher.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ELBAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
};

enum ELBStableStateID
{
	UNKNOWN				=  0,
	Empty				=  1,
	Loaded				=  2,
}

class ELBEmpty: WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Empty E"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Empty E"); }
	override int GetCurrentStateID () { return ELBStableStateID.Empty; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
	override bool IsRepairEnabled () { return true; }
	override void InitMuzzleArray () { m_muzzleHasBullet = {MuzzleState.E}; }
};
class ELBLoaded: WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { Loaded C"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } Loaded C"); }
	override int GetCurrentStateID () { return ELBStableStateID.Loaded; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
	override bool IsRepairEnabled () { return true; }
	override void InitMuzzleArray () { m_muzzleHasBullet = {MuzzleState.L}; }
};

// This is a very basic singleshot FSM wich only has 2 states - empty and loaded
// It will not make any "clicking" noises when trying to fire it empty
class ExpansionLauncher_Base: Rifle_Base
{
	void ExpansionLauncher_Base ()
	{
	}
	

	
	override void InitStateMachine()
	{
		// setup abilities
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));

		// setup state machine
		// basic weapon states
		WeaponStateBase E = new ELBEmpty(this, NULL, ELBAnimState.DEFAULT);
		WeaponStateBase L = new ELBLoaded(this, NULL, ELBAnimState.DEFAULT);
		// unstable (intermediate) states
		WeaponStateBase Mech_L = new ExpansionUnloadHideBullet(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		
		ExpansionReloadShowBullet Chamber_E = new ExpansionReloadShowBullet(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		
		WeaponStateBase Trigger_E = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);
		WeaponStateBase Trigger_L = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);

		

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		
		// eject good cartridge
		m_fsm.AddTransition(new WeaponTransition( L,			__M__,	Mech_L));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_fin_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_fin_,	L));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_abt_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_abt_,	L));

		// load bullet
		m_fsm.AddTransition(new WeaponTransition( E,			__L__,	Chamber_E));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_E,	_fin_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_E,	_fin_,	L));
			Chamber_E.AddTransition(new WeaponTransition(  Chamber_E.m_start,	_abt_,	E));
			Chamber_E.AddTransition(new WeaponTransition(  Chamber_E.m_eject,	_abt_,	E));
			Chamber_E.AddTransition(new WeaponTransition(  Chamber_E.m_chamber,	_abt_,	E));
			Chamber_E.AddTransition(new WeaponTransition(  Chamber_E.m_w4t,		_abt_,	L));
		
		
		m_fsm.AddTransition(new WeaponTransition( L,			__T__,	Trigger_L)); // Meow
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L,	_fin_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L,	_abt_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		
		

		// initial state setup
		bool empty = true;
		bool discharged = false; // @TODO:
		WeaponStateBase init_state = E;
		if (empty)
		{
			if (!discharged)
				init_state = E;
		}
		else
		{
			init_state = L; // can init state == load/jammed?
		}
		m_fsm.SetInitialState(init_state);

		ExpansionHideWeaponPart("ammo", true);

		m_fsm.Start();
	}

	override void OnFire(int muzzle_index)
	{
		super.OnFire(muzzle_index);

		EjectCasing(muzzle_index);
		ExpansionHideWeaponPart("ammo", true);
	}
};
