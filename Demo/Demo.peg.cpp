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
    , EnableSubPropertySet(*new QtnPropertyBool(this))
    , SubPropertySet(*new QtnPropertySetSubPropertySetType(this))
    , QPointProperty(*new QtnPropertyQPoint(this))
    , QSizeProperty(*new QtnPropertyQSize(this))
    , QRectProperty(*new QtnPropertyQRect(this))
    , QColorProperty(*new QtnPropertyQColor(this))
    , QFontProperty(*new QtnPropertyQFont(this))
    , SubPropertySet2(*new QtnPropertySetSubPropertySetType(this))
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
    EnableSubPropertySet = other.EnableSubPropertySet;
    SubPropertySet = other.SubPropertySet;
    QPointProperty = other.QPointProperty;
    QSizeProperty = other.QSizeProperty;
    QRectProperty = other.QRectProperty;
    QColorProperty = other.QColorProperty;
    QFontProperty = other.QFontProperty;
    SubPropertySet2 = other.SubPropertySet2;

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

    if (!(theCopyFrom->EnableSubPropertySet.state() & ignoreMask))
    {
        EnableSubPropertySet = theCopyFrom->EnableSubPropertySet;
    }

    SubPropertySet.copyValues(&theCopyFrom->SubPropertySet, ignoreMask);

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

    SubPropertySet2.copyValues(&theCopyFrom->SubPropertySet2, ignoreMask);

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
    static QString SubPropertySet2_name = tr("SubPropertySet2");
    SubPropertySet2.setName(SubPropertySet2_name);
    SubPropertySet2.setState(QtnPropertyStateCollapsed);
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
