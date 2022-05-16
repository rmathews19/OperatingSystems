/*************************************************************************************************************
 * File: main.cpp
 * Author: Reevan Mathews
 * Modified by: Reevan Mathews - April 20 2022
 * 
 * Procedures:
 * time                         - represents a random time using uniform distribution
 * size                         - represents a random size using poisson distribution
 * calculateTheAverages         - the average TATs and RTATs of each partition style
 * equalPartitionQ              - memory
 * resultOutput                 - prints time heuristics
 * main                         - determines which style is efficient
 **************************************************************************************************************/

#include <iostream>
#include <random>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <chrono>
using namespace std;
#include <math.h>  

struct Data {       //Resources
  public:             
    int time;        
    int size;  
};
struct Results { 
    float TurnAroundTime; //waiting+service
    float RTurnAroundTime; //TAT/Service
    int numberOfFailures; //if process fails   
};
struct partitionProcess {  
    int IndProcess;
    Data process;
};

/************************************************************************************************
 * int time(int lowerBound, int higherBound)
 * Author: Reevan Mathews
 * Date: Reevan Mathews - April 22 2022
 * Description: Uses uniform distribution to generate a random number that simulates time
 *              
 * Parameters:
 *      lowerBound    I/P     int     minimum value
 *      higherBound   I/P     int     maximum value
 *************************************************************************************************/

int time(int lowerBound, int higherBound) { //uniform distribution
    random_device rd;
    mt19937 ranGen(rd());
    uniform_int_distribution<> uniform(lowerBound, higherBound);
    return uniform(ranGen);}

/************************************************************************************************
 * int size(int mAverage)
 * Author: Reevan Mathews
 * Date: Reevan Mathews - April 22 2022
 * Description: Uses poisson distribution to generate a random number that simulates size
 *              
 * Parameters:
 *      mean    I/P     int     Mean of the distribution
 *************************************************************************************************/

int size(int mAverage) {   //poisson distribution
    random_device rd;
    mt19937 ranGen(rd());
    poisson_distribution<> poisson(mAverage);
    return max(1, (int) ceil(poisson(ranGen))); }

/****************************************************************************************************
 * Results equalPartition(Data currentDataProcess[], int numberOfSamples, Results rArrays[])
 * Author: Reevan Mathews
 * Date: Reevan Mathews - April 22 2022
 * Description: Equal Partition Algorithm
 *              
 * Parameters:
 *      Data currentDataProcess        I/P     data[]      Data Structure
 *      rArrays                        O/P     results[]   Holds results
 *      numberOfSamples                I/P     int         Total Samples
 ****************************************************************************************************/

Results equalPartitionQ(Data currentDataProcess[], int numberOfSamples, Results rArrays[]) {
    int sizeBlock[]={8, 8, 8, 8, 8, 8, 8};               
    int processNext=0;  
    int processEnded=0;
    int blockCurrent=0; 
    float clockCounter=0;
    partitionProcess partitions[7]={ {{0, 0}, -1}};           
    while(processEnded<numberOfSamples) {    
        if(blockCurrent==7) {
            blockCurrent=0;             }
        if(partitions[blockCurrent].process.time==0) {                                            
            if(partitions[blockCurrent].process.size!=0) {                                        
                int currentProcessEnded = partitions[blockCurrent].IndProcess;                               
                rArrays[currentProcessEnded].TurnAroundTime=clockCounter;                                             
                rArrays[currentProcessEnded].RTurnAroundTime+=clockCounter/currentDataProcess[currentProcessEnded].time; 
                ++processEnded;}
            if(processNext<numberOfSamples) {                                         //adds a process if there's a process left
                if (currentDataProcess[processNext].size>sizeBlock[blockCurrent]) {      //if its too much space, it fails
                    rArrays[processNext].numberOfFailures=1;                             }
                partitions[blockCurrent].process=currentDataProcess[processNext];         //this puts the next process in line
                partitions[blockCurrent].IndProcess=processNext;                       //stores index
                ++processNext;}
            else if (processNext>=numberOfSamples) {                                  //processes done
                partitions[blockCurrent]={{0, 0}, -1};                           }
            ++blockCurrent;                                    }
            //next partition
        else if (partitions[blockCurrent].process.time>0) {  
            --partitions[blockCurrent].process.time;           
            ++clockCounter;                                        //clockCounter increments
            ++blockCurrent;}}
            //returns averages
    return calculateTheAverages(numberOfSamples, rArrays);                  }

/***************************************************************************************************
 * Results calculateTheAverages(int elements, Results rArrays[])
 * Author: Reevan Mathews
 * Date: Reevan Mathews - April 22 2022
 * Description: Calculates TAT, RTAT and fails
 *              
 * Parameters:
 *      rArrays             I/P     results[]   An array containing all of the results from an experiment
 *      elements    I/P     int         The number of results generated by the experiment
 ***************************************************************************************************/

Results calculateTheAverages(int elements, Results rArrays[]) {
    Results average={0, 0, 0};   //holds the average values
    int failure=0;
    float TurnAroundTime=0;
    float RTurnAroundTime=0;
    for(int i=0; i < elements; i++) {  //summation
        failure+=rArrays[i].numberOfFailures;
        TurnAroundTime+=rArrays[i].TurnAroundTime;
        RTurnAroundTime+=rArrays[i].RTurnAroundTime;}
    //averages
    average.numberOfFailures=failure;    //total summation
    average.TurnAroundTime=TurnAroundTime/elements;
    average.RTurnAroundTime=RTurnAroundTime/elements;
    return average;}

/****************************************************************************************************
 * void resultOutput (int numExp, results resArr[])
 * Author: Reevan Mathews
 * Date: Reevan Mathews - April 22 2022
 * Description: Outputs results
 * Parameters:
 *      resArr              I/P     results[]   Holds all results
 *      numberExp           I/P     int         Total Experiments
 ****************************************************************************************************/

void resultOutput(int numberExp, Results rArr[]) {
    float avgFail=0;
    float avgTurnAroundTime=0;
    float avgRTurnAroundTime=0;
    for(int i=0; i<numberExp; i++) {   //summation
        avgFail+=rArr[i].numberOfFailures;
        avgTurnAroundTime+=rArr[i].TurnAroundTime;
        avgRTurnAroundTime+=rArr[i].RTurnAroundTime;}
    //average
    avgFail=avgFail/numberExp;
    avgTurnAroundTime=avgTurnAroundTime/numberExp;
    avgRTurnAroundTime=avgRTurnAroundTime/numberExp;
    // Prints equal results
    cout << "Final AVG EQUAL Fail:\t" << avgFail << endl;
    cout << "Final AVG EQUAL TurnAroundTime:\t\t\t" << avgTurnAroundTime << endl;
    cout << "Final AVG EQUAL RTurnAroundTime:\t\t" << avgRTurnAroundTime << endl;
    // Prints unequal single queue results
 // cout << "Final AVG UNEQUAL ONE QUEUE TurnAroundTime:\t\t\t" <<  << endl;
 // cout << "Final AVG UNEQUAL ONE QUEUE RTurnAroundTime:\t\t" <<  << endl;
 // cout << "Final AVG UNEQUAL ONE QUEUE FAIL:\t" <<  << endl;
    // Prints Multi Queue results
 // cout << "Final AVG UNEQUAL MULTI QUEUE TurnAroundTime:\t\t\t" <<  << endl;
 // cout << "Final AVG UNEQUAL MULTI QUEUE RTurnAroundTime:\t\t" <<  << endl;
 // cout << "Final AVG UNEQUAL MULTI QUEUE FAIL:\t\t\t" <<  << endl;
    cout << "1000 exp x 1000 samples" << endl;}

/****************************************************************************************************
 * int main()
 * Author: Reevan Mathews
 * Modified by: Reevan Mathews - April 20 2022
 *              Reevan Mathews - April 22 2022
 * Description: Runs 1000 experiments with 1000 samples which is simulating 
 * 1,000,000 processes for each partitioning algorithm. 
 * 
 * Parameters:
 ****************************************************************************************************/

int main() {
    int numberOfSamples=1000; //samples
    int numberOfExperiments=1000; //experiments
    Results currentExperimentResults[numberOfSamples]; 
    Data currentExperimentData[numberOfSamples];           //processes
    Results equalExperiment[numberOfExperiments];       
    for (int currentExperiment=0; 
    currentExperiment<numberOfExperiments; currentExperiment++) {
        for (int currentSample=0; numberOfSamples; currentSample++) 
        {
            currentExperimentData[currentSample]={size(8), time(1, 10)};      //the current process
            currentExperimentResults[currentSample]={0, 0, 0};                                     }
            equalExperiment[currentExperiment]=equalPartitionQ(currentExperimentData, numberOfSamples, currentExperimentResults);    //equal partition results
}
    resultOutput(numberOfExperiments, equalExperiment);
    return 0; }