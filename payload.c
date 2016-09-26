//Payload
#include "main.h"

//Get raw data from the sensor, store with time and attitude in SD card
void getRaw(int Attitude[], int Time) {
  // The code we already have to do 
  
}
//Gets raw data from SD card memory, process this and return in a form of your choosing to be transmitted. 
void process()
{
  
  //NEED TO ADD: Get memory from SD card: distanceM  matrix, theta matrix, time matrix
  //NEED TO ADD: Save the 'finalOutput' matrix to SD card
  
  
  
// read the required data, theta, distance, time, 
 // BLA BLA - add whatever here to read the data from sd card.1. thetaM = matrix of thetas of debris detection, 2. distanceM = matrix of distances of object detection. 3. timeM = matrix of time of object detection. n = matruix length 
// delete the follow 4 lines once the inputs have been red
int n = 3;
double distanceM[n];
distanceM[0] = 0.40001;
distanceM[1] = 0.20001;
distanceM[2] = 0.40001;
double theta[n];
theta[0] = 0*3.1415;
theta[1] = 0.5*3.1415;
theta[2] = 0.65*3.1415;
double timeM[n];
timeM[0]  = 121.1;
timeM[1] = 123.1;
timeM[2] = 125;
// dleete above

  
// Initialise all the matricies
double dt[n];
double xpos[n];
double ypos[n];
double dist[n];
double dir[2];
double vel[2];
double output[3];
double finalOutput[5];

 for(int i  = 0; i < n; i++){
  dt[i] = timeM[i] - timeM[0];
 }
  
   

for(int i = 0; i < n; i ++){
  xpos[i] = distanceM[i]*cos(theta[i]) -distanceM[0]*cos(theta[0]) ;
  ypos[i]  = distanceM[i]*sin(theta[i]) - distanceM[0]*sin(theta[0]);

  // ADD IN THE SQUARE ROOT OF THIS
  dist[i] =sqrt(sq(xpos[i]) + sq(ypos[i]));
  
}


//Find the coefficients of the distance least squares fit. 
  //Where: 
      // a = direction of travel (dy/dx)
      // b = y-intercept
LS(xpos, ypos, output, n);
dir[0] = output[0];
dir[1] = output[1];

// Need to find the reference point and time
double xref = output[2];
double yref= xref*dir[0] + dir[1];

// This is the reference distance point. (in the middle of the data)
double distref = sqrt(sq(xref) + sq(yref));



//Find the coefficients of the velocity least squares fit. 
  //Where: 
      // a = velocity (ddist/dt)
      // b = distance at 0 time
LS(dt, dist,output, n);
vel[0] = output[0];
vel[1] = output[1];




// Now the time at which this distref occurs must be found. As the dist is a linear line (as referenced from first detection points only 1 solution exists)
double timeref = (distref - vel[1])/vel[0];

// timeref is the time from first detection. to get it back into global time. This is just adding the constant that was removed at the start. Hence no errors are encured due to this
timeref = timeref  + timeM[0];

//Save everything that is needed to find the orbit of the piece of space debris

//Reference x point
finalOutput[0] = xref;

// Reference y point
finalOutput[1] = yref;

// Reference time
finalOutput[2] = timeref;

// Direciton of travel
finalOutput[3] = dir[0];

// Velocity of travel
finalOutput[4] = vel[0];

// ADD IN THE CODE TO SAVE TO MEMORY CARD HERE
//Serial.println("Start opf output");
//Serial.println(finalOutput[0], 5) ;

//Serial.println(finalOutput[1], 5) ;

//Serial.println(finalOutput[2], 5) ;

//Serial.println(finalOutput[3], 5) ;

//Serial.println(finalOutput[4], 5) ;
    
}








//Least Squares Function
//Inputs: 
  //xarray = array of all x points
  //yarray = array of all y points
  //outputs = output array that is modifed
  //n = length of input arrays
//Output: 
  //No returns, but 'output' is modified
void LS(double xarray[], double yarray[], double output[], int n){

//Declare everything
   double sumx = 0;
   double sumy = 0;
   double a = 0;
   double b = 0;
   double xdif[n];
   double ydif[n];
   double xdifS = 0;
   double xydif = 0;


   // Find the summation of each of the x and y arrays
  for(int i = 0; i < n; i++){
    sumx = sumx + xarray[i];
    sumy = sumy + yarray[i];
  }
  
  //Find the average of the x and y arrays
  double avgx = sumx/n;
  double avgy = sumy/n;

// For each point find the difference between the average and the actual value
  for(int i = 0; i < n; i++){
    xdif[i] = xarray[i] - avgx;
    ydif[i] = yarray[i] - avgy;
  }

  //For y = ax + b find the coefficients

  //Determine the b coefficient using the standard Least Squares formula
  for(int i = 0; i < n; i++){
        xdifS = xdifS  + sq(xdif[i]);
        xydif = xydif + xdif[i]*ydif[i];
  }

  a = xydif/xdifS;

  //Solving for equation of a line find the coefficient of b (y=intercept). 
 b = avgy - avgx*a;

output[0]  = a;
output[1] = b;
output[2] = avgx;


}


//Returns the amount of memory stored in SD card
int memAvail() {
  // FUCK ALL
  return 8;
}
