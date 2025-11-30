#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>

class ToastWindow : public QMainWindow
{
    Q_OBJECT

public:
    ToastWindow(QWidget *parent = nullptr);
    ~ToastWindow();

private slots:
    void updateStaticSettings();
    void showPresetToast();
    void showCustomToast();

private:
    QGridLayout *m_gridLayout;
    QWidget *m_centralWidget;
    QSpinBox *m_maximumOnScreenSpinbox;
    QSpinBox *m_spacingSpinbox;
    QSpinBox *m_offsetXSpinbox;
    QSpinBox *m_offsetYSpinbox;
    QCheckBox *m_alwaysOnMainScreenCheckbox;
    QComboBox *m_positionDropdown;
    QPushButton *m_updateButton;
    QComboBox *m_presetDropdown;
    QPushButton *showPresetToastButton;
    QSpinBox *m_durationSpinbox;
    QLineEdit *m_titleInput;
    QLineEdit *m_textInput;
    QSpinBox *m_borderRadiusSpinbox;
    QCheckBox *m_showIconCheckbox;
    QComboBox *m_iconDropdown;
    QSpinBox *m_iconSizeSpinbox;
    QCheckBox *m_showDurationBarCheckbox;
    QCheckBox *m_resetOnHoverCheckbox;
    QComboBox *m_closeButtonSettingsDropdown;
    QSpinBox *m_minWidthSpinbox;
    QSpinBox *m_maxWidthSpinbox;
    QSpinBox *m_minHeightSpinbox;
    QSpinBox *m_maxHeightSpinbox;
    QSpinBox *m_fadeInDurationSpinbox;
    QSpinBox *m_fadeOutDurationSpinbox;
    QPushButton *m_customToastButton;

    QGroupBox *createStaticSettingsGroup();
    QGroupBox *createToastPresetGroup();
    QGroupBox *createToastCustomGroup();
};
