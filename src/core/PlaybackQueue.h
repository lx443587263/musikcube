//////////////////////////////////////////////////////////////////////////////
// Copyright � 2007, mC2 team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright 
//      notice, this list of conditions and the following disclaimer in the 
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may 
//      be used to endorse or promote products derived from this software 
//      without specific prior written permission. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE. 
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////

#include <core/audio/Transport.h>
#include <core/tracklist/Standard.h>
#include <core/Query/Tracks.h>
#include <sigslot/sigslot.h>

//////////////////////////////////////////////////////////////////////////////

namespace musik { namespace core { 

//////////////////////////////////////////////////////////////////////////////

class PlaybackQueue : public sigslot::has_slots<>{
    private:
        PlaybackQueue(void);
        static PlaybackQueue sInstance;

        audio::Transport transport;
        tracklist::IRandomAccessPtr nowPlaying;

        bool playing;

    public:
        ~PlaybackQueue(void);
        static PlaybackQueue& Instance(){ return sInstance; };

        // Now Playing control
        tracklist::IRandomAccessPtr NowPlayingTracklist();
        void Play(tracklist::IRandomAccess &tracklist);
        void Append(tracklist::IRandomAccess &tracklist);

        // Playback Control
        void Play();
        void Next();
        void Previous();
        void Stop();

        musik::core::TrackPtr CurrentTrack();

        // Public signals
        sigslot::signal1<musik::core::TrackPtr> CurrentTrackChanged;

    private:
        TrackPtr currentTrack;
        void SetCurrentTrack(TrackPtr track);
        musik::core::Query::Tracks metadataQuery;

        void OnPlaybackEndOrFail();

        bool signalDisabled;

};

//////////////////////////////////////////////////////////////////////////////
} } // musik::core
//////////////////////////////////////////////////////////////////////////////
