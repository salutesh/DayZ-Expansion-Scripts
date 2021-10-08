/**
 * ExpansionMarketMenuColors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

class ExpansionMarketMenuColors
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

	void Update()
	{
		if (!BaseColorVignette)
			BaseColorVignette = ExpansionColor.ARGBToHex(200, 0, 0, 0);
		
		if (!BaseColorHeaders)
			BaseColorHeaders = ExpansionColor.ARGBToHex(255, 19, 23, 27);								//! Expansion dark gray
	
		if (!BaseColorLabels)
			BaseColorLabels = ExpansionColor.ARGBToHex(255, 39, 39, 45);									//! Expansion bright gray 
		
		if (!BaseColorText)
			BaseColorText = ExpansionColor.ARGBToHex(255, 251, 252, 254);								//! Expansion white
		
		if (!BaseColorCheckboxes)
			BaseColorCheckboxes = ExpansionColor.ARGBToHex(255, 251, 252, 254);						//! Expansion white
		
		if (!BaseColorInfoSectionBackground)
			BaseColorInfoSectionBackground = ExpansionColor.ARGBToHex(140, 34, 37, 38);			//! Expansion gray 
		
		if (!BaseColorTooltipsBackground)
			BaseColorTooltipsBackground = ExpansionColor.ARGBToHex(220, 0, 0, 0);						//! Expansion bright gray
		
		if (!BaseColorTooltipsHeaders)
			BaseColorTooltipsHeaders = ExpansionColor.ARGBToHex(240, 0, 0, 0);							//! Expansion bright black 
		
		if (!ColorDecreaseQuantityButton)
			ColorDecreaseQuantityButton = ExpansionColor.ARGBToHex(20, 221, 38, 38);					//! Expansion red 3
		
		if (!ColorDecreaseQuantityIcon)
			ColorDecreaseQuantityIcon = ExpansionColor.ARGBToHex(40, 221, 38, 38);					//! Expansion red 3
		
		if (!ColorSetQuantityButton)
			ColorSetQuantityButton = ExpansionColor.ARGBToHex(20, 199, 38, 81); 						//! Expansion red
		
		if (!ColorIncreaseQuantityButton)
			ColorIncreaseQuantityButton = ExpansionColor.ARGBToHex(20, 160, 204, 113);				//! Expansion green
		
		if (!ColorIncreaseQuantityIcon)
			ColorIncreaseQuantityIcon = ExpansionColor.ARGBToHex(40, 160, 204, 113);					//! Expansion green
		
		if (!ColorSellPanel)
			ColorSellPanel = ExpansionColor.ARGBToHex(255, 39, 39, 45);										//! Expansion bright gray 
		
		if (!ColorSellButton)
			ColorSellButton = ExpansionColor.ARGBToHex(40, 221, 38, 38);									//! Expansion red 3
		
		if (!ColorBuyPanel)
			ColorBuyPanel = ExpansionColor.ARGBToHex(255, 39, 39, 45);										//! Expansion bright gray 
		
		if (!ColorBuyButton)
			ColorBuyButton = ExpansionColor.ARGBToHex(40, 160, 204, 113);									//! Expansion green
		
		if (!ColorMarketIcon)
			ColorMarketIcon = ExpansionColor.ARGBToHex(140, 226, 65, 66);									//! Expansion red 2
		
		if (!ColorFilterOptionsButton)
			ColorFilterOptionsButton = ExpansionColor.ARGBToHex(140, 199, 38, 81);						//! Expansion red
		
		if (!ColorFilterOptionsIcon)
			ColorFilterOptionsIcon = ExpansionColor.ARGBToHex(140, 199, 38, 81);							//! Expansion red
		
		if (!ColorSearchFilterButton)
			ColorSearchFilterButton = ExpansionColor.ARGBToHex(140, 199, 38, 81);						//! Expansion red
		
		if (!ColorCategoryButton)	
			ColorCategoryButton = ExpansionColor.ARGBToHex(140, 199, 38, 81);							//! Expansion red
		
		if (!ColorCategoryCollapseIcon)
			ColorCategoryCollapseIcon = ExpansionColor.ARGBToHex(140, 199, 38, 81);					//! Expansion red
		
		if (!ColorCurrencyDenominationText)
			ColorCurrencyDenominationText = ExpansionColor.ARGBToHex(255, 255, 180, 24);			//! Expansion yellow
		
		if (!ColorItemButton)
			ColorItemButton = ExpansionColor.ARGBToHex(140, 199, 38, 81); 								//! Expansion red
		
		if (!ColorItemInfoIcon)		
			ColorItemInfoIcon = ExpansionColor.ARGBToHex(255, 255, 180, 24);								//! Expansion yellow
		
		if (!ColorItemInfoTitle)
			ColorItemInfoTitle = ExpansionColor.ARGBToHex(255, 255, 180, 24);								//! Expansion yellow
		
		if (!ColorItemInfoHasContainerItems)
			ColorItemInfoHasContainerItems = ExpansionColor.ARGBToHex(255, 255, 180, 24);		//! Expansion yellow
			
		if (!ColorItemInfoHasAttachments)
			ColorItemInfoHasAttachments = ExpansionColor.ARGBToHex(255, 255, 180, 24);			//! Expansion yellow
			
		if (!ColorItemInfoHasBullets)
			ColorItemInfoHasBullets = ExpansionColor.ARGBToHex(255, 255, 180, 24);						//! Expansion yellow
			
		if (!ColorItemInfoIsAttachment)	
			ColorItemInfoIsAttachment = ExpansionColor.ARGBToHex(255, 255, 180, 24);				//! Expansion yellow
			
		if (!ColorItemInfoIsEquipped)	
			ColorItemInfoIsEquipped = ExpansionColor.ARGBToHex(255, 255, 180, 24);					//! Expansion yellow
			
		if (!ColorItemInfoAttachments)
			ColorItemInfoAttachments = ExpansionColor.ARGBToHex(255, 255, 180, 24);					//! Expansion yellow

		if (!ColorToggleCategoriesText)
			ColorToggleCategoriesText = ExpansionColor.ARGBToHex(140, 199, 38, 81);					//! Expansion red
			
		if (!ColorCategoryCorners)
			ColorCategoryCorners = ExpansionColor.ARGBToHex(255, 251, 252, 254);						//! Expansion white
			
		if (!ColorCategoryBackground)	
			ColorCategoryBackground = ExpansionColor.ARGBToHex(255, 34, 37, 38);						//! Expansion gray 
	}
}
