/**
 * Expansion_Satellite_Control.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note: Can only be used when the BuildingsModPack mod by Starlv is loaded: https://steamcommunity.com/sharedfiles/filedetails/?id=2270098553
#ifdef EXPANSION_NAMALSK_ADVENTURE
class Expansion_Satellite_Control: ItemBase
{
	#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	protected SV_Abandoned_Sattelite_Antenna m_LinkedSatellite;
	#ifdef EXPANSIONMODTELEPORTER
	protected Expansion_Teleporter_Big m_LinkedTeleporter;
	#endif
	protected Expansion_Satellite_Panel_Lever m_LinkedPanelLevel;
	protected bool m_CanActivate;
	protected bool m_IsSatelliteActive;
	protected bool m_IsSatelliteBooting;
	protected ref Timer m_ActiveBootTimer;
	protected ref Timer m_ActiveTimer;
	protected EffectSound m_RunSFX;

	void Expansion_Satellite_Control()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		RegisterNetSyncVariableBool("m_CanActivate");
		RegisterNetSyncVariableBool("m_IsSatelliteActive");
		RegisterNetSyncVariableBool("m_IsSatelliteBooting");

		m_Expansion_RPCManager = new ExpansionRPCManager(this, ItemBase);
		m_Expansion_RPCManager.RegisterClient("RPC_Boot");
		m_Expansion_RPCManager.RegisterClient("RPC_Active");
		m_Expansion_RPCManager.RegisterClient("RPC_Shutdown");
	}

	void ~Expansion_Satellite_Control()
	{
		if (!GetGame())
			return;

		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		#ifdef DIAG_DEVELOPER
		#ifdef EXPANSIONMODNAVIGATION
		if (!m_ServerMarker)
			return;

		ExpansionMarkerModule markerModule;
		if (CF_Modules<ExpansionMarkerModule>.Get(markerModule))
			markerModule.RemoveServerMarker(m_ServerMarker.GetUID());
		#endif
		#endif

		#ifndef SERVER
		if (m_RunSFX)
			m_RunSFX.SoundStop();
		#endif
	}

	override void EEInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.EEInit();

		InitSatelliteControl();
	}

	protected void InitSatelliteControl()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		#ifdef SERVER
		InitSatelliteControlServer();
		#endif
	}

	protected void InitSatelliteControlServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		#ifdef DIAG_DEVELOPER
		#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
		#endif
		#endif
	}

	void SetLinkedSatellite(SV_Abandoned_Sattelite_Antenna satellite)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_LinkedSatellite = satellite;
	}

	#ifdef EXPANSIONMODTELEPORTER
	void SetLinkedTeleporter(Expansion_Teleporter_Big teleporter)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_LinkedTeleporter = teleporter;
	}
	#endif
	
	void SetLinkedPanelLever(Expansion_Satellite_Panel_Lever panelLever)
	{
		m_LinkedPanelLevel = panelLever;
	}

	void SetActivateState(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_CanActivate = state;

		SetSynchDirty();
	}

	void SetSatelliteState(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_IsSatelliteActive = state;

		if (m_IsSatelliteBooting && state)
			m_IsSatelliteBooting = false;

		SetSynchDirty();
	}

	void SetSatelliteBooting(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_IsSatelliteBooting = state;

		SetSynchDirty();
	}

	bool CanActivate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		EXTrace.Add(trace, m_IsSatelliteActive);
		EXTrace.Add(trace, m_IsSatelliteBooting);
		EXTrace.Add(trace, m_CanActivate);
		EXTrace.Add(trace, HasKeyCard());
		EXTrace.Add(trace, HasEnergy());
		EXTrace.Add(trace, SatellitePanelState());

		if (m_IsSatelliteActive || m_IsSatelliteBooting || !m_CanActivate || !HasKeyCard() || !HasEnergy() || !SatellitePanelState())
			return false;

		return true;
	}

	bool IsActive()
	{
		return m_IsSatelliteActive;
	}

	void StartSatellite()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_Boot");
		rpc.Write(m_LinkedSatellite);
		PlayerBase.Expansion_SendNear(rpc, GetPosition(), 200.0, this, true);

		SetSatelliteBooting(true);

		m_ActiveBootTimer = new Timer(CALL_CATEGORY_SYSTEM);
		m_ActiveBootTimer.Run(5, this, "BootSatellite");
	}

	void BootSatellite()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_Active");
		rpc.Write(m_LinkedSatellite);
		PlayerBase.Expansion_SendNear(rpc, GetPosition(), 200.0, this, true);

		#ifdef EXPANSIONMODTELEPORTER
		if (m_LinkedTeleporter)
			m_LinkedTeleporter.SetActive(true);
		#endif

		SetSatelliteState(true);

		m_ActiveTimer = new Timer(CALL_CATEGORY_SYSTEM);
		m_ActiveTimer.Run(120, this, "StopSatellite");
	}

	void StopSatellite()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_Shutdown");
		rpc.Write(m_LinkedSatellite);
		PlayerBase.Expansion_SendNear(rpc, GetPosition(), 200.0, this, true);

		#ifdef EXPANSIONMODTELEPORTER
		if (m_LinkedTeleporter)
			m_LinkedTeleporter.SetActive(false);
		#endif

		SetSatelliteState(false);
		m_LinkedSatellite.SetSatelliteActive(false);
		ExpansionNamalskModule.GetModuleInstance().OverloadSatelliteGenerator();
	}

	override void OnVariablesSynchronized()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::OnVariablesSynchronized - Satellite active: " + m_IsSatelliteActive.ToString());

		super.OnVariablesSynchronized();

	#ifndef SERVER
		if (!m_IsSatelliteActive && m_RunSFX)
			m_RunSFX.SoundStop();
	#endif
	}

	protected void PlaySFXBoot(EntityAI satellite)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!GetGame().IsDedicatedServer())
		{
			EffectSound soundEffect = SEffectManager.PlaySound("Expansion_Satellite_Boot_Soundset", satellite.GetPosition());
			if (!soundEffect)
				return;

			soundEffect.SetParent(satellite);
			soundEffect.SetSoundAutodestroy(true);
		}
	}

	protected void PlaySFXActive(EntityAI satellite)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!GetGame().IsDedicatedServer() && !m_RunSFX)
		{
			m_RunSFX = SEffectManager.PlaySound("Expansion_Satellite_Active_Soundset", satellite.GetPosition());
			if (!m_RunSFX)
				return;

			m_RunSFX.SetParent(satellite);
			m_RunSFX.SetSoundAutodestroy(true);
		}
	}

	void RPC_Boot(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		EntityAI satellite;
		if (ctx.Read(satellite))
			PlaySFXBoot(satellite);
	}

	void RPC_Active(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		EntityAI satellite;
		if (ctx.Read(satellite))
			PlaySFXActive(satellite);
	}

	void RPC_Shutdown(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (m_RunSFX)
			m_RunSFX.SoundStop();

		EntityAI satellite;
		if (ctx.Read(satellite))
			PlaySFXActive(satellite);
	}

	bool HasEnergy()
	{
		ExpansionNamalskModule namalskModule;
		if (CF_Modules<ExpansionNamalskModule>.Get(namalskModule))
			return namalskModule.HasSatelliteFacilityPower();

		return false;
	}
	
	bool SatellitePanelState()
	{
		if (m_LinkedPanelLevel)
			return m_LinkedPanelLevel.GetPanelState();

		return false;
	}

	bool HasKeyCard()
	{
		ItemBase keyCard = ItemBase.Cast(FindAttachmentBySlotName("Att_ExpansionKeyCardAntenna"));
		if (keyCard && !keyCard.IsDamageDestroyed())
			return true;

		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ExpansionActionUseSatelliteControl);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		ExpansionMarkerModule markerModule;
		if (CF_Modules<ExpansionMarkerModule>.Get(markerModule))
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Options", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 44, 62, 80), true);
	}
	#endif
	#endif

	protected void ExDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};
#endif
