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

void logTheCurrentDirectory(s_sc &sc);

#endif

void logTheCurrentDirectory(s_sc &sc) {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        sc.AddMessageToLog("Didn't work", 0);
        return;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    sc.AddMessageToLog(cCurrentPath, 0);
}


SCDLLName("DataWriter")

SCSFExport scsf_SC_TradingCrossOverExample(SCStudyInterfaceRef sc) {

    SCInputRef dailyLowRef = sc.Input[0];
    SCInputRef dailyHighRef = sc.Input[1];
    SCInputRef outputFileName = sc.Input[2];
    SCInputRef todayLowRef = sc.Input[3];
    SCInputRef todayHighRef = sc.Input[4];
    //BollingerBangs
    SCInputRef topBandRef = sc.Input[5];
    SCInputRef movingAverageRef = sc.Input[6];
    SCInputRef bottomBandRef = sc.Input[7];

    //logTheCurrentDirectory(sc);

    if(sc.Index == 0)
    {
        SCString outputFileStr;
        outputFileStr.Format("%s-%d.csv",sc.Symbol.GetChars(), sc.SecondsPerBar / 60);

        outputFileName.Name = "Output File";
        //outputFileName.SetString(sc.Symbol);
        outputFileName.SetString(outputFileStr);
        remove(outputFileName.GetString());
    }

    ofstream outputStream;
    outputStream.open (outputFileName.GetString(), std::ofstream::app);

    if (sc.SetDefaults) {
        dailyLowRef.Name = "Daily Low";
        dailyLowRef.SetStudySubgraphValues(1, 2);

        dailyHighRef.Name = "Daily High";
        dailyHighRef.SetStudySubgraphValues(1, 1);

        SCString outputFileStr;
        outputFileStr.Format("%s-%d.csv",sc.Symbol.GetChars(), sc.SecondsPerBar / 60);

        outputFileName.Name = "Output File";
        //outputFileName.SetString(sc.Symbol);
        outputFileName.SetString(outputFileStr);

        todayLowRef.Name = "Today Low";
        todayLowRef.SetStudySubgraphValues(4, 2);

        todayHighRef.Name = "Today High";
        todayHighRef.SetStudySubgraphValues(4, 1);

        //Bollinger Bands
        topBandRef.Name = "Top Band";
        topBandRef.SetStudySubgraphValues(3, 0);

        movingAverageRef.Name = "Moving Average";
        movingAverageRef.SetStudySubgraphValues(3, 1);

        bottomBandRef.Name = "Bottom Band";
        bottomBandRef.SetStudySubgraphValues(3, 2);

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


    SCFloatArray dailyLows;
    sc.GetStudyArrayUsingID(dailyLowRef.GetStudyID(), dailyLowRef.GetSubgraphIndex(), dailyLows);
    float dailyLow = dailyLows[sc.Index];

    SCFloatArray dailyHighs;
    sc.GetStudyArrayUsingID(dailyHighRef.GetStudyID(), dailyHighRef.GetSubgraphIndex(), dailyHighs);
    float dailyHigh = dailyHighs[sc.Index];

    SCFloatArray todayLows;
    sc.GetStudyArrayUsingID(todayLowRef.GetStudyID(), todayLowRef.GetSubgraphIndex(), todayLows);
    float todayLow = todayLows[sc.Index];

    SCFloatArray todayHighs;
    sc.GetStudyArrayUsingID(todayHighRef.GetStudyID(), todayHighRef.GetSubgraphIndex(), todayHighs);
    float todayHigh = todayHighs[sc.Index];

    SCFloatArray topBands;
    sc.GetStudyArrayUsingID(topBandRef.GetStudyID(), topBandRef.GetSubgraphIndex(), topBands);
    float topBand = topBands[sc.Index];

    SCFloatArray movingAverages;
    sc.GetStudyArrayUsingID(movingAverageRef.GetStudyID(), movingAverageRef.GetSubgraphIndex(), movingAverages);
    float movingAverage = movingAverages[sc.Index];

    SCFloatArray bottomBands;
    sc.GetStudyArrayUsingID(bottomBandRef.GetStudyID(), bottomBandRef.GetSubgraphIndex(), bottomBands);
    float bottomBand = bottomBands[sc.Index];


    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    sc.BaseDateTimeIn[sc.Index].GetDateTimeYMDHMS(year, month, day, hour, minute, second );
    float LastTradePrice = sc.Close[sc.Index];
    float Open = sc.Open[sc.Index];
    float Low = sc.Low[sc.Index];
    float PreviousLow = sc.Low[sc.Index - 1];
    float High = sc.High[sc.Index];
    float PreviousHigh = sc.High[sc.Index - 1];
    float PreviousClose = sc.Close[sc.Index - 1];

    float LastBarSize = PreviousClose - PreviousLow;

    SCString dataLine;
    dataLine.Format("%d-%d-%dT%d:%d:%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", year, month, day, hour, minute, second, Open, Low,
                    High, LastTradePrice, dailyLow, dailyHigh, todayLow, todayHigh, topBand, movingAverage, bottomBand);
    //sc.AddMessageToLog(Buffer2, 0);

    outputStream << dataLine;

    s_SCPositionData PositionData;
    int Result = sc.GetTradePosition(PositionData);
}