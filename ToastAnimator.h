#pragma once

#include <QPoint>

class ToastPrivate;
class QPropertyAnimation;

/**
 * @brief 动画管理器（内部类）
 *
 * 负责 Toast 的淡入、淡出、飞入和位置更新动画。
 */
class ToastAnimator
{
public:
    explicit ToastAnimator(ToastPrivate *d);

    void fadeIn();
    void fadeOut();
    void flyIn();
    void updatePosition();

private:
    ToastPrivate *d_ptr;
};
