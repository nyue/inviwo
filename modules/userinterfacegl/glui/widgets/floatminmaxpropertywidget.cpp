/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017-2018 Inviwo Foundation
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

#include <modules/userinterfacegl/glui/widgets/floatminmaxpropertywidget.h>

namespace inviwo {

namespace glui {

FloatMinMaxPropertyWidget::FloatMinMaxPropertyWidget(FloatMinMaxProperty &property,
                                                     Processor &processor, Renderer &uiRenderer,
                                                     const ivec2 &extent, UIOrientation orientation)
    : RangeSlider(property.getDisplayName(), ivec2(0), 0, 100, 0, processor, uiRenderer, extent,
                  orientation)
    , PropertyWidget(&property)
    , sliderMax_(10000)
    , property_(&property) {

    property_->addObserver(this);

    moveAction_ = [this](const dvec2 &delta) {
        bool triggerUpdate = false;
        if (!property_->getReadOnly()) {
            auto calcNewValue = [this, delta](int prev) {
                return static_cast<int>(
                    round(prev + convertDeltaToSlider(delta) *
                                     static_cast<double>(getMaxValue() - getMinValue())));
            };

            const vec2 currentValue = property_->get();
            switch (currentPickingID_) {
                case 0:  // lower bound
                    property_->setStart(sliderToRepr(calcNewValue(getPreviousValue().x)));
                    break;
                case 1:  // upper bound
                    property_->setEnd(sliderToRepr(calcNewValue(getPreviousValue().y)));
                    break;
                case 2:  // center (adjust both, lower and upper bound)
                {
                    int moveby = calcNewValue(getPreviousValue().x) - getPreviousValue().x;
                    // ensure not to move middle part further than min/max values
                    if (moveby > 0.0f) {
                        moveby = std::min(moveby, getMaxValue() - getPreviousValue().y);
                    } else {
                        moveby = -std::min(-moveby, getPreviousValue().x - getMinValue());
                    }
                    property_->set(sliderToRepr(getPreviousValue() + moveby));
                    break;
                }
                default:
                    break;
            }
            triggerUpdate = (property_->get() != currentValue);
        }
        return triggerUpdate;
    };
    updateFromProperty();
}

void FloatMinMaxPropertyWidget::updateFromProperty() {
    const auto value(property_->get());
    set(reprToSlider(property_->get()), 0, sliderMax_, reprSeparationToSlider());
    setEnabled(!property_->getReadOnly());
}

void FloatMinMaxPropertyWidget::onSetVisible(Property *, bool visible) { setVisible(visible); }

void FloatMinMaxPropertyWidget::onSetDisplayName(Property *, const std::string &displayName) {
    setLabel(displayName);
    property_->propertyModified();
}

void FloatMinMaxPropertyWidget::onSetReadOnly(Property *property, bool readonly) {
    setEnabled(!readonly);
}

float FloatMinMaxPropertyWidget::sliderToRepr(int val) const {
    return property_->getRangeMin() +
           (static_cast<float>(val) * (property_->getRangeMax() - property_->getRangeMin()) /
            static_cast<float>(sliderMax_));
}
int FloatMinMaxPropertyWidget::reprToSlider(float val) const {
    return static_cast<int>((val - property_->getRangeMin()) /
                            (property_->getRangeMax() - property_->getRangeMin()) * sliderMax_);
}

vec2 FloatMinMaxPropertyWidget::sliderToRepr(const ivec2 &val) const {
    return vec2(sliderToRepr(val.x), sliderToRepr(val.y));
}

ivec2 FloatMinMaxPropertyWidget::reprToSlider(const vec2 &val) const {
    return ivec2(reprToSlider(val.x), reprToSlider(val.y));
}

int FloatMinMaxPropertyWidget::reprSeparationToSlider() const {
    return static_cast<int>(property_->getMinSeparation() /
                            (property_->getRangeMax() - property_->getRangeMin()) * sliderMax_);
}

}  // namespace glui

}  // namespace inviwo
