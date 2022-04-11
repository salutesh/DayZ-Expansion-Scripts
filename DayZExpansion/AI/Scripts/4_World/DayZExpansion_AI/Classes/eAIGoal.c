class eAIGoal
{
	ref eAIGoal m_Previous;
	ref eAIGoal m_Next;

	Object TargetObject;
	vector Position_WorldSpace;
	float ThreatCost;

	void ~eAIGoal()
	{
#ifndef SERVER
		DestroyDebug();
#endif
	}

#ifndef SERVER
	Widget LayoutRoot;
	TextWidget TextA;
	TextWidget TextB;
	TextWidget TextC;

	void DestroyDebug()
	{
		if (LayoutRoot) LayoutRoot.Unlink();
	}
/*
	void Debug(int idx)
	{
		if (!LayoutRoot)
		{
			LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/AI/GUI/eai_goal_debug.layout");
			if (!LayoutRoot) return;

			Class.CastTo(TextA, LayoutRoot.FindAnyWidget("TextWidget0"));
			Class.CastTo(TextB, LayoutRoot.FindAnyWidget("TextWidget1"));
			Class.CastTo(TextC, LayoutRoot.FindAnyWidget("TextWidget2"));
		}

		vector screenPos = GetGame().GetScreenPos(Position_WorldSpace);
		LayoutRoot.SetPos(screenPos[0], screenPos[1]);
		LayoutRoot.Show(screenPos[2] > 0); 

		TextA.SetText("Type: " + TargetObject.GetType());
		TextB.SetText("Cost: " + ThreatCost);
		TextC.SetText("Index: " + idx);

		handler.AddShape(Shape.CreateSphere(0xFFFFFFFF, ShapeFlags.NOZBUFFER | ShapeFlags.WIREFRAME, Position_WorldSpace, 0.01));
	}
*/
#endif
};