//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2008, Daniel �nnerby
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

#include <core/config.h>
#include <core/library/track/Track.h>
#include <core/library/LocalLibrary.h>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace musik { namespace core {

    //////////////////////////////////////////
    ///\brief
    ///A InMemoryTrack is not related to any library. It must contain a URI
    //////////////////////////////////////////
    class InMemoryTrack : public Track {
        public:
            static TrackPtr Create(const char *uri);
    
    protected:
            InMemoryTrack();
            InMemoryTrack(const char *uri);

        public:
            virtual ~InMemoryTrack();

            virtual std::string GetValue(const char* metakey);
            virtual void SetValue(const char* metakey,const char* value);
            virtual void ClearValue(const char* metakey);
            virtual void SetThumbnail(const char *data, long size);
            virtual std::string URI();
            virtual std::string URL();
            virtual void SetId(DBID id) { };

            virtual MetadataIteratorRange GetValues(const char* metakey);
            virtual MetadataIteratorRange GetAllValues();
            virtual TrackPtr Copy();

        private:
            typedef std::weak_ptr<Track> SelfWeakPtr;
            SelfWeakPtr selfPtr;
            Track::MetadataMap metadata;
            std::string uri;
            std::string title;
            boost::mutex metadataMutex;
    };

} }


