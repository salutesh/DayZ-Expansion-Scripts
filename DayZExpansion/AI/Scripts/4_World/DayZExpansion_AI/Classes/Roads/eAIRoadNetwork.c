class eAIRoadNetwork
{
	const static int LATEST_VERSION = 4;
	
	private static eAIRoadNetwork INSTANCE;

	private bool m_IsEnabled;

	private int m_Width;
	private int m_Height;
	private ref array<ref eAIRoadNode> m_Roads;
	private ref set<ref eAIRoadSection> m_Sections;
	private ref array<ref eAIRoadNodeSection> m_SectionEnds;

	private ref array<string> m_Directories;
	private string m_FilePath;

	private string m_WorldName;
	private vector m_CenterPoint;

	#ifndef SERVER
	private ref array<Shape> m_DebugShapes = new array<Shape>();
	#endif

	void eAIRoadNetwork()
	{
		INSTANCE = this;

		m_IsEnabled = false;
		
		m_Roads = new array<ref eAIRoadNode>();
		m_Sections = new set<ref eAIRoadSection>();
		m_SectionEnds = new array<ref eAIRoadNodeSection>();
		m_Directories = new array<string>();

		m_WorldName = GetGame().GetWorldName();
		m_CenterPoint = GetGame().ConfigGetVector("CfgWorlds " + m_WorldName + " centerPosition");

		m_Directories.Insert("DayZExpansion/AI/Scripts/Data/Roads");

		int mod_count = GetGame().ConfigGetChildrenCount("CfgMods");
		for (int i = 0; i < mod_count; i++)
		{
			string mod_name;
			GetGame().ConfigGetChildName("CfgMods", i, mod_name);
			
			if (GetGame().ConfigIsExisting("CfgMods " + mod_name + " roadNetworkDirectory"))
			{
				string directory;
				GetGame().ConfigGetText("CfgMods " + mod_name + " roadNetworkDirectory", directory);
				m_Directories.Insert(directory);
			}
		}

		Resize(0, 0);
	}

	void ~eAIRoadNetwork()
	{
		DS_Destroy();
	}
	
	#ifndef SERVER
	static void DS_Add(Shape shape)
	{
		INSTANCE.m_DebugShapes.Insert(shape);
	}
	#endif

	void DS_Destroy()
	{
		#ifndef SERVER
		for (int i = 0; i < m_DebugShapes.Count(); i++)
		{
			m_DebugShapes[i].Destroy();
		}
		m_DebugShapes.Clear();
		#endif
	}

	void DS_Create(vector position, float radius)
	{
		#ifndef SERVER
		array<PathNode> visited();
		for (int i = 0; i < m_Roads.Count(); i++)
		{
			if (visited.Find(m_Roads[i]) != -1) continue;
			vector p1 = m_Roads[i].m_Position;
			p1[1] = 0;
			position[1] = 0;
			if (vector.Distance(p1, position) > radius) continue;
			visited.Insert(m_Roads[i]);

			m_DebugShapes.Insert(Shape.CreateSphere(0xFF0000FF, ShapeFlags.VISIBLE | ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Roads[i].m_Position, 0.5));
			
			for (int j = 0; j < m_Roads[i].m_Neighbours.Count(); j++)
			{
				if (visited.Find(m_Roads[i].m_Neighbours[j]) != -1) continue;

				vector points[2];
				points[0] = m_Roads[i].m_Position;
				points[1] = m_Roads[i].m_Neighbours[j].m_Position;
				m_DebugShapes.Insert(Shape.CreateLines(0xFFFF0000, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER, points, 2));
			}
		}
		#endif
	}

	void DS_SectionCreate(vector position, float radius)
	{
		#ifndef SERVER
		for (int i = 0; i < m_Sections.Count(); i++)
		{
			if (m_Sections[i] == null) continue;
			
			vector p1 = m_Sections[i].m_Position;
			p1[1] = 0;
			position[1] = 0;
			
			if (vector.Distance(p1, position) > radius) continue;
			
			m_DebugShapes.Insert(Shape.CreateSphere(0xFF00FFFF, ShapeFlags.VISIBLE | ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Sections[i].m_Position + "0 0.5 0", 0.5));
			m_DebugShapes.Insert(Shape.CreateSphere(0xFFFFFFFF, ShapeFlags.NOCULL | ShapeFlags.TRANSP | ShapeFlags.NOZWRITE | ShapeFlags.DOUBLESIDE | ShapeFlags.WIREFRAME, m_Sections[i].m_Position, m_Sections[i].m_Radius));

			PathNode s = m_Sections[i].m_Head;
			PathNode e = m_Sections[i].m_Tail;
			
			vector points[2];
			points[0] = s.m_Position + "0 0.1 0";
			points[1] = e.m_Position + "0 0.1 0";
			m_DebugShapes.Insert(Shape.CreateLines(0xFFFFFF00, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER, points, 2));
		}
		#endif
	}

	void DS_SectionEndsCreate(vector position, float radius)
	{
		#ifndef SERVER
		array<PathNode> visited();
		for (int i = 0; i < m_SectionEnds.Count(); i++)
		{
			if (visited.Find(m_SectionEnds[i]) != -1) continue;
			vector p1 = m_SectionEnds[i].m_Position;
			p1[1] = 0;
			position[1] = 0;
			if (vector.Distance(p1, position) > radius) continue;
			visited.Insert(m_SectionEnds[i]);

			m_DebugShapes.Insert(Shape.CreateSphere(0xFFFF00FF, ShapeFlags.VISIBLE | ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_SectionEnds[i].m_Position, 0.5));
			
			for (int j = 0; j < m_SectionEnds[i].m_Neighbours.Count(); j++)
			{
				if (visited.Find(m_SectionEnds[i].m_Neighbours[j]) != -1) continue;

				vector points[2];
				points[0] = m_SectionEnds[i].m_Position;
				points[1] = m_SectionEnds[i].m_Neighbours[j].m_Position;
				m_DebugShapes.Insert(Shape.CreateLines(0xFFFFFF00, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER, points, 2));
			}
		}
		#endif
	}

	void Init()
	{
		m_IsEnabled = true;

		bool loaded = false;
		foreach (string directory : m_Directories)
		{
			m_FilePath = directory + "/" + m_WorldName + ".roads";
			if (FileExist(m_FilePath))
			{
				if (Load(m_Directories[0] == directory))
				{
					loaded = true;
					break;
				}
			}
		}

		if (!loaded)
		{
			m_FilePath = m_Directories[0] + "/" + m_WorldName + ".roads";
			Generate();
			Save();
		}

		//GenerateSections();
	}

	void GenerateSections()
	{
		int i;
		int j;
		int k;

		eAIRoadSection section;

		// Generate a section for every node that has only two connecting neighbours
		Print("Generating new sections for each road");
		for (i = 0; i < m_Roads.Count(); i++)
		{
			m_Roads[i].ClearSections();
			if (m_Roads[i].Count() != 2) continue;

			m_Roads[i].InsertSection(CreateSection());
		}

		// Connect the unconnected sections
		Print("Connecting section pieces");
		for (i = 0; i < m_Roads.Count(); i++)
		{
			if (m_Roads[i].Count() != 2) continue;

			for (j = 0; j < m_Roads[i].Count(); j++)
			{
				if (m_Roads[i][j].Count() != 2) continue;
				if (m_Roads[i].GetSection(0) == eAIRoadNode.Cast(m_Roads[i][j]).GetSection(0)) continue;

				MergeSections(eAIRoadNode.Cast(m_Roads[i][j]).GetSection(0), m_Roads[i].GetSection(0));
			}
		}

		// Connect the unconnected sections
		Print("Connecting sections into nodes of neighbours");
		for (i = 0; i < m_Roads.Count(); i++)
		{
			if (m_Roads[i].Count() == 2) continue;
			
			eAIRoadNodeSection roadNodeSection = new eAIRoadNodeSection();
			m_SectionEnds.Insert(roadNodeSection);

			roadNodeSection.m_Radius = 0.0;
			roadNodeSection.m_Position = m_Roads[i].m_Position;
			m_Roads[i].m_SectionNode = roadNodeSection;

			for (j = 0; j < m_Roads[i].Count(); j++)
			{
				if (m_Roads[i][j].Count() != 2) 
				{
					Error("Expected 2 connecting nodes");
				}

				section = eAIRoadNode.Cast(m_Roads[i][j]).GetSection(0);
				roadNodeSection.m_Sections.Insert(section);
				m_Roads[i].InsertSection(section);
			}
		}

		Print("Initializing sections");
		for (i = m_Sections.Count(); i >= 0; i--)
		{
			Print("INITIALIZING");
			Print("  INDEX: " + i);
			if (m_Sections[i] == null)
			{
				Print("  REMOVING");
				m_Sections.Remove(i);
				continue;
			}
			
			m_Sections[i].Init();
		}

		for (i = 0; i < m_Roads.Count(); i++)
		{
			if (m_Roads[i].Count() == 2) continue;

			for (j = 0; j < m_Roads[i].Count(); j++)
			{
				section = eAIRoadNode.Cast(m_Roads[i][j]).GetSection(0);
				if (section.m_Radius > m_Roads[i].m_SectionNode.m_Radius) m_Roads[i].m_SectionNode.m_Radius = section.m_Radius;
			}
		}

		Print("Finished generating sections");
	}

	eAIRoadSection CreateSection()
	{
		eAIRoadSection section = new eAIRoadSection();
		m_Sections.Insert(section);
		return section;
	}

	void MergeSections(eAIRoadSection a, eAIRoadSection b)
	{
		for (int i = 0; i < a.m_Nodes.Count(); i++)
		{
			a.m_Nodes[i].RemoveSection(a);
			a.m_Nodes[i].InsertSection(b);
		}

		delete a;
	}

	private void Resize(int width, int height)
	{
		m_Roads.Clear();
		m_Sections.Clear();
		m_SectionEnds.Clear();

		m_Width = width;
		m_Height = height;
	}

	void NotifyGenerate(vector position, float radius)
	{
		if (!m_IsEnabled) return;

		_Generate(position, radius);

		GenerateSections();

		DS_Create(position, radius);
		DS_SectionCreate(position, radius);
	}

	private void Generate()
	{
		float radius = m_CenterPoint[0] * 2;
		if (radius < m_CenterPoint[1] * 2) radius = m_CenterPoint[1] * 2;

		_Generate(m_CenterPoint, radius * 2.5);
	}

	private void _Generate(vector position, float radius)
	{
		Print("Generating");

		Resize(position[0] * 2, position[1] * 2);
		
		PGFilter pathFilter = new PGFilter();
		AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();
		vector nmHitPos;
		vector nmHitNormal;
		
		int inFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR;
		int exFlags = PGPolyFlags.CLIMB;

		pathFilter.SetFlags(inFlags, exFlags, PGPolyFlags.NONE);
		pathFilter.SetCost(PGAreaType.NONE, 0.0);
		pathFilter.SetCost(PGAreaType.TERRAIN, 1.0);
		//pathFilter.SetCost(PGAreaType.OBJECTS, 1.0);
		//pathFilter.SetCost(PGAreaType.BUILDING, 1.0);

		int x, z, i, j, k;

		Print("Finding Objects");
		array<ref eAIRoadConnection> connections();

		array<Object> objects();
		array<CargoBase> proxyCargos();

		GetGame().GetObjectsAtPosition(position, radius, objects, proxyCargos);

		for (i = 0; i < objects.Count(); i++)
		{
			Object obj = objects[i];
			if (obj.IsInherited(Camera)) continue;
			if (obj.IsInherited(Particle)) continue;
			if (obj.IsInherited(DayZCreature)) continue;
			if (obj.IsMan()) continue;
			if (obj.IsTransport()) continue;
			if (obj.IsItemBase()) continue;

			LOD geometry = obj.GetLODByName("geometry");
			if (!geometry || !eAIRoadNode.ObjectIsRoad(obj, geometry)) continue;

			eAIRoadNode road = new eAIRoadNode();
			if (road.Generate(obj, connections))
			{
				m_Roads.Insert(road);
			}
		}


		eAIRoadConnection a;
		eAIRoadConnection b;

		int best;
		float dist;
		float minDistBest;
		float connectionRadius;
		
		PathNode nodeA;
		PathNode nodeB;

		//if (false)
		{
			Print("Connecting Roads (Memory Points)");
			connectionRadius = 1.0;
	
			//! Connect roads that were placed properly
			for (i = connections.Count() - 1; i >= 0; i--)
			{
				a = connections[i];
				
				if (a.m_Completed) continue;
				a.m_Completed = true;
	
				minDistBest = (a.m_Node.m_Radius * a.m_Node.m_Radius) * 1.5;
	
				for (j = connections.Count() - 1; j >= 0; j--)
				{
					b = connections[j];
	
					if (b.m_Completed) continue;
					if (a.m_Node == b.m_Node) continue;
					if (!b.m_Node.CanConnectToTarget(a.m_Node)) continue;
					
					if (aiWorld.RaycastNavMesh(a.m_Node.m_Position, b.m_Node.m_Position, pathFilter, nmHitPos, nmHitNormal)) continue;
					
	
					vector aPos = a.m_Position;
					vector bPos = b.m_Position;
	
					aPos[1] = 0.0;
					bPos[1] = 0.0;
	
					dist = vector.DistanceSq(aPos, bPos);
					if (dist < 0.0) continue;
					
					if (dist < minDistBest)
					{
						b.m_Completed = true;
						a.m_Node.Add(b.m_Node);
					}
				}
			}
		}
		
		//if (false)
		{
			Print("Connecting Roads (Far)");
			
			connectionRadius = 20.0;
			
			for (i = connections.Count() - 1; i >= 0; i--)
			{
				a = connections[i];
				
				if (a.m_Node.Count() > 1) continue;
	
				best = -1;
				connectionRadius = a.m_Node.m_Radius * 15.0;
				minDistBest = connectionRadius * connectionRadius;
	
				for (j = connections.Count() - 1; j >= 0; j--)
				{
					b = connections[j];
	
					if (a.m_Node == b.m_Node) continue;
					//if (b.m_Node.Count() > 1) continue;
					if (!b.m_Node.CanConnectToTarget(a.m_Node)) continue;
	
					aPos = a.m_Position;
					bPos = b.m_Position;
	
					aPos[1] = 0.0;
					bPos[1] = 0.0;
	
					dist = vector.DistanceSq(aPos, bPos);
					if (dist < 0.0) continue;
					
					if (dist < minDistBest)
					{
						minDistBest = dist;
						best = j;
					}
				}
				
				if (best != -1)
				{
					b = connections[best];
					a.m_Node.Add(b.m_Node);
				}
			}
		}

		if (false)
		{
			Print("Connecting Roads (Remaining unconnected)");
			
			Print(connections.Count());
			for (i = connections.Count() - 1; i >= 0; i--)
			{
				a = connections[i];
				
				if (a.m_Node.Count() > 1) continue;

				nodeA = null;
				nodeB = null;

				best = -1;
				minDistBest = (connectionRadius * connectionRadius);

				float minDistA = 10.0;
				float minDistB = minDistA;
				float distA;
				float distB;
	
				PathNode temp_nodeA;
				PathNode temp_nodeB;

				for (j = 0; j < m_Roads.Count(); j++)
				{
					distA = vector.Distance(m_Roads[j].m_Position, a.m_Position);
					if (distA > minDistA) continue;

					temp_nodeA = m_Roads[j];
					temp_nodeB = null;
					
					for (k = 0; k < m_Roads[j].Count(); k++)
					{
						distB = vector.Distance(m_Roads[j][k].m_Position, a.m_Position);
						if (distB > minDistB) continue;
						minDistB = distB;

						temp_nodeB = m_Roads[j][k];
					}

					if (temp_nodeB)
					{
						minDistA = distA;
						nodeA = temp_nodeA;
						nodeB = temp_nodeB;
					}
				}

				if (!nodeA || !nodeB) continue;

				nodeA.Remove(nodeB);
				nodeB.Remove(nodeA);

				road = new eAIRoadNode();
				m_Roads.Insert(road);
				
				road.m_Position = (nodeA.m_Position + nodeB.m_Position) * 0.5;
				
				road.Add(nodeA);
				road.Add(nodeB);
				road.Add(a.m_Node);
			}
		}

		//if (false)
		{
			Print("Connecting Roads (Nearby)");
	
			connectionRadius = 10.0;
	
			//! Connect roads that weren't placed properly
			for (i = 0; i < m_Roads.Count(); i++)
			{
				//! we have more than 1 neighbour, we are probably fine
				if (m_Roads[i].Count() > 1) continue;
	
				best = -1;
				minDistBest = connectionRadius * connectionRadius;
	
				for (j = 0; j < m_Roads.Count(); j++)
				{
					if (m_Roads[i] == m_Roads[j]) continue;
	
					// check if the road was already added
					if (m_Roads[i].Contains(m_Roads[j])) continue;

					if (!m_Roads[i].CanConnectToTarget(m_Roads[j])) continue;
					
					if (m_Roads[i].RoadAlreadyConnectedWithin(m_Roads[j], aPos, connectionRadius)) continue;
	
					//if (aiWorld.RaycastNavMesh(m_Roads[i].m_Position, m_Roads[j].m_Position, pathFilter, nmHitPos, nmHitNormal)) continue;
					
					dist = vector.DistanceSq(m_Roads[i].m_Position, m_Roads[j].m_Position);
					if (dist >= 0 && dist < minDistBest)
					{
						minDistBest = dist;
						best = j;
					}
				}
	
				if (best != -1)
				{
					m_Roads[i].Add(m_Roads[best]);
				}
			}
		}
		
		//if (false)
		{
			Print("Removing triangles");
	
			for (i = m_Roads.Count() - 1; i >= 0; i--)
			{
				if (m_Roads[i].Count() != 2) continue;
				
				nodeA = m_Roads[i][0];
				nodeB = m_Roads[i][1];
				
				if (nodeA.Contains(nodeB))
				{
					nodeB.Remove(m_Roads[i]);
					nodeA.Remove(m_Roads[i]);
					
					m_Roads.RemoveOrdered(i);
				}
			}
		}

		//if (false)
		{
			Print("Fixing missing links");
	
			for (i = m_Roads.Count() - 1; i >= 0; i--)
			{
				if (m_Roads[i].Count() == 2) continue;
	
				for (j = m_Roads[i].Count() - 1; j >= 0; j--)
				{
					if (m_Roads[i][j].Count() <= 2) continue;
	
					nodeA = m_Roads[i];
					nodeB = m_Roads[i][j];
					vector nPos = (nodeA.m_Position + nodeB.m_Position) * 0.5;
	
					nodeA.Remove(nodeB);
	
					eAIRoadNode nodeC = new eAIRoadNode();
					m_Roads.Insert(nodeC);
					nodeC.m_Position = nPos;
	
					nodeB.Add(nodeC);
					nodeA.Add(nodeC);
				}
			}
		}

		//if (false)
		{
			Print("Optimizing");
	
			for (i = m_Roads.Count() - 1; i >= 0; i--)
			{
				if (!m_Roads[i] || (m_Roads[i] && m_Roads[i].Optimize()))
				{
					m_Roads.RemoveOrdered(i);
				}
			}
		}

		Print("Fixing indices");

		for (i = 0; i < m_Roads.Count(); i++)
		{
			m_Roads[i].m_Index = i;
		}

		Print("Finished Generating");
	}

	private void Save()
	{
		MakeDirectory(m_Directories[0]);

		FileHandle file_handle = OpenFile(m_FilePath, FileMode.WRITE);

		int version = LATEST_VERSION;

		FPrintln(file_handle, LATEST_VERSION);
		FPrintln(file_handle, m_Width);
		FPrintln(file_handle, m_Height);
		FPrintln(file_handle, m_Roads.Count());
		
		for (int i = 0; i < m_Roads.Count(); i++)
		{
			m_Roads[i].Save(file_handle, LATEST_VERSION);
		}

		CloseFile(file_handle);
	}

	private bool OnLoad(FileHandle file_handle, out int version)
	{
		m_Roads.Clear();

		string line_content;
		int x;
		int z;
		int i;
		int count;

		FGets(file_handle, line_content);
		version = line_content.ToInt();

		if (version < 4) return false;

		FGets(file_handle, line_content);
		m_Width = line_content.ToInt();

		FGets(file_handle, line_content);
		m_Height = line_content.ToInt();

		if (m_Width != m_CenterPoint[0] * 2) return false;
		if (m_Height != m_CenterPoint[1] * 2) return false;

		Resize(m_Width, m_Height);

		FGets(file_handle, line_content);
		count = line_content.ToInt();
		
		array<ref eAIRoadNodeJoinMap> connections();

		for (i = 0; i < count; i++)
		{
			eAIRoadNode road = new eAIRoadNode();
			road.Load(file_handle, version, connections);
			m_Roads.Insert(road);
		}

		for (i = 0; i < connections.Count(); i++)
		{
			foreach (int index : connections[i].indices)
			{
				connections[i].node.Add(m_Roads[index]);
			}
		}

		return true;
	}

	private bool Load(bool saveOnFail = true)
	{
		FileHandle file_handle = OpenFile(m_FilePath, FileMode.READ);
		int version;

		if (!OnLoad(file_handle, version))
		{
			CloseFile(file_handle);
			return false;
		}

		CloseFile(file_handle);

		if (version != LATEST_VERSION && saveOnFail)
		{
			Save();
		}

		return true;
	}

	eAIRoadNode GetClosestNode(vector position)
	{
		float minDistance = m_CenterPoint.Length() * 4.0;
		eAIRoadNode closest = null;
		for (int i = 0; i < m_Roads.Count(); i++)
		{
			float dist = vector.Distance(m_Roads[i].m_Position, position);
			if (dist < minDistance)
			{
				closest = m_Roads[i];
				minDistance = dist;
			}
		}

		return closest;
	}

	Param2<eAIRoadNode, eAIRoadNodeSection> _GetClosestNode(vector position)
	{
		Param2<eAIRoadNode, eAIRoadNodeSection> closest = new Param2<eAIRoadNode, eAIRoadNodeSection>(null, null);

		array<eAIRoadNode> nodes();

		float minDistance = m_CenterPoint.Length() * 4.0;
		for (int i = 0; i < m_SectionEnds.Count(); i++)
		{
			float dist = vector.Distance(m_SectionEnds[i].m_Position, position);
			if (dist > (m_SectionEnds[i].m_Radius)) continue;
			
			set<eAIRoadSection> sections = m_SectionEnds[i].m_Sections;
			for (int j = 0; j < sections.Count(); j++)
			{
				if (sections[j]) sections[j].UpdateNodeClosest(position, nodes);
			}
		}

		minDistance = m_CenterPoint.Length() * 4.0;
		for (i = 0; i < nodes.Count(); i++)
		{
			dist = vector.Distance(nodes[i].m_Position, position);
			if (dist < minDistance) 
			{
				minDistance = dist;
				closest.param1 = nodes[i];
				closest.param2 = nodes[i].m_Sections[0].m_Head.m_SectionNode;
			}
		}

		return closest;
	}

	void FindPath(vector start, vector end, ExpansionPathHandler pathFinding)
	{
		if (!m_IsEnabled) return;

		//Print("+eAIRoadNetwork::FindPath");
		//GetGame().GameScript.Call(this, "_FindPath", new Param3<vector, vector, ExpansionPathHandler>(start, end, pathFinding));
		//thread _FindPath(start, end, pathFinding);
		//Print("-eAIRoadNetwork::FindPath");
	}

	void _FindPath(Param3<vector, vector, ExpansionPathHandler> param)
	{
	}
};
