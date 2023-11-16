string GetRandomAI()
{
	auto trace = EXTrace.Start(EXTrace.AI, null);

	if (Math.RandomIntInclusive(0, 1) == 1)
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
	auto trace = EXTrace.Start(EXTrace.AI, null);

	TStringArray FemaleList = { "SurvivorF_Eva", "SurvivorF_Frida", "SurvivorF_Gabi", "SurvivorF_Helga", 
								"SurvivorF_Irena", "SurvivorF_Judy", "SurvivorF_Keiko", "SurvivorF_Linda", 
								"SurvivorF_Maria", "SurvivorF_Naomi", "SurvivorF_Baty"
							  };
	return "eAI_" + FemaleList.GetRandomElement();
}

string GetRandomAIMale()
{
	auto trace = EXTrace.Start(EXTrace.AI, null);

	TStringArray MaleList = { "SurvivorM_Boris", "SurvivorM_Cyril", "SurvivorM_Denis", "SurvivorM_Elias", 
							  "SurvivorM_Francis", "SurvivorM_Guo", "SurvivorM_Hassan", "SurvivorM_Indar", 
							  "SurvivorM_Jose", "SurvivorM_Kaito", "SurvivorM_Lewis", "SurvivorM_Manua", 
							  "SurvivorM_Mirek", "SurvivorM_Niki", "SurvivorM_Oliver", "SurvivorM_Peter", 
							  "SurvivorM_Quinn", "SurvivorM_Rolf", "SurvivorM_Seth", "SurvivorM_Taiki"
							};
	return "eAI_" + MaleList.GetRandomElement();
}

class eAI_SurvivorM_Mirek: eAIBase {};
class eAI_SurvivorM_Denis: eAIBase {};
class eAI_SurvivorM_Boris: eAIBase {};
class eAI_SurvivorM_Cyril: eAIBase {};
class eAI_SurvivorM_Elias: eAIBase {};
class eAI_SurvivorM_Francis: eAIBase {};
class eAI_SurvivorM_Guo: eAIBase {};
class eAI_SurvivorM_Hassan: eAIBase {};
class eAI_SurvivorM_Indar: eAIBase {};
class eAI_SurvivorM_Jose: eAIBase {};
class eAI_SurvivorM_Kaito: eAIBase {};
class eAI_SurvivorM_Lewis: eAIBase {};
class eAI_SurvivorM_Manua: eAIBase {};
class eAI_SurvivorM_Niki: eAIBase {};
class eAI_SurvivorM_Oliver: eAIBase {};
class eAI_SurvivorM_Peter: eAIBase {};
class eAI_SurvivorM_Quinn: eAIBase {};
class eAI_SurvivorM_Rolf: eAIBase {};
class eAI_SurvivorM_Seth: eAIBase {};
class eAI_SurvivorM_Taiki: eAIBase {};
class eAI_SurvivorF_Linda: eAIBase {};
class eAI_SurvivorF_Maria: eAIBase {};
class eAI_SurvivorF_Frida: eAIBase {};
class eAI_SurvivorF_Gabi: eAIBase {};
class eAI_SurvivorF_Helga: eAIBase {};
class eAI_SurvivorF_Irena: eAIBase {};
class eAI_SurvivorF_Judy: eAIBase {};
class eAI_SurvivorF_Keiko: eAIBase {};
class eAI_SurvivorF_Eva: eAIBase {};
class eAI_SurvivorF_Naomi: eAIBase {};
class eAI_SurvivorF_Baty: eAIBase {};
