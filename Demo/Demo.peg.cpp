#include "Demo.peg.h"

#include <QFileDialog>
#include <QDebug>
static QtnEnumInfo& create_COLOR_info()
{
    QVector<QtnEnumValueInfo> staticValues;
    staticValues.append(QtnEnumValueInfo(COLOR::red, "red", "red"));
    staticValues.append(QtnEnumValueInfo(COLOR::blue, "blue", "blue"));
    staticValues.append(QtnEnumValueInfo(COLOR::green, "green", "green"));
    
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
    staticValues.append(QtnEnumValueInfo(FLAGS::opt1, "opt1", "Option1"));
    staticValues.append(QtnEnumValueInfo(FLAGS::opt2, "opt2", "Option2"));
    staticValues.append(QtnEnumValueInfo(FLAGS::opt3, "opt3", "Option3"));
    
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
    , SwitchProperty(*qtnCreateProperty<QtnPropertyBool>(this))
    , ReadOnlyString(*qtnCreateProperty<QtnPropertyQStringCallback>(this))
    , FileNameProperty(*qtnCreateProperty<QtnPropertyQString>(this))
    , FolderNameProperty(*qtnCreateProperty<QtnPropertyQString>(this))
    , StringFromList(*qtnCreateProperty<QtnPropertyQString>(this))
    , CircleShapeColor(*qtnCreateProperty<QtnPropertyQColor>(this))
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
    Q_UNUSED(other);

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
    Q_UNUSED(ignoreMask);

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
    SwitchProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "ComboBox";
        info->attributes["labelFalse"] = "Off";
        info->attributes["labelTrue"] = "On";
        return info.take();
    });
    FileNameProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        info->attributes["acceptMode"] = QFileDialog::AcceptSave;
        info->attributes["invalidColor"] = QColor(Qt::red);
        info->attributes["nameFilters"] = QStringList() << "Text files (*.txt)" << "All files (*)";
        return info.take();
    });
    FolderNameProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "File";
        info->attributes["fileMode"] = QFileDialog::DirectoryOnly;
        info->attributes["invalidColor"] = QColor(Qt::blue);
        return info.take();
    });
    StringFromList.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "List";
        info->attributes["items"] = QStringList() << "one" << "two" << "three" << "four";
        return info.take();
    });
    CircleShapeColor.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["shape"] = QtnColorDelegateShapeCircle;
        return info.take();
    });
}

QtnPropertySetSamplePS::QtnPropertySetSamplePS(QObject* parent)
    : QtnPropertySet(parent)
    , BoolProperty(*qtnCreateProperty<QtnPropertyBool>(this))
    , ButtonProperty(*qtnCreateProperty<QtnPropertyButton>(this))
    , ButtonLinkProperty(*qtnCreateProperty<QtnPropertyButton>(this))
    , RGBColor(*qtnCreateProperty<QtnPropertyABColor>(this))
    , ColorSolidDelegate(*qtnCreateProperty<QtnPropertyQColor>(this))
    , FloatPropertySliderBox(*qtnCreateProperty<QtnPropertyFloat>(this))
    , DoubleProperty(*qtnCreateProperty<QtnPropertyDouble>(this))
    , FloatProperty(*qtnCreateProperty<QtnPropertyFloat>(this))
    , IntProperty(*qtnCreateProperty<QtnPropertyInt>(this))
    , IntPropertyComboBox(*qtnCreateProperty<QtnPropertyInt>(this))
    , UIntProperty(*qtnCreateProperty<QtnPropertyUInt>(this))
    , EnumProperty(*qtnCreateProperty<QtnPropertyEnum>(this))
    , EnumFlagsProperty(*qtnCreateProperty<QtnPropertyEnumFlags>(this))
    , QStringValue(*qtnCreateProperty<QtnPropertyQString>(this))
    , EnableSubPropertySet(*qtnCreateProperty<QtnPropertyBool>(this))
    , SubPropertySet(*qtnCreateProperty<QtnPropertySetSubPropertySetType>(this))
    , QPointProperty(*qtnCreateProperty<QtnPropertyQPoint>(this))
    , QSizeProperty(*qtnCreateProperty<QtnPropertyQSize>(this))
    , QRectProperty(*qtnCreateProperty<QtnPropertyQRect>(this))
    , QPointFProperty(*qtnCreateProperty<QtnPropertyQPointF>(this))
    , QSizeFProperty(*qtnCreateProperty<QtnPropertyQSizeF>(this))
    , QRectFProperty(*qtnCreateProperty<QtnPropertyQRectF>(this))
    , QColorProperty(*qtnCreateProperty<QtnPropertyQColor>(this))
    , QFontProperty(*qtnCreateProperty<QtnPropertyQFont>(this))
    , FreqProperty(*qtnCreateProperty<QtnPropertyFreq>(this))
    , LayerProperty(*qtnCreateProperty<QtnPropertyLayer>(this))
    , BrushStyleProperty(*qtnCreateProperty<QtnPropertyQBrushStyle>(this))
    , PenWidthProperty(*qtnCreateProperty<QtnPropertyPenWidth>(this))
    , PenStyleProperty(*qtnCreateProperty<QtnPropertyQPenStyle>(this))
    , PenProperty(*qtnCreateProperty<QtnPropertyQPen>(this))
    , QStringCallbackProperty(*qtnCreateProperty<QtnPropertyQString>(this))
    , SubPropertySet2(*qtnCreateProperty<QtnPropertySetSubPropertySetType>(this))
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
    Q_UNUSED(other);

    BoolProperty = other.BoolProperty;
    ButtonProperty = other.ButtonProperty;
    ButtonLinkProperty = other.ButtonLinkProperty;
    RGBColor = other.RGBColor;
    ColorSolidDelegate = other.ColorSolidDelegate;
    FloatPropertySliderBox = other.FloatPropertySliderBox;
    DoubleProperty = other.DoubleProperty;
    FloatProperty = other.FloatProperty;
    IntProperty = other.IntProperty;
    IntPropertyComboBox = other.IntPropertyComboBox;
    UIntProperty = other.UIntProperty;
    EnumProperty = other.EnumProperty;
    EnumFlagsProperty = other.EnumFlagsProperty;
    QStringValue = other.QStringValue;
    EnableSubPropertySet = other.EnableSubPropertySet;
    SubPropertySet = other.SubPropertySet;
    QPointProperty = other.QPointProperty;
    QSizeProperty = other.QSizeProperty;
    QRectProperty = other.QRectProperty;
    QPointFProperty = other.QPointFProperty;
    QSizeFProperty = other.QSizeFProperty;
    QRectFProperty = other.QRectFProperty;
    QColorProperty = other.QColorProperty;
    QFontProperty = other.QFontProperty;
    FreqProperty = other.FreqProperty;
    LayerProperty = other.LayerProperty;
    BrushStyleProperty = other.BrushStyleProperty;
    PenWidthProperty = other.PenWidthProperty;
    PenStyleProperty = other.PenStyleProperty;
    PenProperty = other.PenProperty;
    QStringCallbackProperty = other.QStringCallbackProperty;
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
    Q_UNUSED(ignoreMask);

    QtnPropertySetSamplePS* theCopyFrom = qobject_cast<QtnPropertySetSamplePS*>(propertySetCopyFrom);
    if (!theCopyFrom)
        return false;

    if (!(theCopyFrom->BoolProperty.state() & ignoreMask))
    {
        BoolProperty = theCopyFrom->BoolProperty;
    }

    if (!(theCopyFrom->ButtonProperty.state() & ignoreMask))
    {
        ButtonProperty = theCopyFrom->ButtonProperty;
    }

    if (!(theCopyFrom->ButtonLinkProperty.state() & ignoreMask))
    {
        ButtonLinkProperty = theCopyFrom->ButtonLinkProperty;
    }

    if (!(theCopyFrom->RGBColor.state() & ignoreMask))
    {
        RGBColor = theCopyFrom->RGBColor;
    }

    if (!(theCopyFrom->ColorSolidDelegate.state() & ignoreMask))
    {
        ColorSolidDelegate = theCopyFrom->ColorSolidDelegate;
    }

    if (!(theCopyFrom->FloatPropertySliderBox.state() & ignoreMask))
    {
        FloatPropertySliderBox = theCopyFrom->FloatPropertySliderBox;
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

    if (!(theCopyFrom->IntPropertyComboBox.state() & ignoreMask))
    {
        IntPropertyComboBox = theCopyFrom->IntPropertyComboBox;
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

    if (!(theCopyFrom->QPointFProperty.state() & ignoreMask))
    {
        QPointFProperty = theCopyFrom->QPointFProperty;
    }

    if (!(theCopyFrom->QSizeFProperty.state() & ignoreMask))
    {
        QSizeFProperty = theCopyFrom->QSizeFProperty;
    }

    if (!(theCopyFrom->QRectFProperty.state() & ignoreMask))
    {
        QRectFProperty = theCopyFrom->QRectFProperty;
    }

    if (!(theCopyFrom->QColorProperty.state() & ignoreMask))
    {
        QColorProperty = theCopyFrom->QColorProperty;
    }

    if (!(theCopyFrom->QFontProperty.state() & ignoreMask))
    {
        QFontProperty = theCopyFrom->QFontProperty;
    }

    if (!(theCopyFrom->FreqProperty.state() & ignoreMask))
    {
        FreqProperty = theCopyFrom->FreqProperty;
    }

    if (!(theCopyFrom->LayerProperty.state() & ignoreMask))
    {
        LayerProperty = theCopyFrom->LayerProperty;
    }

    if (!(theCopyFrom->BrushStyleProperty.state() & ignoreMask))
    {
        BrushStyleProperty = theCopyFrom->BrushStyleProperty;
    }

    if (!(theCopyFrom->PenWidthProperty.state() & ignoreMask))
    {
        PenWidthProperty = theCopyFrom->PenWidthProperty;
    }

    if (!(theCopyFrom->PenStyleProperty.state() & ignoreMask))
    {
        PenStyleProperty = theCopyFrom->PenStyleProperty;
    }

    if (!(theCopyFrom->PenProperty.state() & ignoreMask))
    {
        PenProperty = theCopyFrom->PenProperty;
    }

    if (!(theCopyFrom->QStringCallbackProperty.state() & ignoreMask))
    {
        QStringCallbackProperty = theCopyFrom->QStringCallbackProperty;
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
    static QString ButtonProperty_name = tr("ButtonProperty");
    ButtonProperty.setName(ButtonProperty_name);
    ButtonProperty.setClickHandler([](const QtnPropertyButton* bttn) {
            qDebug() << Q_FUNC_INFO << "Button has clicked: " << bttn;
        });
    static QString ButtonProperty_description = "Start calculate a long operation.";
    ButtonProperty.setDescription(ButtonProperty_description);
    static QString ButtonLinkProperty_name = tr("ButtonLinkProperty");
    ButtonLinkProperty.setName(ButtonLinkProperty_name);
    ButtonLinkProperty.setClickHandler([](const QtnPropertyButton* bttn) {
            qDebug() << Q_FUNC_INFO << "Link has clicked: " << bttn;
        });
    static QString RGBColor_name = tr("RGBColor");
    RGBColor.setName(RGBColor_name);
    RGBColor.setClickHandler([this](const QtnPropertyABColor* color) {
            qDebug() << Q_FUNC_INFO << "Color has clicked: " << color;
            RGBColor = QColor::fromRgb(qrand()%255, qrand()%255, qrand()%255);
        });
    static QString RGBColor_description = "ABColor property with RGB components";
    RGBColor.setDescription(RGBColor_description);
    RGBColor.setResetCallback([this](QtnPropertyBase&){
            RGBColor = Qt::green;
        });
    RGBColor.setValue(QColor(123, 150, 10));
    static QString ColorSolidDelegate_name = tr("ColorSolidDelegate");
    ColorSolidDelegate.setName(ColorSolidDelegate_name);
    static QString ColorSolidDelegate_description = "QColor property with Solid delegate";
    ColorSolidDelegate.setDescription(ColorSolidDelegate_description);
    ColorSolidDelegate.setValue(QColor(13, 150, 10));
    static QString FloatPropertySliderBox_name = tr("FloatPropertySliderBox");
    FloatPropertySliderBox.setName(FloatPropertySliderBox_name);
    static QString FloatPropertySliderBox_description = "Property to hold float values in range [0, 10].";
    FloatPropertySliderBox.setDescription(FloatPropertySliderBox_description);
    FloatPropertySliderBox.setDisplayName("Float Property Slider Box");
    FloatPropertySliderBox.setMaxValue(10.f);
    FloatPropertySliderBox.setMinValue(0);
    FloatPropertySliderBox.setStepValue(0.1f);
    FloatPropertySliderBox.setValue(1.f);
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
    static QString IntPropertyComboBox_name = tr("IntPropertyComboBox");
    IntPropertyComboBox.setName(IntPropertyComboBox_name);
    static QString IntPropertyComboBox_description = "Property to hold integer values with changing step 15.";
    IntPropertyComboBox.setDescription(IntPropertyComboBox_description);
    IntPropertyComboBox.setStepValue(15);
    IntPropertyComboBox.setValue(10);
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
    static QString QPointFProperty_name = tr("QPointFProperty");
    QPointFProperty.setName(QPointFProperty_name);
    static QString QPointFProperty_description = "Property to hold QPointF value.";
    QPointFProperty.setDescription(QPointFProperty_description);
    QPointFProperty.setValue(QPointF(-10.5, 10.2));
    static QString QSizeFProperty_name = tr("QSizeFProperty");
    QSizeFProperty.setName(QSizeFProperty_name);
    static QString QSizeFProperty_description = "Property to hold QSizeF value.";
    QSizeFProperty.setDescription(QSizeFProperty_description);
    QSizeFProperty.setValue(QSizeF(100.0, 200.1));
    static QString QRectFProperty_name = tr("QRectFProperty");
    QRectFProperty.setName(QRectFProperty_name);
    static QString QRectFProperty_description = "Property to hold QRectF value.";
    QRectFProperty.setDescription(QRectFProperty_description);
    QRectFProperty.setValue(QRectF(10.23, 10.4, 200.2, 200.6));
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
    static QString FreqProperty_name = tr("FreqProperty");
    FreqProperty.setName(FreqProperty_name);
    static QString FreqProperty_description = "Property to hold frequency values.";
    FreqProperty.setDescription(FreqProperty_description);
    FreqProperty.setUnit(FreqUnit::KHz);
    FreqProperty.setValue(15);
    static QString LayerProperty_name = tr("LayerProperty");
    LayerProperty.setName(LayerProperty_name);
    static QString LayerProperty_description = "Property to hold layer.";
    LayerProperty.setDescription(LayerProperty_description);
    LayerProperty.setValue(0);
    static QString BrushStyleProperty_name = tr("BrushStyleProperty");
    BrushStyleProperty.setName(BrushStyleProperty_name);
    static QString BrushStyleProperty_description = "Property to hold QBrushStyle enum.";
    BrushStyleProperty.setDescription(BrushStyleProperty_description);
    BrushStyleProperty.setValue(Qt::HorPattern);
    static QString PenWidthProperty_name = tr("PenWidthProperty");
    PenWidthProperty.setName(PenWidthProperty_name);
    static QString PenWidthProperty_description = "Property to hold PenWidth enum.";
    PenWidthProperty.setDescription(PenWidthProperty_description);
    PenWidthProperty.setValue(PenWidth::Middle);
    static QString PenStyleProperty_name = tr("PenStyleProperty");
    PenStyleProperty.setName(PenStyleProperty_name);
    static QString PenStyleProperty_description = "Property to hold pen style values.";
    PenStyleProperty.setDescription(PenStyleProperty_description);
    PenStyleProperty.setValue(Qt::DashLine);
    static QString PenProperty_name = tr("PenProperty");
    PenProperty.setName(PenProperty_name);
    static QString PenProperty_description = "Property to hold QPen values.";
    PenProperty.setDescription(PenProperty_description);
    static QString QStringCallbackProperty_name = tr("QStringCallbackProperty");
    QStringCallbackProperty.setName(QStringCallbackProperty_name);
    static QString QStringCallbackProperty_description = "Property to hold QString values with candidates.";
    QStringCallbackProperty.setDescription(QStringCallbackProperty_description);
    static QString SubPropertySet2_name = tr("SubPropertySet2");
    SubPropertySet2.setName(SubPropertySet2_name);
    SubPropertySet2.setState(QtnPropertyStateCollapsed);
    // end children initialization
}

void QtnPropertySetSamplePS::connectSlots()
{
    QObject::connect(&EnableSubPropertySet, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange);
    QObject::connect(&QColorProperty, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_QColorProperty_propertyDidChange);
}

void QtnPropertySetSamplePS::disconnectSlots()
{
    QObject::disconnect(&EnableSubPropertySet, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange);
    QObject::disconnect(&QColorProperty, &QtnProperty::propertyDidChange, this, &QtnPropertySetSamplePS::on_QColorProperty_propertyDidChange);
}

void QtnPropertySetSamplePS::on_EnableSubPropertySet_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason);
    
            SubPropertySet.switchState(QtnPropertyStateImmutable, !EnableSubPropertySet);
        
}

void QtnPropertySetSamplePS::on_QColorProperty_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
{
    Q_UNUSED(changedProperty); Q_UNUSED(firedProperty); Q_UNUSED(reason);
    
            qDebug() << Q_FUNC_INFO << "Property has changed: " << &QColorProperty;
        
}

void QtnPropertySetSamplePS::connectDelegates()
{
    ButtonProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["title"] = "Click me";
        return info.take();
    });
    ButtonLinkProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "Link";
        info->attributes["title"] = "Click on me";
        return info.take();
    });
    RGBColor.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["rgbSubItems"] = true;
        return info.take();
    });
    ColorSolidDelegate.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "Solid";
        return info.take();
    });
    FloatPropertySliderBox.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "SliderBox";
        info->attributes["drawBorder"] = false;
        info->attributes["fillColor"] = QColor::fromRgb(170, 170, 255);
        return info.take();
    });
    IntPropertyComboBox.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->name = "IntList";
        info->attributes["values"] = QVariant::fromValue(QList<int>() << 10 << 12 << 15);
        return info.take();
    });
    PenProperty.setDelegateCallback([] () -> QtnPropertyDelegateInfo * {
        QScopedPointer<QtnPropertyDelegateInfo> info(new QtnPropertyDelegateInfo());
        info->attributes["editColor"] = true;
        info->attributes["editStyle"] = true;
        info->attributes["editWidth"] = true;
        return info.take();
    });
}
