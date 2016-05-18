//////////////////////////////////////////////////////////////////////////////
// Copyright � 2007, Daniel �nnerby
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

#include <core/debug.h>
#include <core/playback/Transport.h>

using namespace musik::core::audio;

static std::string TAG = "Transport";

Transport::Transport()
 :volume(1.0)
 ,gapless(true)
{
}

Transport::~Transport(){
    this->nextPlayer.reset();
    this->currentPlayer.reset();
    this->players.clear();
}

void Transport::PrepareNextTrack(std::string trackUrl){

    if(this->gapless && this->currentPlayer){
        this->nextPlayer    = Player::Create(trackUrl,&this->currentPlayer->output);
        this->nextPlayer->Play();
    }else{
        this->nextPlayer    = Player::Create(trackUrl);
    }

}

void Transport::Start(std::string url){
    musik::debug::info(TAG, "we were asked to start the track at " + url);

    // Check if this is already Prepared
    PlayerPtr player = this->nextPlayer;
    this->nextPlayer.reset();

    musik::debug::info(TAG, "creating a Player...");

    // If the nextPlayer wasn't the same as the one started, lets create a new one
    if(!player || player->url != url){
    	Player::OutputPtr output;

        player  = Player::Create(url, &output);
        player->SetVolume(this->volume);

        musik::debug::info(TAG, "Player created successfully");
    }

    // Add to the players
    this->players.push_front(player);
    this->currentPlayer = player;
	
    musik::debug::info(TAG, "player added to player list");

    // Lets connect to the signals of the currentPlayer /* FIXME event binding is reversed here */
    this->currentPlayer->PlaybackStarted.connect(this,&Transport::OnPlaybackStarted);
    this->currentPlayer->PlaybackAlmostEnded.connect(this,&Transport::OnPlaybackAlmostEnded);
    this->currentPlayer->PlaybackEnded.connect(this,&Transport::OnPlaybackEnded);
    this->currentPlayer->PlaybackError.connect(this, &Transport::OnPlaybackError);

    musik::debug::info(TAG, "play()");

    // Start playing
    player->Play();
    this->TrackStarted(url);
}

void Transport::Stop(){
    musik::debug::info(TAG, "stop");
    this->players.clear();
    this->currentPlayer.reset();
    this->nextPlayer.reset();
    this->PlaybackEnded();
}

bool Transport::Pause(){
    musik::debug::info(TAG, "pause");

    // pause all players
    for(PlayerList::iterator player=this->players.begin();player!=this->players.end();++player){
        (*player)->Pause();
    }
    this->PlaybackPause();
    return true;
}
bool Transport::Resume(){
    musik::debug::info(TAG, "resume");

    // Resume all players
    for(PlayerList::iterator player=this->players.begin();player!=this->players.end();++player){
        (*player)->Resume();
    }
    this->PlaybackResume();
    return true;
}


double Transport::Position(){
    if(this->currentPlayer){
        return this->currentPlayer->Position();
    }
    return 0;
}

void Transport::SetPosition(double seconds){
    if(this->currentPlayer){
        return this->currentPlayer->SetPosition(seconds);
    }
}


double Transport::Volume(){
    return this->volume;
}

void Transport::SetVolume(double volume){
    double oldVolume = this->volume;
    
    volume = max(0, min(1.0, volume));

    this->volume = volume;

    if (oldVolume != this->volume) {
        this->VolumeChanged();
    }

    musik::debug::info(TAG, boost::str(boost::format("set volume %d%%") % round(volume * 100)));

    if(this->currentPlayer){
        for(PlayerList::iterator player=this->players.begin();player!=this->players.end();++player){
            (*player)->SetVolume(volume);
        }
    }
}

void Transport::OnPlaybackStarted(Player *player){
    if(this->currentPlayer.get()==player){
        this->PlaybackStarted();
    }
}

void Transport::OnPlaybackAlmostEnded(Player *player){
    if(this->currentPlayer.get()==player){
        this->PlaybackAlmostDone();

        // Reuse the output
        if(this->nextPlayer && this->gapless){
            // TODO
//            this->nex
        }
    }
}

void Transport::OnPlaybackEnded(Player *player){
    if(this->currentPlayer.get()==player){
        this->PlaybackEnded();

        // If the is a nextPlayer, then we should start playing right away
        if(this->nextPlayer){
            this->Start(this->nextPlayer->url.c_str());
        }
    }
}

void Transport::OnPlaybackError(Player *player) {
	this->PlaybackError();
}	




