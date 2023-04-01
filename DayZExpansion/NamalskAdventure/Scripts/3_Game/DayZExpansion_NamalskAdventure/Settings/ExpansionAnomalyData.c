/**
 * ExpansionAnomalyData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyData
{
	string AnomalyType;
	vector CenterPosition;
	int Range;
	int Distance;
	int MaxDistance;
	int Amount;

	void ExpansionAnomalyData(string anomalyType, vector center, int range, int dist, int amount)
	{
		AnomalyType = anomalyType;
		CenterPosition = center;
		Range = range;
		Distance = dist;
		Amount = amount;
	}
};