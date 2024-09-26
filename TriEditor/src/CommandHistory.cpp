#include "CommandHistory.h"

namespace TriEngine {
	CommandHistory::~CommandHistory()
	{
		Reset();
	}

	void CommandHistory::Add(Command* cmd)
	{
		cmd->Execute();
		m_UndoStack.push(cmd);

		while (!m_RedoStack.empty()) {
			delete m_RedoStack.top();
			m_RedoStack.pop();
		}
	}

	void CommandHistory::Undo()
	{
		if (!m_UndoStack.empty()) {
			Command* cmd = m_UndoStack.top();
			cmd->Undo();

			m_UndoStack.pop();
			m_RedoStack.push(cmd);
		}
	}

	void CommandHistory::Redo()
	{
		if (!m_RedoStack.empty()) {
			Command* cmd = m_RedoStack.top();
			cmd->Execute();

			m_RedoStack.pop();
			m_UndoStack.push(cmd);
		}
	}
	void CommandHistory::Reset()
	{
		while (!m_UndoStack.empty()) {
			delete m_UndoStack.top();
			m_UndoStack.pop();
		}

		while (!m_RedoStack.empty()) {
			delete m_RedoStack.top();
			m_RedoStack.pop();
		}
	}
}