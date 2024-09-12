//! Called when dragging ammo pile onto mag in hand
modded class ActionLoadMagazine
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

//! Called when holding the "reload" button (with mag in hand) to load bullets from inventory
modded class ActionLoadMagazineQuick
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

modded class ActionEmptyMagazine
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

//! Called when dragging ammo pile onto gun w/o internal mag in hand
modded class FirearmActionLoadBullet
{
	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		Ammunition_Base ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged(-1);
	}
}

//! Called when holding the "reload" button (with gun in hand) to chamber a bullet from inventory
modded class FirearmActionLoadBulletQuick
{
	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		Ammunition_Base ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged(-1);
	}
}

//! Called when dragging ammo pile onto gun w/ internal mag in hand
modded class FirearmActionLoadMultiBullet
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

//! Called when holding the "reload" button (with gun in hand) to chamber a bullet and fill internal mag from inventory
modded class FirearmActionLoadMultiBulletQuick
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

modded class FirearmActionLoadMultiBulletRadial
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

//! Eject (unload) bullet from chamber
modded class FirearmActionMechanicManipulate
{
	override void OnStartServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}
