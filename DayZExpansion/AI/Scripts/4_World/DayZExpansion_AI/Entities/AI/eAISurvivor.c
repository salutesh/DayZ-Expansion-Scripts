string GetRandomAI()
{
	auto trace = CF_Trace_0(null, "GetRandomAI");

	int FemaleChance = 50;
	if (Math.RandomInt(0, 100) > FemaleChance)
	{
		return GetRandomAIFemale();
	}
	else
	{
		return GetRandomAIMale();
	}
}

string GetRandomAIFemale()
{
	auto trace = CF_Trace_0(null, "GetRandomAIFemale");

	TStringArray FemaleList = { "SurvivorF_Eva", "SurvivorF_Frida", "SurvivorF_Gabi", "SurvivorF_Helga", 
								"SurvivorF_Irena", "SurvivorF_Judy", "SurvivorF_Keiko", "SurvivorF_Linda", 
								"SurvivorF_Maria", "SurvivorF_Naomi", "SurvivorF_Baty"
							  };
	return "eAI_" + FemaleList.GetRandomElement();
}

string GetRandomAIMale()
{
	auto trace = CF_Trace_0(null, "GetRandomAIMale");

	TStringArray MaleList = { "SurvivorM_Boris", "SurvivorM_Cyril", "SurvivorM_Denis", "SurvivorM_Elias", 
							  "SurvivorM_Francis", "SurvivorM_Guo", "SurvivorM_Hassan", "SurvivorM_Indar", 
							  "SurvivorM_Jose", "SurvivorM_Kaito", "SurvivorM_Lewis", "SurvivorM_Manua", 
							  "SurvivorM_Mirek", "SurvivorM_Niki", "SurvivorM_Oliver", "SurvivorM_Peter", 
							  "SurvivorM_Quinn", "SurvivorM_Rolf", "SurvivorM_Seth", "SurvivorM_Taiki"
							};
	return "eAI_" + MaleList.GetRandomElement();
}

class eAI_SurvivorM_Mirek extends eAIBase {}
class eAI_SurvivorM_Denis extends eAIBase {}
class eAI_SurvivorM_Boris extends eAIBase {}
class eAI_SurvivorM_Cyril extends eAIBase {}
class eAI_SurvivorM_Elias extends eAIBase {}
class eAI_SurvivorM_Francis extends eAIBase {}
class eAI_SurvivorM_Guo extends eAIBase {}
class eAI_SurvivorM_Hassan extends eAIBase {}
class eAI_SurvivorM_Indar extends eAIBase {}
class eAI_SurvivorM_Jose extends eAIBase {}
class eAI_SurvivorM_Kaito extends eAIBase {}
class eAI_SurvivorM_Lewis extends eAIBase {}
class eAI_SurvivorM_Manua extends eAIBase {}
class eAI_SurvivorM_Niki extends eAIBase {}
class eAI_SurvivorM_Oliver extends eAIBase {}
class eAI_SurvivorM_Peter extends eAIBase {}
class eAI_SurvivorM_Quinn extends eAIBase {}
class eAI_SurvivorM_Rolf extends eAIBase {}
class eAI_SurvivorM_Seth extends eAIBase {}
class eAI_SurvivorM_Taiki extends eAIBase {}
class eAI_SurvivorF_Linda extends eAIBase {}
class eAI_SurvivorF_Maria extends eAIBase {}
class eAI_SurvivorF_Frida extends eAIBase {}
class eAI_SurvivorF_Gabi extends eAIBase {}
class eAI_SurvivorF_Helga extends eAIBase {}
class eAI_SurvivorF_Irena extends eAIBase {}
class eAI_SurvivorF_Judy extends eAIBase {}
class eAI_SurvivorF_Keiko extends eAIBase {}
class eAI_SurvivorF_Eva extends eAIBase {}
class eAI_SurvivorF_Naomi extends eAIBase {}
class eAI_SurvivorF_Baty extends eAIBase {}
