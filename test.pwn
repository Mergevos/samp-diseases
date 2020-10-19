#include <a_samp>
#include <personal-space>

main()
{
    return ;
}

static 
    Iterator: infected_Players<MAX_PLAYERS>,
    bool: infected_IsPlayer[MAX_PLAYERS],
    infected_Target[MAX_PLAYERS];


public OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection) {
	new str[128];

    if(fromDirection == APPROACH_FROM_FRONT) {
        infected_Target[playerid] = targetid;
        if(infected_IsPlayer[targetid] == true) {
            SetTimerEx("__SpreadDis", 4000, false, "d", playerid);
        }

    }
	return 0;
}

public OnPlayerLeavePlayer(playerid, targetid) {

    infected_Target[playerid] = -1;
	return 0;
}

forward __SpreadDis(playerid);
public __SpreadDis(playerid)
{
    infected_IsPlayer[playerid] = true;
}