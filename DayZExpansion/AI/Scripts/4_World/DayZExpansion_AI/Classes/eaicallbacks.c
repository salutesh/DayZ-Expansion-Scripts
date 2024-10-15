/**
 * @brief callbacks to use by (e.g.) 3rd party mods. Don't forget to call super when overriding any of these!
 * 
 * You can access the respective AI via m_AI if needed, but this should be done sparingly.
 */
class eAICallbacks
{
	eAIBase m_AI;

	void eAICallbacks(eAIBase ai)
	{
		m_AI = ai;
	}

	/**
	 * @brief called when AI is targeting a target
	 */
	void OnTargetSelected(eAITarget target)
	{
	}

	/**
	 * @brief called when AI has no more targets after the last target has been removed
	 */
	void OnNoMoreTargets()
	{
	}

	/**
	 * @brief called when AI has LOS to current target
	 */
	void OnLOS(eAITarget target)
	{
	}

	/**
	 * @brief called when AI lost LOS to current target
	 */
	void OnLOSLost(eAITarget target)
	{
	}

	/**
	 * @brief called when threat level to AI changes (not taking into account line of sight, i.e. level will be the same whether AI has LOS or not)
	 */
	void OnThreatLevelChanged(float oldLevel, float newLevel)
	{
	}

	/**
	 * @brief called when active threat level to AI changes (taking into account line of sight, i.e. without LOS, level will be lower)
	 */
	void OnActiveThreatLevelChanged(float oldLevel, float newLevel)
	{
	}

	/**
	 * @brief called when AI gets behind cover
	 */
	void OnEnterCover(Object coverObj)
	{
	}

	/**
	 * @brief called when AI leaves cover
	 */
	void OnLeaveCover(Object coverObj)
	{
	}
}
