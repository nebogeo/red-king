#!/usr/bin/python
# Naked on Pluto Copyright (C) 2010 Aymeric Mansoux, Marloes de Valk, Dave Griffiths
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# updates all the online stuff from the game's output

import xmlrpclib, sys, os, time, tweepy, time

def tweet_api(secrets):
    auth = tweepy.OAuthHandler(secrets["tweetauth0"], secrets["tweetauth1"])
    # how to get the token the first time around:
    #print(auth.get_authorization_url())
    #verifier = raw_input('Verifier:')
    #auth.get_access_token(verifier)
    #print(auth.access_token.key)
    #print(auth.access_token.secret)
    auth.set_access_token(secrets["tweetauth2"],secrets["tweetauth3"])
    return tweepy.API(auth)

def tweet(msg,api):
        try:
            api.update_status(msg)
        except tweepy.error.TweepError:
            print("oops")

def load_secrets():
    secrets = {}
    secretsfile = open("topsecret","r")
    for line in secretsfile:
        secret=line.split(" ")
        secrets[secret[0]]=secret[1][:-1]
    secretsfile.close()
    return secrets

secrets = load_secrets()
api = tweet_api(secrets)

tweet("hello",api)
