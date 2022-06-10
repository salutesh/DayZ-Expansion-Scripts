/**
 * ExpansionDefaultQuestNPCData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDefaultQuestNPCData
{
	private string m_WorldName;
	
	void ExpansionDefaultQuestNPCData(string worldName)
	{
		m_WorldName = worldName;
	}
	
	 ExpansionQuestNpcData QuestNPCData_1()
	{
		ExpansionQuestNpcData questNPC = new ExpansionQuestNpcData;
		questNPC.SetID(1); //! Unique NPC ID
		questNPC.SetName("ExpansionQuestNpcAIDenis"); //! Class name of the NPC entity

		array<int> questIDs = new array<int>;
		questIDs.Insert(1);
		questIDs.Insert(3);
		questIDs.Insert(4);
		questIDs.Insert(5);
		questIDs.Insert(6); //! AI patrol test quest [WIP]
		questIDs.Insert(7); //! AI camp test quest [WIP]
		questIDs.Insert(8);
		questIDs.Insert(22);
		questNPC.SetQuestIDs(questIDs); //! Quests IDs of the quests this NPC can head-out/ACCEPTABLE_DISTANCE
		
		if (m_WorldName == "namalsk")
		{
			questNPC.SetPosition(Vector(8584.27, 14.73, 10511.6)); //! Quest NPC position
			questNPC.SetOrientation(Vector(200.0, 0, 0)); //! Quest NPC orientation
		
		#ifdef EXPANSIONMODAI
			questNPC.SetIsAI(true);
			questNPC.AddWaypoint(Vector(8584.27, 14.73, 10511.6));
		#endif
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			questNPC.SetPosition(Vector(3706.27, 402.012, 5987.08)); //! Quest NPC position
			questNPC.SetOrientation(Vector(282.0, 0, 0)); //! Quest NPC orientation
			
		#ifdef EXPANSIONMODAI
			questNPC.SetIsAI(true);
			questNPC.AddWaypoint(Vector(3706.27, 402.012, 5987.08));
		#endif
		}

		TStringArray gear = new TStringArray;
		gear.Insert("GasMask");
		gear.Insert("NBCJacketGray");
		gear.Insert("Shirt_GreenCheck");
		gear.Insert("NBCPantsGray");
		gear.Insert("CombatBoots_Brown");
		questNPC.SetGear(gear); //! Quest NPC gear

		questNPC.SetNPCName("Peter");
		questNPC.SetDefaultNPCText("Hm?");

		return questNPC;
	}

	ExpansionQuestNpcData QuestNPCData_2()
	{
		ExpansionQuestNpcData questNPC = new ExpansionQuestNpcData;
		questNPC.SetID(2); //! Unique NPC ID
		questNPC.SetName("ExpansionQuestNpcAIElias"); //! Class name of the NPC entity

		array<int> questIDs = new array<int>;
		questIDs.Insert(1);
		questIDs.Insert(2);
		questIDs.Insert(3);
		questIDs.Insert(22);
		questNPC.SetQuestIDs(questIDs); //! Quests IDs of the quests this NPC can head-out/accept
		questNPC.SetOrientation(Vector(-10.0, 0, 0)); //! Quest NPC orientation
		
		if (m_WorldName == "namalsk")
		{
			questNPC.SetPosition(Vector(8348.39, 15.1237, 10724.7)); //! Quest NPC position
			questNPC.SetOrientation(Vector(-10.0, 0, 0)); //! Quest NPC orientation
			
		#ifdef EXPANSIONMODAI
			questNPC.SetIsAI(true);
			questNPC.AddWaypoint(Vector(8348.39, 15.1237, 10724.7));
		#endif
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			questNPC.SetPosition(Vector(3192.2, 296.707, 6093.31)); //! Quest NPC position
			questNPC.SetOrientation(Vector(145.0, 0, 0)); //! Quest NPC orientation
			
		#ifdef EXPANSIONMODAI
			questNPC.SetIsAI(true);
			questNPC.AddWaypoint(Vector(3192.2, 296.707, 6093.31));
		#endif
		}

		TStringArray gear = new TStringArray;
		gear.Insert("GasMask");
		gear.Insert("NBCJacketGray");
		gear.Insert("Shirt_GreenCheck");
		gear.Insert("NBCPantsGray");
		gear.Insert("CombatBoots_Brown");
		questNPC.SetGear(gear); //! Quest NPC gear

		questNPC.SetNPCName("Steve");
		questNPC.SetDefaultNPCText("What do you want? I am bussy...");

		return questNPC;
	}

#ifdef EXPANSIONMODHARDLINE
	//! Bandit Quest NPC
	ExpansionQuestNpcData QuestNPCData_3()
	{
		ExpansionQuestNpcData questNPC = new ExpansionQuestNpcData;
		questNPC.SetID(3); //! Unique NPC ID
		questNPC.SetName("ExpansionQuestNpcAIJose"); //! Class name of the NPC entity

		array<int> questIDs = new array<int>;
		questIDs.Insert(9);
		questIDs.Insert(10);
		questIDs.Insert(11);
		questIDs.Insert(12);
		questIDs.Insert(17);
		questNPC.SetQuestIDs(questIDs); //! Quests IDs of the quests this NPC can head-out/accept
		questNPC.SetPosition(Vector(8563.02, 14.8878, 10537.7)); //! Quest NPC position
		questNPC.SetOrientation(Vector(4.81, 0, 0)); //! Quest NPC orientation

		TStringArray gear = new TStringArray;
		gear.Insert("GasMask");
		gear.Insert("NBCJacketGray");
		gear.Insert("Shirt_GreenCheck");
		gear.Insert("NBCPantsGray");
		gear.Insert("CombatBoots_Brown");
		questNPC.SetGear(gear); //! Quest NPC gear

		questNPC.SetNPCName("Jose");
		questNPC.SetDefaultNPCText("There is nothing to do here for you...");

	#ifdef EXPANSIONMODAI
		questNPC.SetIsAI(true);
		questNPC.AddWaypoint(Vector(8563.02, 14.8878, 10537.7));
	#endif

		return questNPC;
	}

	//! Hero Quest NPC
	ExpansionQuestNpcData QuestNPCData_4()
	{
		ExpansionQuestNpcData questNPC = new ExpansionQuestNpcData;
		questNPC.SetID(4); //! Unique NPC ID
		questNPC.SetName("ExpansionQuestNpcAIPeter"); //! Class name of the NPC entity

		array<int> questIDs = new array<int>;
		questIDs.Insert(13);
		questIDs.Insert(14);
		questIDs.Insert(15);
		questIDs.Insert(16);
		questIDs.Insert(18);
		questNPC.SetQuestIDs(questIDs); //! Quests IDs of the quests this NPC can head-out/accept
		questNPC.SetPosition(Vector(8597.82, 14.9004, 10493.3)); //! Quest NPC position
		questNPC.SetOrientation(Vector(-91.50, 0, 0)); //! Quest NPC orientation

		TStringArray gear = new TStringArray;
		gear.Insert("GasMask");
		gear.Insert("NBCJacketGray");
		gear.Insert("Shirt_GreenCheck");
		gear.Insert("NBCPantsGray");
		gear.Insert("CombatBoots_Brown");
		questNPC.SetGear(gear); //! Quest NPC gear

		questNPC.SetNPCName("Guillaume");
		questNPC.SetDefaultNPCText("There is nothing to do here for you...");

	#ifdef EXPANSIONMODAI
		questNPC.SetIsAI(true);
		questNPC.AddWaypoint(Vector(8597.82, 14.9004, 10493.3));
	#endif

		return questNPC;
	}
#endif
};