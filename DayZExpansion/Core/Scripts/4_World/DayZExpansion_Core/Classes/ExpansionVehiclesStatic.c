/**
 * ExpansionVehiclesStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionVehiclesStatic
{
	static vector GetCenterSelection(Object target, string pLODName, string pSelectionName)
	{
		LOD lod = target.GetLODByName(pLODName);
		if (!lod)
			return "0 0 0";

		Selection selection = lod.GetSelectionByName(pSelectionName);
		if (!selection)
			return "0 0 0";

		vector offset = vector.Zero; // m_WheelDirectionMS * m_Axle.GetWheelHubRadius() * -0.5;

		vector totalVertices = "0 0 0";
		int count = selection.GetVertexCount();
		for (int i = 0; i < count; ++i)
		{
			vector vp = lod.GetVertexPosition(selection.GetLODVertexIndex(i)) + offset;
			totalVertices += vp;
		}

		if (count == 0)
			return "0 0 0";

		return Vector(totalVertices[0] / count, totalVertices[1] / count, totalVertices[2] / count);
	}
};
