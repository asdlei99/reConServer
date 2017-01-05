#ifndef MYUSERAGENT_HXX
#define MYUSERAGENT_HXX

#include <os/OsIntTypes.h>

#if defined(HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <rutil/ConfigParse.hxx>
#include <rutil/Data.hxx>
#include <rutil/SharedPtr.hxx>
#include <recon/UserAgent.hxx>

#include "B2BCallManager.hxx"
#include "RegistrationForwarder.hxx"

using namespace resip;


namespace recon
{

class MyUserAgent : public UserAgent
{
public:
   MyUserAgent(ConfigParse& configParse, ConversationManager* conversationManager, SharedPtr<UserAgentMasterProfile> profile);
   virtual void onApplicationTimer(unsigned int id, unsigned int durationMs, unsigned int seq);
   virtual void onSubscriptionTerminated(SubscriptionHandle handle, unsigned int statusCode);
   virtual void onSubscriptionNotify(SubscriptionHandle handle, const Data& notifyData);
   virtual resip::SharedPtr<ConversationProfile> getIncomingConversationProfile(const resip::SipMessage& msg);
   virtual resip::SharedPtr<ConversationProfile> getConversationProfileForRefer(const resip::SipMessage& msg);
   virtual void process(int timeoutMs);

private:
   friend class B2BCallManager;

   unsigned int mMaxRegLoops;
   SharedPtr<RegistrationForwarder> mRegistrationForwarder;

   B2BCallManager *getB2BCallManager();
};

}

#endif

/* ====================================================================
 *
 * Copyright 2016 Daniel Pocock http://danielpocock.com  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. Neither the name of the author(s) nor the names of any contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR(S) OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ====================================================================
 *
 *
 */

