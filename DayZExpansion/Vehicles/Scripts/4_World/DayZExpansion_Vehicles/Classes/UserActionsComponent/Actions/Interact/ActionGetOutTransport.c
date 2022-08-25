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
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleLeave)
		{
			GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

			GetExpansionSettings().GetLog().PrintLog("[VehicleLeave] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] left vehicle " + got_action_data.m_Car.GetDisplayName() + " (pos=" + got_action_data.m_Car.GetPosition() + ", type=" + got_action_data.m_Car.GetType() + ")");
		}
	}
};