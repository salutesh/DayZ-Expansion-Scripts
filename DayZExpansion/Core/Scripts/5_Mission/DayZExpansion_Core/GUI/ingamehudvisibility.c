#ifndef DAYZ_1_26
//! 1.27+
modded class IngameHudVisibility
{
	Widget Expansion_GetElementWidget(EHudElement element)
	{
		return m_ElementMap[element];
	}
}
#endif
