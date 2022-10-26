/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Map = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionMapSettings);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if (rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT)
			return false;

		if (GetGame().IsDedicatedServer())
			return false;

		switch (rpc_type)
		{
		case ExpansionSettingsRPC.Map:
			Receive(ExpansionMapSettings, ctx);
			return true;

		case ExpansionSettingsRPC.AddServerMarker:
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return false;
			
			string uid_add;
			if (!Expansion_Assert_False(ctx.Read(uid_add), "ExpansionSettingsRPC.AddServerMarker - Failed reading marker UID"))
			{
				auto marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.SERVER, uid_add);
				marker.OnRecieve(ctx);
				if (GetExpansionSettings().GetMap(false).IsLoaded()) //! No need to add marker if map settings not yet received
					GetExpansionSettings().GetMap().AddServerMarker(marker);
			}
			return true;

		case ExpansionSettingsRPC.RemoveServerMarker:
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return false;
			
			string uid_remove;
			if (!Expansion_Assert_False(ctx.Read(uid_remove), "ExpansionSettingsRPC.RemoveServerMarker - Failed reading marker UID"))
			{
				if (GetExpansionSettings().GetMap(false).IsLoaded()) //! No need to remove marker if map settings not yet received
					GetExpansionSettings().GetMap().RemoveServerMarker(uid_remove);
			}
			return true;
		}

		return false;
	}

	ExpansionMapSettings GetMap(bool checkLoaded = true)
	{
		return ExpansionMapSettings.Cast(Get(ExpansionMapSettings, checkLoaded));
	}
};
