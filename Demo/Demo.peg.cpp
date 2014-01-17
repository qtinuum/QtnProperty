#include "Demo.peg.h"

#include <QFileDialog>
static QtnEnumInfo& create_COLOR_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(COLOR::red, "red"));
    staticValues.append(QtnEnumValueInfo(COLOR::blue, "blue"));
    staticValues.append(QtnEnumValueInfo(COLOR::green, "green"));
    
    static QtnEnumInfo enumInfo("COLOR", staticValues);
    return enumInfo;
}

QtnEnumInfo& COLOR::info()
{
    static QtnEnumInfo& enumInfo = create_COLOR_info();
    return enumInfo;
}
static QtnEnumInfo& create_FLAGS_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(FLAGS::opt1, "Option1"));
    staticValues.append(QtnEnumValueInfo(FLAGS::opt2, "Option2"));
    staticValues.append(QtnEnumValueInfo(FLAGS::opt3, "Option3"));
    
    static QtnEnumInfo enumInfo("FLAGS", staticValues);
    return enumInfo;
}

QtnEnumInfo& FLAGS::info()
{
    static QtnEnumInfo& enumInfo = create_FLAGS_info();
    return enumInfo;
}

QtnPropertySetInternal::QtnPropertySetInternal(QObject* parent)
    : QtnPropertySet(parent)
    , name(*new QtnPropertyQString(this))
    , rect(*new QtnPropertyQRect(this))
    , item1(*new QtnPropertyQString(this))
    , item2(*new QtnPropertyQString(this))
    , item3(*new QtnPropertyQString(this))
    , item4(*new QtnPropertyQString(this))
    , item5(*new QtnPropertyQString(this))
    , item6(*new QtnPropertyBool(this))
    , item7(*new QtnPropertyBool(this))
    , item8(*new QtnPropertyEnum(this))
    , item9(*new QtnPropertyQColor(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetInternal::~QtnPropertySetInternal()
{
    disconnectSlots();
}

QtnPropertySetInternal& QtnPropertySetInternal::operator=(const QtnPropertySetInternal& other)
{
    name = other.name;
    rect = other.rect;
    item1 = other.item1;
    item2 = other.item2;
    item3 = other.item3;
    item4 = other.item4;
    item5 = other.item5;
    item6 = other.item6;
    item7 = other.item7;
    item8 = other.item8;
    item9 = other.item9;

    return *this;
}

QtnPropertySet* QtnPropertySetInternal::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetInternal(parentForNew);
}

QtnPropertySet* QtnPropertySetInternal::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetInternal* p = new QtnPropertySetInternal(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetInternal::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    QtnPropertySetInternal* theCopyFrom = qobject_cast<QtnPropertySetInternal*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->name.state() & ignoreMask))
    {
        name = theCopyFrom->name;
    }

    if (!(theCopyFrom->rect.state() & ignoreMask))
    {
        rect = theCopyFrom->rect;
    }

    if (!(theCopyFrom->item1.state() & ignoreMask))
    {
        item1 = theCopyFrom->item1;
    }

    if (!(theCopyFrom->item2.state() & ignoreMask))
    {
        item2 = theCopyFrom->item2;
    }

    if (!(theCopyFrom->item3.state() & ignoreMask))
    {
        item3 = theCopyFrom->item3;
    }

    if (!(theCopyFrom->item4.state() & ignoreMask))
    {
        item4 = theCopyFrom->item4;
    }

    if (!(theCopyFrom->item5.state() & ignoreMask))
    {
        item5 = theCopyFrom->item5;
    }

    if (!(theCopyFrom->item6.state() & ignoreMask))
    {
        item6 = theCopyFrom->item6;
    }

    if (!(theCopyFrom->item7.state() & ignoreMask))
    {
        item7 = theCopyFrom->item7;
    }

    if (!(theCopyFrom->item8.state() & ignoreMask))
    {
        item8 = theCopyFrom->item8;
    }

    if (!(theCopyFrom->item9.state() & ignoreMask))
    {
        item9 = theCopyFrom->item9;
    }

    return true;
}

void QtnPropertySetInternal::init()
{
    static QString Internal_name = tr("Internal");
    setName(Internal_name);
    setState(QtnPropertyStateImmutable);
    
    // start children initialization
    static QString name_name = tr("name");
    name.setName(name_name);
    static QString name_description = "Name of the item.";
    name.setDescription(name_description);
    name.setValue("Item");
    static QString rect_name = tr("rect");
    rect.setName(rect_name);
    rect.setValue(QRect(10, 23, 19, 2));
    static QString item1_name = tr("item1");
    item1.setName(item1_name);
    item1.setValue("Item1");
    static QString item2_name = tr("item2");
    item2.setName(item2_name);
    item2.setValue("Item2");
    static QString item3_name = tr("item3");
    item3.setName(item3_name);
    item3.setValue("Item3");
    static QString item4_name = tr("item4");
    item4.setName(item4_name);
    item4.setValue("Item4");
    static QString item5_name = tr("item5");
    item5.setName(item5_name);
    item5.setValue("Item5");
    static QString item6_name = tr("item6");
    item6.setName(item6_name);
    static QString item7_name = tr("item7");
    item7.setName(item7_name);
    static QString item8_name = tr("item8");
    item8.setName(item8_name);
    item8.setEnumInfo(&COLOR::info());
    item8.setValue(COLOR::red);
    static QString item9_name = tr("item9");
    item9.setName(item9_name);
    item9.setValue(QColor(100, 100, 100));
    // end children initialization
}

void QtnPropertySetInternal::connectSlots()
{
}

void QtnPropertySetInternal::disconnectSlots()
{
}

void QtnPropertySetInternal::connectDelegates()
{
    item7.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "ComboBox";
        return info.take();
    });
    item9.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["shape"] = QtnColorDelegateShapeCircle;
        return info.take();
    });
}

QtnPropertySetSubProperties::QtnPropertySetSubProperties(QObject* parent)
    : QtnPropertySet(parent)
    , intProp(*new QtnPropertyInt(this))
    , uintProp(*new QtnPropertyUInt(this))
    , doubleProp(*new QtnPropertyDouble(this))
    , floatProp(*new QtnPropertyFloat(this))
    , boolProp(*new QtnPropertyBool(this))
    , enumProp(*new QtnPropertyEnum(this))
    , colorProp(*new QtnPropertyQColor(this))
    , flagsProp(*new QtnPropertyEnumFlags(this))
    , fileName1(*new QtnPropertyQString(this))
    , fileName2(*new QtnPropertyQString(this))
    , pointProp(*new QtnPropertyQPoint(this))
    , sizeProp(*new QtnPropertyQSize(this))
    , fontProp(*new QtnPropertyQFont(this))
    , listProp(*new QtnPropertyQString(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetSubProperties::~QtnPropertySetSubProperties()
{
    disconnectSlots();
}

QtnPropertySetSubProperties& QtnPropertySetSubProperties::operator=(const QtnPropertySetSubProperties& other)
{
    intProp = other.intProp;
    uintProp = other.uintProp;
    doubleProp = other.doubleProp;
    floatProp = other.floatProp;
    boolProp = other.boolProp;
    enumProp = other.enumProp;
    colorProp = other.colorProp;
    flagsProp = other.flagsProp;
    fileName1 = other.fileName1;
    fileName2 = other.fileName2;
    pointProp = other.pointProp;
    sizeProp = other.sizeProp;
    fontProp = other.fontProp;
    listProp = other.listProp;

    return *this;
}

QtnPropertySet* QtnPropertySetSubProperties::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetSubProperties(parentForNew);
}

QtnPropertySet* QtnPropertySetSubProperties::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetSubProperties* p = new QtnPropertySetSubProperties(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetSubProperties::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    QtnPropertySetSubProperties* theCopyFrom = qobject_cast<QtnPropertySetSubProperties*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->intProp.state() & ignoreMask))
    {
        intProp = theCopyFrom->intProp;
    }

    if (!(theCopyFrom->uintProp.state() & ignoreMask))
    {
        uintProp = theCopyFrom->uintProp;
    }

    if (!(theCopyFrom->doubleProp.state() & ignoreMask))
    {
        doubleProp = theCopyFrom->doubleProp;
    }

    if (!(theCopyFrom->floatProp.state() & ignoreMask))
    {
        floatProp = theCopyFrom->floatProp;
    }

    if (!(theCopyFrom->boolProp.state() & ignoreMask))
    {
        boolProp = theCopyFrom->boolProp;
    }

    if (!(theCopyFrom->enumProp.state() & ignoreMask))
    {
        enumProp = theCopyFrom->enumProp;
    }

    if (!(theCopyFrom->colorProp.state() & ignoreMask))
    {
        colorProp = theCopyFrom->colorProp;
    }

    if (!(theCopyFrom->flagsProp.state() & ignoreMask))
    {
        flagsProp = theCopyFrom->flagsProp;
    }

    if (!(theCopyFrom->fileName1.state() & ignoreMask))
    {
        fileName1 = theCopyFrom->fileName1;
    }

    if (!(theCopyFrom->fileName2.state() & ignoreMask))
    {
        fileName2 = theCopyFrom->fileName2;
    }

    if (!(theCopyFrom->pointProp.state() & ignoreMask))
    {
        pointProp = theCopyFrom->pointProp;
    }

    if (!(theCopyFrom->sizeProp.state() & ignoreMask))
    {
        sizeProp = theCopyFrom->sizeProp;
    }

    if (!(theCopyFrom->fontProp.state() & ignoreMask))
    {
        fontProp = theCopyFrom->fontProp;
    }

    if (!(theCopyFrom->listProp.state() & ignoreMask))
    {
        listProp = theCopyFrom->listProp;
    }

    return true;
}

void QtnPropertySetSubProperties::init()
{
    static QString Supplementary_name = tr("Supplementary");
    setName(Supplementary_name);
    static QString description = "Supplementary <b>description</b>";
    setDescription(description);
    
    // start children initialization
    static QString intProp_name = tr("intProp");
    intProp.setName(intProp_name);
    intProp.setMaxValue(10);
    intProp.setMinValue(-10);
    intProp.setStepValue(2);
    intProp.setValue(-6);
    static QString uintProp_name = tr("uintProp");
    uintProp.setName(uintProp_name);
    uintProp.setValue(323);
    static QString doubleProp_name = tr("doubleProp");
    doubleProp.setName(doubleProp_name);
    doubleProp.setValue(762.453);
    static QString floatProp_name = tr("floatProp");
    floatProp.setName(floatProp_name);
    floatProp.setMaxValue(1.f);
    floatProp.setMinValue(0);
    floatProp.setStepValue(0.001f);
    floatProp.setValue(0.234f);
    static QString boolProp_name = tr("boolProp");
    boolProp.setName(boolProp_name);
    boolProp.setValue(true);
    static QString enumProp_name = tr("enumProp");
    enumProp.setName(enumProp_name);
    enumProp.setEnumInfo(&COLOR::info());
    enumProp.setValue(COLOR::green);
    static QString colorProp_name = tr("colorProp");
    colorProp.setName(colorProp_name);
    colorProp.setValue("red");
    static QString flagsProp_name = tr("flagsProp");
    flagsProp.setName(flagsProp_name);
    flagsProp.setEnumInfo(&FLAGS::info());
    flagsProp.setValue(FLAGS::opt2 | FLAGS::opt3);
    static QString fileName1_name = tr("fileName1");
    fileName1.setName(fileName1_name);
    static QString fileName2_name = tr("fileName2");
    fileName2.setName(fileName2_name);
    fileName2.setValue("file name");
    static QString pointProp_name = tr("pointProp");
    pointProp.setName(pointProp_name);
    static QString pointProp_description = "Point property.";
    pointProp.setDescription(pointProp_description);
    pointProp.setValue(QPoint(12, 2));
    static QString sizeProp_name = tr("sizeProp");
    sizeProp.setName(sizeProp_name);
    static QString sizeProp_description = "Size property.";
    sizeProp.setDescription(sizeProp_description);
    sizeProp.setValue(QSize(22, 34));
    static QString fontProp_name = tr("fontProp");
    fontProp.setName(fontProp_name);
    static QString fontProp_description = "Font property.";
    fontProp.setDescription(fontProp_description);
    fontProp.setValue(QFont());
    static QString listProp_name = tr("listProp");
    listProp.setName(listProp_name);
    listProp.setValue("one");
    // end children initialization
}

void QtnPropertySetSubProperties::connectSlots()
{
}

void QtnPropertySetSubProperties::disconnectSlots()
{
}

void QtnPropertySetSubProperties::connectDelegates()
{
    boolProp.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "ComboBox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
    fileName1.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        return info.take();
    });
    fileName2.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        info->attributes["acceptMode"] = QFileDialog::AcceptSave;
        info->attributes["invalidColor"] = QColor(Qt::blue);
        info->attributes["nameFilters"] = QStringList() << "PDF files (*.pdf)" << "All files (*)";
        return info.take();
    });
    listProp.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "List";
        info->attributes["items"] = QStringList() << "one" << "two" << "three";
        return info.take();
    });
}

QtnPropertySetMain::QtnPropertySetMain(QObject* parent)
    : QtnPropertySet(parent)
    , count(*new QtnPropertyInt(this))
    , isValid(*new QtnPropertyBool(this))
    , Internal(*new QtnPropertySetInternal(this))
    , flag(*new QtnPropertyBool(this))
    , Supplementary(*new QtnPropertySetSubProperties(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetMain::~QtnPropertySetMain()
{
    disconnectSlots();
}

QtnPropertySetMain& QtnPropertySetMain::operator=(const QtnPropertySetMain& other)
{
    count = other.count;
    isValid = other.isValid;
    Internal = other.Internal;
    flag = other.flag;
    Supplementary = other.Supplementary;

    return *this;
}

QtnPropertySet* QtnPropertySetMain::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetMain(parentForNew);
}

QtnPropertySet* QtnPropertySetMain::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetMain* p = new QtnPropertySetMain(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetMain::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    QtnPropertySetMain* theCopyFrom = qobject_cast<QtnPropertySetMain*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->count.state() & ignoreMask))
    {
        count = theCopyFrom->count;
    }

    if (!(theCopyFrom->isValid.state() & ignoreMask))
    {
        isValid = theCopyFrom->isValid;
    }

    Internal.copyValues(&theCopyFrom->Internal, ignoreMask);

    if (!(theCopyFrom->flag.state() & ignoreMask))
    {
        flag = theCopyFrom->flag;
    }

    Supplementary.copyValues(&theCopyFrom->Supplementary, ignoreMask);

    return true;
}

void QtnPropertySetMain::init()
{
    static QString Main_name = tr("Main");
    setName(Main_name);
    
    // start children initialization
    static QString count_name = tr("count");
    count.setName(count_name);
    static QString count_description = "Count is a number of items.";
    count.setDescription(count_description);
    count.setValue(5);
    static QString isValid_name = tr("isValid");
    isValid.setName(isValid_name);
    static QString isValid_description = "Validation flag.";
    isValid.setDescription(isValid_description);
    isValid.setValue(false);
    static QString Internal_name = tr("Internal");
    Internal.setName(Internal_name);
    Internal.setState(QtnPropertyStateImmutable);
    static QString flag_name = tr("flag");
    flag.setName(flag_name);
    flag.setValue(false);
    static QString Supplementary_name = tr("Supplementary");
    Supplementary.setName(Supplementary_name);
    static QString Supplementary_description = "Supplementary <b>description</b>";
    Supplementary.setDescription(Supplementary_description);
    // end children initialization
}

void QtnPropertySetMain::connectSlots()
{
    QObject::connect(&isValid, &QtnProperty::propertyDidChange, this, &QtnPropertySetMain::on_isValid_propertyDidChange);
}

void QtnPropertySetMain::disconnectSlots()
{
    QObject::disconnect(&isValid, &QtnProperty::propertyDidChange, this, &QtnPropertySetMain::on_isValid_propertyDidChange);
}

void QtnPropertySetMain::on_isValid_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    
            Internal.switchState(QtnPropertyStateImmutable, !isValid);
        
}

void QtnPropertySetMain::connectDelegates()
{
    flag.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "ComboBox";
        return info.take();
    });
}
