/**
 * Construction.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Construction
{
	//! Builds the whole thing without requiring materials
	//! Useful for client-side preview, currently used by Market for that purpose
	void ExpansionBuildFull()
	{
		TStringArray construction_parts = GetConstructionParts().GetKeyArray();
		construction_parts.Sort();
		foreach (string part_name : construction_parts)
		{
			ExpansionBuildPartFull(part_name);
		}
		GetParent().UpdateVisuals();
	}

	//! Builds the part and all required parts, returns the part if it exists, else NULL
	ConstructionPart ExpansionBuildPartFull(string part_name)
	{
		ConstructionPart constrution_part = GetConstructionPart(part_name);
		if (constrution_part)
			ExpansionBuildPartFull(constrution_part);
		return constrution_part;
	}

	//! Builds the part and all required parts
	void ExpansionBuildPartFull(ConstructionPart constrution_part)
	{
		string part_name = constrution_part.GetPartName();

		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::ExpansionBuildPartFull " + constrution_part.GetName() + " (" + part_name + "), main part " + constrution_part.GetMainPartName() + ", is built " + constrution_part.IsBuilt() + ", has conflicting part " + HasConflictPart(part_name));
		#endif

		if (!constrution_part.IsBuilt() && !HasConflictPart(part_name))
		{
			array<string> required_parts = constrution_part.GetRequiredParts();
			foreach (string required_part_name : required_parts)
			{
				ConstructionPart required_part = GetConstructionPart(required_part_name);
				ExpansionBuildPartFull(required_part);
				if (!required_part.IsBuilt())
					return;
			}

			constrution_part.SetBuiltState(true);

			if (constrution_part.IsBase())
				GetParent().SetBaseState(true);

			GetParent().ExpansionUpdateBaseBuildingStateFromPartBuilt(part_name);
		}
	}
}
