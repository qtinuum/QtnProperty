/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

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

#include "PropertyDelegateAux.h"
#include "PropertyView.h"

QStyle* QtnPropertyDelegateDrawContext::style() const
{
    return widget->style();
}

void QtnPropertyDelegateDrawContext::initStyleOption(QStyleOption& option) const
{
    option.initFrom(widget);
    // State_MouseOver should be set explicitly
    option.state &= ~QStyle::State_MouseOver;
}

const QPalette& QtnPropertyDelegateDrawContext::palette() const
{
    return widget->palette();
}

QPalette::ColorGroup QtnPropertyDelegateDrawContext::colorGroup() const
{
    if (!widget->isEnabled())
        return QPalette::Disabled;
    else if (!widget->hasFocus())
        return QPalette::Inactive;
    else
        return QPalette::Active;
}
