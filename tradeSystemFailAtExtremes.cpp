#include "sierrachart.h"

SCDLLName("TradingSystemFailAtExtremes")

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


SCSFExport scsf_SC_TradingCrossOverExample(SCStudyInterfaceRef sc)
{

    SCInputRef line1Ref = sc.Input[0];
    SCInputRef line2Ref = sc.Input[1];

    SCSubgraphRef bullish = sc.Subgraph[0];
    SCSubgraphRef bearish = sc.Subgraph[1];

    if (sc.SetDefaults)
    {

        // Set the configuration and defaults

        sc.GraphName = "Trading CrossOver Example";

        sc.StudyDescription = "An example of a trading system that enters a new position or \
                               reverses an existing one on the crossover of two study Subgraphs. \
                               When line1 crosses line2 from below, the system will look to be long. \
                               When line1 crosses line2 from above, the system will look to be short.";

        sc.AutoLoop = 1;  // true
        sc.GraphRegion = 0;
        sc.FreeDLL = 0;
        sc.CalculationPrecedence = LOW_PREC_LEVEL;

        line1Ref.Name = "Line1";
        line1Ref.SetStudySubgraphValues(1, 0);

        line2Ref.Name = "Line2";
        line2Ref.SetStudySubgraphValues(1, 0);

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

        sc.CancelAllOrdersOnEntriesAndReversals= true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = true;
        sc.AllowOnlyOneTradePerBar = true;

        sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }

    // only process at the close of the bar; if it has not closed don't do anything
    if (sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_NOT_CLOSED)
    {
        return;
    }


    // using the line1Ref and line2Ref input variables, retrieve the subgraph arrays into line1,
    // line2 arrays respectively
    SCFloatArray line1;
    sc.GetStudyArrayUsingID(line1Ref.GetStudyID(), line1Ref.GetSubgraphIndex(), line1);

    SCFloatArray line2;
    sc.GetStudyArrayUsingID(line2Ref.GetStudyID(), line2Ref.GetSubgraphIndex(), line2);

    // code below is where we check for crossovers and take action accordingly

    if (sc.CrossOver(line1, line2) == CROSS_FROM_BOTTOM)
    {
        // mark the crossover on the chart
        bullish[sc.Index] = 1;

        // Create a market order and enter long.
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        sc.BuyEntry(order);
    }

    if (sc.CrossOver(line1, line2) == CROSS_FROM_TOP)
    {
        // mark the crossover on the chart
        bearish[sc.Index] = 1;

        // create a market order and enter short
        s_SCNewOrder order;
        order.OrderQuantity = 1;
        order.OrderType = SCT_ORDERTYPE_MARKET;

        sc.SellEntry(order);
    }
}