#include <fstream>
#include <iostream>
using namespace std;


#include "sierrachart.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


SCDLLName("DataWriter")

/*
Overview
--------
An example of a trading system that enters a new position or
reverses an existing one on the crossover of two study Subgraphs.

When line1 crosses line2 from below the system will go long.

When line1 crosses line2 from above, the system will go short.

Comments
--------
* Let the user of this trading system study select the two study
Subgraph lines to monitor for a crossover. This is accomplished using
sc.Input[].SetStudySubgraphValues. In the Study Settings the user is
provided with list boxes to select the study and subgraph.

* The example uses the Auto Trade Management reversal functionality by
setting sc.SupportReversals to 1. This means that we simply call
sc.BuyEntry for a long and sc.SellEntry for a short with the number of
contracts we want to long/short. In the example the number of contracts
is 1.

  So:
  ** If we are flat, we will enter 1 contract long/short.
  ** If we are currently short, Sierra Chart will reverse the position
     for us and we will be 1 long.
  ** If we are currently long, Sierra Chart will reverse the Position
     for us and we will be 1 short.

* For the simplicity of the example, the study process events on the
  close of the bar.

* To keep the example simple, the study uses market order types to enter the Position.

* Note that if the system enters a Position, and the user manually
  closes the Position, the system will remain flat until the next
  crossover at which point a new Position will be established.

*/


SCSFExport scsf_SC_TradingCrossOverExample(SCStudyInterfaceRef sc) {

    sc.AddMessageToLog("What what", 0);

    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        //return errno;


        // PositionData.

        sc.AddMessageToLog("Didn't work", 0);
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    printf ("The current working directory is %s", cCurrentPath);

    sc.AddMessageToLog(cCurrentPath, 0);

    ofstream myfile;
    myfile.open ("example.txt", std::ofstream::app);

    if (sc.SetDefaults) {



        //myfile.close();

        // Set the configuration and defaults

        sc.GraphName = "DataWriter";

        sc.StudyDescription = "Write the data to file";

        sc.AutoLoop = 1;  // true
        sc.GraphRegion = 0;
        sc.FreeDLL = 1;
        sc.CalculationPrecedence = LOW_PREC_LEVEL;

        sc.AllowMultipleEntriesInSameDirection = false;
        sc.MaximumPositionAllowed = 5;
        sc.SupportReversals = true;

        // This is false by default. Orders will go to the simulation system always.
        sc.SendOrdersToTradeService = false;

        sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;
        sc.SupportAttachedOrdersForTrading = false;

        sc.CancelAllOrdersOnEntriesAndReversals = true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = true;
        sc.AllowOnlyOneTradePerBar = true;

        sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }

    // code below is where we check for crossovers and take action accordingly

    float LastTradePrice = sc.Close[sc.Index];
    float Open = sc.Open[sc.Index];
    float Low = sc.Low[sc.Index];
    float PreviousLow = sc.Low[sc.Index - 1];
    float High = sc.High[sc.Index];
    float PreviousHigh = sc.High[sc.Index - 1];
    float PreviousClose = sc.Close[sc.Index - 1];

    float LastBarSize = PreviousClose - PreviousLow;


    SCString Buffer2;
    Buffer2.Format("%f,%f,%f,%f\n", Open, Low, High, LastTradePrice);
    sc.AddMessageToLog(Buffer2, 0);

    myfile << Buffer2;

    s_SCPositionData PositionData;
    int Result = sc.GetTradePosition(PositionData);

}