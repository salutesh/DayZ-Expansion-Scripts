modded class WeaponStateBase
{
	void eAI_Vanilla_OnEntry(WeaponEventBase e)
	{
		if (HasFSM() && !m_fsm.IsRunning())
		{
			if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString() + "  Has Sub-FSM! Starting submachine..."); }
			m_fsm.Start(e);
		}
		else
			if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString()); }
	}
}

modded class WeaponStartAction
{
	override void eAI_Vanilla_OnEntry(WeaponEventBase e)
	{
		super.eAI_Vanilla_OnEntry(e);
		if (e)
		{
			if (e.m_player)
			{
				HumanCommandWeapons hcw = e.m_player.GetCommandModifier_Weapons();
				if (hcw)
				{
					HumanCommandAdditives ad = e.m_player.GetCommandModifier_Additives();
					if (ad)
						ad.CancelModifier();
					
					hcw.StartAction(m_action, m_actionType);
		
					if (hcw.GetRunningAction() == m_action && hcw.GetRunningActionType() == m_actionType)
					{
						if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("HCW: playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished()); }
					}
					else
						Error("HCW: NOT playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType) + " fini=" + hcw.IsActionFinished());
				}
				else
					if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("---: remote playing A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType)); }
			}
			else
			{
				if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("---: warning, no player wants to play A=" + typename.EnumToString(WeaponActions, m_action) + " AT=" + WeaponActionTypeToString(m_action, m_actionType)); }
			}
		}
	}
}

modded class WeaponFire
{
	override void OnEntry(WeaponEventBase e)
	{
		if (e)
		{
			eAIBase p;
			if (Class.CastTo(p, e.m_player))
			{
				m_dtAccumulator = 0;
		
				if (LogManager.IsWeaponLogEnable()) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang!"); }
				int mi = m_weapon.GetCurrentMuzzle();
				if (m_weapon.eAI_Fire(mi, p.GetBonePositionWS(p.GetBoneIndexByName("neck")), p.GetWeaponAimDirection(), vector.Forward))
				{
					p.GetAimingModel().SetRecoil(m_weapon);
					m_weapon.OnFire(mi);
				}
				super.eAI_Vanilla_OnEntry(e);
				return;
			}
		}
		super.OnEntry(e);
	}
}

modded class WeaponFireWithEject
{
	override void OnEntry(WeaponEventBase e)
	{
		eAIBase p;
		if (Class.CastTo(p, e.m_player))
		{
			m_dtAccumulator = 0;

			if (LogManager.IsWeaponLogEnable()) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang!"); }
			int mi = m_weapon.GetCurrentMuzzle();
			if (m_weapon.eAI_Fire(mi, p.GetBonePositionWS(p.GetBoneIndexByName("neck")), p.GetWeaponAimDirection(), vector.Forward))
			{
				p.GetAimingModel().SetRecoil(m_weapon);
				m_weapon.EjectCasing(mi);
				m_weapon.EffectBulletHide(mi);
				m_weapon.OnFire(mi);
			}
			super.eAI_Vanilla_OnEntry(e);
			return;
		}
		super.OnEntry(e);
	}
}

modded class WeaponFireMultiMuzzle
{
	override void OnEntry (WeaponEventBase e)
	{
		if (e)
		{
			eAIBase p;
			if (Class.CastTo(p, e.m_player))
			{
				m_dtAccumulator = 0;
		
				if (LogManager.IsWeaponLogEnable()) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang bang!"); }

				int mi = m_weapon.GetCurrentMuzzle();
				int b = m_weapon.GetCurrentModeBurstSize(mi);
				if(b > 1 )
				{
					
					for (int i = 0; i < b; i++)
					{
						if (m_weapon.eAI_Fire(i, p.GetBonePositionWS(p.GetBoneIndexByName("neck")), p.GetWeaponAimDirection(), vector.Forward))
						{
							p.GetAimingModel().SetRecoil(m_weapon);
							m_weapon.OnFire(i);
						}
					}
				}
				else
				{
					if (m_weapon.eAI_Fire(mi, p.GetBonePositionWS(p.GetBoneIndexByName("neck")), p.GetWeaponAimDirection(), vector.Forward))
					{
						p.GetAimingModel().SetRecoil(m_weapon);
						m_weapon.OnFire(mi);
					}
				}
				if(mi >= m_weapon.GetMuzzleCount() - 1 )
					m_weapon.SetCurrentMuzzle(0);
				else
					m_weapon.SetCurrentMuzzle(mi + 1);
				super.eAI_Vanilla_OnEntry(e);
				return;
			}
		}
		super.OnEntry(e);
	}
}

modded class WeaponFireToJam
{
	override void OnEntry (WeaponEventBase e)
	{
		eAIBase p;
		if (Class.CastTo(p, e.m_player))
		{
			if (e)
			{
				m_dtAccumulator = 0;

				if (LogManager.IsWeaponLogEnable()) { wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang! and jam?"); }
				int mi = m_weapon.GetCurrentMuzzle();
				if (m_weapon.eAI_Fire(mi, p.GetBonePositionWS(p.GetBoneIndexByName("neck")), p.GetWeaponAimDirection(), vector.Forward))
				{
					m_weapon.SetJammed(true);
					p.GetAimingModel().SetRecoil(m_weapon);
					m_weapon.OnFire(mi);
				}
			}
			m_weapon.ResetBurstCount();
			super.eAI_Vanilla_OnEntry(e);
			return;
		}
		super.OnEntry(e);
	}
}
