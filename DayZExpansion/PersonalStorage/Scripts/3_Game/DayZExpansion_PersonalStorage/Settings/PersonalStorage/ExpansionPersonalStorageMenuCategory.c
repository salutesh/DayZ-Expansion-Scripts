/**
 * ExpansionPersonalStorageMenuCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuCategory: ExpansionPersonalStorageMenuCategoryBase
{
	protected ref array<ref ExpansionPersonalStorageMenuSubCategory> SubCategories;

	void ExpansionPersonalStorageMenuCategory()
	{
		Included = new TStringArray;
		Excluded = new TStringArray;
		SubCategories = new array<ref ExpansionPersonalStorageMenuSubCategory>;
	}

	void AddSubCategory(ExpansionPersonalStorageMenuSubCategory category)
	{
		SubCategories.Insert(category);
	}

	array<ref ExpansionPersonalStorageMenuSubCategory> GetSubCategories()
	{
		return SubCategories;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		int subCategoriesCount = SubCategories.Count();
		ctx.Write(subCategoriesCount);
		for (int i = 0; i < SubCategories.Count(); i++)
		{
			SubCategories[i].OnSend(ctx);
		}
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		super.OnRecieve(ctx);

		int subCategoriesCount;
		ctx.Read(subCategoriesCount);

		if (subCategoriesCount > 0)
		{
			if (!SubCategories)
				SubCategories = new array<ref ExpansionPersonalStorageMenuSubCategory>;
			else
				SubCategories.Clear();

			for (int i = 0; i < subCategoriesCount; i++)
			{
				ExpansionPersonalStorageMenuSubCategory subCategory = new ExpansionPersonalStorageMenuSubCategory();
				subCategory.OnRecieve(ctx);
				SubCategories.Insert(subCategory);
			}
		}

		return true;
	}
};

class ExpansionPersonalStorageMenuSubCategory: ExpansionPersonalStorageMenuCategoryBase {};