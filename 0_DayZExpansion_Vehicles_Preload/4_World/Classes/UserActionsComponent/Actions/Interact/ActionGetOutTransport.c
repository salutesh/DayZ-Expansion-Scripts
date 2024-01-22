/**
 * ActionGetOutTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionGetOutTransport
{
	//! Equivalent to vanilla ActionGetOutTransport::OnEndServer but will apply jump out dmg only for land vehicles (cars), not helis, planes or boats
	override void OnEndServer(ActionData action_data)
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

		CarScript car = CarScript.Cast(got_action_data.m_Car);

		if (got_action_data.m_WasJumpingOut)
		{
			float carSpeed = got_action_data.m_CarSpeed;
			got_action_data.m_Player.OnJumpOutVehicleFinish(carSpeed);

			//! Jump out damage should only apply for land vehicles (cars), not helis, planes or boats, and only if player is not in godmode
			if (got_action_data.m_Player.Expansion_CanBeDamaged("FallDamage") && car && car.IsCar())
			{
				PlayerBase player = got_action_data.m_Player;

				ApplyJumpOutDmg(action_data);

				vector posMS = player.WorldToModel(player.GetPosition());
				player.DamageAllLegs(got_action_data.m_DmgTaken); //! Additional leg specific damage dealing
				//float healthCoef = Math.InverseLerp(HEALTH_LOW_SPEED_VALUE, HEALTH_HIGH_SPEED_VALUE, carSpeed);  //! private const >:(
				float healthCoef = Math.InverseLerp(30, 70, carSpeed);
				healthCoef = Math.Clamp(healthCoef, 0.0, 1.0);
				player.ProcessDirectDamage(DT_CUSTOM, player, "", DayZPlayerImplementFallDamage.FALL_DAMAGE_AMMO_HEALTH, posMS, healthCoef);
			}
		}

		if (car)
			car.ForceUpdateLightsEnd();
	}
};
