class ExpansionPriorityQueue<Class T>
{
	private ref array<ref Param2<ref T, float>> elements = new array<ref Param2<ref T, float>>();

	int Count()
	{
		return elements.Count();
	}

	void Enqueue(T item, float priority)
	{
		elements.Insert(new Param2<ref T, float>(item, priority));
	}

	T Dequeue()
	{
		int bestIndex = 0;

		int idx = 0;

		for (int i = 0; i < elements.Count(); i++)
		{
			idx++;

			if (idx > 20)
			{
				//Sleep(1);
				idx = 0;
			}

			if (elements[i].param2 < elements[bestIndex].param2)
			{
				bestIndex = i;
			}
		}

		T bestItem = elements[bestIndex].param1;
		elements.Remove(bestIndex);
		return bestItem;
	}
}
