modded class ModifiersManager
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		CachedObjectsArrays.ARRAY_INT.Clear();
		
		int modifierCount;
		foreach (ModifierBase modifier : m_ModifierList)
		{
			if (modifier.IsActive() && modifier.IsPersistent())
			{
				modifierCount++;
				CachedObjectsArrays.ARRAY_INT.Insert(modifier.GetModifierID());
				if (modifier.IsTrackAttachedTime())
					CachedObjectsArrays.ARRAY_INT.Insert(modifier.GetAttachedTime());
				else
					CachedObjectsArrays.ARRAY_INT.Insert(-1);  //! Always write a value
			}
		}

		ctx.Write(modifierCount);

		foreach (int item: CachedObjectsArrays.ARRAY_INT)
		{
			ctx.Write(item);
		}

		//! Unlike vanilla, we write count so we'll always be able to successfully read back what was actually saved
		ctx.Write(m_ParamList.Count());

		foreach (auto param: m_ParamList)
		{
			typename paramType = param.Type();
			int varCount = paramType.GetVariableCount();

			ctx.Write(varCount);

			for (int i = 0; i < varCount; i++)
			{
				typename varType = paramType.GetVariableType(i);
				switch (varType)
				{
					case bool:
						bool vb;
						paramType.GetVariableValue(param, i, vb);
						ctx.Write(CF_ModStorageDataType.BOOL);
						ctx.Write(vb);
						break;

					case int:
						int vi;
						paramType.GetVariableValue(param, i, vi);
						ctx.Write(CF_ModStorageDataType.INT);
						ctx.Write(vi);
						break;

					case float:
						float vf;
						paramType.GetVariableValue(param, i, vf);
						ctx.Write(CF_ModStorageDataType.FLOAT);
						ctx.Write(vf);
						break;

					case string:
						string vs;
						paramType.GetVariableValue(param, i, vs);
						ctx.Write(CF_ModStorageDataType.STRING);
						ctx.Write(vs);
						break;

					case vector:
						vector vv;
						paramType.GetVariableValue(param, i, vv);
						ctx.Write(CF_ModStorageDataType.VECTOR);
						ctx.Write(vv);
						break;

					default:
						EXError.Error(this, "Unsupported variable type " + varType);
						ctx.Write(CF_ModStorageDataType.INVALID);
						break;
				}
			}
		}
	}

	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		int modifierCount;
		if (!ctx.Read(modifierCount))
			return false;

		while (modifierCount--)
		{
			int modifierId;
			if (!ctx.Read(modifierId))
				return false;

			int time = -1;
			if (!ctx.Read(time))
				return false;

			ModifierBase modifier = GetModifier(modifierId);
			if (modifier)
			{
				if (modifier.IsTrackAttachedTime() && time > -1)
					modifier.SetAttachedTime(time);
				
				ActivateModifier(modifierId, EActivationType.TRIGGER_EVENT_ON_CONNECT);
			}
			else
			{
				Debug.LogError("DB loading: non-existent modifier with id:"+modifierId.ToString());
			}
		}

		int paramsCount;
		if (!ctx.Read(paramsCount))
			return false;

		typename paramType;
		int paramVarCount;
		typename paramVarType;

		for (int x = 0; x < paramsCount; x++)
		{
			int varCount = 0;
			if (!ctx.Read(varCount))
				return false;

			auto param = m_ParamList[x];
			
			if (param)
			{
				paramType = param.Type();
				paramVarCount = paramType.GetVariableCount();
			}
			else
			{
				paramVarCount = 0;
			}

			for (int i = 0; i < varCount; i++)
			{
				int varType;
				if (!ctx.Read(varType))
					return false;

				bool vb = false;
				int vi = 0;
				float vf = 0;
				string vs = string.Empty;
				vector vv = vector.Zero;

				if (i < paramVarCount)
					paramVarType = paramType.GetVariableType(i);

				int n = i + 1;

				switch (varType)
				{
					case CF_ModStorageDataType.BOOL:
						if (!ctx.Read(vb))
							return false;
						if (i < paramVarCount && paramVarType == bool)
							EnScript.SetClassVar(param, "param" + n, 0, vb);
						else
							EXError.Error(this, "Invalid variable index or type " + i + " " + paramVarType + ", expected bool");
						break;

					case CF_ModStorageDataType.INT:
						if (!ctx.Read(vi))
							return false;
						if (i < paramVarCount && paramVarType == int)
							EnScript.SetClassVar(param, "param" + n, 0, vi);
						else
							EXError.Error(this, "Invalid variable index or type " + i + " " + paramVarType + ", expected int");
						break;

					case CF_ModStorageDataType.FLOAT:
						if (!ctx.Read(vf))
							return false;
						if (i < paramVarCount && paramVarType == float)
							EnScript.SetClassVar(param, "param" + n, 0, vf);
						else
							EXError.Error(this, "Invalid variable index or type " + i + " " + paramVarType + ", expected float");
						break;

					case CF_ModStorageDataType.STRING:
						if (!ctx.Read(vs))
							return false;
						if (i < paramVarCount && paramVarType == string)
							EnScript.SetClassVar(param, "param" + n, 0, vs);
						else
							EXError.Error(this, "Invalid variable index or type " + i + " " + paramVarType + ", expected string");
						break;

					case CF_ModStorageDataType.VECTOR:
						if (!ctx.Read(vv))
							return false;
						if (i < paramVarCount && paramVarType == vector)
							EnScript.SetClassVar(param, "param" + n, 0, vv);
						else
							EXError.Error(this, "Invalid variable index or type " + i + " " + paramVarType + ", expected vector");
						break;

					default:
						EXError.Error(this, "Unsupported variable type " + varType);
						break;
				}

				n++;
			}
		}

		return true;
	}
#endif
}
