/*Libraries Used*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#define p printf
#define s scanf
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"
#include <string.h>
#include <time.h>
#include<math.h>

//#ifndef HPDF_NOPNGLIB

jmp_buf env;
time_t rawtime;
struct tm * timeinfo;
float *input_values1;   // input values array
float *input_values2;   // input values array
float *diff1;  		// input values array
float *diff2;
float *tim; 		//time array
int *pulseOD;
int *pulseOS;
char patdat[50];
int array_size; 	//size of arrays
int count=0;
long pos;
int textsize1=15;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,			//Error Handling
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

HPDF_Doc  pdf;
static const int PAGE_WIDTH = 595;	//Declaring Page Heights
static const int PAGE_HEIGHT = 845;
static const int CELL_WIDTH = 52;
static const int CELL_HEIGHT = 18;
static const int CELL_DOWN = 470;
static const int CELL_TOP = 630;
int col[]={40,150,207,265,322,380};
char fname[256];
void
draw_graph (HPDF_Page   page1);

using namespace std;

void
draw_graph (HPDF_Page   page1)
{
HPDF_Font font1;
font1 = HPDF_GetFont (pdf, "Helvetica", NULL);  //Declaring Font for template 
    char buf[50];
    int i;
    
    /* Draw 16 X 15 cells */
  
    /* Draw vertical lines. */
    HPDF_Page_SetLineWidth (page1, 0.5);
	
    HPDF_Page_MoveTo (page1, 11, CELL_TOP);				//Column 1
    HPDF_Page_LineTo(page1, 11, CELL_DOWN);
    HPDF_Page_Stroke(page1);
    
    HPDF_Page_MoveTo (page1, 11+CELL_WIDTH-10, CELL_TOP);		//Column 2
    HPDF_Page_LineTo(page1, 11+CELL_WIDTH-10, CELL_DOWN);
    HPDF_Page_Stroke(page1);
    
    HPDF_Page_MoveTo (page1, 11+2*CELL_WIDTH+5, CELL_TOP);		//Column 3
    HPDF_Page_LineTo(page1, 11+2*CELL_WIDTH+5, CELL_DOWN);
    HPDF_Page_Stroke(page1);
	
    for (i=3;i<12;i++){
    HPDF_Page_MoveTo (page1, 11+i*CELL_WIDTH, CELL_TOP);		//Rest of the Columns
    HPDF_Page_LineTo(page1, 11+i*CELL_WIDTH, CELL_DOWN);
    HPDF_Page_Stroke(page1);
    }
     /* Draw horizontal lines. 
    for (i = 0; i <= 9; i++) {

        HPDF_Page_MoveTo (page1, 11+CELL_WIDTH-10, CELL_DOWN+i*CELL_HEIGHT);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 12, CELL_DOWN+i*CELL_HEIGHT);
        HPDF_Page_Stroke (page1);*/
        
    /*Draw Horizontal Dividers for OD and OS */
    for (i = 0; i <= 2; i++) {

        HPDF_Page_MoveTo (page1, 11, CELL_DOWN+i*4*CELL_HEIGHT);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 12, CELL_DOWN+i*4*CELL_HEIGHT);
        HPDF_Page_Stroke (page1);
    }
    
    /*Draw Horizontal Divider for tabs*/
    	HPDF_Page_MoveTo (page1, 11, CELL_TOP);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 12, CELL_TOP);
        HPDF_Page_Stroke (page1);
    
    /*Writing Baseline and Stimulus tabs*/
    	HPDF_Page_SetFontAndSize (page1, font1, 18);
        HPDF_Page_BeginText (page1);	
        HPDF_Page_MoveTextPos (page1, 11+9, CELL_DOWN+4*CELL_HEIGHT+CELL_HEIGHT+10);	//Printing OD
        HPDF_Page_ShowText (page1, "OD");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, 11+9, CELL_DOWN+CELL_HEIGHT+10);			//Printing OS
        HPDF_Page_ShowText (page1,"OS");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 10);
        HPDF_Page_BeginText (page1);							//Printing Baseline
        HPDF_Page_MoveTextPos (page1, 11+2*CELL_WIDTH+10, CELL_TOP-11);
        HPDF_Page_ShowText (page1,"Baseline");
        HPDF_Page_EndText (page1);
                   
    for (i=3; i<11;i++){
   	HPDF_Page_SetFontAndSize (page1, font1, 10);
        HPDF_Page_BeginText (page1);							//Printing Stimulus -
        HPDF_Page_MoveTextPos (page1, 11+i*CELL_WIDTH+10, CELL_TOP-11);
        HPDF_Page_ShowText (page1,"S   (B) ");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);							//Printing stimulus no.
        snprintf(buf, 5, "%X", i-2);
        HPDF_Page_MoveTextPos (page1, 11+i*CELL_WIDTH+18, CELL_TOP-11);
        HPDF_Page_ShowText (page1,buf);
        HPDF_Page_EndText (page1);
        /*
        HPDF_Page_SetFontAndSize (page1, font1, 8);					//Printing Percentage symbol OD
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, 11+i*CELL_WIDTH+35, CELL_TOP-2*CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,"%");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, 11+i*CELL_WIDTH+35, CELL_TOP-6*CELL_HEIGHT-11);	//Printing Percentage symbol OS
        HPDF_Page_ShowText (page1,"%");
        HPDF_Page_EndText (page1);*/
     }
       											//Radius (px)
        for(i=0;i<2;i++){
        HPDF_Page_SetFontAndSize (page1, font1, 10);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH, CELL_TOP-CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"Radius  (px)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);								//Change
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"Change (%)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);									//T1
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH, CELL_TOP-3*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"T1        (ms)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);									//T2
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH, CELL_TOP-4*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"T2        (ms)");
        HPDF_Page_EndText (page1);
        
        /*HPDF_Page_SetFontAndSize (page1, font1, 12);  							//Printing zero in baseline
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2, CELL_TOP-CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"0");
        HPDF_Page_EndText (page1);*/
        
        HPDF_Page_SetFontAndSize (page1, font1, 12);  							//Printing "-" in change
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2, CELL_TOP-2*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"--");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 12);  							//Printing "-" in T1
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2, CELL_TOP-3*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"--");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 12);  							//Printing "-" in T2
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2, CELL_TOP-4*CELL_HEIGHT-CELL_HEIGHT*i*4-11);
        HPDF_Page_ShowText (page1,"--");
        HPDF_Page_EndText (page1);
        
        }             
}

int main (int argc, char **argv)
{
char times[100];
HPDF_Image image;
HPDF_Image image1;

time (&rawtime);
timeinfo = localtime (&rawtime);

pdf = HPDF_NewEx (error_handler, NULL, NULL, 0, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }
    HPDF_Font font;

	font = HPDF_GetFont (pdf, "Helvetica", NULL);
		
	HPDF_Page page1 = HPDF_AddPage (pdf);
        HPDF_Page_SetSize(page1, HPDF_PAGE_SIZE_A4,HPDF_PAGE_PORTRAIT);
        
        /*Adding the LVPEI Logo*/	
        image1 = HPDF_LoadJpegImageFromFile (pdf, "../Data/logo.jpg");		//Logo	
	HPDF_Page_DrawImage (page1, image1, 20,735,90,70);
        
        HPDF_Page_SetFontAndSize (page1, font, 40);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,130,750);				//Pupillometry Report
        HPDF_Page_ShowText (page1,"Pupillometry Report");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetLineWidth (page1, 1);
        HPDF_Page_MoveTo (page1, 11, 730);				//Horizontal Separator Heading
        HPDF_Page_LineTo (page1, PAGE_WIDTH-12, 730);
        HPDF_Page_Stroke (page1);
        
        HPDF_Page_SetFontAndSize (page1, font, 15);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,70,700);				//Name
        HPDF_Page_ShowText (page1,"Name :");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,70,670);				//Age
        HPDF_Page_ShowText (page1,"Age    :");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,330,670);				//Sex
        HPDF_Page_ShowText (page1,"Sex :");
        HPDF_Page_EndText (page1);

        HPDF_Page_MoveTo (page1, 11, 650);	
        HPDF_Page_LineTo (page1, PAGE_WIDTH-12, 650);			//Horizontal Separator Data Table
        HPDF_Page_Stroke (page1);

        HPDF_Page_MoveTo (page1, 11, 455);	
        HPDF_Page_LineTo (page1, PAGE_WIDTH-12, 455);			//Horizontal Separator Data Graph
        HPDF_Page_Stroke (page1);
       
        HPDF_Page_MoveTo (page1, 11, 50);	
        HPDF_Page_LineTo (page1, PAGE_WIDTH-12, 50);			//Horizontal Separator Footer
        HPDF_Page_Stroke (page1);
        
        HPDF_Page_SetFontAndSize (page1, font, 12);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,50,180);				//Remarks :
        HPDF_Page_ShowText (page1,"Remarks :");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font, 15);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,390,65);				//Ophthalmologist Signature
        HPDF_Page_ShowText (page1,"Ophthalmologist Signature");
        HPDF_Page_EndText (page1);
        
        /*Legend*/
        HPDF_Page_SetFontAndSize (page1, font, 8);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11,35);				//Legend 1
        HPDF_Page_ShowText (page1,"OD : Right Eye                     OS : Left Eye");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11,20);				//Legend 1
        HPDF_Page_ShowText (page1,"T1 : Rise Time                      T2 : Recovery Time");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,200,35);				//Legend 2
        HPDF_Page_ShowText (page1,"S1 :  50 lux (OS)                     S2 :  50 lux (OD)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,410,35);				//Legend 2
        HPDF_Page_ShowText (page1,"S3 : 120 lux (OS)                      S4 : 120 lux (OD)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,200,20);				//Legend 2
        HPDF_Page_ShowText (page1,"S5 : 240 lux (OS)                    S6 : 240 lux (OD)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,410,20);				//Legend 2
        HPDF_Page_ShowText (page1,"S7 : 360 lux (OS)                      S8 : 360 lux (OD)");
        HPDF_Page_EndText (page1);
        
        HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

float min, sumOS=0, sumOD=0;
int mink,minj;
int j=0;
int k=0;
int i=0;
int i1,count,plf1=0;
float t1d,t2d,t1s,t2s,t1dd;
int t=0;
float decOD,decOS;
int heap_alloc;
int window=15;
float base1,base2;
int age;
char gend[10];

ifstream image_file("../Data/radtime.csv", std::ios_base::in); //reading the input file containing readius vs  time
image_file.seekg(0,std::ios::end); 				//go to end of file to find size

heap_alloc=image_file.tellg()/2;

ifstream image_file1("../Data/data.csv",std::ios_base::in); 	//Reading the patient's data file
ofstream outfile("../Data/Record.csv",ios_base::app);		//Recording patients data in a csv

image_file.seekg(0,std::ios::beg); 		//go to the beginning of file
input_values1=(float*)malloc(2*heap_alloc);	//allocation of memory
input_values2=(float*)malloc(2*heap_alloc); 	//allocation of memory
diff1=(float*)malloc(2*heap_alloc); 		//allocation of memory
diff2=(float*)malloc(2*heap_alloc);
pulseOD=(int*)malloc(2*heap_alloc); 		//allocation of memory
pulseOS=(int*)malloc(2*heap_alloc); 		//allocation of memory
tim=(float*)malloc(2*heap_alloc);
float ros, rod;
float rad=1.57;
i=0;
char patdat1[20];
image_file1.seekg(0,std::ios::beg);
image_file1>>patdat;			//Get Patient's First Name
image_file1>>patdat1;			//Get Patient's Sur name

strcat(patdat," ");
strcat(patdat, patdat1);

image_file1>>age;			//Patients age
image_file1>>gend;			//Patients Gender

	outfile<<"\n"<<patdat<<"\t"<<age<<"\t"<<gend<<"\t"<<"\t";			//Recording patients detials 
		
	HPDF_Page_SetFontAndSize (page1, font, 15);
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,140,700);		//Name Value
        HPDF_Page_ShowText (page1,patdat);
        HPDF_Page_EndText (page1);
        
        snprintf(times, 5, "%d",age);
        HPDF_Page_BeginText (page1);			// Age Value
        HPDF_Page_MoveTextPos (page1,140,670);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);			// Sex Value
        HPDF_Page_MoveTextPos (page1,380,670);
        HPDF_Page_ShowText (page1,gend);
        HPDF_Page_EndText (page1);
i=0;
while(image_file>>tim[i]){
		image_file>>input_values1[i];		 //Data for OD
		image_file>>input_values2[i]; 		//Data for OS
		image_file>>pulseOS[i];	     		//Pulse OD
		image_file>>pulseOD[i];      		//Pulse OS
		i++;
}

array_size=i;
for (i=0;i<50;i++){
  sumOS=input_values1[i]+sumOS;
  sumOD=input_values2[i]+sumOD;
}
base1=sumOS/50;
base2=sumOD/50;

	HPDF_Page_SetFontAndSize (page1, font, 8);
	snprintf(times, 5, "%F", base1);

	HPDF_Page_BeginText (page1);			//Baseline OS
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2-4, CELL_TOP-CELL_HEIGHT*5-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        outfile<<base1<<"\t";
        
        snprintf(times, 5, "%F", base2);

	HPDF_Page_BeginText (page1);			//Baseline OD
        HPDF_Page_MoveTextPos (page1,11+2*CELL_WIDTH+CELL_WIDTH/2-4, CELL_TOP-CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        outfile<<base2<<"\t"<<"\t";
plf1=0;
i1=-1;
for(count=0;count<8;count++){
for(i=0;i<array_size;i++){ 				//Finding when the pulseOS or pulseOD has gone high
  if(pulseOS[plf1] || pulseOD[plf1]){
     t=tim[plf1];
     cout<<t<<"\n";
     j=plf1;
     k=plf1;
     i1++;
     if(pulseOS[plf1]){
     cout<<"hi";
     base1=input_values1[plf1];
     base2=input_values2[plf1];
     }
     else if (pulseOD[plf1]){
     base2=input_values2[plf1];
     base1=input_values1[plf1];
     }
     break;
  }plf1++;
  
}

/*switch(pulseOD[plf1]){
case 10 : {i1=1;
	   break;}
case 20 : {i1=3;
	   break;}
case 30 : {i1=5;
	   break;}
case 40 : {i1=7;
	   break;}
default : break;
}

switch(pulseOS[plf1]){
case 10 : {i1=0;
	   break;}
case 20 : {i1=2;
	   break;}
case 30 : {i1=4;
	   break;}
case 40 : {i1=6;
	   break;}
default : break;
}*/

plf1=plf1+10;

/*Finding time T1 and radius for OS*/
for(i=0;i<=window;i++){
  diff1[j]=input_values1[j]-base1;
  if(abs(diff1[j])>=2){
     break;   
  }
  j++;
}
min=10000;
minj=j;
for (i=0;i<=10;i++){
  if(input_values1[j]<min){
  
    min=input_values1[j];
    minj=j;
    }j++;
}
t1s=tim[minj]-t;			

	snprintf(times, 5, "%F", input_values1[minj]);	//Printing radius for stimulus 1 OS

	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-20+(i1+3)*CELL_WIDTH, CELL_TOP-5*CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        ros=input_values1[minj];
        
        snprintf(times, 5, "%F", base1);		//Print Baselines OS
    	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2+3+(i1+3)*CELL_WIDTH, CELL_TOP-5*CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);				//printing ( )
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2+(i1+3)*CELL_WIDTH, CELL_TOP-5*CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,"(       )");
        HPDF_Page_EndText (page1);
        
        decOS=(base1-input_values1[minj])*100/base1;	// Percentage Change for OS
        snprintf(times, 5, "%F", decOS);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        if(decOS<=0){
        	HPDF_Page_SetRGBStroke (page1, 1.0, 0.0, 0.0);	//Red Arrow in change field
        	
        	HPDF_Page_SetLineWidth (page1, 0.5);
    		HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+10+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-8);				
     		HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-5);
        	HPDF_Page_Stroke (page1);
        
        	HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+14+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-8);				
        	HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-5);
        	HPDF_Page_Stroke (page1);
        }
        
        else{
        	HPDF_Page_SetRGBStroke (page1, 0.0, 1.0, 0.0);	//Green Arrow in change field
        	
        	HPDF_Page_SetLineWidth (page1, 0.5);
    		HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+10+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-8);				
     		HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-11);
        	HPDF_Page_Stroke (page1);
        
        	HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+14+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-8);				
        	HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-11);
        	HPDF_Page_Stroke (page1);
        }
        
       	HPDF_Page_SetLineWidth (page1, 0.5);
        HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-5);				
        HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-6*CELL_HEIGHT-11);
        HPDF_Page_Stroke (page1);
        
	HPDF_Page_SetRGBStroke (page1, 0.0, 0.0, 0.0);
	snprintf(times, 5, "%F", t1s);
	HPDF_Page_BeginText (page1);			//Printing T1s
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT*7-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        outfile<<base1<<"\t";
	outfile<<t1s<<"\t";
	
/*Finding time T1 for OD*/
for(i=0;i<=window;i++){
  diff2[k]=input_values2[k]-base2;
  if(abs(diff2[k])>=2){
     break;
  }
  k++;
}
//cout<<k<<"\n";
min=10000;
mink=k;
for (i=0;i<=10;i++){
  if(input_values2[k]<min){
    min=input_values2[k];
    mink=k;
    }  k++;
}

t1d=tim[mink]-t;
cout<<t1d<<"\n";
t1dd=t1d;
	snprintf(times, 5, "%F", input_values2[mink]);		//Printing radius for stimulus 1 OD

	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-20+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        rod=input_values2[mink];
        
        snprintf(times, 5, "%F", base2);
        HPDF_Page_BeginText (page1);				//Printing baselines for OD
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2+3+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);				//printing ( )
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT-11);
        HPDF_Page_ShowText (page1,"(       )");
        HPDF_Page_EndText (page1);
        
        decOD=(base2-input_values2[mink])*100/base2;	      // Percentage Decrease OD
        snprintf(times, 5, "%F", decOD);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT*2-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        if(decOD<=0){
        	HPDF_Page_SetRGBStroke (page1, 1.0, 0.0, 0.0);	//Red Arrow in change field
        	
        	HPDF_Page_SetLineWidth (page1, 0.5);
    		HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+10+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-8);				
     		HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-5);
        	HPDF_Page_Stroke (page1);
        
        	HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+14+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-8);				
        	HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-5);
        	HPDF_Page_Stroke (page1);
        }
        
        else{
        	HPDF_Page_SetRGBStroke (page1, 0.0, 1.0, 0.0);	//Green Arrow in change field
        	
        	HPDF_Page_SetLineWidth (page1, 0.5);
    		HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+10+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-8);				
     		HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-11);
        	HPDF_Page_Stroke (page1);
        
        	HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+14+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-8);				
        	HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-11);
        	HPDF_Page_Stroke (page1);
        }
        
        HPDF_Page_SetLineWidth (page1, 0.5);
        HPDF_Page_MoveTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-5);				
        HPDF_Page_LineTo (page1, 11+CELL_WIDTH/2+12+(i1+3)*CELL_WIDTH, CELL_TOP-2*CELL_HEIGHT-11);
        HPDF_Page_Stroke (page1);
        
	HPDF_Page_SetRGBStroke (page1, 0.0, 0.0, 0.0);        
	snprintf(times, 5, "%F", t1d);				//Printing t1d
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT*3-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
/*Finding T2 for OS*/
k=mink;
j=minj;

for(i=0;i<=10;i++){			//Finding the section which is less than baseline by 5 OS
  diff1[j]=input_values1[j]-base1;
  if(abs(diff1[j])<=3){
     break;
  }
  j++;
}

t1d=tim[mink];
t1s=tim[minj];
min=0;					//Finding the max value in a window of 10 surrounding that OS
minj=j;
for (i=0;i<=20;i++){
  if(input_values1[j]>min){
    min=input_values1[j];
    minj=j;
    }j++;
}
t2s=tim[minj]-t1s;
cout<<t2s<<"\n";
snprintf(times, 5, "%F", t2s);

	HPDF_Page_BeginText (page1);			//Printing t2s
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT*8-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        outfile<<t2s<<"\t";
        outfile<<ros<<"\t";
        outfile<<decOS<<"\t"<<"\t";
        outfile<<base2<<"\t";
        outfile<<t1dd<<"\t";

/*Finding T2 for OD*/
for(i=0;i<=20;i++){
  diff2[k]=input_values2[k]-base2;		//Finding the section which is less than baseline by 5 OD
  if(abs(diff2[k])<=4){
     break;
  }
  k++;
}

min=0;
mink=k;
for (i=0;i<=20;i++){
  if(input_values2[k]>min){			//Finding the max value in a window of 10 surrounding that OD
    min=input_values2[k];
    mink=k;
    }k++;
}
	t2d=tim[mink]-t1d;
	cout<<t2d<<"\n";
	snprintf(times, 5, "%F", t2d);				//Printing t2d
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,11+CELL_WIDTH/2-8+(i1+3)*CELL_WIDTH, CELL_TOP-CELL_HEIGHT*4-11);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        outfile<<t2d<<"\t";
        outfile<<rod<<"\t";
        outfile<<decOD<<"\t"<<"\t";
}

/*Appending Graph*/
image = HPDF_LoadPngImageFromFile (pdf, "../Data/output.png");
HPDF_Page_DrawImage (page1, image, 30,210,535,240);

	HPDF_Page_SetFontAndSize (page1, font, 10);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,275,200);				//Time Axis
        HPDF_Page_ShowText (page1,"Time (ms)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,260,230);				//Radius Axis
    	HPDF_Page_SetTextMatrix (page1, cos(rad), sin(rad), -sin(rad), cos(rad),40, 310);
    	HPDF_Page_ShowText (page1,"Radius (px)");
        HPDF_Page_EndText (page1);
        
/*Saving File in pdf*/
    
    strcpy (fname, "../Reports/");
    strcat (fname, patdat);
        HPDF_Page_SetFontAndSize (page1, font, textsize1);
        draw_graph (page1);					//Drawing the Graph
	
	HPDF_Page_SetFontAndSize (page1, font, 10);
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,470, 810);
        HPDF_Page_ShowText (page1,asctime(timeinfo));		//Timestamp
        HPDF_Page_EndText (page1);
        
    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);
	//fclose(outfile);
    /* clean up */
    HPDF_Free (pdf);
    image_file.close(); //close input file
    outfile.close();
    return 0;
}

