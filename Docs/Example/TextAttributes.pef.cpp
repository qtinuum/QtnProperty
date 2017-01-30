#include "TextAttibutes.peg.h"


QtnPropertySetTextAttributes::QtnPropertySetTextAttributes(QObject* parent)
    : QtnPropertySet(parent)
    , WordWrap(*new QtnPropertyBool(this))
    , Height(*new QtnPropertyInt(this))
    , Color(*new QtnPropertyQColor(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetTextAttributes::~QtnPropertySetTextAttributes()
{
    disconnectSlots();
}

QtnPropertySetTextAttributes& QtnPropertySetTextAttributes::operator=(const QtnPropertySetTextAttributes& other)
{
    Q_UNUSED(other);

    WordWrap = other.WordWrap;
    Height = other.Height;
    Color = other.Color;

    return *this;
}

QtnPropertySet* QtnPropertySetTextAttributes::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetTextAttributes(parentForNew);
}

QtnPropertySet* QtnPropertySetTextAttributes::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetTextAttributes* p = new QtnPropertySetTextAttributes(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetTextAttributes::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    Q_UNUSED(ignoreMask);

    QtnPropertySetTextAttributes* theCopyFrom = qobject_cast<QtnPropertySetTextAttributes*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->WordWrap.state() & ignoreMask))
    {
        WordWrap = theCopyFrom->WordWrap;
    }

    if (!(theCopyFrom->Height.state() & ignoreMask))
    {
        Height = theCopyFrom->Height;
    }

    if (!(theCopyFrom->Color.state() & ignoreMask))
    {
        Color = theCopyFrom->Color;
    }

    return true;
}

void QtnPropertySetTextAttributes::init()
{
    static QString TextAttributes_name = tr("TextAttributes");
    setName(TextAttributes_name);
    
    // start children initialization
    static QString WordWrap_name = tr("WordWrap");
    WordWrap.setName(WordWrap_name);
    static QString WordWrap_description = tr("Text word wrapping");
    WordWrap.setDescription(WordWrap_description);
    WordWrap.setValue(true);
    static QString Height_name = tr("Height");
    Height.setName(Height_name);
    static QString Height_description = tr("Text height");
    Height.setDescription(Height_description);
    Height.setMaxValue(100);
    Height.setMinValue(1);
    Height.setStepValue(1);
    Height.setValue(16);
    static QString Color_name = tr("Color");
    Color.setName(Color_name);
    static QString Color_description = tr("Foreground text color");
    Color.setDescription(Color_description);
    Color.setValue(QColor(0, 0, 0));
    // end children initialization
}

void QtnPropertySetTextAttributes::connectSlots()
{
}

void QtnPropertySetTextAttributes::disconnectSlots()
{
}

void QtnPropertySetTextAttributes::connectDelegates()
{
}
