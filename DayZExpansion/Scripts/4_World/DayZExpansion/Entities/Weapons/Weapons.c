/**
 * Weapons.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

void FoldOpticsDown(EntityAI ParentItem)
{
		protected int foldingOpticRaisedId = -1;
		protected int foldingOpticLoweredId = -1;
		TStringArray selectionNames = new TStringArray;

		ParentItem.ConfigGetTextArray("simpleHiddenSelections",selectionNames);

		foldingOpticRaisedId = selectionNames.Find("folding_raised");
		foldingOpticLoweredId = selectionNames.Find("folding_lowered");  

		//HideSelection("folding_raised");
		//ShowSelection("folding_lowered");

		ParentItem.SetSimpleHiddenSelectionState(foldingOpticRaisedId,false);
		ParentItem.SetSimpleHiddenSelectionState(foldingOpticLoweredId,true);
			
}

void FoldOpticsUp(EntityAI ParentItem)
{
		protected int foldingOpticRaisedId = -1;
		protected int foldingOpticLoweredId = -1;
		TStringArray selectionNames = new TStringArray;

		ParentItem.ConfigGetTextArray("simpleHiddenSelections",selectionNames);

		foldingOpticRaisedId = selectionNames.Find("folding_raised");
		foldingOpticLoweredId = selectionNames.Find("folding_lowered");  
		//HideSelection("folding_raised");
		//ShowSelection("folding_lowered");

		ParentItem.SetSimpleHiddenSelectionState(foldingOpticRaisedId,true);
		ParentItem.SetSimpleHiddenSelectionState(foldingOpticLoweredId,false);
}


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
}

class ExpansionFlaregun: FNX45_Base
{
	override typename GetExpansionFireType()
	{
		return ExpansionWeaponFireFlareGun;
	}
	
	override void InitStateMachine ()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_BULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH));

		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_OPENED));

		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED_UNCOCKED));

		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));

		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM));

		// setup state machine
		// basic weapon states
		// open-closed | discharged-charged | nobullet-bullet | nomag-mag
		// regexp: [OC][CDJ][01][01]
		CD00 = new Pistol_CLO_DIS_BU0_MA0(this, NULL, PistolAnimState.DEFAULT);
		CC00 = new Pistol_CLO_CHG_BU0_MA0(this, NULL, PistolAnimState.CLOSED_CHARGED);
		CC10 = new Pistol_CLO_CHG_BU1_MA0(this, NULL, PistolAnimState.CLOSED_CHARGED);
		CJF0 = new Pistol_CLO_JAM_BU1_MA0(this, NULL, PistolAnimState.JAMMED);
		OD00 = new Pistol_OPE_DIS_BU0_MA0(this, NULL, PistolAnimState.OPENED_DISCHARGED);
		CD01 = new Pistol_CLO_DIS_BU0_MA1(this, NULL, PistolAnimState.DEFAULT);
		CC01 = new Pistol_CLO_CHG_BU0_MA1(this, NULL, PistolAnimState.CLOSED_CHARGED);
		CC11 = new Pistol_CLO_CHG_BU1_MA1(this, NULL, PistolAnimState.CLOSED_CHARGED);
		CJF1 = new Pistol_CLO_JAM_BU1_MA1(this, NULL, PistolAnimState.JAMMED);
		OD01 = new Pistol_OPE_DIS_BU0_MA1(this, NULL, PistolAnimState.OPENED_DISCHARGED);

		// unstable (intermediate) states
		WeaponCharging Mech_CD00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED_UNCOCKED);
		WeaponCharging Mech_CC00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED_UNCOCKED);
		WeaponCharging Mech_CD01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED_UNCOCKED);
		WeaponCharging Mech_CC01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponCharging Mech_OD00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED);
		WeaponCharging Mech_OD01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED);
		
		WeaponCharging Mech_CC10 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED); // eject with no mag
		WeaponCharging Mech_CC11 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED); // eject with mag
		
		
		WeaponDryFire Trigger_CC00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED); // cock without clip
		WeaponDryFire Trigger_CD00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);
		WeaponDryFire Trigger_OD00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		WeaponDryFire Trigger_CD01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED);
		WeaponDryFire Trigger_OD01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		WeaponDryFire Trigger_CC01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED); // cock with clip
		WeaponDryFire Trigger_CJF0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		WeaponDryFire Trigger_CJF1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		
		ExpansionWeaponFireAndChamberNext Trigger_CC11 = new ExpansionWeaponFireAndChamberNext(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase Trigger_CC10 = new ExpansionWeaponFireLast(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL); // fire last no mag
		WeaponStateBase Trigger_CC11L = new ExpansionWeaponFireLast(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST); // fire last with mag
		
		WeaponStateBase Trigger_CC10J = new ExpansionWeaponFireToJam(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM); 
		WeaponStateBase Trigger_CC11J = new ExpansionWeaponFireToJam(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM);

		WeaponStateBase Unjam_CJF0 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);
		WeaponStateBase Unjam_CJF1 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);

		WeaponChambering Chamber_CD00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED_UNCOCKED);
		WeaponChambering Chamber_CC00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		WeaponChambering Chamber_OD00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_OPENED);
		WeaponChambering Chamber_CD01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED_UNCOCKED);
		WeaponChambering Chamber_CC01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		WeaponChambering Chamber_OD01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_OPENED);

		WeaponAttachMagazine Attach_CC10 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED);
		WeaponAttachMagazine Attach_CJF0 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED);
		WeaponAttachMagazine Attach_CD00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_UNCOCKED);
		WeaponAttachMagazine Attach_CC00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_COCKED);
		WeaponAttachMagazine Attach_OD00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_OPENED);

		WeaponReplacingMagAndChamberNext Reload_CD01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_UNCOCKED);
		WeaponReplacingMagAndChamberNext Reload_CC01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_COCKED);
		WeaponReplacingMagAndChamberNext Reload_CC11 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_BULLET_CLOSED);
		WeaponReplacingMagAndChamberNext Reload_CJF1 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_BULLET_CLOSED);
		WeaponReplacingMagAndChamberNext Reload_OD01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_OPENED);

		WeaponDetachingMag Detach_CC11 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH);
		WeaponDetachingMag Detach_CC01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH);
		WeaponDetachingMag Detach_CD01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH);
		WeaponDetachingMag Detach_OD01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH);
		WeaponDetachingMag Detach_CJF1 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH);

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __TJ_ = new WeaponEventTriggerToJam;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __A__ = new WeaponEventAttachMagazine;
		WeaponEventBase __D__ = new WeaponEventDetachMagazine;
		WeaponEventBase __S__ = new WeaponEventSwapMagazine;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
	
		m_fsm = new WeaponFSM();

		// charging
		m_fsm.AddTransition(new WeaponTransition( CD00,				__M__,	Mech_CD00)); // charge from closed
		m_fsm.AddTransition(new WeaponTransition(  Mech_CD00,		_fin_,	CC00));
			Mech_CD00.AddTransition(new WeaponTransition( Mech_CD00.m_start,			_abt_,	CD00));
			Mech_CD00.AddTransition(new WeaponTransition( Mech_CD00.m_eject,			_abt_,	CC00));
			Mech_CD00.AddTransition(new WeaponTransition( Mech_CD00.m_onCK,				_abt_,	CC00));
			Mech_CD00.AddTransition(new WeaponTransition( Mech_CD00.m_chamber,			_abt_,	CC00));
		
		m_fsm.AddTransition(new WeaponTransition( CC00,				__M__,	Mech_CC00)); // charge from charged
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC00,		_fin_,	CC00));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC00,		_abt_,	CC00));

		m_fsm.AddTransition(new WeaponTransition( OD00,				__M__,	Mech_OD00)); // charge from opened
		m_fsm.AddTransition(new WeaponTransition(  Mech_OD00,		_fin_,	CC00));
			Mech_OD00.AddTransition(new WeaponTransition( Mech_OD00.m_start,			_abt_,	OD00));
			Mech_OD00.AddTransition(new WeaponTransition( Mech_OD00.m_eject,			_abt_,	CC00));
			Mech_OD00.AddTransition(new WeaponTransition( Mech_OD00.m_onCK,				_abt_,	CC00));
			Mech_OD00.AddTransition(new WeaponTransition( Mech_OD00.m_chamber,			_abt_,	CC00));
		
		m_fsm.AddTransition(new WeaponTransition( OD01,				__M__,	Mech_OD01)); // charge from opened with mag
		m_fsm.AddTransition(new WeaponTransition(  Mech_OD01,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_OD01,		_fin_,	CC11));
			Mech_OD01.AddTransition(new WeaponTransition( Mech_OD01.m_start,			_abt_,	OD01));
			Mech_OD01.AddTransition(new WeaponTransition( Mech_OD01.m_eject,			_abt_,	CC01));
			Mech_OD01.AddTransition(new WeaponTransition( Mech_OD01.m_onCK,				_abt_,	CC01));
			Mech_OD01.AddTransition(new WeaponTransition( Mech_OD01.m_chamber,			_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( CD01,				__M__,	Mech_CD01)); // charge from closed=1 with mag
		m_fsm.AddTransition(new WeaponTransition(  Mech_CD01,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CD01,		_fin_,	CC11));
			Mech_CD01.AddTransition(new WeaponTransition( Mech_CD01.m_start,			_abt_,	CD01));
			Mech_CD01.AddTransition(new WeaponTransition( Mech_CD01.m_eject,			_abt_,	CC01));
			Mech_CD01.AddTransition(new WeaponTransition( Mech_CD01.m_onCK,				_abt_,	CC01));
			Mech_CD01.AddTransition(new WeaponTransition( Mech_CD01.m_chamber,			_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( CC01, 			__M__,	Mech_CC01)); // charge from closed/charged with mag
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC01,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC01,		_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC01,		_abt_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC01,		_abt_,	CC11));
		
		// eject good cartridge
		m_fsm.AddTransition(new WeaponTransition( CC10,				__M__,	Mech_CC10)); // eject chamber nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC10,		_fin_,	CC00));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC10,		_abt_,	CC00, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC10,		_abt_,	CC10));	
		
		m_fsm.AddTransition(new WeaponTransition( CC11,				__M__,	Mech_CC11 )); // eject with mag with ammo
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC11,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC11,		_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC11,		_abt_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CC11,		_abt_,	CC11));

		// Trigger_CC11e
		m_fsm.AddTransition(new WeaponTransition( CC00,				__T__,	Trigger_CC00)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC00,	_fin_,	CD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC00, 	_dto_,	CD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC00, 	_abt_,	CD00));
		
		m_fsm.AddTransition(new WeaponTransition( CD00,				__T__,	Trigger_CD00)); // fire.uncocked
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD00,	_fin_,  CD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD00,	_dto_,  CD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD00,	_abt_,  CD00));
		
		m_fsm.AddTransition(new WeaponTransition( OD00,				__T__,	Trigger_OD00)); // opened fire.uncocked
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD00,	_fin_,  OD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD00,	_dto_,  OD00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD00,	_abt_,  OD00));
		
		m_fsm.AddTransition(new WeaponTransition( CD01,				__T__,	Trigger_CD01)); // fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD01,	_fin_,	CD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD01,	_dto_,  CD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CD01,	_abt_,  CD01));
		
		m_fsm.AddTransition(new WeaponTransition( OD01,				__T__,	Trigger_OD01)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD01,	_fin_,	OD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD01,	_dto_,	OD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_OD01,	_abt_,	OD01));
		
		m_fsm.AddTransition(new WeaponTransition( CJF0,				__T__,	Trigger_CJF0)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF0,	_fin_,	CJF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF0,	_dto_,	CJF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF0,	_abt_,	CJF0));
		
		m_fsm.AddTransition(new WeaponTransition( CJF1,				__T__,	Trigger_CJF1)); // opened fire.uncocked w mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF1,	_fin_,	CJF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF1,	_dto_,	CJF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CJF1,	_abt_,	CJF1));
		
		
		m_fsm.AddTransition(new WeaponTransition( CC10,				__T__,	Trigger_CC10)); // or fire.last (if not jammed)
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10,	_fin_,	CC00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10,	_rto_,	CC00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10,	_abt_,	CC00));
		
		m_fsm.AddTransition(new WeaponTransition( CC10,				__TJ_,	Trigger_CC10J)); // or fire.last (if not jammed)
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10J,	_fin_,	CJF0 ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10J,	_rto_,	CJF0 ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC10J,	_abt_,	CJF0 ));
		
		
		m_fsm.AddTransition(new WeaponTransition( CC11, 			__T__,	Trigger_CC11, NULL, new WeaponGuardHasAmmo(this))); // fire.normal + chamber next
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11,	_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11,	_rto_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11,	_abt_,	CC11));

		m_fsm.AddTransition(new WeaponTransition( CC11,				__T__,	Trigger_CC11L, NULL, new GuardNot(new WeaponGuardHasAmmo(this)))); // fire.last with mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11L,	_fin_,	OD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11L,	_rto_,	OD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11L,	_abt_,	OD01));
		
		m_fsm.AddTransition(new WeaponTransition( CC11,				__TJ_,	Trigger_CC11J )); // fire.last with mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11J,	_fin_,	CJF1 ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11J,	_rto_,	CJF1 ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC11J,	_abt_,	CJF1 ));
		
		m_fsm.AddTransition(new WeaponTransition( CC01,				__T__,	Trigger_CC01)); // fire.cocked with mag
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC01,	_fin_,	CD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC01,	_dto_,	CD01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CC01,	_abt_,	CD01));

		// load cartridge
		m_fsm.AddTransition(new WeaponTransition( CD00,				__L__,	Chamber_CD00)); // chamber from closed=1
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CD00,	_fin_,	CC10));
			Chamber_CD00.AddTransition(new WeaponTransition( Chamber_CD00.m_start,		_abt_,	CD00));
			Chamber_CD00.AddTransition(new WeaponTransition( Chamber_CD00.m_eject,		_abt_,	OD00));
			Chamber_CD00.AddTransition(new WeaponTransition( Chamber_CD00.m_chamber,	_abt_,	OD00));
			Chamber_CD00.AddTransition(new WeaponTransition( Chamber_CD00.m_w4t,		_abt_,	CC10));

		m_fsm.AddTransition(new WeaponTransition( OD00,				__L__,	Chamber_OD00)); // chamber from opened
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD00,	_fin_,	CC10));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD00,	_abt_,	OD00, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD00,	_abt_,	CC10));

		m_fsm.AddTransition(new WeaponTransition( CC00,				__L__,	Chamber_CC00)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CC00,	_fin_,	CC10));
			Chamber_CC00.AddTransition(new WeaponTransition( Chamber_CC00.m_start,		_abt_,	CC00));
			Chamber_CC00.AddTransition(new WeaponTransition( Chamber_CC00.m_eject,		_abt_,	OD00));
			Chamber_CC00.AddTransition(new WeaponTransition( Chamber_CC00.m_chamber,	_abt_,	OD00));
			Chamber_CC00.AddTransition(new WeaponTransition( Chamber_CC00.m_w4t,		_abt_,	CC10));
		
		// load cartridge with mag
		m_fsm.AddTransition(new WeaponTransition( CD01,				__L__,	Chamber_CD01, NULL, new WeaponGuardHasMag(this))); // chamber from closed=1 with mag
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CD01,	_fin_,	CC11));
			Chamber_CD01.AddTransition(new WeaponTransition( Chamber_CD01.m_start,		_abt_,	CD01));
			Chamber_CD01.AddTransition(new WeaponTransition( Chamber_CD01.m_eject,		_abt_,	OD01));
			Chamber_CD01.AddTransition(new WeaponTransition( Chamber_CD01.m_chamber,	_abt_,	OD01));
			Chamber_CD01.AddTransition(new WeaponTransition( Chamber_CD01.m_w4t,		_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( CC01,				__L__,	Chamber_CC01, NULL, new WeaponGuardHasMag(this))); // chamber from closed=1 with mag
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CC01, 	_fin_,	CC11));
			Chamber_CC01.AddTransition(new WeaponTransition( Chamber_CC01.m_start,		_abt_,	CC01));
			Chamber_CC01.AddTransition(new WeaponTransition( Chamber_CC01.m_eject,		_abt_,	OD01));
			Chamber_CC01.AddTransition(new WeaponTransition( Chamber_CC01.m_chamber,	_abt_,	OD01));
			Chamber_CC01.AddTransition(new WeaponTransition( Chamber_CC01.m_w4t,		_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( OD01,				__L__,	Chamber_OD01, NULL, new WeaponGuardHasMag(this))); // chamber from opened with mag
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD01,	_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD01,	_abt_,	OD01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_OD01,	_abt_,	CC11));;

		m_fsm.AddTransition(new WeaponTransition( CJF0,				__A__,	Attach_CJF0, NULL, new WeaponGuardCanAttachMag(this))); // attach from JAM/b1/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_CJF0,	_fin_,	CJF1));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CJF0,	_abt_,  CJF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CJF0,	_abt_,  CJF0));

// zdokumentovano az sem
		// attach mag with no ammo
		m_fsm.AddTransition(new WeaponTransition( CD00,				__A__,	Attach_CD00, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b0/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_CD00,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CD00,		_fin_,	CC11));
			Attach_CD00.AddTransition(new WeaponTransition( Attach_CD00.m_start,		_abt_,	CD00));
			Attach_CD00.AddTransition(new WeaponTransition( Attach_CD00.m_attach,		_abt_,	CD01));
			Attach_CD00.AddTransition(new WeaponTransition( Attach_CD00.m_onCK,			_abt_,	CC01));
			Attach_CD00.AddTransition(new WeaponTransition( Attach_CD00.m_chamber,		_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( CC10,				__A__,	Attach_CC10, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b1/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_CC10,		_fin_,	CC11));
			Attach_CC10.AddTransition(new WeaponTransition( Attach_CC10.m_start,		_abt_,	CC10));
			Attach_CC10.AddTransition(new WeaponTransition( Attach_CC10.m_attach,		_abt_,	CC11));
		
		m_fsm.AddTransition(new WeaponTransition( OD00,				__A__,	Attach_OD00, NULL, new WeaponGuardCanAttachMag(this))); // attach from OPE/b0/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_OD00,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_OD00,		_fin_,	CC11));
			Attach_OD00.AddTransition(new WeaponTransition( Attach_OD00.m_start,		_abt_,	OD00));
			Attach_OD00.AddTransition(new WeaponTransition( Attach_OD00.m_attach,		_abt_,	OD01));
			Attach_OD00.AddTransition(new WeaponTransition( Attach_OD00.m_onCK,			_abt_,	CC01));
			Attach_OD00.AddTransition(new WeaponTransition( Attach_OD00.m_chamber,		_abt_,	CC11));

		m_fsm.AddTransition(new WeaponTransition( CC00, 			__A__,	Attach_CC00, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b0/m0 charged
		m_fsm.AddTransition(new WeaponTransition(  Attach_CC00,	_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CC00,	_fin_,	CC11));
			Attach_CC00.AddTransition(new WeaponTransition( Attach_CC00.m_start,		_abt_,	CC00));
			Attach_CC00.AddTransition(new WeaponTransition( Attach_CC00.m_attach,		_abt_,	CC01));
			Attach_CC00.AddTransition(new WeaponTransition( Attach_CC00.m_onCK,			_abt_,	CC01));
			Attach_CC00.AddTransition(new WeaponTransition( Attach_CC00.m_chamber,		_abt_,	CC11));

		// replace magazine
		m_fsm.AddTransition(new WeaponTransition( CD01,				__S__,	Reload_CD01, NULL, new WeaponGuardCanSwapMag(this))); // swap in CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_CD01,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CD01,		_fin_,	CC11));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_start,		_abt_,	CD01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_eject,		_abt_,	CD01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_detach,		_abt_,	CD01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_hideOld,		_abt_,	CD00));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_swapMags,		_abt_,	CD01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_attach,		_abt_,	CD01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_onCK,			_abt_,	CC01));
			Reload_CD01.AddTransition(new WeaponTransition( Reload_CD01.m_chamber,		_abt_,	CC11));


		m_fsm.AddTransition(new WeaponTransition( CC11, 			__S__, Reload_CC11, NULL, new WeaponGuardCanSwapMag(this))); // swap in CLO/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC11,	_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC11,	_abt_,	CC11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC11,	_abt_,	CC10));

		m_fsm.AddTransition(new WeaponTransition( CC01, 			__S__,	Reload_CC01, NULL, new WeaponGuardCanSwapMag(this))); // CLO/b0/m1 swap with empty mag (no chamber)
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC01,	_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC01,	_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC01,	_abt_,	CC11, NULL, new WeaponGuardCurrentChamberFull(this)) );
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC01,	_abt_,	CC01, NULL, new WeaponGuardHasMag(this)) );
		m_fsm.AddTransition(new WeaponTransition(  Reload_CC01,	_abt_,	CC00));

		m_fsm.AddTransition(new WeaponTransition( OD01, 			__S__,	Reload_OD01, NULL, new WeaponGuardCanSwapMag(this))); // OPE/b0/m1 swap with empty mag (no chamber)
		m_fsm.AddTransition(new WeaponTransition(  Reload_OD01,	_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_OD01,	_fin_,	CC11));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_start,			_abt_,	OD01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_eject,			_abt_,	OD01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_detach,		_abt_,	OD01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_hideOld,		_abt_,	OD00));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_swapMags,		_abt_,	OD01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_attach,		_abt_,	OD01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_onCK,			_abt_,	CC01));
			Reload_OD01.AddTransition(new WeaponTransition(Reload_OD01.m_chamber,		_abt_,	CC11));

		m_fsm.AddTransition(new WeaponTransition( CJF1,			__S__,	Reload_CJF1, NULL, new WeaponGuardCanSwapMag(this))); // swap in JAM/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_CJF1,	_fin_,	CJF1));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CJF1,	_abt_,	CC11, NULL, new WeaponGuardCurrentChamberFull(this)) );
		m_fsm.AddTransition(new WeaponTransition(  Reload_CJF1,	_abt_,	CC01, NULL, new WeaponGuardHasMag(this)) );
		m_fsm.AddTransition(new WeaponTransition(  Reload_CJF1,	_abt_,	CC00));


		// detach magazine
		m_fsm.AddTransition(new WeaponTransition( OD01,			__D__,	Detach_OD01, NULL, new WeaponGuardCanDetachMag(this))); // detach from OPE/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_OD01,	_fin_,	OD00));
		m_fsm.AddTransition(new WeaponTransition(  Detach_OD01,	_abt_,	OD01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_OD01,	_abt_,	OD00));
		
		m_fsm.AddTransition(new WeaponTransition( CC11,			__D__,	Detach_CC11, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC11,	_fin_,	CC10));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC11,	_abt_,	CC11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC11,	_abt_,	CC10));
		
		m_fsm.AddTransition(new WeaponTransition( CC01,			__D__,	Detach_CC01, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC01,	_fin_,	CC00));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC01,	_abt_,	CC01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CC01,	_abt_,	CC00));
		
		m_fsm.AddTransition(new WeaponTransition( CD01,			__D__,	Detach_CD01, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b0/m1 dischg
		m_fsm.AddTransition(new WeaponTransition(  Detach_CD01,	_fin_,	CD00));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CD01,	_abt_,	CD01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CD01,	_abt_,	CD00));
		
		m_fsm.AddTransition(new WeaponTransition( CJF1,			__D__,	Detach_CJF1, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b1/m1 jammed
		m_fsm.AddTransition(new WeaponTransition(  Detach_CJF1,	_fin_,	CJF0));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CJF1,	_abt_,	CJF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CJF1,	_abt_,	CJF0));


		// unjam
		m_fsm.AddTransition(new WeaponTransition( CJF0,			__U__,	Unjam_CJF0)); // unjam nomag
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF0,		_fin_,	CJF0, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF0,		_fin_,	CC00, NULL, new GuardNot(new WeaponGuardJammed(this))));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF0,		_abt_,	CJF0));
		
		m_fsm.AddTransition(new WeaponTransition( CJF1,			__U__,	Unjam_CJF1)); // unjam with mag with ammo
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_fin_,	CJF1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_fin_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_fin_,	CC11));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_abt_,	CJF1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_abt_,	CC01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_CJF1,		_abt_,	CC11));
		

		m_fsm.SetInitialState(CD00);
		SelectionBulletHide();
		HideMagazine();
		m_fsm.Start();
	}
}

class ExpansionCrossbow_Base extends RifleSingleShot_Base
{
	void ExpansionCrossbow_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}
}

class Expansion_MPX_Base : RifleBoltLock_Base
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_MPX_Base::EEItemAttached - Start");
		#endif

		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsDown(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_MPX_Base::EEItemAttached - End");
		#endif
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_MPX_Base::EEItemDettached - Start");
		#endif

		super.EEItemDetached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsUp(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_MPX_Base::EEItemDettached - End");
		#endif
	}	

	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_MPX_Base::SpawnRecoilObject");
		#endif

		return new M4a1Recoil(this);
	}
}

class Expansion_G36_Base : RifleBoltLock_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_G36_Base::SpawnRecoilObject");
		#endif

		return new M4a1Recoil(this);
	}
}

class Expansion_M79_Base extends RifleSingleShot_Base
{
	void Expansion_M79_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}
}
class ExpansionRPG7Base extends RifleSingleShot_Base
{
	void ExpansionRPG7Base()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}
}

class Expansion_M16_Base : RifleBoltLock_Base
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemAttached - Start");
		#endif		

		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsDown(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_16_Base::EEItemAttached - End");
		#endif		
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - Start");
		#endif

		super.EEItemDetached(item,slot_name);
		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsUp(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - End");
		#endif
	}	

	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::SpawnRecoilObject");
		#endif
		return new M4a1Recoil(this);
	}
}

class Expansion_Kedr_Base : RifleBoltLock_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_Kedr_Base::SpawnRecoilObject");
		#endif

		return new Cz61Recoil(this);
	}
}

class Expansion_BenelliM4_Base: ExpansionSemiShotGun
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp133Recoil(this);
	}
}

class Expansion_M9_Base : Pistol_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M9_Base::SpawnRecoilObject");
		#endif

		return new Cz75Recoil(this);
	}
}

class Expansion_MP5_Base : RifleBoltLock_Base
{
	void Expansion_MP5_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp5kRecoil(this);
	}	
}

class Expansion_DT11_Base : DoubleBarrel_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new B95Recoil(this);
	}
}
class Expansion_PMII25Optic extends OpticBase
{


} 

class Expansion_M1A_Base : RifleBoltLock_Base
{
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		if (optic && Expansion_PMII25Optic.Cast(optic))
			return true;
		return super.CanEnterIronsights();
	}
	override RecoilBase SpawnRecoilObject()
	{
		return new SvdRecoil(this);
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        if  ( attachment.IsKindOf("ItemOptics") )
        {            
            if ( FindAttachmentBySlotName("weaponOptics") != NULL || FindAttachmentBySlotName("ExpansionSniperOptics") != NULL || ( !attachment.IsKindOf("Expansion_PMII25Optic") && FindAttachmentBySlotName("Expansion_M1AScopeRail") != NULL ))
            {
                return false;
            }    
        }          
		if  ( attachment.IsKindOf("Expansion_PMII25Optic") )
        {            
            if ( FindAttachmentBySlotName("Expansion_M1AScopeRail") == NULL)
            {
                return false;
            }    
        }     
		if (attachment.IsKindOf("Expansion_M1A_RailAtt") )
        {            
            if ( FindAttachmentBySlotName("weaponOptics") != NULL)
            {
                return false;
            }    
        }  
        return super.CanReceiveAttachment(attachment, slotId);
    }
	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( attachment.IsKindOf("Expansion_M1A_RailAtt") && FindAttachmentBySlotName("ExpansionSniperOptics") )
			return false;
			

		return super.CanReleaseAttachment(attachment);
	}
	override bool CanDisplayAttachmentSlot( string slot_name )
    {    
        if (!super.CanDisplayAttachmentSlot(slot_name))
            return false;   
		
        if ( slot_name == "ExpansionSniperOptics" )
        {
            return this.FindAttachmentBySlotName("ExpansionSniperOptics") != NULL;
        }
        if ( slot_name == "weaponOptics" )
        {
            return this.FindAttachmentBySlotName("ExpansionSniperOptics") == NULL;    
        }        
        return true;
    }		
}
class Expansion_AWM_Base: BoltActionRifle_Base
{
	ref WeaponStateBase C00;
	ref	WeaponStateBase C10;
	ref	WeaponStateBase C01;
	ref	WeaponStateBase C11;
	ref	WeaponStateBase CF0;
	ref	WeaponStateBase CF1;
	ref	WeaponStateBase JF0;
	ref	WeaponStateBase JF1;
   
	override bool CanDisplayAttachmentSlot( string slot_name )
    {    
        if (!super.CanDisplayAttachmentSlot(slot_name))
            return false;   
		
        if ( slot_name == "sniperOptics" )
        {
            return this.FindAttachmentBySlotName("sniperOptics") != NULL;
        }

        if ( slot_name == "weaponOptics" )
        {
            return this.FindAttachmentBySlotName("sniperOptics") == NULL;    
        } 

        return true;
    }	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        if  ( attachment.IsKindOf("ItemOptics") )
        {            
            if (  FindAttachmentBySlotName("ExpansionSniperOptics") != NULL )
            {
                return false;
            }    
        }          
		if  ( attachment.IsKindOf("Expansion_PMII25Optic") )
        {            
            if ( FindAttachmentBySlotName("weaponOptics") != NULL)
            {
                return false;
            }    
        }     
        return super.CanReceiveAttachment(attachment, slotId);
    }
	override RecoilBase SpawnRecoilObject()
	{
		return new CZ527Recoil(this);
	}
	
	override void InitStateMachine()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_BULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_BULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_NOBULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_NOBULLET)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH)));

		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END)));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL)));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)));
		//m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));

		// setup state machine
		// basic weapon states
		// close-jammed | nobullet-bullet-fireout bullet | nomag-mag
		// regexp: [CLJ][01F][01]
		C00 = new CZ527_CLO_BU0_MA0(this, NULL, RBLAnimState.DEFAULT);
		C10 = new CZ527_CLO_BU1_MA0(this, NULL, RBLAnimState.DEFAULT);
		C01 = new CZ527_CLO_BU0_MA1(this, NULL, RBLAnimState.DEFAULT);
		C11 = new CZ527_CLO_BU1_MA1(this, NULL, RBLAnimState.DEFAULT);
		CF0 = new CZ527_CLO_BUF_MA0(this, NULL, RBLAnimState.DEFAULT);
		CF1 = new CZ527_CLO_BUF_MA1(this, NULL, RBLAnimState.DEFAULT);
		JF0 = new CZ527_JAM_BU1_MA0(this, NULL, RBLAnimState.JAMMED);
		JF1 = new CZ527_JAM_BU1_MA1(this, NULL, RBLAnimState.JAMMED);

		// unstable (intermediate) states
		WeaponStateBase Mech_C00 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponStateBase Mech_C01 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponStateBase Mech_C10 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponStateBase Mech_C11 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponStateBase Mech_CF0 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		WeaponStateBase Mech_CF1 = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, GetWeaponSpecificCommand(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
	
		WeaponStateBase Trigger_C00 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY)); // cock without clip
		WeaponStateBase Trigger_C10 = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL)); // cock with clip
		WeaponStateBase Trigger_C11 = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		WeaponStateBase Trigger_C01 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_CF0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_CF1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_JF0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		WeaponStateBase Trigger_JF1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
	
		//Fire and jammed
		WeaponStateBase Trigger_C10J = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM)); // cock with clip
		WeaponStateBase Trigger_C11J = new WeaponFire(this, NULL, WeaponActions.FIRE, GetWeaponSpecificCommand(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM));
		
		// Unjam
		WeaponStateBase Unjam_JF0 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		WeaponStateBase Unjam_JF1 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, GetWeaponSpecificCommand(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));

		WeaponChambering Chamber_C00 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		WeaponChambering Chamber_C01 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		WeaponChambering Chamber_CF0 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		WeaponChambering Chamber_CF1 = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, GetWeaponSpecificCommand(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));

		WeaponAttachMagazine Attach_C00 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_NOBULLET));
		WeaponAttachMagazine Attach_CF0 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_NOBULLET));
		WeaponAttachMagazine Attach_C10 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_BULLET));
		WeaponAttachMagazine Attach_JF0 = new WeaponAttachMagazine(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_BULLET));
	
		WeaponReplacingMagAndChamberNext Reload_C11 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_BULLET));
		WeaponReplacingMagAndChamberNext Reload_C01 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_NOBULLET));
		WeaponReplacingMagAndChamberNext Reload_CF1 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_NOBULLET));
		WeaponReplacingMagAndChamberNext Reload_JF1 = new WeaponReplacingMagAndChamberNext(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_BULLET));
	
		WeaponDetachingMag Detach_C11 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_C01 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_CF1 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		WeaponDetachingMag Detach_JF1 = new WeaponDetachingMag(this, NULL, WeaponActions.RELOAD, GetWeaponSpecificCommand(WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH));
		
		//WeaponStateBase chmMag = new WeaponChamberFromAttMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __TJ_ = new WeaponEventTriggerToJam;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __A__ = new WeaponEventAttachMagazine;
		WeaponEventBase __D__ = new WeaponEventDetachMagazine;
		WeaponEventBase __S__ = new WeaponEventSwapMagazine;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();

		// order: Discharged-Charged | nobullet-bullet | nomag-mag
		// charging
		m_fsm.AddTransition(new WeaponTransition( C00,				__M__,	Mech_C00)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_C00,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C00,		_abt_,	C00));

		m_fsm.AddTransition(new WeaponTransition( C10,				__M__,	Mech_C10)); // charge from bullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_C10,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C10,		_abt_,	C00, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C10,		_abt_,	C10));
		
		m_fsm.AddTransition(new WeaponTransition( CF0,				__M__,	Mech_CF0)); // charge from bullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF0,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF0,		_abt_,	CF0, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF0,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( C11,				__M__,	Mech_C11)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_C11,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C11,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C11,		_abt_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C11,		_abt_,	C11));
		
		m_fsm.AddTransition(new WeaponTransition( C01,				__M__,	Mech_C01)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_C01,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C01,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C01,		_abt_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_C01,		_abt_,	C11));
		
		m_fsm.AddTransition(new WeaponTransition( CF1,				__M__,	Mech_CF1)); // charge from dischg nobullet nomag
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF1,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF1,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF1,		_abt_,	CF1, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF1,		_abt_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_CF1,		_abt_,	C11));
		
		// fire
		m_fsm.AddTransition(new WeaponTransition( C00,				__T__,	Trigger_C00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C00,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C00,		_dto_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C00,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( C01,				__T__,	Trigger_C01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C01,		_fin_,	C01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C01,		_dto_,	C01));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C01,		_abt_,	C01));
		
		m_fsm.AddTransition(new WeaponTransition( CF0,				__T__,	Trigger_CF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF0,		_fin_,	CF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF0,		_dto_,	CF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF0,		_abt_,	CF0));
		
		m_fsm.AddTransition(new WeaponTransition( CF1,				__T__,	Trigger_CF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF1,		_fin_,	CF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF1,		_dto_,	CF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_CF1,		_abt_,	CF1));
		
		m_fsm.AddTransition(new WeaponTransition( JF0,				__T__,	Trigger_JF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF0,		_fin_,	JF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF0,		_dto_,	JF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF0,		_abt_,	JF0));
		
		m_fsm.AddTransition(new WeaponTransition( JF1,				__T__,	Trigger_JF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF1,		_fin_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF1,		_dto_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_JF1,		_abt_,	JF1));
		
		m_fsm.AddTransition(new WeaponTransition( C10,				__T__,	Trigger_C10));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10,		_rto_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( C11,				__T__,	Trigger_C11));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11,		_fin_,	CF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11,		_rto_,	CF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11,		_abt_,	CF1));
		
		m_fsm.AddTransition(new WeaponTransition( C11,				__TJ_,	Trigger_C11J));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11J,	_fin_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11J,	_rto_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C11J,	_abt_,	JF1));
		
		m_fsm.AddTransition(new WeaponTransition( C10,				__TJ_,	Trigger_C10J ));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10J,	_fin_,	JF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10J,	_rto_,	JF0));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_C10J,	_abt_,	JF0));	

		// load cartridge
		m_fsm.AddTransition(new WeaponTransition( C00,				__L__,	Chamber_C00)); // chamber from closed=1
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C00,		_fin_,	C10));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C00,		_abt_,	C00, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C00,		_abt_,	C10));

		m_fsm.AddTransition(new WeaponTransition( C01,				__L__,	Chamber_C01)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C01,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C01,		_abt_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_C01,		_abt_,	C11));
		
		m_fsm.AddTransition(new WeaponTransition( CF0,				__L__,	Chamber_CF0)); // chamber from closed=1
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF0,		_fin_,	C10));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF0,		_abt_,	CF0, NULL, new WeaponGuardCurrentChamberFiredOut(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF0,		_abt_,	C00, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF0,		_abt_,	C10));

		m_fsm.AddTransition(new WeaponTransition( CF1,				__L__,	Chamber_CF1)); // chamber from closed charged
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF1,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF1,		_abt_,	CF1, NULL, new WeaponGuardCurrentChamberFiredOut(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF1,		_abt_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this) ));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_CF1,		_abt_,	C11));

		// attach magazine (no mag in weapon)
		m_fsm.AddTransition(new WeaponTransition( C00,				__A__,	Attach_C00, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b0/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_C00,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C00,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C00,		_abt_,	C11, NULL, new WeaponGuardCurrentChamberFull(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C00,		_abt_,	C01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C00,		_abt_,	C00));

		m_fsm.AddTransition(new WeaponTransition( C10,				__A__,	Attach_C10, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b1/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_C10,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C10,		_abt_,	C11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_C10,		_abt_,	C10));
		
		m_fsm.AddTransition(new WeaponTransition( CF0,				__A__,	Attach_CF0, NULL, new WeaponGuardCanAttachMag(this))); // attach from CLO/b1/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_abt_,	CF1, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_abt_,	C11, NULL, new WeaponGuardCurrentChamberFull(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_abt_,	C01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_CF0,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( JF0,				__A__,	Attach_JF0, NULL, new WeaponGuardCanAttachMag(this))); // attach from JAM/b1/m0
		m_fsm.AddTransition(new WeaponTransition(  Attach_JF0,		_fin_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Attach_JF0,		_abt_,	JF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Attach_JF0,		_abt_,	JF0));

		// replace magazine
		m_fsm.AddTransition(new WeaponTransition( C11,				__S__,	Reload_C11, NULL, new WeaponGuardCanSwapMag(this))); // swap in Chg/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_C11,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C11,		_abt_,	C11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C11,		_abt_,	C10));
		
		m_fsm.AddTransition(new WeaponTransition( C01,				__S__,	Reload_C01, NULL, new WeaponGuardCanSwapMag(this))); // swap in CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_C01,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C01,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C01,		_abt_,	C11, NULL, new WeaponGuardCurrentChamberFull(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C01,		_abt_,	C01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_C01,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( CF1,				__S__,	Reload_CF1, NULL, new WeaponGuardCanSwapMag(this))); // swap in CLO/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_abt_,	CF1, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_abt_,	C11, NULL, new WeaponGuardCurrentChamberFull(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_abt_,	C01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_CF1,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( JF1,				__S__,	Reload_JF1, NULL, new WeaponGuardCanSwapMag(this))); // swap in Chg/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Reload_JF1,		_fin_,	JF1));
		m_fsm.AddTransition(new WeaponTransition(  Reload_JF1,		_abt_,	JF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Reload_JF1,		_abt_,	JF0));

		// detach magazine
		m_fsm.AddTransition(new WeaponTransition( C11,				__D__,	Detach_C11, NULL, new WeaponGuardCanDetachMag(this))); // detach from Chg/b1/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_C11,		_fin_,	C10));
		m_fsm.AddTransition(new WeaponTransition(  Detach_C11,		_abt_,	C11, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_C11,		_abt_,	C10));
		
		m_fsm.AddTransition(new WeaponTransition( C01,				__D__,	Detach_C01, NULL, new WeaponGuardCanDetachMag(this))); // detach from Chg/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_C01,		_fin_,	C00));
		m_fsm.AddTransition(new WeaponTransition(  Detach_C01,		_abt_,	C01, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_C01,		_abt_,	C00));
		
		m_fsm.AddTransition(new WeaponTransition( CF1,				__D__,	Detach_CF1, NULL, new WeaponGuardCanDetachMag(this))); // detach from Chg/b0/m1
		m_fsm.AddTransition(new WeaponTransition(  Detach_CF1,		_fin_,	CF0));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CF1,		_abt_,	CF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_CF1,		_abt_,	CF0));
		
		m_fsm.AddTransition(new WeaponTransition( JF1,				__D__,	Detach_JF1, NULL, new WeaponGuardCanDetachMag(this))); // detach from CLO/b1/m1 jammed
		m_fsm.AddTransition(new WeaponTransition(  Detach_JF1,		_fin_,	JF0));
		m_fsm.AddTransition(new WeaponTransition(  Detach_JF1,		_abt_,	JF1, NULL, new WeaponGuardHasMag(this)));
		m_fsm.AddTransition(new WeaponTransition(  Detach_JF1,		_abt_,	JF0));
		
		// unjam
		m_fsm.AddTransition(new WeaponTransition( JF0,				__U__,	Unjam_JF0)); // unjam nomag
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF0,		_fin_,	JF0, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF0,		_fin_,	C00, NULL, new GuardNot(new WeaponGuardJammed(this))));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF0,		_abt_,	JF0));
		
		m_fsm.AddTransition(new WeaponTransition( JF1,				__U__,	Unjam_JF1)); // unjam with mag with ammo
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF1,		_fin_,	JF1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF1,		_fin_,	C01, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF1,		_fin_,	C11));
		m_fsm.AddTransition(new WeaponTransition(  Unjam_JF1,		_abt_,	JF1));

		m_fsm.SetInitialState(C00);

		SelectionBulletHide();
		HideMagazine();

		m_fsm.Start();
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(FirearmActionDetachMagazine);
		//AddAction(ActionAdvancedDetachMagazine);
	}
}


