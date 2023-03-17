
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

		if (m_WorldName.Contains("namalsk"))
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
		}
		else if (m_WorldName.Contains("chernarusplus") || m_WorldName == "chernarusplusgloom")
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

		if (m_WorldName.Contains("namalsk"))
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
		}
		else if (m_WorldName.Contains("chernarusplus") || m_WorldName == "chernarusplusgloom")
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

		if (m_WorldName.Contains("namalsk"))
		{
			objective.SetPosition(Vector(8584.27, 14.73, 10511.6));
		}
		else if (m_WorldName.Contains("chernarusplus") || m_WorldName == "chernarusplusgloom")
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

		if (m_WorldName.Contains("namalsk"))
		{
			objective.SetPosition(Vector(6573.94, 20.2864, 11832.3));
		}
		else if (m_WorldName.Contains("chernarusplus") || m_WorldName == "chernarusplusgloom")
		{
			objective.SetPosition(Vector(6990.52, 383.992, 11390.1));
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

		if (m_WorldName == "namalsk")
		{
			objective.SetPosition(Vector(8224.93, 0, 10811.0));
			objective.SetMaxDistance(150.0);
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			objective.SetPosition(Vector(2596.7, 306.1, 6378.47));
			objective.SetMaxDistance(150.0);
		}

		ExpansionQuestObjectiveTarget target = new ExpansionQuestObjectiveTarget();
		//! Civilian Infected
		target.AddClassName("ZmbM_CitizenASkinny_Base");
		target.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		target.AddClassName("ZmbM_CitizenBFat_Base");
		target.AddClassName("ZmbM_ClerkFat_Base");
		target.AddClassName("ZmbM_ClerkFat_LT_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		target.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		target.AddClassName("ZmbM_DoctorFat_Base");
		target.AddClassName("ZmbM_FarmerFat_Base");
		target.AddClassName("ZmbM_FarmerFat_LT_Base");
		target.AddClassName("ZmbM_FirefighterNormal_Base");
		target.AddClassName("ZmbM_FishermanOld_Base");
		target.AddClassName("ZmbM_HandymanNormal_Base");
		target.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		target.AddClassName("ZmbM_HermitSkinny_Base");
		target.AddClassName("ZmbM_HikerSkinny_Base");
		target.AddClassName("ZmbM_HunterOld_Base");
		target.AddClassName("ZmbM_Jacket_Base");
		target.AddClassName("ZmbM_Jacket_LT_Base");
		target.AddClassName("ZmbM_JoggerSkinny_Base");
		target.AddClassName("ZmbM_Runner_Base");
		target.AddClassName("ZmbM_JournalistSkinny_Base");
		target.AddClassName("ZmbM_MechanicSkinny_Base");
		target.AddClassName("ZmbM_MotobikerFat_Base");
		target.AddClassName("ZmbM_OffshoreWorker_Base");
		target.AddClassName("ZmbM_ParamedicNormal_Base");
		target.AddClassName("ZmbM_PatientSkinny_Base");
		target.AddClassName("ZmbM_PolicemanFat_Base");
		target.AddClassName("ZmbM_PolicemanSpecForce_Base");
		target.AddClassName("ZmbM_priestPopSkinny_Base");
		target.AddClassName("ZmbM_PrisonerSkinny_Base");
		target.AddClassName("ZmbM_SkaterYoung_Base");
		target.AddClassName("ZmbM_SkaterYoung_LT_Base");
		target.AddClassName("ZmbM_SurvivorDean_Base");
		target.AddClassName("ZmbM_VillagerOld_Base");
		target.AddClassName("ZmbM_VillagerOld_LT_Base");
		target.AddClassName("ZmbF_BlueCollarFat_Base");
		target.AddClassName("ZmbF_CitizenANormal_Base");
		target.AddClassName("ZmbF_CitizenANormal_LT_Base");
		target.AddClassName("ZmbF_CitizenBSkinny_Base");
		target.AddClassName("ZmbF_Clerk_Normal_Base");
		target.AddClassName("ZmbF_ClerkFat_Base");
		target.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		target.AddClassName("ZmbF_DoctorSkinny_Base");
		target.AddClassName("ZmbF_HikerSkinny_Base");
		target.AddClassName("ZmbF_JoggerSkinny_Base");
		target.AddClassName("ZmbF_Runner_Base");
		target.AddClassName("ZmbF_JournalistNormal_Base");
		target.AddClassName("ZmbF_JournalistNormal_LT_Base");
		target.AddClassName("ZmbF_MechanicNormal_Base");
		target.AddClassName("ZmbF_MilkMaidOld_Base");
		target.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		target.AddClassName("ZmbF_NurseFat_Base");
		target.AddClassName("ZmbF_ParamedicNormal_Base");
		target.AddClassName("ZmbF_PatientOld_Base");
		target.AddClassName("ZmbF_PoliceWomanNormal_Base");
		target.AddClassName("ZmbF_ShortSkirt_Base");
		target.AddClassName("ZmbF_ShortSkirt_LT_Base");
		target.AddClassName("ZmbF_SkaterYoung_Base");
		target.AddClassName("ZmbF_SkaterYoung_LT_Base");
		target.AddClassName("ZmbF_SurvivorNormal_Base");
		target.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		target.AddClassName("ZmbF_VillagerOld_Base");
		target.AddClassName("ZmbF_VillagerOld_LT_Base");

		target.SetAmount(10);
		target.SetNeedSpecialWeapon(true);
		target.AddAllowedWeapon("SledgeHammer");

		objective.SetTarget(target);

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_002()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 100 Infected");
		objective.SetMaxDistance(150.0);

		ExpansionQuestObjectiveTarget target = new ExpansionQuestObjectiveTarget();
		//! Civilian Infected
		target.AddClassName("ZmbM_CitizenASkinny_Base");
		target.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		target.AddClassName("ZmbM_CitizenBFat_Base");
		target.AddClassName("ZmbM_ClerkFat_Base");
		target.AddClassName("ZmbM_ClerkFat_LT_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		target.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		target.AddClassName("ZmbM_DoctorFat_Base");
		target.AddClassName("ZmbM_FarmerFat_Base");
		target.AddClassName("ZmbM_FarmerFat_LT_Base");
		target.AddClassName("ZmbM_FirefighterNormal_Base");
		target.AddClassName("ZmbM_FishermanOld_Base");
		target.AddClassName("ZmbM_HandymanNormal_Base");
		target.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		target.AddClassName("ZmbM_HermitSkinny_Base");
		target.AddClassName("ZmbM_HikerSkinny_Base");
		target.AddClassName("ZmbM_HunterOld_Base");
		target.AddClassName("ZmbM_Jacket_Base");
		target.AddClassName("ZmbM_Jacket_LT_Base");
		target.AddClassName("ZmbM_JoggerSkinny_Base");
		target.AddClassName("ZmbM__Runner_Base");
		target.AddClassName("ZmbM_JournalistSkinny_Base");
		target.AddClassName("ZmbM_MechanicSkinny_Base");
		target.AddClassName("ZmbM_MotobikerFat_Base");
		target.AddClassName("ZmbM_OffshoreWorker_Base");
		target.AddClassName("ZmbM_ParamedicNormal_Base");
		target.AddClassName("ZmbM_PatientSkinny_Base");
		target.AddClassName("ZmbM_PolicemanFat_Base");
		target.AddClassName("ZmbM_PolicemanSpecForce_Base");
		target.AddClassName("ZmbM_priestPopSkinny_Base");
		target.AddClassName("ZmbM_PrisonerSkinny_Base");
		target.AddClassName("ZmbM_SkaterYoung_Base");
		target.AddClassName("ZmbM_SkaterYoung_LT_Base");
		target.AddClassName("ZmbM_SurvivorDean_Base");
		target.AddClassName("ZmbM_VillagerOld_Base");
		target.AddClassName("ZmbM_VillagerOld_LT_Base");
		target.AddClassName("ZmbF_BlueCollarFat_Base");
		target.AddClassName("ZmbF_CitizenANormal_Base");
		target.AddClassName("ZmbF_CitizenANormal_LT_Base");
		target.AddClassName("ZmbF_CitizenBSkinny_Base");
		target.AddClassName("ZmbF_Clerk_Normal_Base");
		target.AddClassName("ZmbF_ClerkFat_Base");
		target.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		target.AddClassName("ZmbF_DoctorSkinny_Base");
		target.AddClassName("ZmbF_HikerSkinny_Base");
		target.AddClassName("ZmbF_JoggerSkinny_Base");
		target.AddClassName("ZmbF_Runner_Base");
		target.AddClassName("ZmbF_JournalistNormal_Base");
		target.AddClassName("ZmbF_JournalistNormal_LT_Base");
		target.AddClassName("ZmbF_MechanicNormal_Base");
		target.AddClassName("ZmbF_MilkMaidOld_Base");
		target.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		target.AddClassName("ZmbF_NurseFat_Base");
		target.AddClassName("ZmbF_ParamedicNormal_Base");
		target.AddClassName("ZmbF_PatientOld_Base");
		target.AddClassName("ZmbF_PoliceWomanNormal_Base");
		target.AddClassName("ZmbF_ShortSkirt_Base");
		target.AddClassName("ZmbF_ShortSkirt_LT_Base");
		target.AddClassName("ZmbF_SkaterYoung_Base");
		target.AddClassName("ZmbF_SkaterYoung_LT_Base");
		target.AddClassName("ZmbF_SurvivorNormal_Base");
		target.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		target.AddClassName("ZmbF_VillagerOld_Base");
		target.AddClassName("ZmbF_VillagerOld_LT_Base");

		target.SetAmount(100);

		objective.SetTarget(target);

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_003()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(3);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 10 Infected");
		objective.SetMaxDistance(150.0);

		ExpansionQuestObjectiveTarget target = new ExpansionQuestObjectiveTarget();
		//! Civilian Infected
		target.AddClassName("ZmbM_CitizenASkinny_Base");
		target.AddClassName("ZmbM_CitizenASkinny_LT_Base");
		target.AddClassName("ZmbM_CitizenBFat_Base");
		target.AddClassName("ZmbM_ClerkFat_Base");
		target.AddClassName("ZmbM_ClerkFat_LT_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_Base");
		target.AddClassName("ZmbM_CommercialPilotOld_LT_Base");
		target.AddClassName("ZmbM_ConstrWorkerNormal_Base");
		target.AddClassName("ZmbM_DoctorFat_Base");
		target.AddClassName("ZmbM_FarmerFat_Base");
		target.AddClassName("ZmbM_FarmerFat_LT_Base");
		target.AddClassName("ZmbM_FirefighterNormal_Base");
		target.AddClassName("ZmbM_FishermanOld_Base");
		target.AddClassName("ZmbM_HandymanNormal_Base");
		target.AddClassName("ZmbM_HeavyIndustryWorker_Base");
		target.AddClassName("ZmbM_HermitSkinny_Base");
		target.AddClassName("ZmbM_HikerSkinny_Base");
		target.AddClassName("ZmbM_HunterOld_Base");
		target.AddClassName("ZmbM_Jacket_Base");
		target.AddClassName("ZmbM_Jacket_LT_Base");
		target.AddClassName("ZmbM_JoggerSkinny_Base");
		target.AddClassName("ZmbM__Runner_Base");
		target.AddClassName("ZmbM_JournalistSkinny_Base");
		target.AddClassName("ZmbM_MechanicSkinny_Base");
		target.AddClassName("ZmbM_MotobikerFat_Base");
		target.AddClassName("ZmbM_OffshoreWorker_Base");
		target.AddClassName("ZmbM_ParamedicNormal_Base");
		target.AddClassName("ZmbM_PatientSkinny_Base");
		target.AddClassName("ZmbM_PolicemanFat_Base");
		target.AddClassName("ZmbM_PolicemanSpecForce_Base");
		target.AddClassName("ZmbM_priestPopSkinny_Base");
		target.AddClassName("ZmbM_PrisonerSkinny_Base");
		target.AddClassName("ZmbM_SkaterYoung_Base");
		target.AddClassName("ZmbM_SkaterYoung_LT_Base");
		target.AddClassName("ZmbM_SurvivorDean_Base");
		target.AddClassName("ZmbM_VillagerOld_Base");
		target.AddClassName("ZmbM_VillagerOld_LT_Base");
		target.AddClassName("ZmbF_BlueCollarFat_Base");
		target.AddClassName("ZmbF_CitizenANormal_Base");
		target.AddClassName("ZmbF_CitizenANormal_LT_Base");
		target.AddClassName("ZmbF_CitizenBSkinny_Base");
		target.AddClassName("ZmbF_Clerk_Normal_Base");
		target.AddClassName("ZmbF_ClerkFat_Base");
		target.AddClassName("ZmbF_Clerk_Normal_LT_Base");
		target.AddClassName("ZmbF_DoctorSkinny_Base");
		target.AddClassName("ZmbF_HikerSkinny_Base");
		target.AddClassName("ZmbF_JoggerSkinny_Base");
		target.AddClassName("ZmbF_Runner_Base");
		target.AddClassName("ZmbF_JournalistNormal_Base");
		target.AddClassName("ZmbF_JournalistNormal_LT_Base");
		target.AddClassName("ZmbF_MechanicNormal_Base");
		target.AddClassName("ZmbF_MilkMaidOld_Base");
		target.AddClassName("ZmbF_MilkMaidOld_LT_Base");
		target.AddClassName("ZmbF_NurseFat_Base");
		target.AddClassName("ZmbF_ParamedicNormal_Base");
		target.AddClassName("ZmbF_PatientOld_Base");
		target.AddClassName("ZmbF_PoliceWomanNormal_Base");
		target.AddClassName("ZmbF_ShortSkirt_Base");
		target.AddClassName("ZmbF_ShortSkirt_LT_Base");
		target.AddClassName("ZmbF_SkaterYoung_Base");
		target.AddClassName("ZmbF_SkaterYoung_LT_Base");
		target.AddClassName("ZmbF_SurvivorNormal_Base");
		target.AddClassName("ZmbF_SurvivorNormal_LT_Base");
		target.AddClassName("ZmbF_VillagerOld_Base");
		target.AddClassName("ZmbF_VillagerOld_LT_Base");

		//! Military Infected
		target.AddClassName("ZmbM_PatrolNormal_Base");
		target.AddClassName("ZmbM_Soldier_Base");
		target.AddClassName("ZmbM_SoldierNormal_Base");
		target.AddClassName("ZmbM_usSoldier_normal_Base");

		//! Imune Infected
		target.AddClassName("ZmbM_NBC_Yellow");

		//! Imune Military Infected
		target.AddClassName("ZmbM_NBC_Grey");

		target.SetAmount(10);

		objective.SetTarget(target);

		return objective;
	}

	ExpansionQuestObjectiveTargetConfig ExpansionQuestObjective_Target_004()
	{
		ExpansionQuestObjectiveTargetConfig objective = new ExpansionQuestObjectiveTargetConfig();
		objective.SetID(4);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		objective.SetObjectiveText("Kill 10 Military/NPC Infected");

		ExpansionQuestObjectiveTarget target = new ExpansionQuestObjectiveTarget();
		//! Military Infected
		target.AddClassName("ZmbM_PatrolNormal_Base");
		target.AddClassName("ZmbM_Soldier_Base");
		target.AddClassName("ZmbM_SoldierNormal_Base");
		target.AddClassName("ZmbM_usSoldier_normal_Base");

		//! Imune Infected
		target.AddClassName("ZmbM_NBC_Yellow");

		//! Imune Military Infected
		target.AddClassName("ZmbM_NBC_Grey");

		target.SetAmount(10);

		objective.SetTarget(target);

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
		objective.SetMaxDistance(10.0);
		objective.SetMarkerName("Deliver Items");

		return objective;
	}

	ExpansionQuestObjectiveCollectionConfig ExpansionQuestObjective_Collection_002()
	{
		ExpansionQuestObjectiveCollectionConfig objective = new ExpansionQuestObjectiveCollectionConfig();
		objective.SetID(2);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		objective.SetObjectiveText("Catch a mackerel");
		objective.AddCollection(1, "Mackerel");
		objective.SetMaxDistance(10.0);
		objective.SetMarkerName("Deliver Items");

		return objective;
	}

	ExpansionQuestObjectiveCollectionConfig ExpansionQuestObjective_Collection_003()
	{
		ExpansionQuestObjectiveCollectionConfig objective = new ExpansionQuestObjectiveCollectionConfig();
		objective.SetID(3);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		objective.SetObjectiveText("Get the yellow king's helmet");
		objective.AddCollection(1, "DarkMotoHelmet_YellowScarred");
		objective.SetMaxDistance(10.0);
		objective.SetMarkerName("Deliver Item");

		return objective;
	}

	//! TREASURE HUNT OBJECTIVES
	ExpansionQuestObjectiveTreasureHuntConfig ExpansionQuestObjective_TreasureHunt_001()
	{
		ExpansionQuestObjectiveTreasureHuntConfig objective = new ExpansionQuestObjectiveTreasureHuntConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.TREASUREHUNT);
		objective.SetObjectiveText("Find the location of the treasure");

		if (m_WorldName == "namalsk")
		{
			objective.AddPosition(Vector(8541.46, 15.3878, 10299.1));
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			objective.AddPosition(Vector(2936.48, 350.404, 6369.39));
			objective.AddPosition(Vector(3143.35, 365.776, 6942.04));
			objective.AddPosition(Vector(5233.51, 290.881, 6246.37));
		}

		return objective;
	}

#ifdef EXPANSIONMODAI
	//! AI PATROL OBJECTIVES
	ExpansionQuestObjectiveAIPatrolConfig ExpansionQuestObjective_AIPatrol_001()
	{
		ExpansionQuestObjectiveAIPatrolConfig objective = new ExpansionQuestObjectiveAIPatrolConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AIPATROL);
		objective.SetObjectiveText("Kill all 4 units of the bandit patrol at the marked location");
		objective.SetCanLootAI(true);

		ExpansionQuestObjectiveAIPatrol aiPatrol = new ExpansionQuestObjectiveAIPatrol();
		aiPatrol.SetNPCUnits(4);
		aiPatrol.SetNPCSpeed("WALK");
		aiPatrol.SetNPCMode("REVERSE");
		aiPatrol.SetNPCFaction("West");
		aiPatrol.NPCFormation = "RANDOM";
		aiPatrol.SetNPCLoadoutFile("BanditLoadout.json");

		if (m_WorldName == "namalsk")
		{
			aiPatrol.AddWaypoint(Vector(6307.7, 14.4, 11810.6));
			aiPatrol.AddWaypoint(Vector(6519.9, 17.3, 11901.6));
			aiPatrol.AddWaypoint(Vector(6967.4, 6.5, 11884.6));
			aiPatrol.AddWaypoint(Vector(7216.6, 10.147, 11843.1));
			aiPatrol.AddWaypoint(Vector(7405.78, 23.18, 11655.8));
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			aiPatrol.AddWaypoint(Vector(6914.7, 403.027, 11381.7));
			aiPatrol.AddWaypoint(Vector(6931.27, 399.86, 11456.7));
			aiPatrol.AddWaypoint(Vector(6891.92, 397.455, 11496.1));
			aiPatrol.AddWaypoint(Vector(6850.88, 399.147, 11468.1));
			aiPatrol.AddWaypoint(Vector(6883.98, 403.192, 11380.8));
		}

		aiPatrol.AddClassName("eAI_SurvivorM_Mirek");
		aiPatrol.AddClassName("eAI_SurvivorM_Denis");
		aiPatrol.AddClassName("eAI_SurvivorM_Boris");
		aiPatrol.AddClassName("eAI_SurvivorM_Cyril");
		aiPatrol.AddClassName("eAI_SurvivorM_Elias");
		aiPatrol.AddClassName("eAI_SurvivorM_Francis");
		aiPatrol.AddClassName("eAI_SurvivorM_Guo");
		aiPatrol.AddClassName("eAI_SurvivorM_Hassan");
		aiPatrol.AddClassName("eAI_SurvivorM_Indar");
		aiPatrol.AddClassName("eAI_SurvivorM_Jose");
		aiPatrol.AddClassName("eAI_SurvivorM_Kaito");
		aiPatrol.AddClassName("eAI_SurvivorM_Lewis");
		aiPatrol.AddClassName("eAI_SurvivorM_Manua");
		aiPatrol.AddClassName("eAI_SurvivorM_Niki");
		aiPatrol.AddClassName("eAI_SurvivorM_Oliver");
		aiPatrol.AddClassName("eAI_SurvivorM_Peter");
		aiPatrol.AddClassName("eAI_SurvivorM_Quinn");
		aiPatrol.AddClassName("eAI_SurvivorM_Rolf");
		aiPatrol.AddClassName("eAI_SurvivorM_Seth");
		aiPatrol.AddClassName("eAI_SurvivorM_Taiki");
		aiPatrol.AddClassName("eAI_SurvivorF_Linda");
		aiPatrol.AddClassName("eAI_SurvivorF_Maria");
		aiPatrol.AddClassName("eAI_SurvivorF_Frida");
		aiPatrol.AddClassName("eAI_SurvivorF_Gabi");
		aiPatrol.AddClassName("eAI_SurvivorF_Helga");
		aiPatrol.AddClassName("eAI_SurvivorF_Irena");
		aiPatrol.AddClassName("eAI_SurvivorF_Judy");
		aiPatrol.AddClassName("eAI_SurvivorF_Keiko");
		aiPatrol.AddClassName("eAI_SurvivorF_Eva");
		aiPatrol.AddClassName("eAI_SurvivorF_Naomi");
		aiPatrol.AddClassName("eAI_SurvivorF_Baty");

		objective.SetAIPatrol(aiPatrol);

		return objective;
	}

	//! AI CAMP OBJECTIVES
	ExpansionQuestObjectiveAICampConfig ExpansionQuestObjective_AICamp_001()
	{
		ExpansionQuestObjectiveAICampConfig objective = new ExpansionQuestObjectiveAICampConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AICAMP);
		objective.SetObjectiveText("Kill all 10 bandits at the marked location");
		objective.SetCanLootAI(true);

		ExpansionQuestObjectiveAICamp aiCamp = new ExpansionQuestObjectiveAICamp();
		aiCamp.SetNPCSpeed("RUN");
		aiCamp.SetNPCMode("HALT");
		aiCamp.SetNPCFaction("West");
		aiCamp.SetNPCLoadoutFile("BanditLoadout.json");

		if (m_WorldName == "namalsk")
		{
			aiCamp.AddPosition(Vector(8091.23, 15.37, 10832.4));
			aiCamp.AddPosition(Vector(8095.07, 15.62, 10865.4));
			aiCamp.AddPosition(Vector(8121.96, 16.09, 10850.6));
			aiCamp.AddPosition(Vector(8124.99, 20.46, 10852.5));
			aiCamp.AddPosition(Vector(8082.32, 15.37, 10838.6));
			aiCamp.AddPosition(Vector(8123.08, 15.30, 10819.7));
			aiCamp.AddPosition(Vector(8151.75, 15.45, 10815.7));
			aiCamp.AddPosition(Vector(8165.89, 15.81, 10840.5));
			aiCamp.AddPosition(Vector(8172.00, 15.90, 10886.2));
			aiCamp.AddPosition(Vector(8117.52, 15.23, 10864.8));
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			aiCamp.AddPosition(Vector(4351.33, 307.068, 6400.52));
			aiCamp.AddPosition(Vector(4389.03, 302.802, 6391.77));
			aiCamp.AddPosition(Vector(4416.67, 301.977, 6395.7));
			aiCamp.AddPosition(Vector(4431.82, 298.901, 6452.22));
			aiCamp.AddPosition(Vector(4418.52, 299.6, 6441.92));
			aiCamp.AddPosition(Vector(4475.16, 295.22, 6461.57));
			aiCamp.AddPosition(Vector(4485.01, 295.22, 6466.97));
			aiCamp.AddPosition(Vector(4435.54, 297.747, 6470.3));
			aiCamp.AddPosition(Vector(4556.3, 289.275, 6390.84));
			aiCamp.AddPosition(Vector(4523.01, 290.427, 6448.3));
		}

		aiCamp.AddClassName("eAI_SurvivorM_Mirek");
		aiCamp.AddClassName("eAI_SurvivorM_Denis");
		aiCamp.AddClassName("eAI_SurvivorM_Boris");
		aiCamp.AddClassName("eAI_SurvivorM_Cyril");
		aiCamp.AddClassName("eAI_SurvivorM_Elias");
		aiCamp.AddClassName("eAI_SurvivorM_Francis");
		aiCamp.AddClassName("eAI_SurvivorM_Guo");
		aiCamp.AddClassName("eAI_SurvivorM_Hassan");
		aiCamp.AddClassName("eAI_SurvivorM_Indar");
		aiCamp.AddClassName("eAI_SurvivorM_Jose");
		aiCamp.AddClassName("eAI_SurvivorM_Kaito");
		aiCamp.AddClassName("eAI_SurvivorM_Lewis");
		aiCamp.AddClassName("eAI_SurvivorM_Manua");
		aiCamp.AddClassName("eAI_SurvivorM_Niki");
		aiCamp.AddClassName("eAI_SurvivorM_Oliver");
		aiCamp.AddClassName("eAI_SurvivorM_Peter");
		aiCamp.AddClassName("eAI_SurvivorM_Quinn");
		aiCamp.AddClassName("eAI_SurvivorM_Rolf");
		aiCamp.AddClassName("eAI_SurvivorM_Seth");
		aiCamp.AddClassName("eAI_SurvivorM_Taiki");
		aiCamp.AddClassName("eAI_SurvivorF_Linda");
		aiCamp.AddClassName("eAI_SurvivorF_Maria");
		aiCamp.AddClassName("eAI_SurvivorF_Frida");
		aiCamp.AddClassName("eAI_SurvivorF_Gabi");
		aiCamp.AddClassName("eAI_SurvivorF_Helga");
		aiCamp.AddClassName("eAI_SurvivorF_Irena");
		aiCamp.AddClassName("eAI_SurvivorF_Judy");
		aiCamp.AddClassName("eAI_SurvivorF_Keiko");
		aiCamp.AddClassName("eAI_SurvivorF_Eva");
		aiCamp.AddClassName("eAI_SurvivorF_Naomi");
		aiCamp.AddClassName("eAI_SurvivorF_Baty");

		objective.SetAICamp(aiCamp);

		return objective;
	}

	//! AI VIP OBJECTIVES
	ExpansionQuestObjectiveAIEscortConfig ExpansionQuestObjective_AIEscort_001()
	{
		ExpansionQuestObjectiveAIEscortConfig objective = new ExpansionQuestObjectiveAIEscortConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.AIESCORT);
		objective.SetObjectiveText("Bring the VIP to the marked location.");
		objective.SetTimeLimit(180);
		objective.SetMaxDistance(20.0);
		objective.SetMarkerName("Escort VIP");

		if (m_WorldName == "namalsk")
		{
			objective.SetPosition(Vector(8348.39, 0, 10724.7));
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			objective.SetPosition(Vector(3193.59, 296.707, 6090.57));
		}

		ExpansionQuestObjectiveAIVIP aiVIP = new ExpansionQuestObjectiveAIVIP();
		aiVIP.SetNPCLoadoutFile("BanditLoadout.json");

		objective.SetAIVIP(aiVIP);

		return objective;
	}
#endif

	//! ACTION OBJECTIVES
	ExpansionQuestObjectiveActionConfig ExpansionQuestObjective_Action_001()
	{
		ExpansionQuestObjectiveActionConfig objective = new ExpansionQuestObjectiveActionConfig();
		objective.SetID(1);
		objective.SetObjectiveType(ExpansionQuestObjectiveType.ACTION);
		objective.AddActionName("ExpansionVehicleActionPickLock");
		objective.AddActionName("ExpansionActionPickVehicleLockBase");
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