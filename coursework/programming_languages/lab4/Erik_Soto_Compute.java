/*
CMPS 3500 
Erik_Soto
Lab04 - Introduction to Java 
*/


import java.io.*;     // import everything having to do with I/O
import java.util.*; 
import java.util.Formatter; 

public class Erik_Soto_Compute
{
    private String x = String.format("%10s %10s", "Name:", "Santa Clause CMPS 3500 Lab04"); 
    private String y = String.format("%10s %10s %10s %10s", "Exam1", "Exam2", "Exam3", "Exam4");
    private String z = String.format("%10s %10s %10s %10s", "=====", "=====", "=====", "=====");
    private Scanner s; 
    private int e1=0,e2=0,e3=0,e4=0, calcz1=0,calcz2=0,calcz3=0,calcz4=0;   
    private String ex1,ex2,ex3,ex4; 
    private int size; 
    public void openFile(){
        try {
                s = new Scanner(new File("lab04.txt")); 
        } 
        catch (Exception e) 
        { 
            System.out.println("Could not find file"); 
        }
    } 
    public void readFile() { 
    Formatter formatter=null;
    File file = new File ("out.file");                  //write to out.file using Formatter

    try {
        formatter = new Formatter(file); 
    } 
    catch (FileNotFoundException e) {}
    formatter.format ("%10s %10s %n", "Name:", "Santa Clause CMPS 3500 3500 Lab04");
    formatter.format("%10s %10s %10s %10s %n", "Exam1", "Exam2", "Exam3", "Exam4");
    formatter.format("%10s %10s %10s %10s %n", "=====", "=====", "=====", "=====");
    
        System.out.println(x);
        System.out.println(y);
        System.out.println(z);
        while(s.hasNext()) { 
            ex1 =  s.next();        // read 
            e1+= Integer.parseInt(ex1); // store 
             if (Integer.parseInt(ex1)==0) 
                calcz1++;
            ex2 =  s.next(); 
            e2+= Integer.parseInt(ex2); 
             if (Integer.parseInt(ex2)==0) 
                calcz2++;
            ex3 =  s.next(); 
            e3+= Integer.parseInt(ex3); 
             if (Integer.parseInt(ex3)==0) 
                calcz3++;
            ex4 =  s.next(); 
            e4+= Integer.parseInt(ex4); 
             if (Integer.parseInt(ex4)==0) 
                calcz4++;
            size++;
             String output = String.format("     %-3s %9s %10s %10s", ex1,ex2,ex3,ex4); 
             System.out.println(output);
             formatter.format("     %-3s %9s %10s %10s %n", ex1,ex2,ex3,ex4);
        }
        e1=e1/ (size-calcz1);    //to get the mean of exam1-exam4
        e2=e2/ (size-calcz2);    
        e3=e3/ (size-calcz3);
        e4=e4/ (size-calcz4);
        System.out.println(calcz3);
            System.out.println(z); 
            formatter.format("%10s %10s %10s %10s %n", "=====", "=====", "=====", "=====");
            System.out.format("%s %3d %10d %10d %10d","Mean:", e1,e2,e3,e4);
            System.out.println();
            formatter.format("%s %3d %10d %10d %10d %n","Mean:", e1,e2,e3,e4);
    
    formatter.flush(); 
    formatter.close(); 


    }
   public void closeFile() { 
    s.close();
   }  

}

