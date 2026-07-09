typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
void renderBackground(float s, float sh, float w, float h );
void pauseScreen();
void renderlevel();
void teleportCheck();
void erikRender();
void newGame();
void erikInit();
void renderDeath();
void platformPhysics();
void checkKeysDeath();
void Lives(int xres, int yres);
 class Level {

    public:

        unsigned char arr[21][700];
        int nrows, ncols;
        int dynamicHeight[180];
        int tilesize[2];
        Flt ftsz[2];
        Flt tile_base;
        Level();

        void removeCrLf(char *str);
	void renderlevel();
};
class Platform {
	class point{
	    public:
		float x,y;
	};
	class color {
	public: 
	    float r,g,b;
	};
    public:
	point one,two,three,four;
	color set;
	int top;
	int bottom;
	int left;
	int right;
	int centerx;
	int centery;
	Platform(
		float xpos, float ypos,
		float xpos2, float ypos2,
		float xpos3, float ypos3,
		float xpos4, float ypos4);
	void drawPlatform();
	void setColor(float r, float g, float b);
	float greatest(float one,float two, float three, float four);
	float greatest(float one, float two);
	float least(float one,float two, float three, float four);
	float least(float one, float two);
};

