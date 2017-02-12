/*
 * @file	mazda_miata_vvt.cpp
 *
 * set engine_type 47
 *
 * @date Oct 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 * http://rusefi.com/forum/viewtopic.php?f=3&t=1095
 */

#include "mazda_miata_vvt.h"
#include "custom_engine.h"
#include "fsio_impl.h"
#include "ego.h"

EXTERN_ENGINE;


static const  float ve18fsioRpmBins[FSIO_TABLE_8] = {
		650, 800,1650,2500,3350,4200,5050,7000
};


float fsio_table[FSIO_TABLE_8][FSIO_TABLE_8] = {
	/* Generated by TS2C on Fri Jan 20 20:35:41 EST 2017*/
	{/* 0 20.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 1 30.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 2 40.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 3 60.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 4 70.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 5 90.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 6 100.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},
	{/* 7 120.000	*//* 0 650.0*/0.000,	/* 1 800.0*/5.000,	/* 2 1650.0*/20.000,	/* 3 2500.0*/20.000,	/* 4 3350.0*/32.000,	/* 5 4200.0*/32.000,	/* 6 5050.0*/32.000,	/* 7 7000.0*/32.000,	},

};


static const  float ve18vvtRpmBins[FUEL_RPM_COUNT] = {
		700.0, 		         850.0 ,		         943.0 ,
		         1112.0 ,		         1310.0 ,		         1545.0 ,
		         1821.0, 		         2146.0, 		         2530.0,
		         2982.0, 		         3515.0 ,		         4144.0 ,
		         4884.0 ,		         5757.0 ,		         6787.0, 		         8000.0};

static const float ve18vvtLoadBins[FUEL_LOAD_COUNT] = {
		25.0 ,		         29.10009765625 ,		         34.0 ,		         39.60009765625 ,
		         46.2001953125 ,		         53.89990234375 ,		         62.7998046875 ,
				 73.2001953125 ,		         85.400390625 ,		         99.5 ,		         116.0 ,
		         135.30078125 ,		         157.69921875 ,		         183.900390625 ,		         214.400390625 ,
		         250.0};

static const fuel_table_t mapBased18vvtVeTable = {
/* Generated by TS2C on Thu Dec 15 22:39:00 EST 2016*/
{/* 0 15.000	*//* 0 700.0*/45.000,	/* 1 1000.0*/45.000,	/* 2 1300.0*/33.400,	/* 3 1700.0*/30.300,	/* 4 2100.0*/25.300,	/* 5 2500.0*/23.500,	/* 6 2900.0*/22.200,	/* 7 3300.0*/21.200,	/* 8 3800.0*/21.300,	/* 9 4300.0*/22.900,	/* 10 4800.0*/32.100,	/* 11 5300.0*/29.400,	/* 12 5800.0*/26.600,	/* 13 6400.0*/25.200,	/* 14 7000.0*/24.400,	/* 15 7600.0*/23.500,	},
{/* 1 30.500	*//* 0 700.0*/45.000,	/* 1 1000.0*/45.000,	/* 2 1300.0*/33.700,	/* 3 1700.0*/33.400,	/* 4 2100.0*/36.300,	/* 5 2500.0*/35.300,	/* 6 2900.0*/35.000,	/* 7 3300.0*/32.200,	/* 8 3800.0*/30.800,	/* 9 4300.0*/32.700,	/* 10 4800.0*/44.400,	/* 11 5300.0*/43.600,	/* 12 5800.0*/38.200,	/* 13 6400.0*/36.100,	/* 14 7000.0*/34.700,	/* 15 7600.0*/34.000,	},
{/* 2 41.500	*//* 0 700.0*/45.000,	/* 1 1000.0*/47.000,	/* 2 1300.0*/37.300,	/* 3 1700.0*/40.300,	/* 4 2100.0*/41.000,	/* 5 2500.0*/40.000,	/* 6 2900.0*/39.700,	/* 7 3300.0*/37.700,	/* 8 3800.0*/37.400,	/* 9 4300.0*/40.100,	/* 10 4800.0*/48.200,	/* 11 5300.0*/47.900,	/* 12 5800.0*/44.000,	/* 13 6400.0*/40.400,	/* 14 7000.0*/39.600,	/* 15 7600.0*/39.500,	},
{/* 3 53.200	*//* 0 700.0*/45.000,	/* 1 1000.0*/45.000,	/* 2 1300.0*/44.000,	/* 3 1700.0*/44.000,	/* 4 2100.0*/44.000,	/* 5 2500.0*/43.500,	/* 6 2900.0*/43.700,	/* 7 3300.0*/42.300,	/* 8 3800.0*/42.000,	/* 9 4300.0*/44.100,	/* 10 4800.0*/51.700,	/* 11 5300.0*/52.200,	/* 12 5800.0*/48.800,	/* 13 6400.0*/45.900,	/* 14 7000.0*/44.300,	/* 15 7600.0*/44.000,	},
{/* 4 65.400	*//* 0 700.0*/49.900,	/* 1 1000.0*/48.400,	/* 2 1300.0*/47.600,	/* 3 1700.0*/47.000,	/* 4 2100.0*/47.500,	/* 5 2500.0*/47.000,	/* 6 2900.0*/47.300,	/* 7 3300.0*/46.100,	/* 8 3800.0*/45.800,	/* 9 4300.0*/47.700,	/* 10 4800.0*/53.900,	/* 11 5300.0*/54.300,	/* 12 5800.0*/53.200,	/* 13 6400.0*/50.300,	/* 14 7000.0*/49.100,	/* 15 7600.0*/49.400,	},
{/* 5 78.300	*//* 0 700.0*/55.400,	/* 1 1000.0*/53.700,	/* 2 1300.0*/50.400,	/* 3 1700.0*/49.500,	/* 4 2100.0*/50.000,	/* 5 2500.0*/49.900,	/* 6 2900.0*/50.200,	/* 7 3300.0*/49.300,	/* 8 3800.0*/48.800,	/* 9 4300.0*/51.000,	/* 10 4800.0*/55.300,	/* 11 5300.0*/57.100,	/* 12 5800.0*/57.900,	/* 13 6400.0*/55.100,	/* 14 7000.0*/54.200,	/* 15 7600.0*/55.300,	},
{/* 6 91.800	*//* 0 700.0*/59.100,	/* 1 1000.0*/57.800,	/* 2 1300.0*/54.200,	/* 3 1700.0*/52.200,	/* 4 2100.0*/52.600,	/* 5 2500.0*/52.800,	/* 6 2900.0*/53.200,	/* 7 3300.0*/52.300,	/* 8 3800.0*/52.000,	/* 9 4300.0*/53.700,	/* 10 4800.0*/57.900,	/* 11 5300.0*/60.800,	/* 12 5800.0*/61.900,	/* 13 6400.0*/60.300,	/* 14 7000.0*/59.300,	/* 15 7600.0*/60.300,	},
{/* 7 106.000	*//* 0 700.0*/62.000,	/* 1 1000.0*/61.700,	/* 2 1300.0*/59.800,	/* 3 1700.0*/57.700,	/* 4 2100.0*/56.900,	/* 5 2500.0*/56.500,	/* 6 2900.0*/56.600,	/* 7 3300.0*/55.700,	/* 8 3800.0*/54.600,	/* 9 4300.0*/56.300,	/* 10 4800.0*/61.600,	/* 11 5300.0*/64.800,	/* 12 5800.0*/64.900,	/* 13 6400.0*/63.800,	/* 14 7000.0*/63.300,	/* 15 7600.0*/64.400,	},
{/* 8 121.000	*//* 0 700.0*/65.600,	/* 1 1000.0*/65.500,	/* 2 1300.0*/64.200,	/* 3 1700.0*/60.800,	/* 4 2100.0*/59.600,	/* 5 2500.0*/60.100,	/* 6 2900.0*/60.700,	/* 7 3300.0*/59.500,	/* 8 3800.0*/57.500,	/* 9 4300.0*/59.500,	/* 10 4800.0*/65.500,	/* 11 5300.0*/70.400,	/* 12 5800.0*/69.800,	/* 13 6400.0*/67.400,	/* 14 7000.0*/67.000,	/* 15 7600.0*/68.300,	},
{/* 9 136.699	*//* 0 700.0*/70.200,	/* 1 1000.0*/69.800,	/* 2 1300.0*/69.100,	/* 3 1700.0*/66.900,	/* 4 2100.0*/64.200,	/* 5 2500.0*/64.300,	/* 6 2900.0*/64.300,	/* 7 3300.0*/63.400,	/* 8 3800.0*/61.700,	/* 9 4300.0*/64.200,	/* 10 4800.0*/69.400,	/* 11 5300.0*/75.800,	/* 12 5800.0*/75.100,	/* 13 6400.0*/72.300,	/* 14 7000.0*/70.000,	/* 15 7600.0*/70.600,	},
{/* 10 153.301	*//* 0 700.0*/75.500,	/* 1 1000.0*/75.200,	/* 2 1300.0*/74.700,	/* 3 1700.0*/73.400,	/* 4 2100.0*/70.800,	/* 5 2500.0*/69.500,	/* 6 2900.0*/68.600,	/* 7 3300.0*/66.200,	/* 8 3800.0*/65.100,	/* 9 4300.0*/69.300,	/* 10 4800.0*/74.500,	/* 11 5300.0*/80.900,	/* 12 5800.0*/82.000,	/* 13 6400.0*/77.500,	/* 14 7000.0*/74.100,	/* 15 7600.0*/74.000,	},
{/* 11 170.699	*//* 0 700.0*/82.600,	/* 1 1000.0*/82.300,	/* 2 1300.0*/82.000,	/* 3 1700.0*/81.000,	/* 4 2100.0*/78.800,	/* 5 2500.0*/76.500,	/* 6 2900.0*/74.700,	/* 7 3300.0*/71.500,	/* 8 3800.0*/71.200,	/* 9 4300.0*/75.200,	/* 10 4800.0*/81.000,	/* 11 5300.0*/90.800,	/* 12 5800.0*/91.100,	/* 13 6400.0*/85.700,	/* 14 7000.0*/81.000,	/* 15 7600.0*/79.700,	},
{/* 12 189.100	*//* 0 700.0*/91.700,	/* 1 1000.0*/91.600,	/* 2 1300.0*/90.900,	/* 3 1700.0*/89.900,	/* 4 2100.0*/88.900,	/* 5 2500.0*/87.300,	/* 6 2900.0*/86.000,	/* 7 3300.0*/81.400,	/* 8 3800.0*/79.400,	/* 9 4300.0*/80.000,	/* 10 4800.0*/90.800,	/* 11 5300.0*/102.100,	/* 12 5800.0*/102.700,	/* 13 6400.0*/99.600,	/* 14 7000.0*/92.900,	/* 15 7600.0*/88.500,	},
{/* 13 208.400	*//* 0 700.0*/100.000,	/* 1 1000.0*/100.000,	/* 2 1300.0*/100.000,	/* 3 1700.0*/99.800,	/* 4 2100.0*/98.900,	/* 5 2500.0*/97.500,	/* 6 2900.0*/96.400,	/* 7 3300.0*/94.700,	/* 8 3800.0*/93.600,	/* 9 4300.0*/93.700,	/* 10 4800.0*/99.400,	/* 11 5300.0*/109.000,	/* 12 5800.0*/110.000,	/* 13 6400.0*/107.500,	/* 14 7000.0*/102.000,	/* 15 7600.0*/97.000,	},
{/* 14 228.699	*//* 0 700.0*/100.000,	/* 1 1000.0*/100.000,	/* 2 1300.0*/100.000,	/* 3 1700.0*/100.000,	/* 4 2100.0*/100.000,	/* 5 2500.0*/100.000,	/* 6 2900.0*/100.000,	/* 7 3300.0*/100.000,	/* 8 3800.0*/100.000,	/* 9 4300.0*/100.000,	/* 10 4800.0*/104.000,	/* 11 5300.0*/116.000,	/* 12 5800.0*/117.400,	/* 13 6400.0*/114.800,	/* 14 7000.0*/108.200,	/* 15 7600.0*/104.300,	},
{/* 15 250.000	*//* 0 700.0*/100.000,	/* 1 1000.0*/100.000,	/* 2 1300.0*/100.000,	/* 3 1700.0*/100.000,	/* 4 2100.0*/100.000,	/* 5 2500.0*/100.000,	/* 6 2900.0*/100.000,	/* 7 3300.0*/100.000,	/* 8 3800.0*/100.000,	/* 9 4300.0*/100.000,	/* 10 4800.0*/107.400,	/* 11 5300.0*/121.500,	/* 12 5800.0*/122.400,	/* 13 6400.0*/120.200,	/* 14 7000.0*/112.700,	/* 15 7600.0*/109.500,	},
};

static const ignition_table_t mapBased18vvtTimingTable = {
/* Generated by TS2C on Thu Dec 15 22:45:29 EST 2016*/
{/* 0 25.000	*//* 0 700.0*/14.800,	/* 1 850.0*/14.800,	/* 2 943.0*/14.800,	/* 3 1112.0*/15.000,	/* 4 1310.0*/20.500,	/* 5 1545.0*/24.100,	/* 6 1821.0*/27.300,	/* 7 2146.0*/30.000,	/* 8 2530.0*/32.600,	/* 9 2982.0*/35.598,	/* 10 3515.0*/37.698,	/* 11 4144.0*/38.728,	/* 12 4884.0*/39.346,	/* 13 5757.0*/39.758,	/* 14 6787.0*/39.964,	/* 15 8000.0*/40.170,	},
{/* 1 29.100	*//* 0 700.0*/14.800,	/* 1 850.0*/14.800,	/* 2 943.0*/14.800,	/* 3 1112.0*/15.100,	/* 4 1310.0*/20.500,	/* 5 1545.0*/24.000,	/* 6 1821.0*/27.200,	/* 7 2146.0*/29.600,	/* 8 2530.0*/32.000,	/* 9 2982.0*/35.190,	/* 10 3515.0*/37.286,	/* 11 4144.0*/38.316,	/* 12 4884.0*/39.037,	/* 13 5757.0*/39.552,	/* 14 6787.0*/39.758,	/* 15 8000.0*/39.964,	},
{/* 2 34.000	*//* 0 700.0*/14.800,	/* 1 850.0*/14.800,	/* 2 943.0*/14.900,	/* 3 1112.0*/15.400,	/* 4 1310.0*/20.600,	/* 5 1545.0*/23.800,	/* 6 1821.0*/26.800,	/* 7 2146.0*/29.000,	/* 8 2530.0*/31.400,	/* 9 2982.0*/34.578,	/* 10 3515.0*/36.771,	/* 11 4144.0*/37.801,	/* 12 4884.0*/38.522,	/* 13 5757.0*/39.140,	/* 14 6787.0*/39.346,	/* 15 8000.0*/39.552,	},
{/* 3 39.600	*//* 0 700.0*/15.200,	/* 1 850.0*/15.200,	/* 2 943.0*/15.400,	/* 3 1112.0*/16.900,	/* 4 1310.0*/20.700,	/* 5 1545.0*/23.600,	/* 6 1821.0*/26.400,	/* 7 2146.0*/28.400,	/* 8 2530.0*/30.800,	/* 9 2982.0*/33.762,	/* 10 3515.0*/35.947,	/* 11 4144.0*/37.183,	/* 12 4884.0*/37.904,	/* 13 5757.0*/38.522,	/* 14 6787.0*/38.728,	/* 15 8000.0*/38.934,	},
{/* 4 46.200	*//* 0 700.0*/15.500,	/* 1 850.0*/15.500,	/* 2 943.0*/15.800,	/* 3 1112.0*/18.000,	/* 4 1310.0*/20.800,	/* 5 1545.0*/23.400,	/* 6 1821.0*/25.900,	/* 7 2146.0*/27.700,	/* 8 2530.0*/30.000,	/* 9 2982.0*/32.844,	/* 10 3515.0*/35.123,	/* 11 4144.0*/36.359,	/* 12 4884.0*/37.183,	/* 13 5757.0*/37.698,	/* 14 6787.0*/37.904,	/* 15 8000.0*/38.213,	},
{/* 5 53.900	*//* 0 700.0*/15.700,	/* 1 850.0*/15.800,	/* 2 943.0*/16.400,	/* 3 1112.0*/18.400,	/* 4 1310.0*/20.900,	/* 5 1545.0*/23.000,	/* 6 1821.0*/25.200,	/* 7 2146.0*/27.000,	/* 8 2530.0*/29.100,	/* 9 2982.0*/31.926,	/* 10 3515.0*/33.990,	/* 11 4144.0*/35.226,	/* 12 4884.0*/36.050,	/* 13 5757.0*/36.771,	/* 14 6787.0*/37.080,	/* 15 8000.0*/37.492,	},
{/* 6 62.800	*//* 0 700.0*/15.900,	/* 1 850.0*/16.100,	/* 2 943.0*/16.900,	/* 3 1112.0*/18.600,	/* 4 1310.0*/20.700,	/* 5 1545.0*/22.600,	/* 6 1821.0*/24.600,	/* 7 2146.0*/26.100,	/* 8 2530.0*/28.000,	/* 9 2982.0*/30.702,	/* 10 3515.0*/32.436,	/* 11 4144.0*/33.558,	/* 12 4884.0*/34.476,	/* 13 5757.0*/35.190,	/* 14 6787.0*/35.598,	/* 15 8000.0*/36.210,	},
{/* 7 73.200	*//* 0 700.0*/16.200,	/* 1 850.0*/16.500,	/* 2 943.0*/17.200,	/* 3 1112.0*/18.700,	/* 4 1310.0*/20.400,	/* 5 1545.0*/22.000,	/* 6 1821.0*/23.800,	/* 7 2146.0*/25.000,	/* 8 2530.0*/26.700,	/* 9 2982.0*/28.600,	/* 10 3515.0*/30.400,	/* 11 4144.0*/31.600,	/* 12 4884.0*/32.300,	/* 13 5757.0*/33.100,	/* 14 6787.0*/33.700,	/* 15 8000.0*/34.300,	},
{/* 8 85.400	*//* 0 700.0*/16.300,	/* 1 850.0*/17.000,	/* 2 943.0*/17.500,	/* 3 1112.0*/18.600,	/* 4 1310.0*/19.800,	/* 5 1545.0*/21.300,	/* 6 1821.0*/22.700,	/* 7 2146.0*/23.600,	/* 8 2530.0*/25.300,	/* 9 2982.0*/27.000,	/* 10 3515.0*/28.600,	/* 11 4144.0*/30.000,	/* 12 4884.0*/30.900,	/* 13 5757.0*/31.600,	/* 14 6787.0*/32.200,	/* 15 8000.0*/32.900,	},
{/* 9 99.500	*//* 0 700.0*/16.200,	/* 1 850.0*/16.800,	/* 2 943.0*/17.300,	/* 3 1112.0*/18.000,	/* 4 1310.0*/19.000,	/* 5 1545.0*/20.200,	/* 6 1821.0*/21.300,	/* 7 2146.0*/22.100,	/* 8 2530.0*/23.400,	/* 9 2982.0*/25.200,	/* 10 3515.0*/26.700,	/* 11 4144.0*/28.000,	/* 12 4884.0*/28.900,	/* 13 5757.0*/29.800,	/* 14 6787.0*/30.700,	/* 15 8000.0*/31.400,	},
{/* 10 116.000	*//* 0 700.0*/15.300,	/* 1 850.0*/15.900,	/* 2 943.0*/16.300,	/* 3 1112.0*/16.800,	/* 4 1310.0*/17.400,	/* 5 1545.0*/18.300,	/* 6 1821.0*/19.300,	/* 7 2146.0*/20.000,	/* 8 2530.0*/21.500,	/* 9 2982.0*/23.000,	/* 10 3515.0*/24.500,	/* 11 4144.0*/25.900,	/* 12 4884.0*/26.900,	/* 13 5757.0*/27.700,	/* 14 6787.0*/28.700,	/* 15 8000.0*/29.500,	},
{/* 11 135.301	*//* 0 700.0*/13.600,	/* 1 850.0*/13.900,	/* 2 943.0*/14.100,	/* 3 1112.0*/14.600,	/* 4 1310.0*/15.100,	/* 5 1545.0*/15.900,	/* 6 1821.0*/17.000,	/* 7 2146.0*/17.700,	/* 8 2530.0*/19.100,	/* 9 2982.0*/20.600,	/* 10 3515.0*/22.100,	/* 11 4144.0*/23.300,	/* 12 4884.0*/24.500,	/* 13 5757.0*/25.500,	/* 14 6787.0*/26.500,	/* 15 8000.0*/27.300,	},
{/* 12 157.699	*//* 0 700.0*/11.200,	/* 1 850.0*/11.600,	/* 2 943.0*/11.800,	/* 3 1112.0*/12.200,	/* 4 1310.0*/12.500,	/* 5 1545.0*/13.200,	/* 6 1821.0*/14.200,	/* 7 2146.0*/15.100,	/* 8 2530.0*/16.500,	/* 9 2982.0*/17.700,	/* 10 3515.0*/19.200,	/* 11 4144.0*/20.600,	/* 12 4884.0*/21.700,	/* 13 5757.0*/22.800,	/* 14 6787.0*/24.000,	/* 15 8000.0*/25.000,	},
{/* 13 183.900	*//* 0 700.0*/8.300,	/* 1 850.0*/8.700,	/* 2 943.0*/9.000,	/* 3 1112.0*/9.300,	/* 4 1310.0*/9.800,	/* 5 1545.0*/10.400,	/* 6 1821.0*/11.300,	/* 7 2146.0*/12.100,	/* 8 2530.0*/13.100,	/* 9 2982.0*/14.500,	/* 10 3515.0*/16.000,	/* 11 4144.0*/17.200,	/* 12 4884.0*/18.500,	/* 13 5757.0*/19.700,	/* 14 6787.0*/21.000,	/* 15 8000.0*/22.100,	},
{/* 14 214.400	*//* 0 700.0*/5.000,	/* 1 850.0*/5.300,	/* 2 943.0*/5.600,	/* 3 1112.0*/5.900,	/* 4 1310.0*/6.400,	/* 5 1545.0*/7.000,	/* 6 1821.0*/7.600,	/* 7 2146.0*/8.400,	/* 8 2530.0*/9.600,	/* 9 2982.0*/10.800,	/* 10 3515.0*/12.100,	/* 11 4144.0*/13.400,	/* 12 4884.0*/14.800,	/* 13 5757.0*/16.200,	/* 14 6787.0*/17.400,	/* 15 8000.0*/18.700,	},
{/* 15 250.000	*//* 0 700.0*/1.100,	/* 1 850.0*/1.400,	/* 2 943.0*/1.600,	/* 3 1112.0*/2.000,	/* 4 1310.0*/2.500,	/* 5 1545.0*/3.000,	/* 6 1821.0*/3.700,	/* 7 2146.0*/4.400,	/* 8 2530.0*/5.400,	/* 9 2982.0*/6.600,	/* 10 3515.0*/7.600,	/* 11 4144.0*/9.000,	/* 12 4884.0*/10.500,	/* 13 5757.0*/12.000,	/* 14 6787.0*/13.400,	/* 15 8000.0*/14.900,	},
};

void setMazdaMiata2003EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	engineConfiguration->trigger.type = TT_MIATA_VVT;
	setOperationMode(engineConfiguration, FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);
	engineConfiguration->specs.displacement = 1.8;

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInput = GPIOC_6;

	boardConfiguration->alternatorControlPin = GPIOE_10;
	boardConfiguration->alternatorControlPinMode = OM_OPENDRAIN;

	// enable altdebug
	engineConfiguration->targetVBatt = 14.5;
	engineConfiguration->alternatorControl.offset = 40;
	engineConfiguration->alternatorControl.pFactor = 14;
	engineConfiguration->alternatorControl.iFactor = 0.1;
	engineConfiguration->alternatorControl.dFactor = 0;
	engineConfiguration->alternatorDT = 10;

	// set idle_position 35
	boardConfiguration->manIdlePosition = 35;


	// set vvt_mode 3
	engineConfiguration->vvtMode = MIATA_NB2;
	boardConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->vvtDisplayInverted = true;
	engineConfiguration->vvtOffset = 83;

	engineConfiguration->activateAuxPid1 = true; // todo: remove this field?
	engineConfiguration->auxPidFrequency[0] = 300;
	engineConfiguration->auxPidPins[0] = GPIOE_3; // VVT solenoid control
	//	/**
	//	 * set_fsio_setting 0 0.55
	//	 */
	boardConfiguration->fsio_setting[0] = 0.0;
//	setFsioExt(0, GPIOE_3, "0 fsio_setting", 400 PASS_ENGINE_PARAMETER);



	engineConfiguration->dizzySparkOutputPin = GPIOE_8;

	// set global_trigger_offset_angle 0
	engineConfiguration->globalTriggerAngleOffset = 0;

	// enable trigger_details
	engineConfiguration->isPrintTriggerSynchDetails = false;

	// todo: change to 15?
	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 3;
	// set cranking_fuel 9
	engineConfiguration->cranking.baseFuel = 9;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOC_9;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;


	// set_whole_ve_map 80

	memcpy(config->veRpmBins, ve18vvtRpmBins, sizeof(ve18vvtRpmBins));
	memcpy(config->veLoadBins, ve18vvtLoadBins, sizeof(ve18vvtLoadBins));
	copyFuelTable(mapBased18vvtVeTable, config->veTable);

	memcpy(config->ignitionRpmBins, ve18vvtRpmBins, sizeof(ve18vvtRpmBins));
	memcpy(config->ignitionLoadBins, ve18vvtLoadBins, sizeof(ve18vvtLoadBins));
	copyTimingTable(mapBased18vvtTimingTable, config->ignitionTable);


	memcpy(config->fsioTable1RpmBins, ve18fsioRpmBins, sizeof(ve18fsioRpmBins));
	for (int loadIndex = 0; loadIndex < FSIO_TABLE_8; loadIndex++) {
			for (int rpmIndex = 0; rpmIndex < FSIO_TABLE_8; rpmIndex++) {
				config->fsioTable1[loadIndex][rpmIndex] = fsio_table[loadIndex][rpmIndex];
			}
		}


	engineConfiguration->tpsMin = 100; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 650; // convert 12to10 bit (ADC/4)

	// enable cylinder_cleanup
	engineConfiguration->isCylinderCleanupEnabled = true;



//	0.0825
//	0.1375
//	6.375
//	10.625
	boardConfiguration->nb2ratioFrom = 8.50 * 0.75;
	boardConfiguration->nb2ratioTo = 14;
	engineConfiguration->nbVvtIndex = 0;


	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 01-05 (purple) - #195500-4060
	 */
	engineConfiguration->injector.flow = 265;

	boardConfiguration->malfunctionIndicatorPin = GPIOD_9;
//	boardConfiguration->malfunctionIndicatorPinMode = OM_INVERTED;


	/**
	 * PA4 Wideband O2 Sensor
	 */
	// todo: re-wire the board to use default input channel!
	engineConfiguration->afr.hwChannel = EFI_ADC_4;

	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER);

}


