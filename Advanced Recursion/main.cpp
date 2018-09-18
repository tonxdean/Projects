#include <fstream>
#include <string>
#include <vector>
#include <iostream>


void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f);
void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions);
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,int x,int y,unsigned int direction,const std::vector<std::vector<std::string> >&  start_grid,int& start_x,int& start_y,bool top);
void do_fill(std::vector<std::vector<std::vector<std::string> > >& solutions,const std::vector<std::string>& bad_words);
void fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions,int x,int y,int let,const std::vector<std::string>& bad_words);
bool remove_bad(const std::vector<std::vector<std::string> >&  grid,std::string word,unsigned int word_position,int x,int y,unsigned int direction);
bool bad_word(std::vector<std::vector<std::string> >  grid,std::string word);
std::string reverse_word(const std::string& word);
bool same_grid(const std::vector<std::vector<std::string> >&  grid1,const std::vector<std::vector<std::string> >&  grid2);

const std::string ALPHABET("abcdefghijklmnopqrstuvwxyz");

int main(int arc,char* argv[])
{
	//open the files and check to see if it worked
	std::ifstream in_f(argv[1]);
	std::ofstream out_f(argv[2]);
	if(!in_f || !out_f)
	{	
		std::cout<<"ya done goofed"<<std::endl;
		exit(1);
	}
	//take in the width and heigth
	unsigned int width,heigth;
	in_f>>width>>heigth;
	
	//create the blank grid
	std::vector<std::vector<std::string> > grid;
	std::vector<std::string> blank;
	for(unsigned int i=0;i<heigth;i++)
	{
		grid.push_back(blank);
		for(unsigned int j=0;j<width;j++)
		{
			grid[i].push_back(".");
		}
	}

	std::vector<std::string> bad_words;
	std::vector<std::vector<std::vector<std::string> > > solutions;
	
	std::string temp;
	//this loop goes through the input file and adds all the words to the grids, coming up with  all the solutions.
	//if a bad word appears, it is added to a vector so that the grids can be checked against the bad words later
	//the blank_sol variable is a check to see if there are no words added to the grid
	bool blank_sol = true;
	while(in_f>>temp)
	{
		if(temp=="+")
		{
			in_f>>temp;
			add_word(grid,temp,solutions);
			blank_sol = false;
		}
		else
		{
			in_f>>temp;
			bad_words.push_back(temp);
		}
	}
	if(blank_sol)
	{
		solutions.push_back(grid);
	}
	
	//goes through and erases any solution with a bad word in it
	for(unsigned int j=0;j<bad_words.size();j++)
	{
		for(unsigned int i=0;i<solutions.size();i++)
		{
			if(bad_word(solutions[i],bad_words[j]))
			{
				solutions.erase(solutions.begin()+i);
				--i;
			}
		}
	}

	//this function fills all the empty spaces on all the grids
	do_fill(solutions,bad_words);
	
	
	//check again for bad words that may have appeared after everything was filled in
	for(unsigned int j=0;j<bad_words.size();j++)
	{
		for(unsigned int i=0;i<solutions.size();i++)
		{
			if(bad_word(solutions[i],bad_words[j]))
			{
				solutions.erase(solutions.begin()+i);
				--i;
			}
		}
	}
	
	//erase duplicate grids
	for(unsigned int i = 0;i<solutions.size();i++)
	{
		for(unsigned int j = i+1;j<solutions.size();j++)
		{
			if(same_grid(solutions[i],solutions[j]))
			{
				solutions.erase(solutions.begin()+j);
				--j;
			}
			
		}
		
	}
	
	//output the solutions or one solution if requested
	if(solutions.size()!=0)
	{
		if(std::string(argv[3])!="one_solution")
		{
			out_f<< solutions.size()<<" solution(s)"<<std::endl;
			for(unsigned int i = 0;i<solutions.size();i++)
			{
				prt(solutions[i],out_f);
			}
		}
		else
		{
			prt(solutions[0],out_f);
		}
	}
	else
	{
		out_f<<"No Solutions"<<std::endl;
	}

		
	
	return 0;
}

//the driver for the add function, takes in the grid and adds the required word to it and places it in the solutions vector.
//takes in the grid, a word that is added to the grid, and the solutions vector
void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions)
{
	//keeps track of the starting position
	int start_x=0;
	int start_y=0;
	//basically everytime a word is added to the grid, those grids with the new words are temporarily stored in the solutions vector
	//when another word is added, the solutions vector has to be cleared or else it will hold grids without all the words on them
	
	//this if statement fills the origianl solutions vector
	if(solutions.size()==0)
	{
		add(grid,word,solutions,0,0,0,0,grid,start_x,start_y,true);
	}
	//this else statement clears and then refills the solutions vector with the grids with the new word added on to them
	else
	{
		std::vector<std::vector<std::vector<std::string> > > temp = solutions;
		solutions.clear();
		
		for(unsigned int z=0;z<temp.size();z++)
		{
			add(temp[z],word,solutions,0,0,0,0,temp[z],start_x,start_y,true);
			start_x=0;
			start_y=0;
		}
		
	}
}

//this function drives the fill function so that it does not need to have 3 zeros, that are necessary for the recursion, passed into it from main
//this takes the solutions vector in
void do_fill(std::vector<std::vector<std::vector<std::string> > >& solutions,const std::vector<std::string>& bad_words)
{
	//clear the solutions vector to let it be filled with the new filled in solutions
	std::vector<std::vector<std::vector<std::string> > > temp = solutions;
	solutions.clear();
	//go through each solution stored in the temp vector and add all the filled solutions into the solutions vector
	for(unsigned int i=0;i<temp.size();i++)
	{
		fill(temp[i],solutions,0,0,0,bad_words);
	}
}


//function to fill up the empty spaces on the board
//takes the grid as an argument, the solutions so that it may add to them, coordinates to go from
// and then the list of bad words to check for while filling, to reduce run time
void fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions,
int x,int y,int let,const std::vector<std::string>& bad_words)
{
	
	
	//if you go out of the grid, then all the solutions have been found and return
	if(y>grid.size()-1)
	{
		solutions.push_back(grid);
		return;
	}
	//if theres a period at the place in the grid you are currently at...
	if(grid[y][x]==".")
	{
		//call the fill function once for every possible letter, this creates a copy of the grid for each letter
		if(let<25)
		{
			fill(grid,solutions,x,y,let+1,bad_words);
		}
		grid[y][x]=ALPHABET[let];
		//check for bad words
		for(unsigned int i = 0;i<bad_words.size();i++)
		{
			if(bad_word(grid,bad_words[i]))
			{
				return;
			}
		}
	}
	//this if statement is here to go increment the x location
	if(x<grid[0].size()-1)
	{
		fill(grid,solutions,x+1,y,0,bad_words);
	}
	//this statement increments the y position on the grid
	else
	{
		fill(grid,solutions,0,y+1,0,bad_words);
	}

}


//driver function that trys to find bad word in the grid
bool bad_word(std::vector<std::vector<std::string> >  grid,std::string word)
{
	//this calls the remove_bad function on every part of the grid
	for(unsigned int i = 0;i<grid.size();i++)
	{
		for(unsigned int j=0;j<grid[0].size();j++)
		{
			if(remove_bad(grid,word,0,j,i,0))
			{
				//if a bad word is found, return true
				return true;
			}
		}
	}
	//if a bad word is never found, return false
	return false;
	
	
}

//this function starts from a specific point on the grid and checks for bad words
//takes the grid by reference to check it, the bad word to find, as well as something to keep track of the position in the word, the coordinates, and what direction to go
//this works mostly the same as the add function below
bool remove_bad(const std::vector<std::vector<std::string> >&  grid,std::string word,unsigned int word_position,int x,int y,unsigned int direction)
{
	//if the function got through the word, then it must be in the grid, return true to indicate it was found
	if(word_position == word.length())
	{
		return true;
	}
	//if the word does not fit at the location, then there is no need to keep checking
	if(y>=grid.size()||x>=grid[0].size()||x<0||y<0)
	{
		return false;
	}

	//if the letter at the current position in the word matches the letter at the current spot in the grid...
	if(grid[y][x].at(0)==word.at(word_position))
	{
		// if its the first time through, the direction is zero, meaning every direction is checked
		//if the word is found in any direction, the function will return true
		if(direction==0)
		{
			return(remove_bad(grid,word,word_position+1,x,y+1,1)||remove_bad(grid,word,word_position+1,x+1,y+1,2)||remove_bad(grid,word,word_position+1,x+1,y,3)||
					remove_bad(grid,word,word_position+1,x+1,y-1,4)||remove_bad(grid,word,word_position+1,x,y-1,5)||remove_bad(grid,word,word_position+1,x-1,y-1,6)||
					remove_bad(grid,word,word_position+1,x-1,y,7)||remove_bad(grid,word,word_position+1,x-1,y+1,8));
			
		}
		
		//each direction is essentially the same, the grid is passed in, as is the word, and then the position in the word is incremented as are the coordinates the required
		//direction
		if(direction==1)
		{
			return remove_bad(grid,word,word_position+1,x,y+1,1);
		}
		if(direction==2)
		{
			return remove_bad(grid,word,word_position+1,x+1,y+1,2);
		}
		if(direction==3)
		{
			return remove_bad(grid,word,word_position+1,x+1,y,3);
		}
		if(direction==4)
		{
			return remove_bad(grid,word,word_position+1,x+1,y-1,4);
		}
		if(direction==5)
		{
			return remove_bad(grid,word,word_position+1,x,y-1,5);
		}
		if(direction==6)
		{
			return remove_bad(grid,word,word_position+1,x-1,y-1,6);
		}
		if(direction==7)
		{
			return remove_bad(grid,word,word_position+1,x-1,y,7);
		}
		if(direction==8)
		{
			return remove_bad(grid,word,word_position+1,x-1,y+1,8);
		}
		
		
	}
	//if the letters on the grid and at the current position in the word do not match, return false
	else
	{
		return false;
	}
}



//passing by copy in this case makes the program work better, as I would need to make copies anyway, and this makes it much less confusing
//this function takes a grid that serves as the start for the word currently being added, as well as a vector to store all the possible grid outcomes in
//next is word position, which keeps track of what letter in the word is being used. x and y keep track of the current position on the grid. startx and starty are used to tell where
//the word currently being put on the board starts from. direction is used to keep the word going in the same direction once started
//start grid is a copy of the grid thats having a word added to it, so that it can be reset if a word is not succesfully added.
//and top is used so that functions that the function can keep track of when it is on the starting call for a specific position.
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,
unsigned int word_position,int x,int y,unsigned int direction,const std::vector<std::vector<std::string> >&  start_grid,int& start_x,int& start_y,bool top)
{
	//this check is to make sure that duplicate grids are not added when the word is the same forwards and backwards
	//it does this by not adding the word in the directions that are opposite other directions on the board
	if(word==reverse_word(word)&&(direction==5||direction==6||direction==7||direction==8))
	{
		return;
	}
	//if the word is succesfully completed, then a solution has been found. add it to the vector
	if(word_position == word.length())
	{
		solutions.push_back(grid);
		return;
	}
	//if the word starts to go out of the bounds of the grid, it does not work, and if the starting place goes out of bounds, this will end the recursion
	if(y>=grid.size()||x>=grid[0].size()||x<0||y<0||start_x>=grid[0].size()||start_y>=grid.size())
	{
		return;
	}
	//if the current letter of the word can be put in the current place...
	if(grid[y][x]=="."||grid[y][x].at(0)==word.at(word_position))
	{
		//a copy of what is currently in the grid, so that the grid goes back to the way it started once the current call is done with it
		std::string current = grid[y][x];
		//these next if statements do the same thing but in different directions
		//they continue the word in the specified direction, erasing what they did to the board once they are done using it
		if(direction==0||direction==1)
		{
			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x,y+1,1,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==2)
		{
			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x+1,y+1,2,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==3)
		{
			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x+1,y,3,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==4)
		{
			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x+1,y-1,4,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==5)
		{

			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x,y-1,5,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==6)
		{

			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x-1,y-1,6,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==7)
		{

			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x-1,y,7,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		if(direction==0||direction==8)
		{
			grid[y][x].at(0)=word.at(word_position);
			add(grid,word,solutions,word_position+1,x-1,y+1,8,start_grid,start_x,start_y,false);
			grid[y][x]=current;
		}
		
		
	}
	//if you are at the top of the function, and went through every possible direction with the word, shift the starting place
	if(top)
	{
		//if you are at the end of a row, change the row
		if(start_x>=grid[0].size()-1)
		{
			start_y++;
			start_x=0;
			add(start_grid,word,solutions,0,start_x,start_y,0,start_grid,start_x,start_y,true);
		}
		//otherwise change the column
		else
		{
			start_x++;
			add(start_grid,word,solutions,0,start_x,start_y,0,start_grid,start_x,start_y,true);
		}
	}
	
	
}

//a simple function that outputs the grid passed in
//takes a grid and a out file stream by refernce
void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f)
{
	//print the header
	out_f<<"Board:"<<std::endl;
	for(unsigned int i=0;i<grid.size();i++)
	{
		//go through each row and print every item in the row
		out_f<<"\t";
		for(unsigned int j=0;j<grid[i].size();j++)
		{
			out_f<<grid[i][j];
		}
		//go to the next line once the row is over
		out_f<<std::endl;
	}
	
	
	return;
}

//a function to revers a word
//takes a string by refernce
std::string reverse_word(const std::string& word)
{
	std::string ret = "";
	//goes through the string backwards and appends each letter to the string that is to be returned
	for(unsigned int i=0;i<word.size();i++)
	{
		ret+=word[word.size()-1-i];
		
		
	}
	//then returns it
	return ret;
	
	
}


//this function goes through to check if the two grids passed in by reference are the same
bool same_grid(const std::vector<std::vector<std::string> >&  grid1,const std::vector<std::vector<std::string> >&  grid2)
{
	//if they arent the same size, then they arent the same, return false
	if(grid1.size()!=grid2.size()||grid1[0].size()!=grid2[0].size())
	{
		return false;
	}
	
	//goes through each grid position
	for(unsigned int i = 0;i<grid1.size();i++)
	{
		for(unsigned int j=0;j<grid1[0].size();j++)
		{
			//if they ever arent the same, return false
			if(grid1[i][j]!=grid2[i][j])
			{
				return false;
			}
			
		}
		
		
	}
	//return true if the grids are never shown to be different
	return true;
}



