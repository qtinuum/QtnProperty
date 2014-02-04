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

const QtnEnumInfo& COLOR::info()
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

const QtnEnumInfo& FLAGS::info()
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

QtnPropertySetSubPropertySetType::QtnPropertySetSubPropertySetType(QObject* parent)
    : QtnPropertySet(parent)
    , SwitchProperty(*new QtnPropertyBool(this))
    , ReadOnlyString(*new QtnPropertyQStringCallback(this))
    , FileNameProperty(*new QtnPropertyQString(this))
    , FolderNameProperty(*new QtnPropertyQString(this))
    , StringFromList(*new QtnPropertyQString(this))
    , CircleShapeColor(*new QtnPropertyQColor(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetSubPropertySetType::~QtnPropertySetSubPropertySetType()
{
    disconnectSlots();
}

QtnPropertySetSubPropertySetType& QtnPropertySetSubPropertySetType::operator=(const QtnPropertySetSubPropertySetType& other)
{
    SwitchProperty = other.SwitchProperty;
    ReadOnlyString = other.ReadOnlyString;
    FileNameProperty = other.FileNameProperty;
    FolderNameProperty = other.FolderNameProperty;
    StringFromList = other.StringFromList;
    CircleShapeColor = other.CircleShapeColor;

    return *this;
}

QtnPropertySet* QtnPropertySetSubPropertySetType::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetSubPropertySetType(parentForNew);
}

QtnPropertySet* QtnPropertySetSubPropertySetType::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetSubPropertySetType* p = new QtnPropertySetSubPropertySetType(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetSubPropertySetType::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    QtnPropertySetSubPropertySetType* theCopyFrom = qobject_cast<QtnPropertySetSubPropertySetType*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->SwitchProperty.state() & ignoreMask))
    {
        SwitchProperty = theCopyFrom->SwitchProperty;
    }

    if (!(theCopyFrom->ReadOnlyString.state() & ignoreMask))
    {
        ReadOnlyString = theCopyFrom->ReadOnlyString;
    }

    if (!(theCopyFrom->FileNameProperty.state() & ignoreMask))
    {
        FileNameProperty = theCopyFrom->FileNameProperty;
    }

    if (!(theCopyFrom->FolderNameProperty.state() & ignoreMask))
    {
        FolderNameProperty = theCopyFrom->FolderNameProperty;
    }

    if (!(theCopyFrom->StringFromList.state() & ignoreMask))
    {
        StringFromList = theCopyFrom->StringFromList;
    }

    if (!(theCopyFrom->CircleShapeColor.state() & ignoreMask))
    {
        CircleShapeColor = theCopyFrom->CircleShapeColor;
    }

    return true;
}

void QtnPropertySetSubPropertySetType::init()
{
    static QString SubPropertySet_name = tr("SubPropertySet");
    setName(SubPropertySet_name);
    static QString description = "This property set is part of the root property set.";
    setDescription(description);
    setState(QtnPropertyStateImmutable);
    
    // start children initialization
    static QString SwitchProperty_name = tr("SwitchProperty");
    SwitchProperty.setName(SwitchProperty_name);
    static QString SwitchProperty_description = "Boolean property with customized True/False values.";
    SwitchProperty.setDescription(SwitchProperty_description);
    SwitchProperty.setValue(true);
    static QString ReadOnlyString_name = tr("ReadOnlyString");
    ReadOnlyString.setName(ReadOnlyString_name);
    ReadOnlyString.setCallbackValueGet([this] ()->QString {
                if (SwitchProperty)
                    return "Switch is on";
                else
                    return "Switch is off";
            });
    static QString ReadOnlyString_description = "This property is callback and read-only.";
    ReadOnlyString.setDescription(ReadOnlyString_description);
    ReadOnlyString.setState(QtnPropertyStateImmutable);
    static QString FileNameProperty_name = tr("FileNameProperty");
    FileNameProperty.setName(FileNameProperty_name);
    static QString FileNameProperty_description = "QString property tuned to handle file names.";
    FileNameProperty.setDescription(FileNameProperty_description);
    FileNameProperty.setValue("~/test_file.txt");
    static QString FolderNameProperty_name = tr("FolderNameProperty");
    FolderNameProperty.setName(FolderNameProperty_name);
    static QString FolderNameProperty_description = "QString property tuned to handle folder names.";
    FolderNameProperty.setDescription(FolderNameProperty_description);
    FolderNameProperty.setValue("/var");
    static QString StringFromList_name = tr("StringFromList");
    StringFromList.setName(StringFromList_name);
    static QString StringFromList_description = "QString property with list of acepted values (one, two, three, four).";
    StringFromList.setDescription(StringFromList_description);
    StringFromList.setValue("two");
    static QString CircleShapeColor_name = tr("CircleShapeColor");
    CircleShapeColor.setName(CircleShapeColor_name);
    static QString CircleShapeColor_description = "QColor property with delegate tuned to draw circle";
    CircleShapeColor.setDescription(CircleShapeColor_description);
    CircleShapeColor.setValue(QColor(255, 100, 100));
    // end children initialization
}

void QtnPropertySetSubPropertySetType::connectSlots()
{
}

void QtnPropertySetSubPropertySetType::disconnectSlots()
{
}

void QtnPropertySetSubPropertySetType::connectDelegates()
{
    SwitchProperty.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "ComboBox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
    FileNameProperty.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        info->attributes["acceptMode"] = QFileDialog::AcceptSave;
        info->attributes["invalidColor"] = QColor(Qt::red);
        info->attributes["nameFilters"] = QStringList() << "Text files (*.txt)" << "All files (*)";
        return info.take();
    });
    FolderNameProperty.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        info->attributes["fileMode"] = QFileDialog::DirectoryOnly;
        info->attributes["invalidColor"] = QColor(Qt::blue);
        return info.take();
    });
    StringFromList.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "List";
        info->attributes["items"] = QStringList() << "one" << "two" << "three" << "four";
        return info.take();
    });
    CircleShapeColor.setDelegateCallback([] () -> const QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["shape"] = QtnColorDelegateShapeCircle;
        return info.take();
    });
}

QtnPropertySetSamplePS::QtnPropertySetSamplePS(QObject* parent)
    : QtnPropertySet(parent)
    , BoolProperty(*new QtnPropertyBool(this))
    , DoubleProperty(*new QtnPropertyDouble(this))
    , FloatProperty(*new QtnPropertyFloat(this))
    , IntProperty(*new QtnPropertyInt(this))
    , UIntProperty(*new QtnPropertyUInt(this))
    , EnumProperty(*new QtnPropertyEnum(this))
    , EnumFlagsProperty(*new QtnPropertyEnumFlags(this))
    , QStringValue(*new QtnPropertyQString(this))
    , QPointProperty(*new QtnPropertyQPoint(this))
    , QSizeProperty(*new QtnPropertyQSize(this))
    , QRectProperty(*new QtnPropertyQRect(this))
    , QColorProperty(*new QtnPropertyQColor(this))
    , QFontProperty(*new QtnPropertyQFont(this))
    , EnableSubPropertySet(*new QtnPropertyBool(this))
    , SubPropertySet(*new QtnPropertySetSubPropertySetType(this))
{
    init();
    connectSlots();
    connectDelegates();
}

QtnPropertySetSamplePS::~QtnPropertySetSamplePS()
{
    disconnectSlots();
}

QtnPropertySetSamplePS& QtnPropertySetSamplePS::operator=(const QtnPropertySetSamplePS& other)
{
    BoolProperty = other.BoolProperty;
    DoubleProperty = other.DoubleProperty;
    FloatProperty = other.FloatProperty;
    IntProperty = other.IntProperty;
    UIntProperty = other.UIntProperty;
    EnumProperty = other.EnumProperty;
    EnumFlagsProperty = other.EnumFlagsProperty;
    QStringValue = other.QStringValue;
    QPointProperty = other.QPointProperty;
    QSizeProperty = other.QSizeProperty;
    QRectProperty = other.QRectProperty;
    QColorProperty = other.QColorProperty;
    QFontProperty = other.QFontProperty;
    EnableSubPropertySet = other.EnableSubPropertySet;
    SubPropertySet = other.SubPropertySet;

    return *this;
}

QtnPropertySet* QtnPropertySetSamplePS::createNewImpl(QObject* parentForNew) const
{
    return new QtnPropertySetSamplePS(parentForNew);
}

QtnPropertySet* QtnPropertySetSamplePS::createCopyImpl(QObject* parentForCopy) const
{
    QtnPropertySetSamplePS* p = new QtnPropertySetSamplePS(parentForCopy);
    *p = *this;
    return p;
}

bool QtnPropertySetSamplePS::copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask)
{
    QtnPropertySetSamplePS* theCopyFrom = qobject_cast<QtnPropertySetSamplePS*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->BoolProperty.state() & ignoreMask))
    {
        BoolProperty = theCopyFrom->BoolProperty;
    }

    if (!(theCopyFrom->DoubleProperty.state() & ignoreMask))
    {
        DoubleProperty = theCopyFrom->DoubleProperty;
    }

    if (!(theCopyFrom->FloatProperty.state() & ignoreMask))
    {
        FloatProperty = theCopyFrom->FloatProperty;
    }

    if (!(theCopyFrom->IntProperty.state() & ignoreMask))
    {
        IntProperty = theCopyFrom->IntProperty;
    }

    if (!(theCopyFrom->UIntProperty.state() & ignoreMask))
    {
        UIntProperty = theCopyFrom->UIntProperty;
    }

    if (!(theCopyFrom->EnumProperty.state() & ignoreMask))
    {
        EnumProperty = theCopyFrom->EnumProperty;
    }

    if (!(theCopyFrom->EnumFlagsProperty.state() & ignoreMask))
    {
        EnumFlagsProperty = theCopyFrom->EnumFlagsProperty;
    }

    if (!(theCopyFrom->QStringValue.state() & ignoreMask))
    {
        QStringValue = theCopyFrom->QStringValue;
    }

    if (!(theCopyFrom->QPointProperty.state() & ignoreMask))
    {
        QPointProperty = theCopyFrom->QPointProperty;
    }

    if (!(theCopyFrom->QSizeProperty.state() & ignoreMask))
    {
        QSizeProperty = theCopyFrom->QSizeProperty;
    }

    if (!(theCopyFrom->QRectProperty.state() & ignoreMask))
    {
        QRectProperty = theCopyFrom->QRectProperty;
    }

    if (!(theCopyFrom->QColorProperty.state() & ignoreMask))
    {
        QColorProperty = theCopyFrom->QColorProperty;
    }

    if (!(theCopyFrom->QFontProperty.state() & ignoreMask))
    {
        QFontProperty = theCopyFrom->QFontProperty;
    }

    if (!(theCopyFrom->EnableSubPropertySet.state() & ignoreMask))
    {
        EnableSubPropertySet = theCopyFrom->EnableSubPropertySet;
    }

    SubPropertySet.copyValues(&theCopyFrom->SubPropertySet, ignoreMask);

    return true;
}

void QtnPropertySetSamplePS::init()
{
    static QString SamplePS_name = tr("SamplePS");
    setName(SamplePS_name);
    
    // start children initialization
    static QString BoolProperty_name = tr("BoolProperty");
    BoolProperty.setName(BoolProperty_name);
    static QString BoolProperty_description = "Property to hold boolean values.";
    BoolProperty.setDescription(BoolProperty_description);
    BoolProperty.setValue(false);
    static QString DoubleProperty_name = tr("DoubleProperty");
    DoubleProperty.setName(DoubleProperty_name);
    static QString DoubleProperty_description = "Property to hold double values in range [10, 20].";
    DoubleProperty.setDescription(DoubleProperty_description);
    DoubleProperty.setMaxValue(20);
    DoubleProperty.setMinValue(10);
    DoubleProperty.setStepValue(0.5);
    DoubleProperty.setValue(12.3);
    static QString FloatProperty_name = tr("FloatProperty");
    FloatProperty.setName(FloatProperty_name);
    static QString FloatProperty_description = "Property to hold float values in range [-10, 0].";
    FloatProperty.setDescription(FloatProperty_description);
    FloatProperty.setMaxValue(0);
    FloatProperty.setMinValue(-10);
    FloatProperty.setStepValue(0.5);
    FloatProperty.setValue(-3.5);
    static QString IntProperty_name = tr("IntProperty");
    IntProperty.setName(IntProperty_name);
    static QString IntProperty_description = "Property to hold integer values with changing step 15.";
    IntProperty.setDescription(IntProperty_description);
    IntProperty.setStepValue(15);
    IntProperty.setValue(10);
    static QString UIntProperty_name = tr("UIntProperty");
    UIntProperty.setName(UIntProperty_name);
    static QString UIntProperty_description = "Property to hold unsigned integer values in range [100, 200].";
    UIntProperty.setDescription(UIntProperty_description);
    UIntProperty.setMaxValue(200);
    UIntProperty.setMinValue(100);
    UIntProperty.setValue(100);
    static QString EnumProperty_name = tr("EnumProperty");
    EnumProperty.setName(EnumProperty_name);
    static QString EnumProperty_description = "Property to hold enum value (color).";
    EnumProperty.setDescription(EnumProperty_description);
    EnumProperty.setEnumInfo(&COLOR::info());
    EnumProperty.setValue(COLOR::red);
    static QString EnumFlagsProperty_name = tr("EnumFlagsProperty");
    EnumFlagsProperty.setName(EnumFlagsProperty_name);
    static QString EnumFlagsProperty_description = "Property to hold combination of enum values (options).";
    EnumFlagsProperty.setDescription(EnumFlagsProperty_description);
    EnumFlagsProperty.setEnumInfo(&FLAGS::info());
    EnumFlagsProperty.setValue(FLAGS::opt2);
    static QString QStringValue_name = tr("QStringValue");
    QStringValue.setName(QStringValue_name);
    static QString QStringValue_description = "Property to hold QString value.";
    QStringValue.setDescription(QStringValue_description);
    QStringValue.setValue("Hello world!");
    static QString QPointProperty_name = tr("QPointProperty");
    QPointProperty.setName(QPointProperty_name);
    static QString QPointProperty_description = "Property to hold QPoint value.";
    QPointProperty.setDescription(QPointProperty_description);
    QPointProperty.setValue(QPoint(-10, 10));
    static QString QSizeProperty_name = tr("QSizeProperty");
    QSizeProperty.setName(QSizeProperty_name);
    static QString QSizeProperty_description = "Property to hold QSize value.";
    QSizeProperty.setDescription(QSizeProperty_description);
    QSizeProperty.setValue(QSize(100, 200));
    static QString QRectProperty_name = tr("QRectProperty");
    QRectProperty.setName(QRectProperty_name);
    static QString QRectProperty_description = "Property to hold QRect value.";
    QRectProperty.setDescription(QRectProperty_description);
    QRectProperty.setValue(QRect(10, 10, 200, 200));
    static QString QColorProperty_name = tr("QColorProperty");
    QColorProperty.setName(QColorProperty_name);
    static QString QColorProperty_description = "Property to hold QColor value.";
    QColorProperty.setDescription(QColorProperty_description);
    QColorProperty.setValue(Qt::blue);
    static QString QFontProperty_name = tr("QFontProperty");
    QFontProperty.setName(QFontProperty_name);
    static QString QFontProperty_description = "Property to hold QFont value.";
    QFontProperty.setDescription(QFontProperty_description);
    QFontProperty.setValue(QFont("Sans Serif", 14));
    static QString EnableSubPropertySet_name = tr("EnableSubPropertySet");
    EnableSubPropertySet.setName(EnableSubPropertySet_name);
    static QString EnableSubPropertySet_description = "Enable/Disable Sub-PropertySet.";
    EnableSubPropertySet.setDescription(EnableSubPropertySet_description);
    EnableSubPropertySet.setValue(false);
    static QString SubPropertySet_name = tr("SubPropertySet");
    SubPropertySet.setName(SubPropertySet_name);
    static QString SubPropertySet_description = "This property set is part of the root property set.";
    SubPropertySet.setDescription(SubPropertySet_description);
    SubPropertySet.setState(QtnPropertyStateImmutable);
    // end children initialization
}

void QtnPropertySetSamplePS::connectSlots()
{
    QObject::connect(&EnableSubPropertySet, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange);
}

void QtnPropertySetSamplePS::disconnectSlots()
{
    QObject::disconnect(&EnableSubPropertySet, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange);
}

void QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    
            SubPropertySet.switchState(QtnPropertyStateImmutable, !EnableSubPropertySet);
        
}

void QtnPropertySetSamplePS::connectDelegates()
{
}
