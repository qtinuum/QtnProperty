/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyDelegateIntList.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "Core/PropertyInt.h"
#include "Delegates/Utils/PropertyEditorHandler.h"
#include <QComboBox>
#include <QLineEdit>

void regIntListDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegate(&QtnPropertyIntBase::staticMetaObject
                 , &qtnCreateDelegate<QtnPropertyDelegateIntList, QtnPropertyIntBase>
                 , "IntList");
}

class QtnPropertyIntComboBoxHandler: public QtnPropertyEditorHandler<QtnPropertyIntBase, QComboBox>
{
public:
    QtnPropertyIntComboBoxHandler(QtnPropertyIntBase& property, QComboBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        updateEditor();

        if (!property.isEditableByUser())
            editor.setDisabled(true);

        QObject::connect(  &editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                         , this, &QtnPropertyIntComboBoxHandler::onCurrentIndexChanged);
    }

private:
    void updateEditor() override
    {
        int index = editor().findData((int)property());
        Q_ASSERT(index >= 0);
        editor().setCurrentIndex(index);
    }

    void onCurrentIndexChanged(int index)
    {
        QVariant data = editor().itemData(index);
        if (data.canConvert<int>())
            property() = data.toInt();
    }
};

QtnPropertyDelegateIntList::QtnPropertyDelegateIntList(QtnPropertyIntBase& owner)
    : QtnPropertyDelegateInt(owner)
{
}

QWidget* QtnPropertyDelegateIntList::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    if (owner().isEditableByUser())
    {
        QComboBox *comboBox = new QComboBox(parent);

        auto delegate = owner().delegate();
        if (delegate)
        {
            QList<int> values;
            qtnGetAttribute(delegate->attributes, "values", values);
            for (auto value : values)
            {
                comboBox->addItem(QString::number(value), value);
            }
        }

        comboBox->setGeometry(rect);

        // connect widget and property
        new QtnPropertyIntComboBoxHandler(owner(), *comboBox);

        if (inplaceInfo)
            comboBox->showPopup();

        return comboBox;
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setReadOnly(true);
        lineEdit->setText(QString::number((int)owner()));

        lineEdit->setGeometry(rect);

        return lineEdit;
    }
}
