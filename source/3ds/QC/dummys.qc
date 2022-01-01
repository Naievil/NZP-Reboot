// Dummys that should be here
void GiveAchievement(float achievement_id, optional entity who) = {};

#define MOVE_HITMODEL 0

void() LoadWaypointData = {};

.float punchangle_x;
.float punchangle_y;

// 2001-09-20 QuakeC string manipulation by FrikaC  start
float(string s) strlen = {return 0;};
string(string s1, string s2) strcat = {return "";};
string(string s, float start, float length) substring = {return "";};
vector(string s) stov = {return '0 0 0';};
string(string s) strzone = {return "";};
string(string s) strunzone =  {return "";};
string(string s) strtrim =  {return "";};
// 2001-09-20 QuakeC string manipulation by FrikaC  end

// 2001-11-15 DarkPlaces general builtin functions by Lord Havoc  start
float(string s) tokenize = {return 0;};
string(float num) argv =  {return "";};
// 2001-11-15 DarkPlaces general builtin functions by Lord Havoc  end


// 2001-09-20 QuakeC file access by FrikaC  start
float(string filename, float mode) fopen = {return 0;};
void(float fhandle) fclose = {};
string(float fhandle) fgets = {return "";};
void(float fhandle, string s) fputs = {};
// 2001-09-20 QuakeC file access by FrikaC  end

#define		FILE_READ 	 	0
.float currentHitBoxSetup;
.entity larm;
.entity head;
.entity rarm;
.vector		bbmins, bbmaxs;		// Used for zombie hitboxes
.float		zoom;
.float facingenemy;

entity (entity start, .float field, float match) findfloat = {return world;}


.string Weapon_Name;
.vector Flash_Offset;
.float 		Flash_Size;
.float button7;
.float button3;
.float button4;
.float button5;
.float button6;
.float button7;
.float button8;
.float button9;

void() Soft_Restart = {};
void(vector o, vector d, float color, float count) particle = {};

void(entity client, float type, float cost, float weapon) useprint = {};

.string		Weapon_Name_Touch;

#define	EF_BLUE					1
#define	EF_MUZZLEFLASH 			2
#define	EF_BRIGHTLIGHT 			4
#define	EF_RED		 			8
#define	EF_ORANGELIGHT			16
#define	EF_GREEN				32
#define	EF_LIGHT				64
#define	EF_NODRAW				128
#define EF_BRIGHTFIELD			256
#define EF_FULLBRIGHT 			512
#define EF_DARKLIGHT			1024
#define EF_DARKFIELD			2048
#define EF_PURPLELIGHT    		4096
#define EF_RAYRED		 	8196
#define EF_RAYGREEN		 	16384

void() Do_Zombie_A = {};
void() func_door_nzp;
void() mystery_box;

entity windows[32];
float wincnt;

float(entity zombie, entity target) Do_Pathfind_psp = {return 0;};
void(string s) Open_Waypoint = {};
vector(entity zombie,vector start,vector min, vector max) Get_Next_Waypoint = {return '0 0 0';};
vector(entity zombie,vector start,vector min, vector max) Get_First_Waypoint = {return '0 0 0';};
void(string s) Close_Waypoint = {};
.vector		ADS_Offset;
#define	SVC_HITMARK				 36



void () CL_SendWeaponFire =
{

}

void() ParseClientCommand = {}

void() PutClientInServer;
void() InitRounds;

//moto -- put this here because it keeps soft_restart somewhat clean..
void(entity door) reclose_door = {

}

vector(entity what) Get_Waypoint_Near = {return '0 0 0';};

#define	SOLID_NOT				  0	// no interaction with other objects
#define	SOLID_TRIGGER			  1	// touch on edge, but not blocking
#define	SOLID_BBOX				  2	// touch on edge, block
#define	SOLID_SLIDEBOX			  3	// touch on edge, but not an onground
#define	SOLID_BSP				  4	// bsp clip, touch on edge, block
#define	SOLID_CORPSE			  5	// bsp clip, touch on edge, block

float(entity zombie, float which, entity limb) updateLimb		= {return 0;};

void(entity targ, entity inflictor, entity attacker, float damage) T_Damage = {};

float(entity e, float healamount, float ignore) T_Heal = {return 0;};
float(vector start, vector min, vector max, vector end, float nomonsters, entity forent) tracemove = {return 0;};

#define FILE_WRITE 1