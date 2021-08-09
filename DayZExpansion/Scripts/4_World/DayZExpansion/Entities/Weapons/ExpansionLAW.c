/**
 * ExpansionLAW.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLAW: SKS_Base
{
	override void InitStateMachine()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED_KEEP));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD_CLIP, 0));
	
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED)));//????
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)));
		
		//m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));

		// setup state machine
		// basic weapon states
		// close-locked-jammed | nobullet-bullet | nomag-mag
		// regexp: [CLJ][01][01]
		C0 = new SKS_CLO_BU0(this, NULL, SKSAnimState.DEFAULT);
		C1 = new SKS_CLO_BU1(this, NULL, SKSAnimState.DEFAULT);
		L0 = new SKS_OPN_BU0(this, NULL, SKSAnimState.OPENED);
		JF = new SKS_JAM_BU1(this, NULL, SKSAnimState.JAMMED);

		// unstable (intermediate) states
		WeaponChargingInnerMag Mech_C0 = new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponChargingInnerMag Mech_L0 = new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED));
		WeaponChargingInnerMag Mech_C1 = new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		
		//Fire
		WeaponStateBase Trigger_C0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)); // cock without clip
		ExpansionWeaponFireAndChamberFromInnerMagazine Trigger_C1 = new ExpansionWeaponFireAndChamberFromInnerMagazine(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL) );
		WeaponStateBase Trigger_C1L = new ExpansionWeaponFireLast(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST));
		WeaponStateBase Trigger_L0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_JF = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		
		WeaponStateBase Trigger_C1J = new ExpansionWeaponFireToJam(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM); 
	
		//Unjam
		WeaponStateBase Unjam_JF = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));

		//Chambering
		LoopedChambering 	Chamber_C0 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_C1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED_KEEP, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_L0 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		
		//WeaponStateBase chmMag = new WeaponChamberFromAttMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __TJ_ = new WeaponEventTriggerToJam;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();

		// order: Discharged-Charged | nobullet-bullet | nomag-mag
		// charging
		m_fsm.AddTransition(new WeaponTransition( C0,			__M__,	Mech_C1)); // charge from dischg nobullet nomag

		m_fsm.AddTransition(new WeaponTransition( C1,			__M__,	Mech_C1)); // charge from bullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_C1,		_fin_,	C0, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C1,		_fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C1,		_abt_,	C0, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C1,		_abt_,	C1));
		
		m_fsm.AddTransition(new WeaponTransition( L0, 			__M__,	Mech_L0)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_L0,		_fin_,	C0, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L0,		_fin_,	C0));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_start,		_abt_,	L0));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_onBE,			_abt_,	C0));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_onBEFireOut,	_abt_,	C0));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_hideB,		_abt_,	C0));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_chamber,		_abt_,	C1));
			Mech_L0.AddTransition(new WeaponTransition( Mech_L0.m_onCK,			_abt_,	C0));
		
		
				/*m_start = new WeaponCharging_Start(m_weapon, this, m_action, m_actionType);
		m_onBE = new WeaponEjectBullet_Cartridge_W4T(m_weapon, this);
		m_onBEFireOut = new WeaponEjectCasing_W4T(m_weapon, this);
		m_hideB = new BulletHide_W4T(m_weapon, this);
		m_onCK = new WeaponCharging_CK(m_weapon, this);
		m_chamber = new WeaponChamberFromInnerMag_W4T(m_weapon, this);*/
		
		
		

		// fire
		m_fsm.AddTransition(new WeaponTransition( C0, 			__T__,	Trigger_C0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C0,	_fin_,	C0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C0,	_dto_,	C0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C0,	_abt_,	C0));
		
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__T__,	Trigger_C1, NULL, new WeaponGuardHasAmmoInnerMagazine(this)));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1,	_fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1,	_rto_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1,	_abt_,	C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__T__,	Trigger_C1L));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1L,	_fin_,	L0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1L,	_rto_,	L0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1L,	_abt_,	L0));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__TJ_,	Trigger_C1J));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1J,	_fin_,	JF ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1J,	_rto_,	JF ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C1J,	_abt_,	JF ));
	
		m_fsm.AddTransition(new WeaponTransition( L0,			__T__,	Trigger_L0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L0,	_fin_,	L0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L0,	_dto_,	L0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L0,	_abt_,	L0));
		
		m_fsm.AddTransition(new WeaponTransition( JF,			__T__,	Trigger_JF)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF,	_fin_,	JF));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF,	_dto_,	JF));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF,	_abt_,	JF));


		// load cartridge
		m_fsm.AddTransition(new WeaponTransition( C0,			__L__,	Chamber_C0));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C0,	_fin_,	C1));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_start,		_abt_,	C0));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_eject,		_abt_,	L0));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_chamber,	_abt_,	C1));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_w4sb2,		_abt_,	C1));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_hideB,		_abt_,	C1));
			Chamber_C0.AddTransition(new WeaponTransition( Chamber_C0.m_endLoop,	_abt_,	C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__L__,	Chamber_C1, NULL, new GuardNot(new WeaponGuardInnerMagazineFullShareChamber(this))));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C1,	_fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C1,	_abt_,	C1));
		
		m_fsm.AddTransition(new WeaponTransition( L0,			__L__,	Chamber_L0, NULL, new GuardNot(new WeaponGuardInnerMagazineFullShareChamber(this))));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_L0,	_fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_L0,	_abt_,	L0, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_L0,	_abt_,	C1));

		
		// unjam
		m_fsm.AddTransition(new WeaponTransition( JF,			__U__,	Unjam_JF)); // unjam nomag
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_fin_,	JF, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_fin_,	C0, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_abt_,	JF, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_abt_,	C0, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF,	_abt_,	C1));

		m_fsm.SetInitialState(C0);

		SelectionBulletHide();
		HideMagazine();

		m_fsm.Start();
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
  	{
		if ( IsMissionClient() )
		{
			Particle.PlayOnObject( ParticleList.EXPANSION_ROCKET_DUST, this, GetMemoryPointPos("usti hlavne") );
		}

		super.EEFired(muzzleType, mode, ammoType);
	}
};