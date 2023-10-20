//! Called when dragging ammo pile onto mag in hand
modded class ActionLoadMagazine
{
	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnExecuteServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged();
	}
}

//! Called when holding the "reload" button (with mag in hand) to load a bullet from inventory
modded class ActionLoadMagazineQuick
{
	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		Magazine trg = action_data.m_Player.GetWeaponManager().GetPreparedMagazine();

		super.OnExecuteServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, trg))
			ammo.Expansion_OnStackSizeChanged();
	}
}

modded class ActionEmptyMagazine
{
	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnExecuteServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}

//! Called when dragging ammo pile onto gun in hand
modded class FirearmActionLoadBullet
{
	override void OnEndServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnEndServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged();
	}
}

//! Called when holding the "reload" button (with gun in hand) to chamber a bullet from inventory
modded class FirearmActionLoadBulletQuick
{
	override void OnEndServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnEndServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged();
	}
}

modded class FirearmActionLoadMultiBullet
{
	override void OnEndServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnEndServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged();
	}
}

modded class FirearmActionLoadMultiBulletRadial
{
	override void OnEndServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnEndServer(action_data);

		ItemBase ammo;
		if (Class.CastTo(ammo, action_data.m_Target.GetObject()))
			ammo.Expansion_OnStackSizeChanged();
	}
}

//! Eject (unload) bullet from chamber
modded class FirearmActionMechanicManipulate
{
	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		action_data.m_Player.Expansion_RememberAmmoInInventoryQuantity();

		super.OnStartServer(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_CheckAmmoInInventoryQuantityChanged();
	}
}
