#include "sierrachart.h"

bool getLowCheck(const s_sc &sc, const SCFloatArray &lowOfTheDay, float Low, float PreviousLow, int lowCheckPref);

bool getHighCheck(const s_sc &sc, const SCFloatArray &highOfTheDay, float High, float PreviousHigh, int highCheckPref);

void logEntryMessage(s_sc &sc, const s_SCPositionData &PositionData, const s_SCNewOrder &order);

SCDLLName("TradingSystemFailAtExtremes")

bool getLowCheck(const s_sc &sc, const SCFloatArray &lowOfTheDay, float Low, float PreviousLow, int lowCheckPref) {
    bool lowCheck;
    if (lowCheckPref == 0) {
        //Current Candle is below last candle.
        lowCheck = Low < PreviousLow;
    } else {
        //New low for the day is put in.
        lowCheck = lowOfTheDay[sc.Index] < lowOfTheDay[sc.Index - 1];
    }
    return lowCheck;
}

bool getHighCheck(const s_sc &sc, const SCFloatArray &highOfTheDay, float High, float PreviousHigh, int highCheckPref) {
    bool highCheck;
    if (highCheckPref == 0) {
        //Current Candle is above the last candle.
        highCheck = High > PreviousHigh;
    } else {
        //New High for the day put in.
        highCheck = highOfTheDay[sc.Index] > highOfTheDay[sc.Index - 1];
    }
    return highCheck;
}

void logEntryMessage(s_sc &sc, const s_SCPositionData &PositionData, const s_SCNewOrder &order) {
    SCString Buffer2;

    Buffer2.Format(" AveragePrice %f Target %f Stop %f Total Trades %d", PositionData.AveragePrice,
                   order.Target1Price, order.Stop1Price, PositionData.TotalTrades);
    sc.AddMessageToLog(PositionData.Symbol + Buffer2, 0);
}

SCSFExport scsf_SC_FadeBreakouts(SCStudyInterfaceRef sc) {

    SCInputRef line1Ref = sc.Input[0];
    SCInputRef line2Ref = sc.Input[1];
    SCInputRef line3Ref = sc.Input[2];
    SCInputRef line4Ref = sc.Input[3];
    int highLowCheckPref = sc.Input[4].GetInt();

    SCSubgraphRef bullish = sc.Subgraph[0];
    SCSubgraphRef bearish = sc.Subgraph[1];

    if (sc.SetDefaults) {

        sc.GraphName = "Fade Breakouts";
        sc.StudyDescription = "Fade breakouts";

        sc.AutoLoop = 1;  // true
        sc.GraphRegion = 0;
        sc.FreeDLL = 1;
        sc.CalculationPrecedence = LOW_PREC_LEVEL;

        line1Ref.Name = "Daily Low";
        line1Ref.SetStudySubgraphValues(1, 0);

        line2Ref.Name = "Daily High";
        line2Ref.SetStudySubgraphValues(1, 0);

        line3Ref.Name = "Today's Low";
        line3Ref.SetStudySubgraphValues(1, 0);

        line4Ref.Name = "Today's High";
        line4Ref.SetStudySubgraphValues(1, 0);


        bullish.Name = "Bullish";
        bullish.DrawStyle = DRAWSTYLE_POINTHIGH;
        bullish.LineWidth = 3;

        bearish.Name = "Bearish";
        bearish.DrawStyle = DRAWSTYLE_POINTLOW;
        bearish.LineWidth = 3;

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

    // only process at the close of the bar; if it has not closed don't do anything
    if (sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_NOT_CLOSED) {
        return;
    }

    // using the line1Ref and line2Ref input variables, retrieve the subgraph arrays into line1,
    // line2 arrays respectively
    SCFloatArray dailyLows;
    sc.GetStudyArrayUsingID(line1Ref.GetStudyID(), line1Ref.GetSubgraphIndex(), dailyLows);

    SCFloatArray dailyHighs;
    sc.GetStudyArrayUsingID(line2Ref.GetStudyID(), line2Ref.GetSubgraphIndex(), dailyHighs);


    SCFloatArray todaysLows;
    sc.GetStudyArrayUsingID(line3Ref.GetStudyID(), line3Ref.GetSubgraphIndex(), todaysLows);

    SCFloatArray todaysHighs;
    sc.GetStudyArrayUsingID(line4Ref.GetStudyID(), line4Ref.GetSubgraphIndex(), todaysHighs);

    // code below is where we check for crossovers and take action accordingly

    float LastTradePrice = sc.Close[sc.Index];
    float Open = sc.Open[sc.Index];
    float Low = sc.Low[sc.Index];
    float PreviousLow = sc.Low[sc.Index - 1];
    float High = sc.High[sc.Index];
    float PreviousHigh = sc.High[sc.Index - 1];
    float PreviousClose = sc.Close[sc.Index - 1];

    float currentLow = dailyLows[sc.Index];
    float currentHigh = dailyHighs[sc.Index];

    float LastBarSize = PreviousClose - PreviousLow;

    s_SCPositionData PositionData;
    int Result = sc.GetTradePosition(PositionData);

    //Buy at lows
    if (Low < currentLow && LastTradePrice > Open && LastTradePrice > currentLow && Open > currentLow &&
        todaysLows[sc.Index] < todaysLows[sc.Index - 1] && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {


        // mark the crossover on the chart
        bullish[sc.Index] = 1;

        // Create a market order and enter long.
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        order.Stop1Price = Low - 1 * sc.TickSize;
        order.Target1Price = LastTradePrice + (LastTradePrice - Low) + (10 * sc.TickSize);
        order.OCOGroup1Quantity = 1; // If this is left at the default of 0, then it will be automatically set.

        sc.BuyEntry(order);
        SCString Buffer2;

        Buffer2.Format(" AveragePrice %f Target %f Stop %f Total Trades %d", PositionData.AveragePrice,
                       order.Target1Price, order.Stop1Price, PositionData.TotalTrades);
        sc.AddMessageToLog(PositionData.Symbol + Buffer2, 0);
    }
        //Sell at highs
    else if (High > currentHigh && LastTradePrice < Open && LastTradePrice < currentHigh && Open < currentHigh &&
             todaysHighs[sc.Index] > todaysHighs[sc.Index - 1] && sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {

        // mark the crossover on the chart
        bearish[sc.Index] = 1;

        // create a market order and enter short
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        order.Stop1Price = High + 1 * sc.TickSize;
        order.Target1Price = LastTradePrice - (High - LastTradePrice) - (10 * sc.TickSize);

        order.OCOGroup1Quantity = 1; // If this is left at the default of 0, then it will be automatically set.

        SCString Buffer2;
        Buffer2.Format(" AveragePrice %f Target %fStop %f TotalTrades %d", PositionData.AveragePrice,
                       order.Target1Price, order.Stop1Price, PositionData.TotalTrades);
        sc.AddMessageToLog(PositionData.Symbol + Buffer2, 0);


        sc.SellEntry(order);
    }
}

SCSFExport scsf_SC_Breakouts(SCStudyInterfaceRef sc) {

    SCInputRef line1Ref = sc.Input[0];
    SCInputRef line2Ref = sc.Input[1];
    SCInputRef line3Ref = sc.Input[2];
    SCInputRef line4Ref = sc.Input[3];
    SCInputRef highLowCheckPrefInput = sc.Input[4];

    SCSubgraphRef bullish = sc.Subgraph[0];
    SCSubgraphRef bearish = sc.Subgraph[1];

    if (sc.SetDefaults) {

        sc.GraphName = "Breakouts";
        sc.StudyDescription = "Breakouts";

        sc.AutoLoop = 1;  // true
        sc.GraphRegion = 0;
        sc.FreeDLL = 1;
        sc.CalculationPrecedence = LOW_PREC_LEVEL;

        line1Ref.Name = "Daily Low";
        line1Ref.SetStudySubgraphValues(1, 2);

        line2Ref.Name = "Daily High";
        line2Ref.SetStudySubgraphValues(1, 1);

        line3Ref.Name = "Today's Low";
        line3Ref.SetStudySubgraphValues(4, 2);

        line4Ref.Name = "Today's High";
        line4Ref.SetStudySubgraphValues(4, 1);

        highLowCheckPrefInput.Name = "Entry Candles to take out";
        highLowCheckPrefInput.SetCustomInputStrings("Previous Candle;All todays Candles");
        highLowCheckPrefInput.SetCustomInputIndex(0);


        bullish.Name = "Bullish";
        bullish.DrawStyle = DRAWSTYLE_POINTHIGH;
        bullish.LineWidth = 3;

        bearish.Name = "Bearish";
        bearish.DrawStyle = DRAWSTYLE_POINTLOW;
        bearish.LineWidth = 3;

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

    // only process at the close of the bar; if it has not closed don't do anything
    if (sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_NOT_CLOSED) {
        return;
    }

    SCFloatArray dailyLows;
    sc.GetStudyArrayUsingID(line1Ref.GetStudyID(), line1Ref.GetSubgraphIndex(), dailyLows);

    SCFloatArray dailyHighs;
    sc.GetStudyArrayUsingID(line2Ref.GetStudyID(), line2Ref.GetSubgraphIndex(), dailyHighs);

    SCFloatArray lowOfTheDay;
    sc.GetStudyArrayUsingID(line3Ref.GetStudyID(), line3Ref.GetSubgraphIndex(), lowOfTheDay);

    SCFloatArray highOfTheDay;
    sc.GetStudyArrayUsingID(line4Ref.GetStudyID(), line4Ref.GetSubgraphIndex(), highOfTheDay);

    float CandleClose = sc.Close[sc.Index];
    float CandleOpen = sc.Open[sc.Index];
    float CandleLow = sc.Low[sc.Index];
    float PreviousCandleLow = sc.Low[sc.Index - 1];
    float CandleHigh = sc.High[sc.Index];
    float PreviousCandleHigh = sc.High[sc.Index - 1];
    float PreviousCandleClose = sc.Close[sc.Index - 1];

    float yesterdaysLow = dailyLows[sc.Index];
    float yesterdaysHigh = dailyHighs[sc.Index];

    float LastBarSize = PreviousCandleClose - PreviousCandleLow;

    s_SCPositionData PositionData;
    int Result = sc.GetTradePosition(PositionData);

    bool takeOutYesterdaysLow = CandleLow < yesterdaysLow;
    bool closePositive = CandleClose > CandleOpen;
    bool closeAboveYesterdaysLow = CandleClose > yesterdaysLow;
    bool openAboveYesterdaysLow = CandleOpen > yesterdaysLow;

    bool takeOutYesterdaysHigh = CandleHigh > yesterdaysHigh;
    bool closeNegative = CandleClose < CandleOpen;
    bool closeBelowYesterdaysHigh = CandleClose < yesterdaysHigh;
    bool openBelowYesterdaysLow = CandleOpen < yesterdaysHigh;

    float extraTicks = 10 * sc.TickSize;
    if (takeOutYesterdaysLow &&
        closePositive &&
        closeAboveYesterdaysLow &&
        openAboveYesterdaysLow &&
        getLowCheck(sc, lowOfTheDay, CandleLow, PreviousCandleLow, highLowCheckPrefInput.GetIndex()) &&
        sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {

        // mark the crossover on the chart
        bearish[sc.Index] = 1;

        // Create a market order and enter long.
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        order.Target1Price = CandleLow - extraTicks;
        order.Stop1Price = CandleClose + (CandleClose - CandleLow) + extraTicks;
        order.OCOGroup1Quantity = 1; // If this is left at the default of 0, then it will be automatically set.

        //logEntryMessage(sc, PositionData, order);
        sc.SellEntry(order);
    }
        //Buy the highs
    else if (takeOutYesterdaysHigh &&
             closeNegative &&
             closeBelowYesterdaysHigh &&
             openBelowYesterdaysLow &&
             getHighCheck(sc, highOfTheDay, CandleHigh, PreviousCandleHigh, highLowCheckPrefInput.GetIndex()) &&
             sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_CLOSED) {

        // mark the crossover on the chart
        bullish[sc.Index] = 1;

        // create a market order and enter short
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        order.Target1Price = CandleHigh + extraTicks;
        order.Stop1Price = CandleClose - (CandleHigh - CandleClose) - extraTicks;

        order.OCOGroup1Quantity = 1; // If this is left at the default of 0, then it will be automatically set.

        //logEntryMessage(sc, PositionData, order);
        sc.BuyEntry(order);
    }
}