#include "ToastWindow.h"
#include "Toast.h"

ToastWindow::ToastWindow(QWidget *parent)
{
    // Window settings
    setFixedSize(650, 360);
    setWindowTitle("Qt Toast Demo");

    // Create main layout
    m_gridLayout = new QGridLayout();
    m_gridLayout->addWidget(createStaticSettingsGroup(), 0, 0);
    m_gridLayout->addWidget(createToastPresetGroup(), 1, 0);
    m_gridLayout->addWidget(createToastCustomGroup(), 0, 1, 2, 1, Qt::AlignmentFlag::AlignTop);

    // Set layout
    m_centralWidget = new QWidget();
    m_centralWidget->setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget);
    setFocus();
}

ToastWindow::~ToastWindow() {}

void ToastWindow::updateStaticSettings()
{
    auto &config = ToastGlobalConfig::instance();

    config.setMaximumOnScreen(m_maximumOnScreenSpinbox->value());
    config.setSpacing(m_spacingSpinbox->value());
    config.setOffset(m_offsetXSpinbox->value(), m_offsetYSpinbox->value());
    config.setAlwaysOnMainScreen(m_alwaysOnMainScreenCheckbox->isChecked());

    QString position = m_positionDropdown->currentText();

    if (position == "BOTTOM_LEFT") {
        config.setPosition(ToastPosition::BottomLeft);
    } else if (position == "BOTTOM_MIDDLE") {
        config.setPosition(ToastPosition::BottomMiddle);
    } else if (position == "BOTTOM_RIGHT") {
        config.setPosition(ToastPosition::BottomRight);
    } else if (position == "TOP_LEFT") {
        config.setPosition(ToastPosition::TopLeft);
    } else if (position == "TOP_MIDDLE") {
        config.setPosition(ToastPosition::TopMiddle);
    } else if (position == "TOP_RIGHT") {
        config.setPosition(ToastPosition::TopRight);
    } else if (position == "CENTER") {
        config.setPosition(ToastPosition::Center);
    }

    ToastManager::instance().updateAllPositions();
}

void ToastWindow::showPresetToast()
{
    auto *toast = new Toast(this);
    auto &config = toast->config();
    config.setDuration(rand() % 3001 + 2000); // 随机持续时间 2-5秒

    QString selectedPreset = m_presetDropdown->currentText();

    if (selectedPreset == "SUCCESS") {
        config.setTitle("Success! Confirmation email sent.");
        config.setText("Check your email to complete signup.");
        config.setPreset(ToastPreset::Success);
    } else if (selectedPreset == "SUCCESS_DARK") {
        config.setTitle("Success! Confirmation email sent.");
        config.setText("Check your email to complete signup.");
        config.setPreset(ToastPreset::SuccessDark);
    } else if (selectedPreset == "WARNING") {
        config.setTitle("Warning! Passwords do not match.");
        config.setText("Please confirm your password again.");
        config.setPreset(ToastPreset::Warning);
    } else if (selectedPreset == "WARNING_DARK") {
        config.setTitle("Warning! Passwords do not match.");
        config.setText("Please confirm your password again.");
        config.setPreset(ToastPreset::WarningDark);
    } else if (selectedPreset == "ERROR") {
        config.setTitle("Error! Cannot complete request.");
        config.setText("Please try again in a few minutes.");
        config.setPreset(ToastPreset::Error);
    } else if (selectedPreset == "ERROR_DARK") {
        config.setTitle("Error! Cannot complete request.");
        config.setText("Please try again in a few minutes.");
        config.setPreset(ToastPreset::ErrorDark);
    } else if (selectedPreset == "INFORMATION") {
        config.setTitle("Info: Restart required.");
        config.setText("Please restart the application.");
        config.setPreset(ToastPreset::Information);
    } else if (selectedPreset == "INFORMATION_DARK") {
        config.setTitle("Info: Restart required.");
        config.setText("Please restart the application.");
        config.setPreset(ToastPreset::InformationDark);
    }

    toast->present();
}

void ToastWindow::showCustomToast()
{
    auto *toast = new Toast(this);
    auto &config = toast->config();
    config.setDuration(m_durationSpinbox->value());
    config.setTitle(m_titleInput->text());
    config.setText(m_textInput->text());
    config.setBorderRadius(m_borderRadiusSpinbox->value());
    config.setShowIcon(m_showIconCheckbox->isChecked());
    config.setIconSize(QSize(m_iconSizeSpinbox->value(), m_iconSizeSpinbox->value()));
    config.setShowDurationBar(m_showDurationBarCheckbox->isChecked());
    config.setResetDurationOnHover(m_resetOnHoverCheckbox->isChecked());
    config.setFadeInDuration(m_fadeInDurationSpinbox->value());
    config.setFadeOutDuration(m_fadeOutDurationSpinbox->value());
    toast->setMinimumWidth(m_minWidthSpinbox->value());
    toast->setMaximumWidth(m_maxWidthSpinbox->value());
    toast->setMinimumHeight(m_minHeightSpinbox->value());
    toast->setMaximumHeight(m_maxHeightSpinbox->value());

    QString selectedIcon = m_iconDropdown->currentText();

    if (selectedIcon == "SUCCESS") {
        config.setIcon(ToastIcon::Success);
    } else if (selectedIcon == "WARNING") {
        config.setIcon(ToastIcon::Warning);
    } else if (selectedIcon == "ERROR") {
        config.setIcon(ToastIcon::Error);
    } else if (selectedIcon == "INFORMATION") {
        config.setIcon(ToastIcon::Information);
    } else if (selectedIcon == "CLOSE") {
        config.setIcon(ToastIcon::Close);
    }

    QString selectedCloseButtonSetting = m_closeButtonSettingsDropdown->currentText();

    if (selectedCloseButtonSetting == "TOP") {
        config.setCloseButtonAlignment(ToastButtonAlignment::Top);
    } else if (selectedCloseButtonSetting == "MIDDLE") {
        config.setCloseButtonAlignment(ToastButtonAlignment::Middle);
    } else if (selectedCloseButtonSetting == "BOTTOM") {
        config.setCloseButtonAlignment(ToastButtonAlignment::Bottom);
    } else if (selectedCloseButtonSetting == "DISABLED") {
        config.setShowCloseButton(false);
    }

    toast->present();
}

QGroupBox *ToastWindow::createStaticSettingsGroup()
{
    QGroupBox *groupBox = new QGroupBox("Static Settings");
    const auto &config = ToastGlobalConfig::instance();
    const auto &manager = ToastManager::instance();

    m_maximumOnScreenSpinbox = new QSpinBox();
    m_maximumOnScreenSpinbox->setRange(1, 10);
    m_maximumOnScreenSpinbox->setValue(config.maximumOnScreen()); // 使用 getter
    m_maximumOnScreenSpinbox->setFixedHeight(24);

    m_spacingSpinbox = new QSpinBox();
    m_spacingSpinbox->setRange(0, 100);
    m_spacingSpinbox->setValue(config.spacing());
    m_spacingSpinbox->setFixedHeight(24);

    m_offsetXSpinbox = new QSpinBox();
    m_offsetXSpinbox->setRange(0, 500);
    m_offsetXSpinbox->setValue(config.offsetX());
    m_offsetXSpinbox->setFixedHeight(24);

    m_offsetYSpinbox = new QSpinBox();
    m_offsetYSpinbox->setRange(0, 500);
    m_offsetYSpinbox->setValue(config.offsetY());
    m_offsetYSpinbox->setFixedHeight(24);

    m_alwaysOnMainScreenCheckbox = new QCheckBox("Always on main screen");
    m_alwaysOnMainScreenCheckbox->setChecked(config.alwaysOnMainScreen());

    m_positionDropdown = new QComboBox();
    m_positionDropdown->addItems(
        {"BOTTOM_LEFT",
         "BOTTOM_MIDDLE",
         "BOTTOM_RIGHT",
         "TOP_LEFT",
         "TOP_MIDDLE",
         "TOP_RIGHT",
         "CENTER"});
    switch (config.position()) {
    case ToastPosition::BottomLeft:
        m_positionDropdown->setCurrentIndex(0);
        break;
    case ToastPosition::BottomMiddle:
        m_positionDropdown->setCurrentIndex(1);
        break;
    case ToastPosition::BottomRight:
        m_positionDropdown->setCurrentIndex(2);
        break;
    case ToastPosition::TopLeft:
        m_positionDropdown->setCurrentIndex(3);
        break;
    case ToastPosition::TopMiddle:
        m_positionDropdown->setCurrentIndex(4);
        break;
    case ToastPosition::TopRight:
        m_positionDropdown->setCurrentIndex(5);
        break;
    case ToastPosition::Center:
        m_positionDropdown->setCurrentIndex(6);
        break;
    }
    m_positionDropdown->setFixedHeight(24);

    m_updateButton = new QPushButton("Update");
    m_updateButton->setFixedHeight(32);
    connect(m_updateButton, &QPushButton::clicked, this, &ToastWindow::updateStaticSettings);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Max on screen:", m_maximumOnScreenSpinbox);
    formLayout->addRow("Spacing:", m_spacingSpinbox);
    formLayout->addRow("Offset X:", m_offsetXSpinbox);
    formLayout->addRow("Offset Y:", m_offsetYSpinbox);
    formLayout->addRow("Position:", m_positionDropdown);

    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addLayout(formLayout);
    vboxLayout->addWidget(m_alwaysOnMainScreenCheckbox);
    vboxLayout->addWidget(m_updateButton);
    vboxLayout->addStretch(1);
    groupBox->setLayout(vboxLayout);

    return groupBox;
}

QGroupBox *ToastWindow::createToastPresetGroup()
{
    QGroupBox *groupBox = new QGroupBox("Toast Preset");

    m_presetDropdown = new QComboBox();
    m_presetDropdown->addItems(
        {"SUCCESS",
         "WARNING",
         "ERROR",
         "INFORMATION",
         "SUCCESS_DARK",
         "WARNING_DARK",
         "ERROR_DARK",
         "INFORMATION_DARK"});
    m_presetDropdown->setFixedHeight(26);

    showPresetToastButton = new QPushButton("Show preset toast");
    showPresetToastButton->setFixedHeight(32);
    connect(showPresetToastButton, &QPushButton::clicked, this, &ToastWindow::showPresetToast);

    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addWidget(m_presetDropdown);
    vboxLayout->addWidget(showPresetToastButton);
    vboxLayout->addStretch(1);
    groupBox->setLayout(vboxLayout);

    return groupBox;
}

QGroupBox *ToastWindow::createToastCustomGroup()
{
    // createToastCustomGroup 函数内容与原示例完全相同
    // 因为它只涉及单个 Toast 实例的创建和配置，不依赖全局静态方法
    QGroupBox *groupBox = new QGroupBox("Custom Toast");

    m_durationSpinbox = new QSpinBox();
    m_durationSpinbox->setRange(0, 50000);
    m_durationSpinbox->setValue(5000);
    m_durationSpinbox->setFixedHeight(24);

    m_titleInput = new QLineEdit("Lorem ipsum dolor sit");
    m_titleInput->setFixedHeight(24);

    m_textInput = new QLineEdit("Lorem ipsum dolor sit amet consetetur");
    m_textInput->setFixedHeight(24);

    m_borderRadiusSpinbox = new QSpinBox();
    m_borderRadiusSpinbox->setRange(0, 20);
    m_borderRadiusSpinbox->setValue(2);
    m_borderRadiusSpinbox->setFixedHeight(24);

    m_showIconCheckbox = new QCheckBox("Show icon");

    m_iconDropdown = new QComboBox();
    m_iconDropdown->addItems({"SUCCESS", "WARNING", "ERROR", "INFORMATION", "CLOSE"});
    m_iconDropdown->setFixedHeight(24);

    m_iconSizeSpinbox = new QSpinBox();
    m_iconSizeSpinbox->setRange(5, 50);
    m_iconSizeSpinbox->setValue(18);
    m_iconSizeSpinbox->setFixedHeight(24);

    m_showDurationBarCheckbox = new QCheckBox("Show duration bar");
    m_showDurationBarCheckbox->setChecked(true);

    m_resetOnHoverCheckbox = new QCheckBox("Reset duration on hover");
    m_resetOnHoverCheckbox->setChecked(true);

    m_closeButtonSettingsDropdown = new QComboBox();
    m_closeButtonSettingsDropdown->addItems({"TOP", "MIDDLE", "BOTTOM", "DISABLED"});
    m_closeButtonSettingsDropdown->setFixedHeight(24);

    m_minWidthSpinbox = new QSpinBox();
    m_minWidthSpinbox->setRange(0, 1000);
    m_minWidthSpinbox->setFixedHeight(24);

    m_maxWidthSpinbox = new QSpinBox();
    m_maxWidthSpinbox->setRange(0, 1000);
    m_maxWidthSpinbox->setValue(1000);
    m_maxWidthSpinbox->setFixedHeight(24);

    m_minHeightSpinbox = new QSpinBox();
    m_minHeightSpinbox->setRange(0, 1000);
    m_minHeightSpinbox->setFixedHeight(24);

    m_maxHeightSpinbox = new QSpinBox();
    m_maxHeightSpinbox->setRange(0, 1000);
    m_maxHeightSpinbox->setValue(1000);
    m_maxHeightSpinbox->setFixedHeight(24);

    m_fadeInDurationSpinbox = new QSpinBox();
    m_fadeInDurationSpinbox->setRange(0, 1000);
    m_fadeInDurationSpinbox->setValue(250);
    m_fadeInDurationSpinbox->setFixedHeight(24);

    m_fadeOutDurationSpinbox = new QSpinBox();
    m_fadeOutDurationSpinbox->setRange(0, 1000);
    m_fadeOutDurationSpinbox->setValue(250);
    m_fadeOutDurationSpinbox->setFixedHeight(24);

    m_customToastButton = new QPushButton("Show custom toast");
    m_customToastButton->setFixedHeight(32);
    connect(m_customToastButton, &QPushButton::clicked, this, &ToastWindow::showCustomToast);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Duration:", m_durationSpinbox);
    formLayout->addRow("Title:", m_titleInput);
    formLayout->addRow("Text:", m_textInput);

    QFormLayout *iconSizeLayout = new QFormLayout();
    iconSizeLayout->addRow("Icon size:", m_iconSizeSpinbox);

    QHBoxLayout *iconLayout = new QHBoxLayout();
    iconLayout->addWidget(m_showIconCheckbox);
    iconLayout->addWidget(m_iconDropdown);
    iconLayout->addLayout(iconSizeLayout);
    iconLayout->setContentsMargins(0, 5, 0, 3);

    QHBoxLayout *checkboxLayout = new QHBoxLayout();
    checkboxLayout->addWidget(m_showDurationBarCheckbox);
    checkboxLayout->addWidget(m_resetOnHoverCheckbox);
    checkboxLayout->setContentsMargins(0, 0, 0, 5);

    QHBoxLayout *doubleFormLayout1 = new QHBoxLayout();
    doubleFormLayout1->addWidget(new QLabel("Border radius:"));
    doubleFormLayout1->addWidget(m_borderRadiusSpinbox);
    doubleFormLayout1->addWidget(new QLabel("Close button:"));
    doubleFormLayout1->addWidget(m_closeButtonSettingsDropdown);

    QHBoxLayout *doubleFormLayout2 = new QHBoxLayout();
    doubleFormLayout2->addWidget(new QLabel("Min width:"));
    doubleFormLayout2->addWidget(m_minWidthSpinbox);
    doubleFormLayout2->addWidget(new QLabel("Max width:"));
    doubleFormLayout2->addWidget(m_maxWidthSpinbox);

    QHBoxLayout *doubleFormLayout3 = new QHBoxLayout();
    doubleFormLayout3->addWidget(new QLabel("Min height:"));
    doubleFormLayout3->addWidget(m_minHeightSpinbox);
    doubleFormLayout3->addWidget(new QLabel("Max height:"));
    doubleFormLayout3->addWidget(m_maxHeightSpinbox);

    QHBoxLayout *doubleFormLayout4 = new QHBoxLayout();
    doubleFormLayout4->addWidget(new QLabel("Fade in duration:"));
    doubleFormLayout4->addWidget(m_fadeInDurationSpinbox);
    doubleFormLayout4->addWidget(new QLabel("Fade out duration:"));
    doubleFormLayout4->addWidget(m_fadeOutDurationSpinbox);
    doubleFormLayout4->setContentsMargins(0, 0, 0, 3);

    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addLayout(formLayout);
    vboxLayout->addLayout(iconLayout);
    vboxLayout->addLayout(checkboxLayout);
    vboxLayout->addLayout(doubleFormLayout1);
    vboxLayout->addLayout(doubleFormLayout2);
    vboxLayout->addLayout(doubleFormLayout3);
    vboxLayout->addLayout(doubleFormLayout4);
    vboxLayout->addWidget(m_customToastButton);
    vboxLayout->addStretch(1);
    groupBox->setLayout(vboxLayout);

    return groupBox;
}
