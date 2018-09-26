#include "PropertyDelegateGeoCoord.h"
#include "../../../Core/Core/PropertyDouble.h"
#include "../PropertyDelegateFactory.h"
#include "../Utils/PropertyEditorHandler.h"


#include <QLineEdit>

void regGeoCoordDelegates()
{
  QtnPropertyDelegateFactory::staticInstance().registerDelegate(&QtnPropertyDoubleBase::staticMetaObject
               , &qtnCreateDelegate<QtnPropertyDelegateGeoCoord, QtnPropertyDoubleBase>
               , "GeoCoord");
}

void regGeoCoordDelegates(QtnPropertyDelegateFactory &factory)
{
  factory.registerDelegate(&QtnPropertyDoubleBase::staticMetaObject
               , &qtnCreateDelegate<QtnPropertyDelegateGeoCoord, QtnPropertyDoubleBase>
               , "GeoCoord");
}


QString val2strGeoCoord(const qreal c)
{
    QString s;
    s = c < 0 ? "-" : "";
    qreal r = qAbs(c);
    int deg = static_cast<int>(r);
    r -= deg;
    r *= 60;
    int min = static_cast<int>(r);
    r -= min;
    r *= 60;
    int sec = static_cast<int>(r);
    if (sec == 60){
        sec = 0;
        ++min;
        if (min == 60){
            min = 0;
            ++deg;
        }
    }

    QString txt = QString("%1 %2° %3\' %4\"").
            arg(s).
            arg(deg, 3, 10, QChar('0')).
            arg(min, 2, 10, QChar('0')).
            arg(sec, 2, 10, QChar('0'));
    return txt;
}

qreal str2valGeoCoord(const QString &strVal)
{
    static const QRegExp parserDeg(".*(\\d+)°.*", Qt::CaseInsensitive);
    static const QRegExp parserMin(".*(\\d+)\'.*", Qt::CaseInsensitive);
    static const QRegExp parserSec(".*(\\d+\\.?\\d*)\".*", Qt::CaseInsensitive);
    static const QRegExp parserSign("^(-).*", Qt::CaseInsensitive);

    QString str = strVal;
    str.remove(" ");
    qreal val = 0.;
    if (parserDeg.exactMatch(str)){
        if (parserDeg.capturedTexts().size() == 2){
            val += parserDeg.capturedTexts().at(1).toInt();
        }
    }
    if (parserMin.exactMatch(str)){
        if (parserMin.capturedTexts().size() == 2){
            val += parserMin.capturedTexts().at(1).toInt() / 60.;
        }
    }
    if (parserSec.exactMatch(str)){
        if (parserSec.capturedTexts().size() == 2){
            val += parserSec.capturedTexts().at(1).toDouble() / 60./ 60.;
        }
    }
    if (parserSign.exactMatch(str)){
        val = -val;
    }
    return val;
}

class QtnPropertyGeoCoordLineEditHandler: public QtnPropertyEditorHandler<QtnPropertyDoubleBase, QLineEdit>
{
public:
    QtnPropertyGeoCoordLineEditHandler(QtnPropertyDoubleBase& property, QLineEdit& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        double propertyValue = static_cast<qreal>(property.value());
        editor.setInputMask("# 000° 00\' 00\"");
        editor.setText(val2strGeoCoord(propertyValue));

        QObject::connect(  &editor, static_cast<void (QLineEdit::*)()>(&QLineEdit::editingFinished)
                         , this, &QtnPropertyGeoCoordLineEditHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
        double editorValue = str2valGeoCoord(editor().text());
        double propertyValue = static_cast<qreal>(property());

        if (!qFuzzyCompare(editorValue, propertyValue))
            editor().setText(val2strGeoCoord(propertyValue));
    }

    void onValueChanged()
    {
        qreal val = str2valGeoCoord(editor().text());
        property() = val;
    }
};


QWidget* QtnPropertyDelegateGeoCoord::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(rect);

    new QtnPropertyGeoCoordLineEditHandler(owner(), *lineEdit);

    if (inplaceInfo)
    {
        lineEdit->selectAll();
    }

    return lineEdit;
}

bool QtnPropertyDelegateGeoCoord::propertyValueToStrImpl(QString& strValue) const
{    
    strValue = val2strGeoCoord(owner().value());
    return true;
}
