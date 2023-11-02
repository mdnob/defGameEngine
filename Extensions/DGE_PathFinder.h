#ifndef DGE_PATHFINDER_HPP
#define DGE_PATHFINDER_HPP

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022 Alex
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***/
#pragma endregion

#pragma region includes

#include <list>
#include <cmath>
#include <functional>
#include <vector>

#pragma endregion

namespace def
{
	struct Node
	{
		Node();

		bool isObstacle;
		bool isVisited;

		float globalGoal;
		float localGoal;

		def::vi2d pos;

		std::vector<Node*> neighbours;
		Node* parent;
	};

	class PathFinder
	{
	public:
		PathFinder();
		~PathFinder();

	private:
		def::vi2d m_MapSize;

		Node* m_Nodes;

		Node* m_Start;
		Node* m_Goal;

		bool m_IsMapFreed;

	public:
		void ClearMap();
		bool FreeMap();
		bool ConstructMap(const def::vi2d& size);

		bool SetNodes(const def::vi2d& start, const def::vi2d& goal);
		void SetNodes(Node* start = nullptr, Node* goal = nullptr);

		Node* GetStartNode();
		Node* GetGoalNode();
		Node* GetNodes();

		void ResetNodes();

		int GetMapWidth() const;
		int GetMapHeight() const;
		def::vi2d GetMapSize() const;

		void FindPath(float (*dist)(Node*, Node*), float (*heuristic)(Node*, Node*));

	};

#ifdef DGE_PATHFINDER
#undef DGE_PATHFINDER

	Node::Node()
	{
		isObstacle = false;
		isVisited = false;

		globalGoal = INFINITY;
		localGoal = INFINITY;

		parent = nullptr;
	}

	PathFinder::PathFinder()
	{
		m_Start = nullptr;
		m_Goal = nullptr;

		m_IsMapFreed = false;
	}

	PathFinder::~PathFinder()
	{
		FreeMap();
	}

	void PathFinder::ClearMap()
	{
		for (int x = 0; x < m_MapSize.x; x++)
			for (int y = 0; y < m_MapSize.y; y++)
			{
				int p = y * m_MapSize.x + x;

				m_Nodes[p].isVisited = false;

				m_Nodes[p].globalGoal = INFINITY;
				m_Nodes[p].localGoal = INFINITY;

				m_Nodes[p].pos = { x, y };

				m_Nodes[p].parent = nullptr;
			}
	}

	bool PathFinder::FreeMap()
	{
		if (!m_IsMapFreed)
		{
			delete[] m_Nodes;
			m_IsMapFreed = true;
		}

		return m_IsMapFreed;
	}

	bool PathFinder::ConstructMap(const def::vi2d& size)
	{
		if (size.x <= 0 || size.y <= 0)
			return false;

		m_MapSize = size;

		if (FreeMap())
			m_IsMapFreed = false;

		m_Nodes = new Node[size.x * size.y];

		ClearMap();

		for (int x = 0; x < size.x; x++)
			for (int y = 0; y < size.y; y++)
			{
				bool topFits = y > 0;
				bool bottomFits = y < size.y - 1;

				bool leftFits = x > 0;
				bool rightFits = x < size.x - 1;

				if (topFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y - 1) * size.x + x]);

				if (bottomFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y + 1) * size.x + x]);

				if (leftFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[y * size.x + x - 1]);

				if (rightFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[y * size.x + x + 1]);
			
				if (topFits && leftFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y - 1) * size.x + x - 1]);

				if (bottomFits && rightFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y + 1) * size.x + x + 1]);

				if (leftFits && bottomFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y + 1) * size.x + x - 1]);

				if (rightFits && topFits)
					m_Nodes[y * size.x + x].neighbours.push_back(&m_Nodes[(y - 1) * size.x + x + 1]);
			}

		return true;
	}

	bool PathFinder::SetNodes(const def::vi2d& start, const def::vi2d& goal)
	{
		if (start < def::vi2d(0, 0) || goal < def::vi2d(0, 0) || start >= m_MapSize || goal >= m_MapSize)
			return false;

		m_Start = &m_Nodes[start.y * m_MapSize.x + start.x];
		m_Goal = &m_Nodes[goal.y * m_MapSize.x + goal.x];

		return true;
	}

	void PathFinder::SetNodes(Node* start, Node* goal)
	{
		if (start) m_Start = start;
		if (goal) m_Goal = goal;
	}

	Node* PathFinder::GetStartNode()
	{
		return m_Start;
	}

	Node* PathFinder::GetGoalNode()
	{
		return m_Goal;
	}

	void PathFinder::ResetNodes()
	{
		m_Start = nullptr;
		m_Goal = nullptr;
	}

	Node* PathFinder::GetNodes()
	{
		return m_Nodes;
	}
	
	int PathFinder::GetMapWidth() const
	{
		return m_MapSize.x;
	}

	int PathFinder::GetMapHeight() const
	{
		return m_MapSize.y;
	}

	def::vi2d PathFinder::GetMapSize() const
	{
		return m_MapSize;
	}

	void PathFinder::FindPath(float (*dist)(Node*, Node*), float (*heuristic)(Node*, Node*))
	{
		Node* current = m_Start;
		current->localGoal = 0.0f;
		current->globalGoal = heuristic(current, m_Goal);

		std::list<Node*> nodesToTest;
		nodesToTest.push_back(current);

		while (!nodesToTest.empty() && current != m_Goal)
		{
			nodesToTest.sort(
				[](const Node* lhs, const Node* rhs)
				{
					return lhs->globalGoal < rhs->globalGoal;
				}
			);

			while (!nodesToTest.empty() && nodesToTest.front()->isVisited)
				nodesToTest.pop_front();

			if (nodesToTest.empty()) break;

			current = nodesToTest.front();
			current->isVisited = true;

			for (auto& n : current->neighbours)
			{
				if (!n->isVisited && !n->isObstacle)
					nodesToTest.push_back(n);

				float possiblyLowerGoal = current->localGoal + dist(current, n);

				if (possiblyLowerGoal < n->localGoal)
				{
					n->parent = current;
					n->localGoal = possiblyLowerGoal;
					n->globalGoal = n->localGoal + heuristic(n, m_Goal);
				}
			}
		}
	}

#endif
}

#endif
