class ExpansionBarrelRollDetector
{
	static const float MIN_ROLL_RATE = 20.0;  //! degrees/sec
	static const float ABORT_TIMEOUT = 0.5;  //! seconds
	static const float MAX_PITCH = 70.0;  //! degrees
	static const float MIN_SPEED = 15.0;  //! m/s (54 km/h)
	static const string FILENAME = "barrelroll";

	Widget m_Comms_Falco;
	Widget m_Comms_Fox;
	Widget m_Comms_Peppy;
	Widget m_Comms_Slippy;

	TextWidget m_Comms_Peppy_Chat;

	float m_LastRoll;
	float m_AccumulatedRoll;
	int m_RollDirection;  //! -1 = left, 1 = right, 0 = not rolling
	float m_TimeSinceRolling;
	bool m_RollCompleted;

	void ExpansionBarrelRollDetector(Widget vehiclePanel)
	{
		m_Comms_Falco = vehiclePanel.FindAnyWidget("Falco");
		m_Comms_Fox = vehiclePanel.FindAnyWidget("Fox");
		m_Comms_Peppy = vehiclePanel.FindAnyWidget("Peppy");
		m_Comms_Slippy = vehiclePanel.FindAnyWidget("Slippy");

		m_Comms_Peppy_Chat = TextWidget.Cast(m_Comms_Peppy.FindAnyWidget("Chat_Peppy_0"));

		Comms_Reset();

		m_RollCompleted = FileExist(EXPANSION_FOLDER + FILENAME);
	}

	void Update(EntityAI heli, ExpansionPhysicsState pState, float dt)
	{
	#ifndef DIAG_DEVELOPER
		if (m_RollCompleted)
			return;
	#endif

		vector ypr = heli.GetYawPitchRoll();

		float roll = ypr[2];

		//! Abort if not fast enough
		if (pState.m_LinearVelocityMS[2] < MIN_SPEED)
		{
			Reset();
			m_LastRoll = roll;
			return;
		}

		float pitch = ypr[1];

		//! Abort if pitch becomes extreme
		if (Math.AbsFloat(pitch) > MAX_PITCH)
		{
			Reset();
			m_LastRoll = roll;
			return;
		}

		float delta = roll - m_LastRoll;

		if (delta > 180)
			delta -= 360;
		else if (delta < -180)
			delta += 360;

		float rollRate = delta / dt;

		//! Abort if not rolling fast enough
		if (Math.AbsFloat(rollRate) < MIN_ROLL_RATE)
		{
			m_TimeSinceRolling += dt;

			if (m_TimeSinceRolling >= ABORT_TIMEOUT)
				Reset();

			m_LastRoll = roll;
			return;
		}

		m_TimeSinceRolling = 0;

		int currentDir = Math.SignFloat(rollRate);

		if (m_RollDirection == 0)
		{
			//! Start new roll
			m_RollDirection = currentDir;
		}
		else if (currentDir != m_RollDirection)
		{
			//! Player reversed direction, reset
			Reset();

			m_RollDirection = currentDir;
		}

		m_AccumulatedRoll += delta;

		if (Math.AbsFloat(m_AccumulatedRoll) >= 340.0)
		{
			OnBarrelRollCompleted(heli);
			Reset();
		}

		m_LastRoll = roll;
	}

	protected void Reset()
	{
		m_AccumulatedRoll = 0;
		m_RollDirection = 0;
		m_TimeSinceRolling = 0;
	}

	void OnBarrelRollCompleted(EntityAI heli)
	{
		m_RollCompleted = true;

		SEffectManager.PlaySoundOnObject("Expansion_SF_StageClear_Short_SoundSet", heli);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Stage0, 1000, false, heli);

		if (FileExist(EXPANSION_FOLDER))
		{
			FileHandle file = OpenFile(EXPANSION_FOLDER + FILENAME, FileMode.APPEND);

			if (file)
				CloseFile(file);
		}
	}

	void Comms_Stage0(EntityAI heli)
	{
		if (!heli)
		{
			Comms_Reset();
			return;
		}

		m_Comms_Peppy_Chat.SetText("BARREL ROLL!!!");
		m_Comms_Peppy.Show(true);

		SEffectManager.PlaySoundOnObject("Expansion_SF_Comms_Peppy_SoundSet", heli);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Stage1, 2000, false, heli);
	}

	void Comms_Stage1(EntityAI heli)
	{
		if (!heli)
		{
			Comms_Reset();
			return;
		}

		m_Comms_Peppy.Show(false);
		m_Comms_Falco.Show(true);

		SEffectManager.PlaySoundOnObject("Expansion_SF_Comms_Falco_SoundSet", heli);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Stage2, 2000, false, heli);
	}

	void Comms_Stage2(EntityAI heli)
	{
		if (!heli)
		{
			Comms_Reset();
			return;
		}

		m_Comms_Falco.Show(false);
		m_Comms_Fox.Show(true);

		SEffectManager.PlaySoundOnObject("Expansion_SF_Comms_Fox_SoundSet", heli);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Stage3, 2000, false, heli);
	}

	void Comms_Stage3(EntityAI heli)
	{
		if (!heli)
		{
			Comms_Reset();
			return;
		}

		m_Comms_Fox.Show(false);
		m_Comms_Peppy_Chat.SetText("...");
		m_Comms_Peppy.Show(true);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Stage4, 1000, false, heli);
	}

	void Comms_Stage4(EntityAI heli)
	{
		if (!heli)
		{
			Comms_Reset();
			return;
		}

		m_Comms_Peppy.Show(false);
		m_Comms_Slippy.Show(true);

		SEffectManager.PlaySoundOnObject("Expansion_SF_Comms_Slippy_SoundSet", heli);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Comms_Reset, 2000);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Achievement_Notif, 3000);
	}

	void Comms_Reset()
	{
		m_Comms_Falco.Show(false);
		m_Comms_Fox.Show(false);
		m_Comms_Peppy.Show(false);
		m_Comms_Slippy.Show(false);
	}

	void Achievement_Notif()
	{
		ExpansionNotification("Achievement: Peppy's Obsession", "Do a barrel roll.", "set:dayz_gui image:icon_Gamepad", ARGB(255, 248, 228, 51), 1.0, ExpansionNotificationType.ACTIVITY).Create();
	}
}
