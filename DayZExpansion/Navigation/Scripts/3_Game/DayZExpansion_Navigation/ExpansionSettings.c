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

		Init(ExpansionMapSettings, true);
	}

	override void OnInit(typename type)
	{
		super.OnInit(type);

		if (type == ExpansionMapSettings)
		{
			m_Expansion_RPCManager.RegisterClient("RPC_AddServerMarker");
			m_Expansion_RPCManager.RegisterClient("RPC_RemoveServerMarker");
		}
	}

	void RPC_MapSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionMapSettings, ctx);
	}

	void RPC_AddServerMarker(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		string uid_add;
		if (!Expansion_Assert_False(ctx.Read(uid_add), "ExpansionSettingsRPC.AddServerMarker - Failed reading marker UID"))
		{
			auto marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.SERVER, uid_add);
			marker.OnRecieve(ctx);
			if (GetExpansionSettings().GetMap(false).IsLoaded()) //! No need to add marker if map settings not yet received
				GetExpansionSettings().GetMap().AddServerMarker(marker);
		}
	}

	void RPC_RemoveServerMarker(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		string uid_remove;
		if (!Expansion_Assert_False(ctx.Read(uid_remove), "ExpansionSettingsRPC.RemoveServerMarker - Failed reading marker UID"))
		{
			if (GetExpansionSettings().GetMap(false).IsLoaded()) //! No need to remove marker if map settings not yet received
				GetExpansionSettings().GetMap().RemoveServerMarker(uid_remove);
		}
	}

	ExpansionMapSettings GetMap(bool checkLoaded = true)
	{
		return ExpansionMapSettings.Cast(Get(ExpansionMapSettings, checkLoaded));
	}
};
