/**
 * Expansion_Satellite_Generator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Satellite_Generator: PowerGenerator
{
	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SetAnimationPhase("switch", 1.0);
		}
	}

	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SetAnimationPhase("switch", 0.0);
		}
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();
	#ifdef SERVER
		ExpansionNamalskModule.GetModuleInstance().SetSatelliteFacilityPower(true);
	#endif
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();
	#ifdef SERVER
		ExpansionNamalskModule.GetModuleInstance().SetSatelliteFacilityPower(false);
	#endif
	}

	bool IsSwitch(string selection = "")
	{
		return selection == "switch";
	}

	bool IsFuelTank(string selection = "")
	{
		return selection == "fuel_tank";
	}

	override bool IsInventoryVisible()
	{
		return true;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ExpansionActionRemoveSparkPlug);
	}
	
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		GetGame().ObjectDelete(this);
	}
};
