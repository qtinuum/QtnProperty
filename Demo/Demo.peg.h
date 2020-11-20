#ifndef DEMO_H
#define DEMO_H

#include "QtnProperty/PropertyCore.h"
#include "QtnProperty/PropertyGUI.h"
#include "QtnProperty/PropertyInt64.h"
#include "QtnProperty/PropertyUInt64.h"
#include "AB/PropertyABColor.h"
#include "Layer/PropertyLayer.h"
#include "PenWidth/PropertyPenWidth.h"
#include "Freq/PropertyFreq.h"

class COLOR
{
public:
    enum Enum
    {
        red = 1,
        blue = 2,
        green = 3
    };
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 3;
};

class FLAGS
{
public:
    enum Enum
    {
        opt1 = 1,
        opt2 = 2,
        opt3 = 4
    };
    
    static const QtnEnumInfo& info();
    static const unsigned int values_count = 3;
};

class QtnPropertySetSubPropertySetType: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSubPropertySetType)

public:
    // constructor declaration
    explicit QtnPropertySetSubPropertySetType(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetSubPropertySetType() override;
    // assignment declaration
    QtnPropertySetSubPropertySetType& operator=(const QtnPropertySetSubPropertySetType& other);
    
    // start children declarations
    QtnPropertyBool& SwitchProperty;
    QtnPropertyQStringCallback& ReadOnlyString;
    QtnPropertyQString& FileNameProperty;
    QtnPropertyQString& FolderNameProperty;
    QtnPropertyQString& StringFromList;
    QtnPropertyQColor& CircleShapeColor;
    // end children declarations

protected:
    // cloning implementation
    QtnPropertySet* createNewImpl(QObject* parentForNew) const override;
    QtnPropertySet* createCopyImpl(QObject* parentForCopy) const override;
    // copy values implementation
    bool copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask) override;

private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
};

class QtnPropertySetSamplePS: public QtnPropertySet
{
    Q_OBJECT
    //Q_DISABLE_COPY(QtnPropertySetSamplePS)

public:
    // constructor declaration
    explicit QtnPropertySetSamplePS(QObject* parent = nullptr);
    // destructor declaration
    virtual ~QtnPropertySetSamplePS() override;
    // assignment declaration
    QtnPropertySetSamplePS& operator=(const QtnPropertySetSamplePS& other);
    
    // start children declarations
    QtnPropertyBool& BoolProperty;
    QtnPropertyButton& ButtonProperty;
    QtnPropertyButton& ButtonLinkProperty;
    QtnPropertyABColor& RGBColor;
    QtnPropertyQColor& ColorSolidDelegate;
    QtnPropertyFloat& FloatPropertySliderBox;
    QtnPropertyFloat& FloatPercent;
    QtnPropertyDouble& DoublePercent;
    QtnPropertyFloat& FloatIntRangeSliderBox;
    QtnPropertyDouble& DoubleFullRangeSliderBox;
    QtnPropertyDouble& DoubleProperty;
    QtnPropertyFloat& FloatProperty;
    QtnPropertyInt& IntProperty;
    QtnPropertyInt& IntPropertyComboBox;
    QtnPropertyUInt& UIntProperty;
    QtnPropertyInt64& Int64Property;
    QtnPropertyUInt64& UInt64Property;
    QtnPropertyInt64& Int64SliderBox;
    QtnPropertyUInt64& UInt64SliderBox;
    QtnPropertyEnum& EnumProperty;
    QtnPropertyEnumFlags& EnumFlagsProperty;
    QtnPropertyQString& QStringValue;
    QtnPropertyBool& EnableSubPropertySet;
    QtnPropertySetSubPropertySetType& SubPropertySet;
    QtnPropertyQPoint& QPointProperty;
    QtnPropertyQSize& QSizeProperty;
    QtnPropertyQRect& QRectProperty;
    QtnPropertyQPointF& QPointFProperty;
    QtnPropertyQSizeF& QSizeFProperty;
    QtnPropertyQRectF& QRectFProperty;
    QtnPropertyQVector3D& QVector3DProperty;
    QtnPropertyQColor& QColorProperty;
    QtnPropertyQFont& QFontProperty;
    QtnPropertyFreq& FreqProperty;
    QtnPropertyLayer& LayerProperty;
    QtnPropertyQBrushStyle& BrushStyleProperty;
    QtnPropertyPenWidth& PenWidthProperty;
    QtnPropertyQPenStyle& PenStyleProperty;
    QtnPropertyQPen& PenProperty;
    QtnPropertyQString& QStringCallbackProperty;
    QtnPropertySetSubPropertySetType& SubPropertySet2;
    // end children declarations

protected:
    // cloning implementation
    QtnPropertySet* createNewImpl(QObject* parentForNew) const override;
    QtnPropertySet* createCopyImpl(QObject* parentForCopy) const override;
    // copy values implementation
    bool copyValuesImpl(QtnPropertySet* propertySetCopyFrom, QtnPropertyState ignoreMask) override;

private:
    void init();
    void connectSlots();
    void disconnectSlots();
    void connectDelegates();
    
    // start slot declarations
    void on_EnableSubPropertySet_propertyDidChange(QtnPropertyChangeReason reason);
    void on_QColorProperty_propertyDidChange(QtnPropertyChangeReason reason);
    // end slot declarations
};

#endif // DEMO_H
