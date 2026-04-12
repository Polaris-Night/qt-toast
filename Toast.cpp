#include "Toast.h"
#include "ToastAnimator.h"
#include "ToastPrivate.h"
#include <algorithm>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

// ====================
// ToastGlobalConfig 实现
// ====================

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

int ToastGlobalConfig::buttonsPerRow() const noexcept
{
    return m_buttonsPerRow;
}

ToastGlobalConfig &ToastGlobalConfig::setButtonsPerRow(int count) noexcept
{
    m_buttonsPerRow = count;
    return *this;
}

// ====================
// ToastManager 实现
// ====================

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
    if (toast == nullptr || exists(toast)) {
        return *this;
    }
    // Clean up destroyed QPointers
    m_currentlyShown.erase(
        std::remove_if(
            m_currentlyShown.begin(),
            m_currentlyShown.end(),
            [](const QPointer<Toast> &t) { return t.isNull(); }),
        m_currentlyShown.end());
    m_queue.erase(
        std::remove_if(
            m_queue.begin(), m_queue.end(), [](const QPointer<Toast> &t) { return t.isNull(); }),
        m_queue.end());

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
        QTimer::singleShot(ToastGlobalConfig::instance().spacing(), [this]() {
            m_queue.erase(
                std::remove_if(
                    m_queue.begin(),
                    m_queue.end(),
                    [](const QPointer<Toast> &t) { return t.isNull(); }),
                m_queue.end());
            if (!m_queue.empty()) {
                Toast *nextToast = m_queue.front().data();
                m_queue.pop_front();
                if (nextToast) {
                    present(nextToast);
                }
            }
        });
    }
    if (toast) {
        toast->deleteLater();
    }
    return *this;
}

ToastManager &ToastManager::updateAllPositions() noexcept
{
    for (const auto &toast : m_currentlyShown) {
        if (toast) {
            toast->updatePosition();
        }
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

    for (const auto &toast : m_currentlyShown) {
        if (toast) {
            toast->hide();
            toast->deleteLater();
        }
    }
    for (const auto &toast : m_queue) {
        if (toast) {
            toast->deleteLater();
        }
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
    if (toast == nullptr)
        return 0;
    int offset = 0;
    for (const auto &t : m_currentlyShown) {
        if (t.data() == toast)
            break;
        if (t) {
            offset += t->notificationHeight() + ToastGlobalConfig::instance().spacing();
        }
    }
    return offset;
}

Toast *ToastManager::getPredecessorToast(Toast *toast) const noexcept
{
    if (toast == nullptr) {
        return nullptr;
    }
    Toast *prev = nullptr;
    for (const auto &t : m_currentlyShown) {
        if (t.data() == toast) {
            return prev;
        }
        prev = t.data();
    }
    return prev;
}

bool ToastManager::exists(const Toast *toast) const noexcept
{
    if (toast == nullptr) {
        return false;
    }
    return (std::find_if(
                m_currentlyShown.begin(),
                m_currentlyShown.end(),
                [toast](const QPointer<Toast> &t) { return t.data() == toast; })
            != m_currentlyShown.end())
           || (std::find_if(
                   m_queue.begin(),
                   m_queue.end(),
                   [toast](const QPointer<Toast> &t) { return t.data() == toast; })
               != m_queue.end());
}

QPointer<Toast> ToastManager::success(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Success, title, text, duration).release();
}

QPointer<Toast> ToastManager::warning(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Warning, title, text, duration).release();
}

QPointer<Toast> ToastManager::error(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Error, title, text, duration).release();
}

QPointer<Toast> ToastManager::information(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::Information, title, text, duration).release();
}

QPointer<Toast> ToastManager::successDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::SuccessDark, title, text, duration).release();
}

QPointer<Toast> ToastManager::warningDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::WarningDark, title, text, duration).release();
}

QPointer<Toast> ToastManager::errorDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::ErrorDark, title, text, duration).release();
}

QPointer<Toast> ToastManager::informationDark(const QString &title, const QString &text, int duration)
{
    return createPresetToast(ToastPreset::InformationDark, title, text, duration).release();
}

std::unique_ptr<Toast> ToastManager::createPresetToast(
    ToastPreset preset, const QString &title, const QString &text, int duration)
{
    try {
        auto toast = std::make_unique<Toast>(nullptr);
        toast->config().setPreset(preset).setTitle(title).setText(text).setDuration(duration);
        return toast;
    } catch (...) {
        qCritical() << "Failed to create preset Toast";
        return nullptr;
    }
}

// ====================
// Toast 实现 (PIMPL 委托)
// ====================

Toast::Toast(QWidget *parent)
    : QDialog(parent)
    , d_ptr(new ToastPrivate(this))
{
    Q_D(Toast);
    updateConfig();
}

Toast::~Toast() = default;

Toast &Toast::setConfig(const ToastConfig &config)
{
    Q_D(Toast);
    d->m_config = config;
    return *this;
}

Toast &Toast::setConfig(ToastConfig &&config)
{
    Q_D(Toast);
    d->m_config = std::move(config);
    return *this;
}

ToastConfig &Toast::config()
{
    Q_D(Toast);
    return d->m_config;
}

const ToastConfig &Toast::config() const
{
    Q_D(const Toast);
    return d->m_config;
}

Toast &Toast::setData(const QVariant &data)
{
    Q_D(Toast);
    d->m_data = data;
    return *this;
}

Toast &Toast::setData(QVariant &&data)
{
    Q_D(Toast);
    d->m_data = std::move(data);
    return *this;
}

QVariant &Toast::data()
{
    Q_D(Toast);
    return d->m_data;
}

const QVariant &Toast::data() const
{
    Q_D(const Toast);
    return d->m_data;
}

Toast &Toast::addButton(const QString &text, std::function<void()> onClicked)
{
    Q_D(Toast);
    d->m_buttons.push_back({text, onClicked});
    return *this;
}

int Toast::notificationHeight() const noexcept
{
    Q_D(const Toast);
    return d->m_notification ? d->m_notification->height() : 0;
}

void Toast::flyIn()
{
    Q_D(Toast);
    d->m_animator->flyIn();
}

Toast &Toast::updateConfig()
{
    Q_D(Toast);
    auto formatIcon = [](const QPixmap &pm, const QSize &size, const QColor &color) -> QIcon {
        QIcon icon(pm);
        auto iconImage = ToastConfig::recolorImage(icon.pixmap(size).toImage(), color);
        icon = QIcon(QPixmap::fromImage(iconImage));
        return icon;
    };
    updateWindowFlags();

    auto icon = formatIcon(d->m_config.icon(), d->m_config.iconSize(), d->m_config.iconColor());
    d->m_iconWidget->setIconSize(d->m_config.iconSize());
    d->m_iconWidget->setIcon(icon);
    d->m_iconWidget->setFixedSize(d->m_config.iconSize());
    if (d->m_config.showIconSeparator()) {
        d->m_iconSeparator->setFixedWidth(d->m_config.iconSeparatorWidth());
    }

    auto closeIcon = formatIcon(
        d->m_config.closeButtonIcon(),
        d->m_config.closeButtonIconSize(),
        d->m_config.closeButtonIconColor());
    d->m_closeButton->setIconSize(d->m_config.closeButtonIconSize());
    d->m_closeButton->setIcon(closeIcon);

    d->m_titleLabel->setFont(d->m_config.titleFont());
    d->m_titleLabel->setText(d->m_config.title());
    d->m_textLabel->setFont(d->m_config.textFont());
    d->m_textLabel->setText(d->m_config.text());

    d->updateStylesheet();
    return *this;
}

Toast &Toast::updateWindowFlags()
{
    Q_D(Toast);
    if (d->m_used || isVisible())
        return *this;
    if (d->m_config.stayOnTop()) {
        if (parentWidget()) {
            setWindowFlags(
                Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint
                | Qt::WindowStaysOnTopHint);
        } else {
            setWindowFlags(
                Qt::Tool | Qt::CustomizeWindowHint | Qt::FramelessWindowHint
                | Qt::WindowStaysOnTopHint);
        }
    } else {
        if (parentWidget()) {
            setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        } else {
            setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
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
    Q_D(Toast);
    if (!visible && d->m_used && !d->m_fadingOut) {
        // 外部调用 hide/close 时，走 hideToast 流程触发淡出动画
        d->hideToast();
        return;
    }
    QDialog::setVisible(visible);
}

void Toast::showEvent(QShowEvent *event)
{
    Q_D(Toast);
    if (d->m_used) {
        QDialog::showEvent(event);
        return;
    }
    d->m_used = true;
    d->setupUI();
    if (d->m_config.duration() != 0)
        d->m_durationTimer->start(d->m_config.duration());
    if (d->m_config.duration() != 0 && d->m_config.showDurationBar())
        d->m_durationBarTimer->start(ToastGlobalConfig::DurationBarUpdateInterval);
    d->m_animator->fadeIn();
    if (parentWidget())
        parentWidget()->activateWindow();
    QDialog::showEvent(event);
}

void Toast::enterEvent(QEvent *event)
{
    Q_D(Toast);
    if (d->m_config.duration() != 0 && d->m_durationTimer->isActive()
        && d->m_config.resetDurationOnHover()) {
        d->m_durationTimer->stop();
        if (d->m_config.showDurationBar()) {
            d->m_durationBarTimer->stop();
            d->m_durationBarChunk->setFixedWidth(d->m_notification->width());
            d->m_elapsedTime = 0;
        }
    }
    QDialog::enterEvent(event);
}

void Toast::leaveEvent(QEvent *event)
{
    Q_D(Toast);
    if (d->m_config.duration() != 0 && !d->m_durationTimer->isActive()
        && d->m_config.resetDurationOnHover()) {
        d->m_durationTimer->start(d->m_config.duration());
        if (d->m_config.showDurationBar())
            d->m_durationBarTimer->start(ToastGlobalConfig::DurationBarUpdateInterval);
    }
    QDialog::leaveEvent(event);
}

void Toast::hideToast()
{
    Q_D(Toast);
    d->hideToast();
}

void Toast::updateDurationBar()
{
    Q_D(Toast);
    d->updateDurationBar();
}

QPoint Toast::calculatePosition()
{
    Q_D(Toast);
    return d->calculatePosition();
}

void Toast::setupUI()
{
    Q_D(Toast);
    d->setupUI();
}

void Toast::setupDefaultContent()
{
    Q_D(Toast);
    d->setupDefaultContent();
}

void Toast::updateStylesheet()
{
    Q_D(Toast);
    d->updateStylesheet();
}

void Toast::fadeIn()
{
    Q_D(Toast);
    d->m_animator->fadeIn();
}

void Toast::fadeOut()
{
    Q_D(Toast);
    d->m_animator->fadeOut();
}

void Toast::updatePosition()
{
    Q_D(Toast);
    d->m_animator->updatePosition();
}
