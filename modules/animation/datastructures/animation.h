/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2018 Inviwo Foundation
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

#ifndef IVW_ANIMATION_H
#define IVW_ANIMATION_H

#include <modules/animation/animationmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/serializable.h>

#include <modules/animation/datastructures/animationtime.h>
#include <modules/animation/datastructures/track.h>
#include <modules/animation/datastructures/trackobserver.h>
#include <modules/animation/datastructures/animationobserver.h>
#include <modules/animation/datastructures/animationstate.h>

namespace inviwo {

namespace animation {

/** \class Animation
 * Animation data structure, owns a list of tracks. 
 * Each Track usually represents a value to be animated.
 */
class IVW_MODULE_ANIMATION_API Animation : public AnimationObservable,
                                           public Serializable,
                                           public TrackObserver {
public:
    Animation();
    Animation(const Animation&) = delete;
    Animation& operator=(const Animation& that) = delete;

    AniamtionTimeState operator()(Seconds from, Seconds to, AnimationState state) const;

    bool empty() const;
    size_t size() const;
    Track& operator[](size_t i);
    const Track& operator[](size_t i) const;

    void add(std::unique_ptr<Track> track);
    /**
     * Remove tracks at index i, indicating the order in which the track was added, 
     * not the order in which they are sorted by Track priority.
     * No range check is done.
     * Calls TrackObserver::notifyTrackRemoved after removing track.
     */
    void removeTrack(size_t i);
    /**
     * Remove tracks based on Track::getIdentifier
     * Does nothing if no match was found.
     * Calls TrackObserver::notifyTrackRemoved after removing track.
     */
    void removeTrack(const std::string& id);

    /**
     * Remove Keyframe if matching any of the Keyframes in the tracks.
     * Calls TrackObserver::notifyKeyframeRemoved after removing Keyframe.
     * Removes the KeyFrameSequence owning the Keyframe if it does not contain any Keyframe after removal,
     * thereby calling KeyFrameSequenceObserver::notifyKeyframeSequenceRemoved
     * Does nothing if no match was found.
     * @note Keyframe will be deleted if removed so do not use pointer after calling this function.
     */
    void removeKeyframe(Keyframe* key);
    /**
     * Remove all tracks. Calls TrackObserver::notifyTrackRemoved for each removed track.
     */
    void clear();
    /**
     * Return a sorted list, in ascending order, of all Keyframe times existing in the animation.
     */
    std::vector<Seconds> getAllTimes() const;
    /**
     * Return time of first Keyframe in all tracks, or 0 if no track exist.
     */
    Seconds firstTime() const;
    /**
     * Return time of last Keyframe in all tracks, or 0 if no track exist.
     */
    Seconds lastTime() const;

    virtual void serialize(Serializer& s) const override;
    virtual void deserialize(Deserializer& d) override;

private:
    virtual void onPriorityChanged(Track* t) override;
    void doPrioritySort();

    virtual void onFirstMoved(Track* t) override;
    virtual void onLastMoved(Track* t) override;

    std::vector<std::unique_ptr<Track>> tracks_;
    std::vector<Track*> priorityTracks_;
};

}  // namespace

}  // namespace

#endif // IVW_ANIMATION_H

