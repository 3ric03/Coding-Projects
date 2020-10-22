#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

/*
ERIC CHEN
ICS3U1 - 02
*/

//Function prototypes
double** priceRead(string);
char** seatRead(string, int&, int&, int&);
void seatUpdater(char**, string);
void menu(double**, char**, int&, int&, int&);
void deleter(double**, char**);

//Global declaration of file imput and output
ifstream fin;
ofstream fout;

int main()
{
   //Local var declarations
   int avaliableSeats = 0;
   int soldSeats = 0;
   int revenue = 0;

   //Function call (int variables are passed by reference, values are chaged in all scopes)
   double** priceMap = priceRead("prices.txt");
   char** seatMap = seatRead ("seats.txt", avaliableSeats, soldSeats, revenue);
   menu(priceMap, seatMap, avaliableSeats, soldSeats, revenue);
   deleter(priceMap, seatMap);

   cout << "\nThank you for using!\n";

}

//Reads from prices.txt, input price data into a dynamic 2D array
double** priceRead(string txt)
{
  //open file
  fin.open(txt);

  //create a ouble pointer, size of 15
  double** priceMap = new double* [15];

  //loop through 15 rows
  for (int i = 0; i < 15; i++)
  {

    //even row, 20 prices
    if (i % 2 == 0)
    {
      //create array (size is one larger than number of prices, read the comment below)
      priceMap[i] = new double [21];
      //set row size as first element, this makes the array easier to access later
      priceMap[i][0] = 20;

      //read and input the 20 prices into columns
      for (int j = 0; j < 20; j++)
          //as element 0 is row size, prices needs to be stored to the (j + 1)th index
          fin >> priceMap [i][j+1];
    }

    //odd row, 15 prices
    else
    {
      //same procedure as above, 15 prices this time
      priceMap[i] = new double [16];
      priceMap[i][0] = 15;
      for (int j = 0; j < 15; j++)
          fin >> priceMap [i][j+1];
    }

  }// end main for loop

  //close file, return the 2D array
  fin.close();
  return priceMap;

}// end priceRead

//reads from seat.txt, input seat data into 2D array
//Determine number of avaliable and sold seats, as well as total revenue
char** seatRead(string txt,  int& avaliableSeats, int& soldSeats, int& revenue)
{
  //open file
  fin.open(txt);
  //create double pointer
  char** seatMap = new char* [15];

  //read data into arrays, identical procedure as priceRead function
  for (int i = 0; i < 15; i++)
  {
    if (i % 2 == 0)
    {
      seatMap[i] = new char [21];
      seatMap[i][0] = 20;
      for (int j = 0; j < 20; j++)
          fin >> seatMap [i][j+1];
    }

    else
    {
      seatMap[i] = new char [16];
      seatMap[i][0] = 15;
      for (int j = 0; j < 15; j++)
          fin >> seatMap [i][j+1];
    }
  }// end main for loop

  //close file
  fin.close();

  //Determine number of avaliable and sold seats, as well as total revenue
  //loop through 15 rows
    for (int i = 0; i < 15; i++)
      {
        //Loop through columns
        //loop condition is based on the size of the row, which is stored in element 0 as shown above
        for (int j = 0; j < seatMap[i][0]; j++)
        {
          //if empty, increment avaliableseats variable
          if (seatMap [i][j+1] == '#')
            {
              avaliableSeats++;
            }

            //if taken, increment soldSeats, increase revenue based on seat location
            else if (seatMap [i][j+1] == 'X' && i < 10)
            {
              soldSeats++;
              revenue += 15;
            }

            else
            {
              soldSeats++;
              revenue += 8;
            }

          }//end inner for loop

      }//end main for loop

  // return 2D array
  return seatMap;

}//end seatRead

//After a purchase is made, output new seating chart into seats.txt
void seatUpdater(char** seatMap, string txt)
{
    //open file
    fout.open(txt);

    //loop through 15 rows
    for (int i = 0; i < 15; i++)
    {
        //loop conditin based on element 0 (size of row)
        for (int j = 0; j < seatMap[i][0]; j++)
            //write into txt file
            fout << seatMap [i][j+1] << " ";
        fout << endl;
    }

    //close file
    fout.close();

}//end seatUpdater


//Main function, displays seating charts, prompts purchase, input validation
void menu(double** priceMap, char** seatMap, int& avaliableSeats, int& soldSeats, int& revenue)
{
  //local var declaration
  int column, row;
  bool running = true;
  bool invalid = true;
  char command;

  //loop
  while (running)
  {
  invalid = true;

  //display seating chart, use appropiate formatting
  cout << "SEATING CHART" << endl << endl;
  cout << "    ";
  for (int i = 0; i < 20; i++)
    cout << left << setw(3) << i << " ";

  cout << endl << endl;

  for (int i = 0; i < 15; i++)
  {
      cout << left << setw(3) << i << " ";
      //element 0 stores size of row
      for (int j = 0; j < seatMap[i][0]; j++)
          cout << left << setw(3) << seatMap [i][j+1] << " ";
      cout << endl << endl;
  }// end main for loop

  //prompt seat input
  cout << "\nPurchase Seats\n";
  cout << "Enter row and column number, seperated by a space: ";
  cin >> row >> column;

  //check validity
  while (invalid)
      {

      //check if seat exists
      if ((row < 0 || column < 0) || (row % 2 == 0 && column > 19) || (row % 2 == 1 && column > 14))
        cout << "Invalid input, please try again: ";
      //check if seat is taken

      else if (seatMap[row][column + 1] == 'X')
        cout << "Seat is taken, please choose a different one: ";

      //if valid, break out of loop
      else
        break;

      //allow the user to try again
      cin >> row >> column;

      }

    //purchase is valid if this line is reached
    //Update the seatMap array to account for new purchase
    seatMap[row][column + 1] = 'X';

    //Update sales information
    soldSeats++;
    avaliableSeats--;

    //Increase revenue based on seat location
    revenue += priceMap[row][column + 1];

    //Display information
    cout << "Purchase confirmed: row " << row << " column " << column << endl;
    cout << "Ticket price: $" << priceMap[row][column + 1] << endl;

    cout << endl << "Seats remaining: " << avaliableSeats << endl;
    cout << "Seats sold: " << soldSeats << endl;
    cout << "Total Revenue: $" << revenue;

    //Call function to update seats.txt. Pass in seatMap array, which contains the newest seating data
    seatUpdater(seatMap, "seats.txt");

    //Ask if another purchase is wanted
    cout << "\n\nEnter 'y' to purchase another seat, a different key to quit: ";
    cin >> command;

    //continue loop is y is entered
    if (tolower(command) == 'y')
      continue;

    //if not, exit loop
    running = false;

  }//end main while loop

}//end menu function

//delete allocated data
void deleter(double** priceMap, char** seatMap)
{
  for (int i = 0; i < 15; i++)
    delete [] priceMap[i];
  delete [] priceMap;

  for (int i = 0; i < 15; i++)
    delete [] seatMap[i];
  delete [] seatMap;
}// end deleter
