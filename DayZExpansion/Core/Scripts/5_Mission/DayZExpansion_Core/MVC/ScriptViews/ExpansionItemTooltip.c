/**
 * ExpansionItemTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionItemTooltip: ExpansionItemInspectionBase
{
	protected ItemPreviewWidget ItemFrameWidget;
	protected ImageWidget ItemDamageWidgetBackground;
	protected TextWidget ItemWeightWidget;
	protected TextWidget ItemQuantityWidget;
	protected GridSpacerWidget Content;

	protected bool m_ShowPreview = false;
	protected bool m_ShowContent = true;

	protected float m_ContentOffsetX;
	protected float m_ContentOffsetY;
	
	protected bool m_UpdateStatsOnShow;

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/expansion_item_tooltip.layout";
	}

	void SetItem(EntityAI item)
	{
		m_Item = item;
	}
	
	protected void SetView()
	{
		if (!ShowItemPreview())
		{
			ItemFrameWidget.Show(false);
		}
		else
		{
			m_ItemInspectionController.ItemPreview = m_Item;
			m_ItemInspectionController.NotifyPropertyChanged("ItemPreview");
		}

		if (!ShowContent())
			Content.Show(false);

		m_ItemInspectionController.ItemName = m_Item.GetDisplayName();
		
		string itemDesc = ExpansionStatic.GetItemDescriptionWithType(m_Item.GetType());
		if (itemDesc.IndexOf("STR_") == 0)
			itemDesc = "";

		m_ItemInspectionController.ItemDescription = itemDesc;
		ItemDescWidget.Update();
		m_ItemInspectionController.NotifyPropertiesChanged({"ItemDescription", "ItemName"});

		if (m_UpdateStatsOnShow)
		{
			UpdateItemStats();
			UpdateItemInfoDamage();
			UpdateItemInfoQuantity();
			UpdateItemInfoWeight();
		}
	}

	bool ShowContent()
	{
		return m_ShowContent;
	}

	void SetShowContent(bool state)
	{
		m_ShowContent = state;
	}

	bool ShowItemPreview()
	{
		return m_ShowPreview;
	}

	void SetShowItemPreview(bool state)
	{
		m_ShowPreview = state;
	}

	override void UpdateItemInfoDamage(int damageLevel)
	{
		string text;
		int color;
		UpdateItemInfoDamageEx(damageLevel, text, color);

		m_ItemInspectionController.ItemDamage = text;
		m_ItemInspectionController.NotifyPropertyChanged("ItemDamage");
		ItemDamageWidgetBackground.SetColor(color | 0x7F000000);
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
		if (text != "")
		{
			m_ItemInspectionController.ItemQuantity = text;
			m_ItemInspectionController.NotifyPropertyChanged("ItemQuantity");
			ItemQuantityWidget.SetColor(color | 0x7F000000);
		}
		else
		{
			ItemQuantityWidget.Show(false);
		}
	}

	protected void UpdateItemInfoWeight()
	{
		string text;
		int color;
		UpdateItemInfoWeightEx(text, color);

		m_ItemInspectionController.ItemWeight = text;
		m_ItemInspectionController.NotifyPropertyChanged("ItemWeight");
		ItemWeightWidget.SetColor(color | 0x7F000000);
	}

	override void Show()
	{
		super.Show();
		SetView();
	}

	void SetContentOffset(float x, float y)
	{
		m_ContentOffsetX = x;
		m_ContentOffsetY = y;
	}

	override void OnShow()
	{
		super.OnShow();

		int x, y;
		GetMousePos(x, y);
		GetLayoutRoot().SetPos(x + m_ContentOffsetX, y + m_ContentOffsetY, true);
	}

	void SetUpdateStatsOnShow(bool state)
	{
		m_UpdateStatsOnShow = state;
	}
};

class ExpansionItemTooltipStatElement: ExpansionScriptView
{
	ref ExpansionItemTooltipStatElementController m_TooltipStatElementController;
	string m_Text;
	int m_Color;

	protected ImageWidget Background;

	void ExpansionItemTooltipStatElement(string text, int color)
	{
		m_Text = text;
		m_Color = color;

		if (!m_TooltipStatElementController)
			m_TooltipStatElementController = ExpansionItemTooltipStatElementController.Cast(GetController());

		SetView();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/expansion_item_tooltip_stat_element.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionItemTooltipStatElementController;
	}

	void SetView()
	{
		m_TooltipStatElementController.StatText = m_Text;
		m_TooltipStatElementController.NotifyPropertyChanged("StatText");
		Background.SetColor(m_Color | 0x7F000000);
	}
};

class ExpansionItemTooltipStatElementController: ExpansionViewController
{
	string StatText;
};

class ExpansionItemPreviewTooltip: ExpansionScriptView
{
	ref ExpansionItemPreviewTooltipController m_ItemInspectionController;
	Object m_Item;

	private float m_ContentOffsetX;
	private float m_ContentOffsetY;

	void ExpansionItemPreviewTooltip(Object item)
	{
		m_Item = item;

		if (!m_ItemInspectionController)
			m_ItemInspectionController = ExpansionItemPreviewTooltipController.Cast(GetController());
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/expansion_item_preview_tooltip.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionItemPreviewTooltipController;
	}

	void SetView()
	{
		m_ItemInspectionController.ItemPreview = m_Item;
		m_ItemInspectionController.NotifyPropertyChanged("ItemPreview");
		m_ItemInspectionController.ItemName = m_Item.GetDisplayName();
		m_ItemInspectionController.NotifyPropertyChanged("ItemName");
	}

	void SetContentOffset(float x, float y)
	{
		m_ContentOffsetX = x;
		m_ContentOffsetY = y;
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
		GetMousePos(x, y);
		GetLayoutRoot().SetPos(x + m_ContentOffsetX, y + m_ContentOffsetY, true);
	}
};

class ExpansionItemPreviewTooltipController: ExpansionViewController
{
	string ItemName;
	Object ItemPreview;
};
#endif
