/**
 * Animal_LepusEuropaeus.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Animal_LepusEuropaeus extends AnimalBase
{
	override void RegisterHitComponentsForAI()
	{
		m_DefaultHitComponent = "Spine";
		m_DefaultHitPositionComponent = "Spine";
		
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Head", 1);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Head", 5);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Head", 20);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "LArm1", 33);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "RArm1", 33);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Spine", 70);
	}
}