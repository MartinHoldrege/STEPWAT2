/**
 * \file ST_mortality.h
 * \brief Functions and variables exported from the \ref MORTALITY module.
 * 
 * \author Chandler Haukap
 * \date February 12 2020
 * \ingroup MORTALITY
 */

#ifndef MORTALITY_H
#define MORTALITY_H

#include "sw_src/include/generic.h"
#include "sw_src/include/SW_Defines.h"

/* --------------------------- Exported Structs ---------------------------- */

/**
 * @brief 10 year running average climate information and 3 year average biomass for use in the wild fire probability model.
 *
 * 10 year simple moving averages of proportion of precipitation during the summer months, mean annual temperature,
 * and annual precipitation are created using this structure, and stored in this structure. 3 year moving average
 * of annual forbs and grasses biomass and perennial forbs an grasses biomass.
 *
 * @author Michael Novotny
 * @date 17 October 2022
 *
 */
typedef struct WildfireClimate_st {
	// the past 10 years of proportion of precipitation during the summer months.
	// the index of the oldest element is count % 10.
	double propSummerPrecip[10];
	double propSummerPrecipAvg;

	double meanAnnTemp[10];
	double meanAnnTempAvg;

	double annPrecip[10];
	double annPrecipAvg;

	// annual forbs and grasses above ground biomass
	double afgAGB[3];
	double afgAGBAvg;

	// perennial forbs and grasses above ground biomass
	double pfgAGB[3];
	double pfgAGBAvg;

	// this value is incremented every time new values are added and new averages are calculated.
	// it is used to index the arrays to update the oldest item in order to calculate
	// a simple 10 year moving average.
	int count;

	// incrementer/indexer for the biomass arrays.
	int biomassCount;
} WildfireClimate;


/**
 * \brief Information used when simulating the cheatgrass-wildfire loop.
 *
 * The biggest determinant in cheatgrass driven wildfire is precipitation,
 * specifically precipitation in Spring and Winter. This struct stores the
 * values from previous Spring and Winter precipitation as well as the running
 * averages of both.
 *
 * Note that "year" in this context refers to the water year, which runs from
 * October to September.
 *
 * \sa _updateCheatgrassPrecip, where this struct is updated each year.
 * \author Chandler Haukap
 * \date 13 January 2020
 * \ingroup MORTALITY
 */
typedef struct CheatgrassPrecip_st {
  /** \brief The Spring precipitation in the previous 3 years.
   * The array is indexed from newest to oldest, meaning prevSpring[0] is the 
   * most recent value. */
  double prevSprings[3];
  /** \brief The precipitation in the last Winter, meaning the Oct-Dec
   * precipitation from 2 years ago and the Jan-Mar precipitation from last 
   * year. */
  double lastWinter;
  /** \brief The current year's Spring precipitation. */
  double currentSpring;

  /** \brief The running average of Spring precipitation. */
  double springMean;
  /** \brief The running average of Winter precipitation. */
  double winterMean;

  /** \brief The sum of October - December precipitation information from last
   *         year.
   * 
   * The variable we use to get precipitation information, \ref SXW, stores 
   * values for the current year, but we need the values from 2 years ago to 
   * calculate last year's winter precipitation. We therefore store it here,
   * even though it does make the struct a little more confusing. 
   */ 
  double lastOctThruDec;
  /** \brief The sum of October - December precipitation information from the
   *         current year.
   * 
   * The variable we use to get precipitation information, \ref SXW, stores 
   * values for the current year, but we need the values from 2 years ago to 
   * calculate last year's winter precipitation. We therefore store it here,
   * even though it does make the struct a little more confusing. 
   */ 
  double thisOctThruDec;
  /** \brief The sum of January - March precipitation information from last
   *         year.
   * 
   * The variable we use to get precipitation information, \ref SXW, stores 
   * values for the current year, but we need the values from 1 year ago to 
   * calculate last year's winter precipitation. We therefore store it here,
   * even though it does make the struct a little more confusing. 
   */ 
  double thisJanThruMar;
} CheatgrassPrecip;

/* -------------------------- Exported Functions --------------------------- */
// See ST_mortality.c for definitions and documentation of these functions.

void mort_Main(Bool *killed);
void mort_EndOfYear(void);

void freeMortalityMemory(void);
void proportion_Recovery(void);
void grazing_EndOfYear(void);
void killAnnuals(void);
void killMaxage(void);
void killExtraGrowth(void);

void initWildfireClimate(void);
WildfireClimate *getWildfireClimate(void);
void setWildfireClimate(WildfireClimate *newWildfireClimate);
void _updateWildfireClimate(double propSummerPrecip, double meanAnnualTemperature, double annualPrecipitation);
void _updateWildfireClimateBiomass(double afgAGB, double pfgAGB);
void _resetWildfireClimateBiomass(void);


void initCheatgrassPrecip(void);
void setCheatgrassPrecip(CheatgrassPrecip* newCheatgrassPrecip);
CheatgrassPrecip* getCheatgrassPrecip(void);

/* ---------------------------- Exported Enums ----------------------------- */

/**
 * \brief All types of mortality.
 *
 * Used to record what killed an individual.
 *
 * \sa indiv_st which instantiates this enumerator.
 *
 * \ingroup MORTALITY
 */
typedef enum {
    Slow,
    NoResources,
    Intrinsic,
    Disturbance,
    LastMort
} MortalityType;


/* =================================================== */
/*            Externed Global Variables                */
/* --------------------------------------------------- */
extern sw_random_t mortality_rng;
extern Bool *_SomeKillage;
extern Bool UseWildfire;

#endif
