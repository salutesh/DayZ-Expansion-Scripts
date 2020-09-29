/**
 * ExpansionWeaponFire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionWeaponFireBase
{
	void FireServer(Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir)
	{
	}

	void FireClient(Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir)
	{
	}
};

bool ExpansionTryFireWeapon( DayZPlayerImplement player, Weapon_Base weapon, int muzzleIndex )
{
	if ( !TryFireWeapon( weapon, muzzleIndex ) )
		return false;

	typename firehandle = weapon.GetExpansionFireType();
	if ( !firehandle )
		return false;

	if ( GetGame().IsServer() )
	{
		weapon.ExpansionSetNextFire( muzzleIndex );
	}

	if (GetGame().IsClient() || !GetGame().IsMultiplayer())
	{
		ExpansionWeaponFireBase fireBase;
		if ( Class.CastTo( fireBase, firehandle.Spawn() ) )
		{
			vector usti_hlavne_position = weapon.GetSelectionPositionLS("usti hlavne");
			vector konec_hlavne_position = weapon.GetSelectionPositionLS("konec hlavne");
			float barrel_length = vector.Distance(usti_hlavne_position, konec_hlavne_position);

			vector w_usti_hlavne_position = weapon.ModelToWorld(usti_hlavne_position);
			vector w_konec_hlavne_position = weapon.ModelToWorld(konec_hlavne_position);

			vector direction = vector.Direction(w_konec_hlavne_position, w_usti_hlavne_position).Normalized();

			vector position = weapon.GetFirePosition( player ) + (direction * barrel_length);

			fireBase.FireClient( weapon, muzzleIndex, player, position, direction );

			GetRPCManager().SendRPC("DayZExpansion", "ConfirmWeaponFire", new Param1<vector>(direction), true, NULL, weapon );
		}
	}

	return true;
};

class ExpansionWeaponFire extends WeaponStartAction
{
	float m_dtAccumulator;

	override bool IsWaitingForActionFinish() { return true; }

	override void OnEntry(WeaponEventBase e)
	{
		m_dtAccumulator = 0;

		wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang!");

		DayZPlayerImplement player;
		if ( Class.CastTo( player, e.m_player ) )
		{
			int muzzleIndex = m_weapon.GetCurrentMuzzle();

			if ( ExpansionTryFireWeapon( player, m_weapon, muzzleIndex ) )
			{
				player.GetAimingModel().SetRecoil( m_weapon );
			}
		}

		super.OnEntry(e);
	}

	override void OnUpdate(float dt)
	{
		m_dtAccumulator += dt;

		DayZPlayer p;
		Class.CastTo(p, m_weapon.GetHierarchyParent());

		int muzzleIndex = m_weapon.GetCurrentMuzzle();
		float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
		if (m_dtAccumulator >= reloadTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
	}

	override void OnExit(WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};

class ExpansionWeaponFireToJam extends WeaponStartAction
{
	float m_dtAccumulator;

	override bool IsWaitingForActionFinish() { return true; }

	override void OnEntry(WeaponEventBase e)
	{
		m_dtAccumulator = 0;

		wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang! and jam?");

		DayZPlayerImplement player;
		if ( Class.CastTo( player, e.m_player ) )
		{
			int muzzleIndex = m_weapon.GetCurrentMuzzle();

			if ( ExpansionTryFireWeapon( player, m_weapon, muzzleIndex ) )
			{
				player.GetAimingModel().SetRecoil( m_weapon );

				m_weapon.SetJammed(true);
			}
		}

		super.OnEntry(e);
	}

	override void OnUpdate(float dt)
	{
		m_dtAccumulator += dt;

		DayZPlayer p;
		Class.CastTo(p, m_weapon.GetHierarchyParent());

		int muzzleIndex = m_weapon.GetCurrentMuzzle();
		float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
		if (m_dtAccumulator >= reloadTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
	}

	override void OnExit(WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};

class ExpansionWeaponFireAndChamber extends ExpansionWeaponFire
{
	override void OnEntry(WeaponEventBase e)
	{
		super.OnEntry(e);

		if (!m_weapon.IsJammed())
		{
			wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " ejected fired out casing");
			int mi = m_weapon.GetCurrentMuzzle();
			m_weapon.EjectCasing(mi);
			m_weapon.EffectBulletHide(mi);
			m_weapon.SelectionBulletHide();

			pushToChamberFromAttachedMagazine(m_weapon, mi);
		}
	}
};

class ExpansionWeaponFireAndChamberFromInnerMagazine extends ExpansionWeaponFire
{
	override void OnEntry(WeaponEventBase e)
	{
		super.OnEntry(e);

		if (!m_weapon.IsJammed())
		{
			wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " ejected fired out casing");
			int mi = m_weapon.GetCurrentMuzzle();
			m_weapon.EjectCasing(mi);
			m_weapon.EffectBulletHide(mi);
			m_weapon.SelectionBulletHide();

			pushToChamberFromInnerMagazine(m_weapon, mi);
		}
	}
};

class ExpansionWeaponFireLast extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;

	float m_dtAccumulator;
	ref ExpansionWeaponFire m_fire;
	ref WeaponEjectCasing_W4T m_eject;

	void ExpansionWeaponFireLast(Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_fire = new ExpansionWeaponFire(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectCasing_W4T(m_weapon, this);

		// events
		WeaponEventAnimBulletEject __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventReloadTimeout __to_ = new WeaponEventReloadTimeout;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		// transitions
		m_fsm.AddTransition(new WeaponTransition(m_fire, __be_, m_eject, NULL, new GuardNot(WeaponGuardJammed(m_weapon))));
		m_fsm.AddTransition(new WeaponTransition(m_eject, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(m_eject, __to_, NULL));
		m_fsm.AddTransition(new WeaponTransition(m_fire, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(m_fire, __to_, NULL));

		m_fsm.SetInitialState(m_fire);
	}

	override void OnEntry(WeaponEventBase e)
	{
		super.OnEntry(e);
		m_dtAccumulator = 0;
	}

	override void OnUpdate(float dt)
	{
		m_dtAccumulator += dt;

		WeaponStateBase curstate = GetFSM().GetCurrentState();

		if (curstate == m_eject)
		{
			DayZPlayer p;
			Class.CastTo(p, m_weapon.GetHierarchyParent());

			if (p)
			{
				HumanInputController hic = p.GetInputController();

				int muzzleIndex = m_weapon.GetCurrentMuzzle();
				float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
				if (hic.IsUseButton() && m_dtAccumulator >= reloadTime)
					if (m_weapon.CanProcessWeaponEvents())
						m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
			}
		}
	}

	override void OnExit(WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};

class ExpansionWeaponFireAndChamberNext extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;

	float m_dtAccumulator;
	ref ExpansionWeaponFire m_fire;

	void ExpansionWeaponFireAndChamberNext (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_fire = new ExpansionWeaponFireAndChamber(m_weapon, this, m_action, m_actionType);

		// events
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventAnimBulletEject  __be_ = new WeaponEventAnimBulletEject;
		WeaponEventReloadTimeout __to_ = new WeaponEventReloadTimeout;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		// transitions
		m_fsm.AddTransition(new WeaponTransition(m_fire, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(m_fire, __to_, NULL));

		m_fsm.SetInitialState(m_fire);
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		m_dtAccumulator = 0;
	}

	override void OnUpdate (float dt)
	{
		m_dtAccumulator += dt;
		DayZPlayer p;
		Class.CastTo(p, m_weapon.GetHierarchyParent());
		if( p )
		{
			HumanInputController hic = p.GetInputController();

			int muzzleIndex = m_weapon.GetCurrentMuzzle();
			float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
			if ( hic.IsUseButton() && m_dtAccumulator >= reloadTime)
				if (m_weapon.CanProcessWeaponEvents())
					m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
		}
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};