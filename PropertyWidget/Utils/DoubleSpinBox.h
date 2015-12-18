#include <QDoubleSpinBox>

#include "PropertyWidgetAPI.h"

class QTN_PW_EXPORT QtnDoubleSpinBox : public QDoubleSpinBox
{
public:
	explicit QtnDoubleSpinBox(QWidget *parent = 0);

	bool getHavePercentSuffix() const;
	void setHavePercentSuffix(bool value);

	virtual QValidator::State validate(QString &input, int &pos) const override;
	virtual double valueFromText(const QString &text) const override;
	virtual QString textFromValue(double val) const override;

private:
	QString getNumberText(const QString &source) const;

	bool percent_suffix;
};

