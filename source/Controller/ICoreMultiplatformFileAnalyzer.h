/**
 * File: ICoreMultiplatformFileAnalyzer.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/14
*/

#ifndef ICOREMULTIPLATFORMFILEANALYZER_H
#define ICOREMULTIPLATFORMFILEANALYZER_H

class ResultData;
class InputData;

/**
 * @brief The ICoreMultiplatformFileAnalyzer interface that represent the analysis core
 */
class ICoreMultiplatformFileAnalyzer
{
public:
    /**
     * @brief ~ICoreMultiplatformFileAnalyzer virtual destructor
     */
    virtual ~ICoreMultiplatformFileAnalyzer();

    /**
     * @brief execute: start the analysis by control the input data and then call the real analysis function
     * @param resultData: where store the result of the analysis
     * @param inputData: input data of the analysisi
     */
    virtual void Execute(ResultData *o_pResultData, InputData* pInputData) = 0;
};

#endif // ICOREMULTIPLATFORMFILEANALYZER_H
