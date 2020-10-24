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
    infected_Timer[MAX_PLAYERS];

public OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection, Float: angle) {

 /*   new str[130];
    format(str, sizeof(str), "Angle of direction %f, approach %d needed approach %d", angle, _:GetApproachDirection(angle), _:APPROACH_FROM_FRONT);
    SendClientMessageToAll(-1, str);
        

    if(fromDirection == APPROACH_FROM_FRONT) {
        */
    new globalstr[128];
    format(globalstr, sizeof(globalstr), "Igrac %d prisao igracu %d", playerid, targetid);
    SendClientMessageToAll(-1, globalstr);
    infected_Timer[playerid] = SetTimerEx("__SpreadDis", 4000, false, "dd", playerid, targetid);
    SendClientMessageToAll(-1, "Pokrenut timer");

	return 0;
}

public OnPlayerLeavePlayer(playerid, targetid) {

    new globalstr[128];
    format(globalstr, sizeof(globalstr), "Igrac %d otisao od igraca %d", playerid, targetid);
    SendClientMessageToAll(-1, globalstr);
    if(infected_Timer[playerid] != 0) {
        KillTimer(infected_Timer[playerid]);
    }
	return 0;
}

forward __SpreadDis(playerid, targetid);
public __SpreadDis(playerid, targetid)
{
    new 
        Float: Chance = frandom(100);
    SendClientMessageToAll(-1, "Pozvan spreaddis");
    if(IsPlayerNextToPlayer(playerid, targetid)) {
        if(!infected_IsPlayer[playerid] && !infected_IsPlayer[targetid]) {
            return SendClientMessageToAll(-1, "Nezarazeni.");
        }
        else if(infected_IsPlayer[playerid] && infected_IsPlayer[targetid]) {
            return SendClientMessageToAll(-1, "zarazeni oboje.");
        }
        if(Chance < 60.0) {
            if(infected_IsPlayer[targetid] && !infected_IsPlayer[playerid]) {
                infected_IsPlayer[playerid] = true;
                SendClientMessageToAll(-1, "Uspesna transmijsija covid19 na igraca koji je prisao");
                return 0;
            } else if(infected_IsPlayer[playerid] && !infected_IsPlayer[targetid]) {
                infected_IsPlayer[targetid] = true;
                SendClientMessageToAll(-1, "Uspesna transmijsija covid19 na igraca koji je stajao");
                return 0;
            }
        } else SendClientMessageToAll(-1, "Neuspela transmisija covid19");
    }
    return 0;
}

YCMD:test(playerid, params[], help)
{
    infected_IsPlayer[playerid] = true;
    return COMMAND_OK;
}

YCMD:test2(playerid, params[], help)
{
    infected_IsPlayer[playerid] = false;
    return COMMAND_OK;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    return 1;
}