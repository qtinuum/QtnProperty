#include <QDoubleSpinBox>

#include "QtnProperty/CoreAPI.h"

class QTN_IMPORT_EXPORT QtnDoubleSpinBox : public QDoubleSpinBox
{
public:
	explicit QtnDoubleSpinBox(QWidget *parent = 0);

	bool getHavePercentSuffix() const;
	void setHavePercentSuffix(bool value);

	virtual QValidator::State validate(QString &input, int &pos) const override;
	virtual double valueFromText(const QString &text) const override;
	virtual QString textFromValue(double val) const override;

	static QString valueToText(double value, const QLocale &locale = QLocale(), int decimals = 10, bool groupSeparatorShown = false);

private:
	QString getNumberText(const QString &source) const;

	bool percentSuffix;
};

