class Expansion_Halloween_GiftBox_Small extends ItemBase
{
	protected ref array<string> m_Treats = new array<string>;
	
	void Expansion_Halloween_GiftBox_Small()
	{
		m_Treats.Insert("dzn_module_card");
		m_Treats.Insert("dzn_module_lantia");
		m_Treats.Insert("dzn_printer_filament_tpc");
		m_Treats.Insert("dzn_printer_filament_abs");
		m_Treats.Insert("dzn_printer_filament_nylon");
		m_Treats.Insert("Expansion_SupplyCrate_WorldT1_Key");
		m_Treats.Insert("Expansion_KeyCard_A1_B1");
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ExpansionActionUnpackHalloweenGift);
	}
	
	override void Open()
	{
		SpawnTreat();
	}
	
	protected void SpawnTreat()
	{
		string treat = m_Treats.GetRandomElement();
		GetGame().CreateObjectEx(treat, GetPosition(), ECE_PLACE_ON_SURFACE, RF_DEFAULT);
	}
	
	protected void SpawnTrick()
	{
		GetGame().CreateObjectEx("ZmbM_Mummy", GetPosition(), ECE_PLACE_ON_SURFACE, RF_DEFAULT);
	}
};