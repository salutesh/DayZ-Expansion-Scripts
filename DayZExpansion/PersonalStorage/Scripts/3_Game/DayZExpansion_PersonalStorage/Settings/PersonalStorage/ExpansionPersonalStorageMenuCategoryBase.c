/**
 * ExpansionPersonalStorageMenuCategoryBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuCategoryBase
{
	protected string DisplayName;
	protected string IconPath;
	protected ref TStringArray Included;
	protected ref TStringArray Excluded;

	void ExpansionPersonalStorageMenuCategoryBase()
	{
		Included = new TStringArray;
		Excluded = new TStringArray;
	}

	void SetDisplayName(string name)
	{
		DisplayName = name;
	}

	string GetDisplayName()
	{
		return DisplayName;
	}

	void SetIconPath(string path)
	{
		IconPath = path;
	}

	string GetIconPath()
	{
		return IconPath;
	}

	void AddIncluded(string typeName)
	{
		Included.Insert(typeName);
	}

	TStringArray GetIncluded()
	{
		return Included;
	}

	void AddExcluded(string typeName)
	{
		Excluded.Insert(typeName);
	}

	TStringArray GetExcluded()
	{
		return Excluded;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(DisplayName);
		ctx.Write(IconPath);

		int i;
		int includedCount = Included.Count();
		ctx.Write(includedCount);
		for (i = 0; i < Included.Count(); i++)
		{
			string includedTypeName = Included[i];
			ctx.Write(includedTypeName);
		}

		int excludedCount = Excluded.Count();
		ctx.Write(excludedCount);
		for (i = 0; i < Excluded.Count(); i++)
		{
			string excludedTypeName = Excluded[i];
			ctx.Write(excludedTypeName);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		ctx.Read(DisplayName);
		ctx.Read(IconPath);

		int i;
		int includedCount;
		ctx.Read(includedCount);

		if (includedCount > 0)
		{
			if (!Included)
				Included = new TStringArray;
			else
				Included.Clear();

			for (i = 0; i < includedCount; i++)
			{
				string includedTypeName;
				ctx.Read(includedTypeName);
				Included.Insert(includedTypeName);
			}
		}

		int excludedCount;
		ctx.Read(excludedCount);

		if (excludedCount > 0)
		{
			if (!Excluded)
				Excluded = new TStringArray;
			else
				Excluded.Clear();

			for (i = 0; i < excludedCount; i++)
			{
				string excludedTypeName;
				ctx.Read(excludedTypeName);
				Excluded.Insert(excludedTypeName);
			}
		}

		return true;
	}
};