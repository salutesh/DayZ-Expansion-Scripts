modded class ModifierBase
{
	//! Only allow modifiers that do not affect health/blood/energy/water negatively for AI if dmg is disabled
	override void Tick(float delta_time)
	{
		if (m_Player.IsAI())
		{
			typename type = Type();
			switch (type)
			{
				case BloodRegenMdfr:
				case SalineMdfr:
				case HealthRegenMdfr:
				case StomachMdfr:
				case ImmunityBoost:
				case AntibioticsMdfr:
				case BreathVapourMdfr:
				case ShockMdfr:
				case CharcoalMdfr:
				case MorphineMdfr:
				case PainKillersMdfr:
				case EpinephrineMdfr:
				case DisinfectionMdfr:
				case FatigueMdfr:
				case MaskMdfr:
				case FliesMdfr:
					break;
				case HeatComfortMdfr:
				case WoundInfectStage1Mdfr:
				case WoundInfectStage2Mdfr:
				case HungerMdfr:
				case ThirstMdfr:
					return;  //! Maybe re-enable hunger/thirst when we teach AI to eat & drink?
				default:
					if (!m_Player.Expansion_CanBeDamaged())
						return;
			}
		}

		super.Tick(delta_time);
	}
}
