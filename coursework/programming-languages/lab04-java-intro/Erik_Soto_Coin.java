/*
CMPS 3500 
Erik Soto
Lab04 - Introduction to Java 
*/


//********************************************************************
//  Coin.java     
//  Project:  a class that simulates a coin toss  - use this class for
//  lab04 without modifaction 
//********************************************************************

import java.util.Random;

public class Erik_Soto_Coin
{
   private final int HEADS = 0;
   private final int TAILS = 1;

   private int face;

   public Erik_Soto_Coin ()
   {
      toss();    // initialize the coin 
   }

   public void toss ()
   {
      face = (int) (Math.random() * 2);  // a random toss
   }

   public boolean isHeads ()
   {
      return (face == HEADS);
   }

   // the toString method is automatically called when an object name is used
   // as an operand of the string concatenation operator (+) or as an argument
   // in a print method 

   public String toString()
   {
      String faceName;

      if (face == HEADS)
         faceName = "Heads";
      else
         faceName = "Tails";

      return faceName;
   }
}
