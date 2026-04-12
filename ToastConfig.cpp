#include "ToastConfig.h"
#include "Toast.h"
#include <QImage>

ToastConfig::ToastConfig()
{
    // 设置默认值
    m_content.closeButtonAlignment = ToastButtonAlignment::Middle;
    m_appearance.backgroundColor = ToastGlobalConfig::DefaultBackgroundColor;
    m_appearance.titleColor = ToastGlobalConfig::DefaultTitleColor;
    m_appearance.textColor = ToastGlobalConfig::DefaultTextColor;
    m_appearance.iconColor = ToastGlobalConfig::DefaultAccentColor;
    m_appearance.iconSeparatorColor = ToastGlobalConfig::DefaultIconSeparatorColor;
    m_appearance.closeButtonIconColor = ToastGlobalConfig::DefaultCloseButtonIconColor;
    m_appearance.durationBarColor = ToastGlobalConfig::DefaultAccentColor;
    m_content.icon = getIconFromEnum(ToastIcon::Information);
    m_content.closeButtonIcon = getIconFromEnum(ToastIcon::Close);
}

QPixmap ToastConfig::getIconFromEnum(ToastIcon enumIcon) noexcept
{
    switch (enumIcon) {
    case ToastIcon::Success:
        return QPixmap(":/icons/success.png");
    case ToastIcon::Warning:
        return QPixmap(":/icons/warning.png");
    case ToastIcon::Error:
        return QPixmap(":/icons/error.png");
    case ToastIcon::Information:
        return QPixmap(":/icons/information.png");
    case ToastIcon::Close:
        return QPixmap(":/icons/close.png");
    }
    return QPixmap();
}

QImage ToastConfig::recolorImage(QImage image, const QColor &color) noexcept
{
    if (image.format() != QImage::Format_ARGB32) {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }
    const uchar r = color.red();
    const uchar g = color.green();
    const uchar b = color.blue();
    const int pixelCount = image.width() * image.height();
    auto *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (int i = 0; i < pixelCount; ++i) {
        const QRgb old = pixels[i];
        pixels[i] = qRgba(r, g, b, qAlpha(old));
    }
    return image;
}

ToastConfig &ToastConfig::setPreset(ToastPreset preset) noexcept
{
    // 图标和颜色
    switch (preset) {
    case ToastPreset::Success:
    case ToastPreset::SuccessDark:
        m_content.icon = getIconFromEnum(ToastIcon::Success);
        m_appearance.iconColor = ToastGlobalConfig::SuccessAccentColor;
        m_appearance.durationBarColor = ToastGlobalConfig::SuccessAccentColor;
        break;
    case ToastPreset::Warning:
    case ToastPreset::WarningDark:
        m_content.icon = getIconFromEnum(ToastIcon::Warning);
        m_appearance.iconColor = ToastGlobalConfig::WarningAccentColor;
        m_appearance.durationBarColor = ToastGlobalConfig::WarningAccentColor;
        break;
    case ToastPreset::Error:
    case ToastPreset::ErrorDark:
        m_content.icon = getIconFromEnum(ToastIcon::Error);
        m_appearance.iconColor = ToastGlobalConfig::ErrorAccentColor;
        m_appearance.durationBarColor = ToastGlobalConfig::ErrorAccentColor;
        break;
    case ToastPreset::Information:
    case ToastPreset::InformationDark:
        m_content.icon = getIconFromEnum(ToastIcon::Information);
        m_appearance.iconColor = ToastGlobalConfig::InformationAccentColor;
        m_appearance.durationBarColor = ToastGlobalConfig::InformationAccentColor;
        break;
    }

    // 主题颜色
    switch (preset) {
    case ToastPreset::Success:
    case ToastPreset::Warning:
    case ToastPreset::Error:
    case ToastPreset::Information:
        m_appearance.backgroundColor = ToastGlobalConfig::DefaultBackgroundColor;
        m_appearance.closeButtonIconColor = ToastGlobalConfig::DefaultCloseButtonIconColor;
        m_appearance.iconSeparatorColor = ToastGlobalConfig::DefaultIconSeparatorColor;
        m_appearance.titleColor = ToastGlobalConfig::DefaultTitleColor;
        m_appearance.textColor = ToastGlobalConfig::DefaultTextColor;
        break;
    case ToastPreset::SuccessDark:
    case ToastPreset::WarningDark:
    case ToastPreset::ErrorDark:
    case ToastPreset::InformationDark:
        m_appearance.backgroundColor = ToastGlobalConfig::DefaultBackgroundColorDark;
        m_appearance.closeButtonIconColor = ToastGlobalConfig::DefaultCloseButtonIconColorDark;
        m_appearance.iconSeparatorColor = ToastGlobalConfig::DefaultIconSeparatorColorDark;
        m_appearance.titleColor = ToastGlobalConfig::DefaultTitleColorDark;
        m_appearance.textColor = ToastGlobalConfig::DefaultTextColorDark;
        break;
    }

    m_behavior.showDurationBar = true;
    m_content.showIcon = true;
    m_content.showIconSeparator = true;
    m_content.iconSeparatorWidth = 2;
    return *this;
}

// ====================
// Setter 实现（全部委托到子结构体）
// ====================

ToastConfig &ToastConfig::setDuration(int duration) noexcept
{
    m_behavior.duration = duration;
    return *this;
}

ToastConfig &ToastConfig::setShowDurationBar(bool enabled) noexcept
{
    m_behavior.showDurationBar = enabled;
    return *this;
}

ToastConfig &ToastConfig::setTitle(const QString &title) noexcept
{
    m_content.title = title;
    return *this;
}

ToastConfig &ToastConfig::setText(const QString &text) noexcept
{
    m_content.text = text;
    return *this;
}

ToastConfig &ToastConfig::setIcon(const QPixmap &icon) noexcept
{
    m_content.icon = icon;
    return *this;
}

ToastConfig &ToastConfig::setIcon(ToastIcon icon) noexcept
{
    m_content.icon = getIconFromEnum(icon);
    return *this;
}

ToastConfig &ToastConfig::setShowIcon(bool enabled) noexcept
{
    m_content.showIcon = enabled;
    return *this;
}

ToastConfig &ToastConfig::setIconSize(const QSize &size) noexcept
{
    m_content.iconSize = size;
    return *this;
}

ToastConfig &ToastConfig::setShowIconSeparator(bool enabled) noexcept
{
    m_content.showIconSeparator = enabled;
    return *this;
}

ToastConfig &ToastConfig::setIconSeparatorWidth(int width) noexcept
{
    m_content.iconSeparatorWidth = width;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIcon(const QPixmap &icon) noexcept
{
    m_content.closeButtonIcon = icon;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIcon(ToastIcon icon) noexcept
{
    m_content.closeButtonIcon = getIconFromEnum(icon);
    return *this;
}

ToastConfig &ToastConfig::setShowCloseButton(bool enabled) noexcept
{
    m_content.showCloseButton = enabled;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIconSize(const QSize &size) noexcept
{
    m_content.closeButtonIconSize = size;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonSize(const QSize &size) noexcept
{
    m_content.closeButtonSize = size;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonAlignment(ToastButtonAlignment alignment) noexcept
{
    m_content.closeButtonAlignment = alignment;
    return *this;
}

ToastConfig &ToastConfig::setFadeInDuration(int duration) noexcept
{
    m_behavior.fadeInDuration = duration;
    return *this;
}

ToastConfig &ToastConfig::setFadeOutDuration(int duration) noexcept
{
    m_behavior.fadeOutDuration = duration;
    return *this;
}

ToastConfig &ToastConfig::setResetDurationOnHover(bool enabled) noexcept
{
    m_behavior.resetDurationOnHover = enabled;
    return *this;
}

ToastConfig &ToastConfig::setStayOnTop(bool enabled) noexcept
{
    m_behavior.stayOnTop = enabled;
    return *this;
}

ToastConfig &ToastConfig::setBorderRadius(int borderRadius) noexcept
{
    m_appearance.borderRadius = borderRadius;
    return *this;
}

ToastConfig &ToastConfig::setBackgroundColor(const QColor &color) noexcept
{
    m_appearance.backgroundColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTitleColor(const QColor &color) noexcept
{
    m_appearance.titleColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTextColor(const QColor &color) noexcept
{
    m_appearance.textColor = color;
    return *this;
}

ToastConfig &ToastConfig::setIconColor(const QColor &color) noexcept
{
    m_appearance.iconColor = color;
    return *this;
}

ToastConfig &ToastConfig::setIconSeparatorColor(const QColor &color) noexcept
{
    m_appearance.iconSeparatorColor = color;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIconColor(const QColor &color) noexcept
{
    m_appearance.closeButtonIconColor = color;
    return *this;
}

ToastConfig &ToastConfig::setDurationBarColor(const QColor &color) noexcept
{
    m_appearance.durationBarColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTitleFont(const QFont &font) noexcept
{
    m_appearance.titleFont = font;
    return *this;
}

ToastConfig &ToastConfig::setTextFont(const QFont &font) noexcept
{
    m_appearance.textFont = font;
    return *this;
}

ToastConfig &ToastConfig::setMargins(const QMargins &margins) noexcept
{
    m_appearance.margins = margins;
    return *this;
}

ToastConfig &ToastConfig::setIconMargins(const QMargins &margins) noexcept
{
    m_appearance.iconMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setIconSectionMargins(const QMargins &margins) noexcept
{
    m_appearance.iconSectionMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setTextSectionMargins(const QMargins &margins) noexcept
{
    m_appearance.textSectionMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonMargins(const QMargins &margins) noexcept
{
    m_appearance.closeButtonMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setTextSectionSpacing(int spacing) noexcept
{
    m_appearance.textSectionSpacing = spacing;
    return *this;
}

ToastConfig &ToastConfig::setButtonsPerRow(int count) noexcept
{
    m_behavior.buttonsPerRow = count;
    return *this;
}

// ====================
// Getter 实现
// ====================

int ToastConfig::duration() const noexcept
{
    return m_behavior.duration;
}

bool ToastConfig::showDurationBar() const noexcept
{
    return m_behavior.showDurationBar;
}

const QString &ToastConfig::text() const noexcept
{
    return m_content.text;
}

const QString &ToastConfig::title() const noexcept
{
    return m_content.title;
}

const QPixmap &ToastConfig::icon() const noexcept
{
    return m_content.icon;
}

bool ToastConfig::showIcon() const noexcept
{
    return m_content.showIcon;
}

const QSize &ToastConfig::iconSize() const noexcept
{
    return m_content.iconSize;
}

bool ToastConfig::showIconSeparator() const noexcept
{
    return m_content.showIconSeparator;
}

int ToastConfig::iconSeparatorWidth() const noexcept
{
    return m_content.iconSeparatorWidth;
}

const QPixmap &ToastConfig::closeButtonIcon() const noexcept
{
    return m_content.closeButtonIcon;
}

bool ToastConfig::showCloseButton() const noexcept
{
    return m_content.showCloseButton;
}
const QSize &ToastConfig::closeButtonIconSize() const noexcept
{
    return m_content.closeButtonIconSize;
}

const QSize &ToastConfig::closeButtonSize() const noexcept
{
    return m_content.closeButtonSize;
}

ToastButtonAlignment ToastConfig::closeButtonAlignment() const noexcept
{
    return m_content.closeButtonAlignment;
}

int ToastConfig::fadeInDuration() const noexcept
{
    return m_behavior.fadeInDuration;
}

int ToastConfig::fadeOutDuration() const noexcept
{
    return m_behavior.fadeOutDuration;
}

bool ToastConfig::resetDurationOnHover() const noexcept
{
    return m_behavior.resetDurationOnHover;
}

bool ToastConfig::stayOnTop() const noexcept
{
    return m_behavior.stayOnTop;
}

int ToastConfig::borderRadius() const noexcept
{
    return m_appearance.borderRadius;
}

const QColor &ToastConfig::backgroundColor() const noexcept
{
    return m_appearance.backgroundColor;
}

const QColor &ToastConfig::titleColor() const noexcept
{
    return m_appearance.titleColor;
}

const QColor &ToastConfig::textColor() const noexcept
{
    return m_appearance.textColor;
}

const QColor &ToastConfig::iconColor() const noexcept
{
    return m_appearance.iconColor;
}

const QColor &ToastConfig::iconSeparatorColor() const noexcept
{
    return m_appearance.iconSeparatorColor;
}

const QColor &ToastConfig::closeButtonIconColor() const noexcept
{
    return m_appearance.closeButtonIconColor;
}

const QColor &ToastConfig::durationBarColor() const noexcept
{
    return m_appearance.durationBarColor;
}

const QFont &ToastConfig::titleFont() const noexcept
{
    return m_appearance.titleFont;
}

const QFont &ToastConfig::textFont() const noexcept
{
    return m_appearance.textFont;
}

const QMargins &ToastConfig::margins() const noexcept
{
    return m_appearance.margins;
}

const QMargins &ToastConfig::iconMargins() const noexcept
{
    return m_appearance.iconMargins;
}

const QMargins &ToastConfig::iconSectionMargins() const noexcept
{
    return m_appearance.iconSectionMargins;
}

const QMargins &ToastConfig::textSectionMargins() const noexcept
{
    return m_appearance.textSectionMargins;
}

const QMargins &ToastConfig::closeButtonMargins() const noexcept
{
    return m_appearance.closeButtonMargins;
}

int ToastConfig::textSectionSpacing() const noexcept
{
    return m_appearance.textSectionSpacing;
}

int ToastConfig::buttonsPerRow() const noexcept
{
    return m_behavior.buttonsPerRow;
}
