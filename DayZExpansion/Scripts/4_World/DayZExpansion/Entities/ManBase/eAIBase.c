#ifdef EXPANSIONMODAI
modded class eAIBase
{
	override bool Expansion_IsGravecrossEnabled()
	{
        if (!m_Expansion_CanBeLooted)
            return false;

        return super.Expansion_IsGravecrossEnabled();
	}
};
#endif