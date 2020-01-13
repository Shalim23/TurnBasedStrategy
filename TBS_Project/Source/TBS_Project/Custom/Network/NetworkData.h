#pragma once

//JSON messages keys
static const char* TBSGameMessageJsonKey = "tbsGameMessage";
static const char* MessageNameJsonKey = "messageName";
static const char* PlayersAmountJsonKey = "playersAmount";

//messages from server
static const char* WaitingForPlayersMessageName = "WaitingForPlayers";
static const char* WaitingForReadinessMessageName = "WaitingForReadiness";
static const char* AllAreReadyMessageName = "AllAreReady";
static const char* NotReadyMessageName = "NotReady";

//messages from client
static const char* IsReadyMessageName = "IsReady";
static const char* ClientCancelMessageName = "ClientCancel";
static const char* ClientConnectionMessageName = "ClientConnection";
