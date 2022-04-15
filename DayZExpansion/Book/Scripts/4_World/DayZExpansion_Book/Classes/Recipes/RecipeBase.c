/**
 * RecipeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class RecipeBase
{
	protected static ref map<string, string> m_Expansion_TypeDisplayNames;

	array<ref array<ref ExpansionBookCraftingItem>> ExpansionGetIngredients()
	{
		array<ref array<ref ExpansionBookCraftingItem>> allIngredients = new array<ref array<ref ExpansionBookCraftingItem>>;

		for (int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			map<string, ref ExpansionBookCraftingItem> tmp = new map<string, ref ExpansionBookCraftingItem>;
			map<string, ref array<string>> displayNamesTypes = new map<string, ref array<string>>;

			for (int j = 0; j < m_Ingredients[i].Count(); j++)
			{
				string className = m_Ingredients[i][j];

				if (!ExpansionStatic.ItemExists(className))
					continue;

				string displayName = "";

				bool updateAmount = true;

				//! Hmm. This is still a bit iffy but should work ok for now
				array<string> itemInfo = new array<string>;
				GetGame().ConfigGetTextArray("CfgVehicles " + className + " itemInfo", itemInfo);
				bool isBaseballBat = false;
				bool isKnife = false;
				bool isAxe = false;
				bool isBayonet = false;
				bool isSaw = false;
				if (className == "Hacksaw" || className == "HandSaw")
					isSaw = true;
				else if (className.Contains("BaseballBat"))
					isBaseballBat = true;
				else if (className.Contains("Knife") && itemInfo.Find("Knife") > -1)
					isKnife = true;
				else if (className.Contains("Axe") && itemInfo.Find("Axe") > -1)
					isAxe = true;
				else if (className.Contains("Bayonet") && itemInfo.Find("Knife") > -1)
					isBayonet = true;
				if (isBaseballBat || isKnife || isAxe || isBayonet || isSaw)
				{
					//! Don't show all the different types, just show a generic "Knife", "Axe", "Bayonet" or "Baseball Bat"
					if (isKnife)
						displayName = "#STR_EXPANSION_GENERIC_KNIFE";
					else if (isAxe)
						displayName = "#STR_EXPANSION_GENERIC_AXE";
					else if (isBayonet)
						displayName = "#STR_EXPANSION_GENERIC_BAYONET";
					else if (isSaw)
						displayName = "#STR_EXPANSION_GENERIC_SAW";
					else
						displayName = "#str_cfgvehicles_baseballbat0";

					updateAmount = false;
				}
				else
				{
					displayName = ExpansionStatic.GetItemDisplayNameWithType(className, m_Expansion_TypeDisplayNames);
					
					//! Deal with the case where several different class names have the same display name
					array<string> displayNameTypes;
					if (displayNamesTypes.Find(displayName, displayNameTypes))
					{
						updateAmount = displayNameTypes.Find(className) > -1;
					}
					else
					{
						displayNameTypes = new array<string>;
						displayNamesTypes.Insert(displayName, displayNameTypes);
					}
					displayNameTypes.Insert(className);
				}

				if (!displayName || displayName.IndexOf("$UNT$") == 0)  //! Skip not ready items
					continue;

				int amount = ExpansionGetAmount(className, m_MinQuantityIngredient[i]);

				ExpansionBookCraftingItem ingredient;
				if (tmp.Find(displayName, ingredient))
				{
					if (updateAmount)
						ingredient.Amount += amount;
				}
				else
				{
					ingredient = new ExpansionBookCraftingItem(displayName, amount);
					tmp.Insert(displayName, ingredient);
				}
				ingredient.ClassNames.Insert(className);
			}

			allIngredients.Insert(ExpansionGetItems(tmp));
		}

		return allIngredients;
	}

	array<ref ExpansionBookCraftingItem> ExpansionGetResults()
	{
		map<string, ref ExpansionBookCraftingItem> tmp = new map<string, ref ExpansionBookCraftingItem>;
		for (int i = 0; i < m_NumberOfResults; i++)
		{
			string className = m_ItemsToCreate[i];
			string displayName = "";

			if( m_ResultInheritsColor[i] != -1 )
				className += "ColorBase";

			displayName = ExpansionStatic.GetItemDisplayNameWithType(className, m_Expansion_TypeDisplayNames);

			if (!displayName || displayName.IndexOf("$UNT$") == 0)  //! Skip not ready items
				continue;

			int amount = ExpansionGetAmount(className, m_ResultSetQuantity[i]);

			ExpansionBookCraftingItem result;
			if (tmp.Find(displayName, result))
			{
				result.Amount += amount;
			}
			else
			{
				result = new ExpansionBookCraftingItem(displayName, amount);
				tmp.Insert(displayName, result);
			}
			result.ClassNames.Insert(className);
		}

		return ExpansionGetItems(tmp);
	}

	protected static int ExpansionGetAmount(string className, int quantity)
	{
		if (quantity != -1)
		{
			TStringArray cfgSearches = {CFG_VEHICLESPATH, CFG_MAGAZINESPATH};
			foreach (string cfgSearch : cfgSearches)
			{
				//! Only set amount if item can be split
				string cfgPath = cfgSearch + " " + className + " canBeSplit";
				if (GetGame().ConfigIsExisting(cfgPath) && GetGame().ConfigGetInt(cfgPath))
				{
					return quantity;
				}
			}
		}

		return 1;
	}

	array<ref ExpansionBookCraftingItem> ExpansionGetItems(map<string, ref ExpansionBookCraftingItem> tmp)
	{
		array<ref ExpansionBookCraftingItem> items = new array<ref ExpansionBookCraftingItem>;
		for (int i = 0; i < tmp.Count(); i++)
		{
			items.Insert(tmp.GetElement(i));
		}
		return items;
	}
}
