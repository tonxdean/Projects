#include "Matrix.h"

//constuctor that takes rows, columns, and a fill value
Matrix::Matrix(unsigned int n_rows,unsigned int n_col,double fill)
{
	create(n_rows,n_col,fill);
}

//consturcutor to create an empty matrix
Matrix::Matrix()
{
	data = NULL;
	rows=0;
	columns=0;
}


//copy constructor
Matrix::Matrix(const Matrix& m)
{
	this->copy(m);
}

//destructor
Matrix::~Matrix()
{
	clear();
}

//the next two functions are pretty easy to get, they return the numer of rows or columns
unsigned int Matrix::num_rows() const
{
	return rows;
}

unsigned int Matrix::num_cols() const
{
	return columns;
}

//clears all the memory in the matrix
void Matrix::clear()
{
	//if it isnt already empty
	if(!(rows==0||columns==0))
	{
		//delete each row
		for(unsigned int i=0;i<rows;i++)
		{
			delete [] data[i];
		}
		//delete the array that holds each row
		delete [] data;
		//set all the values to zero, and the pointer to null
		rows=0;
		columns=0;
		data=NULL;
	}
}

//the copy function, takes another matrix as an argument and makes the current matrix equivalent
void Matrix::copy(const Matrix& m)
{
	double temp;
	
	//if its literally the same matrix, dont do anything
	if(this!=&m)
	{
		//if it is not a size zero matrix
		if(m.num_cols()!=0&&m.num_rows()!=0)
		{
			//copy the values for rows and columns over
			rows = m.num_rows();
			columns = m.num_cols();
			//make the rows
			data = new double* [m.num_rows()];
			for(unsigned int i=0;i<m.num_rows();i++)
			{
				//make the columns
				data[i]=new double[m.num_cols()];
				for(unsigned int j=0;j<m.num_cols();j++)
				{
					//get the value from the copied matrix, and put it in the matrix copied to
					m.get(i,j,temp);
					data[i][j] = temp;
				}
			}
		}
		//if thee array is size zero
		else
		{
			//set default values
			data=NULL;
			rows=0;
			columns=0;
		}
	}
}

//the create helper functions, takes in number of rows, columns, and a fill value
void Matrix::create(unsigned int n_rows,unsigned int n_col,double fill)
{
	//if it isnt a size zero matrix...
	if(!(n_rows==0||n_col==0))
	{
		//make the rows and columns equal to what was passed in
		rows = n_rows;
		columns = n_col;
		//create the rows
		data = new double*[n_rows];
		for(unsigned int i=0;i<n_rows;i++)
		{
			//create the columns
			data[i] = new double[n_col];
			for(unsigned int j=0;j<columns;j++)
			{
				//assign the fill value to each element
				data[i][j]=fill;
			}
		}
	}
	//if it is a size zero matrix
	else
	{
		//assign 0 values
		rows=0;
		columns=0;
		data = NULL;
	}
	
}


//gets a specific value at a point signaled by the row and column passed in. the value is stored in dub
bool Matrix::get(unsigned int arow, unsigned int acolumn,double& dub) const
{
	
	//if the index is out of bounds, return false
	if(arow>=rows||acolumn>=columns)
	{
		return false;
	}
	//otherwise, store the value at the index, and return true
	dub = data[arow][acolumn];

	return true;
}


//changes the value at given point in the matrix
bool Matrix::set(unsigned int arow,unsigned int acolumn,double dub) 
{
	//if the value is not out of range
	if(arow<rows&&acolumn<columns)
	{
		//change the value at the given point
		data[arow][acolumn]=dub;
		return true;
	}
	return false;
}


//changes every value by the passed in coefficient
void Matrix::multiply_by_coefficient(double coef)
{
	//goes through the rows
	for(unsigned int i=0;i<rows;i++)
	{
		//then columns
		for(unsigned int j=0;j<columns;j++)
		{
			//and multiplies every value by the coefficient
			data[i][j] = data[i][j]*coef;
		}
	}
}


//changes the order of the rows in the matrix, takes in a first and second row unsigned integer
bool Matrix::swap_row(unsigned int first,unsigned int second)
{
	//if the row numbers are out of bounds, return false
	if(first>=rows||second>=rows)
	{
		return false;
	}
	//otherwise just switch the pointers around, easy peasy
	double *temp = data[first];
	data[first]=data[second];
	data[second]=temp;
	return true;
}



//changes the rows to columns and columns to rows
void Matrix::transpose()
{
	//creates a temporary matrix that has rows= to the columns of the original and columns= to the rows
	Matrix temp=Matrix(columns,rows,0);
	
	//this nested for loop essentially places the data at the point [i][j] in what would be the equivalent transposed point on the temp matrix
	for(unsigned int i=0;i<rows;i++)
	{
		for(unsigned int j=0;j<columns;j++)
		{
			temp.set(j,i,data[i][j]);
		}
	}
	//clears what is in the matrix, and makes it a copy of the transposed matrix
	this -> clear();
	copy(temp);
	temp.clear();
}


//adds two matrices together, takes in a matrix as an argument
bool Matrix::add(const Matrix& m)
{
	double temp;
	//check to see if they are the sam size
	if(rows==m.num_rows()&&columns==m.num_cols())
	{
		for(unsigned int i=0;i<rows;i++)
		{
			for(unsigned int j=0;j<columns;j++)
			{
				//if they are the same size, just add each element together
				m.get(i,j,temp);

				data[i][j]+=temp;

			}
		}
		//if they are the same size and the addition was succesfull return true
		return true;
	}
	//if they arent the same size return false
	return false;
}


//subtracts a matrix from the other, takes in a matrix as an argument
bool Matrix::subtract(const Matrix& m)
{
	
	//not much to see here, same as the add, but with subtraction
	double temp;
	if(rows==m.num_rows()&&columns==m.num_cols())
	{
		for(unsigned int i=0;i<rows;i++)
		{
			for(unsigned int j=0;j<columns;j++)
			{
				m.get(i,j,temp);
				data[i][j]-=temp;
			}
		}
		return true;
	}
	return false;
}


//returns the specified row, takes the row number as an argument
double* Matrix::get_row(unsigned int row) const
{
	//if the row is out of bounds, return a null pointer
	if(row>=rows||rows==0||columns==0)
	{
		return NULL;
	}
	
	//if not, create an array, copy each value over, and then return the array
	double* ret = new double[columns];
	for(unsigned int i=0;i<columns;i++)
	{
		ret[i]=data[row][i];
	}
	return ret;
}


//returns the specified column, takes the column number as an argument
double* Matrix::get_col(unsigned int col) const
{
	//pretty much the same as get row
	if(col>=columns||rows==0||columns==0)
	{
		return NULL;
	}
	double* ret=new double[rows];
	for(unsigned int i=0;i<rows;i++)
	{
		ret[i]= data[i][col];
	}
	return ret;
}


//the asignment operator
Matrix& Matrix::operator=(const Matrix& m)
{
	//doesnt do anything if a matrix is assigned to itself
	if(this!=&m)
	{
		//other wise, clear the matrix and make it a copy of the one being passed in
		clear();
		copy(m);
	}
	//return a reference to the matrix
	return *this;
}


//operator that checks the equality of two matrices
bool Matrix::operator==(const Matrix& m)
{
	double temp;
	//if a matrix is compared to itself, return true
	if(this==&m)
	{
		return true;
	}
	//if the two matrices have different sizes, return false
	if(rows!=m.num_rows()||columns!=m.num_cols())
	{
		return false;
	}
	//if there are any values not the same, return false
	for(unsigned int i=0;i<rows;i++)
	{
		for(unsigned int j=0;j<columns;j++)
		{
			m.get(i,j,temp);
			if(data[i][j]!=temp)
			{
				return false;
			}
		}
	}
	//if not returned false, return true
	return true;
}

bool Matrix::operator!=(const Matrix& m)
{
	//like == but opposite
	double temp;
	if(this==&m)
	{
		return false;
	}
	if(rows!=m.num_rows()||columns!=m.num_cols())
	{
		return true;
	}
	for(unsigned int i=0;i<rows;i++)
	{
		for(unsigned int j=0;j<columns;j++)
		{
			m.get(i,j,temp);
			if(data[i][j]!=temp)
			{
				return true;
			}
		}
	}
	return false;
}

//the legendary quarter function. splits a matrix into four equal sections and returns them as an array of matrices
Matrix* Matrix::quarter() const
{
	double temp;
	//allocate memory for the matrices
	Matrix* ret = new Matrix[4];
	//calculate split rows-- half the rows rounded up. same for split_columns
	unsigned int split_rows = (int)(ceil((double) rows/2));
	unsigned int split_cols=(int)(ceil((double) columns/2));
	
	//this determines where to start for quarters that copy rows and columns not starting with the firsts
	unsigned int row_start = rows-split_rows;
	unsigned int col_start = columns - split_cols;
	
	//create the matrices
	for(unsigned int i=0;i<4;i++)
	{
		ret[i] = Matrix(split_rows,split_cols,0);
	}
	
	
	//the loops all start at 0, but the values take from the original matrix are affected by row_start and col_start
	
	//q1 -- start from 0 for rows and columns, copy up untill split
	for(unsigned int i=0;i<split_rows;i++)
	{
		for(unsigned int j=0;j<split_cols;j++)
		{
			get(i,j,temp);
			ret[0].set(i,j,temp);
		}
	}
	//q2--start from 0 for rows and col_start for columns
	for(unsigned int i=0;i<split_rows;i++)
	{
		for(unsigned int j = 0;j<split_cols;j++)
		{
			get(i,j+col_start,temp);
			ret[1].set(i,j,temp);
		}
	}
	//q3 -- start from row_start for rows and 0 for columns
	for(unsigned int i=0;i<split_rows;i++)
	{
		for(unsigned int j = 0;j<split_cols;j++)
		{
			get(i+row_start,j,temp);
			ret[2].set(i,j,temp);
		}
	}
	//q4 -- start from row_start and column_start
	for(unsigned int i=0;i<split_rows;i++)
	{
		for(unsigned int j = 0;j<split_cols;j++)
		{
			get(i+row_start,j+col_start,temp);
			ret[3].set(i,j,temp);
		}
	}
	return ret;
}


//the output stream operator
std::ostream& operator<<(std::ostream& out, const Matrix& m)
{
	
	double temp;
	//format the output in the necessary fashion with [ data ]
	out<<"[";
	for(unsigned int i=0;i<m.num_rows();i++)
	{
		//for every row output spaces, then each element in the row, and then endl
		if(i!=0)
		{
			out<<" ";
		}
		//output each element in a row
		for(unsigned int j=0;j<m.num_cols();j++)
		{
			m.get(i,j,temp);
			out<<" "<<temp;
		}
		if(i!=m.num_rows()-1)
		{
			out<<" "<<std::endl;
		}
	}
	out<<" ]";
	out<<std::endl;
	return out;
}

//resizes the matrix, takes in the number of rows, columns and a fill value
void Matrix::resize(unsigned int num_row,unsigned int num_col, double fill)
{
	double input;
	//creates a temporary matrix as a copy of the current one
	Matrix temp(*this);
	//clears the current matrix
	clear();
	//creates this matrix with the size specified by the resize
	create(num_row,num_col,fill);
	//goes through and copies the values from the temporary matrix. does not copy if out of the new matrix
	for(unsigned int i=0;i<num_row;i++)
	{
		for(unsigned int j=0;j<num_col;j++)
		{
			if(i<temp.num_rows()&&j<temp.num_cols())
			{
				temp.get(i,j,input);
				set(i,j,input);
			}
		}
	}
	
}






