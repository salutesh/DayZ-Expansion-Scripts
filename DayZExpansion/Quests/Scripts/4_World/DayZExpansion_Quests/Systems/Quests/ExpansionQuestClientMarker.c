/**
 * ExpansionQuestClientMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODNAVIGATION
class ExpansionQuestClientMarker
{
	protected int QuestID = -1;
	protected string MarkerUID = string.Empty;

	void ExpansionQuestClientMarker(int questID, string markerUID)
	{
		QuestID = questID;
		MarkerUID = markerUID;
	}

	int GetQuestID()
	{
		return QuestID;
	}

	string GetMarkerUID()
	{
		return MarkerUID;
	}
};
#endif