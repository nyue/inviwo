/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#ifndef IVW_EVENTPROPERTYWIDGETQT_H
#define IVW_EVENTPROPERTYWIDGETQT_H

#include <modules/qtwidgets/qtwidgetsmoduledefine.h>
#include <modules/qtwidgets/properties/propertywidgetqt.h>

class QKeyEvent;
class QMouseEvent;
class QFocusEvent;

namespace inviwo {

class IvwPushButton;
class EditableLabelQt;
class EventProperty;
class EventMatcher;
class MouseEventMatcher;
class KeyboardEventMatcher;

class IVW_MODULE_QTWIDGETS_API EventPropertyWidgetQt : public PropertyWidgetQt {

public:
    EventPropertyWidgetQt(EventProperty* eventproperty);
    virtual ~EventPropertyWidgetQt();
    void updateFromProperty();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

    virtual void focusOutEvent(QFocusEvent* event);
    void clickedSlot();

private:
    void setButtonText();

    EventProperty* eventproperty_;
    IvwPushButton* button_;
    EditableLabelQt* label_;

    std::unique_ptr<EventMatcher> matcher_;
    KeyboardEventMatcher* keyMatcher_ = nullptr;
    MouseEventMatcher* mouseMatcher_ = nullptr;
};

} //namespace

#endif // IVW_EVENTPROPERTYWIDGETQT_H