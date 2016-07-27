// TestBed for Writing Data from Study to File.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows header file
#include <windows.h>
#include "C:\SierraChart\SierraChartTransActMA\ACS_Source\sierrachart.h"
#include "C:\SierraChart\SierraChartTransActMA\ACS_Source\scstudyfunctions.h"

SCDLLName("Export Subgraph Data to File")

SCSFExport scsf_ExportStudyDataToFile(SCStudyInterfaceRef sc)
{
    SCInputRef Separator = sc.Input[0];
    SCInputRef UseGMTTime = sc.Input[1];
    SCInputRef DateFormat = sc.Input[2];
    SCInputRef ColumnName = sc.Input[3];
    SCInputRef DisplaySubgraphName = sc.Input[4];
    SCInputRef StudySubgraphReference = sc.Input[5];

    if (sc.SetDefaults)
    {
        // Set the configuration and defaults

        sc.GraphName = "Export Study Data To File";
        sc.StudyDescription="This study exports the selected study's values to a text file.  If the file name is not specified, the file is written to the '\data' directory where the program is installed with the name 'symbol-BarData.txt'.  If you specify a filename, it must be of the form c:[backslash]directory[backslash]another directory[backslash]filename.(csv or txt or some other extension).";

        sc.DrawZeros = false;
        sc.AutoLoop = 0;
        sc.GraphRegion = 0;

        Separator.Name = "Separator";
        Separator.SetCustomInputStrings("Space;Comma;Tab");
        Separator.SetCustomInputIndex(1);

        UseGMTTime.Name = "Use GMT Time";
        UseGMTTime.SetYesNo(0);

        DateFormat.Name = "Date Format";
        DateFormat.SetCustomInputStrings("MM/DD/YYYY;YYYY/MM/DD;DD/MM/YYYY");
        DateFormat.SetCustomInputIndex(0);

        //TO DO
        ColumnName.Name = "Enter the Column Name you want to use.";


        StudySubgraphReference.Name = "Study And Subgraph To Display";
        StudySubgraphReference.SetChartStudySubgraphValues(1,1, 0);

        sc.TextInputName = "'File Path + File Name'  If left blank, file will be written to Sierra Chart in the Data directory.";


        // During development set this flag to 1, so the DLL can be modified. When development is completed, set it to 0 to improve performance.
        sc.FreeDLL = 0;

        return;
    }

    SCString OutputPathAndFileName;
    SCFloatArray SubgraphArray;
    SCString SubgraphName;
    SCString ValueText;

    sc.GetStudyArrayFromChartUsingID(StudySubgraphReference.GetChartStudySubgraphValues(), SubgraphArray);

    //TO DO - Get the subgraph.name for the column header
    //SubgraphName = sc.Subgraph [SubgraphArray.GetArraySize()-1-ColumnsBack].Name ;

    if(sc.TextInput.GetLength() > 0)
    {
        OutputPathAndFileName = sc.TextInput;
    }
    else
    {
        OutputPathAndFileName.Format("%s-BarData.txt", sc.Symbol.GetChars());
    }

    // ***** Process the File

    int start = sc.UpdateStartIndex;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD numOfWrittenByte;
    if(start == 0)
    {
        // create new file
        hFile = CreateFile(OutputPathAndFileName.GetChars(),GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);

        // write file header

        // creating/opening fail
        if(hFile == INVALID_HANDLE_VALUE)
        {
            SCString message;
            message.Format("Write Bar Data To File study: Error creating output data file: %s", OutputPathAndFileName.GetChars());
            sc.AddMessageToLog(message.GetChars(), 1);
            return;
        }


        //Write the file's Column Headers
        SCString ColumnHdrText;
        //TO DO insert the study name for the header
        ColumnHdrText = "Study Value" ;
        SCString fileHeader;

        switch(Separator.GetInt())
        {
            case 0:
                //fileHeader = "Date Time Open High Low Close Volume NumberOfTrades BidVolume AskVolume ColumnHdrText\r\n";
                sc.FormatString( fileHeader, "Date Time Open High Low Close Volume NumberOfTrades BidVolume AskVolume %s\r\n", ColumnHdrText );
                break;
            case 1:
                //fileHeader = "Date, Time, Open, High, Low, Close, Volume, NumberOfTrades, BidVolume, AskVolume, ColumnHdrText\r\n";
                sc.FormatString( fileHeader, "Date, Time, Open, High, Low, Close, Volume, NumberOfTrades, BidVolume, AskVolume, %s\r\n", ColumnHdrText );
                break;
            case 2:
                //fileHeader = "Date\tTime\tOpen\tHigh\tLow\tClose\tVolume\tNumberOfTrades\tBidVolume\tAskVolume\ColumnHdrText\r\n";
                sc.FormatString( fileHeader,"\tDate\tTime\tOpen\tHigh\tLow\tClose\tVolume\tNumberOfTrades\tBidVolume\tAskVolume\t%st\r\n", ColumnHdrText );
                break;
        }

        WriteFile(hFile,fileHeader.GetChars(),(DWORD)fileHeader.GetLength(),&numOfWrittenByte,NULL);
    }
    else
    {
        // open existing file
        hFile = CreateFile(OutputPathAndFileName.GetChars() ,GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        // creating/opening fail
        if(hFile == INVALID_HANDLE_VALUE)
        {
            SCString message;
            message.Format("Write Bar Data To File study: Error creating output data file: %s", OutputPathAndFileName.GetChars());
            sc.AddMessageToLog(message.GetChars(), 1);
            return;
        }

        SetFilePointer(hFile, 0, NULL, FILE_END);
    }

    //Process the data that is already on the chart.  After the first call where start is = 0,
    //iterate through the bars that are already on the chart.  After the initial iteration, start will be > 0.
    //Therefore, only the newest bar on the chart will be added to the end of the file.
    //The file closes when Sierra Chart closes or the study is removed from the chart.

    for (int Index=start;Index<(sc.ArraySize-1);Index++)
    {
        if (sc.GetBarHasClosedStatus(Index)!=BHCS_BAR_HAS_CLOSED)
            continue;			// write on close

        int year,month,day;
        int hour, minute, second;
        SCDateTime BaseDateTimeIn = sc.BaseDateTimeIn[Index];
        if (UseGMTTime.GetBoolean() != 0)
            BaseDateTimeIn = sc.AdjustDateTimeToGMT(BaseDateTimeIn);

        BaseDateTimeIn.GetDateTimeYMDHMS(year, month, day, hour, minute, second);

        SCString DateString;
        switch (DateFormat.GetInt())
        {			case 1:

            case 0:
                DateString.Format("%02d/%02d/%d", month, day, year);
                break;

                DateString.Format("%d/%02d/%02d", year, month, day);
                break;

            case 2:
                DateString.Format("%02d/%02d/%d", day, month, year);
                break;
        }

        // write bar data string to file using the current format.
        int ValueFormat = sc.BaseGraphValueFormat;

        char *formatString = "";
        switch(Separator.GetInt())
        {
            case 0:
                formatString = "%s %.2d:%.2d:%.2d %s %s %s %s %.0f %.0f %.0f %.0f %s\r\n";
                break;
            case 1:
                formatString = "%s, %.2d:%.2d:%.2d, %s, %s, %s, %s, %.0f, %.0f, %.0f, %.0f, %s\r\n";
                break;
            case 2:
                formatString = "%s\t%.2d:%.2d:%.2d\t%s\t%s\t%s\t%s\t%.0f\t%.0f\t%.0f\t%.0f\t%s\r\n";
                break;
        }


        //Load the StudyReferences - subgraph value
        ValueText = sc.FormatGraphValue(SubgraphArray[Index], sc.GetValueFormat());

        SCString BarDataString;

        BarDataString.Format(formatString,
                             DateString.GetChars(),
                             hour,
                             minute,
                             second,
                             sc.FormatGraphValue(sc.Open[Index],ValueFormat).GetChars(),
                             sc.FormatGraphValue(sc.High[Index],ValueFormat).GetChars(),
                             sc.FormatGraphValue(sc.Low[Index],ValueFormat).GetChars(),
                             sc.FormatGraphValue(sc.Close[Index],ValueFormat).GetChars(),
                             sc.Volume[Index],
                             sc.NumberOfTrades[Index],
                             sc.BidVolume[Index],

                             sc.AskVolume[Index],
                             ValueText.GetChars ());

        WriteFile(hFile,BarDataString.GetChars(),(DWORD)BarDataString.GetLength(),&numOfWrittenByte,NULL);

    }
    CloseHandle(hFile);

    return;
}
