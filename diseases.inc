#include <a_samp>
#include <logging>
#include <personal-space>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

// Iter_init
const 
    MAX_DISEASE_NAME = 40,
    MAX_DISEASES = 30,
    MAX_DISEASE_SYMPTOMS = 15,
    OUTSIDE_CHANCES = 0, 
    INSIDE_CHANCES = 1;

enum e_TRANSMISSION_TYPE {
    TRANSMISSION_CLOSE = 0
};

enum e_SYMPTOMS {
    bool: SYMPTOM_NONE = 0,
    bool: SYMPTOM_COUGHING,
    bool: SYMPTOM_SNEEZING,
    bool: SYMPTOM_HIGHTEMPERATURE,
    bool: SYMPTOM_BLOOD_PRESSURE,
    bool: SYMPTOM_ERECTILE_DISFUNCTION,
    bool: SYMPTOM_NOSE_BLEEDING,
    bool: SYMPTOM_EAR_BLEEDING,
    bool: SYMPTOM_DIARRHEA,
    bool: SYMPTOM_VOMITING,
    bool: SYMPTOM_INSOMNIA,
    bool: SYMTPOM_HEADACHE,
    bool: SYMPTOM_STOMACHACHE,
    bool: SYMPTOM_CHOKING, 
    bool: SYMPTOM_SWELLING,
    bool: SYMPTOM_TOTAL_ORGAN_FAILURE
};

static
    Iterator: disease_Iter<MAX_DISEASES>,
    Iterator: disease_Infecteds<MAX_PLAYERS>,
    disease_Name[MAX_DISEASES][MAX_DISEASE_NAME],
    disease_Level[MAX_DISEASES],
    disease_TimeOfTransmission[MAX_DISEASES],
   // disease_Timer[MAX_DISEASES], 
    disease_TransChances[MAX_DISEASES][2],
    e_TRANSMISSION_TYPE: disease_TransType[MAX_DISEASES],
    e_SYMPTOMS: disease_Symptoms[MAX_DISEASES][MAX_DISEASE_SYMPTOMS];

static 
    bool: player_IsInfected[MAX_PLAYERS];

hook OnGameModeInit()
{
    Iter_Init(disease_Iter);
    Iter_Init(disease_Infecteds);
    return 1;
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

stock Disease_Delete(id)
{
    if(!Iter_Contains(disease_Iter, id)) {
        #if defined DISEASES_LOGGING 
            Log_Error("Invalid disease ID. ID: %d", "diseases.inc", 97, id);
        #endif
        return 0;
    }
    Iter_Remove(disease_Iter, id);
    Log_Info("Deleted Disease > ID: %d", "diseases.inc", 102, id);
    return 1;
}

/*
 * <summary>Infects a player with provided playerid</summary>
 * <param name="playerid">The playerid of the player we want to infect</param>
 */

stock Disease_InfectPlayer(playerid)
{
    player_IsInfected[playerid] = true;
    Iter_Add(disease_Infecteds, playerid);
}

/*
 * <summary>Checks if a player with provided playerid is infected</summary>
 * <param name="playerid">The playerid of the player we want to check</param>
 */

stock bool: Disease_IsPlayerInfected(playerid)
{
    return player_IsInfected[playerid];
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
    if(!Iter_Contains(disease_Iter, diseaseid)) {
        return Log_Error("Unexisting disease > ID: %d", "diseases.inc", 147, diseaseid);
    }
    e_SYMPTOMS: disease_Symptoms[diseaseid][_:symptom] = symptom;
}