/* lab14.cs
 * C# lab10
 Erik Soto 
 */

using System;

namespace Assignment {
  public class Application  {
    static void Main() {
      double rectWidth = 8.75f, rectHeight = 20.5f;
      IShape shape1 = new Rectangle(rectWidth, rectHeight);
      
      double triBase = 5.5f, triHeight = 12.25f;
      IShape shape2 = new Triangle(triBase, triHeight);

      double cirRadius = 5.5f;
      Circle shape3 = new Circle(cirRadius);
      // test Radius property
      shape3.Radius = 3.5f;
      Console.WriteLine("My Radius is '{0}'\t: ", shape3.Radius);
      
      Console.WriteLine("area of type '{0}'\t: {1}", shape1, shape1.Area);
      Console.WriteLine("area of type '{0}'\t: {1}", shape2, shape2.Area);  
      Console.WriteLine("area of type '{0}'\t: {1}", shape3, shape3.Area);
      shape1.Display();
      shape2.Display();
      shape3.Display();
    }
  }

  public interface IShape {
     void Display ();
      
     double Area {
       get;  
     }
  }

  ////////////////////////////////////////////////////////////////////////
  public class Rectangle: IShape {
     public Rectangle(double inWidth, double inHeight) {
       m_Width = inWidth;
       m_Height = inHeight;
     }
    
     public double Area {
       get {
         return m_Width * m_Height;
       }
     }

     public void Display() {
         Console.WriteLine(
        "I'm a rectangle!I have width {0} and height {1}", m_Width, m_Height);
     }

     private double m_Width = 0;
     private double m_Height = 0;
  }
  ////////////////////////////////////////////////////////////////////////
  public class Triangle: IShape {
    public Triangle(double inBase, double inHeight) {
       m_Base = inBase;
       m_Height = inHeight;
    }

    public double Area {
      get {
        double area = 0.5f * m_Base * m_Height;
        return area;
      }
    }

    public void Display() {
        Console.WriteLine(
         "I'm a triangle!\tI have base {0} and height {1}", 
           m_Base, m_Height);
    }

    private double m_Base = 0;
    private double m_Height = 0;
  }

  ////////////////////////////////////////////////////////////////////////
  public class Circle: IShape {
    public Circle(double inRadius) {
       m_Radius = inRadius;
    }

    public double Radius {
      get {
        return m_Radius;
      }
      set {
        m_Radius = value;
      }
    }

    public double Area {
      get {
        double area = 3.14 * (m_Radius * m_Radius);
        return area;
      }
    }

    public void Display() {
        Console.WriteLine("I'm a Circle!\tI have radius {0}", m_Radius);
    }

    private double m_Radius = 0;
  } 


} // end namespace 
