/**
 * ExpansionGuardAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
string GetRandomGuardAI()
{
	auto trace = CF_Trace_0(null, "GetRandomAI");

	if (Math.RandomIntInclusive(0, 1) == 1)
	{
		return GetRandomGuardAIFemale();
	}
	else
	{
		return GetRandomGuardAIMale();
	}
};

string GetRandomGuardAIFemale()
{
	auto trace = CF_Trace_0(null, "GetRandomGuardAIFemale");

	TStringArray FemaleList = { "Eva", "Frida", "Gabi", "Helga",
								"Irena", "Judy", "Keiko", "Linda",
								"Maria", "Naomi", "Baty"
							  };
	return "ExpansionGuardAI" + FemaleList.GetRandomElement();
};

string GetRandomGuardAIMale()
{
	auto trace = CF_Trace_0(null, "GetRandomGuardAIMale");

	TStringArray MaleList = { "Boris", "Cyril", "Denis", "Elias",
							  "Francis", "Guo", "Hassan", "Indar",
							  "Jose", "Kaito", "Lewis", "Manua",
							  "Mirek", "Niki", "Oliver", "Peter",
							  "Quinn", "Rolf", "Seth", "Taiki"
							};
	return "ExpansionGuardAI" + MaleList.GetRandomElement();
}

class ExpansionGuardAI: eAIBase
{
};

class ExpansionGuardAIMirek: ExpansionGuardAI {};
class ExpansionGuardAIDenis: ExpansionGuardAI {};
class ExpansionGuardAIBoris: ExpansionGuardAI {};
class ExpansionGuardAICyril: ExpansionGuardAI {};
class ExpansionGuardAIElias: ExpansionGuardAI {};
class ExpansionGuardAIFrancis: ExpansionGuardAI {};
class ExpansionGuardAIGuo: ExpansionGuardAI {};
class ExpansionGuardAIHassan: ExpansionGuardAI {};
class ExpansionGuardAIIndar: ExpansionGuardAI {};
class ExpansionGuardAIJose: ExpansionGuardAI {};
class ExpansionGuardAIKaito: ExpansionGuardAI {};
class ExpansionGuardAILewis: ExpansionGuardAI {};
class ExpansionGuardAIManua: ExpansionGuardAI {};
class ExpansionGuardAINiki: ExpansionGuardAI {};
class ExpansionGuardAIOliver: ExpansionGuardAI {};
class ExpansionGuardAIPeter: ExpansionGuardAI {};
class ExpansionGuardAIQuinn: ExpansionGuardAI {};
class ExpansionGuardAIRolf: ExpansionGuardAI  {};
class ExpansionGuardAISeth: ExpansionGuardAI {};
class ExpansionGuardAITaiki: ExpansionGuardAI {};
class ExpansionGuardAILinda: ExpansionGuardAI {};
class ExpansionGuardAIMaria: ExpansionGuardAI {};
class ExpansionGuardAIFrida: ExpansionGuardAI {};
class ExpansionGuardAIGabi: ExpansionGuardAI {};
class ExpansionGuardAIHelga: ExpansionGuardAI {};
class ExpansionGuardAIIrena: ExpansionGuardAI {};
class ExpansionGuardAIJudy: ExpansionGuardAI {};
class ExpansionGuardAIKeiko: ExpansionGuardAI {};
class ExpansionGuardAIEva: ExpansionGuardAI {};
class ExpansionGuardAINaomi: ExpansionGuardAI {};
class ExpansionGuardAIBaty: ExpansionGuardAI {};
#endif