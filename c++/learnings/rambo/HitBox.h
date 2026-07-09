#ifndef HITBOX_H
#define HITBOX_H
//cx and cy in walk.cpp
class HitBox{
  private:
    int top;
    int bottom;
    int left;
    int right;

  public:
    //getter & setter
    int getTop() const;
    int getBottom() const;
    int getLeft() const;
    int getRight() const;
    void setTop(int t);
    void setBottom(int b);
    void setLeft(int l);
    void setRight(int r);
    //other member functions
    HitBox(int top, int bottom, int left, int right);
    void updateHitBox(int top, int bottom, int left, int right);
    void draw();
    void draw(float, float, float);
};

#endif
