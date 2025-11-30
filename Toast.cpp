#include "Toast.h"
#include <algorithm>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtMath>

const int ToastGlobalConfig::UpdatePositionDuration = 200;
const int ToastGlobalConfig::DurationBarUpdateInterval = 5;
const int ToastGlobalConfig::DropShadowSize = 5;
const QColor ToastGlobalConfig::SuccessAccentColor = QColor(QStringLiteral("#3e9141ff"));
const QColor ToastGlobalConfig::WarningAccentColor = QColor(QStringLiteral("#E8B849"));
const QColor ToastGlobalConfig::ErrorAccentColor = QColor(QStringLiteral("#BA2626"));
const QColor ToastGlobalConfig::InformationAccentColor = QColor(QStringLiteral("#007FFF"));
const QColor ToastGlobalConfig::DefaultAccentColor = QColor(QStringLiteral("#5C5C5C"));
const QColor ToastGlobalConfig::DefaultBackgroundColor = QColor(QStringLiteral("#E7F4F9"));
const QColor ToastGlobalConfig::DefaultTitleColor = QColor(QStringLiteral("#000000"));
const QColor ToastGlobalConfig::DefaultTextColor = QColor(QStringLiteral("#5C5C5C"));
const QColor ToastGlobalConfig::DefaultIconSeparatorColor = QColor(QStringLiteral("#D9D9D9"));
const QColor ToastGlobalConfig::DefaultCloseButtonIconColor = QColor(QStringLiteral("#000000"));
const QColor ToastGlobalConfig::DefaultBackgroundColorDark = QColor(QStringLiteral("#292929"));
const QColor ToastGlobalConfig::DefaultTitleColorDark = QColor(QStringLiteral("#FFFFFF"));
const QColor ToastGlobalConfig::DefaultTextColorDark = QColor(QStringLiteral("#D0D0D0"));
const QColor ToastGlobalConfig::DefaultIconSeparatorColorDark = QColor(QStringLiteral("#585858"));
const QColor ToastGlobalConfig::DefaultCloseButtonIconColorDark = QColor(QStringLiteral("#C9C9C9"));

ToastGlobalConfig &ToastGlobalConfig::instance() noexcept
{
    static ToastGlobalConfig s_instance;
    return s_instance;
}

int ToastGlobalConfig::maximumOnScreen() const noexcept
{
    return m_maximumOnScreen;
}

ToastGlobalConfig &ToastGlobalConfig::setMaximumOnScreen(int maximum) noexcept
{
    m_maximumOnScreen = maximum;
    return *this;
}

int ToastGlobalConfig::spacing() const noexcept
{
    return m_spacing;
}

ToastGlobalConfig &ToastGlobalConfig::setSpacing(int spacing) noexcept
{
    m_spacing = spacing;
    return *this;
}

int ToastGlobalConfig::offsetY() const noexcept
{
    return m_offsetY;
}

int ToastGlobalConfig::offsetX() const noexcept
{
    return m_offsetX;
}

ToastGlobalConfig &ToastGlobalConfig::setOffset(int x, int y) noexcept
{
    m_offsetX = x;
    m_offsetY = y;
    return *this;
}

ToastGlobalConfig &ToastGlobalConfig::setOffsetX(int offsetX) noexcept
{
    m_offsetX = offsetX;
    return *this;
}

ToastGlobalConfig &ToastGlobalConfig::setOffsetY(int offsetY) noexcept
{
    m_offsetY = offsetY;
    return *this;
}

bool ToastGlobalConfig::alwaysOnMainScreen() const noexcept
{
    return m_alwaysOnMainScreen;
}

ToastGlobalConfig &ToastGlobalConfig::setAlwaysOnMainScreen(bool enabled) noexcept
{
    m_alwaysOnMainScreen = enabled;
    return *this;
}

QScreen *ToastGlobalConfig::fixedScreen() const noexcept
{
    return m_fixedScreen;
}

ToastGlobalConfig &ToastGlobalConfig::setFixedScreen(QScreen *screen) noexcept
{
    m_fixedScreen = screen;
    return *this;
}

ToastPosition ToastGlobalConfig::position() const noexcept
{
    return m_position;
}

ToastGlobalConfig &ToastGlobalConfig::setPosition(ToastPosition position) noexcept
{
    m_position = position;
    return *this;
}

QPixmap ToastConfig::getIconFromEnum(ToastIcon enumIcon) noexcept
{
    if (enumIcon == ToastIcon::Success) {
        return QPixmap(":/icons/success.png");
    } else if (enumIcon == ToastIcon::Warning) {
        return QPixmap(":/icons/warning.png");
    } else if (enumIcon == ToastIcon::Error) {
        return QPixmap(":/icons/error.png");
    } else if (enumIcon == ToastIcon::Information) {
        return QPixmap(":/icons/information.png");
    } else if (enumIcon == ToastIcon::Close) {
        return QPixmap(":/icons/close.png");
    }
    return QPixmap();
}

QImage ToastConfig::recolorImage(QImage image, const QColor &color) noexcept
{
    // Loop through every pixel
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            // Get current color of the pixel
            QColor currentColor = image.pixelColor(x, y);

            // Replace the rgb values with rgb of new color and keep alpha the same
            QColor newColor = QColor::fromRgba(
                qRgba(color.red(), color.green(), color.blue(), currentColor.alpha()));
            image.setPixelColor(x, y, newColor);
        }
    }
    return image;
}

ToastConfig &ToastConfig::setPreset(ToastPreset preset) noexcept
{
    if (preset == ToastPreset::Success || preset == ToastPreset::SuccessDark) {
        setIcon(ToastIcon::Success);
        setIconColor(ToastGlobalConfig::SuccessAccentColor);
        setDurationBarColor(ToastGlobalConfig::SuccessAccentColor);
    } else if (preset == ToastPreset::Warning || preset == ToastPreset::WarningDark) {
        setIcon(ToastIcon::Warning);
        setIconColor(ToastGlobalConfig::WarningAccentColor);
        setDurationBarColor(ToastGlobalConfig::WarningAccentColor);
    } else if (preset == ToastPreset::Error || preset == ToastPreset::ErrorDark) {
        setIcon(ToastIcon::Error);
        setIconColor(ToastGlobalConfig::ErrorAccentColor);
        setDurationBarColor(ToastGlobalConfig::ErrorAccentColor);
    } else if (preset == ToastPreset::Information || preset == ToastPreset::InformationDark) {
        setIcon(ToastIcon::Information);
        setIconColor(ToastGlobalConfig::InformationAccentColor);
        setDurationBarColor(ToastGlobalConfig::InformationAccentColor);
    }

    if (preset == ToastPreset::Success || preset == ToastPreset::Warning
        || preset == ToastPreset::Error || preset == ToastPreset::Information) {
        setBackgroundColor(ToastGlobalConfig::DefaultBackgroundColor);
        setCloseButtonIconColor(ToastGlobalConfig::DefaultCloseButtonIconColor);
        setIconSeparatorColor(ToastGlobalConfig::DefaultIconSeparatorColor);
        setTitleColor(ToastGlobalConfig::DefaultTitleColor);
        setTextColor(ToastGlobalConfig::DefaultTextColor);
    } else if (
        preset == ToastPreset::SuccessDark || preset == ToastPreset::WarningDark
        || preset == ToastPreset::ErrorDark || preset == ToastPreset::InformationDark) {
        setBackgroundColor(ToastGlobalConfig::DefaultBackgroundColorDark);
        setCloseButtonIconColor(ToastGlobalConfig::DefaultCloseButtonIconColorDark);
        setIconSeparatorColor(ToastGlobalConfig::DefaultIconSeparatorColorDark);
        setTitleColor(ToastGlobalConfig::DefaultTitleColorDark);
        setTextColor(ToastGlobalConfig::DefaultTextColorDark);
    }
    setShowDurationBar(true);
    setShowIcon(true);
    setShowIconSeparator(true);
    setIconSeparatorWidth(2);
    return *this;
}

ToastConfig &ToastConfig::setDuration(int duration) noexcept
{
    m_duration = duration;
    return *this;
}

ToastConfig &ToastConfig::setShowDurationBar(bool enabled) noexcept
{
    m_showDurationBar = enabled;
    return *this;
}

ToastConfig &ToastConfig::setTitle(const QString &title) noexcept
{
    m_title = title;
    return *this;
}

ToastConfig &ToastConfig::setText(const QString &text) noexcept
{
    m_text = text;
    return *this;
}

ToastConfig &ToastConfig::setIcon(const QPixmap &icon) noexcept
{
    m_icon = icon;
    return *this;
}

ToastConfig &ToastConfig::setIcon(ToastIcon icon) noexcept
{
    m_icon = getIconFromEnum(icon);
    return *this;
}

ToastConfig &ToastConfig::setShowIcon(bool enabled) noexcept
{
    m_showIcon = enabled;
    return *this;
}

ToastConfig &ToastConfig::setIconSize(const QSize &size) noexcept
{
    m_iconSize = size;
    return *this;
}

ToastConfig &ToastConfig::setShowIconSeparator(bool enabled) noexcept
{
    m_showIconSeparator = enabled;
    return *this;
}

ToastConfig &ToastConfig::setIconSeparatorWidth(int width) noexcept
{
    m_iconSeparatorWidth = width;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIcon(const QPixmap &icon) noexcept
{
    m_closeButtonIcon = icon;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIcon(ToastIcon icon) noexcept
{
    m_closeButtonIcon = getIconFromEnum(icon);
    return *this;
}

ToastConfig &ToastConfig::setShowCloseButton(bool enabled) noexcept
{
    m_showCloseButton = enabled;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIconSize(const QSize &size) noexcept
{
    m_closeButtonIconSize = size;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonSize(const QSize &size) noexcept
{
    m_closeButtonSize = size;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonAlignment(ToastButtonAlignment alignment) noexcept
{
    m_closeButtonAlignment = alignment;
    return *this;
}

ToastConfig &ToastConfig::setFadeInDuration(int duration) noexcept
{
    m_fadeInDuration = duration;
    return *this;
}

ToastConfig &ToastConfig::setFadeOutDuration(int duration) noexcept
{
    m_fadeOutDuration = duration;
    return *this;
}

ToastConfig &ToastConfig::setResetDurationOnHover(bool enabled) noexcept
{
    m_resetDurationOnHover = enabled;
    return *this;
}

ToastConfig &ToastConfig::setStayOnTop(bool enabled) noexcept
{
    m_stayOnTop = enabled;
    return *this;
}

ToastConfig &ToastConfig::setBorderRadius(int borderRadius) noexcept
{
    m_borderRadius = borderRadius;
    return *this;
}

ToastConfig &ToastConfig::setBackgroundColor(const QColor &color) noexcept
{
    m_backgroundColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTitleColor(const QColor &color) noexcept
{
    m_titleColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTextColor(const QColor &color) noexcept
{
    m_textColor = color;
    return *this;
}

ToastConfig &ToastConfig::setIconColor(const QColor &color) noexcept
{
    m_iconColor = color;
    return *this;
}

ToastConfig &ToastConfig::setIconSeparatorColor(const QColor &color) noexcept
{
    m_iconSeparatorColor = color;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonIconColor(const QColor &color) noexcept
{
    m_closeButtonIconColor = color;
    return *this;
}

ToastConfig &ToastConfig::setDurationBarColor(const QColor &color) noexcept
{
    m_durationBarColor = color;
    return *this;
}

ToastConfig &ToastConfig::setTitleFont(const QFont &font) noexcept
{
    m_titleFont = font;
    return *this;
}

ToastConfig &ToastConfig::setTextFont(const QFont &font) noexcept
{
    m_textFont = font;
    return *this;
}

ToastConfig &ToastConfig::setMargins(const QMargins &margins) noexcept
{
    m_margins = margins;
    return *this;
}

ToastConfig &ToastConfig::setIconMargins(const QMargins &margins) noexcept
{
    m_iconMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setIconSectionMargins(const QMargins &margins) noexcept
{
    m_iconSectionMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setTextSectionMargins(const QMargins &margins) noexcept
{
    m_textSectionMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setCloseButtonMargins(const QMargins &margins) noexcept
{
    m_closeButtonMargins = margins;
    return *this;
}

ToastConfig &ToastConfig::setTextSectionSpacing(int spacing) noexcept
{
    m_textSectionSpacing = spacing;
    return *this;
}

int ToastConfig::duration() const noexcept
{
    return m_duration;
}

bool ToastConfig::showDurationBar() const noexcept
{
    return m_showDurationBar;
}

const QString &ToastConfig::text() const noexcept
{
    return m_text;
}

const QString &ToastConfig::title() const noexcept
{
    return m_title;
}

const QPixmap &ToastConfig::icon() const noexcept
{
    return m_icon;
}

bool ToastConfig::showIcon() const noexcept
{
    return m_showIcon;
}

const QSize &ToastConfig::iconSize() const noexcept
{
    return m_iconSize;
}

bool ToastConfig::showIconSeparator() const noexcept
{
    return m_showIconSeparator;
}

int ToastConfig::iconSeparatorWidth() const noexcept
{
    return m_iconSeparatorWidth;
}

const QPixmap &ToastConfig::closeButtonIcon() const noexcept
{
    return m_closeButtonIcon;
}

bool ToastConfig::showCloseButton() const noexcept
{
    return m_showCloseButton;
}

const QSize &ToastConfig::closeButtonIconSize() const noexcept
{
    return m_closeButtonIconSize;
}

const QSize &ToastConfig::closeButtonSize() const noexcept
{
    return m_closeButtonSize;
}

ToastButtonAlignment ToastConfig::closeButtonAlignment() const noexcept
{
    return m_closeButtonAlignment;
}

int ToastConfig::fadeInDuration() const noexcept
{
    return m_fadeInDuration;
}

int ToastConfig::fadeOutDuration() const noexcept
{
    return m_fadeOutDuration;
}

bool ToastConfig::resetDurationOnHover() const noexcept
{
    return m_resetDurationOnHover;
}

bool ToastConfig::stayOnTop() const noexcept
{
    return m_stayOnTop;
}

int ToastConfig::borderRadius() const noexcept
{
    return m_borderRadius;
}

const QColor &ToastConfig::backgroundColor() const noexcept
{
    return m_backgroundColor;
}

const QColor &ToastConfig::titleColor() const noexcept
{
    return m_titleColor;
}

const QColor &ToastConfig::textColor() const noexcept
{
    return m_textColor;
}

const QColor &ToastConfig::iconColor() const noexcept
{
    return m_iconColor;
}

const QColor &ToastConfig::iconSeparatorColor() const noexcept
{
    return m_iconSeparatorColor;
}

const QColor &ToastConfig::closeButtonIconColor() const noexcept
{
    return m_closeButtonIconColor;
}

const QColor &ToastConfig::durationBarColor() const noexcept
{
    return m_durationBarColor;
}

const QFont &ToastConfig::titleFont() const noexcept
{
    return m_titleFont;
}

const QFont &ToastConfig::textFont() const noexcept
{
    return m_textFont;
}

const QMargins &ToastConfig::margins() const noexcept
{
    return m_margins;
}

const QMargins &ToastConfig::iconMargins() const noexcept
{
    return m_iconMargins;
}

const QMargins &ToastConfig::iconSectionMargins() const noexcept
{
    return m_iconSectionMargins;
}

const QMargins &ToastConfig::textSectionMargins() const noexcept
{
    return m_textSectionMargins;
}

const QMargins &ToastConfig::closeButtonMargins() const noexcept
{
    return m_closeButtonMargins;
}

int ToastConfig::textSectionSpacing() const noexcept
{
    return m_textSectionSpacing;
}

ToastManager &ToastManager::instance() noexcept
{
    static ToastManager s_instance;
    return s_instance;
}

ToastManager::ToastManager()
{
    QObject::connect(qApp, &QGuiApplication::lastWindowClosed, []() {
        ToastManager::instance().reset();
    });
}

ToastManager::~ToastManager()
{
    reset();
}

ToastManager &ToastManager::present(Toast *toast) noexcept
{
    if (exists(toast)) {
        return *this;
    }
    auto &config = ToastGlobalConfig::instance();
    if (m_currentlyShown.size() < static_cast<size_t>(config.maximumOnScreen())) {
        m_currentlyShown.push_back(toast);
        toast->show();
        toast->flyIn();
    } else {
        m_queue.push_back(toast);
    }
    return *this;
}

ToastManager &ToastManager::complete(Toast *toast) noexcept
{
    if (toast == nullptr) {
        return *this;
    }
    auto it = std::find(m_currentlyShown.begin(), m_currentlyShown.end(), toast);
    if (it != m_currentlyShown.end()) {
        m_currentlyShown.erase(it);
        updateAllPositions();
        QTimer::singleShot(toast->config().fadeInDuration(), [this]() {
            if (!m_queue.empty()) {
                Toast *nextToast = m_queue.front();
                m_queue.pop_front();
                present(nextToast);
            }
        });
    }
    toast->deleteLater();
    return *this;
}

ToastManager &ToastManager::updateAllPositions() noexcept
{
    for (auto *toast : m_currentlyShown) {
        toast->updatePosition();
    }
    return *this;
}

ToastManager &ToastManager::reset() noexcept
{
    auto &config = ToastGlobalConfig::instance();
    config.setMaximumOnScreen(3);
    config.setSpacing(10);
    config.setOffset(20, 45);
    config.setAlwaysOnMainScreen(false);
    config.setFixedScreen(nullptr);
    config.setPosition(ToastPosition::BottomRight);

    for (auto *toast : m_currentlyShown) {
        toast->hide();
        toast->deleteLater();
    }
    for (auto *toast : m_queue) {
        toast->deleteLater();
    }
    m_currentlyShown.clear();
    m_queue.clear();
    return *this;
}

int ToastManager::count() const noexcept
{
    return static_cast<int>(m_currentlyShown.size() + m_queue.size());
}

int ToastManager::visibleCount() const noexcept
{
    return static_cast<int>(m_currentlyShown.size());
}

int ToastManager::queuedCount() const noexcept
{
    return static_cast<int>(m_queue.size());
}

int ToastManager::toastOffsetY(Toast *toast) const noexcept
{
    if (toast == nullptr) {
        return 0;
    }
    int offset = 0;
    for (auto *t : m_currentlyShown) {
        if (t == toast) {
            break;
        }
        offset += t->m_notification->height() + ToastGlobalConfig::instance().spacing();
    }
    return offset;
}

Toast *ToastManager::getPredecessorToast(Toast *toast) const noexcept
{
    if (toast == nullptr) {
        return nullptr;
    }
    Toast *prev = nullptr;
    for (auto *t : m_currentlyShown) {
        if (t == toast) {
            return prev;
        }
        prev = t;
    }
    return prev;
}

bool ToastManager::exists(const Toast *toast) const noexcept
{
    if (toast == nullptr) {
        return false;
    }
    return (std::find(m_currentlyShown.begin(), m_currentlyShown.end(), toast)
            != m_currentlyShown.end())
           || (std::find(m_queue.begin(), m_queue.end(), toast) != m_queue.end());
}

QPointer<Toast> ToastManager::success(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Success, title, text, duration);
}

QPointer<Toast> ToastManager::warning(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Warning, title, text, duration);
}

QPointer<Toast> ToastManager::error(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Error, title, text, duration);
}

QPointer<Toast> ToastManager::information(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Information, title, text, duration);
}

QPointer<Toast> ToastManager::successDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::SuccessDark, title, text, duration);
}

QPointer<Toast> ToastManager::warningDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::WarningDark, title, text, duration);
}

QPointer<Toast> ToastManager::errorDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::ErrorDark, title, text, duration);
}

QPointer<Toast> ToastManager::informationDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::InformationDark, title, text, duration);
}

Toast *ToastManager::createPresetToast(
    ToastPreset preset, const QString &title, const QString &text, int duration)
{
    try {
        auto toast = std::make_unique<Toast>(nullptr);
        toast->config().setPreset(preset).setTitle(title).setText(text).setDuration(duration);
        return toast.release();
    } catch (const std::exception &e) {
        qCritical() << "Failed to create preset Toast:" << e.what();
        return nullptr;
    } catch (...) {
        qCritical() << "Failed to create preset Toast: Unknown exception";
        return nullptr;
    }
}

Toast::Toast(QWidget *parent)
    : QDialog(parent)
{
    // Notification widget (QLabel because QWidget has weird behaviour with stylesheets)
    m_notification = new QLabel(this);
    // Drop shadow (has to be drawn manually since only one graphics effect can be applied)
    for (size_t i = 0; i < m_dropShadowLayer.size(); i++) {
        m_dropShadowLayer[i] = new QWidget(this);
        m_dropShadowLayer[i]->setObjectName(QString("toast-drop-shadow-layer-%1").arg(i + 1));
    }
    // Opacity effect for fading animations
    m_opacityEffect = new QGraphicsOpacityEffect();
    m_opacityEffect->setOpacity(1);
    // Close button
    m_closeButton = new QPushButton(m_notification);
    m_closeButton->setCursor(Qt::CursorShape::PointingHandCursor);
    m_closeButton->setObjectName("toast-close-button");
    connect(m_closeButton, &QPushButton::clicked, this, &Toast::hide);
    // Text and title labels
    m_titleLabel = new QLabel(m_notification);
    m_textLabel = new QLabel(m_notification);
    // Icon (QPushButton instead of QLabel to get better icon quality)
    m_iconWidget = new QPushButton(m_notification);
    m_iconWidget->setObjectName("toast-icon-widget");
    // Icon separator
    m_iconSeparator = new QWidget(m_notification);
    m_iconSeparator->setFixedWidth(2);
    // Duration bar container (used to make border radius possible on 4 px high widget)
    m_durationBarContainer = new QWidget(m_notification);
    m_durationBarContainer->setFixedHeight(4);
    m_durationBarContainer->setStyleSheet("background: transparent;");
    // Duration bar
    m_durationBar = new QWidget(m_durationBarContainer);
    m_durationBar->setFixedHeight(20);
    m_durationBar->move(0, -16);
    // Duration bar chunk
    m_durationBarChunk = new QWidget(m_durationBarContainer);
    m_durationBarChunk->setFixedHeight(20);
    m_durationBarChunk->move(0, -16);
    // Timer for hiding the notification after set duration
    m_durationTimer = new QTimer(this);
    m_durationTimer->setSingleShot(true);
    connect(m_durationTimer, &QTimer::timeout, this, &Toast::fadeOut);
    // Timer for updating the duration bar
    m_durationBarTimer = new QTimer(this);
    connect(m_durationBarTimer, &QTimer::timeout, this, &Toast::updateDurationBar);
    // Window settings
    setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
    setGraphicsEffect(m_opacityEffect);
    // Set defaults
    updateConfig();
    // Apply stylesheet
    QFile file(":/css/toast.css");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());
}

Toast &Toast::setConfig(const ToastConfig &config)
{
    m_config = config;
    return *this;
}

Toast &Toast::setConfig(ToastConfig &&config)
{
    m_config = std::move(config);
    return *this;
}

ToastConfig &Toast::config()
{
    return m_config;
}

const ToastConfig &Toast::config() const
{
    return m_config;
}

Toast &Toast::setData(const QVariant &data)
{
    m_data = data;
    return *this;
}

Toast &Toast::setData(QVariant &&data)
{
    m_data = std::move(data);
    return *this;
}

QVariant &Toast::data()
{
    return m_data;
}

const QVariant &Toast::data() const
{
    return m_data;
}

Toast &Toast::addButton(const QString &text, std::function<void()> onClicked)
{
    m_buttons.push_back({text, onClicked});
    return *this;
}

Toast &Toast::updateConfig()
{
    auto formatIcon = [](const QPixmap &pm, const QSize &size, const QColor &color) -> QIcon {
        QIcon icon(pm);
        auto iconImage = ToastConfig::recolorImage(icon.pixmap(size).toImage(), color);
        icon = QIcon(QPixmap::fromImage(iconImage));
        return icon;
    };

    updateWindowFlags();

    // icon
    {
        auto icon = formatIcon(m_config.icon(), m_config.iconSize(), m_config.iconColor());
        m_iconWidget->setIconSize(m_config.iconSize());
        m_iconWidget->setIcon(icon);
        m_iconWidget->setFixedSize(m_config.iconSize());
        if (m_config.showIconSeparator()) {
            m_iconSeparator->setFixedWidth(m_config.iconSeparatorWidth());
        }
    }

    // close button
    {
        auto icon = formatIcon(
            m_config.closeButtonIcon(),
            m_config.closeButtonIconSize(),
            m_config.closeButtonIconColor());
        m_closeButton->setIconSize(m_config.closeButtonIconSize());
        m_closeButton->setIcon(icon);
        m_iconWidget->setFixedSize(m_config.closeButtonSize());
    }

    // text
    {
        m_titleLabel->setFont(m_config.titleFont());
        m_titleLabel->setText(m_config.title());
        m_textLabel->setFont(m_config.textFont());
        m_textLabel->setText(m_config.text());
    }

    updateStylesheet();

    return *this;
}

Toast &Toast::updateWindowFlags()
{
    if (m_used || isVisible()) {
        return *this;
    }
    if (m_config.stayOnTop()) {
        if (parentWidget()) {
            setWindowFlags(
                Qt::WindowType::Window | Qt::WindowType::CustomizeWindowHint
                | Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
        } else {
            setWindowFlags(
                Qt::WindowType::Tool | Qt::WindowType::CustomizeWindowHint
                | Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
        }
    } else {
        if (parentWidget()) {
            setWindowFlags(
                Qt::WindowType::Window | Qt::WindowType::CustomizeWindowHint
                | Qt::WindowType::FramelessWindowHint);
        } else {
            setWindowFlags(
                Qt::WindowType::Tool | Qt::WindowType::CustomizeWindowHint
                | Qt::WindowType::FramelessWindowHint);
        }
    }
    return *this;
}

void Toast::present()
{
    ToastManager::instance().present(this);
}

void Toast::setVisible(bool visible)
{
    if (!visible && m_used && !m_fadingOut) {
        if (m_config.duration() != 0) {
            m_durationTimer->stop();
        }
        fadeOut();
        return;
    }
    QDialog::setVisible(visible);
}

void Toast::showEvent(QShowEvent *event)
{
    if (m_used) {
        QDialog::showEvent(event);
        return;
    }
    m_used = true;

    setupUI();
    if (m_config.duration() != 0) {
        m_durationTimer->start(m_config.duration());
    }
    if (m_config.duration() != 0 && m_config.showDurationBar()) {
        m_durationBarTimer->start(ToastGlobalConfig::DurationBarUpdateInterval);
    }

    fadeIn();

    if (parentWidget()) {
        parentWidget()->activateWindow();
    }

    QDialog::showEvent(event);
}

void Toast::enterEvent(QEvent *event)
{
    if (m_config.duration() != 0 && m_durationTimer->isActive() && m_config.resetDurationOnHover()) {
        m_durationTimer->stop();
        if (m_config.showDurationBar()) {
            m_durationBarTimer->stop();
            m_durationBarChunk->setFixedWidth(m_notification->width());
            m_elapsedTime = 0;
        }
    }
    QDialog::enterEvent(event);
}

void Toast::leaveEvent(QEvent *event)
{
    if (m_config.duration() != 0 && !m_durationTimer->isActive()
        && m_config.resetDurationOnHover()) {
        m_durationTimer->start(m_config.duration());
        if (m_config.showDurationBar()) {
            m_durationBarTimer->start(ToastGlobalConfig::DurationBarUpdateInterval);
        }
    }
    QDialog::leaveEvent(event);
}

void Toast::hideToast()
{
    close();
    m_elapsedTime = 0;
    m_fadingOut = false;
    m_used = false;
    emit closed();
    ToastManager::instance().complete(this);
}

void Toast::updateDurationBar()
{
    m_elapsedTime += ToastGlobalConfig::DurationBarUpdateInterval;
    if (m_elapsedTime >= m_config.duration()) {
        m_durationBarTimer->stop();
        return;
    }
    double newChunkWidth = floor(
        m_durationBarContainer->width()
        - (static_cast<double>(m_elapsedTime) / m_config.duration())
              * m_durationBarContainer->width());
    m_durationBarChunk->setFixedWidth(static_cast<int>(newChunkWidth));
}

void Toast::setupUI()
{
    setupDefaultContent();
}

void Toast::setupDefaultContent()
{
    // 更新样式
    updateConfig();

    // ========================
    // 布局构建 (Layout Construction)
    // ========================
    auto setupLayout = [this]() -> void {
        QVBoxLayout *rootLayout = new QVBoxLayout();
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);
        m_notification->setLayout(rootLayout);

        QHBoxLayout *mainLayout = new QHBoxLayout();
        mainLayout->setContentsMargins(m_config.margins());
        mainLayout->setSpacing(0);

        // 图标区域
        if (m_config.showIcon()) {
            QHBoxLayout *iconLayout = new QHBoxLayout();
            iconLayout->setContentsMargins(m_config.iconSectionMargins());

            QVBoxLayout *iconInnerLayout = new QVBoxLayout();
            iconInnerLayout->setContentsMargins(m_config.iconMargins());
            iconInnerLayout->addWidget(m_iconWidget);
            iconLayout->addLayout(iconInnerLayout);

            if (m_config.showIconSeparator()) {
                m_iconSeparator->setFixedWidth(m_config.iconSeparatorWidth());
                iconLayout->addWidget(m_iconSeparator);
            }

            mainLayout->addLayout(iconLayout);
        } else {
            m_iconWidget->setVisible(false);
            m_iconSeparator->setVisible(false);
        }

        // 内容区域
        QVBoxLayout *contentLayout = new QVBoxLayout();
        contentLayout->setContentsMargins(m_config.textSectionMargins());
        const bool hasTitleAndText = !m_config.title().isEmpty() && !m_config.text().isEmpty();
        contentLayout->setSpacing(hasTitleAndText ? m_config.textSectionSpacing() : 0);
        contentLayout->addWidget(m_titleLabel);
        contentLayout->addWidget(m_textLabel);
        contentLayout->addStretch();

        QWidget *contentWidget = new QWidget();
        contentWidget->setLayout(contentLayout);
        mainLayout->addWidget(contentWidget);

        // 按钮区域
        if (!m_buttons.empty()) {
            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->setSpacing(10);

            QFontMetrics fm(m_config.textFont());
            for (const auto &buttonInfo : m_buttons) {
                QPushButton *button = new QPushButton(buttonInfo.first, m_notification);
                button->setFont(m_config.textFont());

                // 动态计算按钮尺寸
                int textWidth = fm.horizontalAdvance(buttonInfo.first);
                int buttonWidth = textWidth + 24;
                int buttonHeight = fm.height() + 8;
                button->setMinimumSize(buttonWidth, buttonHeight);
                button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

                // 设置按钮样式
                button->setStyleSheet(QString(
                                          "QPushButton {"
                                          "    background-color: %1;"
                                          "    color: %2;"
                                          "    border: 1px solid %3;"
                                          "    border-radius: 4px;"
                                          "    padding: 4px 12px;"
                                          "    font-family: %4;"
                                          "    font-size: %5pt;"
                                          "}"
                                          "QPushButton:hover {"
                                          "    background-color: %6;"
                                          "}"
                                          "QPushButton:pressed {"
                                          "    background-color: %7;"
                                          "}")
                                          .arg(m_config.backgroundColor().darker(120).name())
                                          .arg(m_config.textColor().name())
                                          .arg(m_config.backgroundColor().darker(150).name())
                                          .arg(m_config.textFont().family())
                                          .arg(m_config.textFont().pointSize())
                                          .arg(m_config.backgroundColor().darker(130).name())
                                          .arg(m_config.backgroundColor().darker(160).name()));

                if (buttonInfo.second) {
                    QObject::connect(button, &QPushButton::clicked, buttonInfo.second);
                }
                buttonLayout->addWidget(button);
            }
            buttonLayout->addStretch();
            contentLayout->addLayout(buttonLayout);
        }

        // 关闭按钮
        if (m_config.showCloseButton()) {
            Qt::Alignment alignment = Qt::AlignTop;
            switch (m_config.closeButtonAlignment()) {
            case ToastButtonAlignment::Middle:
                alignment = Qt::AlignVCenter;
                break;
            case ToastButtonAlignment::Bottom:
                alignment = Qt::AlignBottom;
                break;
            default:
                break;
            }
            mainLayout->addWidget(m_closeButton, 0, alignment);
        } else {
            m_closeButton->setVisible(false);
        }

        rootLayout->addLayout(mainLayout);

        // 持续时间条
        if (m_config.showDurationBar()) {
            m_durationBarContainer->setFixedHeight(4);
            QHBoxLayout *durationLayout = new QHBoxLayout(m_durationBarContainer);
            durationLayout->setContentsMargins(0, 0, 0, 0);
            durationLayout->addWidget(m_durationBar);
            m_durationBar->setFixedHeight(4);
            rootLayout->addWidget(m_durationBarContainer);
        } else {
            m_durationBarContainer->setVisible(false);
        }
    };

    // ========================
    // 大小处理
    // ========================
    auto setupGeometry = [this]() -> void {
        // 计算文本尺寸（支持多行）
        QFontMetrics titleFM(m_config.titleFont());
        QFontMetrics textFM(m_config.textFont());

        int titleWidth = 0, titleHeight = 0;
        if (!m_config.title().isEmpty()) {
            QRect rect = titleFM.boundingRect(
                QRect(0, 0, 500, INT_MAX), Qt::TextWordWrap | Qt::AlignLeft, m_config.title());
            titleWidth = rect.width() + 1;
            titleHeight = rect.height();
        }

        int textWidth = 0, textHeight = 0;
        if (!m_config.text().isEmpty()) {
            QRect rect = textFM.boundingRect(
                QRect(0, 0, 500, INT_MAX), Qt::TextWordWrap | Qt::AlignLeft, m_config.text());
            textWidth = rect.width() + 1;
            textHeight = rect.height();
        }

        int maxWidth = std::max(titleWidth, textWidth);
        int contentHeight = titleHeight
                            + (!m_config.title().isEmpty() && !m_config.text().isEmpty()
                                   ? m_config.textSectionSpacing()
                                   : 0)
                            + textHeight;

        // 最小尺寸
        int minWidth = m_config.margins().left() + (m_config.showIcon() ? 50 : 0) + maxWidth
                       + m_config.margins().right() + (m_config.showCloseButton() ? 30 : 0);

        int minHeight = m_config.margins().top() + contentHeight + m_config.margins().bottom();
        if (!m_buttons.empty()) {
            minHeight += 35; // 按钮区域高度估算
        }
        if (m_config.showDurationBar()) {
            minHeight += 4;
        }

        m_notification->setMinimumSize(minWidth, minHeight);
        m_notification->setMaximumSize(maximumWidth(), maximumHeight());
        m_notification->adjustSize(); // 调整到合适大小

        // 阴影层尺寸与位置
        int totalWidth = m_notification->width() + ToastGlobalConfig::DropShadowSize * 2;
        int totalHeight = m_notification->height() + ToastGlobalConfig::DropShadowSize * 2;

        for (size_t i = 0; i < m_dropShadowLayer.size(); i++) {
            m_dropShadowLayer.at(i)->resize(totalWidth - i * 2, totalHeight - i * 2);
            m_dropShadowLayer.at(i)->move(i, i);
        }

        QDialog::setFixedSize(totalWidth, totalHeight);
        m_notification->move(ToastGlobalConfig::DropShadowSize, ToastGlobalConfig::DropShadowSize);
        m_notification->raise();

        // 持续时间条适配宽度
        if (m_config.showDurationBar()) {
            int notificationWidth = m_notification->width();
            m_durationBarContainer->setFixedWidth(notificationWidth);
            m_durationBarContainer->move(0, m_notification->height());
            m_durationBar->setFixedWidth(notificationWidth);
            m_durationBarChunk->setFixedWidth(notificationWidth);
        }
    };

    setupLayout();             // 构建 UI 布局
    setupGeometry();           // 计算尺寸并设置几何属性
    move(calculatePosition()); // 定位窗口
}

void Toast::fadeIn()
{
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeInAnimation->setDuration(m_config.fadeInDuration());
    fadeInAnimation->setStartValue(0);
    fadeInAnimation->setEndValue(1);
    fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Toast::fadeOut()
{
    if (m_fadingOut) {
        return;
    }
    m_fadingOut = true;
    QPropertyAnimation *fadeOutAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
    connect(fadeOutAnimation, &QPropertyAnimation::finished, this, &Toast::hideToast);
    fadeOutAnimation->setDuration(m_config.fadeOutDuration());
    fadeOutAnimation->setStartValue(1);
    fadeOutAnimation->setEndValue(0);
    fadeOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Toast::flyIn()
{
    auto position = calculatePosition();
    // If not first toast on screen, also do a fade down / up animation
    if (ToastManager::instance().visibleCount() > 1) {
        // Calculate offset if predecessor toast is still in fade down / up animation
        auto *predecessorToast = ToastManager::instance().getPredecessorToast(this);
        QPoint predecessorTarget = predecessorToast->calculatePosition();
        int predecessorTargetDifferenceY = abs(predecessorToast->y() - predecessorTarget.y());

        // Calculate start position of fade down / up animation based on position
        auto gPosition = ToastGlobalConfig::instance().position();
        if (gPosition == ToastPosition::BottomRight || gPosition == ToastPosition::BottomLeft
            || gPosition == ToastPosition::BottomMiddle) {
            move(
                position.x(),
                position.y() - static_cast<int>(height() / 1.5) - predecessorTargetDifferenceY);
        } else if (
            gPosition == ToastPosition::TopRight || gPosition == ToastPosition::TopLeft
            || gPosition == ToastPosition::TopMiddle || gPosition == ToastPosition::Center) {
            move(
                position.x(),
                position.y() + static_cast<int>(height() / 1.5) + predecessorTargetDifferenceY);
        }

        // Start fade down / up animation
        QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos");
        posAnimation->setEndValue(QPoint(position.x(), position.y()));
        posAnimation->setDuration(m_config.fadeInDuration());
        posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        move(position);
    }
}

void Toast::updatePosition()
{
    QPoint position = calculatePosition();
    QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos");
    posAnimation->setEndValue(position);
    posAnimation->setDuration(ToastGlobalConfig::UpdatePositionDuration);
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Toast::updateStylesheet()
{
    m_notification->setStyleSheet(QString(
                                      "background: %1;"
                                      "border-radius: %2px; ")
                                      .arg(m_config.backgroundColor().name())
                                      .arg(m_config.borderRadius()));

    m_durationBar->setStyleSheet(QString(
                                     "background: rgba(%1, %2, %3, 100);"
                                     "border-radius: %4px;")
                                     .arg(m_config.durationBarColor().red())
                                     .arg(m_config.durationBarColor().green())
                                     .arg(m_config.durationBarColor().blue())
                                     .arg(m_config.borderRadius()));

    m_durationBarChunk->setStyleSheet(
        QString(
            "background: rgba(%1, %2, %3, 255);"
            "border-bottom-left-radius: %4px; border-bottom-right-radius: %5px;")
            .arg(m_config.durationBarColor().red())
            .arg(m_config.durationBarColor().green())
            .arg(m_config.durationBarColor().blue())
            .arg(m_config.borderRadius())
            .arg(m_config.duration() == 0 ? m_config.borderRadius() : 0));

    m_iconSeparator->setStyleSheet(
        QString("background: %1;").arg(m_config.iconSeparatorColor().name()));

    m_titleLabel->setStyleSheet(QString("color: %1;").arg(m_config.titleColor().name()));
    m_textLabel->setStyleSheet(QString("color: %1;").arg(m_config.textColor().name()));
}

QPoint Toast::calculatePosition()
{
    int offsetY = ToastManager::instance().toastOffsetY(this);

    // Get screen
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QScreen *currentScreen = nullptr;

    if (ToastGlobalConfig::instance().fixedScreen()) {
        currentScreen = ToastGlobalConfig::instance().fixedScreen();
    } else if (ToastGlobalConfig::instance().alwaysOnMainScreen() || !parentWidget()) {
        currentScreen = primaryScreen;
    } else {
        QList<QScreen *> screens = QGuiApplication::screens();

        for (auto *&screen : screens) {
            if (parentWidget()->geometry().intersects(screen->geometry())) {
                if (!currentScreen) {
                    currentScreen = screen;
                } else {
                    currentScreen = primaryScreen;
                    break;
                }
            }
        }
    }

    if (currentScreen == nullptr) {
        return QPoint();
    }

    int x = 0;
    int y = 0;

    auto gPosition = ToastGlobalConfig::instance().position();
    auto gOffsetX = ToastGlobalConfig::instance().offsetX();
    auto gOffsetY = ToastGlobalConfig::instance().offsetY();
    if (gPosition == ToastPosition::BottomRight) {
        x = currentScreen->geometry().width() - m_notification->width() - gOffsetX
            + currentScreen->geometry().x();
        y = currentScreen->geometry().height() - m_notification->height() - gOffsetY
            + currentScreen->geometry().y() - offsetY;
    } else if (gPosition == ToastPosition::BottomLeft) {
        x = currentScreen->geometry().x() + gOffsetX;
        y = currentScreen->geometry().height() - m_notification->height() - gOffsetY
            + currentScreen->geometry().y() - offsetY;
    } else if (gPosition == ToastPosition::BottomMiddle) {
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2
            - m_notification->width() / 2);
        y = currentScreen->geometry().height() - m_notification->height() - gOffsetY
            + currentScreen->geometry().y() - offsetY;
    } else if (gPosition == ToastPosition::TopRight) {
        x = currentScreen->geometry().width() - m_notification->width() - gOffsetX
            + currentScreen->geometry().x();
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
    } else if (gPosition == ToastPosition::TopLeft) {
        x = currentScreen->geometry().x() + gOffsetX;
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
    } else if (gPosition == ToastPosition::TopMiddle) {
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2
            - m_notification->width() / 2);
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
    } else if (gPosition == ToastPosition::Center) {
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2
            - m_notification->width() / 2);
        y = static_cast<int>(
            currentScreen->geometry().y() + currentScreen->geometry().height() / 2
            - m_notification->height() / 2 + offsetY);
    }

    x -= ToastGlobalConfig::DropShadowSize;
    y -= ToastGlobalConfig::DropShadowSize;

    return QPoint(x, y);
}
