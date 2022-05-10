#ifndef EXPANSIONMODVEHICLE
CarScript g_Expansion_Car;
#else
modded class CarScript
{
	override bool Expansion_CanObjectAttach(Object obj)
	{
#ifdef EXPANSION_DISABLE_AI_ATTACHMENT
		if (obj.IsInherited(eAIBase))
			return false;
#endif

		return super.Expansion_CanObjectAttach(obj);
	}
}
#endif
