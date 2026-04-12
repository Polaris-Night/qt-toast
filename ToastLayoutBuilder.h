#pragma once

#include <functional>
#include <utility>
#include <QSize>
#include <QString>
#include <QVector>

class ToastPrivate;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QPushButton;
class ToastConfig;

/**
 * @brief 布局构建器（内部类）
 *
 * 负责从 ToastConfig 构建 Toast 的完整 UI 布局。
 */
class ToastLayoutBuilder
{
public:
    static void buildLayout(ToastPrivate *d);
    static QSize calculateSize(
        const ToastConfig &config,
        const std::vector<std::pair<QString, std::function<void()>>> &buttons,
        int maxTextWidth = 500);

private:
    static void buildIconSection(ToastPrivate *d, QHBoxLayout *mainLayout);
    static void buildContentArea(ToastPrivate *d, QHBoxLayout *mainLayout);
    static void buildButtons(ToastPrivate *d, QVBoxLayout *contentLayout);
    static void buildCloseButton(ToastPrivate *d, QHBoxLayout *mainLayout);
    static void buildDurationBar(ToastPrivate *d, QVBoxLayout *rootLayout);
    static void applyButtonStyles(QPushButton *button, const ToastConfig &config);
};
