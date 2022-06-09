/**
 * ExpansionQuestHUDDeliveryObjective.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
class ExpansionQuestHUDDeliveryObjective: ExpansionScriptView
{
	protected ref ExpansionQuestHUDDeliveryObjectiveController m_QuestHUDDeliveryObjectiveController;
	protected ExpansionQuestObjectiveDelivery m_Delivery;

	void ExpansionQuestHUDDeliveryObjective(ExpansionQuestObjectiveDelivery delivery)
	{
		m_Delivery = delivery;
		Class.CastTo(m_QuestHUDDeliveryObjectiveController, GetController());

		SetView();
	}

	void SetView()
	{
		if (!m_Delivery)
			return;

		m_QuestHUDDeliveryObjectiveController.DeliveryName = ExpansionStatic.GetItemDisplayNameWithType(m_Delivery.GetClassName());
		m_QuestHUDDeliveryObjectiveController.NotifyPropertyChanged("DeliveryName");

		m_QuestHUDDeliveryObjectiveController.DeliveryValue = "x" + m_Delivery.GetAmount().ToString();
		m_QuestHUDDeliveryObjectiveController.NotifyPropertyChanged("DeliveryValue");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud_delivery_objective.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDDeliveryObjectiveController;
	}
};

class ExpansionQuestHUDDeliveryObjectiveController: ExpansionViewController
{
	string DeliveryName;
	string DeliveryValue;
};
#endif