#include "Demo.peg.h"

#include <QFileDialog>
static Qtinuum::EnumInfo &create_COLOR_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::red, "red"));
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::blue, "blue"));
    staticValues.append(Qtinuum::EnumValueInfo(COLOR::green, "green"));
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &COLOR::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_COLOR_info();
    return enumInfo;
}
static Qtinuum::EnumInfo &create_FLAGS_info()
{
    QVector<Qtinuum::EnumValueInfo> staticValues;
    staticValues.append(Qtinuum::EnumValueInfo(FLAGS::opt1, "Option1"));
    staticValues.append(Qtinuum::EnumValueInfo(FLAGS::opt2, "Option2"));
    staticValues.append(Qtinuum::EnumValueInfo(FLAGS::opt3, "Option3"));
    
    static Qtinuum::EnumInfo enumInfo(staticValues);
    return enumInfo;
}

Qtinuum::EnumInfo &FLAGS::info()
{
    static Qtinuum::EnumInfo &enumInfo = create_FLAGS_info();
    return enumInfo;
}

PropertySetInternal::PropertySetInternal(QObject *parent)
    : Qtinuum::Property(parent)
    , name(*new Qtinuum::PropertyQString(this))
    , rect(*new Qtinuum::PropertyQRect(this))
    , item1(*new Qtinuum::PropertyQString(this))
    , item2(*new Qtinuum::PropertyQString(this))
    , item3(*new Qtinuum::PropertyQString(this))
    , item4(*new Qtinuum::PropertyQString(this))
    , item5(*new Qtinuum::PropertyQString(this))
    , item6(*new Qtinuum::PropertyBool(this))
    , item7(*new Qtinuum::PropertyBool(this))
    , item8(*new Qtinuum::PropertyEnum(this))
    , item9(*new Qtinuum::PropertyQColor(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetInternal::~PropertySetInternal()
{
    disconnectSlots();
}

PropertySetInternal& PropertySetInternal::operator=(const PropertySetInternal& other)
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

Qtinuum::Property* PropertySetInternal::createNewImpl(QObject* parentForNew) const
{
    return new PropertySetInternal(parentForNew);
}

Qtinuum::Property* PropertySetInternal::createCopyImpl(QObject* parentForCopy) const
{
    PropertySetInternal* p = new PropertySetInternal(parentForCopy);
    *p = *this;
    return p;
}

void PropertySetInternal::init()
{
    static QString Internal_name = tr("Internal");
    setName(Internal_name);
    setState(qtn::PropertyStateImmutable);
    
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

void PropertySetInternal::connectSlots()
{
}

void PropertySetInternal::disconnectSlots()
{
}

void PropertySetInternal::connectDelegates()
{
    item7.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "ComboBox";
        return info.take();
    });
    item9.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->attributes["shape"] = qtn::QColorDelegateShapeCircle;
        return info.take();
    });
}

PropertySetSubProperties::PropertySetSubProperties(QObject *parent)
    : qtn::PropertyBool(parent)
    , intProp(*new Qtinuum::PropertyInt(this))
    , uintProp(*new Qtinuum::PropertyUInt(this))
    , doubleProp(*new Qtinuum::PropertyDouble(this))
    , floatProp(*new Qtinuum::PropertyFloat(this))
    , boolProp(*new Qtinuum::PropertyBool(this))
    , enumProp(*new Qtinuum::PropertyEnum(this))
    , colorProp(*new Qtinuum::PropertyQColor(this))
    , flagsProp(*new Qtinuum::PropertyEnumFlags(this))
    , fileName1(*new Qtinuum::PropertyQString(this))
    , fileName2(*new Qtinuum::PropertyQString(this))
    , pointProp(*new Qtinuum::PropertyQPoint(this))
    , sizeProp(*new Qtinuum::PropertyQSize(this))
    , fontProp(*new Qtinuum::PropertyQFont(this))
    , listProp(*new Qtinuum::PropertyQString(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetSubProperties::~PropertySetSubProperties()
{
    disconnectSlots();
}

PropertySetSubProperties& PropertySetSubProperties::operator=(const PropertySetSubProperties& other)
{
    qtn::PropertyBool::operator=(other);
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

Qtinuum::Property* PropertySetSubProperties::createNewImpl(QObject* parentForNew) const
{
    return new PropertySetSubProperties(parentForNew);
}

Qtinuum::Property* PropertySetSubProperties::createCopyImpl(QObject* parentForCopy) const
{
    PropertySetSubProperties* p = new PropertySetSubProperties(parentForCopy);
    *p = *this;
    return p;
}

void PropertySetSubProperties::init()
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

void PropertySetSubProperties::connectSlots()
{
    QObject::connect(this, &Qtinuum::Property::propertyDidChange, this, &PropertySetSubProperties::on_propertyDidChange);
}

void PropertySetSubProperties::disconnectSlots()
{
    QObject::disconnect(this, &Qtinuum::Property::propertyDidChange, this, &PropertySetSubProperties::on_propertyDidChange);
}

void PropertySetSubProperties::on_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason)
{
    
            intProp.switchState(qtn::PropertyStateImmutable, !value());
        
}

void PropertySetSubProperties::connectDelegates()
{
    boolProp.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "ComboBox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
    fileName1.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "File";
        return info.take();
    });
    fileName2.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "File";
        info->attributes["acceptMode"] = QFileDialog::AcceptSave;
        info->attributes["invalidColor"] = QColor(Qt::blue);
        info->attributes["nameFilters"] = QStringList() << "PDF files (*.pdf)" << "All files (*)";
        return info.take();
    });
    listProp.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "List";
        info->attributes["items"] = QStringList() << "one" << "two" << "three";
        return info.take();
    });
}

PropertySetMain::PropertySetMain(QObject *parent)
    : Qtinuum::Property(parent)
    , count(*new Qtinuum::PropertyInt(this))
    , isValid(*new Qtinuum::PropertyBool(this))
    , Internal(*new PropertySetInternal(this))
    , flag(*new Qtinuum::PropertyBool(this))
    , Supplementary(*new PropertySetSubProperties(this))
{
    init();
    connectSlots();
    connectDelegates();
}

PropertySetMain::~PropertySetMain()
{
    disconnectSlots();
}

PropertySetMain& PropertySetMain::operator=(const PropertySetMain& other)
{
    count = other.count;
    isValid = other.isValid;
    Internal = other.Internal;
    flag = other.flag;
    Supplementary = other.Supplementary;

    return *this;
}

Qtinuum::Property* PropertySetMain::createNewImpl(QObject* parentForNew) const
{
    return new PropertySetMain(parentForNew);
}

Qtinuum::Property* PropertySetMain::createCopyImpl(QObject* parentForCopy) const
{
    PropertySetMain* p = new PropertySetMain(parentForCopy);
    *p = *this;
    return p;
}

void PropertySetMain::init()
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
    Internal.setState(qtn::PropertyStateImmutable);
    static QString flag_name = tr("flag");
    flag.setName(flag_name);
    flag.setValue(false);
    static QString Supplementary_name = tr("Supplementary");
    Supplementary.setName(Supplementary_name);
    static QString Supplementary_description = "Supplementary <b>description</b>";
    Supplementary.setDescription(Supplementary_description);
    // end children initialization
}

void PropertySetMain::connectSlots()
{
    QObject::connect(&isValid, &Qtinuum::Property::propertyDidChange, this, &PropertySetMain::on_isValid_propertyDidChange);
}

void PropertySetMain::disconnectSlots()
{
    QObject::disconnect(&isValid, &Qtinuum::Property::propertyDidChange, this, &PropertySetMain::on_isValid_propertyDidChange);
}

void PropertySetMain::on_isValid_propertyDidChange(const Qtinuum::Property *changedProperty, const Qtinuum::Property *firedProperty, Qtinuum::PropertyChangeReason reason)
{
    
            Internal.switchState(qtn::PropertyStateImmutable, !isValid);
        
}

void PropertySetMain::connectDelegates()
{
    flag.setDelegateCallback([] () -> const Qtinuum::PropertyDelegateInfo * {
        QScopedPointer<Qtinuum::PropertyDelegateInfo> info(new Qtinuum::PropertyDelegateInfo());
        info->name = "ComboBox";
        return info.take();
    });
}
