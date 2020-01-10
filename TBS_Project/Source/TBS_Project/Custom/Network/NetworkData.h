#pragma once

//JSON messages keys
static const char* TBSGameMessageJsonKey = "TBSGameMessage";
static const char* MessageNameJsonKey = "MessageName";
static const char* PlayersAmountJsonKey = "PlayersAmount";

//messages from server
static const char* WaitingForPlayersMessage = "WaitingForPlayers";
static const char* WaitingForReadinessMessage = "WaitingForReadiness";
static const char* AllAreReadyMessage = "AllAreReady";
static const char* NotReadyMessage = "NotReady";

//messages from client
static const char* IsReadyMessage = "IsReady";
static const char* ClientCancelMessage = "ClientCancel";
