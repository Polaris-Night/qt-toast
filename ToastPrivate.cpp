#include "ToastPrivate.h"
#include "Toast.h"
#include "ToastAnimator.h"
#include "ToastConfig.h"
#include "ToastLayoutBuilder.h"
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QGraphicsOpacityEffect>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

ToastPrivate::ToastPrivate(Toast *q)
    : q_ptr(q)
{
    // Notification widget
    m_notification = new QLabel(q);
    // Drop shadow
    for (size_t i = 0; i < m_dropShadowLayer.size(); i++) {
        m_dropShadowLayer[i] = new QWidget(q);
        m_dropShadowLayer[i]->setObjectName(QString("toast-drop-shadow-layer-%1").arg(i + 1));
    }
    // Opacity effect
    m_opacityEffect = new QGraphicsOpacityEffect(q);
    m_opacityEffect->setOpacity(1);
    // Close button
    m_closeButton = new QPushButton(m_notification);
    m_closeButton->setObjectName("toast-close-button");
    m_closeButton->setCursor(Qt::CursorShape::PointingHandCursor);
    QObject::connect(m_closeButton, &QPushButton::clicked, q, &Toast::hide);
    // Text and title labels
    m_titleLabel = new QLabel(m_notification);
    m_textLabel = new QLabel(m_notification);
    // Icon
    m_iconWidget = new QPushButton(m_notification);
    m_iconWidget->setObjectName("toast-icon-widget");
    // Icon separator
    m_iconSeparator = new QWidget(m_notification);
    m_iconSeparator->setFixedWidth(2);
    // Duration bar container
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
    // Timers
    m_durationTimer = new QTimer(q);
    m_durationTimer->setSingleShot(true);
    QObject::connect(m_durationTimer, &QTimer::timeout, q, &Toast::hideToast);
    m_durationBarTimer = new QTimer(q);
    QObject::connect(m_durationBarTimer, &QTimer::timeout, q, [this]() { updateDurationBar(); });

    // Window settings
    q->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    q->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    q->setGraphicsEffect(m_opacityEffect);

    // Load stylesheet
    QFile file(":/css/toast.css");
    if (file.open(QFile::ReadOnly)) {
        q->setStyleSheet(file.readAll());
    }

    // Initialize animator
    m_animator = std::make_unique<ToastAnimator>(this);
}

void ToastPrivate::setupUI()
{
    setupDefaultContent();
}

void ToastPrivate::setupDefaultContent()
{
    updateStylesheet();
    ToastLayoutBuilder::buildLayout(this);

    // 计算尺寸（包含按钮区域）
    auto geometry = ToastLayoutBuilder::calculateSize(m_config, m_buttons);
    int minWidth = geometry.width();
    int minHeight = geometry.height();

    if (m_config.showDurationBar()) {
        minHeight += 4;
    }

    m_notification->setMinimumSize(minWidth, minHeight);
    m_notification->setMaximumSize(q_ptr->maximumWidth(), q_ptr->maximumHeight());
    m_notification->adjustSize();

    // 确保实际高度不小于计算的最小高度
    if (m_notification->height() < minHeight) {
        m_notification->setFixedHeight(minHeight);
    }

    // 阴影层
    int totalWidth = m_notification->width() + ToastGlobalConfig::DropShadowSize * 2;
    int totalHeight = m_notification->height() + ToastGlobalConfig::DropShadowSize * 2;

    for (size_t i = 0; i < m_dropShadowLayer.size(); i++) {
        m_dropShadowLayer.at(i)
            ->resize(totalWidth - static_cast<int>(i) * 2, totalHeight - static_cast<int>(i) * 2);
        m_dropShadowLayer.at(i)->move(static_cast<int>(i), static_cast<int>(i));
    }

    q_ptr->setFixedSize(totalWidth, totalHeight);
    m_notification->move(ToastGlobalConfig::DropShadowSize, ToastGlobalConfig::DropShadowSize);
    m_notification->raise();

    // 持续时间条适配
    if (m_config.showDurationBar()) {
        int notificationWidth = m_notification->width();
        m_durationBarContainer->setFixedWidth(notificationWidth);
        m_durationBarContainer->move(0, m_notification->height());
        m_durationBar->setFixedWidth(notificationWidth);
        m_durationBarChunk->setFixedWidth(notificationWidth);
    }
}

void ToastPrivate::updateStylesheet()
{
    m_notification->setStyleSheet(QString("background: %1; border-radius: %2px; ")
                                      .arg(m_config.backgroundColor().name())
                                      .arg(m_config.borderRadius()));

    m_durationBar->setStyleSheet(QString("background: rgba(%1, %2, %3, 100); border-radius: %4px;")
                                     .arg(m_config.durationBarColor().red())
                                     .arg(m_config.durationBarColor().green())
                                     .arg(m_config.durationBarColor().blue())
                                     .arg(m_config.borderRadius()));

    m_durationBarChunk->setStyleSheet(
        QString(
            "background: rgba(%1, %2, %3, 255);"
            "border-bottom-left-radius: %4px; "
            "border-bottom-right-radius: %5px;")
            .arg(m_config.durationBarColor().red())
            .arg(m_config.durationBarColor().green())
            .arg(m_config.durationBarColor().blue())
            .arg(m_config.borderRadius())
            .arg(m_config.duration() == 0 ? m_config.borderRadius() : 0));

    m_iconSeparator->setStyleSheet(
        QString("background: %1;").arg(m_config.iconSeparatorColor().name()));

    m_titleLabel->setStyleSheet(QString("color: %1;").arg(m_config.titleColor().name()));
    m_textLabel->setStyleSheet(QString("color: %1;").arg(m_config.textColor().name()));

    // 图标和关闭按钮
    auto formatIcon = [](const QPixmap &pm, const QSize &size, const QColor &color) -> QIcon {
        QIcon icon(pm);
        auto iconImage = ToastConfig::recolorImage(icon.pixmap(size).toImage(), color);
        icon = QIcon(QPixmap::fromImage(iconImage));
        return icon;
    };

    {
        auto icon = formatIcon(m_config.icon(), m_config.iconSize(), m_config.iconColor());
        m_iconWidget->setIconSize(m_config.iconSize());
        m_iconWidget->setIcon(icon);
        m_iconWidget->setFixedSize(m_config.iconSize());
        if (m_config.showIconSeparator()) {
            m_iconSeparator->setFixedWidth(m_config.iconSeparatorWidth());
        }
    }

    {
        auto icon = formatIcon(
            m_config.closeButtonIcon(),
            m_config.closeButtonIconSize(),
            m_config.closeButtonIconColor());
        m_closeButton->setIconSize(m_config.closeButtonIconSize());
        m_closeButton->setIcon(icon);
    }

    // 文本
    m_titleLabel->setFont(m_config.titleFont());
    m_titleLabel->setText(m_config.title());
    m_textLabel->setFont(m_config.textFont());
    m_textLabel->setText(m_config.text());
}

QPoint ToastPrivate::calculatePosition()
{
    int offsetY = ToastManager::instance().toastOffsetY(q_ptr);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QScreen *currentScreen = nullptr;

    if (ToastGlobalConfig::instance().fixedScreen()) {
        currentScreen = ToastGlobalConfig::instance().fixedScreen();
    } else if (ToastGlobalConfig::instance().alwaysOnMainScreen() || !q_ptr->parentWidget()) {
        currentScreen = primaryScreen;
    } else {
        QList<QScreen *> screens = QGuiApplication::screens();
        for (auto *screen : screens) {
            if (q_ptr->parentWidget()->geometry().intersects(screen->geometry())) {
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

    int notifWidth = m_notification->width();
    int notifHeight = m_notification->height();

    switch (gPosition) {
    case ToastPosition::BottomRight:
        x = currentScreen->geometry().width() - notifWidth - gOffsetX
            + currentScreen->geometry().x();
        y = currentScreen->geometry().height() - notifHeight - gOffsetY
            + currentScreen->geometry().y() - offsetY;
        break;
    case ToastPosition::BottomLeft:
        x = currentScreen->geometry().x() + gOffsetX;
        y = currentScreen->geometry().height() - notifHeight - gOffsetY
            + currentScreen->geometry().y() - offsetY;
        break;
    case ToastPosition::BottomMiddle:
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2 - notifWidth / 2);
        y = currentScreen->geometry().height() - notifHeight - gOffsetY
            + currentScreen->geometry().y() - offsetY;
        break;
    case ToastPosition::TopRight:
        x = currentScreen->geometry().width() - notifWidth - gOffsetX
            + currentScreen->geometry().x();
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
        break;
    case ToastPosition::TopLeft:
        x = currentScreen->geometry().x() + gOffsetX;
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
        break;
    case ToastPosition::TopMiddle:
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2 - notifWidth / 2);
        y = currentScreen->geometry().y() + gOffsetY + offsetY;
        break;
    case ToastPosition::Center:
        x = static_cast<int>(
            currentScreen->geometry().x() + currentScreen->geometry().width() / 2 - notifWidth / 2);
        y = static_cast<int>(
            currentScreen->geometry().y() + currentScreen->geometry().height() / 2 - notifHeight / 2
            + offsetY);
        break;
    }

    x -= ToastGlobalConfig::DropShadowSize;
    y -= ToastGlobalConfig::DropShadowSize;

    return QPoint(x, y);
}

void ToastPrivate::hideToast()
{
    m_elapsedTime = 0;
    if (m_config.duration() != 0)
        m_durationTimer->stop();
    if (m_config.showDurationBar())
        m_durationBarTimer->stop();
    m_animator->fadeOut();
}

void ToastPrivate::updateDurationBar()
{
    m_elapsedTime += ToastGlobalConfig::DurationBarUpdateInterval;
    if (m_elapsedTime >= m_config.duration()) {
        m_durationBarTimer->stop();
        return;
    }
    double newChunkWidth = std::floor(
        m_durationBarContainer->width()
        - (static_cast<double>(m_elapsedTime) / m_config.duration())
              * m_durationBarContainer->width());
    m_durationBarChunk->setFixedWidth(static_cast<int>(newChunkWidth));
}
