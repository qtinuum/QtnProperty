#ifndef DEMO_H
#define DEMO_H

#include "../Core/PropertyCore.h"
#include "../Core/PropertyGUI.h"
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
    explicit QtnPropertySetSubPropertySetType(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetSubPropertySetType();
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
    explicit QtnPropertySetSamplePS(QObject* parent = 0);
    // destructor declaration
    virtual ~QtnPropertySetSamplePS();
    // assignment declaration
    QtnPropertySetSamplePS& operator=(const QtnPropertySetSamplePS& other);
    
    // start children declarations
    QtnPropertyBool& BoolProperty;
    QtnPropertyButton& ButtonProperty;
    QtnPropertyButton& ButtonLinkProperty;
    QtnPropertyABColor& RGBColor;
    QtnPropertyQColor& ColorSolidDelegate;
    QtnPropertyFloat& FloatPropertySliderBox;
    QtnPropertyDouble& DoubleProperty;
    QtnPropertyFloat& FloatProperty;
    QtnPropertyInt& IntProperty;
    QtnPropertyInt& IntPropertyComboBox;
    QtnPropertyUInt& UIntProperty;
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
    void on_EnableSubPropertySet_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
    void on_QColorProperty_propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
    // end slot declarations
};

#endif // DEMO_H
