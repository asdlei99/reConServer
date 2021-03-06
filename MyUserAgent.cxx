#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdio>

#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>

#include <resip/stack/MessageFilterRule.hxx>
#include <AppSubsystem.hxx>

#include "B2BCallManager.hxx"
#include "MyUserAgent.hxx"

#define RESIPROCATE_SUBSYSTEM AppSubsystem::RECONSERVER

using namespace resip;
using namespace recon;
using namespace reconserver;
using namespace std;


MyUserAgent::MyUserAgent(ConfigParse& configParse, ConversationManager* conversationManager, SharedPtr<UserAgentMasterProfile> profile) :
   UserAgent(conversationManager, profile),
   mMaxRegLoops(1000)
{
   mRegistrationForwarder.reset(new RegistrationForwarder(configParse, getSipStack()));
   mSubscriptionForwarder.reset(new SubscriptionForwarder(configParse, getSipStack()));
   MessageFilterRuleList ruleList;
   MessageFilterRule::MethodList methodList;
   methodList.push_back(resip::INVITE);
   methodList.push_back(resip::CANCEL);
   methodList.push_back(resip::BYE);
   methodList.push_back(resip::ACK);
   methodList.push_back(resip::REFER);
   methodList.push_back(resip::PUBLISH);
   methodList.push_back(resip::OPTIONS);
   methodList.push_back(resip::PRACK);
   ruleList.push_back(MessageFilterRule(resip::MessageFilterRule::SchemeList(),
                                        resip::MessageFilterRule::Any,
                                        methodList) );

   // We have to be more selective about NOTIFY because some of the
   // Event types must fall through to the SubscriptionForwarder
   MessageFilterRule::MethodList methodList2;
   methodList2.push_back(resip::NOTIFY);
   MessageFilterRule::EventList eventList;
   eventList.push_back("refer"); // FIXME
   ruleList.push_back(MessageFilterRule(resip::MessageFilterRule::SchemeList(),
                                        resip::MessageFilterRule::Any,
                                        methodList2,
                                        eventList) );

   getDialogUsageManager().setMessageFilterRuleList(ruleList);
}

void
MyUserAgent::onApplicationTimer(unsigned int id, unsigned int durationMs, unsigned int seq)
{
   InfoLog(<< "onApplicationTimeout: id=" << id << " dur=" << durationMs << " seq=" << seq);
}

void
MyUserAgent::onSubscriptionTerminated(SubscriptionHandle handle, unsigned int statusCode)
{
   InfoLog(<< "onSubscriptionTerminated: handle=" << handle << " statusCode=" << statusCode);
}

void
MyUserAgent::onSubscriptionNotify(SubscriptionHandle handle, const Data& notifyData)
{
   InfoLog(<< "onSubscriptionNotify: handle=" << handle << " data=" << endl << notifyData);
}

resip::SharedPtr<ConversationProfile>
MyUserAgent::getIncomingConversationProfile(const resip::SipMessage& msg)
{
   B2BCallManager *b2bcm = getB2BCallManager();
   SharedPtr<ConversationProfile> defaultProfile = UserAgent::getIncomingConversationProfile(msg);
   if(b2bcm)
   {
      return b2bcm->getIncomingConversationProfile(msg, defaultProfile);
   }
   return defaultProfile;
}

void
MyUserAgent::process(int timeoutMs)
{
   // Keep calling process() as long as there appear to be messages
   // available from the stack
   for(int i = 0; i < mMaxRegLoops && mRegistrationForwarder->process() ; i++);
   for(int i = 0; i < mMaxRegLoops && mSubscriptionForwarder->process() ; i++);

   UserAgent::process(timeoutMs);
}

B2BCallManager*
MyUserAgent::getB2BCallManager()
{
   return dynamic_cast<B2BCallManager*>(getConversationManager());
}

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

