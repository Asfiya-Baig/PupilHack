#include <stdio.h>
#include <iostream>
#include <fstream>
#define p printf
#define s scanf
#include <stdlib.h>



int working_window=7; //size of working window
float work_window1[7]; //working window array
float work_window2[7]; //working window array
float *input_values1;  // input values array
float *input_values2;  // input values array
float *output_values1; //output values array
float *output_values2; //output values array
float *tim; //time array
int *pulse1;
int *pulse2;
int array_size; //size of arrays
int count=0;
int sum1=0;
int sum2=0;

using namespace std;

int main()
{
int i=1;
int j,k;
int temp;
int flag=0;
int heap_alloc;
ifstream image_file("radtime.csv", std::ios_base::in); ///reading the input file containing readius vs  time
ofstream image_filee("radtime1.csv"); //creating file for the output after applying filter
image_file.seekg(0,std::ios::end); //go to end of file to find size
heap_alloc=image_file.tellg()/2; //find the size of file and divide it by two to allocate size for heap arrays input values, output values and tim
//cout<< image_file.tellg()/(2*sizeof(float));
image_file.seekg(0,std::ios::beg); //go to the beginning of file
input_values1=(float*)malloc(2*heap_alloc); //allocation of memory
input_values2=(float*)malloc(2*heap_alloc); //allocation of memory
output_values1=(float*)malloc(2*heap_alloc); //allocation of memory
output_values2=(float*)malloc(2*heap_alloc); //allocation of memory
tim=(float*)malloc(2*heap_alloc); //time
pulse1=(int*)malloc(2*heap_alloc); //allocation of memory
pulse2=(int*)malloc(2*heap_alloc); //allocation of memory
while(image_file>>tim[i])
{
		image_file >>input_values1[i];
		image_file >>input_values2[i];
		image_file>>pulse1[i];
		image_file>>pulse2[i];
		//cout << working_window[i] << " "; //read values
		i++;
}
//cout<<i;
array_size=i;
for(i=1;i<array_size;i++)
{
	if(abs(input_values1[i]-input_values1[i-1])>(float)10)
	{
	cout<<i<<" ";
	output_values1[i]=input_values1[i-1];
	
	}
	else
	{
	output_values1[i]=input_values1[i];
	}
}
for(i=1;i<array_size;i++)
{
	if(abs(input_values2[i]-input_values2[i-1])>(float)10)
	{
	cout<<i<<" ";
	output_values2[i]=input_values2[i-1];
	}
	else
	{
	output_values2[i]=input_values2[i];
	}
}
	//cout<<output_values[i]<<" ";
	for(i=0;i<array_size;i++)
{
	image_filee<<tim[i]<<"\t"<<output_values1[i]<<"\t"<<output_values2[i]<<"\t"<<pulse1[i]<<"\t"<<pulse2[i]<<"\n"; //writing output values
}
//cout<<"\n";
//cout<<count;
image_file.close(); //close input file
image_filee.close(); //close output file
getchar();
}

 





