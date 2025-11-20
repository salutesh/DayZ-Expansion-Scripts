/**
 * ExpansionItemInspection.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionItemInspectionBase: ExpansionScriptView
{
	protected ref ExpansionItemInspectionController m_ItemInspectionController;
	
	protected Object m_Item;

	protected MultilineTextWidget ItemDescWidget;
	
	void ExpansionItemInspectionBase()
	{
		m_ItemInspectionController = ExpansionItemInspectionController.Cast(GetController());
	}
	
	void ~ExpansionItemInspectionBase()
	{
		m_ItemInspectionController.ItemElements.Clear();
	}

	override typename GetControllerType()
	{
		return ExpansionItemInspectionController;
	}
	
	void UpdateItemInfoCargoSize()
	{
		EntityAI entity;
		if (!Class.CastTo(entity, m_Item))
			return;

		GameInventory inventory = entity.GetInventory();
		if (!inventory)
			return;

		CargoBase cargo = inventory.GetCargo();
		if (!cargo)
			return;
		
		int width = cargo.GetWidth();
		int height = cargo.GetHeight();
		
		if (width > 0 && height > 0)
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::UpdateItemInfoCargoSize - " + m_Item.GetType() + " - storage space: " + width + "x" + height);

			string text = "#STR_EXPANSION_INV_CARGO_SIZE " + width + "x" + height;
			int color = Colors.WHITEGRAY;
			ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
	}

	protected void UpdateItemInfoCleanness()
	{
		string text;
		int color;
		ItemBase ib = ItemBase.Cast(m_Item);
		if (ib && ib.m_Cleanness == 1)
		{
			text = "#inv_inspect_cleaned";
			color = Colors.WHITEGRAY;

			ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
	}
	
	protected void UpdateItemInfoDamage()
	{
		int damageLevel = m_Item.GetHealthLevel();
		UpdateItemInfoDamage(damageLevel);
	}

	void UpdateItemInfoDamage(int damageLevel)
	{
	}
	
	void UpdateItemInfoDamageEx(int damageLevel, out string text, out int color)
	{
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
	}

	void UpdateItemInfoFoodStage(int food_stage_type)
	{
		if (food_stage_type == -1)
			return;

		string text;
		int color;
		switch (food_stage_type)
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

		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}

	protected void UpdateItemInfoFoodStage()
	{
		Edible_Base food_item = Edible_Base.Cast(m_Item);
		if (food_item && food_item.HasFoodStage())
		{
			FoodStage food_stage = food_item.GetFoodStage();
			FoodStageType food_stage_type = food_stage.GetFoodStageType();
			UpdateItemInfoFoodStage(food_stage_type);
		}
	}

	protected void UpdateItemInfoLiquidType()
	{
		string text;
		int color;
		ExpansionItemTooltipStatElement element;
		ItemBase item_base = ItemBase.Cast(m_Item);
		if (!item_base || item_base.GetQuantity() <= 0)
			return;

		if (item_base.IsBloodContainer())
		{
			BloodContainerBase blood_container = BloodContainerBase.Cast(item_base);
			if (blood_container.GetBloodTypeVisible())
			{
				UpdateItemInfoLiquidType(blood_container.GetLiquidType(), true);
			}
			else
			{
				UpdateItemInfoLiquidType(-1, true);
			}
		}
		else if (item_base && item_base.GetQuantity() > 0 && item_base.IsLiquidContainer())
		{
			int liquid_type = -1;
			liquid_type = item_base.GetLiquidType();
			UpdateItemInfoLiquidType(liquid_type);
		}
	}

	void UpdateItemInfoLiquidType(int liquid_type, bool isBloodContainer = false)
	{
		string text;
		int color;
		ExpansionItemTooltipStatElement element;
		if (isBloodContainer)
		{
			if (liquid_type > -1)
			{
				string type;
				bool positive;
				string blood_type_name = BloodTypes.GetBloodTypeName(liquid_type, type, positive);
				text = "#inv_inspect_blood: " + blood_type_name;
				color = Colors.COLOR_LIQUID;
			}
			else if (liquid_type == -1)
			{
				text = "#inv_inspect_blood";
				color = Colors.COLOR_LIQUID;
			}

			element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
		else if (liquid_type > -1)
		{
			text = ExpansionWorld.GetLiquidDisplayName(liquid_type, true, color);

			element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
	}

	void UpdateItemInfoQuantityEx(int quantityType, float quantity, int quantityMax, bool ratio = true, out string text = "", out int color = 0)
	{
		string quantity_str;
		if (quantityMax > 0) // Some items, like books, have quantityMax set to 0 => division by ZERO error in quantity_ratio
		{
			float value;
			float quantity_ratio = Math.Round((quantity / quantityMax) * 100);
			if (ratio)
				value = quantity_ratio;
			else
				value = quantity;

			if (quantityType == ExpansionItemQuantityType.PC)
			{
				if (quantity == 1)
				{
					text =  quantity.ToString() + " " + "#inv_inspect_piece";
					color = ARGB(255, 255, 255, 255);
				}
				else
				{
					text =  quantity.ToString() + " " + "#inv_inspect_pieces";
					color = ARGB(255, 255, 255, 255);
				}
			}
			else if (quantityType == ExpansionItemQuantityType.PERCENTAGE)
			{
				text = "#inv_inspect_remaining " + value.ToString() + "#inv_inspect_percent";
				color = ARGB(255, 149, 165, 166);
			}
			else if (quantityType == ExpansionItemQuantityType.GRAM)
			{
				text = "#inv_inspect_remaining " + value.ToString() + "#inv_inspect_percent";
				color = ARGB(255, 160, 223, 59);
			}
			else if (quantityType == ExpansionItemQuantityType.MILLILITER)
			{
				text = "#inv_inspect_remaining " + value.ToString() + "#inv_inspect_percent";
				color = ARGB(255, 52, 152, 219);
			}
			else if (quantityType == ExpansionItemQuantityType.MAGAZINE)
			{
				text = value.ToString() + " " + "#inv_inspect_piece";
				color = ARGB(255, 155, 89, 182);
			}
			else if (quantityType == ExpansionItemQuantityType.POWER)
			{
				text = "#inv_inspect_remaining " + value.ToString() + "#inv_inspect_percent";
				color = ARGB(255, 243, 156, 18);
			}
		}
		else
		{
			if (quantityType == ExpansionItemQuantityType.HEATISOLATION)
			{
				if (quantity <= GameConstants.HEATISO_THRESHOLD_BAD)
				{
					text = "#inv_inspect_iso_bad";
					color = GetTemperatureColor(10) | 0x7F000000;
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_BAD) && (quantity <= GameConstants.HEATISO_THRESHOLD_LOW))
				{
					text = "#inv_inspect_iso_low";
					color = GetTemperatureColor(20) | 0x7F000000;
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_LOW) && (quantity <= GameConstants.HEATISO_THRESHOLD_MEDIUM))
				{
					text = "#inv_inspect_iso_medium";
					color = GetTemperatureColor(30) | 0x7F000000;
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_MEDIUM) && (quantity <= GameConstants.HEATISO_THRESHOLD_HIGH))
				{
					text = "#inv_inspect_iso_high";
					color = GetTemperatureColor(50) | 0x7F000000;
				}
				else
				{
					text = "#inv_inspect_iso_excel";
					color = GetTemperatureColor(70) | 0x7F000000;
				}
			}
		}
	}

	void UpdateItemInfoQuantityEx(out string text, out int color)
	{
		ItemBase item_base = ItemBase.Cast(m_Item);
		if (item_base)
		{
			float item_quantity = item_base.GetQuantity();
			int max_quantity = item_base.GetQuantityMax();
			
			float quantity_ratio;
			
			if (max_quantity > 0) // Some items, like books, have max_quantity set to 0 => division by ZERO error in quantity_ratio
			{
				string quantity_str;
				string stackedUnit = m_Item.ConfigGetString("stackedUnit");
				if (stackedUnit == "pc." || stackedUnit == "pills")
				{
					if (item_quantity == 1)
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
				else if (stackedUnit == "percentage")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;	
				}
				else if (stackedUnit == "g")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;			
				}
				else if (stackedUnit == "ml")
				{
					quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
					quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
					text = quantity_str;
					color = Colors.COLOR_DEFAULT;	
				}
				else if (m_Item.IsInherited(Magazine))
				{
					Magazine magazine_item;
					Class.CastTo(magazine_item, m_Item);
					
					if (magazine_item.GetAmmoCount() == 1)
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
			}
		}
	}

	protected void UpdateItemInfoTemperature()
	{
		string text;
		int color;
		float temperature;
		ItemBase item_base = ItemBase.Cast(m_Item);
		if (!item_base)
			return;

		temperature = item_base.GetTemperature();

		if (temperature > 30)
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

			ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
	}

	protected void UpdateItemInfoWeightEx(out string text, out int color)
	{
		int weight;
		ItemBase item_IB = ItemBase.Cast(m_Item);
		if (item_IB)
		{
			weight = item_IB.GetWeight();
		}
		else
		{
			weight = dBodyGetMass(m_Item) * 1000;
		}

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

	protected void UpdateItemInfoWetness()
	{
		string text;
		int color;
		float wetness = 0;
		ItemBase item_IB = ItemBase.Cast(m_Item);
		if (!item_IB)
			return;

		wetness = item_IB.GetWet();

		if (wetness < GameConstants.STATE_DAMP)
		{
			return;
		}
		else if (wetness >= GameConstants.STATE_DAMP && wetness < GameConstants.STATE_WET)
		{
			text = "#inv_inspcet_damp";
			color = Colors.COLOR_DAMP;
		}
		else if (wetness >= GameConstants.STATE_WET && wetness < GameConstants.STATE_SOAKING_WET)
		{
			text = "#inv_inspect_wet";
			color = Colors.COLOR_WET;
		}
		else if (wetness >= GameConstants.STATE_SOAKING_WET && wetness < GameConstants.STATE_DRENCHED)
		{
			text = "#inv_inspect_soaking_wet";
			color = Colors.COLOR_SOAKING_WET;
		}
		else
		{
			text = "#inv_inspect_drenched";
			color = Colors.COLOR_DRENCHED;
		}

		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}
	
	void UpdateItemStats()
	{
		UpdateItemInfoWetness();
		UpdateItemInfoTemperature();
		UpdateItemInfoLiquidType();
		UpdateItemInfoFoodStage();
		UpdateItemInfoCleanness();
		UpdateItemInfoCargoSize();
	}
}

class ExpansionItemInspection: ExpansionItemInspectionBase
{
	ItemPreviewWidget ItemPreview;
	protected ButtonWidget BackButton;
	protected ImageWidget IconClose;
	
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;
	protected vector m_CharacterOrientation;
	
	protected ScriptView m_ParentView;
	protected ref ScriptInvoker m_OnInspectCloseSI;
	
	void ExpansionItemInspection()
	{
		m_OnInspectCloseSI = new ScriptInvoker();
	}
	
	void ~ExpansionItemInspection()
	{
		if (m_OnInspectCloseSI)
		{
			m_OnInspectCloseSI.Clear();
			m_OnInspectCloseSI = null;
		}
	}
		
	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/expansion_item_inspection.layout";
	}
	
	void SetItem(Object item)
	{
		m_Item = item;
		
		if (!m_ItemInspectionController)
			m_ItemInspectionController = ExpansionItemInspectionController.Cast(GetController());

		m_ItemInspectionController.ItemName = m_Item.GetDisplayName();
		
		string itemDesc = ExpansionStatic.GetItemDescriptionWithType(m_Item.GetType());
		if (itemDesc.IndexOf("STR_") == 0)
			itemDesc = "";

		m_ItemInspectionController.ItemDescription = itemDesc;
		ItemDescWidget.Update();
		m_ItemInspectionController.NotifyPropertiesChanged({"ItemDescription", "ItemName"});
		
		if (!ItemPreview)
		{
			Widget preview_frame = GetLayoutRoot().FindAnyWidget("ItemPreview");
			if (preview_frame)
			{
				ItemPreview = ItemPreviewWidget.Cast(preview_frame);
			}
		}
		
		ItemPreview.SetItem(EntityAI.Cast(item));
		ItemBase itemIB = ItemBase.Cast(item);
		if (itemIB)
			ItemPreview.SetView(itemIB.GetViewIndex());

		ItemPreview.SetModelPosition(Vector(0,0,1));
	}
	
	void SetParentView(ScriptView parentView)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this, "Parent view: " + parentView);
		#endif
		
		m_ParentView = parentView;
	}
	
	override void UpdateItemInfoDamage(int damageLevel)
	{
		string text;
		int color;
		UpdateItemInfoDamageEx(damageLevel, text, color);

		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}

	void UpdateItemInfoQuantity(int quantityType, float quantity, int quantityMax, bool ratio = true)
	{
		string text;
		int color;
		UpdateItemInfoQuantityEx(quantityType, quantity, quantityMax, ratio, text, color);
		UpdateItemInfoQuantity(text, color);
	}

	void UpdateItemInfoQuantity()
	{
		string text;
		int color;
		UpdateItemInfoQuantityEx(text, color);
		UpdateItemInfoQuantity(text, color);
	}

	void UpdateItemInfoQuantity(string text, int color)
	{
		if (text != string.Empty)
		{
			ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
			m_ItemInspectionController.ItemElements.Insert(element);
		}
	}

	protected void UpdateItemInfoWeight()
	{
		string text;
		int color;
		UpdateItemInfoWeightEx(text, color);
		
		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}

	override void OnShow()
	{
		super.OnShow();
		
		SetFocus(GetLayoutRoot());
	}
	
	void OnBackButtonClick()
	{
		if (m_ParentView)
			m_ParentView.GetLayoutRoot().Show(true);

		if (m_OnInspectCloseSI)
			m_OnInspectCloseSI.Invoke(false);
		
		MissionGameplay.Expansion_DestroyItemInspection();		
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && w == BackButton)
		{
			BackButton.SetColor(ARGB(255, 255, 255, 255));
			IconClose.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL && w == BackButton)
		{
			BackButton.SetColor(ARGB(255, 0, 0, 0));
			IconClose.SetColor(ARGB(255, 255, 255, 255));
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		if (w == ItemPreview)
		{
			g_Game.GetDragQueue().Call(this, "UpdateRotation");
			GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}
		return false;
	}
	
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[0] = o[0] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);
		
		ItemPreview.SetModelOrientation(o);
		
		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}
	
	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		if (w == ItemPreview)
		{
			m_CharacterScaleDelta = wheel;
			UpdateScale();
		}

		return false;
	}

	void UpdateScale()
	{
		float w, h, x, y;		
		ItemPreview.GetPos(x, y);
		ItemPreview.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 4);
		h = h + (m_CharacterScaleDelta / 4);
		if (w > 0.5 && w < 3)
		{
			ItemPreview.SetSize(w, h);
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 8);
			float new_y = y - (m_CharacterScaleDelta / 8);
			ItemPreview.SetPos(new_x, new_y);
		}
	}

	ScriptInvoker GetCloseInspectionSI()
	{
		return m_OnInspectCloseSI;
	}
};

class ExpansionItemInspectionController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionItemTooltipStatElement> ItemElements = new ObservableCollection<ref ExpansionItemTooltipStatElement>(this);

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
};
#endif
