#pragma once

#include "ToastConfig.h"

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include <QVariant>

class Toast;
class QLabel;
class QPushButton;
class QWidget;
class QGraphicsOpacityEffect;
class QTimer;

/**
 * @brief Toast 的 PIMPL 实现类（内部）
 *
 * 包含所有 Toast 的 UI 组件和状态数据。
 */
class ToastPrivate
{
    Q_DECLARE_PUBLIC(Toast)
public:
    explicit ToastPrivate(Toast *q);

    void setupUI();
    void setupDefaultContent();
    void updateStylesheet();
    QPoint calculatePosition();
    void hideToast();
    void updateDurationBar();

    // 状态
    Toast *q_ptr;
    ToastConfig m_config;
    QVariant m_data;
    int m_elapsedTime = 0;
    bool m_fadingOut = false;
    bool m_used = false;

    // 动画器（Task 6 中将初始化）
    std::unique_ptr<class ToastAnimator> m_animator;

    // UI 组件
    QLabel *m_notification = nullptr;
    std::array<QWidget *, 5> m_dropShadowLayer{};
    QGraphicsOpacityEffect *m_opacityEffect = nullptr;
    QPushButton *m_closeButton = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_textLabel = nullptr;
    QPushButton *m_iconWidget = nullptr;
    QWidget *m_iconSeparator = nullptr;
    QWidget *m_durationBarContainer = nullptr;
    QWidget *m_durationBar = nullptr;
    QWidget *m_durationBarChunk = nullptr;
    QTimer *m_durationTimer = nullptr;
    QTimer *m_durationBarTimer = nullptr;
    std::vector<std::pair<QString, std::function<void()>>> m_buttons;
};
