// main.cpp - a very simple seating chart gnerator for a fictitious airline
//

#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>


//function headers, for more information about each function, see below
void create_chart(std::vector<std::vector<std::string> > &chart,std::ifstream &seats);
void prt(std::vector<std::vector<std::string> > &chart,std::string flight);
bool is_best_seat(std::vector<std::vector<std::string> > &chart,int row,int column);
bool srn(std::vector<std::vector<std::string> > &chart,std::string id,std::string fbc,std::string freq,std::vector<std::string> &upList);
bool ssr(std::vector<std::vector<std::string> > &chart,std::string pid,std::string sid,std::string freq,std::vector<std::string> &upList);
bool seat_row_col(int &row, int &col,std::vector<std::vector<std::string> > &chart,std::string seat);
bool cxl(std::vector<std::vector<std::string> > &chart,std::vector<std::string> &upList,std::string id);
void upl(std::vector<std::string> &upList);
void upp(std::vector<std::vector<std::string> > &chart,std::vector<std::string> &upList);
bool find_seat(std::string id, std::vector<std::vector<std::string> > &chart,std::string &column,int &row);
void del(std::vector<std::string> &upList,std::string thing);


// a constant for genrating seat aisle labels
const std::string ALPHABET("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

int main(int argc, char* argv[]) {

	//various variables, like the flight number, the upgrade list and the chart
	std::string flight;
	std::vector<std::string> upgrade_list;
	std::vector<std::vector<std::string> > chart;
	std::ifstream commandfile(argv[2]);
	std::ifstream seats(argv[1]);
	if(!commandfile.good()||!seats.good())
	{
		std::cerr<<"One or more file not opened correctly" <<std::endl;
		exit(1);
	}

	//variables to temporarily store the information from the file
	std::string id,sid,fbc,freq,thing;

	create_chart(chart,seats);



	//goes through the commandfile, on string at a time, and uses the functions created below to manage the information
	commandfile >> flight;
	while(commandfile>>thing)
	{
		if(thing == "SRN")
		{
			commandfile >> id >>fbc >> freq;
			srn(chart,id,fbc,freq,upgrade_list);
		}
		else if(thing == "SSR" )
		{
			commandfile >>id>>sid>>freq;
			ssr(chart,id,sid,freq,upgrade_list);
		}
		else if(thing == "CXL")
		{
			commandfile >> id;
			cxl(chart,upgrade_list,id);
		}
		else if(thing == "UPL")
		{
			upl(upgrade_list);
		}
		else if(thing =="UPP")
		{
			upp(chart,upgrade_list);
		}
		else if(thing =="PRT")
		{
			prt(chart,flight);
		}
		else
		{
			std::cout <<"Invalid command: "<<thing<<"."<<std::endl;
		}
	}

  return 0;
}

//a function to create the inital empty chart from the input file
//reads in the chart and the file that gives the seating chart
void create_chart(std::vector<std::vector<std::string> > &chart,std::ifstream &seats)
{
	//temporary variable to store information. one to store the current string input from the file, the other an empty vector
	//to push back on to the vector of vectors
	std::string current;
	std::vector<std::string> temp;

	//while the end of the file has not been reached
	while(!seats.eof())
	{
		//if something can be streamed into current
		if(seats>>current)
		{
			//if what was just input is a letter, create a new row. this is because letters label each row of seats as first class, business, or coach
			//and the letters precede the diagram of the seats in each row
			if(ALPHABET.find(current)!=std::string::npos)
			{
				//push back the empty vector onto the vector of vectors
				chart.push_back(temp);
				chart[chart.size()-1].push_back(current);
			}
			else
			{
				//if it isnt a letter, just push back the the current string onto the vector of strings. this will create the columns in a row.
				chart[chart.size()-1].push_back(current+current+current+current);
			}
		}
	}
}

//function to print out the current seating chart the correct way
void prt(std::vector<std::vector<std::string> > &chart,std::string flight)
{
	std::cout<<"Seating Chart for Flight "<<flight<<std::endl;
	//prints the spaces before the letter A
	std::cout<<"\t";

	//For however many column there are, print out that many letters on top
	for(int i = 0;i<chart[0].size()-1;i++)
	{
		std::string letter(1,ALPHABET[i]);
		std::cout<<std::left<<letter+"    ";
	}
	//print out the word seating class at the end of the first row
	std::cout<<"Seating Class"<<std::endl;

	//create the rows of the seating chart
	for(unsigned int i = 0;i<chart.size();i++)
	{
		//print out the number of each row first
		std::cout<<i+1<<"\t";
		//Then print out what is in the row. printing what is stored at the [i][0] portion last because that stores the seating class
		for(unsigned int x=1;x<chart[i].size();x++)
		{
			std::cout<<chart[i][x]+" ";
		}
		std::cout<<chart[i][0]<<std::endl;
	}
}

//the srn function. finds a passenger a place to sit based on the class they chose
//takes in the chart, an passenger id, the fbc string which determines the class, the freq string for frequent flyer y or n
//and the list for upgrades so that it may be added to
bool srn(std::vector<std::vector<std::string> > &chart,std::string id,std::string fbc,std::string freq,std::vector<std::string> &upList)
{
	if(fbc!="F" && fbc!="B" && fbc!="C")
	{
		std::cout << "invalid seating class - "<<fbc<<std::endl;
		return false;
	}

	std::string column;
	int row;
	//this loop goes through the rows with the right class and sees if one of the good seats is available, if so, it assigns it

	//go through rows
	for(unsigned int i = 0;i<chart.size();i++)
	{
		//check if the row is the right class
		if(chart[i][0]==fbc)
		{
			//go through each element in the selected row
			for (unsigned int x= 1;x<chart[i].size();x++)
			{
				//if the seat is empty, and is one of the best seats, as in window or isle seat
				if(chart[i][x]=="...." && is_best_seat(chart,i,x))
				{
					chart[i][x]=id;
					find_seat(id,chart,column,row);
					std::cout << "Passenger " << id << " was given seat " << row << column <<"." <<std::endl;
					return true;
				}
			}//end x for
		}//end if
	}//end i for

	//goes through and sees if there are any empty seats that are not window or isle seats and assigns the passenger to the first one found

	//goes through rows
	for(unsigned int i = 0;i<chart.size();i++)
		{
			//checks if the row is the right class
			if(chart[i][0]==fbc)
			{
				//goes through the elements in the selected row
				for (unsigned int x= 1;x<chart[i].size();x++)
				{
					//if the seat is empty assign it
					if(chart[i][x]=="....")
					{
						chart[i][x]=id;
						find_seat(id,chart,column,row);
						std::cout << "Passenger " << id << "was given seat " << row << column <<"." <<std::endl;
						return true;
					}
				}//end x for
			}//end if
		}//end i for

	//if the seat was not found and the passenger is a frequent flyer, add him to the list, and print the appropriate message

	if(freq=="Y")
	{
		upList.push_back(id+fbc);
	}
	else
	{
		std:: cout << "Could not find a seat in the " << fbc <<" section for passenger "<<id<<std::endl;
	}
	return false;
}

//assigns a passenger a seat by request. if that doesnt work, assign them to a seat in the same class
//takes the chart, a passenger id, a seat id, a string for frequent flyer status, and the upgrade list
bool ssr(std::vector<std::vector<std::string> > &chart,std::string pid,std::string sid,std::string freq,std::vector<std::string> &upList)
{
	//variables to be passed in by reference to seat_row_col
	int row,col;

	//if the seat exists...
	if(seat_row_col(row,col,chart,sid))
	{
		//check if its empty, if it is, assign it
		if(chart[row][col]=="....")
		{
			chart[row][col]=pid;
			std::cout << "Passenger "<<pid<<" was given seat "<<sid<<"."<<std::endl;
			return true;
		}
		else
		{
			std::cout <<"Passenger "<<pid<<" requested an occupied seat "<<sid<<"."<<std::endl;
		}
	}
	else
	{
		std::cout << "Passenger "<<pid <<" requested an invalid seat "<<sid<<"."<<std::endl;
	}
	//if either the seat doesnt exist or is full, assign the passenger a seat in the same class if possible
	if(row<chart.size())
	{
		return srn(chart,pid,chart[row][0],freq,upList);
	}
	else
	{
		return false;
	}
}

//simple boolean helper function to check if a seat is a window or isle seat
bool is_best_seat(std::vector<std::vector<std::string> > &chart,int row,int column)
{
	//if it is the first or last seat in a row it is a window seat, so return true
	if(column == 1||column == chart[0].size()-1)
	{
		return true;
	}
	else
	{
		//if it is adjacent to !!!! return true, otherwise, false
		return (chart[row][column+1]=="!!!!"||chart[row][column-1]=="!!!!");
	}
}

//changes a seat id like 30C into the proper form of two integers
//takes in a row and column passed by reference so that they may be saved, the chart, and the seat id
bool seat_row_col(int &row, int &col,std::vector<std::vector<std::string> > &chart,std::string seat)
{
	//temporary string variables for the string versions of the row and column
	std::string srow;
	std::string scol;
	//go through the seat string and add anything that is a didgit to the row, and anything that isnt to the column
	for(unsigned int i =0;i<seat.size();i++)
	{
		if(isdigit(seat[i]))
		{
			srow += seat[i];
		}
		else
		{
			scol +=seat[i];
		}
	}
	//make the row to a number
	row = atoi(srow.c_str())-1;

	//if the letter for the column is not found, return false
	if (col==std::string::npos)
	{
		return false;
	}


	//do the same with the column
	col = ALPHABET.find(scol)+1;

	//if the column is out of bounds, return false
	if(col > chart[1].size()-1)
	{
		return false;
	}

	//if the row is not on the chart, return false
	if(row <0||row > chart.size()-1)
	{
		return false;
	}
	return true;
}

//function to cancel a passenger
//takes the char, the upgrade list, and the id of the passenger
bool cxl(std::vector<std::vector<std::string> > &chart,std::vector<std::string> &upList,std::string id)
{
	//boolean to store wether or not the passengers id was found in the seats or the upgrade list
	//loop to go through the rows and columns to find the passenger and remove their id
	for(unsigned int i = 0;i<chart.size();i++)
	{
		for(unsigned int x=1;x<chart[0].size();x++)
		{
			if(chart[i][x]==id)
			{
				chart[i][x] ="....";
				return true;
			}
		}
	}
	//loop to go through the upgrade list and remove the id if found
	for(unsigned int i = 0;i<upList.size();i++)
	{
		if(upList[i].substr(0,4)==id)
		{
			del(upList,upList[i]);
			return true;
		}
	}
	//returns true if the passenger is found, false otherwise
	std::cout << "Passenger " <<id << " could not be found."<<std::endl;
	return false;
}

//function to print the passengers on the upgrade list
void upl(std::vector<std::string> &upList)
{
	//the passengers id and class are in the format ....* where the .... is the id and the *is the class
	//got through and print out the passengers being upgraded to first class first
	std::cout << "The following passengers are scheduled to be upgraded to first class:"<<std::endl;
	for(unsigned int i = 0; i<upList.size();i++)
	{
		if(upList[i][4]=='B')
		{
			std::cout << upList[i].substr(0,4)<<std::endl;
		}
	}
	//print the passengers being upgraded to business class
	std::cout <<"The following passengers are scheduled to be upgraded to business class:"<<std::endl;
	for(unsigned int i = 0;i<upList.size();i++)
	{
		if(upList[i][4]=='C')
			std::cout << upList[i].substr(0,4)<<std::endl;
	}
}

//a simiple function that finds the seat of a passenger with the given id. returns true if the seat exists, false otherwise
bool find_seat(std::string id, std::vector<std::vector<std::string> > &chart,std::string &column,int &row)
{
	for(unsigned int i=0;i<chart.size();i++)
	{
		for(unsigned int j=0;j<chart[0].size();j++)
		{
			if(chart[i][j]==id)
			{
				row = i+1;
				column = std::string(1,ALPHABET[j-1]);
				return true;
			}
		}
	}
	return false;
}

//function to upgrade the passengers on the upgrade list
void upp(std::vector<std::vector<std::string> > &chart,std::vector<std::string> &upList)
{

	int row;
	std::string column;
	//goes through each element on the upgrade list
	for(unsigned int i = 0;i<upList.size();i++)
	{
		//if the person is to be upgraded to the first class seciont
		if(upList[i][4]=='B')
		{
			//if they can be put into the section
			if(srn(chart,upList[i].substr(0,4),"F","N",upList))
			{
				//give them their seat, and then print the necessary information
				find_seat(upList[i].substr(0,4),chart,column,row);
				std::cout << "Passenger "<<upList[i].substr(0,4)<<" was upgraded to the first class section."<<std::endl;
				//remove them from the upgrade list
				del(upList,upList[i]);
			}
			else
			{
				//if there isnt a seat for them, print this
				std::cout <<"Could not find a seat in the first class section for passenger "<<upList[i].substr(0,4)<<"."<<std::endl;
			}
		}
		//same thing as above, but for upgrades from b to c
		else if(upList[i][4]=='C')
		{
			if(srn(chart,upList[i].substr(0,4),"B","N",upList))
			{
				find_seat(upList[i].substr(0,4),chart,column,row);
				std::cout << "Passenger "<<upList[i].substr(0,4)<<" was given seat "<<row<<column<<"."<<std::endl;
				std::cout << "Passenger "<<upList[i].substr(0,4)<<" was upgraded to the business class section."<<std::endl;
				del(upList,upList[i]);
			}
			else
			{
				std::cout <<"Could not find a seat in the business class section for passenger "<<upList[i].substr(0,4)<<"."<<std::endl;
			}
		}
	}
}

//deletes a specific value in a vector
void del(std::vector<std::string> &upList,std::string thing)
{
	//a temporary vector to hold all the non deleted values
	std::vector<std::string> temp;
	for(int i = upList.size()-1;i>=0;i--)
	{
		//copy all values except the one not wanted into the temp vector
		if(upList[i]!=thing)
		{
			temp.push_back(upList[i]);
		}
	}
	//assign the temp to upList
	upList = temp;
}
