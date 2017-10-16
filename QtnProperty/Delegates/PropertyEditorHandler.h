/*
   Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef PROPERTY_EDITOR_HANDLER_H
#define PROPERTY_EDITOR_HANDLER_H

#include "QtnProperty/CoreAPI.h"
#include "QtnProperty/Property.h"

#include <QWidget>
#include <QEvent>
#include <QCoreApplication>

#include <memory>

class QDialog;

class QTN_IMPORT_EXPORT QtnPropertyEditorHandlerBase : public QObject
{
protected:
	QtnPropertyEditorHandlerBase(QtnProperty &property, QWidget &editor);

	inline QtnProperty *propertyBase() const;
	inline QWidget *editorBase() const;
	virtual void updateEditor() = 0;
	virtual void revertInput();

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

	virtual bool canApply() const;
	virtual void applyReset();

	struct DialogContainer
	{
		QDialog *dialog;

		DialogContainer(QDialog *dialog);
		~DialogContainer();
	};

	typedef std::shared_ptr<DialogContainer> DialogContainerPtr;

	static DialogContainerPtr connectDialog(QDialog *dialog);
	static void connectDialog(const DialogContainerPtr &containerPtr);

protected:
	QtnProperty *m_property;
	QWidget *m_editor;

	bool reverted : 1;
	bool returned : 1;

private:
	void onPropertyDestroyed();
	void onPropertyDidChange(QtnPropertyChangeReason reason);
};

QtnProperty *QtnPropertyEditorHandlerBase::propertyBase() const
{
	return m_property;
}

QWidget *QtnPropertyEditorHandlerBase::editorBase() const
{
	return m_editor;
}

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandler : public QtnPropertyEditorHandlerBase
{
protected:
	typedef QtnPropertyEditorHandler<PropertyClass,
									 PropertyEditorClass>
		QtnPropertyEditorHandlerType;

	QtnPropertyEditorHandler(
		PropertyClass &property,
		PropertyEditorClass &editor)
		: QtnPropertyEditorHandlerBase(property, editor)
	{
	}

	PropertyClass &property() const
	{
		return *static_cast<PropertyClass *>(m_property);
	}

	PropertyEditorClass &editor() const
	{
		return *static_cast<PropertyEditorClass *>(m_editor);
	}
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandlerVT
	: public QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
{
protected:
	using Inherited = QtnPropertyEditorHandler<PropertyClass,
											   PropertyEditorClass>;
	using ValueType = typename PropertyClass::ValueType;
	using ValueTypeStore = typename PropertyClass::ValueTypeStore;

	QtnPropertyEditorHandlerVT(
		PropertyClass &property,
		PropertyEditorClass &editor)
		: Inherited(property, editor)
		, newValueEvent(nullptr)
		, updating(0)
	{
		newValue = property.value();
	}

	void onValueChanged(ValueType value)
	{
		if (updating > 0)
			return;

		newValue = value;

		if (newValueEvent == nullptr)
		{
			newValueEvent = new QEvent(QEvent::User);

			QCoreApplication::postEvent(this, newValueEvent);
		}
	}

	virtual void updateValue()
	{
		Inherited::property().edit(newValue);
	}

	virtual bool event(QEvent *event) override
	{
		if (event == newValueEvent)
		{
			newValueEvent = nullptr;
			updateValue();
			return true;
		}

		return QtnPropertyEditorHandlerBase::event(event);
	}

	QEvent *newValueEvent;
	ValueTypeStore newValue;
	unsigned updating;
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorBttnHandler
	: public QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
{
private:
	typedef QtnPropertyEditorHandler<PropertyClass,
									 PropertyEditorClass> Inherited;

protected:
	typedef QtnPropertyEditorBttnHandler QtnPropertyEditorHandlerType;

	QtnPropertyEditorBttnHandler(
		PropertyClass &property, PropertyEditorClass &editor)
		: Inherited(property, editor)
		, double_clicked(false)
	{
	}

	virtual void onToolButtonClick() = 0;
	virtual bool eventFilter(QObject *obj, QEvent *event) override
	{
		if (nullptr != Inherited::m_property &&
			Inherited::m_property->isEditableByUser())
			switch (event->type())
			{
				case QEvent::MouseButtonDblClick:
					double_clicked = true;
					return true;

				case QEvent::MouseButtonRelease:

					if (double_clicked)
					{
						double_clicked = false;
						onToolButtonClick();
						return true;
					}

					break;

				default:
					break;
			}

		return Inherited::eventFilter(obj, event);
	}

private:
	bool double_clicked;
};

#endif	// PROPERTY_EDITOR_HANDLER_H
