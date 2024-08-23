/*
CMPS 3500 
Bakraa Mohammed 
Lab04 - Introduction to Java 
*/


import java.io.*;
import java.util.Scanner;


public class Erik_Soto_Lab04
{
   //-----------------------------------------------------------------
   //  Toss two coins until one of them comes up heads the requisite
   //  number of times in a row.
   //-----------------------------------------------------------------
   public static void main (String[] args) 
   {
      Integer cnt;     
      int heads=0,tails=0; 
      if (args.length == 0) 
      { 
          System.out.println("Please enter number of time to toss the coin: "); 
          Scanner userInput = new Scanner(System.in);
          cnt = userInput.nextInt();
      } 
      else {
         cnt = Integer.valueOf(args[0]);
      } 

      
      // Create two separate coin objects
      Erik_Soto_Coin coin = new Erik_Soto_Coin();
       for (int i=0; i < cnt; i++) {
            coin.toss();
           System.out.println(coin); 
            if (coin.isHeads())
               heads++;
            else   
            tails++; 
       } 

      // Display results

      System.out.println ("The number of tosses: "+ cnt);
      System.out.println ("The number of heads: "+ heads);
      System.out.println ("The number of tails: "+ tails +"\n");
      
      Erik_Soto_Compute cmp = new Erik_Soto_Compute(); 
      cmp.openFile();
      cmp.readFile();
      cmp.closeFile();

   }

}
