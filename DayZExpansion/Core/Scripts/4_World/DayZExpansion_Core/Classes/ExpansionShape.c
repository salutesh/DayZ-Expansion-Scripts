class ExpansionShapeBase : Managed
{
	bool m_NotAligned;

	ExpansionShapeBase NotAligned()
	{
		m_NotAligned = true;
		return this;
	}

	void Update(vector objTransform[4], vector physTransform[4])
	{
		//m_Shape.SetMatrix(inTransform);
	}
};

class ExpansionShape : ExpansionShapeBase
{
	static int s_MatrixColours[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
	
	Shape m_Shape;
	vector m_Origin;

	static ExpansionShape Create(ShapeType type, int color, ShapeFlags flags, vector p1, vector p2)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Shape = Shape.Create(type, color, flags, p1, p2);
#endif

		return res;
	}

	static ExpansionShape CreateLines(int color, ShapeFlags flags, vector p[], int num)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Shape = Shape.CreateLines(color, flags, p, num);
#endif

		return res;
	}

	static ExpansionShape CreateMatrix(ShapeFlags flags, vector mat[4], float scale = 0.5)
	{
		ExpansionShape res();
		
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		vector line[6];
		res.m_Origin = mat[3];
		
		for (int i = 0; i < 3; i++)
		{
			int ii = i * 2;
			line[ii + 0] = "0 0 0";
			line[ii + 1] = (mat[i] * scale);
		}
		
		res.m_Shape = Shape.CreateLines(s_MatrixColours[i], flags, line, 6);
#endif
				
		return res;
	}

	static ExpansionShape CreateLine(int color, ShapeFlags flags, vector p0, vector p1)
	{
		vector line[2];
		line[0] = p0;
		line[1] = p1;
		return CreateLines(color, flags, line, 2);
	}

	static ExpansionShape CreateDirLine(int color, ShapeFlags flags, vector p, vector dir)
	{
		vector line[2];
		line[0] = p;
		line[1] = p + dir;
		return CreateLines(color, flags, line, 2);
	}

	static ExpansionShape CreateTris(int color, ShapeFlags flags, vector p[], int num)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Shape = Shape.CreateTris(color, flags, p, num);
#endif

		return res;
	}

	static ExpansionShape CreateTriangle(int color, ShapeFlags flags, vector p0, vector p1, vector p2)
	{
		vector tri[3];
		tri[0] = p0;
		tri[1] = p1;
		tri[2] = p2;
		return CreateTris(color, flags, tri, 1);
	}

	static ExpansionShape CreateSphere(int color, ShapeFlags flags, vector origin, float radius)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Origin = origin;
		res.m_Shape = Shape.CreateSphere(color, flags, origin, radius);
#endif

		return res;
	}

	static ExpansionShape CreateSphere(bool toggled, int colorA, int colorB, ShapeFlags flags, vector origin, float radius)
	{
		int color = colorA;
		if (toggled)
		{
			color = colorB;
		}

		return CreateSphere(color, flags, origin, radius);
	}
	
	static ExpansionShape CreateCylinder(int color, ShapeFlags flags, vector origin, float radius, float length)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Origin = origin;
		res.m_Shape = Shape.CreateCylinder(color, flags, origin, radius, length);
#endif

		return res;
	}
	
	static ExpansionShape CreateArrow(vector from, vector to, float size, int color, ShapeFlags flags)
	{
		ExpansionShape res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		res.m_Shape = Shape.CreateArrow(from, to, size, color, flags);
#endif

		return res;
	}
	
	void ~ExpansionShape()
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_Shape.Destroy();
		m_Shape = null;
#endif
	}

	override void Update(vector objTransform[4], vector physTransform[4])
	{
		if (m_NotAligned)
		{
			return;
		}

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		vector objPhysTransform[4];
		Math3D.MatrixMultiply4(physTransform, objTransform, objPhysTransform);
		
		vector localTransform[4];
		Math3D.MatrixIdentity4(localTransform);
		localTransform[3] = m_Origin;

		vector worldTransform[4];
		Math3D.MatrixMultiply4(objTransform, localTransform, worldTransform);
		
		m_Shape.SetMatrix(worldTransform);
#endif
	}
};

class ExpansionDebugText : ExpansionShapeBase
{
	ref TextWidget m_Widget;
	vector m_Origin;

	static ExpansionDebugText Create(string text, vector position, int size = 16)
	{
		ExpansionDebugText res();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		Widget layoutRoot = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Core/GUI/layouts/expansion_debug.layout");
		
		res.m_Origin = position;

		if (Class.CastTo(res.m_Widget, layoutRoot))
		{
			res.m_Widget.SetText(text);
			res.m_Widget.SetTextExactSize(size);
		}
#endif

		return res;
	}

	void ~ExpansionDebugText()
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		if (!m_Widget)
		{
			return;
		}

		m_Widget.Unlink();
		m_Widget = null;
#endif
	}

	override void Update(vector objTransform[4], vector physTransform[4])
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		if (!m_Widget)
		{
			return;
		}
		
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		transform[3] = m_Origin;
		
		Math3D.MatrixMultiply4(objTransform, transform, transform);

		vector position = g_Game.GetScreenPos(transform[3]);

		float width;
		float height;
		m_Widget.GetScreenSize(width, height);
		
		float aWidth = 200;
		float aHeight = height;
		
		position[0] = position[0] - (aWidth * 0.5);
		position[1] = position[1] - (aHeight * 0.5);
		
		
		float scale = 18.0 * (1.0 / position[2]);
		
		m_Widget.SetFlags(WidgetFlags.EXACTSIZE);
		//m_Widget.SetSize(width * scale, height * scale);
		m_Widget.SetTextExactSize(scale);
		
		m_Widget.SetPos(position[0], position[1]);
		m_Widget.Show(position[2] > 0);
		m_Widget.Update();
#endif
	}
};
