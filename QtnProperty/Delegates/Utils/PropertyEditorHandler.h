/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#ifndef PROPERTY_EDITOR_HANDLER_H
#define PROPERTY_EDITOR_HANDLER_H

#include "QtnProperty/CoreAPI.h"
#include "QtnProperty/Property.h"
#include "QtnProperty/Delegates/PropertyDelegate.h"

#include <QWidget>
#include <QEvent>
#include <QCoreApplication>

#include <memory>

class QDialog;

class QTN_IMPORT_EXPORT QtnPropertyEditorHandlerBase : public QObject
{
protected:
	QtnPropertyEditorHandlerBase(
		QtnPropertyDelegate *delegate, QWidget &editor);

	inline QtnPropertyDelegate *delegate() const;
	inline QtnPropertyBase *propertyBase() const;
	inline QtnPropertyBase *stateProperty() const;
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
	bool reverted : 1;
	bool returned : 1;

private:
	QtnPropertyDelegate *m_delegate;
	QWidget *m_editor;

private:
	void onPropertyDestroyed();
	void onPropertyDidChange(QtnPropertyChangeReason reason);
};

QtnPropertyBase *QtnPropertyEditorHandlerBase::propertyBase() const
{
	return m_delegate->property();
}

QtnPropertyDelegate *QtnPropertyEditorHandlerBase::delegate() const
{
	return m_delegate;
}

QtnPropertyBase *QtnPropertyEditorHandlerBase::stateProperty() const
{
	return m_delegate->stateProperty();
}

QWidget *QtnPropertyEditorHandlerBase::editorBase() const
{
	return m_editor;
}

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandler : public QtnPropertyEditorHandlerBase
{
protected:
	typedef QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
		QtnPropertyEditorHandlerType;

	QtnPropertyEditorHandler(
		QtnPropertyDelegate *delegate, PropertyEditorClass &editor)
		: QtnPropertyEditorHandlerBase(delegate, editor)
	{
	}

	PropertyClass &property() const
	{
		return *static_cast<PropertyClass *>(propertyBase());
	}

	PropertyEditorClass &editor() const
	{
		return *static_cast<PropertyEditorClass *>(editorBase());
	}
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandlerVT
	: public QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
{
protected:
	using Inherited =
		QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>;
	using ValueType = typename PropertyClass::ValueType;
	using ValueTypeStore = typename PropertyClass::ValueTypeStore;

	QtnPropertyEditorHandlerVT(
		QtnPropertyDelegate *delegate, PropertyEditorClass &editor)
		: Inherited(delegate, editor)
		, updating(0)
	{
		newValue = this->property().value();
	}

	void onValueChanged(ValueType value)
	{
		if (updating > 0)
			return;
		newValue = value;
		updateValue();
	}

	virtual void updateValue()
	{
		if (this->propertyBase())
			this->property().setValue(newValue, this->delegate()->editReason());
	}

	ValueTypeStore newValue;
	unsigned updating;
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorBttnHandler
	: public QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
{
private:
	typedef QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
		Inherited;

protected:
	typedef QtnPropertyEditorBttnHandler QtnPropertyEditorHandlerType;

	QtnPropertyEditorBttnHandler(
		QtnPropertyDelegate *delegate, PropertyEditorClass &editor)
		: Inherited(delegate, editor)
		, double_clicked(false)
	{
	}

	virtual void onToolButtonClick() = 0;
	virtual bool eventFilter(QObject *obj, QEvent *event) override
	{
		if (nullptr != this->stateProperty() &&
			this->stateProperty()->isEditableByUser())
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

#endif // PROPERTY_EDITOR_HANDLER_H
