/**
 * Expansion_AnomalyCore_Fire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyCoreLightFire extends ExpansionAnomalyCoreLightBase
{
	void ExpansionAnomalyCoreLightFire()
	{
		SetDiffuseColor(0.751, 0.196, 0.890);
		SetAmbientColor(0.751, 0.196, 0.890);
	}
};

class Expansion_AnomalyCore_Fire: Expansion_AnomalyCore_Base
{
	void Expansion_AnomalyCore_Fire()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetEventMask(EntityEvent.CONTACT | EntityEvent.TOUCH);
		SetFlags(EntityFlags.TRIGGER, false);
	}

	override protected void OnExplode()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsServer())
		{
			GetGame().CreateObject("ExpansionAnomalyAreaFire_Local", GetPosition());
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.ACTIVATED);
		}
	}
	
	override typename GetAnomalyCoreLight()
	{
		return ExpansionAnomalyCoreLightFire;
	}
};