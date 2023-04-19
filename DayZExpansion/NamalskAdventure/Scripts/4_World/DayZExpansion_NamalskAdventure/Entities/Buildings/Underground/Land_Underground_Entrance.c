/**
 * Land_Underground_Entrance.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*modded class Land_Underground_EntranceBase
{
	protected Land_Underground_Entrance m_LinkedFakeEntrance;

	override void Manipulate(Param param = null)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_LinkedFakeEntrance)
			m_LinkedFakeEntrance.Manipulate();

		super.Manipulate(param);
	}

	void SetLinkedFakeEntrance(Land_Underground_Entrance entrance)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_LinkedFakeEntrance = entrance;
	}
};*/

modded class Land_Underground_Panel
{
	protected Land_Underground_Panel m_LinkedFakePanel;
	
	override void Interact()
	{
		super.Interact();
		
		if (m_PanelWasUsed && m_LinkedFakePanel)
		{
			m_LinkedFakePanel.Interact();
		}
	}
	
	void SetLinkedFakePanel(Land_Underground_Panel panel)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_LinkedFakePanel = panel;
	}
};