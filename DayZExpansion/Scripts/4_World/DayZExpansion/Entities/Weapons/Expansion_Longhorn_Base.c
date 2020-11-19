class Expansion_Longhorn: Weapon_Base
{
	#ifdef DAYZ_1_10
	override void InitStateMachine()
	{
		// setup abilities
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));

		// setup state machine
		// basic weapon states
		WeaponStateBase E = new FLAREEmpty(this, NULL, FLAREAnimState.UNCOCKED);
		WeaponStateBase F = new FLAREFireout(this, NULL, FLAREAnimState.UNCOCKED);
		WeaponStateBase L = new FLARELoaded(this, NULL, FLAREAnimState.COCKED);
		// unstable (intermediate) states
		WeaponStateBase Mech_F = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase Mech_L = new WeaponEjectBullet(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED);
		
		WeaponStateBase Chamber_E = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		WeaponStateBase Chamber_F = new WeaponChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		//WeaponStateBase Chamber_L = new RifleReChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED);
		
		WeaponStateBase Trigger_E = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);
		WeaponStateBase Trigger_L = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase Trigger_F = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED);


		// events
		WeaponEventBase __M__ = new WeaponEventMechanism;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __TJ_ = new WeaponEventTriggerToJam;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		// charging
		m_fsm.AddTransition(new WeaponTransition( E,			__M__,	Mech_F));
		m_fsm.AddTransition(new WeaponTransition( F,			__M__,	Mech_F));
		m_fsm.AddTransition(new WeaponTransition(  Mech_F,		_fin_,	E));
		m_fsm.AddTransition(new WeaponTransition(  Mech_F,		_abt_,	F, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_F,		_abt_,	E));
		
		// eject good cartridge
		m_fsm.AddTransition(new WeaponTransition( L,			__M__,	Mech_L));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_fin_,	E));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_abt_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Mech_L,		_abt_,	L));

		m_fsm.AddTransition(new WeaponTransition( E,			__L__,	Chamber_F));
		m_fsm.AddTransition(new WeaponTransition( F,			__L__,	Chamber_F));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_F,	_fin_,	L));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_F,	_abt_,	F, NULL, new WeaponGuardCurrentChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_F,	_abt_,	E, NULL, new WeaponGuardCurrentChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(  Chamber_F,	_abt_,	L));

		// fire
		m_fsm.AddTransition(new WeaponTransition( E,			__T__,	Trigger_E)); // fire cocked
		m_fsm.AddTransition(new WeaponTransition(  Trigger_E,	_fin_,	E));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_E,	_dto_,	E));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_E,	_abt_,	E));
		
		m_fsm.AddTransition(new WeaponTransition( F,			__T__,	Trigger_F)); // fire uncocked
		m_fsm.AddTransition(new WeaponTransition(  Trigger_F,	_fin_,	F));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_F,	_dto_,	F));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_F,	_abt_,	F));
		
		m_fsm.AddTransition(new WeaponTransition( L,			__T__,	Trigger_L)); // a) fire if not jammed
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L,	_fin_,	F));
		m_fsm.AddTransition(new WeaponTransition(  Trigger_L,	_abt_,	F));
		

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

		SelectionBulletHide();

		m_fsm.Start();
	}
	#endif
};