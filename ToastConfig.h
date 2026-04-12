#pragma once

#include <QColor>
#include <QFont>
#include <QMargins>
#include <QPixmap>
#include <QSize>
#include <QString>

// 前向声明（来自 Toast.h）
enum class ToastIcon;
enum class ToastPreset;
enum class ToastButtonAlignment;

/**
 * @brief 配置类，用于定义单个 Toast 实例的外观和行为。
 *
 * 内部使用子结构体分组：Content（内容）、Appearance（外观）、Behavior（行为）。
 * 公共 API 与原 ToastConfig 完全一致。
 */
class ToastConfig final
{
public:
    // ====================
    // 子结构体定义
    // ====================

    struct Content
    {
        QString title;
        QString text;
        QPixmap icon;
        bool showIcon = false;
        QSize iconSize{18, 18};
        bool showIconSeparator = true;
        int iconSeparatorWidth = 2;
        QPixmap closeButtonIcon;
        bool showCloseButton = true;
        QSize closeButtonIconSize{10, 10};
        QSize closeButtonSize{24, 24};
        ToastButtonAlignment closeButtonAlignment;
    };

    struct Appearance
    {
        int borderRadius = 4;
        QColor backgroundColor;
        QColor titleColor;
        QColor textColor;
        QColor iconColor;
        QColor iconSeparatorColor;
        QColor closeButtonIconColor;
        QColor durationBarColor;
        QFont titleFont{"Arial", 10, QFont::Bold};
        QFont textFont{"Arial", 9};
        QMargins margins{20, 18, 10, 18};
        QMargins iconMargins{0, 0, 15, 0};
        QMargins iconSectionMargins{0, 0, 15, 0};
        QMargins textSectionMargins{0, 0, 15, 0};
        QMargins closeButtonMargins{0, -8, 0, -8};
        int textSectionSpacing = 8;
    };

    struct Behavior
    {
        int duration = 5000;
        bool showDurationBar = true;
        int fadeInDuration = 250;
        int fadeOutDuration = 250;
        bool resetDurationOnHover = true;
        bool stayOnTop = true;
        int buttonsPerRow = 0; // 0 = 使用全局默认值
    };

    // ====================
    // 公共 API（与原 ToastConfig 完全一致）
    // ====================

    ToastConfig();

    static QPixmap getIconFromEnum(ToastIcon enumIcon) noexcept;
    static QImage recolorImage(QImage image, const QColor &color) noexcept;

    ToastConfig &setPreset(ToastPreset preset) noexcept;
    ToastConfig &setDuration(int duration) noexcept;
    ToastConfig &setShowDurationBar(bool enabled) noexcept;
    ToastConfig &setTitle(const QString &title) noexcept;
    ToastConfig &setText(const QString &text) noexcept;
    ToastConfig &setIcon(const QPixmap &icon) noexcept;
    ToastConfig &setIcon(ToastIcon icon) noexcept;
    ToastConfig &setShowIcon(bool enabled) noexcept;
    ToastConfig &setIconSize(const QSize &size) noexcept;
    ToastConfig &setShowIconSeparator(bool enabled) noexcept;
    ToastConfig &setIconSeparatorWidth(int width) noexcept;
    ToastConfig &setCloseButtonIcon(const QPixmap &icon) noexcept;
    ToastConfig &setCloseButtonIcon(ToastIcon icon) noexcept;
    ToastConfig &setShowCloseButton(bool enabled) noexcept;
    ToastConfig &setCloseButtonIconSize(const QSize &size) noexcept;
    ToastConfig &setCloseButtonSize(const QSize &size) noexcept;
    ToastConfig &setCloseButtonAlignment(ToastButtonAlignment alignment) noexcept;
    ToastConfig &setFadeInDuration(int duration) noexcept;
    ToastConfig &setFadeOutDuration(int duration) noexcept;
    ToastConfig &setResetDurationOnHover(bool enabled) noexcept;
    ToastConfig &setStayOnTop(bool enabled) noexcept;
    ToastConfig &setBorderRadius(int borderRadius) noexcept;
    ToastConfig &setBackgroundColor(const QColor &color) noexcept;
    ToastConfig &setTitleColor(const QColor &color) noexcept;
    ToastConfig &setTextColor(const QColor &color) noexcept;
    ToastConfig &setIconColor(const QColor &color) noexcept;
    ToastConfig &setIconSeparatorColor(const QColor &color) noexcept;
    ToastConfig &setCloseButtonIconColor(const QColor &color) noexcept;
    ToastConfig &setDurationBarColor(const QColor &color) noexcept;
    ToastConfig &setTitleFont(const QFont &font) noexcept;
    ToastConfig &setTextFont(const QFont &font) noexcept;
    ToastConfig &setMargins(const QMargins &margins) noexcept;
    ToastConfig &setIconMargins(const QMargins &margins) noexcept;
    ToastConfig &setIconSectionMargins(const QMargins &margins) noexcept;
    ToastConfig &setTextSectionMargins(const QMargins &margins) noexcept;
    ToastConfig &setCloseButtonMargins(const QMargins &margins) noexcept;
    ToastConfig &setTextSectionSpacing(int spacing) noexcept;
    ToastConfig &setButtonsPerRow(int count) noexcept;

    // Getters
    [[nodiscard]] int duration() const noexcept;
    [[nodiscard]] bool showDurationBar() const noexcept;
    [[nodiscard]] const QString &text() const noexcept;
    [[nodiscard]] const QString &title() const noexcept;
    [[nodiscard]] const QPixmap &icon() const noexcept;
    [[nodiscard]] bool showIcon() const noexcept;
    [[nodiscard]] const QSize &iconSize() const noexcept;
    [[nodiscard]] bool showIconSeparator() const noexcept;
    [[nodiscard]] int iconSeparatorWidth() const noexcept;
    [[nodiscard]] const QPixmap &closeButtonIcon() const noexcept;
    [[nodiscard]] bool showCloseButton() const noexcept;
    [[nodiscard]] const QSize &closeButtonIconSize() const noexcept;
    [[nodiscard]] const QSize &closeButtonSize() const noexcept;
    [[nodiscard]] ToastButtonAlignment closeButtonAlignment() const noexcept;
    [[nodiscard]] int fadeInDuration() const noexcept;
    [[nodiscard]] int fadeOutDuration() const noexcept;
    [[nodiscard]] bool resetDurationOnHover() const noexcept;
    [[nodiscard]] bool stayOnTop() const noexcept;
    [[nodiscard]] int borderRadius() const noexcept;
    [[nodiscard]] const QColor &backgroundColor() const noexcept;
    [[nodiscard]] const QColor &titleColor() const noexcept;
    [[nodiscard]] const QColor &textColor() const noexcept;
    [[nodiscard]] const QColor &iconColor() const noexcept;
    [[nodiscard]] const QColor &iconSeparatorColor() const noexcept;
    [[nodiscard]] const QColor &closeButtonIconColor() const noexcept;
    [[nodiscard]] const QColor &durationBarColor() const noexcept;
    [[nodiscard]] const QFont &titleFont() const noexcept;
    [[nodiscard]] const QFont &textFont() const noexcept;
    [[nodiscard]] const QMargins &margins() const noexcept;
    [[nodiscard]] const QMargins &iconMargins() const noexcept;
    [[nodiscard]] const QMargins &iconSectionMargins() const noexcept;
    [[nodiscard]] const QMargins &textSectionMargins() const noexcept;
    [[nodiscard]] const QMargins &closeButtonMargins() const noexcept;
    [[nodiscard]] int textSectionSpacing() const noexcept;
    [[nodiscard]] int buttonsPerRow() const noexcept;

    // 子结构体访问（内部使用）
    [[nodiscard]] const Content &content() const noexcept { return m_content; }
    [[nodiscard]] const Appearance &appearance() const noexcept { return m_appearance; }
    [[nodiscard]] const Behavior &behavior() const noexcept { return m_behavior; }

private:
    Content m_content;
    Appearance m_appearance;
    Behavior m_behavior;
};
