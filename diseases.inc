#include <a_samp>
#include <logging>
#include <personal-space>
#include <YSI_Coding\y_hooks>

forward OnDiseaseLevelChange(diseaseid, newlevel, oldlevel);


// Iter_init
const 
    MAX_DISEASE_NAME = 40,
    OUTSIDE_CHANCES = 0, 
    INSIDE_CHANCES = 1;


enum e_DISEASES(<<=1) {
    DISEASE_NONE = 0,
    DISEASE_COVID19 = 1,
    DISEASE_EBOLA,
    DISEASE_MERS,
    DISEASE_SARS, 
    DISEASE_AIDS,
    DISEASE_LUNG_CANCER,
    DISEASE_LIVER_CANCER,
    DISEASE_DIABETES,
    DISEASE_TUBERCULOSIS,
    DISEASE_CRE,
    DISEASE_EPILEPSY,
    DISEASE_HEPATITIS_A,
    DISEASE_HEPATITIS_B,
    DISEASE_MEASLESS
};

enum e_SYMPTOMS(<<=1) {
    SYMPTOM_NONE = 0,
    SYMPTOM_COUGHING = 1,
    SYMPTOM_SNEEZING,
    SYMPTOM_HIGHTEMPERATURE,
    SYMPTOM_BLOOD_PRESSURE,
    SYMPTOM_ERECTILE_DISFUNCTION,
    SYMPTOM_NOSE_BLEEDING,
    SYMPTOM_EAR_BLEEDING,
    SYMPTOM_DIARRHEA,
    SYMPTOM_VOMITING,
    SYMPTOM_INSOMNIA,
    SYMTPOM_HEADACHE,
    SYMPTOM_STOMACHACHE,
    SYMPTOM_CHOKING, 
    SYMPTOM_SWELLING,
    SYMPTOM_TOTAL_ORGAN_FAILURE,
    SYMPTOM_FATIGUE,
    SYMPTOM_HAIR_LOSS,
    SYMPTOM_LOSE_APPETITION
    
};

static
    Iterator: disease_Infecteds<MAX_PLAYERS>,
    e_SYMPTOMS: disease_Symptoms[MAX_PLAYERS],
    disease_Timer[MAX_PLAYERS];

static 
    e_DISEASES: player_Diseases[MAX_PLAYERS];

hook OnGameModeInit()
{
    Iter_Init(disease_Iter);
    Iter_Init(disease_Infecteds);
    return 1;
}

/*
 * <summary>Infects a player with provided playerid</summary>
 * <param name="playerid">The playerid of the player we want to infect</param>
 */

stock Disease_InfectPlayer(playerid, e_DISEASES: diseaseid)
{
    player_Diseases[playerid] |= diseaseid;
    Iter_Add(disease_Infecteds, playerid);
}

/*
 * <summary>Checks if a player with provided playerid is infected</summary>
 * <param name="playerid">The playerid of the player we want to check</param>
 */

stock bool: Disease_IsPlayerInfected(playerid, e_DISEASES: diseaseid)
{
    return !!(player_Diseases[playerid] & diseaseid);
}

/*
 * <summary>Cures a player with provided playerid</summary>
 * <param name="playerid">The playerid of the player we want to cure</param>
 */

stock Disease_CurePlayerDisease(playerid, e_DISEASES: diseaseid)
{
    if(!(Disease_IsPlayerInfected(playerid, diseaseid))) {
        return 0;
    }
    player_Diseases[playerid] &= ~diseaseid;
    Iter_Remove(disease_Infecteds, playerid);
}

/*
 * <summary>Adds a symptom to a disease</summary>
 * <param name="diseaseid">The disease we want to add the symptom to</param>
 * <param name="e_SYMPTOMS: symptom">The symptom we want to add</param>
 */

stock Disease_AddSymptomToPlayer(playerid, e_SYMPTOMS: symptom)
{
    disease_Symptoms[playerid] |= symptom;
    Log_Info("Added symptom to from player: %d", "diseases.inc", 149, playerid);
    return 1;
}

/*
 * <summary>Removes a symptom to a disease</summary>
 * <param name="diseaseid">The disease we want to remove the symptom from</param>
 * <param name="e_SYMPTOMS: symptom">The symptom we want to remove</param>
 */

stock Disease_RemoveSymptom(playerid, e_SYMPTOMS: symptom)
{
    disease_Symptoms[playerid] &= ~symptom;
    Log_Info("Removed symptom from player: %d", "diseases.inc", 166, playerid);
    return 1;
}

// ----------------------------------------------

hook OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection, Float:angle)
{
    disease_Timer[playerid] = SetTimerEx("__SpreadDis", 4000, false, "dd", playerid, targetid);
    return 0;
}

forward __SpreadDis(playerid, targetid);
public __SpreadDis(playerid, targetid)
{
    new 
        Float: Chance = frandom(100);
    if(IsPlayerNextToPlayer(playerid, targetid)) {
        if(!Disease_IsPlayerInfected(playerid, e_DISEASES) && !Disease_IsPlayerInfected(targetid, e_DISEASES)) {
            return SendClientMessageToAll(-1, "Nezarazeni."); // has to check if player have any disease
        }
        else if(!Disease_IsPlayerInfected(playerid, e_DISEASES) && !Disease_IsPlayerInfected(targetid, e_DISEASES)) {
            return SendClientMessageToAll(-1, "zarazeni oboje.");
        }
        /*if(Chance < 60.0) { // add within an interior and outside
            if(Disease_IsPlayerInfected(targetid, e_DISEASES) && !infected_IsPlayer[playerid]) {
                infected_IsPlayer[playerid] = true;
                SendClientMessageToAll(-1, "Uspesna transmijsija covid19 na igraca koji je prisao");
                return 0;
            } else if(infected_IsPlayer[playerid] && !infected_IsPlayer[targetid]) {
                infected_IsPlayer[targetid] = true;
                SendClientMessageToAll(-1, "Uspesna transmijsija covid19 na igraca koji je stajao");
                return 0;
            }
        } else SendClientMessageToAll(-1, "Neuspela transmisija covid19");*/
    }
    return 0;
}