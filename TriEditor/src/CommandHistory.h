#pragma once

#include "Command.h"
#include <stack>

namespace TriEngine {
	class CommandHistory {
	public:
		CommandHistory() = default;
		~CommandHistory();

		void Add(Command* cmd);
		void Undo();
		void Redo();

		void Reset();
	private:
		std::stack<Command*> m_UndoStack;
		std::stack<Command*> m_RedoStack;
	};
}
