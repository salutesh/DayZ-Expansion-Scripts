class ExpansionInfectedType
{
	private static ref map<typename, ref ExpansionInfectedType> s_InfectedTypes = new map<typename, ref ExpansionInfectedType>;

	private typename m_Type;
	private ref array<ref DayZInfectedAttackType> m_ChaseAttacksGroup = {};
	private ref array<ref DayZInfectedAttackType> m_FightAttacksGroup = {};

	static ExpansionInfectedType Get(typename type)
	{
		ExpansionInfectedType infectedType;

		if (!s_InfectedTypes.Find(type, infectedType))
		{
			infectedType = new ExpansionInfectedType(type);
			s_InfectedTypes[type] = infectedType;
		}

		return infectedType;
	}

	private void ExpansionInfectedType(typename type)
	{
		m_Type = type;

		RegisterAttacks();
	}

	string GetName()
	{
		return m_Type.ToString();
	}

	void RegisterAttacks()
	{
		string name = GetName();
		string lightAttPath = string.Format("cfgVehicles %1 AttackActions AttackShort ammoType", name);
		string heavyAttPath = string.Format("cfgVehicles %1 AttackActions AttackLong ammoType", name);
		string chaseAttPath = string.Format("cfgVehicles %1 AttackActions AttackRun ammoType", name);

		string lightAttAmmo;
		g_Game.ConfigGetText(lightAttPath, lightAttAmmo);
		string heavyAttAmmo;
		g_Game.ConfigGetText(heavyAttPath, heavyAttAmmo);
		string chaseAttAmmo;
		g_Game.ConfigGetText(chaseAttPath, chaseAttAmmo);

		//! Push w/ both hands against obstacle
		//RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.7,  0, 9, 0, lightAttAmmo, 0, 0.4, 1.0);  //! DoorPushAttack

		//! Slam upper body/arms/head against wall, arms raised
		//RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.4,  0, 10, 0, heavyAttAmmo, 0, 0.4, 1.0);  //! DoorHeadAttack

		//! Smash shoulder against obstacle
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.7,  0, 11, 0, lightAttAmmo, 0, 0.4, 1.0);  //! DoorShoulderAttack

		//! Slam fists in alternating fashion (L/R) against wall
		//RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.4,  0, 12, 0, heavyAttAmmo, 0, 0.5, 1.0);  //! DoorLRAttack

		//! Forward kick
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.7,  0, 13, 0, heavyAttAmmo, 0, 0.5, 1.0);  //! DoorKickAttack

		//! Strike in up direction with right hand
		//RegisterAttack(DayZInfectedAttackGroupType.CHASE, 2.4,  1, 14, 0, chaseAttAmmo, 0, 0.4, 1.0);  //! DoorChargeR

		//! Strike in up direction with left hand
		//RegisterAttack(DayZInfectedAttackGroupType.CHASE, 2.4,  1, 15, 0, chaseAttAmmo, 0, 0.4, 1.0);  //! DoorChargeL
	}
	
	DayZInfectedAttackType ChooseAttack(DayZInfectedAttackGroupType pAttackGroupType, float pDistance, int pPitch)
	{
		array<ref DayZInfectedAttackType> attackGroup = GetAttackGroup(pAttackGroupType);
		float rnd;
		float highestUtility = -1;
		DayZInfectedAttackType mostSuitableAttack = null;
		
		Math.Randomize(GetWorldTime() + (int)pDistance);
		
		for (int i = 0; i < attackGroup.Count(); ++i)
		{
			rnd = Math.RandomFloat01();
			DayZInfectedAttackType attackType = attackGroup.Get(i);
			float utility = ComputeAttackUtility(attackType, pDistance, pPitch, rnd);
			if (utility <= 0)
				continue;
			
			if (utility > highestUtility)
			{
				mostSuitableAttack = attackType;
				highestUtility = utility;
			}
		}

		//Print("Most sutable attack selected: " + mostSuitableAttack);
		return mostSuitableAttack;
	}

	protected void RegisterAttack(DayZInfectedAttackGroupType pAttackGroupType, float pDistance, int pPitch, int pType, float pSubtype, string pAmmoType, int pIsHeavy, float pCooldown, float pProbability)
	{
		DayZInfectedAttackType newType = new DayZInfectedAttackType();
		
		newType.m_Distance = pDistance;
		newType.m_Pitch = pPitch;
		newType.m_Type = pType; //If attack is left or right
		newType.m_Subtype = pSubtype; //If attack is center, down, up or run
		newType.m_AmmoType = pAmmoType;
		newType.m_IsHeavy = pIsHeavy;
		newType.m_Cooldown = pCooldown;
		newType.m_Probability = pProbability;
		
		GetAttackGroup(pAttackGroupType).Insert(newType);
	}
	
	protected float ComputeAttackUtility(DayZInfectedAttackType pAttackType, float pTargetDistance, int pPitch, float pProbability)
	{
		// pitch
		if (pAttackType.m_Pitch != pPitch)
			return 0;

		// distance
		float distDiff = pAttackType.m_Distance - pTargetDistance;
		if (distDiff < 0)
			return 0;

		float distDiffFrac = distDiff / 10;
		float utilityDistance = (1 - distDiffFrac) * 100; // distance is most important
		
		// probability
		float probDiff = pAttackType.m_Probability - pProbability;
		if (probDiff < 0)
			return 0;

		float utilityProbability = (1 - probDiff) * 10; // distance is most important
		
		// total
		float util = utilityDistance + utilityProbability;
		//Print("Attack Utility " + util);

		return util;
	}

	protected array<ref DayZInfectedAttackType> GetAttackGroup(DayZInfectedAttackGroupType pType)
	{
		switch (pType)
		{
			case DayZInfectedAttackGroupType.CHASE:
				return m_ChaseAttacksGroup;
				
			case DayZInfectedAttackGroupType.FIGHT:
				return m_FightAttacksGroup;
		}
		
		return null;
	}
}
