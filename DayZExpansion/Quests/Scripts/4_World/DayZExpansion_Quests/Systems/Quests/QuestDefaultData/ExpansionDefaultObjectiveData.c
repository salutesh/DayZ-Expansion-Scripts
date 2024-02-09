
/**
 * ExpansionDefaultObjectiveData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDefaultObjectiveData
{
	protected string m_WorldName;

	void ExpansionDefaultObjectiveData(string worldName)
	{
		m_WorldName = worldName;
	}

	//! TRAVEL OBJECTIVES
	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_001()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Get to the Village");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Get to the Village");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(4333.37, 311.779, 6299.88));
		}

		return objective;
	}

	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_002()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Get to the Village");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Get to the Village");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(2596.7, 306.1, 6378.47));
		}

		return objective;
	}

	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_003()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(3);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Get to the Camp");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Get to the Camp");

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(8584.27, 14.73, 10511.6));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(3706.27, 402.012, 5987.08));
		}

		return objective;
	}

	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_004()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(4);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Find the patrol");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Find the patrol");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(6573.94, 20.2864, 11832.3));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(6990.52, 383.992, 11390.1));
		}

		return objective;
	}
	
	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_005()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(5);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Reach destination A.");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Location A");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(6573.94, 20.2864, 11832.3));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(3385.34, 344.645, 6458.37));
		}

		return objective;
	}
	
	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_006()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(6);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Reach destination B.");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Location B");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(6573.94, 20.2864, 11832.3));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(3278.24, 302.404, 6090.88));
		}

		return objective;
	}
	
	ExpansionQuestObjectiveTravelConfig ExpansionQuestObjective_Travel_007()
	{
		ExpansionQuestObjectiveTravelConfig objective = new ExpansionQuestObjectiveTravelConfig();
		objective.SetID(7);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		objective.SetObjectiveText("Reach destination C.");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Location C");
		objective.SetTriggerOnExit(false);

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(6573.94, 20.2864, 11832.3));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(3192.11, 296.707, 6093.09));
		}

		return objective;
	}

	//! DELIVERY OBJECTIVES
	ExpansionQuestObjectiveDeliveryConfig ExpansionQuestObjective_Delivery_001()
	{
		ExpansionQuestObjectiveDeliveryConfig objective = new ExpansionQuestObjectiveDeliveryConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.DELIVERY);
		objective.SetObjectiveText("Deliver the note to Steve");
		objective.AddCollection(1, "ExpansionQuestItemPaper");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Talk to Steve");

		return objective;
	}

	ExpansionQuestObjectiveDeliveryConfig ExpansionQuestObjective_Delivery_002()
	{
		ExpansionQuestObjectiveDeliveryConfig objective = new ExpansionQuestObjectiveDeliveryConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.DELIVERY);
		objective.SetObjectiveText("Deliver the package to Peter");
		objective.AddCollection(1, "ExpansionQuestItemPackage");
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Talk to Peter");

		return objective;
	}

	//! TARGET OBJECTIVES
	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_001()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 10 Infected with a sledgehammer");

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
			objective.SetMaxDistance(150.0);
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(2596.7, 306.1, 6378.47));
			objective.SetMaxDistance(150.0);
		}

		//! Civilian Infected
		objective.AddClassName("ZmbM_CitizenASkinny_Base");
		objective.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		objective.AddClassName("ZmbM_CitizenBFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_LT_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		objective.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		objective.AddClassName("ZmbM_DoctorFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_LT_Base");
		objective.AddClassName("ZmbM_FirefighterNormal_Base");
		objective.AddClassName("ZmbM_FishermanOld_Base");
		objective.AddClassName("ZmbM_HandymanNormal_Base");
		objective.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		objective.AddClassName("ZmbM_HermitSkinny_Base");
		objective.AddClassName("ZmbM_HikerSkinny_Base");
		objective.AddClassName("ZmbM_HunterOld_Base");
		objective.AddClassName("ZmbM_Jacket_Base");
		objective.AddClassName("ZmbM_Jacket_LT_Base");
		objective.AddClassName("ZmbM_JoggerSkinny_Base");
		objective.AddClassName("ZmbM_Runner_Base");
		objective.AddClassName("ZmbM_JournalistSkinny_Base");
		objective.AddClassName("ZmbM_MechanicSkinny_Base");
		objective.AddClassName("ZmbM_MotobikerFat_Base");
		objective.AddClassName("ZmbM_OffshoreWorker_Base");
		objective.AddClassName("ZmbM_ParamedicNormal_Base");
		objective.AddClassName("ZmbM_PatientSkinny_Base");
		objective.AddClassName("ZmbM_PolicemanFat_Base");
		objective.AddClassName("ZmbM_PolicemanSpecForce_Base");
		objective.AddClassName("ZmbM_priestPopSkinny_Base");
		objective.AddClassName("ZmbM_PrisonerSkinny_Base");
		objective.AddClassName("ZmbM_SkaterYoung_Base");
		objective.AddClassName("ZmbM_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbM_SurvivorDean_Base");
		objective.AddClassName("ZmbM_VillagerOld_Base");
		objective.AddClassName("ZmbM_VillagerOld_LT_Base");
		objective.AddClassName("ZmbF_BlueCollarFat_Base");
		objective.AddClassName("ZmbF_CitizenANormal_Base");
		objective.AddClassName("ZmbF_CitizenANormal_LT_Base");
		objective.AddClassName("ZmbF_CitizenBSkinny_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_Base");
		objective.AddClassName("ZmbF_ClerkFat_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		objective.AddClassName("ZmbF_DoctorSkinny_Base");
		objective.AddClassName("ZmbF_HikerSkinny_Base");
		objective.AddClassName("ZmbF_JoggerSkinny_Base");
		objective.AddClassName("ZmbF_Runner_Base");
		objective.AddClassName("ZmbF_JournalistNormal_Base");
		objective.AddClassName("ZmbF_JournalistNormal_LT_Base");
		objective.AddClassName("ZmbF_MechanicNormal_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		objective.AddClassName("ZmbF_NurseFat_Base");
		objective.AddClassName("ZmbF_ParamedicNormal_Base");
		objective.AddClassName("ZmbF_PatientOld_Base");
		objective.AddClassName("ZmbF_PoliceWomanNormal_Base");
		objective.AddClassName("ZmbF_ShortSkirt_Base");
		objective.AddClassName("ZmbF_ShortSkirt_LT_Base");
		objective.AddClassName("ZmbF_SkaterYoung_Base");
		objective.AddClassName("ZmbF_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		objective.AddClassName("ZmbF_VillagerOld_Base");
		objective.AddClassName("ZmbF_VillagerOld_LT_Base");

		objective.SetAmount(10);
		objective.AddAllowedWeapon("SledgeHammer");

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_002()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 100 Infected");
		objective.SetMaxDistance(150.0);

		//! Civilian Infected
		objective.AddClassName("ZmbM_CitizenASkinny_Base");
		objective.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		objective.AddClassName("ZmbM_CitizenBFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_LT_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		objective.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		objective.AddClassName("ZmbM_DoctorFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_LT_Base");
		objective.AddClassName("ZmbM_FirefighterNormal_Base");
		objective.AddClassName("ZmbM_FishermanOld_Base");
		objective.AddClassName("ZmbM_HandymanNormal_Base");
		objective.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		objective.AddClassName("ZmbM_HermitSkinny_Base");
		objective.AddClassName("ZmbM_HikerSkinny_Base");
		objective.AddClassName("ZmbM_HunterOld_Base");
		objective.AddClassName("ZmbM_Jacket_Base");
		objective.AddClassName("ZmbM_Jacket_LT_Base");
		objective.AddClassName("ZmbM_JoggerSkinny_Base");
		objective.AddClassName("ZmbM__Runner_Base");
		objective.AddClassName("ZmbM_JournalistSkinny_Base");
		objective.AddClassName("ZmbM_MechanicSkinny_Base");
		objective.AddClassName("ZmbM_MotobikerFat_Base");
		objective.AddClassName("ZmbM_OffshoreWorker_Base");
		objective.AddClassName("ZmbM_ParamedicNormal_Base");
		objective.AddClassName("ZmbM_PatientSkinny_Base");
		objective.AddClassName("ZmbM_PolicemanFat_Base");
		objective.AddClassName("ZmbM_PolicemanSpecForce_Base");
		objective.AddClassName("ZmbM_priestPopSkinny_Base");
		objective.AddClassName("ZmbM_PrisonerSkinny_Base");
		objective.AddClassName("ZmbM_SkaterYoung_Base");
		objective.AddClassName("ZmbM_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbM_SurvivorDean_Base");
		objective.AddClassName("ZmbM_VillagerOld_Base");
		objective.AddClassName("ZmbM_VillagerOld_LT_Base");
		objective.AddClassName("ZmbF_BlueCollarFat_Base");
		objective.AddClassName("ZmbF_CitizenANormal_Base");
		objective.AddClassName("ZmbF_CitizenANormal_LT_Base");
		objective.AddClassName("ZmbF_CitizenBSkinny_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_Base");
		objective.AddClassName("ZmbF_ClerkFat_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		objective.AddClassName("ZmbF_DoctorSkinny_Base");
		objective.AddClassName("ZmbF_HikerSkinny_Base");
		objective.AddClassName("ZmbF_JoggerSkinny_Base");
		objective.AddClassName("ZmbF_Runner_Base");
		objective.AddClassName("ZmbF_JournalistNormal_Base");
		objective.AddClassName("ZmbF_JournalistNormal_LT_Base");
		objective.AddClassName("ZmbF_MechanicNormal_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		objective.AddClassName("ZmbF_NurseFat_Base");
		objective.AddClassName("ZmbF_ParamedicNormal_Base");
		objective.AddClassName("ZmbF_PatientOld_Base");
		objective.AddClassName("ZmbF_PoliceWomanNormal_Base");
		objective.AddClassName("ZmbF_ShortSkirt_Base");
		objective.AddClassName("ZmbF_ShortSkirt_LT_Base");
		objective.AddClassName("ZmbF_SkaterYoung_Base");
		objective.AddClassName("ZmbF_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		objective.AddClassName("ZmbF_VillagerOld_Base");
		objective.AddClassName("ZmbF_VillagerOld_LT_Base");

		objective.SetAmount(100);

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_003()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(3);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 10 Infected");
		objective.SetMaxDistance(150.0);

		//! Civilian Infected
		objective.AddClassName("ZmbM_CitizenASkinny_Base");
		objective.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		objective.AddClassName("ZmbM_CitizenBFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_Base");
		objective.AddClassName("ZmbM_ClerkFat_LT_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_Base");
		objective.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		objective.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		objective.AddClassName("ZmbM_DoctorFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_Base");
		objective.AddClassName("ZmbM_FarmerFat_LT_Base");
		objective.AddClassName("ZmbM_FirefighterNormal_Base");
		objective.AddClassName("ZmbM_FishermanOld_Base");
		objective.AddClassName("ZmbM_HandymanNormal_Base");
		objective.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		objective.AddClassName("ZmbM_HermitSkinny_Base");
		objective.AddClassName("ZmbM_HikerSkinny_Base");
		objective.AddClassName("ZmbM_HunterOld_Base");
		objective.AddClassName("ZmbM_Jacket_Base");
		objective.AddClassName("ZmbM_Jacket_LT_Base");
		objective.AddClassName("ZmbM_JoggerSkinny_Base");
		objective.AddClassName("ZmbM__Runner_Base");
		objective.AddClassName("ZmbM_JournalistSkinny_Base");
		objective.AddClassName("ZmbM_MechanicSkinny_Base");
		objective.AddClassName("ZmbM_MotobikerFat_Base");
		objective.AddClassName("ZmbM_OffshoreWorker_Base");
		objective.AddClassName("ZmbM_ParamedicNormal_Base");
		objective.AddClassName("ZmbM_PatientSkinny_Base");
		objective.AddClassName("ZmbM_PolicemanFat_Base");
		objective.AddClassName("ZmbM_PolicemanSpecForce_Base");
		objective.AddClassName("ZmbM_priestPopSkinny_Base");
		objective.AddClassName("ZmbM_PrisonerSkinny_Base");
		objective.AddClassName("ZmbM_SkaterYoung_Base");
		objective.AddClassName("ZmbM_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbM_SurvivorDean_Base");
		objective.AddClassName("ZmbM_VillagerOld_Base");
		objective.AddClassName("ZmbM_VillagerOld_LT_Base");
		objective.AddClassName("ZmbF_BlueCollarFat_Base");
		objective.AddClassName("ZmbF_CitizenANormal_Base");
		objective.AddClassName("ZmbF_CitizenANormal_LT_Base");
		objective.AddClassName("ZmbF_CitizenBSkinny_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_Base");
		objective.AddClassName("ZmbF_ClerkFat_Base");
		objective.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		objective.AddClassName("ZmbF_DoctorSkinny_Base");
		objective.AddClassName("ZmbF_HikerSkinny_Base");
		objective.AddClassName("ZmbF_JoggerSkinny_Base");
		objective.AddClassName("ZmbF_Runner_Base");
		objective.AddClassName("ZmbF_JournalistNormal_Base");
		objective.AddClassName("ZmbF_JournalistNormal_LT_Base");
		objective.AddClassName("ZmbF_MechanicNormal_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_Base");
		objective.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		objective.AddClassName("ZmbF_NurseFat_Base");
		objective.AddClassName("ZmbF_ParamedicNormal_Base");
		objective.AddClassName("ZmbF_PatientOld_Base");
		objective.AddClassName("ZmbF_PoliceWomanNormal_Base");
		objective.AddClassName("ZmbF_ShortSkirt_Base");
		objective.AddClassName("ZmbF_ShortSkirt_LT_Base");
		objective.AddClassName("ZmbF_SkaterYoung_Base");
		objective.AddClassName("ZmbF_SkaterYoung_LT_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_Base");
		objective.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		objective.AddClassName("ZmbF_VillagerOld_Base");
		objective.AddClassName("ZmbF_VillagerOld_LT_Base");

		//! Military Infected
		objective.AddClassName("ZmbM_PatrolNormal_Base");
		objective.AddClassName("ZmbM_Soldier_Base");
		objective.AddClassName("ZmbM_SoldierNormal_Base");
		objective.AddClassName("ZmbM_usSoldier_normal_Base");

		//! Imune Infected
		objective.AddClassName("ZmbM_NBC_Yellow");

		//! Imune Military Infected
		objective.AddClassName("ZmbM_NBC_Grey");

		objective.SetAmount(10);

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_004()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(4);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 10 Military/NBC Infected");

		//! Military Infected
		objective.AddClassName("ZmbM_PatrolNormal_Base");
		objective.AddClassName("ZmbM_Soldier_Base");
		objective.AddClassName("ZmbM_SoldierNormal_Base");
		objective.AddClassName("ZmbM_usSoldier_normal_Base");

		//! Imune Infected
		objective.AddClassName("ZmbM_NBC_Yellow");

		//! Imune Military Infected
		objective.AddClassName("ZmbM_NBC_Grey");

		objective.SetAmount(10);

		return objective;
	}

	//! COLLECTION OBJECTIVES
	ExpansionQuestObjectiveCollectionConfig ExpansionQuestObjective_Collection_001()
	{
		ExpansionQuestObjectiveCollectionConfig objective = new ExpansionQuestObjectiveCollectionConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		objective.SetObjectiveText("Collect 5 apples");
		objective.AddCollection(5, "Apple");

		return objective;
	}

	ExpansionQuestObjectiveCollectionConfig ExpansionQuestObjective_Collection_002()
	{
		ExpansionQuestObjectiveCollectionConfig objective = new ExpansionQuestObjectiveCollectionConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		objective.SetObjectiveText("Catch a mackerel");
		objective.AddCollection(1, "Mackerel");

		return objective;
	}

	ExpansionQuestObjectiveCollectionConfig ExpansionQuestObjective_Collection_003()
	{
		ExpansionQuestObjectiveCollectionConfig objective = new ExpansionQuestObjectiveCollectionConfig();
		objective.SetID(3);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		objective.SetObjectiveText("Get the yellow king's helmet");
		objective.AddCollection(1, "DarkMotoHelmet_YellowScarred");

		return objective;
	}

	//! TREASURE HUNT OBJECTIVES
	ExpansionQuestObjectiveTreasureHuntConfig ExpansionQuestObjective_TreasureHunt_001()
	{
		ExpansionQuestObjectiveTreasureHuntConfig objective = new ExpansionQuestObjectiveTreasureHuntConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TREASUREHUNT);
		objective.SetObjectiveText("Find the location of the treasure");

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.AddPosition(Vector(8541.46, 15.3878, 10299.1));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.AddPosition(Vector(2936.48, 350.404, 6369.39));
			objective.AddPosition(Vector(3143.35, 365.776, 6942.04));
			objective.AddPosition(Vector(5233.51, 290.881, 6246.37));
		}
		
		array <ref ExpansionLoot> loot = new array <ref ExpansionLoot>;
		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));
			
		loot.Insert(new ExpansionLoot("Binoculars", NULL, 0.3));
		loot.Insert(new ExpansionLoot("Rangefinder", battery, 0.05));
		loot.Insert(new ExpansionLoot("GhillieAtt_Mossy", NULL, 0.05));
		loot.Insert(new ExpansionLoot("WeaponCleaningKit", NULL, 0.05));

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			loot.Insert(new ExpansionLoot("dzn_map_namalsk", NULL, 0.3));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			loot.Insert(new ExpansionLoot("ChernarusMap", NULL, 0.3));
		}
		
		objective.InsertLoot(loot);

		return objective;
	}

#ifdef EXPANSIONMODAI
	//! AI PATROL OBJECTIVES
	ExpansionQuestObjectiveAIPatrolConfig ExpansionQuestObjective_AIPatrol_001()
	{
		ExpansionQuestObjectiveAIPatrolConfig objective = new ExpansionQuestObjectiveAIPatrolConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AIPATROL);
		objective.SetObjectiveText("Kill all 5 units of the bandit patrol at the marked location");
		
		ExpansionQuestAISpawn aiSpawn = new ExpansionQuestAISpawn();
		aiSpawn.SetNumberOfAI(5);
		aiSpawn.SetName("Bandit");
		aiSpawn.SetCanBeLooted(true);
		
		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			aiSpawn.AddWaypoint(Vector(6307.7, 14.4, 11810.6));
			aiSpawn.AddWaypoint(Vector(6519.9, 17.3, 11901.6));
			aiSpawn.AddWaypoint(Vector(6967.4, 6.5, 11884.6));
			aiSpawn.AddWaypoint(Vector(7216.6, 10.147, 11843.1));
			aiSpawn.AddWaypoint(Vector(7405.78, 23.18, 11655.8));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			aiSpawn.AddWaypoint(Vector(6914.7, 403.027, 11381.7));
			aiSpawn.AddWaypoint(Vector(6931.27, 399.86, 11456.7));
			aiSpawn.AddWaypoint(Vector(6891.92, 397.455, 11496.1));
			aiSpawn.AddWaypoint(Vector(6850.88, 399.147, 11468.1));
			aiSpawn.AddWaypoint(Vector(6883.98, 403.192, 11380.8));
		}
		
		aiSpawn.SetLoadout("BanditLoadout");
		aiSpawn.SetFaction("West");
		aiSpawn.SetBehaviour(eAIWaypointBehavior.HALT_OR_LOOP);
		aiSpawn.SetFormation("Vee");
		aiSpawn.SetSpeed(2.0);
		aiSpawn.SetThreatSpeed(3.0);
		aiSpawn.SetMinAccuracy(0.5);
		aiSpawn.SetMaxAccuracy(0.8);
		aiSpawn.SetCanBeLooted(true);
		aiSpawn.SetUnlimitedReload(true);
		aiSpawn.SetThreatDistanceLimit(500.0);
		aiSpawn.SetDamageMultiplier(1.0);
		aiSpawn.SetDamageReceivedMultiplier(1.0);
		aiSpawn.SetClassNames(ExpansionQuestAISpawn.eAI_UNITS);
		aiSpawn.SetSniperProneDistanceThreshold(300.0);
		aiSpawn.SetRespawnTime(1.0);
		aiSpawn.SetDespawnTime(1.0);
		aiSpawn.SetMinDistanceRadius(50.0);
		aiSpawn.SetMaxDistanceRadius(500.0);
		aiSpawn.SetDespawnRadius(600.0);

		objective.SetAISpawn(aiSpawn);

		return objective;
	}

	//! AI CAMP OBJECTIVES
	ExpansionQuestObjectiveAICampConfig ExpansionQuestObjective_AICamp_001()
	{
		ExpansionQuestObjectiveAICampConfig objective = new ExpansionQuestObjectiveAICampConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AICAMP);
		objective.SetObjectiveText("Kill all 10 bandits at the marked location");
		
		TVectorArray aiPositions = {};
		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			aiPositions.Insert(Vector(8091.23, 15.37, 10832.4));
			aiPositions.Insert(Vector(8095.07, 15.62, 10865.4));
			aiPositions.Insert(Vector(8121.96, 16.09, 10850.6));
			aiPositions.Insert(Vector(8124.99, 20.46, 10852.5));
			aiPositions.Insert(Vector(8082.32, 15.37, 10838.6));
			aiPositions.Insert(Vector(8123.08, 15.30, 10819.7));
			aiPositions.Insert(Vector(8151.75, 15.45, 10815.7));
			aiPositions.Insert(Vector(8165.89, 15.81, 10840.5));
			aiPositions.Insert(Vector(8172.00, 15.90, 10886.2));
			aiPositions.Insert(Vector(8117.52, 15.23, 10864.8));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			aiPositions.Insert(Vector(4351.33, 307.068, 6400.52));
			aiPositions.Insert(Vector(4389.03, 302.802, 6391.77));
			aiPositions.Insert(Vector(4416.67, 301.977, 6395.7));
			aiPositions.Insert(Vector(4431.82, 298.901, 6452.22));
			aiPositions.Insert(Vector(4418.52, 299.6, 6441.92));
			aiPositions.Insert(Vector(4475.16, 295.22, 6461.57));
			aiPositions.Insert(Vector(4485.01, 295.22, 6466.97));
			aiPositions.Insert(Vector(4435.54, 297.747, 6470.3));
			aiPositions.Insert(Vector(4556.3, 289.275, 6390.84));
			aiPositions.Insert(Vector(4523.01, 290.427, 6448.3));
		}
		
		for (int i = 0; i < aiPositions.Count(); i++)
		{
			ExpansionQuestAISpawn aiSpawn = new ExpansionQuestAISpawn();
			aiSpawn.SetNumberOfAI(1);
			aiSpawn.SetName("Bandit");
			aiSpawn.SetCanBeLooted(true);
			aiSpawn.AddWaypoint(aiPositions[i]);
			aiSpawn.SetLoadout("BanditLoadout");
			aiSpawn.SetFaction("West");
			aiSpawn.SetBehaviour(eAIWaypointBehavior.HALT_OR_ALTERNATE);
			aiSpawn.SetFormation("RANDOM");
			aiSpawn.SetSpeed(1.0);
			aiSpawn.SetThreatSpeed(3.0);
			aiSpawn.SetMinAccuracy(0.5);
			aiSpawn.SetMaxAccuracy(0.8);
			aiSpawn.SetCanBeLooted(true);
			aiSpawn.SetUnlimitedReload(true);
			aiSpawn.SetThreatDistanceLimit(400.0);
			aiSpawn.SetDamageMultiplier(1.0);
			aiSpawn.SetDamageReceivedMultiplier(1.0);
			aiSpawn.SetClassNames(ExpansionQuestAISpawn.eAI_UNITS);
			aiSpawn.SetSniperProneDistanceThreshold(300.0);
			aiSpawn.SetRespawnTime(1.0);
			aiSpawn.SetDespawnTime(1.0);
			aiSpawn.SetMinDistanceRadius(50.0);
			aiSpawn.SetMaxDistanceRadius(300.0);
			aiSpawn.SetDespawnRadius(500.0);
			
			objective.AddAISpawn(aiSpawn);
		}

		return objective;
	}

	//! AI VIP OBJECTIVES
	ExpansionQuestObjectiveAIEscortConfig ExpansionQuestObjective_AIEscort_001()
	{
		ExpansionQuestObjectiveAIEscortConfig objective = new ExpansionQuestObjectiveAIEscortConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AIESCORT);
		objective.SetObjectiveText("Bring the Survivor to the marked location.");
		objective.SetTimeLimit(180);

		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Escort Survivor");
		objective.SetNPCClassName(ExpansionQuestAISpawn.eAI_UNITS.GetRandomElement());
		objective.SetCanBeLooted(false);
		objective.SetLoadout("SurvivorLoadout");
		objective.SetNPCName("Survivior");

		if (m_WorldName.IndexOf("namalsk") > -1)
		{
			objective.SetPosition(Vector(8348.39, 0, 10724.7));
		}
		else if (m_WorldName.IndexOf("chernarusplus") > -1)
		{
			objective.SetPosition(Vector(3193.59, 296.707, 6090.57));
		}

		return objective;
	}
#endif

	//! ACTION OBJECTIVES
	ExpansionQuestObjectiveActionConfig ExpansionQuestObjective_Action_001()
	{
		ExpansionQuestObjectiveActionConfig objective = new ExpansionQuestObjectiveActionConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.ACTION);
		objective.AddActionName("ExpansionActionPickVehicleLock");
		objective.SetObjectiveText("Lockpick a vehicle:");
		objective.SetExecutionAmount(1);

		return objective;
	}

	ExpansionQuestObjectiveActionConfig ExpansionQuestObjective_Action_002()
	{
		ExpansionQuestObjectiveActionConfig objective = new ExpansionQuestObjectiveActionConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.ACTION);
		objective.AddActionName("ActionPlantSeed");
		objective.AddAllowedClassName("PumpkinSeeds");
		objective.SetObjectiveText("Plant pumkin seeds:");
		objective.SetExecutionAmount(1);

		return objective;
	}

	//! CRAFTING OBJECTIVES
	ExpansionQuestObjectiveCraftingConfig ExpansionQuestObjective_Crafting_001()
	{
		ExpansionQuestObjectiveCraftingConfig objective = new ExpansionQuestObjectiveCraftingConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.CRAFTING);
		objective.SetObjectiveText("Craft a improvised fishing rod:");
		objective.AddItemName("ImprovisedFishingRod");
		objective.SetExecutionAmount(1);

		return objective;
	}
};