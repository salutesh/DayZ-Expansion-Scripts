/**
 * ExpansionAnomalyTriggerSingularity_Dynamic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTriggerSingularity_Dynamic : ExpansionAnomalyTriggerBase_Dynamic
{
	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnStayServerEvent - Insider: " + insider.GetObject().ToString());

		super.OnStayServerEvent(insider, deltaTime);

		if (m_DealDamageFlag)
		{
			PlayerBase player = PlayerBase.Cast(insider.GetObject());
			if (player)
			{
				player.GetStatHeatComfort().Add(-1000.0); //! Let the character feel the cold.
				player.GetStatRadiation().Add(10.0); //! Let the character get some radiation.
				player.ForceUpdateRadiation();
			}
		}
	}
};