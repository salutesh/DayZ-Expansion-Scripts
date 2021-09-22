class ExpansionVehicleSoundShader
{
	private ref Expression m_Volume;
	private ref Expression m_Frequency;

	void ExpansionVehicleSoundShader(string name)
	{
		string path;
		string expression;

		expression = "0";
		path = "CfgSoundShaders " + name + " ex_volume";
		if (GetGame().ConfigIsExisting(path))
		{
			expression = GetGame().ConfigGetTextOut(path);
		}

#ifdef CF_EXPRESSION
		Class.CastTo(m_Volume, CF_ExpressionVM.Compile(expression, ExpansionVehicleSoundManager.s_SoundShaderParameters, Expression));
#endif

		expression = "0";
		path = "CfgSoundShaders " + name + " ex_frequency";
		if (GetGame().ConfigIsExisting(path))
		{
			expression = GetGame().ConfigGetTextOut(path);
		}

#ifdef CF_EXPRESSION
		Class.CastTo(m_Frequency, CF_ExpressionVM.Compile(expression, ExpansionVehicleSoundManager.s_SoundShaderParameters, Expression));
#endif
	}

	void Calculate(array<float> variables, inout float volume, inout float frequency)
	{
#ifdef CF_EXPRESSION
		frequency = m_Frequency.Evaluate(variables);

		if (frequency < 0.5 || frequency > 1.75)
		{
			volume = 0.0;
		}
		else
		{
			volume = m_Volume.Evaluate(variables);
		}
#endif
	}
};

class ExpansionVehicleSoundManager
{
	static ref array<string> s_SoundShaderParameters = {
		"rpm",
		"engineOn",
		"campos",
		"doors",
		"speed",
		"thrust",
	};

	static ref map<string, ref ExpansionVehicleSoundShader> s_SoundShaders = new map<string, ref ExpansionVehicleSoundShader>();

	static void Get(string name, out ExpansionVehicleSoundShader shader)
	{
		if (s_SoundShaders.Find(name, shader))
			return;

		shader = new ExpansionVehicleSoundShader(name);
		s_SoundShaders[name] = shader;
	}
};

class ExpansionVehicleSound
{
	private ExpansionVehicleBase m_Vehicle;

	private bool m_HasVolume = true;
	private bool m_HasFrequency = true;

	private ref ExpansionVehicleSoundShader m_Shader;

	private float m_TargetVolume;
	private float m_TargetFrequency;

	private float m_CurrentVolume;
	private float m_CurrentFrequency;

	private vector m_Offset;

	private string m_SoundSetName;

	private ref SoundParams m_SoundParams;
	private ref SoundObjectBuilder m_SoundObjectBuilder;
	private ref SoundObject m_SoundObject;
	private ref AbstractWave m_AbstractWave;

	void ExpansionVehicleSound(ExpansionVehicleBase vehicle, string soundSetName)
	{
		m_Vehicle = vehicle;
		m_SoundSetName = soundSetName;

		string path;

		array<string> soundShaders = new array<string>;
		path = "CfgSoundSets " + soundSetName + " soundShaders";
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetTextArray(path, soundShaders);

		if (soundShaders.Count() > 0)
			ExpansionVehicleSoundManager.Get(soundShaders[0], m_Shader);

		path = "CfgSoundSets " + soundSetName + " positionOffset";
		TFloatArray values = new TFloatArray;
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, values);

		if (values.Count() == 3)
		{
			m_Offset[0] = values[0];
			m_Offset[1] = values[1];
			m_Offset[2] = values[2];
		}
	}

	bool Update(float pDt, array<float> variables)
	{
		if (!m_SoundParams)
		{
			m_SoundParams = new SoundParams(m_SoundSetName);
			if (!m_SoundParams.IsValid())
				return false;
		}

		if (!m_SoundObjectBuilder)
		{
			m_SoundObjectBuilder = new SoundObjectBuilder(m_SoundParams);
			m_SoundObjectBuilder.UpdateEnvSoundControllers(m_Vehicle.GetPosition());

			if (!m_SoundObjectBuilder)
				return false;
		}

		if (!m_SoundObject)
		{
			m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();
			if (!m_SoundObject)
				return false;

			m_SoundObject.SetPosition(m_Vehicle.GetPosition());
			m_SoundObject.SetKind(WaveKind.WAVEEFFECTEX);
		}

		if (!m_AbstractWave)
		{
			m_AbstractWave = GetGame().GetSoundScene().Play3D(m_SoundObject, m_SoundObjectBuilder);
			m_AbstractWave.Play();
			m_AbstractWave.Loop(true);

			if (!m_AbstractWave)
				return false;
		}
		
		m_AbstractWave.SetPosition(m_Vehicle.ModelToWorld(m_Offset));

		m_Shader.Calculate(variables, m_TargetVolume, m_TargetFrequency);

		m_AbstractWave.SetVolume(m_CurrentVolume);
		m_AbstractWave.SetFrequency(m_CurrentFrequency);

		m_CurrentVolume += Math.Clamp(m_TargetVolume - m_CurrentVolume, -20.0 * pDt, 20.0 * pDt);
		m_CurrentFrequency += Math.Clamp(m_TargetFrequency - m_CurrentFrequency, -20.0 * pDt, 20.0 * pDt);

		return true;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Sound Set", m_SoundSetName);

		instance.Add("Target Volume", m_TargetVolume);
		instance.Add("Target Frequency", m_TargetFrequency);

		instance.Add("Current Volume", m_CurrentVolume);
		instance.Add("Current Frequency", m_CurrentFrequency);

		return true;
	}
#endif
};