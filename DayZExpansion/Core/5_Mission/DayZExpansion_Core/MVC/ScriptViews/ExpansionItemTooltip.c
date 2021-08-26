/**
 * ExpansionItemTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionItemTooltip: ExpansionScriptView
{
	ref ExpansionItemTooltipController m_ItemTooltipController;
	private ItemPreviewWidget ItemFrameWidget;
	private ImageWidget ItemDamageWidgetBackground;
	private ImageWidget ItemWetnessWidgetBackground;
	private ImageWidget ItemTemperatureWidgetBackground;
	private ImageWidget ItemLiquidTypeWidgetBackground;
	private ImageWidget ItemFoodStageWidgetBackground;
	private ImageWidget ItemCleannessWidgetBackground;
	private TextWidget ItemWeightWidget;
	private TextWidget ItemQuantityWidget;
	private RichTextWidget ItemDescWidget;
	private GridSpacerWidget Content;
	
	EntityAI m_Item;

	void ExpansionItemTooltip(EntityAI item)
	{
		m_Item = item;
		
		if (!m_ItemTooltipController)
			m_ItemTooltipController = ExpansionItemTooltipController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/expansion_item_tooltip.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionItemTooltipController;
	}
	
	void SetView()
	{
		if (!ShowItemPreview())
			ItemFrameWidget.Show(false);
		
		if (!ShowContent())
			Content.Show(false);
		
		m_ItemTooltipController.ItemName = ExpansionStatic.GetItemDisplayNameWithType(m_Item.GetType());
		m_ItemTooltipController.NotifyPropertyChanged("ItemName");
		
		if (!m_Item.IsInherited(ZombieBase) && !m_Item.IsInherited(Car))
		{
			InventoryItem iItem = InventoryItem.Cast(m_Item);
			m_ItemTooltipController.ItemDescription = iItem.GetTooltip();
			ItemDescWidget.Update();
			
			UpdateItemInfoDamage();
			UpdateItemInfoLiquidType();
			UpdateItemInfoTemperature();
			UpdateItemInfoQuantity();
			UpdateItemInfoWetness();
			UpdateItemInfoWeight();
			UpdateItemInfoFoodStage();
			UpdateItemInfoCleanness();
					
			m_ItemTooltipController.NotifyPropertiesChanged({"ItemDescription", "ItemDamage", "ItemLiquidType", "ItemTemperature", "ItemWetness", "ItemQuantity", "ItemFoodStage", "ItemCleanness"});
		}
		else
		{
			if (ShowContent())
				Content.Show(false);
		}
	}
	
	bool ShowContent()
	{
		return true;
	}
	
	bool ShowItemPreview()
	{
		return false;
	}

	void UpdateItemInfoDamage()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(CarScript)) return;
		
		int damageLevel = m_Item.GetHealthLevel();
		string text;
		int color;
		
		switch (damageLevel)
		{
			case GameConstants.STATE_RUINED:
			{
				text = "#inv_inspect_ruined";
				color = Colors.COLOR_RUINED;
				break;
			}
			case GameConstants.STATE_BADLY_DAMAGED:
			{
				text = "#inv_inspect_badly";
				color = Colors.COLOR_BADLY_DAMAGED;
				break;
			}
			
			case GameConstants.STATE_DAMAGED:
			{
				text = "#inv_inspect_damaged";
				color = Colors.COLOR_DAMAGED;
				break;
			}
			
			case GameConstants.STATE_WORN:
			{
				text = "#inv_inspect_worn";
				color = Colors.COLOR_WORN;
				break;
			}
			
			case GameConstants.STATE_PRISTINE:
			{
				text = "#inv_inspect_pristine";
				color = Colors.COLOR_PRISTINE;
				break;
			}
					
			default:
			{
				text = "ERROR";
				color = Colors.COLOR_PRISTINE;
				break;
			}
		}
		
		m_ItemTooltipController.ItemDamage = text;
		ItemDamageWidgetBackground.SetColor(color | 0x7F000000);
	}
	
	void UpdateItemInfoLiquidType()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(Car)) return;
		
		string text;
		int color;
		ItemBase item_base = ItemBase.Cast(m_Item);
		if(item_base && item_base.GetQuantity() > 0 && item_base.IsBloodContainer())
		{
			BloodContainerBase blood_container = BloodContainerBase.Cast(item_base);
			if(blood_container.GetBloodTypeVisible())
			{
				string type;
				bool positive;
				string blood_type_name = BloodTypes.GetBloodTypeName(blood_container.GetLiquidType(), type, positive);
				text = "#inv_inspect_blood: " + blood_type_name;
				color = Colors.COLOR_LIQUID;
			}
			else
			{
				text = "#inv_inspect_blood";
				color = Colors.COLOR_LIQUID;
			}
		}
		else if(item_base && item_base.GetQuantity() > 0 && item_base.IsLiquidContainer())
		{
			int liquid_type = item_base.GetLiquidType();
			switch(liquid_type)
			{
				case LIQUID_WATER:
				{
					text = "#inv_inspect_water";
					color = Colors.COLOR_LIQUID;
					break;
				}
					
				case LIQUID_RIVERWATER:
				{
					text = "#inv_inspect_river_water";
					color = Colors.COLOR_LIQUID;
					break;
				}
					
				case LIQUID_VODKA:
				{
					text = "#inv_inspect_vodka";
					color = Colors.COLOR_LIQUID;
					break;
				}
				
				case LIQUID_BEER:
				{
					text = "#inv_inspect_beer";
					color = Colors.COLOR_LIQUID;
					break;
				}

				case LIQUID_MILK:
				{
					text = "MILK";
					color = Colors.COLOR_LIQUID;
					break;
				}
				
				case LIQUID_GASOLINE:
				{
					text = "#inv_inspect_gasoline";
					color = Colors.COLOR_LIQUID;
					break;
				}
				
				case LIQUID_DIESEL:
				{
					text = "#inv_inspect_diesel";
					color = Colors.COLOR_LIQUID;
					break;
				}
				
				case LIQUID_DISINFECTANT:
				{
					text = "#inv_inspect_disinfectant";
					color = Colors.COLOR_LIQUID;
					break;
				}
	
				case LIQUID_SALINE:
				{
					text = "#inv_inspect_saline";
					color = Colors.COLOR_LIQUID;
					break;
				}
				
				default:
				{
					text = "ERROR";
					color = Colors.COLOR_LIQUID;
					break;
				}
			}
		}
		else
		{
			ItemLiquidTypeWidgetBackground.Show(false);
		}
		
		m_ItemTooltipController.ItemLiquidType = text;
		ItemLiquidTypeWidgetBackground.SetColor(color | 0x7F000000);
	}
	
	void UpdateItemInfoTemperature()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(Car)) return;
		
		string text;
		int color;
		float temperature;
		ItemBase item_base = ItemBase.Cast(m_Item);
		if(item_base)
		{
			temperature = item_base.GetTemperature();
		}
		
		if(temperature > 30)
		{
			if (temperature > 100)
			{
				temperature = 100 * Math.Floor(temperature / 100.0);
			}
			else
			{
				temperature = 10 * Math.Floor(temperature / 10.0);
			}
			text = "#inv_inspect_about " + temperature.ToString() +  " " + "#inv_inspect_celsius";
			color = GetTemperatureColor(temperature);
		}	
		else
		{
			ItemTemperatureWidgetBackground.Show(false);
		}
		
		m_ItemTooltipController.ItemTemperature = text;
		ItemTemperatureWidgetBackground.SetColor(color | 0x7F000000);
	}

	void UpdateItemInfoWetness()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(Car)) return;
		
		string text;
		int color;
		float wetness = 0;	
		if (m_Item.IsInherited(ItemBase))
		{
			ItemBase item_IB = ItemBase.Cast(m_Item);
			wetness = item_IB.GetWet();
		}

		if(wetness < GameConstants.STATE_DAMP)
		{
			text = "";
			color = 0;
			ItemWetnessWidgetBackground.Show(false);
		}
		else if(wetness >= GameConstants.STATE_DAMP && wetness < GameConstants.STATE_WET)
		{
			text = "#inv_inspcet_damp";
			color = Colors.COLOR_DAMP;
		}
		else if(wetness >= GameConstants.STATE_WET && wetness < GameConstants.STATE_SOAKING_WET)
		{
			text = "#inv_inspect_wet";
			color = Colors.COLOR_WET;
		}
		else if(wetness >= GameConstants.STATE_SOAKING_WET && wetness < GameConstants.STATE_DRENCHED)
		{
			text = "#inv_inspect_soaking_wet";
			color = Colors.COLOR_SOAKING_WET;
		}
		else
		{
			text = "#inv_inspect_drenched";
			color = Colors.COLOR_DRENCHED;
		}
		
		m_ItemTooltipController.ItemWetness = text;
		ItemWetnessWidgetBackground.SetColor(color | 0x7F000000);
	}
	
	void UpdateItemInfoQuantity()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(Car)) return;
		
		string text;
		int color;
		ItemBase item_base = ItemBase.Cast(m_Item);
		if(item_base)
		{
			float item_quantity = item_base.GetQuantity();
			int max_quantity = m_Item.GetQuantityMax();
			
			float quantity_ratio;
			
			if( max_quantity > 0 ) // Some items, like books, have max_quantity set to 0 => division by ZERO error in quantity_ratio
			{
				string quantity_str;
				if( m_Item.ConfigGetString("stackedUnit") == "pc." )
				{
					if( item_quantity == 1 )
					{
						text =  item_quantity.ToString() + " " + "#inv_inspect_piece";
						color = Colors.COLOR_DEFAULT;	
					}
					else
					{
						text =  item_quantity.ToString() + " " + "#inv_inspect_pieces";
						color = Colors.COLOR_DEFAULT;	
					}		
				}
				else if(m_Item.ConfigGetString("stackedUnit") == "percentage")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;	
				}
				else if(m_Item.ConfigGetString("stackedUnit") == "g")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;			
				}
				else if(m_Item.ConfigGetString("stackedUnit") == "ml")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;	
				}
				else if(m_Item.IsInherited(Magazine))
				{
					Magazine magazine_item;
					Class.CastTo(magazine_item, m_Item);
					
					if(magazine_item.GetAmmoCount() == 1)
					{
						text = magazine_item.GetAmmoCount().ToString() + " " + "#inv_inspect_piece";
						color = Colors.COLOR_DEFAULT;		
					}
					else
					{
						text = magazine_item.GetAmmoCount().ToString() + " " + "#inv_inspect_pieces";
						color = Colors.COLOR_DEFAULT;		
					}
				}
				else
				{	
					text = "";
					color = 0;
					ItemQuantityWidget.Show(false);
				}
			}
			else
			{
				if (m_Item.IsInherited(ClothingBase))
				{
					float heatIsolation = MiscGameplayFunctions.GetCurrentItemHeatIsolation(item_base);
					if (heatIsolation <= GameConstants.HEATISO_THRESHOLD_BAD)
					{
						text = "#inv_inspect_iso_bad";
						color = GetTemperatureColor(10);
					}
					else if ((heatIsolation > GameConstants.HEATISO_THRESHOLD_BAD) && (heatIsolation <= GameConstants.HEATISO_THRESHOLD_LOW))
					{
						text = "#inv_inspect_iso_low";
						color = GetTemperatureColor(20);
					}
					else if ((heatIsolation > GameConstants.HEATISO_THRESHOLD_LOW) && (heatIsolation <= GameConstants.HEATISO_THRESHOLD_MEDIUM))
					{
						text = "#inv_inspect_iso_medium";
						color = GetTemperatureColor(30);
					}
					else if ((heatIsolation > GameConstants.HEATISO_THRESHOLD_MEDIUM) && (heatIsolation <= GameConstants.HEATISO_THRESHOLD_HIGH))
					{
						text = "#inv_inspect_iso_high";
						color = GetTemperatureColor(50);
					}
					else
					{
						text = "#inv_inspect_iso_excel";
						color = GetTemperatureColor(70);
					}
				}
				else
				{
					text = "";
					color = 0;
					ItemQuantityWidget.Show(false);
				}
			}
		}
		
		m_ItemTooltipController.ItemQuantity = text;
		ItemQuantityWidget.SetColor(color | 0x7F000000);
	}

	void UpdateItemInfoWeight()
	{
		if (m_Item.IsInherited(ZombieBase) || m_Item.IsInherited(Car)) return;
		
		string text;
		int color;
		ItemBase item_IB = ItemBase.Cast(m_Item);
		if(item_IB)
		{
			int weight = item_IB.GetWeight();
			
			if (GetLayoutRoot().GetName() != "ExpansionItemTooltip")
			{
				weight = item_IB.GetSingleInventoryItemWeight();
			}
			
			if (weight >= 1000)
			{
				int kilos = Math.Round(weight / 1000.0);
				text = "#inv_inspect_about" + " " + kilos.ToString() + " " + "#inv_inspect_kg";
				color = Colors.COLOR_DEFAULT;
			}
			else if (weight >= 500)
			{
				text = "#inv_inspect_under_1";
				color = Colors.COLOR_DEFAULT;
			} 
			else if (weight >= 250)
			{
				text = "#inv_inspect_under_05";
				color = Colors.COLOR_DEFAULT;
			}
			else 
			{
				text = "#inv_inspect_under_025";
				color = Colors.COLOR_DEFAULT;
			}
		}
		
		m_ItemTooltipController.ItemWeight = text;
		ItemWeightWidget.SetColor(color | 0x7F000000);
	}
	
	void UpdateItemInfoFoodStage()
	{
		string text;
		int color;
		Edible_Base food_item = Edible_Base.Cast(m_Item);
		if (food_item && food_item.HasFoodStage())
		{
			ref FoodStage food_stage = food_item.GetFoodStage();
			FoodStageType food_stage_type = food_stage.GetFoodStageType();
					
			switch(food_stage_type)
			{				
				case FoodStageType.RAW:
				{
					text = "#inv_inspect_raw";
					color = Colors.COLOR_RAW;
					break;
				}
				case FoodStageType.BAKED:
				{
					text = "#inv_inspect_baked";
					color = Colors.COLOR_BAKED;
					break;
				}
				case FoodStageType.BOILED:
				{
					text = "#inv_inspect_boiled";
					color = Colors.COLOR_BOILED;
					break;
				}
				case FoodStageType.DRIED:
				{
					text = "#inv_inspect_dried";
					color = Colors.COLOR_DRIED;
					break;
				}
				case FoodStageType.BURNED:
				{
					text = "#inv_inspect_burned";
					color = Colors.COLOR_BURNED;
					break;
				}
				case FoodStageType.ROTTEN:
				{
					text = "#inv_inspect_rotten";
					color = Colors.COLOR_ROTTEN;
					break;
				}
			}
		}
		else
		{
			ItemFoodStageWidgetBackground.Show(false);
		}
		
		m_ItemTooltipController.ItemFoodStage = text;
		ItemFoodStageWidgetBackground.SetColor(color | 0x7F000000);
	}
	
	void UpdateItemInfoCleanness()
	{
		string text;
		int color;
		ItemBase ib = ItemBase.Cast(m_Item);
		if(ib && ib.m_Cleanness==1)
		{
			text = "#inv_inspect_cleaned";
			color = Colors.WHITEGRAY;
		}
		else
		{
			ItemCleannessWidgetBackground.Show(false);
		}
		
		m_ItemTooltipController.ItemCleanness = text;
		ItemCleannessWidgetBackground.SetColor(color | 0x7F000000);
	}
	
	override void Show()
	{
		super.Show();
		SetView();
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		int x, y;
		GetGame().GetMousePos(x, y);
		GetLayoutRoot().SetPos(x + 0.0, y + 0.0, true);
	}
}

class ExpansionItemTooltipController: ExpansionViewController
{
	string ItemName;
	string ItemDamage;
	string ItemQuantity;	
	string ItemWeight;
	string ItemWetness;
	string ItemTemperature;
	string ItemLiquidType;
	string ItemFoodStage;
	string ItemCleanness;
	string ItemDescription;
	Object ItemPreview;
}