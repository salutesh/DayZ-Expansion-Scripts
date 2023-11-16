/**
 * ActionDrink.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*modded class ActionDrink
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		Param1<float> nacdata = Param1<float>.Cast(action_data.m_ActionComponent.GetACData());
		if (nacdata)
		{
			float amount = UAQuantityConsumed.DRINK;
			Print(ToString() + "::OnFinishProgressServer - Drink amount: " + amount);
			action_data.m_Player.GetStatRadiation().Add(-amount);
		}
	}

	override void OnEndServer(ActionData action_data)
	{	
		ItemBase item = action_data.m_MainItem;
		
		if (item && item.GetQuantity() <= 0.01)
		{
			item.SetQuantity(0);
		}
		else if (item && GetProgress(action_data) > 0)
		{
			// we don't want to inject an agent into an empty container
			PlayerBase player = action_data.m_Player;
			PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
			plugin.TransmitAgents(player, item, AGT_UACTION_TO_ITEM);
		}
	}
};*/
