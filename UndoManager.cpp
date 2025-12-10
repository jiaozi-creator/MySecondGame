#include "UndoManager.h"

UndoManager::UndoManager() {
    undoStack.reserve(MAX_UNDO_HISTORY);
}

UndoManager::~UndoManager() {
    clear();
}

void UndoManager::recordUndo(const UndoModel& undoRecord) {
    undoStack.push_back(undoRecord);

    // 限制历史记录数量
    if (undoStack.size() > MAX_UNDO_HISTORY) {
        undoStack.erase(undoStack.begin());
    }
}

bool UndoManager::canUndo() const {
    return !undoStack.empty();
}

UndoModel UndoManager::getLastUndo() const {
    if (undoStack.empty()) {
        return UndoModel();
    }
    return undoStack.back();
}

void UndoManager::popLastUndo() {
    if (!undoStack.empty()) {
        undoStack.pop_back();
    }
}

void UndoManager::clear() {
    undoStack.clear();
}

size_t UndoManager::getHistoryCount() const {
    return undoStack.size();
}