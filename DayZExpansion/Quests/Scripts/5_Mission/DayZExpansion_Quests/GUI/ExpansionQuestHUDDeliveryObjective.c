/**
 * ExpansionQuestHUDDeliveryEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestHUDDeliveryEntry: ExpansionScriptView
{
	protected ref ExpansionQuestHUDDeliveryEntryController m_QuestHUDDeliveryObjectiveController;
	protected ExpansionQuestObjectiveDelivery m_Delivery;
	protected int m_Count;
	protected ExpansionQuestHUDObjective m_ObjectiveEntry;
	protected ImageWidget delivery_icon;
	protected TextWidget delivery_name;
	protected TextWidget delivery_value;
	protected Widget spacer;

	void ExpansionQuestHUDDeliveryEntry(ExpansionQuestObjectiveDelivery delivery, int count, ExpansionQuestHUDObjective objectiveEntry)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		m_Delivery = delivery;
		m_Count = count;
		m_ObjectiveEntry = objectiveEntry;
		Class.CastTo(m_QuestHUDDeliveryObjectiveController, GetController());
		
		SetView();
	}

	void SetView()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		if (m_Delivery)
		{
			delivery_icon.LoadImageFile(0, "{EB294B6B8215EC25}DayZExpansion/Core/GUI/icons/hud/arrow_64x64.edds");
			delivery_icon.LoadImageFile(1, "{B0A741F8CA2EA4D5}DayZExpansion/Core/GUI/icons/hud/hook_64x64.edds");
	
			m_QuestHUDDeliveryObjectiveController.DeliveryName = ExpansionStatic.GetItemDisplayNameWithType(m_Delivery.GetClassName());
			m_QuestHUDDeliveryObjectiveController.NotifyPropertyChanged("DeliveryName");
			
			int needed = m_Delivery.GetAmount();
			m_QuestHUDDeliveryObjectiveController.DeliveryValue = m_Count.ToString() + "/" + needed.ToString();
			m_QuestHUDDeliveryObjectiveController.NotifyPropertyChanged("DeliveryValue");
			
			if (m_Count >= needed)
			{
				delivery_name.SetColor(ARGB(255, 160, 223, 59));
				delivery_value.SetColor(ARGB(255, 160, 223, 59));
				delivery_icon.SetImage(1);
				delivery_icon.SetRotation(0, 0, 0, true);
				delivery_icon.SetColor(ARGB(255, 160, 223, 59));				
				spacer.SetColor(ARGB(200, 160, 223, 59));
			}
			else
			{
				delivery_name.SetColor(ARGB(255, 255, 255, 255));
				delivery_value.SetColor(ARGB(255, 255, 255, 255));
				delivery_icon.SetImage(0);
				delivery_icon.SetRotation(0, 0, 90, true);
				delivery_icon.SetColor(ARGB(255, 255, 255, 255));
				
				int color = ExpansionQuestModule.GetQuestColor(m_ObjectiveEntry.GetQuestConfig());
				spacer.SetColor(color);
			}
		}
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud_delivery_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDDeliveryEntryController;
	}
};

class ExpansionQuestHUDDeliveryEntryController: ExpansionViewController
{
	string DeliveryName;
	string DeliveryValue;
};
