#include "togglebutton.h"

ToggleButton::ToggleButton(int trackRadius, int thumbRadius, bool showIcons, QWidget* parent) : QAbstractButton(parent)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    mTrackRadius = trackRadius;
    mThumbRadius = thumbRadius;
    mAnimation = new QPropertyAnimation(this);
    mAnimation->setTargetObject(this);

    mMargin = 0 > (mThumbRadius - mTrackRadius) ? 0 : (mThumbRadius - mTrackRadius);
    mBaseOffset = mThumbRadius > mTrackRadius ? mThumbRadius : mTrackRadius;
    mEndOffset.insert(true, 4 * mTrackRadius + 2 * mMargin - mBaseOffset);
    mEndOffset.insert(false, mBaseOffset);
    mOffset = mBaseOffset;
    QPalette palette = this->palette();

    if (mThumbRadius > mTrackRadius)
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlight());
        mThumbColor.insert(false, palette.light());
        mTextColor.insert(true, palette.highlightedText().color());
        mTextColor.insert(false, palette.dark().color());
        mOpacity = 0.5;
    }
    else
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlightedText());
        mThumbColor.insert(false, palette.light());
        mTextColor.insert(true, palette.highlight().color());
        mTextColor.insert(false, palette.dark().color());
        mOpacity = 1.0;
    }

    // display the icons on the knob
    if (showIcons)
    {
        mThumbText.insert(true, QChar(0xf00c));
        mThumbText.insert(false, QChar(0xf00d));
    }
    else
    {
        mThumbText.insert(true, "");
        mThumbText.insert(false, "");
    }
}

ToggleButton::~ToggleButton()
{
    delete mAnimation;
}

void ToggleButton::overrideTrackColor(bool checked, QColor color)
{
    mTrackColor.insert(checked, color);
}

void ToggleButton::overrideThumbColor(bool checked, QColor color)
{
    mThumbColor.insert(checked, color);
}

void ToggleButton::overrideTextColor(bool checked, QColor color)
{
    mTextColor.insert(checked, color);
}

void ToggleButton::overrideOpacity(int opacity)
{
    mOpacity = opacity;
}

QSize ToggleButton::sizeHint() const
{
    int w = 4 * mTrackRadius + 2 * mMargin;
    int h = 2 * mTrackRadius + 2 * mMargin;

    return QSize(w, h);
}

void ToggleButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPainter::RenderHints m_paintFlags = QPainter::RenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    p.setRenderHints(m_paintFlags, true);
    p.setPen(Qt::NoPen);
    bool check = isChecked();
    qreal trackOpacity = mOpacity;
    qreal textOpacity = 1.0;
    qreal thumbOpacity = 1.0;
    QBrush trackBrush;
    QBrush thumbBrush;
    QColor textColor;

    if (this->isEnabled())
    {

        trackBrush = mTrackColor[check];
        thumbBrush = mThumbColor[check];
        textColor = mTextColor[check];
    }
    else
    {
        trackOpacity *= 0.8;
        trackBrush = this->palette().shadow();
        thumbBrush = this->palette().mid();
        textColor = this->palette().shadow().color();
    }

    p.setBrush(trackBrush);
    p.setOpacity(trackOpacity);
    p.drawRoundedRect(mMargin, mMargin, width() - 2 * mMargin, height() - 2 * mMargin, mTrackRadius, mTrackRadius);

    p.setBrush(thumbBrush);
    p.setOpacity(thumbOpacity);
    p.drawEllipse(mOffset - mThumbRadius, mBaseOffset - mThumbRadius, 2 * mThumbRadius, 2 * mThumbRadius);

    p.setPen(textColor);
    p.setOpacity(textOpacity);

    QFont font = QFont();
    font.setFamily("FontAwesome");

    font.setPixelSize(.8 * mThumbRadius);
    p.setFont(font);

    p.drawText(QRectF(mOffset - mThumbRadius,
        mBaseOffset - mThumbRadius,
        2 * mThumbRadius,
        2 * mThumbRadius),
        Qt::AlignCenter,
        mThumbText.value(check));
}

void ToggleButton::resizeEvent(QResizeEvent* e)
{
    QAbstractButton::resizeEvent(e);
    mOffset = mEndOffset.value(isChecked());
}

void ToggleButton::mouseReleaseEvent(QMouseEvent  *e)
{
    QAbstractButton::mouseReleaseEvent(e);
    if (e->button() == Qt::LeftButton)
    {
        mAnimation->setDuration(120);
        mAnimation->setPropertyName("mOffset");
        mAnimation->setStartValue(mOffset);
        mAnimation->setEndValue(mEndOffset[isChecked()]);
        mAnimation->start();
    }
}

void ToggleButton::enterEvent(QEnterEvent * event)
{
    setCursor(Qt::PointingHandCursor);
    QAbstractButton::enterEvent(event);
}

void ToggleButton::setChecked(bool checked)
{
    QAbstractButton::setChecked(checked);
    mOffset = mEndOffset.value(checked);
}

int ToggleButton::offset()
{
    return mOffset;
}

void ToggleButton::setOffset(int value)
{
    mOffset = value;
    update();
}
