#include <a_samp>
#include <logging>
#include <personal-space>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

forward OnDiseaseLevelChange(diseaseid, newlevel, oldlevel);


// Iter_init
const 
    MAX_DISEASE_NAME = 40,
    MAX_DISEASES = 30,
    OUTSIDE_CHANCES = 0, 
    INSIDE_CHANCES = 1;

enum e_TRANSMISSION_TYPE {
    TRANSMISSION_CLOSE = 0
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
    SYMPTOM_TOTAL_ORGAN_FAILURE
};

static
    Iterator: disease_Iter<MAX_DISEASES>,
    Iterator: disease_Infecteds<MAX_PLAYERS>,
    disease_Name[MAX_DISEASES][MAX_DISEASE_NAME],
    disease_Level[MAX_DISEASES],
    disease_TimeOfTransmission[MAX_DISEASES],
    Timer: disease_Timer[MAX_PLAYERS], 
    disease_TransChances[MAX_DISEASES][2],
    e_TRANSMISSION_TYPE: disease_TransType[MAX_DISEASES],
    e_SYMPTOMS: disease_Symptoms[MAX_DISEASES];

static 
    player_Diseases[MAX_PLAYERS];

hook OnGameModeInit()
{
    Iter_Init(disease_Iter);
    Iter_Init(disease_Infecteds);
    return 1;
}

stock bool: Disease_IsValid(diseaseid)
{
    if(!Iter_Contains(disease_Iter, diseaseid)) {
        Log_Error("Unexisting disease > ID: %d", "diseases.inc", 163, diseaseid);
        return false;
    }
    return true;
}

/*
 * <summary>Creates a disease with provided params</summary>
 * <param name="name">Name of disease we're creating</param>
 * <param name="transmission_time">Name of disease we're creating</param>
 * <param name="e_TRANSMISSION_TYPE: transmission_type">Type of transmission</param>
 * <param name="transmission_chances_outside">Chances of getting the disease outside</param>
 * <param name="transmission_chances_inside">Chances of getting the disease inside</param>
 * <param name="disease_lvl">Level of disease</param>
 */

stock Disease_Create(const name[MAX_DISEASE_NAME], transmission_time, e_TRANSMISSION_TYPE: trasmission_type, transmission_chances_outside, transmission_chances_inside, disease_lvl) 
{
    new id = Iter_Alloc(disease_Iter);
    
    strcopy(disease_Name[id], name);
    disease_Level[id] = disease_lvl;
    disease_TimeOfTransmission[id] = transmission_time;
    disease_TransType[id] = trasmission_type;
    disease_TransChances[id][OUTSIDE_CHANCES] = transmission_chances_outside;
    disease_TransChances[id][INSIDE_CHANCES] = transmission_chances_inside;

    #if defined DISEASES_LOGGING 
        Log_Info("Created Disease > Name: %s, transmission time: %d, transmission type: %d, chances of transmitting outside: %d, chances of transmitting inside: %d, disease level: %d", "diseases.inc", 82, 
        disease_Name[id], disease_TimeOfTransmission[id], _:disease_TransType[id], disease_TransChances[id][OUTSIDE_CHANCES], disease_TransChances[id][INSIDE_CHANCES], disease_Level[id]);
    #endif
    return id;
}

/*
 * <summary>Deletes a disease with provided id</summary>
 * <param name="id">The id of the disease we want to delete</param>
 */

stock Disease_Delete(diseaseid)
{
    if(!Disease_IsValid(diseaseid)) {
        return 0;
    }
    Iter_Remove(disease_Iter, diseaseid);
    Log_Info("Deleted Disease > ID: %d", "diseases.inc", 102, diseaseid);
    return 1;
}

/*
 * <summary>Infects a player with provided playerid</summary>
 * <param name="playerid">The playerid of the player we want to infect</param>
 */

stock Disease_InfectPlayer(playerid, diseaseid)
{
    player_Diseases[playerid] |= diseaseid;
    disease_Timer[playerid] = Timer: 0;
    Iter_Add(disease_Infecteds, playerid);
}

/*
 * <summary>Checks if a player with provided playerid is infected</summary>
 * <param name="playerid">The playerid of the player we want to check</param>
 */

stock bool: Disease_IsPlayerInfected(playerid, diseaseid)
{
    if((player_Diseases[playerid] & diseaseid) == diseaseid) {
        return true;
    }
    return false;
}

/*
 * <summary>Cures a player with provided playerid</summary>
 * <param name="playerid">The playerid of the player we want to cure</param>
 */

stock Disease_CurePlayer(playerid)
{
    player_IsInfected[playerid] = false;
    Iter_Remove(disease_Infecteds, playerid);
}

/*
 * <summary>Adds a symptom to a disease</summary>
 * <param name="diseaseid">The disease we want to add the symptom to</param>
 * <param name="e_SYMPTOMS: symptom">The symptom we want to add</param>
 */

stock Disease_AddSymptom(diseaseid, e_SYMPTOMS: symptom)
{
    if(!Disease_IsValid(diseaseid)) {
        return 0;
    }
    disease_Symptoms[diseaseid] |= symptom;
    Log_Info("Added symptom to disease: %d", "diseases.inc", 149, diseaseid);
    return 1;
}

/*
 * <summary>Removes a symptom to a disease</summary>
 * <param name="diseaseid">The disease we want to remove the symptom from</param>
 * <param name="e_SYMPTOMS: symptom">The symptom we want to remove</param>
 */

stock Disease_RemoveSymptom(diseaseid, e_SYMPTOMS: symptom)
{
    if(!Disease_IsValid(diseaseid)) {
        return 0;
    }
    disease_Symptoms[diseaseid] &= ~symptom;
    Log_Info("Removed symptom from disease: %d", "diseases.inc", 166, disease);
    return 1;
}

stock Disease_SetLevel(diseaseid, level)
{
    if(!Disease_IsValid(diseaseid)) {
        return 0;
    }
    CallLocalFunction("OnDiseaseLevelChange", "ddd", diseaseid, level, disease_Level[diseaseid]);
    disease_Level[diseaseid] = level;
}

