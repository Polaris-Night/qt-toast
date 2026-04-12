#include "ToastAnimator.h"
#include "Toast.h"
#include "ToastConfig.h"
#include "ToastPrivate.h"
#include <cmath>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

ToastAnimator::ToastAnimator(ToastPrivate *d)
    : d_ptr(d)
{}

void ToastAnimator::fadeIn()
{
    auto *anim = new QPropertyAnimation(d_ptr->m_opacityEffect, "opacity", d_ptr->q_ptr);
    anim->setDuration(d_ptr->m_config.fadeInDuration());
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ToastAnimator::fadeOut()
{
    if (d_ptr->m_fadingOut)
        return;
    d_ptr->m_fadingOut = true;
    auto *anim = new QPropertyAnimation(d_ptr->m_opacityEffect, "opacity", d_ptr->q_ptr);
    QObject::connect(anim, &QPropertyAnimation::finished, d_ptr->q_ptr, [d = d_ptr]() {
        d->q_ptr->close();
        d->m_elapsedTime = 0;
        d->m_fadingOut = false;
        d->m_used = false;
        emit d->q_ptr->closed();
        ToastManager::instance().complete(d->q_ptr);
    });
    anim->setDuration(d_ptr->m_config.fadeOutDuration());
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ToastAnimator::flyIn()
{
    auto position = d_ptr->calculatePosition();
    if (ToastManager::instance().visibleCount() > 1) {
        auto *predecessorToast = ToastManager::instance().getPredecessorToast(
            qobject_cast<Toast *>(d_ptr->q_ptr));
        if (!predecessorToast) {
            d_ptr->q_ptr->move(position);
            return;
        }
        QPoint predecessorTarget = predecessorToast->calculatePosition();
        int predecessorTargetDifferenceY = std::abs(predecessorToast->y() - predecessorTarget.y());

        auto gPosition = ToastGlobalConfig::instance().position();
        if (gPosition == ToastPosition::BottomRight || gPosition == ToastPosition::BottomLeft
            || gPosition == ToastPosition::BottomMiddle) {
            d_ptr->q_ptr->move(
                position.x(),
                position.y() - static_cast<int>(d_ptr->q_ptr->height() / 1.5)
                    - predecessorTargetDifferenceY);
        } else if (
            gPosition == ToastPosition::TopRight || gPosition == ToastPosition::TopLeft
            || gPosition == ToastPosition::TopMiddle || gPosition == ToastPosition::Center) {
            d_ptr->q_ptr->move(
                position.x(),
                position.y() + static_cast<int>(d_ptr->q_ptr->height() / 1.5)
                    + predecessorTargetDifferenceY);
        }

        auto *posAnim = new QPropertyAnimation(d_ptr->q_ptr, "pos", d_ptr->q_ptr);
        posAnim->setEndValue(QPoint(position.x(), position.y()));
        posAnim->setDuration(d_ptr->m_config.fadeInDuration());
        posAnim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        d_ptr->q_ptr->move(position);
    }
}

void ToastAnimator::updatePosition()
{
    auto position = d_ptr->calculatePosition();
    auto *anim = new QPropertyAnimation(d_ptr->q_ptr, "pos", d_ptr->q_ptr);
    anim->setEndValue(position);
    anim->setDuration(ToastGlobalConfig::UpdatePositionDuration);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
