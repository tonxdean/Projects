#include <fstream>
#include <string>
#include <vector>
#include <iostream>


void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f);
void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions);
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,unsigned int x,unsigned int y,unsigned int direction);
void add_in_direction(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,unsigned int x,unsigned int y,unsigned int direction);

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
			
		}
	}
	
	for(unsigned int i = 0;i<solutions.size();i++)
	{
		prt(solutions[i],out_f);
	}
		
	
	return 0;
}

void add_word(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions)
{
		add(grid,word,solutions,0,0,0,0);
}

//passing by copy in this case makes the program work better, as I would need to make copies anyway, and this makes it much less confusing
void add(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,unsigned int x,unsigned int y,unsigned int direction)
{
	std::vector<std::vector<std::string> > temp = grid;
	//if the word gets finished, a solution has been found
	if(word_position==word.length())
	{
		solutions.push_back(grid);
		return;
	}
	//if the solution starts to go out of bounds, it does not work
	if(y>=grid.size()||y<0)
	{
		return;
	}
	
	if(grid[y][x]=="." || grid[y][x].at(0)==word.at(word_position))
	{
		for(unsigned int i=0;i<grid.size();i++)
		{
		for(unsigned int j=0;j<grid[i].size();j++)
		{
			std::cout<<grid[i][j];
		}
		std::cout<<std::endl;
		}
		
		temp[y][x].at(0)=word.at(word_position);
		if(direction == 0||direction==1)
		{
			add(temp,word,solutions,word_position+1,x,y+1,1);
		}
		else if(direction == 0||direction == 2)
		{
			add(temp,word,solutions,word_position+1,x+1,y+1,2);
		}
		else if(direction == 0||direction == 3)
		{
			add(temp,word,solutions,word_position+1,x+1,y,3);
		}
		else if(direction == 0||direction == 4)
		{
			add(temp,word,solutions,word_position+1,x+1,y-1,4);
		}
		else if(direction == 0||direction == 5)
		{
			add(temp,word,solutions,word_position+1,x,y-1,5);
		}
		else if(direction == 0||direction == 6)
		{
			add(temp,word,solutions,word_position+1,x-1,y-1,6);
		}
		else if(direction == 0||direction == 7)
		{
			add(temp,word,solutions,word_position+1,x-1,y,7);
		}
		else if(direction == 0||direction == 8)
		{
			add(temp,word,solutions,word_position+1,x-1,y+1,8);
		}
		
		
	}
	else
	{
		if(x>=grid[y].size())
		{
			add(grid,word,solutions,word_position,0,y+1,direction);
		}
		else
		{
			add(grid,word,solutions,word_position,x+1,y,direction);
		}
	}
}

void prt(std::vector<std::vector<std::string> >& grid,std::ofstream& out_f)
{
	for(unsigned int i=0;i<grid.size();i++)
	{
		for(unsigned int j=0;j<grid[i].size();j++)
		{
			out_f<<grid[i][j];
		}
		out_f<<std::endl;
	}
	
	
	return;
}

void add_in_direction(std::vector<std::vector<std::string> >  grid,std::string word,std::vector<std::vector<std::vector<std::string> > >& solutions,unsigned int word_position,unsigned int x,unsigned int y,unsigned int direction)
{
		grid[y][x]=word.at(word_position);
		if(word_position == word.length())
		{
			solutions.push_back(grid);
			return;
		}
		if(direction == 1)
		{
			add_in_direction(grid,word,solutions,word_position+1,x,y+1,1);
		}
		else if(direction == 0||direction == 2)
		{
			add_in_direction(grid,word,solutions,word_position+1,x+1,y+1,2);
		}
		else if(direction == 0||direction == 3)
		{
			add_in_direction(grid,word,solutions,word_position+1,x+1,y,3);
		}
		else if(direction == 0||direction == 4)
		{
			add_in_direction(grid,word,solutions,word_position+1,x+1,y-1,4);
		}
		else if(direction == 0||direction == 5)
		{
			add_in_direction(grid,word,solutions,word_position+1,x,y-1,5);
		}
		else if(direction == 0||direction == 6)
		{
			add_in_direction(grid,word,solutions,word_position+1,x-1,y-1,6);
		}
		else if(direction == 0||direction == 7)
		{
			add_in_direction(grid,word,solutions,word_position+1,x-1,y,7);
		}
		else if(direction == 0||direction == 8)
		{
			add_in_direction(grid,word,solutions,word_position+1,x-1,y+1,8);
		}
	
	
	
}






