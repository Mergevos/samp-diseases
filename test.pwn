#include <a_samp>
#include <personal-space>
#include <mathutil>
#include <YSI_Visual\y_commands>

main()
{
    return ;
}

static 
    //Iterator: infected_Players<MAX_PLAYERS>,
    bool: infected_IsPlayer[MAX_PLAYERS],
    infected_Target[MAX_PLAYERS];



public OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection, Float: angle) {

 /*   new str[130];
    format(str, sizeof(str), "Angle of direction %f, approach %d needed approach %d", angle, _:GetApproachDirection(angle), _:APPROACH_FROM_FRONT);
    SendClientMessageToAll(-1, str);
        

    if(fromDirection == APPROACH_FROM_FRONT) {
        */
    new globalstr[128];
    format(globalstr, sizeof(globalstr), "Igrac %d prisao igracu %d", playerid, targetid);
    SendClientMessageToAll(-1, globalstr);
    infected_Target[playerid] = targetid;
    if(infected_IsPlayer[targetid] == true) {
        SetTimerEx("__SpreadDis", 4000, false, "dd", playerid, targetid);
        SendClientMessageToAll(-1, "Pokrenut timer");
    }

	return 0;
}

public OnPlayerLeavePlayer(playerid, targetid) {

    infected_Target[playerid] = -1;
    new globalstr[128];
    format(globalstr, sizeof(globalstr), "Igrac %d otisao od igraca %d", playerid, targetid);
    SendClientMessageToAll(-1, globalstr);
	return 0;
}

forward __SpreadDis(playerid, targetid);
public __SpreadDis(playerid, targetid)
{
    SendClientMessageToAll(-1, "Pozvan spreaddis");
    if(IsPlayerNextToPlayer(playerid, targetid)) {
        if(frandom(100) < 60.0) {
            infected_IsPlayer[playerid] = true;
            SendClientMessageToAll(-1, "Uspesna transmijsija covid19");
        }
        else SendClientMessageToAll(-1, "Neuspela transmisija covid19");
    }
}

YCMD:test(playerid, params[], help)
{
    infected_IsPlayer[playerid] = true;
    return COMMAND_OK;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    return 1;
}