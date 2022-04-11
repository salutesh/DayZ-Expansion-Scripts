/**
 * ContainerWithCargoAndAttachments.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ContainerWithCargoAndAttachments
{
	override void OnShow()
	{
		if (!m_Entity || !m_Entity.IsInherited(ExpansionOwnedContainer) || m_Entity.CanDisplayCargo())
			super.OnShow();
	}

	override void SetOpenState(bool state)
	{
		if (m_Entity && m_Entity.IsInherited(ExpansionOwnedContainer) && !m_Entity.CanDisplayCargo())
			state = false;

		super.SetOpenState(state);
	}

	override void SetEntity(EntityAI entity, bool immedUpdate = true)
	{
		if (entity && entity.IsInherited(ExpansionOwnedContainer) && !entity.CanDisplayCargo())
			m_Closed = true;

		super.SetEntity(entity, immedUpdate);
	}
}
