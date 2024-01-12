class eAISurvivor
{
	static ref ExpansionArray<string> s_Females = new ExpansionArray<string>;
	static ref ExpansionArray<string> s_Males = new ExpansionArray<string>;

	static bool s_Initialized = Init();

	static bool Init()
	{
		TStringArray females = {"Eva", "Frida", "Gabi", "Helga", 
								"Irena", "Judy", "Keiko", "Linda", 
								"Maria", "Naomi", "Baty"};

		s_Females.InsertAll(females);

		TStringArray males = {"Boris", "Cyril", "Denis", "Elias", 
							  "Francis", "Guo", "Hassan", "Indar", 
							  "Jose", "Kaito", "Lewis", "Manua", 
							  "Mirek", "Niki", "Oliver", "Peter", 
							  "Quinn", "Rolf", "Seth", "Taiki"};

		s_Males.InsertAll(males);

		return true;
	}

	static string GetQuasiRandom()
	{
		if (Math.RandomIntInclusive(0, 1) == 1)
			return GetQuasiRandomFemale();
		else
			return GetQuasiRandomMale();
	}

	static string GetQuasiRandomFemale()
	{
		return "eAI_SurvivorF_" + s_Females.GetQuasiRandomElementAvoidRepetition();
	}

	static string GetQuasiRandomMale()
	{
		return "eAI_SurvivorM_" + s_Males.GetQuasiRandomElementAvoidRepetition();
	}

	static string GetRandom()
	{
		if (Math.RandomIntInclusive(0, 1) == 1)
			return GetRandomFemale();
		else
			return GetRandomMale();
	}

	static string GetRandomFemale()
	{
		return "eAI_SurvivorF_" + s_Females.GetRandomElement();
	}

	static string GetRandomMale()
	{
		return "eAI_SurvivorM_" + s_Males.GetRandomElement();
	}
}

string GetRandomAI()
{
	return eAISurvivor.GetQuasiRandom();
}

string GetRandomAIFemale()
{
	return eAISurvivor.GetQuasiRandomFemale();
}

string GetRandomAIMale()
{
	return eAISurvivor.GetQuasiRandomMale();
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
