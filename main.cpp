/* Written by Arcoyle, who accidentally included his full name in the
previous link. This code takes image inputs and sews them together
where they are most similar in color on the edges (basically sews
them where they are similar. In a split image it recreates them, can also
be used to assemble square panaramas. Can be edited for other shapes.*/

#include "EasyBMP.h"
#include <cmath>
#include <iostream>
using namespace std;

// Filenames for input and output
const string INFILENAMES [16] = { "img1.bmp", "img2.bmp", "img3.bmp",
	"img4.bmp", "img5.bmp", "img6.bmp", "img7.bmp", "img8.bmp",
	"img9.bmp", "img10.bmp", "img11.bmp", "img12.bmp",
				  "img13.bmp", "img14.bmp", "img15.bmp", "img16.bmp"};


int rowMatch (BMP& UseTop, BMP& UseBottom)
{ // Compares the top edge of UseTop to the bottom edge of UseBottom.
  // Assumes UseTop and UseBottom are squares of same size
  // score obtained by adding the difference between color components
    int score = 0;
    int Width = UseTop.TellWidth();
    int Height = UseTop.TellHeight();
    for (int i = 0; i < Width; ++i) {
        RGBApixel Toppler = UseTop.GetPixel(i, 0);
        RGBApixel Bottler = UseBottom.GetPixel(i, Height-1);
        score += abs(Toppler.Red - Bottler.Red);
        score += abs(Toppler.Green - Bottler.Green);
        score += abs(Toppler.Blue - Toppler.Blue);
    }
    return score;
}

int columnMatch ( BMP& UseRight, BMP& UseLeft)
{
  // Compares the top edge of UseTop to the bottom edge of UseBottom.
  // Assumes UseRight and UseLeft are squares of same size
  // score obtained by adding the difference between color components
  // similar to the rowMatch
    int score = 0;
    int Width = UseRight.TellWidth();
    int Height = UseRight.TellHeight();
    for (int i = 0; i < Height; ++i) {
        RGBApixel Rightler = UseRight.GetPixel(0, i);
        RGBApixel Leftler = UseLeft.GetPixel(Width-1, i);
        score += abs(Rightler.Red - Leftler.Red);
        score += abs(Rightler.Green - Leftler.Green);
        score += abs(Rightler.Blue - Leftler.Blue);
    }
    return score;
}

void finalScore (BMP images[16], int score[2][16][16]) {  //REMOVED PASSING BY REFERENCE, CAUSED ERROR
// uses the rowMatch and columnMatch to create scores
// score[0][i][j] is the EAST SCORE and score[1][i][j] is the NORTH SCORE
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            score[0][i][j] = columnMatch(images[i], images[j]);
            score[1][i][j] = rowMatch(images[i], images[j]);
        }
    }
}

int findNorthWest(int score[2][16][16]) {
// finds the North West tile by adding the best North and West scores for each tile and
// choosing the one that maximizes the score
    int maxMinScore = 0;
    int idx;
    for(int i = 0; i < 16; i++)
    {
      int minNorth = 9999999;
      int minEast = 9999999;
      for(int j = 0; j < 16; ++j)
	{
          if(i == j)
	    {
	      continue;
	    }
	  minNorth = min(minNorth, score[1][i][j]);
	  minEast = min(minEast, score[0][i][j]);
	}
      int result = minNorth + minEast;
      if(result > maxMinScore)
	{
	  idx = i;
	  maxMinScore = result;
	}
    }
  return idx;
}

int findEastNeighbor(int score[2][16][16], int tile, bool remaining[16]) {
   // for a given tile, find its eastern neighbor among the remaining ones
   // remaining[j] is true for tiles that have not yet been placed in the final image
    /*int idx = 0; //holds the value and is returned
    int high = 99999999; //really high number for original comparison

    for (int i = 0; i < 16; i++) {
        if (score[0][i][idx] < high && remaining[i]){
            idx = i;
            high = score[0][i][idx];
        }
    }
    return idx;*/ //Broken, failed code.
    int idx = 0;
    int high = 9999999;

    for(int i = 0; i < 16; i++) {
        if (tile == i || !remaining[i]) {
            continue;
        }
        if(score[0][i][tile] < high) {
            high = score[0][i][tile];
            idx = i;
        }
    }

  return idx;
}

int findSouthNeighbor(int score[2][16][16], int tile, bool remaining[16]) {
   // for a given tile, find its southern neighbor, among the remaining ones
   // remaining[j] is true for tiles that have not yet been selected for placement
   // similar to findEastNeighbor
    /*int idx = 0; //holds the value and is returned
    int high = 99999999; //really high number for original comparison

    for (int i = 0; i < 16; i++) {
        if (score[0][i][idx] < high && remaining[i]){
            idx = i;
            high = score[0][i][idx];
        }
    }
    return idx;*/ //Broken, failed code.
    int idx = 0;
    int high = 9999999;

    for(int i = 0; i < 16; i++) {
        if (tile == i || !remaining[i]) {
            continue;
        }
        if(score[1][i][tile] < high) {
            high = score[1][i][tile];
            idx = i;
        }
    }

    return idx;
}

void copy(BMP & InImg, BMP & OutImg, int i, int j) {
  // copy image to larger final picture so that the InImg is placed in row i, column j of OutImg
    int Width = InImg.TellWidth();
    int Height = InImg.TellHeight();
    RangedPixelToPixelCopy(InImg, 0, Width-1, Height-1, 0, OutImg, i * Width, j * Height);
}

void greedy(int score[2][16][16], BMP InImg[16], BMP & OutImg) {
  //greedy algorithm to put the image together. Of course this failed, so the actualy
  //working code under this dysfunctional mess.
  
     /*bool remaining[16] = {true, true, true, true,
                           true, true, true, true,
                           true, true, true, true,for(int col = 0; col < InImg.TellWidth(); col++)
    {

      for(int row = 0; row < InImg.TellHeight(); row++)
		{
		int x = col + i * InImg.TellWidth();for(int col = 0; col < InImg.TellWidth(); col++)
    {

      for(int row = 0; row < InImg.TellHeight(); row++)
		{
		int x = col + i * InImg.TellWidth();
		int y = row + j * InImg.TellHeight();

	    OutImg.SetPixel(x,y,InImg.GetPixel(col, row));
		}

    }
		int y = row + j * InImg.TellHeight();

	    OutImg.SetPixel(x,y,InImg.GetPixel(col, row));
		}

    }
                           true, true, true, true};
     int northWest = findNorthWest(score); //finding the index for the upper left image
     int x = 0;
     int y =0;
     int r;
     int prevX;
     int prevY;

     remaining[northWest] = false;
     copy(InImg[northWest], OutImg, 0, 0);
     prevX = northWest;
     prevY = prevX;

     for (int m = 1; m < 5; m++)
     {
         for (int n = 1; n < 5; n++)
         {
             if (m == 1 && n == 1)
             {
             }
             else if (n > 1)
             {
                   r = findEastNeighbor(score, prevX, remaining);
                   remaining[r] = false;
                   prevX = r;
                   x = x + InImg[0].TellWidth();
                   copy(InImg[r], OutImg, x,y);
             }
             else if (n == 1)
             {
                  x = 0;
                  y = y + InImg[0].TellHeight();
                  r = findSouthNeighbor(score, prevY, remaining);
                  remaining[r] = false;
                  prevX = r;
                  prevY = prevX;
                  copy(InImg[r], OutImg , x,y);
             }
         }
     }*/ //Well, that sucked. This will be better.
    bool remaining[16];
    for(int i = 0; i <16; ++i) {
      remaining[i] = true;
    }
    int topLeft =  findNorthWest(score);
    copy(InImg[topLeft], OutImg, 0, 0);
    remaining[topLeft] = false;
    int lastEast = topLeft;
    int lastNorth = topLeft;
    for(int row = 0; row < 4; row++) {
      for(int col = 0; col < 4; col++) {
            if(row == 0 && col == 0) {
                continue;
            }
            else if(row == 0 && col != 0) {
                int east = findEastNeighbor(score, lastEast, remaining);
                copy(InImg[east], OutImg, col, row);
                remaining[east] = false;
                lastEast = east;
            }
            else if (row > 0 && col == 0){
                int south = findSouthNeighbor(score, lastNorth, remaining);
                copy(InImg[south], OutImg, col, row);
                remaining[south] = false;
                lastNorth = lastEast = south;
            }
            else if(row > 0 && col != 0) {
                int east = findEastNeighbor(score, lastEast, remaining);
                copy(InImg[east], OutImg, col, row);
                remaining[east] = false;
                lastEast = east;
            }
        }

    }


}


int main()
{
  BMP InImg[16], OutImg; // vector of input images and output image
  int score [2][16][16] ;       // holds EAST and NORTH scores
  for( int i=0; i<16; ++i ) { // Read in the sub-images
    InImg[i].ReadFromFile( INFILENAMES[i].c_str());
  }
  int subsize = InImg[0].TellWidth();
  OutImg.SetSize( 4*subsize, 4*subsize ); // Set size of output image
  finalScore(InImg, score);
  greedy(score, InImg, OutImg);
  OutImg.WriteToFile("output.bmp");

  return 0;
 }


