#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include <vector>
#include "models/UndoModel.h"

class UndoManager {
private:
    std::vector<UndoModel> undoStack;
    static const int MAX_UNDO_HISTORY = 50;

public:
    UndoManager();
    ~UndoManager();

    // 记录回退操作
    void recordUndo(const UndoModel& undoRecord);

    // 检查是否可以回退
    bool canUndo() const;

    // 获取最后一个回退操作
    UndoModel getLastUndo() const;

    // 弹出最后一个回退操作
    void popLastUndo();

    // 清空回退历史
    void clear();

    // 获取回退历史数量
    size_t getHistoryCount() const;
};

#endif // __UNDO_MANAGER_H__
