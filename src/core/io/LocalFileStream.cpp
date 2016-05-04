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

#include "pch.hpp"

#include <core/io/LocalFileStream.h>
#include <core/config.h>
#include <core/support/Common.h>
#include <core/config.h>

#ifdef UTF_WIDECHAR
#define UTFFopen    _wfopen
typedef fpos_t  stdioPositionType;
#else
#define UTFFopen    fopen
typedef fpos_t stdioPositionType;
#endif

using namespace musik::core::io;

LocalFileStream::LocalFileStream()
: file(NULL)
, filesize(-1)
{

}
LocalFileStream::~LocalFileStream() {
    this->Close();
}

bool LocalFileStream::Open(const char *filename,unsigned int options){
    if(filename == NULL) {
        return false;
    }

    try {
        boost::filesystem::path file(filename);

		if (!boost::filesystem::exists(file)) {
			std::cerr << "File not found" << std::endl;
		}

		if (!boost::filesystem::is_regular(file)) {
			std::cerr << "File not a regular file" << std::endl;
		}

        this->filesize = (long)boost::filesystem::file_size(file);
        this->extension = file.extension().string();
		this->file = UTFFopen(filename,"rb");
        this->fd = new boost::iostreams::file_descriptor(file);
		this->fileStream = new boost::iostreams::stream<boost::iostreams::file_descriptor>(*this->fd);
		this->fileStream->exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);

	    return this->file!=NULL;
    }
    catch(...) {
        return false;
    }
}

bool LocalFileStream::Close() {
    if (this->file) {
        if (fclose(this->file) == 0) {
            this->file  = NULL;
			delete this->fd;
			delete this->fileStream;
            return true;
        }
    }

    return false;
}

void LocalFileStream::Destroy() {
    delete this;
}

PositionType LocalFileStream::Read(void* buffer,PositionType readBytes) {
    try	{
    	this->fileStream->read((char*) buffer, readBytes);
    }
    catch (std::ios_base::failure){
		if(!this->fileStream->eof()) {
			std::cerr << "Error reading from file" << std::endl;
			return 0;
		}
    }

	return this->fileStream->gcount();
}

bool LocalFileStream::SetPosition(PositionType position) {
    try	{
		this->fileStream->clear();
		this->fileStream->seekg(position);
    }
    catch (std::ios_base::failure ex) {
    	return false;
    }

    return true;
}

PositionType LocalFileStream::Position() {
    return this->fileStream->tellg();
}

bool LocalFileStream::Eof() {
    return this->fileStream->eof();
}

long LocalFileStream::Filesize() {
    return this->filesize;
}

const char* LocalFileStream::Type() {
    return this->extension.c_str();
}