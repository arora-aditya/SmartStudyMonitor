#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "functionLogging.h"
using namespace std;

// void logger(char errorTag[], char functionName[], char message[], int errorCode);
float processSensor(int output[CHAR_BIT]){
  /*
    pre-process sensor data
  */
  logger("DEBUG","processSensor", "entered pre-process sensor data");
  float val = 0.4 * output[0] + 0.15 * (output[1]+output[2]+output[3]+output[4]);
  if(val > 0.8){
    return 1;
  }
  logger("DEBUG","processSensor", "exited pre-process sensor data");
  return 0;
}

float processHalfSensor(int output[2]){
  /*
  process data for left and right halves seperately
  */
  logger("DEBUG","processHalfSensor", "entered process data for left and right halves seperately");
  int sum = output[0] + output[1];
  if(sum >= 1){
    return sum;
  }
  logger("DEBUG","processHalfSensor", "exited process data for left and right halves seperately");
  return 0;
}

float summation(int buffer[]){
  /*
  sum over buffer
  */
  logger("DEBUG","summation", "entered sum over buffer");
  float sum = 0;
  for(int i = 0; i < 10; i++){
    sum += buffer[i];
  }
  logger("DEBUG","summation", "exited sum over buffer");
  return sum;
}

char getSensorData(){
  char j = 0;
  for(int i = 0; i < 5; i++){
    j *= 2;
    j += rand()%2;
  }
  return j;
}

void sensorLogger(){
  /*
  infinite loop of sensor reading and writing to file after processing
  */
  logger("DEBUG", "sensorLogger", "entered infinite loop of sensor reading and writing to file after processing");
  ofstream ofs;
  int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int leftBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int rightBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
  int j = 0;
  ofs.open ("movement.csv", ofstream::out | ofstream::app);
  if (!ofs.is_open()) {
    logger("FATAL", "OpenFile", "Unable to open file movement.csv", 3);
    return;
  }
  ofs<<"date,movement,left,right,fsr\n";
  while(true){
    char val = getSensorData();
    int output[5] = {0,0,0,0,0} ;
    /*
      output[5] = {FSR, sensor1, sensor2, sensor3, sensor4}
      value     = {sensor4, sensor3, sensor2, sensor1, FSR}
    */
    for (int i = 0; i < 5; ++i) {
      output[i] = (val >> i) & 1;
      cout<<output[i];
    }
    cout<<'\n';
    int leftOutput[2] = {output[1],output[2]};
    int rightOutput[2] = {output[3],output[4]};
    leftBuffer[j] = processHalfSensor(leftOutput);
    rightBuffer[j] = processHalfSensor(rightOutput);
    buffer[j] = processSensor(output);
    j++;
    if(j > 9){
      j = 0;
      float sumTotal = summation(buffer);
      float sumLeft = summation(leftBuffer);
      float sumRight = summation(rightBuffer);
      ofs<<currentDateTime()<<","<<sumTotal<<","<<sumLeft<<","<<sumRight<<","<<output[0]<<"\n";
      ofs.flush();
    }
  }
  logger("DEBUG", "sensorLogger", "closed all files and exited");
  ofs.close();
}

int main()
{
	sensorLogger();

	return 0;
}
