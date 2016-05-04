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

//////////////////////////////////////////////////////////////////////////////
// Forward declare
namespace musik{ namespace core{
    namespace query{
        class  Base;
    }
} }
//////////////////////////////////////////////////////////////////////////////

#include <core/config.h>
#include <core/db/Connection.h>
#include <core/db/Statement.h>
#include <core/db/ScopedTransaction.h>
#include <core/library/LibraryBase.h>
#include <core/library/Indexer.h>

//////////////////////////////////////////////////////////////////////////////

namespace musik{ namespace core{ namespace library{

    //////////////////////////////////////////
    ///\brief
    ///Library used for your local music.
    ///
    ///This library is used for music located
    ///on you local computer.
    ///
    ///\see
    ///Indexer
    //////////////////////////////////////////
    class LocalLibrary : public library::LibraryBase {
	    private:
            LocalLibrary(std::string name, int id);

        public:
		    static LibraryPtr Create(std::string name,int id);
            ~LocalLibrary(void);

            bool Startup();
            std::string GetInfo();
            musik::core::Indexer *Indexer();

        protected:
            void CancelCurrentQuery( );

        private:
            // Methods:

            void ThreadLoop();

        private:
            // Variables:
            db::Connection db;

            //////////////////////////////////////////
            ///\brief
            ///Indexer that indexes all your tracks.
            //////////////////////////////////////////
            musik::core::Indexer indexer;
    };

} } }
