#include <fstream>
#include <string>
#include <vector>
#include <iostream>


void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f);
void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions);
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,int x,int y,unsigned int direction,const std::vector<std::vector<std::string> >&  start_grid,int& start_x,int& start_y,bool top);
void do_fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions);
void fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions,int x,int y,int let);
bool remove_bad(const std::vector<std::vector<std::string> >&  grid,std::string word,unsigned int word_position,int x,int y,unsigned int direction);
bool bad_word(std::vector<std::vector<std::string> >  grid,std::string word);
std::string reverse_word(const std::string& word);

const std::string ALPHABET("abcdefghijklmnopqrstuvwxyz");

int main(int arc,char* argv[])
{
	//open the files and check to see if it worked
	std::ifstream in_f(argv[1]);
	std::ofstream out_f(argv[2]);
	if(!in_f || !out_f)
	{	
		std::cout<<"ya done goofed"<<std::endl;
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
	while(in_f>>temp)
	{
		if(temp=="+")
		{
			in_f>>temp;
			add_word(grid,temp,solutions);
		}
		else
		{
			in_f>>temp;
			bad_words.push_back(temp);
		}
	}
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

	
	
	//do_fill(grid,solutions);
	
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

void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions)
{
	int start_x=0;
	int start_y=0;
	if(solutions.size()==0)
	{
		add(grid,word,solutions,0,0,0,0,grid,start_x,start_y,true);
	}
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

void do_fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions)
{
	std::vector<std::vector<std::vector<std::string > > > temp = solutions;
	solutions.clear();
	
	for(unsigned int i = 0;i<temp.size();i++)
	{
		fill(temp[i],solutions,0,0,0);
	}
	
}

bool bad_word(std::vector<std::vector<std::string> >  grid,std::string word)
{
	for(unsigned int i = 0;i<grid.size();i++)
	{
		for(unsigned int j=0;j<grid[0].size();j++)
		{
			if(remove_bad(grid,word,0,j,i,0))
			{
				return true;
			}
		}
	}
	
	return false;
	
	
}

bool remove_bad(const std::vector<std::vector<std::string> >&  grid,std::string word,unsigned int word_position,int x,int y,unsigned int direction)
{
	if(word_position == word.length())
	{
		return true;
	}
	if(y>=grid.size()||x>=grid[0].size()||x<0||y<0)
	{
		return false;
	}

	if(grid[y][x].at(0)==word.at(word_position))
	{
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
		if(direction==0)
		{
			return(remove_bad(grid,word,word_position+1,x,y+1,1)||remove_bad(grid,word,word_position+1,x+1,y+1,2)||remove_bad(grid,word,word_position+1,x+1,y,3)||
					remove_bad(grid,word,word_position+1,x+1,y-1,4)||remove_bad(grid,word,word_position+1,x,y-1,5)||remove_bad(grid,word,word_position+1,x-1,y-1,6)||
					remove_bad(grid,word,word_position+1,x-1,y,7)||remove_bad(grid,word,word_position+1,x-1,y+1,8));
			
		}
		
	}
	else
	{
		return false;
	}
}


void fill(std::vector<std::vector<std::string> >  grid,std::vector<std::vector<std::vector<std::string> > >& solutions,int x,int y,int let)
{
	//std::cout<<x<<"  "<<y<<std::endl;
	if(x>=grid[0].size())
	{
		//std::cout<<"1"<<std::endl;
		fill(grid,solutions,0,y+1,0);
	}
	if(y>=grid.size())
	{
		for(unsigned int i=0;i<grid.size();i++)
		{
			std::cout<<"\t";
			for(unsigned int j=0;j<grid[i].size();j++)
			{
				std::cout<<grid[i][j];
			}
			std::cout<<std::endl;
		}	
		
		
		solutions.push_back(grid);
		return;
	}
	if(!(x>=grid[0].size()||y>=grid.size()))
	{
	if(grid[y][x]!=".")
	{
		//std::cout<<"3"<<std::endl;
		fill(grid,solutions,x+1,y,0);
	}
	if(let>25)
	{
		//std::cout<<"5"<<std::endl;
		fill(grid,solutions,x+1,y,0);
	}
	if(grid[y][x]==".")
	{
		//std::cout<<"4"<<std::endl;
		fill(grid,solutions,x,y,let+1);
		grid[y][x]=ALPHABET[let];
	}
	}
}

//passing by copy in this case makes the program work better, as I would need to make copies anyway, and this makes it much less confusing
//this function takes a grid that serves as the start for the word currently being added, as well as a vector to store all the possible grid outcomes in
//next is word position, which keeps track of what letter in the word is being used. x and y keep track of the current position on the grid. startx and starty are used to tell where
//the word currently being put on the board starts from. direction is used to keep the word going in the same direction once started
//start grid is a copy of the grid thats having a word added to it, so that it can be reset if a word is not succesfully added.
//and top is used so that functions that the function can keep track of when it is on the starting call for a specific position.
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,int x,int y,unsigned int direction,const std::vector<std::vector<std::string> >&  start_grid,int& start_x,int& start_y,bool top)
{
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

void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f)
{
	out_f<<"Board:"<<std::endl;
	for(unsigned int i=0;i<grid.size();i++)
	{
		out_f<<"\t";
		for(unsigned int j=0;j<grid[i].size();j++)
		{
			out_f<<grid[i][j];
		}
		out_f<<std::endl;
	}
	
	
	return;
}

std::string reverse_word(const std::string& word)
{
	std::string ret = "";
	for(unsigned int i=0;i<word.size();i++)
	{
		ret+=word[word.size()-1-i];
		
		
	}
	return ret;
	
	
}



