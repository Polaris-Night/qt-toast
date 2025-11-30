#pragma once

#include <deque>
#include <functional>
#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVariant>
#include <QWidget>

class Toast;

enum class ToastPreset {
    Success,
    Warning,
    Error,
    Information,
    SuccessDark,
    WarningDark,
    ErrorDark,
    InformationDark
};

enum class ToastIcon { Success, Warning, Error, Information, Close };

enum class ToastPosition {
    BottomLeft,
    BottomMiddle,
    BottomRight,
    TopLeft,
    TopMiddle,
    TopRight,
    Center
};

enum class ToastButtonAlignment { Top, Middle, Bottom };

/**
 * @brief 全局单例类，用于管理 Toast 组件的所有全局配置。
 *
 */
class ToastGlobalConfig final
{
public:
    static ToastGlobalConfig &instance() noexcept;

    Q_DISABLE_COPY_MOVE(ToastGlobalConfig)

    // 全局常量 (Colors & Sizes)
    static const int UpdatePositionDuration;    ///< 位置更新动画时长 (毫秒)
    static const int DurationBarUpdateInterval; ///< 进度条更新间隔 (毫秒)
    static const int DropShadowSize;            ///< 阴影大小 (像素)
    static const QColor SuccessAccentColor;
    static const QColor WarningAccentColor;
    static const QColor ErrorAccentColor;
    static const QColor InformationAccentColor;
    static const QColor DefaultAccentColor;
    static const QColor DefaultBackgroundColor;
    static const QColor DefaultTitleColor;
    static const QColor DefaultTextColor;
    static const QColor DefaultIconSeparatorColor;
    static const QColor DefaultCloseButtonIconColor;
    static const QColor DefaultBackgroundColorDark;
    static const QColor DefaultTitleColorDark;
    static const QColor DefaultTextColorDark;
    static const QColor DefaultIconSeparatorColorDark;
    static const QColor DefaultCloseButtonIconColorDark;

    /**
     * @brief 获取当前允许同时在屏幕上显示的最大 Toast 数量。
     * @return 最大显示数量。
     */
    [[nodiscard]] int maximumOnScreen() const noexcept;

    /**
     * @brief 设置允许同时在屏幕上显示的最大 Toast 数量。
     * @param maximum 指定的最大数量。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setMaximumOnScreen(int maximum) noexcept;

    /**
     * @brief 获取相邻 Toast 之间的垂直间距（像素）。
     * @return 间距值。
     */
    [[nodiscard]] int spacing() const noexcept;

    /**
     * @brief 设置相邻 Toast 之间的垂直间距（像素）。
     * @param spacing 指定的间距值。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setSpacing(int spacing) noexcept;

    /**
     * @brief 获取 Toast 相对于其锚定边缘的水平偏移量（像素）。
     * @return 水平偏移量。
     */
    [[nodiscard]] int offsetX() const noexcept;

    /**
     * @brief 获取 Toast 相对于其锚定边缘的垂直偏移量（像素）。
     * @return 垂直偏移量。
     */
    [[nodiscard]] int offsetY() const noexcept;

    /**
     * @brief 同时设置 Toast 相对于其锚定边缘的水平和垂直偏移量（像素）。
     * @param x 水平偏移量。
     * @param y 垂直偏移量。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setOffset(int x, int y) noexcept;

    /**
     * @brief 设置 Toast 相对于其锚定边缘的水平偏移量（像素）。
     * @param offsetX 指定的水平偏移量。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setOffsetX(int offsetX) noexcept;

    /**
     * @brief 设置 Toast 相对于其锚定边缘的垂直偏移量（像素）。
     * @param offsetY 指定的垂直偏移量。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setOffsetY(int offsetY) noexcept;

    /**
     * @brief 查询是否强制将所有 Toast 显示在主屏幕上。
     * @return true 表示强制在主屏幕显示。
     */
    [[nodiscard]] bool alwaysOnMainScreen() const noexcept;

    /**
     * @brief 设置是否强制将所有 Toast 显示在主屏幕上。
     * @param enabled true 表示启用此功能。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setAlwaysOnMainScreen(bool enabled) noexcept;

    /**
     * @brief 获取当前被固定作为 Toast 显示目标的特定屏幕。
     * @return 固定的 QScreen 指针，如果未设置则返回 nullptr。
     */
    [[nodiscard]] QScreen *fixedScreen() const noexcept;

    /**
     * @brief 设置一个固定的屏幕，所有 Toast 将显示在此屏幕上。
     * @param screen 指向目标 QScreen 的指针。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setFixedScreen(QScreen *screen) noexcept;

    /**
     * @brief 获取 Toast 在屏幕上的默认显示位置。
     * @return ToastPosition 枚举值。
     */
    [[nodiscard]] ToastPosition position() const noexcept;

    /**
     * @brief 设置 Toast 在屏幕上的默认显示位置。
     * @param position ToastPosition 枚举值。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastGlobalConfig &setPosition(ToastPosition position) noexcept;

private:
    explicit ToastGlobalConfig() = default;
    int m_maximumOnScreen{3};
    int m_spacing{10};
    int m_offsetX{20};
    int m_offsetY{45};
    bool m_alwaysOnMainScreen{false};
    QScreen *m_fixedScreen{nullptr};
    ToastPosition m_position{ToastPosition::BottomRight};
};

/**
 * @brief 配置类，用于定义单个 Toast 实例的外观和行为。
 *
 */
class ToastConfig final
{
public:
    ToastConfig() = default;

    /**
     * @brief 根据枚举值获取对应的图标 QPixmap。
     * @param enumIcon ToastIcon 枚举值。
     * @return 对应图标的 QPixmap。
     */
    static QPixmap getIconFromEnum(ToastIcon enumIcon) noexcept;

    /**
     * @brief 将给定的 QImage 的颜色重新着色为指定的颜色，同时保留原始透明度。
     * @param image 需要重新着色的 QImage。
     * @param color 新的颜色。
     * @return 重新着色后的 QImage。
     */
    static QImage recolorImage(QImage image, const QColor &color) noexcept;

    /**
     * @brief 应用预设样式到此配置。
     * @param preset 要应用的预设（例如，Success, Warning）。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setPreset(ToastPreset preset) noexcept;

    /**
     * @brief 设置 Toast 的显示持续时间。
     * @param duration 持续时间，单位为毫秒；0 表示无限持续时间。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setDuration(int duration) noexcept;

    /**
     * @brief 启用或禁用持续时间进度条的显示。
     * @param enabled true 表示显示进度条。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setShowDurationBar(bool enabled) noexcept;

    /**
     * @brief 设置标题文本。
     * @param title 标题字符串。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTitle(const QString &title) noexcept;

    /**
     * @brief 设置主要内容文本。
     * @param text 要显示的文本。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setText(const QString &text) noexcept;

    /**
     * @brief 设置自定义图标 pixmap。
     * @param icon 用作图标的 pixmap。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIcon(const QPixmap &icon) noexcept;

    /**
     * @brief 使用内置枚举设置图标。
     * @param icon 图标类型（Success, Warning 等）。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIcon(ToastIcon icon) noexcept;

    /**
     * @brief 显示或隐藏图标区域。
     * @param enabled true 表示显示图标。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setShowIcon(bool enabled) noexcept;

    /**
     * @brief 设置图标的尺寸。
     * @param size 期望的尺寸。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconSize(const QSize &size) noexcept;

    /**
     * @brief 显示或隐藏图标与文本之间的垂直分隔线。
     * @param enabled true 表示显示分隔线。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setShowIconSeparator(bool enabled) noexcept;

    /**
     * @brief 设置图标分隔线的宽度。
     * @param width 宽度，单位为像素。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconSeparatorWidth(int width) noexcept;

    /**
     * @brief 设置自定义关闭按钮图标。
     * @param icon 关闭按钮的 pixmap。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonIcon(const QPixmap &icon) noexcept;

    /**
     * @brief 使用内置枚举设置关闭按钮图标。
     * @param icon 应为 Close 图标。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonIcon(ToastIcon icon) noexcept;

    /**
     * @brief 显示或隐藏关闭按钮。
     * @param enabled true 表示显示按钮。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setShowCloseButton(bool enabled) noexcept;

    /**
     * @brief 设置关闭按钮图标的尺寸。
     * @param size 图标尺寸。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonIconSize(const QSize &size) noexcept;

    /**
     * @brief 设置关闭按钮区域的总尺寸。
     * @param size 区域尺寸。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonSize(const QSize &size) noexcept;

    /**
     * @brief 设置关闭按钮在其区域内对齐方式。
     * @param alignment Top, Middle 或 Bottom。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonAlignment(ToastButtonAlignment alignment) noexcept;

    /**
     * @brief 设置淡入动画的持续时间。
     * @param duration 持续时间，单位为毫秒。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setFadeInDuration(int duration) noexcept;

    /**
     * @brief 设置淡出动画的持续时间。
     * @param duration 持续时间，单位为毫秒。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setFadeOutDuration(int duration) noexcept;

    /**
     * @brief 当鼠标悬停在 Toast 上时重置其持续时间计时器。
     * @param enabled true 表示启用悬停时重置功能。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setResetDurationOnHover(bool enabled) noexcept;

    /**
     * @brief 使 Toast 保持在其他窗口之上。
     * @param enabled true 表示启用置顶行为。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setStayOnTop(bool enabled) noexcept;

    /**
     * @brief 设置圆角矩形的圆角半径。
     * @param borderRadius 半径，单位为像素。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setBorderRadius(int borderRadius) noexcept;

    /**
     * @brief 设置 Toast 的背景颜色。
     * @param color 背景颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setBackgroundColor(const QColor &color) noexcept;

    /**
     * @brief 设置标题文本的颜色。
     * @param color 标题文本颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTitleColor(const QColor &color) noexcept;

    /**
     * @brief 设置正文文本的颜色。
     * @param color 正文文本颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTextColor(const QColor &color) noexcept;

    /**
     * @brief 设置应用于图标的色调颜色。
     * @param color 图标色调颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconColor(const QColor &color) noexcept;

    /**
     * @brief 设置图标分隔线的颜色。
     * @param color 分隔线颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconSeparatorColor(const QColor &color) noexcept;

    /**
     * @brief 设置关闭按钮图标的颜色。
     * @param color 关闭图标颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonIconColor(const QColor &color) noexcept;

    /**
     * @brief 设置持续时间进度条的颜色。
     * @param color 进度条颜色。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setDurationBarColor(const QColor &color) noexcept;

    /**
     * @brief 设置标题的字体。
     * @param font 标题字体。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTitleFont(const QFont &font) noexcept;

    /**
     * @brief 设置正文文本的字体。
     * @param font 正文文本字体。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTextFont(const QFont &font) noexcept;

    /**
     * @brief 设置围绕整个 Toast 的外边距。
     * @param margins 边距（左、上、右、下）。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setMargins(const QMargins &margins) noexcept;

    /**
     * @brief 设置围绕图标的边距。
     * @param margins 图标专用边距。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconMargins(const QMargins &margins) noexcept;

    /**
     * @brief 设置围绕图标区域（容器）的边距。
     * @param margins 容器边距。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setIconSectionMargins(const QMargins &margins) noexcept;

    /**
     * @brief 设置围绕文本区域（容器）的边距。
     * @param margins 容器边距。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTextSectionMargins(const QMargins &margins) noexcept;

    /**
     * @brief 设置围绕关闭按钮的边距。
     * @param margins 按钮边距。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setCloseButtonMargins(const QMargins &margins) noexcept;

    /**
     * @brief 设置图标区域和文本区域之间的间距。
     * @param spacing 间距，单位为像素。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastConfig &setTextSectionSpacing(int spacing) noexcept;

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

private:
    int m_duration{5000};
    bool m_showDurationBar{true};
    QString m_title;
    QString m_text;
    QPixmap m_icon{getIconFromEnum(ToastIcon::Information)};
    bool m_showIcon{false};
    QSize m_iconSize{18, 18};
    bool m_showIconSeparator{true};
    int m_iconSeparatorWidth{2};
    QPixmap m_closeButtonIcon{getIconFromEnum(ToastIcon::Close)};
    bool m_showCloseButton{true};
    QSize m_closeButtonIconSize{10, 10};
    QSize m_closeButtonSize{24, 24};
    ToastButtonAlignment m_closeButtonAlignment{ToastButtonAlignment::Middle};
    int m_fadeInDuration = 250;
    int m_fadeOutDuration = 250;
    bool m_resetDurationOnHover{true};
    bool m_stayOnTop{true};
    int m_borderRadius{4};
    QColor m_backgroundColor{ToastGlobalConfig::DefaultBackgroundColor};
    QColor m_titleColor{ToastGlobalConfig::DefaultTitleColor};
    QColor m_textColor{ToastGlobalConfig::DefaultTextColor};
    QColor m_iconColor{ToastGlobalConfig::DefaultAccentColor};
    QColor m_iconSeparatorColor{ToastGlobalConfig::DefaultIconSeparatorColor};
    QColor m_closeButtonIconColor{ToastGlobalConfig::DefaultCloseButtonIconColor};
    QColor m_durationBarColor{ToastGlobalConfig::DefaultAccentColor};
    QFont m_titleFont{"Arial", 10, QFont::Bold};
    QFont m_textFont{"Arial", 9};
    QMargins m_margins{20, 18, 10, 18};
    QMargins m_iconMargins{0, 0, 15, 0};
    QMargins m_iconSectionMargins{0, 0, 15, 0};
    QMargins m_textSectionMargins{0, 0, 15, 0};
    QMargins m_closeButtonMargins{0, -8, 0, -8};
    int m_textSectionSpacing = 8;
};

/**
 * @brief 管理类，负责管理所有 Toast 实例的生命周期、队列和屏幕布局。
 *
 */
class ToastManager final
{
public:
    static ToastManager &instance() noexcept;

    Q_DISABLE_COPY_MOVE(ToastManager)
    ~ToastManager();

    /**
     * @brief 请求立即显示一个 Toast。如果达到最大显示数量，则将其加入队列。
     * @param toast 指向待显示 Toast 的指针。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastManager &present(Toast *toast) noexcept;

    /**
     * @brief 通知管理器某个 Toast 已完成（通常是因为用户关闭或超时），需要从显示列表中移除并处理队列中的下一个。
     * @param toast 指向已完成 Toast 的指针。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastManager &complete(Toast *toast) noexcept;

    /**
     * @brief 重新计算并更新所有正在显示的 Toast 的位置，以反映任何可能的变化（如新 Toast 出现或旧 Toast 消失）。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastManager &updateAllPositions() noexcept;

    /**
     * @brief 重置管理器状态：清除所有配置、清空显示和等待队列，并删除所有现存的 Toast。
     * @return 当前对象的引用，支持链式调用。
     */
    ToastManager &reset() noexcept;

    /**
     * @brief 获取当前总共存在的 Toast 数量（包括正在显示的和在队列中的）。
     * @return 总数量。
     */
    [[nodiscard]] int count() const noexcept;

    /**
     * @brief 获取当前正在屏幕上显示的 Toast 数量。
     * @return 可见数量。
     */
    [[nodiscard]] int visibleCount() const noexcept;

    /**
     * @brief 获取当前在等待队列中的 Toast 数量。
     * @return 队列数量。
     */
    [[nodiscard]] int queuedCount() const noexcept;

    /**
     * @brief 计算指定 Toast 相对于其位置锚点的垂直偏移量，该偏移量由其前面的 Toast 决定。
     * @param toast 指向目标 Toast 的指针。
     * @return 垂直偏移量（像素）。
     */
    [[nodiscard]] int toastOffsetY(Toast *toast) const noexcept;

    /**
     * @brief 获取在屏幕上位于指定 Toast 之前的那个 Toast。
     * @param toast 指向目标 Toast 的指针。
     * @return 指向前驱 Toast 的指针，如果没有前驱则返回 nullptr。
     */
    [[nodiscard]] Toast *getPredecessorToast(Toast *toast) const noexcept;

    /**
     * @brief 检查指定的 Toast 是否存在于管理器的显示列表或等待队列中。
     * @param toast 指向待检查 Toast 的指针。
     * @return 如果存在则返回 true。
     */
    [[nodiscard]] bool exists(const Toast *toast) const noexcept;

    /**
     * @brief 快速显示一个成功样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒），0 表示无限时长。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> success(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个警告样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> warning(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个错误样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> error(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个信息样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> information(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个深色主题-成功样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> successDark(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个深色主题-警告样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> warningDark(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个深色主题-错误样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> errorDark(
        const QString &title, const QString &text, int duration = 5000);

    /**
     * @brief 快速显示一个深色主题-信息样式的 Toast。
     * @param title Toast 的标题。
     * @param text Toast 的正文内容。
     * @param duration 消息显示的持续时间（毫秒）。
     * @return 返回新创建的 Toast 对象指针（使用 QPointer 包装以保证安全）。
     */
    [[nodiscard]] static QPointer<Toast> informationDark(
        const QString &title, const QString &text, int duration = 5000);

private:
    explicit ToastManager();
    [[nodiscard]] static Toast *createPresetToast(
        ToastPreset preset, const QString &title, const QString &text, int duration);
    std::deque<Toast *> m_currentlyShown; ///< 当前正在屏幕上的 Toast 列表
    std::deque<Toast *> m_queue;          ///< 等待显示的 Toast 队列
};

/**
 * @brief Toast 组件的核心实现类，代表一个独立的通知消息框。
 *
 */
class Toast final : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数。
     * @param parent 父级 QWidget，默认为 nullptr。
     */
    explicit Toast(QWidget *parent = nullptr);

    /**
     * @brief 设置 Toast 的配置。
     * @param config ToastConfig 常量引用。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &setConfig(const ToastConfig &config);

    /**
     * @brief 设置 Toast 的配置 (移动语义)。
     * @param config ToastConfig 右值引用。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &setConfig(ToastConfig &&config);

    /**
     * @brief 获取 Toast 的配置 (非常量)。
     * @return ToastConfig 引用。
     */
    ToastConfig &config();

    /**
     * @brief 获取 Toast 的配置 (常量)。
     * @return ToastConfig 常量引用。
     */
    const ToastConfig &config() const;

    /**
     * @brief 设置与 Toast 关联的用户数据。
     * @param data QVariant 常量引用。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &setData(const QVariant &data);

    /**
     * @brief 设置与 Toast 关联的用户数据 (移动语义)。
     * @param data QVariant 右值引用。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &setData(QVariant &&data);

    /**
     * @brief 获取与 Toast 关联的用户数据 (非常量)。
     * @return QVariant 引用。
     */
    QVariant &data();

    /**
     * @brief 获取与 Toast 关联的用户数据 (常量)。
     * @return QVariant 常量引用。
     */
    const QVariant &data() const;

    /**
     * @brief 为 Toast 添加一个自定义按钮。
     * @param text 按钮上显示的文本。
     * @param onClicked 按钮点击时执行的回调函数。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &addButton(const QString &text, std::function<void()> onClicked);

    /**
     * @brief 根据当前配置更新 Toast 的 UI 样式。
     * @return 对自身的引用，用于链式调用。
     */
    Toast &updateConfig();

    /**
     * @brief 根据当前配置更新窗口标志
     * @return 对自身的引用，用于链式调用。
     */
    Toast &updateWindowFlags();

public slots:
    /**
     * @brief 槽函数：请求显示此 Toast。会触发 ToastManager 的 present 流程。
     */
    void present();

signals:
    /**
     * @brief 当 Toast 被关闭（无论是自动还是手动）时发出的信号。
     */
    void closed();

protected:
    /**
     * @brief 重写 setVisible 以控制关闭逻辑，特别是处理再次显示的情况。
     * @param visible 是否可见。
     */
    void setVisible(bool visible) override;

    /**
     * @brief 重写 showEvent 以处理首次显示时的初始化逻辑，如启动定时器和动画。
     * @param event 显示事件。
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 重写 enterEvent 以处理鼠标进入 Toast 区域的事件，可能影响持续时间计时器。
     * @param event 鼠标进入事件。
     */
    void enterEvent(QEvent *event) override;

    /**
     * @brief 重写 leaveEvent 以处理鼠标离开 Toast 区域的事件，可能影响持续时间计时器。
     * @param event 鼠标离开事件。
     */
    void leaveEvent(QEvent *event) override;

private slots:
    /**
     * @brief 开始执行淡出动画，并在动画结束后彻底隐藏和清理 Toast。
     */
    void hideToast();

    /**
     * @brief 更新持续时间进度条的视觉宽度，以反映剩余时间。
     */
    void updateDurationBar();

private:
    /**
     * @brief 初始化所有 UI 组件，如标签、按钮等。
     */
    void setupUI();

    /**
     * @brief 构建默认的 UI 内容布局，包括图标、文本、按钮和进度条。
     */
    void setupDefaultContent();

    /**
     * @brief 执行淡入动画效果。
     */
    void fadeIn();

    /**
     * @brief 执行淡出动画效果。
     */
    void fadeOut();

    /**
     * @brief 执行飞入动画效果
     */
    void flyIn();

    /**
     * @brief 更新 Toast 在屏幕上的位置，通常伴随动画。
     */
    void updatePosition();

    /**
     * @brief 根据当前配置更新控件的内联样式表，以改变其外观。
     */
    void updateStylesheet();

    /**
     * @brief 计算 Toast 在屏幕上的最终目标位置坐标。
     * @return 目标的 QPoint。
     */
    QPoint calculatePosition();

private:
    friend class ToastManager;

    ToastConfig m_config;
    QVariant m_data;
    int m_elapsedTime = 0;
    bool m_fadingOut{false};
    bool m_used{false};

    QLabel *m_notification{nullptr};
    std::array<QWidget *, 5> m_dropShadowLayer{};
    QGraphicsOpacityEffect *m_opacityEffect{nullptr};
    QPushButton *m_closeButton{nullptr};
    QLabel *m_titleLabel{nullptr};
    QLabel *m_textLabel{nullptr};
    QPushButton *m_iconWidget{nullptr};
    QWidget *m_iconSeparator{nullptr};
    QWidget *m_durationBarContainer{nullptr};
    QWidget *m_durationBar{nullptr};
    QWidget *m_durationBarChunk{nullptr};
    QTimer *m_durationTimer{nullptr};
    QTimer *m_durationBarTimer{nullptr};
    std::vector<std::pair<QString, std::function<void()>>> m_buttons;
};
