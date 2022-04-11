/**
 * ExpansionMarketMenuColors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMarketMenuColorsV2
 * @brief		Market color settings v2 class
 **/

class ExpansionMarketMenuColorsV2
{
	int BaseColorVignette;
	int BaseColorHeaders;
	int BaseColorLabels;
	int BaseColorTexts;
	int BaseColorCheckboxes;
	int BaseColorInfoSectionBackground;
	int BaseColorTooltipsCorners;
	int BaseColorTooltipsSeperatorLine;
	int BaseColorTooltipsBackground;
	
	int ColorDecreaseQuantityButton;
	int ColorDecreaseQuantityIcon;
	int ColorSetQuantityButton;
	int ColorIncreaseQuantityButton;
	int ColorIncreaseQuantityIcon;
	int ColorSellPanel;
	int ColorSellButton;
	int ColorBuyPanel;
	int ColorBuyButton;
		
	int ColorMarketIcon;
	int ColorFilterOptionsButton;
	int ColorFilterOptionsIcon;
	int ColorSearchFilterButton;
	int ColorCategoryButton;
	int ColorCategoryCollapseIcon;
	int ColorCurrencyDenominationTexts;
	int ColorItemButton;
	int ColorItemInfoIcon;
	int ColorItemInfoTitle;
	int ColorItemInfoHasContainerItems;
	int ColorItemInfoHasAttachments;
	int ColorItemInfoHasBullets;
	int ColorItemInfoIsAttachment;
	int ColorItemInfoIsEquiped;
	int ColorItemInfoAttachments;
	int ColorToggleCategoriesText;
	
	int ColorCategoryCorners;
	int ColorCategoryBackground;
}

/**@class		ExpansionMarketMenuColors
 * @brief		Market color settings class
 **/

class ExpansionMarketMenuColors: ExpansionColorSettingsBase
{
	string BaseColorVignette;
	string BaseColorHeaders;
	string BaseColorLabels;
	string BaseColorText;
	string BaseColorCheckboxes;
	string BaseColorInfoSectionBackground;
	string BaseColorTooltipsHeaders;
	string BaseColorTooltipsBackground;
	
	string ColorDecreaseQuantityButton;
	string ColorDecreaseQuantityIcon;
	string ColorSetQuantityButton;
	string ColorIncreaseQuantityButton;
	string ColorIncreaseQuantityIcon;
	string ColorSellPanel;
	string ColorSellButton;
	string ColorBuyPanel;
	string ColorBuyButton;
		
	string ColorMarketIcon;
	string ColorFilterOptionsButton;
	string ColorFilterOptionsIcon;
	string ColorSearchFilterButton;
	string ColorCategoryButton;
	string ColorCategoryCollapseIcon;
	string ColorCurrencyDenominationText;
	string ColorItemButton;
	string ColorItemInfoIcon;
	string ColorItemInfoTitle;
	string ColorItemInfoHasContainerItems;
	string ColorItemInfoHasAttachments;
	string ColorItemInfoHasBullets;
	string ColorItemInfoIsAttachment;
	string ColorItemInfoIsEquipped;
	string ColorItemInfoAttachments;
	string ColorToggleCategoriesText;
	
	string ColorCategoryCorners;
	string ColorCategoryBackground;

	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults
		if (!Get("BaseColorVignette"))
			Set("BaseColorVignette", ARGB(200, 0, 0, 0));
		
		if (!Get("BaseColorHeaders"))
			Set("BaseColorHeaders", ARGB(255, 19, 23, 27));								//! Expansion dark gray
	
		if (!Get("BaseColorLabels"))
			Set("BaseColorLabels", ARGB(255, 39, 39, 45));									//! Expansion bright gray 
		
		if (!Get("BaseColorText"))
			Set("BaseColorText", ARGB(255, 251, 252, 254));								//! Expansion white
		
		if (!Get("BaseColorCheckboxes"))
			Set("BaseColorCheckboxes", ARGB(255, 251, 252, 254));						//! Expansion white
		
		if (!Get("BaseColorInfoSectionBackground"))
			Set("BaseColorInfoSectionBackground", ARGB(140, 34, 37, 38));			//! Expansion gray 
		
		if (!Get("BaseColorTooltipsBackground"))
			Set("BaseColorTooltipsBackground", ARGB(220, 0, 0, 0));						//! Expansion bright gray
		
		if (!Get("BaseColorTooltipsHeaders"))
			Set("BaseColorTooltipsHeaders", ARGB(240, 0, 0, 0));							//! Expansion bright black 
		
		if (!Get("ColorDecreaseQuantityButton"))
			Set("ColorDecreaseQuantityButton", ARGB(20, 221, 38, 38));					//! Expansion red 3
		
		if (!Get("ColorDecreaseQuantityIcon"))
			Set("ColorDecreaseQuantityIcon", ARGB(40, 221, 38, 38));					//! Expansion red 3
		
		if (!Get("ColorSetQuantityButton"))
			Set("ColorSetQuantityButton", ARGB(20, 199, 38, 81)); 						//! Expansion red
		
		if (!Get("ColorIncreaseQuantityButton"))
			Set("ColorIncreaseQuantityButton", ARGB(20, 160, 204, 113));				//! Expansion green
		
		if (!Get("ColorIncreaseQuantityIcon"))
			Set("ColorIncreaseQuantityIcon", ARGB(40, 160, 204, 113));					//! Expansion green
		
		if (!Get("ColorSellPanel"))
			Set("ColorSellPanel", ARGB(255, 39, 39, 45));										//! Expansion bright gray 
		
		if (!Get("ColorSellButton"))
			Set("ColorSellButton", ARGB(40, 221, 38, 38));									//! Expansion red 3
		
		if (!Get("ColorBuyPanel"))
			Set("ColorBuyPanel", ARGB(255, 39, 39, 45));										//! Expansion bright gray 
		
		if (!Get("ColorBuyButton"))
			Set("ColorBuyButton", ARGB(40, 160, 204, 113));									//! Expansion green
		
		if (!Get("ColorMarketIcon"))
			Set("ColorMarketIcon", ARGB(140, 226, 65, 66));									//! Expansion red 2
		
		if (!Get("ColorFilterOptionsButton"))
			Set("ColorFilterOptionsButton", ARGB(140, 199, 38, 81));						//! Expansion red
		
		if (!Get("ColorFilterOptionsIcon"))
			Set("ColorFilterOptionsIcon", ARGB(140, 199, 38, 81));							//! Expansion red
		
		if (!Get("ColorSearchFilterButton"))
			Set("ColorSearchFilterButton", ARGB(140, 199, 38, 81));						//! Expansion red
		
		if (!Get("ColorCategoryButton"))	
			Set("ColorCategoryButton", ARGB(140, 199, 38, 81));							//! Expansion red
		
		if (!Get("ColorCategoryCollapseIcon"))
			Set("ColorCategoryCollapseIcon", ARGB(140, 199, 38, 81));					//! Expansion red
		
		if (!Get("ColorCurrencyDenominationText"))
			Set("ColorCurrencyDenominationText", ARGB(255, 255, 180, 24));			//! Expansion yellow
		
		if (!Get("ColorItemButton"))
			Set("ColorItemButton", ARGB(140, 199, 38, 81)); 								//! Expansion red
		
		if (!Get("ColorItemInfoIcon"))		
			Set("ColorItemInfoIcon", ARGB(255, 255, 180, 24));								//! Expansion yellow
		
		if (!Get("ColorItemInfoTitle"))
			Set("ColorItemInfoTitle", ARGB(255, 255, 180, 24));								//! Expansion yellow
		
		if (!Get("ColorItemInfoHasContainerItems"))
			Set("ColorItemInfoHasContainerItems", ARGB(255, 255, 180, 24));		//! Expansion yellow
			
		if (!Get("ColorItemInfoHasAttachments"))
			Set("ColorItemInfoHasAttachments", ARGB(255, 255, 180, 24));			//! Expansion yellow
			
		if (!Get("ColorItemInfoHasBullets"))
			Set("ColorItemInfoHasBullets", ARGB(255, 255, 180, 24));						//! Expansion yellow
			
		if (!Get("ColorItemInfoIsAttachment"))	
			Set("ColorItemInfoIsAttachment", ARGB(255, 255, 180, 24));				//! Expansion yellow
			
		if (!Get("ColorItemInfoIsEquipped"))	
			Set("ColorItemInfoIsEquipped", ARGB(255, 255, 180, 24));					//! Expansion yellow
			
		if (!Get("ColorItemInfoAttachments"))
			Set("ColorItemInfoAttachments", ARGB(255, 255, 180, 24));					//! Expansion yellow

		if (!Get("ColorToggleCategoriesText"))
			Set("ColorToggleCategoriesText", ARGB(140, 199, 38, 81));					//! Expansion red
			
		if (!Get("ColorCategoryCorners"))
			Set("ColorCategoryCorners", ARGB(255, 251, 252, 254));						//! Expansion white
			
		if (!Get("ColorCategoryBackground"))	
			Set("ColorCategoryBackground", ARGB(255, 34, 37, 38));						//! Expansion gray 
	}
}
