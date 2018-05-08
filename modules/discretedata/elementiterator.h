/*********************************************************************************
*
* Inviwo - Interactive Visualization Workshop
*
* Copyright (c) 2012-2018 Inviwo Foundation
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

#pragma once

#include <discretedata/discretedatamoduledefine.h>
#include <inviwo/core/common/inviwo.h>

#include "connectivity.h"

namespace inviwo {
namespace dd {

class ConnectionRange;

/** \class ElementIterator
*   Iterates over one GridPrimitive type in a Connectivity.
*/
class IVW_MODULE_DISCRETEDATA_API ElementIterator {
    friend ElementIterator operator+(ind, ElementIterator&);
    friend ElementIterator operator-(ind, ElementIterator&);

public:
    ElementIterator(const Connectivity* parent, GridPrimitive dimension, ind index = 0)
        : index_(index), parent_(parent), dimension_(dimension) {}

    ElementIterator() : index_(-1), parent_(nullptr), dimension_(GridPrimitive(-1)) {}

    ~ElementIterator() = default;

    /** Dereference to 'get data' */
    ElementIterator operator*() const;

    //*** Bidirectional Iteration ***\\

    /** Walk forward */
    ElementIterator& operator++() {
        index_++;
        return *this;
    }

    /** Walk backward */
    ElementIterator& operator--() {
        index_--;
        return *this;
    }

    /** Compare */
    bool operator==(ElementIterator& other) {
        return other.parent_ == parent_  // Compare pointers.
               && other.dimension_ == dimension_ && other.index_ == index_;
    }

    /** Compare */
    bool operator!=(ElementIterator& other) { return !(other == *this); }

    //*** Random Access Iteration ***\\

    /** Increment randomly */
    ElementIterator operator+(ind offset) {
        return ElementIterator(parent_, dimension_, index_ + offset);
    }

    /** Increment randomly */
    ElementIterator& operator+=(ind offset) {
        index_ += offset;
        return *this;
    }

    /** Decrement randomly */
    ElementIterator operator-(ind offset) {
        return ElementIterator(parent_, dimension_, index_ - offset);
    }

    /** Decrement randomly */
    ElementIterator& operator-=(ind offset) {
        index_ -= offset;
        return *this;
    }

    /** GridPrimitive type the iterator walks through */
    GridPrimitive getType() const { return dimension_; }

    /** The current index. Equivalent to dereferencing. */
    ind getIndex() const { return index_; }

    /** Iterate over connected GridPrimitives (neighbors etc) */
    ConnectionRange connection(GridPrimitive toType) const;

    // Members
protected:
    /** Index to the current element */
    ind index_;

    /** Pointer to Connectivity iterated through - Does not delete */
    const Connectivity* parent_;

    /** GridPrimitive type iterated over (0D vertices etc) */
    const GridPrimitive dimension_;
};

class IVW_MODULE_DISCRETEDATA_API ElementRange {
public:
    ElementRange(GridPrimitive dim, const Connectivity* parent) : dimension_(dim), parent_(parent) {}

    ElementIterator begin() { return ElementIterator(parent_, dimension_, 0); }
    ElementIterator end() {
        return ElementIterator(parent_, dimension_, parent_->getNumElements(dimension_));
    }

protected:
    GridPrimitive dimension_;
    const Connectivity* parent_;
};

}  // namespace
}