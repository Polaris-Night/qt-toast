#include "ToastLayoutBuilder.h"
#include "Toast.h"
#include "ToastConfig.h"
#include "ToastPrivate.h"
#include <algorithm>
#include <climits>
#include <QFontMetrics>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QRect>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <Qt>

void ToastLayoutBuilder::buildLayout(ToastPrivate *d)
{
    QVBoxLayout *rootLayout = new QVBoxLayout();
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    d->m_notification->setLayout(rootLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(d->m_config.margins());
    mainLayout->setSpacing(0);

    // Icon section
    buildIconSection(d, mainLayout);

    // Content area
    buildContentArea(d, mainLayout);

    // Close button
    buildCloseButton(d, mainLayout);

    rootLayout->addLayout(mainLayout);

    // Duration bar
    buildDurationBar(d, rootLayout);
}

void ToastLayoutBuilder::buildIconSection(ToastPrivate *d, QHBoxLayout *mainLayout)
{
    if (d->m_config.showIcon()) {
        QHBoxLayout *iconLayout = new QHBoxLayout();
        iconLayout->setContentsMargins(d->m_config.iconSectionMargins());

        QVBoxLayout *iconInnerLayout = new QVBoxLayout();
        iconInnerLayout->setContentsMargins(d->m_config.iconMargins());
        iconInnerLayout->addWidget(d->m_iconWidget);
        iconLayout->addLayout(iconInnerLayout);

        if (d->m_config.showIconSeparator()) {
            d->m_iconSeparator->setFixedWidth(d->m_config.iconSeparatorWidth());
            iconLayout->addWidget(d->m_iconSeparator);
        }

        mainLayout->addLayout(iconLayout);
    } else {
        d->m_iconWidget->setVisible(false);
        d->m_iconSeparator->setVisible(false);
    }
}

void ToastLayoutBuilder::buildContentArea(ToastPrivate *d, QHBoxLayout *mainLayout)
{
    QVBoxLayout *contentLayout = new QVBoxLayout();
    contentLayout->setContentsMargins(d->m_config.textSectionMargins());
    const bool hasTitleAndText = !d->m_config.title().isEmpty() && !d->m_config.text().isEmpty();
    contentLayout->setSpacing(hasTitleAndText ? d->m_config.textSectionSpacing() : 0);
    contentLayout->addWidget(d->m_titleLabel);
    contentLayout->addWidget(d->m_textLabel);

    // 按钮在 stretch 之前，确保按钮区域有足够空间
    buildButtons(d, contentLayout);

    contentLayout->addStretch();

    QWidget *contentWidget = new QWidget();
    contentWidget->setLayout(contentLayout);
    mainLayout->addWidget(contentWidget);
}

void ToastLayoutBuilder::buildButtons(ToastPrivate *d, QVBoxLayout *contentLayout)
{
    if (d->m_buttons.empty()) {
        return;
    }

    // 使用 QGridLayout 实现按钮自动换行
    QGridLayout *buttonGrid = new QGridLayout();
    buttonGrid->setSpacing(10);
    buttonGrid->setContentsMargins(0, 5, 0, 0);

    // 获取 buttonsPerRow：实例配置 > 全局默认值
    int buttonsPerRow = d->m_config.buttonsPerRow();
    if (buttonsPerRow <= 0) {
        buttonsPerRow = ToastGlobalConfig::instance().buttonsPerRow();
    }
    if (buttonsPerRow <= 0) {
        buttonsPerRow = 3; // 兜底默认值
    }

    QFontMetrics fm(d->m_config.textFont());

    // 预先计算所有按钮尺寸，找出最大宽度用于均匀列宽
    struct ButtonInfo2
    {
        QString text;
        int width;
        int height;
        std::function<void()> onClicked;
    };
    std::vector<ButtonInfo2> btnInfos;
    btnInfos.reserve(d->m_buttons.size());
    for (const auto &bi : d->m_buttons) {
        int textW = fm.horizontalAdvance(bi.first);
        int w = std::max(textW + 32, 70); // +32 for padding+border, min 70px
        int h = fm.height() + 16;
        btnInfos.push_back({bi.first, w, h, bi.second});
    }

    // 每行的最大按钮宽度（用于均匀列宽）
    int maxBtnWidthInRow = 0;
    for (size_t i = 0; i < btnInfos.size(); i++) {
        int colInRow = static_cast<int>(i) % buttonsPerRow;
        if (colInRow == 0)
            maxBtnWidthInRow = 0;
        maxBtnWidthInRow = std::max(maxBtnWidthInRow, btnInfos[i].width);
        if (colInRow == buttonsPerRow - 1 || i == btnInfos.size() - 1) {
            // 设置该列的宽度
            // QGridLayout 会自动根据最大 widget 调整
        }
    }

    int row = 0, col = 0;
    for (const auto &bi : btnInfos) {
        QPushButton *button = new QPushButton(bi.text, d->m_notification);
        button->setFont(d->m_config.textFont());

        button->setMinimumSize(bi.width, bi.height);
        // 使用 MinimumExpanding 确保按钮不会收缩，也不会过度扩张
        button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

        applyButtonStyles(button, d->m_config);

        if (bi.onClicked) {
            QObject::connect(button, &QPushButton::clicked, bi.onClicked);
        }
        buttonGrid->addWidget(button, row, col);

        col++;
        if (col >= buttonsPerRow) {
            col = 0;
            row++;
        }
    }

    // 为最后一行设置 stretch 占位，防止按钮过度扩张
    int lastRow = row;
    int lastCol = col;
    if (lastCol > 0 && lastCol < buttonsPerRow) {
        buttonGrid->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum),
            lastRow,
            lastCol,
            1,
            buttonsPerRow - lastCol);
    }

    contentLayout->addLayout(buttonGrid);
}

void ToastLayoutBuilder::applyButtonStyles(QPushButton *button, const ToastConfig &config)
{
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
                              .arg(config.backgroundColor().darker(120).name())
                              .arg(config.textColor().name())
                              .arg(config.backgroundColor().darker(150).name())
                              .arg(config.textFont().family())
                              .arg(config.textFont().pointSize())
                              .arg(config.backgroundColor().darker(130).name())
                              .arg(config.backgroundColor().darker(160).name()));
}

void ToastLayoutBuilder::buildCloseButton(ToastPrivate *d, QHBoxLayout *mainLayout)
{
    if (d->m_config.showCloseButton()) {
        Qt::Alignment alignment = Qt::AlignTop;
        switch (d->m_config.closeButtonAlignment()) {
        case ToastButtonAlignment::Middle:
            alignment = Qt::AlignVCenter;
            break;
        case ToastButtonAlignment::Bottom:
            alignment = Qt::AlignBottom;
            break;
        default:
            break;
        }
        mainLayout->addWidget(d->m_closeButton, 0, alignment);
    } else {
        d->m_closeButton->setVisible(false);
    }
}

void ToastLayoutBuilder::buildDurationBar(ToastPrivate *d, QVBoxLayout *rootLayout)
{
    if (d->m_config.showDurationBar()) {
        d->m_durationBarContainer->setFixedHeight(4);
        QHBoxLayout *durationLayout = new QHBoxLayout(d->m_durationBarContainer);
        durationLayout->setContentsMargins(0, 0, 0, 0);
        durationLayout->addWidget(d->m_durationBar);
        d->m_durationBar->setFixedHeight(4);
        rootLayout->addWidget(d->m_durationBarContainer);
    } else {
        d->m_durationBarContainer->setVisible(false);
    }
}

QSize ToastLayoutBuilder::calculateSize(
    const ToastConfig &config,
    const std::vector<std::pair<QString, std::function<void()>>> &buttons,
    int maxTextWidth)
{
    QFontMetrics titleFM(config.titleFont());
    QFontMetrics textFM(config.textFont());

    int titleWidth = 0, titleHeight = 0;
    if (!config.title().isEmpty()) {
        QRect rect = titleFM.boundingRect(
            QRect(0, 0, maxTextWidth, INT_MAX), Qt::TextWordWrap | Qt::AlignLeft, config.title());
        titleWidth = rect.width() + 1;
        titleHeight = rect.height();
    }

    int textWidth = 0, textHeight = 0;
    if (!config.text().isEmpty()) {
        QRect rect = textFM.boundingRect(
            QRect(0, 0, maxTextWidth, INT_MAX), Qt::TextWordWrap | Qt::AlignLeft, config.text());
        textWidth = rect.width() + 1;
        textHeight = rect.height();
    }

    int contentMaxWidth = std::max(titleWidth, textWidth);
    int contentHeight = titleHeight
                        + (!config.title().isEmpty() && !config.text().isEmpty()
                               ? config.textSectionSpacing()
                               : 0)
                        + textHeight;

    // 计算按钮区域宽度
    int buttonMaxWidth = 0;
    int buttonTotalHeight = 0;
    if (!buttons.empty()) {
        int buttonsPerRow = config.buttonsPerRow();
        if (buttonsPerRow <= 0) {
            buttonsPerRow = ToastGlobalConfig::instance().buttonsPerRow();
        }
        if (buttonsPerRow <= 0) {
            buttonsPerRow = 3; // 兜底默认值
        }
        int buttonSpacing = 10;
        int numRows = (static_cast<int>(buttons.size()) + buttonsPerRow - 1) / buttonsPerRow;

        // 计算每行按钮总宽度（每列取该行最大按钮宽度）
        for (int r = 0; r < numRows; r++) {
            int rowWidth = 0;
            int rowMaxHeight = 0;
            int startIdx = r * buttonsPerRow;
            int endIdx = std::min(startIdx + buttonsPerRow, static_cast<int>(buttons.size()));

            // 先找出该行最大按钮宽度（用于均匀列宽）
            int colMaxWidth = 0;
            for (int i = startIdx; i < endIdx; i++) {
                int textW = textFM.horizontalAdvance(buttons[i].first);
                int btnW = std::max(textW + 32, 70);
                colMaxWidth = std::max(colMaxWidth, btnW);
            }

            // 每行宽度 = 列数 × 最大列宽 + 间距
            int colsInRow = endIdx - startIdx;
            rowWidth = colsInRow * colMaxWidth + (colsInRow - 1) * buttonSpacing;
            buttonMaxWidth = std::max(buttonMaxWidth, rowWidth);

            // 行高度
            for (int i = startIdx; i < endIdx; i++) {
                int btnH = textFM.height() + 16;
                rowMaxHeight = std::max(rowMaxHeight, btnH);
            }
            buttonTotalHeight += rowMaxHeight;
        }
        buttonTotalHeight += (numRows - 1) * buttonSpacing;
        buttonTotalHeight += 15; // margin
    }

    // 窗口宽度取内容宽度和按钮宽度的最大值
    int effectiveMaxWidth = std::max(contentMaxWidth, buttonMaxWidth);

    int minWidth = config.margins().left() + (config.showIcon() ? 50 : 0) + effectiveMaxWidth
                   + config.margins().right() + (config.showCloseButton() ? 30 : 0);

    int minHeight = config.margins().top() + contentHeight + buttonTotalHeight
                    + config.margins().bottom();

    return QSize(minWidth, minHeight);
}
