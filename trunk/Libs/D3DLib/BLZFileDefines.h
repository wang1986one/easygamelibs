#pragma once

namespace D3DLib{

#define BLZ_DBC_HEADER_TAG					((DWORD)'CBDW')
#define BLZ_M2_HEADER_TAG					((DWORD)'02DM')
#define BLZ_SKIN_HEADER_TAG					((DWORD)'NIKS')

#define M2_MODEL_FX_FILE_NAME				"M2Model.fx"
#define M2_PARTICLE_FX_FILE_NAME			"M2Particle.fx"
#define M2_RIBBON_FX_FILE_NAME				"M2Ribbon.fx"
#define WMO_MODEL_FX_FILE_NAME				"WMOModel.fx"
#define ADT_MODEL_FX_FILE_NAME				"ADTModel.fx"




#define BLZ_DBC_CHAR_SECTION_RECORD_SIZE				(40)
#define BLZ_DBC_ANIMATION_DATA_RECORD_SIZE				(32)
#define BLZ_DBC_CHAR_HAIR_GEOSET_RECORD_SIZE			(24)
#define BLZ_DBC_CHAR_WHISKER_GEOSET_RECORD_SIZE			(32)
#define BLZ_DBC_CHAR_RACE_RECORD_SIZE					(276)
#define BLZ_DBC_CREATURE_DISPLAY_INFO_RECORD_SIZE		(64)
#define BLZ_DBC_CREATURE_MODEL_INFO_RECORD_SIZE			(104)
#define BLZ_DBC_ITEM_CLASS_RECORD_SIZE					(80)
#define BLZ_DBC_ITEM_SUB_CLASS_RECORD_SIZE				(176)
#define BLZ_DBC_ITEM_SUB_CLASS_MASK_RECORD_SIZE			(76)
#define BLZ_DBC_ITEM_DATA_RECORD_SIZE					(32)
#define BLZ_DBC_ITEM_DISPLAY_INFO_RECORD_SIZE			(100)
#define BLZ_DBC_HELMET_GEOSET_VISIBLE_RECORD_SIZE		(32)
#define BLZ_DBC_MAP_RECORD_SIZE							(472)

#define BLZ_DBC_STR_LOCAL_ZH_CN							4
#define BLZ_DBC_STR_LOCAL_NUM							17

#define BLZ_DBC_CHAR_SECTION_FILE_NAME			"CharSections.dbc"
#define BLZ_DBC_ANIMATION_DATA_FILE_NAME		"AnimationData.dbc"
#define BLZ_DBC_CHAR_HAIR_GEOSET_FILE_NAME		"CharHairGeosets.dbc"
#define BLZ_DBC_CHAR_WHISKER_GEOSET_FILE_NAME	"CharacterFacialHairStyles.dbc"
#define BLZ_DBC_CHAR_RACE_FILE_NAME				"ChrRaces.dbc"
#define BLZ_DBC_CREATURE_DISPLAY_INFO_FILE_NAME	"CreatureDisplayInfo.dbc"
#define BLZ_DBC_CREATURE_MODEL_INFO_FILE_NAME	"CreatureModelData.dbc"
#define BLZ_DBC_ITEM_CLASS_FILE_NAME			"ItemClass.dbc"
#define BLZ_DBC_ITEM_SUB_CLASS_FILE_NAME		"ItemSubClass.dbc"
#define BLZ_DBC_ITEM_SUB_CLASS_MASK_FILE_NAME	"ItemSubClassMask.dbc"
#define BLZ_DBC_ITEM_DATA_FILE_NAME				"Item.dbc"
#define BLZ_DBC_ITEM_CACHE_DATA_FILE_NAME		"ItemData.csv"
#define BLZ_DBC_ITEM_DISPLAY_INFO_FILE_NAME		"ItemDisplayInfo.dbc"
#define BLZ_DBC_HELMET_GEOSET_VISIBLE_FILE_NAME	"HelmetGeosetVisData.dbc"
#define BLZ_DBC_MAP_FILE_NAME					"Map.dbc"


#define EQUIPMENT_ARM_UPPER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\ARMUPPERTEXTURE"
#define EQUIPMENT_ARM_LOWER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\ARMLOWERTEXTURE"
#define EQUIPMENT_HAND_TEXTURE_PATH				"ITEM\\TEXTURECOMPONENTS\\HANDTEXTURE"
#define EQUIPMENT_TORSO_UPPER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\TorsoUpperTexture"
#define EQUIPMENT_TORSO_LOWER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\TorsoLowerTexture"
#define EQUIPMENT_LEG_UPPER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\LEGUPPERTEXTURE"
#define EQUIPMENT_LEG_LOWER_TEXTURE_PATH		"ITEM\\TEXTURECOMPONENTS\\LEGLOWERTEXTURE"
#define EQUIPMENT_FOOT_TEXTURE_PATH				"ITEM\\TEXTURECOMPONENTS\\FOOTTEXTURE"

#define WOW_MAP_ADT_FILE_DIR					"WORLD\\MAPS"


#define BLZ_ADT_MAP_TILE_SIZE					(33.33333333f)
#define BLZ_ADT_MAP_AREA_SIZE					(BLZ_ADT_MAP_TILE_SIZE*16.0f)
#define BLZ_ADT_MAP_TRANS_VALUE					(BLZ_ADT_MAP_AREA_SIZE*32.0f)
#define BLZ_ADT_TEXTURE_LAYER_COUNT				(4)

//WMO Root
#define CHUNK_ID_VERSION						((DWORD)'MVER')
#define CHUNK_ID_WMO_MOHD						((DWORD)'MOHD')
#define CHUNK_ID_WMO_MOTX						((DWORD)'MOTX')
#define CHUNK_ID_WMO_MOMT						((DWORD)'MOMT')
#define CHUNK_ID_WMO_MOGN						((DWORD)'MOGN')
#define CHUNK_ID_WMO_MOGI						((DWORD)'MOGI')
#define CHUNK_ID_WMO_MOPV						((DWORD)'MOPV')
#define CHUNK_ID_WMO_MOPT						((DWORD)'MOPT')
#define CHUNK_ID_WMO_MOPR						((DWORD)'MOPR')
#define CHUNK_ID_WMO_MOVV						((DWORD)'MOVV')
#define CHUNK_ID_WMO_MOVB						((DWORD)'MOVB')
#define CHUNK_ID_WMO_MOLT						((DWORD)'MOLT')
#define CHUNK_ID_WMO_MODS						((DWORD)'MODS')
#define CHUNK_ID_WMO_MODN						((DWORD)'MODN')
#define CHUNK_ID_WMO_MODD						((DWORD)'MODD')
#define CHUNK_ID_WMO_MFOG						((DWORD)'MFOG')
#define CHUNK_ID_WMO_MCVP						((DWORD)'MCVP')
//WMO Group
#define CHUNK_ID_WMO_MOGP						((DWORD)'MOGP')
#define CHUNK_ID_WMO_MOPY						((DWORD)'MOPY')
#define CHUNK_ID_WMO_MOVI						((DWORD)'MOVI')
#define CHUNK_ID_WMO_MOVT						((DWORD)'MOVT')
#define CHUNK_ID_WMO_MONR						((DWORD)'MONR')
#define CHUNK_ID_WMO_MOTV						((DWORD)'MOTV')
#define CHUNK_ID_WMO_MOBA						((DWORD)'MOBA')
#define CHUNK_ID_WMO_MOLR						((DWORD)'MOLR')
#define CHUNK_ID_WMO_MODR						((DWORD)'MODR')
#define CHUNK_ID_WMO_MOBN						((DWORD)'MOBN')
#define CHUNK_ID_WMO_MOBR						((DWORD)'MOBR')
#define CHUNK_ID_WMO_MOCV						((DWORD)'MOCV')
#define CHUNK_ID_WMO_MLIQ						((DWORD)'MLIQ')
//ADT
#define CHUNK_ID_ADT_MHDR						((DWORD)'MHDR')
#define CHUNK_ID_ADT_MCIN						((DWORD)'MCIN')
#define CHUNK_ID_ADT_MTEX						((DWORD)'MTEX')
#define CHUNK_ID_ADT_MMDX						((DWORD)'MMDX')
#define CHUNK_ID_ADT_MMID						((DWORD)'MMID')
#define CHUNK_ID_ADT_MWMO						((DWORD)'MWMO')
#define CHUNK_ID_ADT_MWID						((DWORD)'MWID')
#define CHUNK_ID_ADT_MDDF						((DWORD)'MDDF')
#define CHUNK_ID_ADT_MODF						((DWORD)'MODF')
#define CHUNK_ID_ADT_MH2O						((DWORD)'MH2O')
#define CHUNK_ID_ADT_MCNK						((DWORD)'MCNK')
#define CHUNK_ID_ADT_MCVT						((DWORD)'MCVT')
#define CHUNK_ID_ADT_MCCV						((DWORD)'MCCV')
#define CHUNK_ID_ADT_MCNR						((DWORD)'MCNR')
#define CHUNK_ID_ADT_MCLY						((DWORD)'MCLY')
#define CHUNK_ID_ADT_MCRF						((DWORD)'MCRF')
#define CHUNK_ID_ADT_MCSH						((DWORD)'MCSH')
#define CHUNK_ID_ADT_MCAL						((DWORD)'MCAL')
#define CHUNK_ID_ADT_MCLQ						((DWORD)'MCLQ')
#define CHUNK_ID_ADT_MCSE						((DWORD)'MCSE')
#define CHUNK_ID_ADT_MFBO						((DWORD)'MFBO')
#define CHUNK_ID_ADT_MTFX						((DWORD)'MTFX')

enum BLZ_M2_RENDER_FLAG
{
	BLZ_M2_RENDER_FLAG_NO_LIGHT=1,
	BLZ_M2_RENDER_FLAG_NO_FOG=(1<<1),
	BLZ_M2_RENDER_FLAG_NO_CULL=(1<<2),
	BLZ_M2_RENDER_FLAG_BILL_BOARD=(1<<3),
	BLZ_M2_RENDER_FLAG_DISABLE_ZBUFFER=(1<<4),
};

enum BLZ_M2_BLENDING_MODE
{
	BLZ_M2_BLENDING_MODE_OPAQUE,
	BLZ_M2_BLENDING_MODE_ALPHA_TEST,
	BLZ_M2_BLENDING_MODE_ALPHA_BLENDING,
	BLZ_M2_BLENDING_MODE_ADDITIVE,
	BLZ_M2_BLENDING_MODE_ADDITIVE_ALPHA,
	BLZ_M2_BLENDING_MODE_MODULATE,
	BLZ_M2_BLENDING_MODE_MUL,
};

enum BLZ_M2_ANIMATION_SEQUENCE_FLAG
{
	BLZ_M2_ANIMATION_SEQUENCE_FLAG_HAVE_KEY_DATA=0x20,
};

enum BLZ_M2_INTERPOLATION_TYPE
{
	BLZ_M2_INTERPOLATE_NONE,
	BLZ_M2_INTERPOLATE_LINEAR,
	BLZ_M2_INTERPOLATE_HERMITE,
};

enum KEY_BONE_TYPE
{	
	BONE_LARM = 0,		// 0, Left upper arm
	BONE_RARM,			// 1, Right upper arm
	BONE_LSHOULDER,		// 2, Left Shoulder / deltoid area
	BONE_RSHOULDER,		// 3, Right Shoulder / deltoid area
	BONE_STOMACH,		// 4, (upper?) abdomen
	BONE_WAIST,			// 5, (lower abdomen?) waist
	BONE_HEAD,			// 6, head
	BONE_JAW,			// 7, jaw/mouth
	BONE_RFINGER1,		// 8, (Trolls have 3 "fingers", this points to the 2nd one.
	BONE_RFINGER2,		// 9, center finger - only used by dwarfs.. don't know why
	BONE_RFINGER3,		// 10, (Trolls have 3 "fingers", this points to the 3rd one.
	BONE_RFINGERS,		// 11, Right fingers -- this is -1 for trolls, they have no fingers, only the 3 thumb like thingys
	BONE_RTHUMB,		// 12, Right Thumb
	BONE_LFINGER1,		// 13, (Trolls have 3 "fingers", this points to the 2nd one.
	BONE_LFINGER2,		// 14, Center finger - only used by dwarfs.
	BONE_LFINGER3,		// 15, (Trolls have 3 "fingers", this points to the 3rd one.
	BONE_LFINGERS,		// 16, Left fingers
	BONE_LTHUMB,		// 17, Left Thumb
	UnK19,	// ?
	UnK20,	// ?
	UnK21,	// ?
	UnK22,	// ?
	UnK23,	// ?
	UnK24,	// ?
	UnK25,	// ?
	UnK26,	// ?
	BONE_ROOT			// 26, The "Root" bone,  this controls rotations, transformations, etc of the whole model and all subsequent bones.
};

enum CHAR_ATTACHMENT_ID
{
	CAI_LEFT_WRIST					=0,	
	CAI_RIGHT_PALM1					=1,	 
	CAI_LEFT_PALM1					=2,	 
	CAI_RIGHT_ELBOW					=3,	 
	CAI_LEFT_ELBOW					=4,	 
	CAI_RIGHT_SHOULDER				=5,	 
	CAI_LEFT_SHOULDER				=6,	 
	CAI_RIGHT_KNEE					=7,	 
	CAI_LEFT_KNEE					=8,	
	CAI_RIGHT_WAIST					=9,
	CAI_LEFT_FRONT_WAIST			=10,
	CAI_HELMET						=11,	 
	CAI_BACK						=12,
	CAI_IN_RIGHT_SHOULDER			=13,
	CAI_LEFT_UPPER_ARM				=14,
	CAI_BUST1						=15,	 
	CAI_BUST2						=16,	 
	CAI_FACE						=17,	 
	CAI_ABOVE_CHARACTER				=18,	 
	CAI_GROUND 						=19,	
	CAI_TOP_OF_HEAD 				=20,	
	CAI_LEFT_PALM2 					=21,	
	CAI_RIGHT_PALM2					=22,	
	CAI_RIGHT_BACK_SHEATH 			=26,	
	CAI_LEFT_BACK_SHEATH 			=27,	
	CAI_MIDDLE_BACK_SHEATH 			=28,	
	CAI_BELLY 						=29,	
	CAI_LEFT_BACK 					=30,	
	CAI_RIGHT_BACK 					=31,	
	CAI_LEFT_HIP_SHEATH 			=32,	
	CAI_RIGHT_HIP_SHEATH 			=33,	
	CAI_BUST3 						=34,	
	CAI_RIGHT_PALM3 				=35,
	CAI_RIGHT_PALM4 				=36,
	CAI_LEFT_PALM3 					=37,
	CAI_LEFT_PALM4					=38,
	CAI_MAX,
};

extern LPCTSTR CHAR_ATTACHMENT_NAME[CAI_MAX];


enum ITEM_TYPE_FLAGS
{
	ITEM_TYPE_UNKNOW1=1,				//Unknown 	Didn't find any items with this mask 
	ITEM_TYPE_CONJURED=(1<<2),			//Conjured item 	Warlock/Mage stones, water etc 
	ITEM_TYPE_OPENABLE=(1<<3),			//Openable item 	Lockboxes, clams etc 
	ITEM_TYPE_UNKNOW2=(1<<4),			//Unknown 	Didn't find any items with this mask 
	ITEM_TYPE_DEPRECATED=(1<<5),		//Deprecated item	ie 128, 16999, 18566, 21785, 21786 - sometimes contains non deprecated recipes 
	ITEM_TYPE_TOTEM=(1<<6),				//Totem 		Shaman totem tools 
	ITEM_TYPE_SPELL_TRIGGERER=(1<<7),	//Spelltriggerer 	Items bearing a spell (Equip/Use/Proc etc) 
	ITEM_TYPE_UNKNOW3=(1<<8),			//Unknown 	Torch of Retribution 
	ITEM_TYPE_WAND=(1<<9),				//Wand 		These items would appear to do ranged magical damage 
	ITEM_TYPE_WRAP=(1<<10),				//Wrap 		These items can wrap other items (wrapping paper) 
	ITEM_TYPE_PRODUCER=(1<<11),			//Producer 	These items produce other items when right clicked (motes, enchanting essences, darkmoon cards...) 
	ITEM_TYPE_MULTI_LOOT=(1<<12),		//Multi loot 	Everyone in the group/raid can loot a copy of the item. 
	ITEM_TYPE_BG=(1<<13),				//BG Item? 	Do any items still have this? --Jb55 02:19, 31 December 2006 (EST) ) 
	ITEM_TYPE_CHARTER=(1<<14),			//Charter 	Guild charters 
	ITEM_TYPE_READABLE=(1<<15),			//Readable item 	<Right Click to Read> 
	ITEM_TYPE_PVP=(1<<16),				//PvP item 	Item bought with Honor or Arena Points - also contains a lot of reputation rewards and random stuff like Egbert's Egg (?!) 
	ITEM_TYPE_DURATION=(1<<17),			//Duration 	Item expires after a certain amount of time 
	ITEM_TYPE_UNKNOW4=(1<<18),			//Unknown 	
	ITEM_TYPE_PROSPECTABLE=(1<<19),		//Prospectable 	Items a jewel crafter can prospect 
	ITEM_TYPE_UNIQUE=(1<<20),			//Unique-Equipped	Items you can only have one of equipped, but multiple in your inventory 
	ITEM_TYPE_UNKNOW5=(1<<21),			//Unknown 	
	ITEM_TYPE_UNKNOW6=(1<<22),			//Unknown 	Bandages, Poisons + Most conjured items (not all) 
	ITEM_TYPE_UNLIMIT_THROWING=(1<<23), //Throwing Weapon	Lowers durability on cast (also contains deprecated throwing weapons) 
	ITEM_TYPE_UNKNOW7=(1<<24), 			//Unknown 	Shiny Red Apple

};


enum ITEM_QUALITY 
{
	ITEM_QUALITY_Poor=0,				//(Gray) 
	ITEM_QUALITY_Common=1,				//(White)
	ITEM_QUALITY_Uncommon=2,			//(Green)
	ITEM_QUALITY_Rare=3,				//(Blue) 
	ITEM_QUALITY_Epic=4,				//(Purple) 
	ITEM_QUALITY_Legendary=5,			//(Orange) 
	ITEM_ARTIFACT=6,					//(Red) 
};

enum ITEM_SHEATH_TYPE
{
	ITEM_SHEATH_TYPE_UPPER_RIGHT_BACK=26,
	ITEM_SHEATH_TYPE_UPPER_LEFT_BACK=27, 
	ITEM_SHEATH_TYPE_CENTER_BACK=28, 
	ITEM_SHEATH_TYPE_UPSIDE_DOWN=30,
	ITEM_SHEATH_TYPE_LEFT_HIP=32,
	ITEM_SHEATH_TYPE_RIGHT_HIP=33,
};

enum ITEM_INVENTORY_SLOT_ID
{
	IISI_NONE				=0,
	IISI_HEAD				=1,	
	IISI_NECK				=2,	
	IISI_SHOULDERS			=3,	
	IISI_SHIRT				=4,	
	IISI_VEST				=5,	
	IISI_WAIST				=6,	
	IISI_LEGS				=7,	
	IISI_FEET				=8,	
	IISI_WRIST				=9,	
	IISI_HANDS				=10,  
	IISI_RING				=11,  
	IISI_TRINKET			=12,  
	IISI_ONE_HAND			=13,  
	IISI_SHIELD				=14,	
	IISI_BOW				=15,  
	IISI_BACK				=16,  
	IISI_TWO_HAND			=17,  
	IISI_BAG				=18,  
	IISI_TABARD				=19,	
	IISI_ROBE				=20,  
	IISI_MAIN_HAND			=21,  
	IISI_OFF_HAND			=22,  
	IISI_HELD				=23,  
	IISI_AMMO				=24,  
	IISI_THROWN				=25,  
	IISI_RANGED				=26,  
	IISI_RANGED2			=27,  
	IISI_RELIC				=28,  
	IISI_MAX,

};

extern LPCTSTR ITEM_INVENTORY_SLOT_NAME[IISI_MAX];

extern LPCTSTR ITEM_PATH_BY_SLOT[IISI_MAX];


enum CHAR_SUBMESH_PART
{
	CSP_HAIR=0,
	CSP_WHISKER1=1,
	CSP_WHISKER2=2,
	CSP_WHISKER3=3,
	CSP_GLOVE=4,
	CSP_FOOT=5,
	CSP_EAR=7,
	CSP_SLEEVE=8,
	CSP_PAINTS=9,
	CSP_LAP=10,
	CSP_SKIRT=11,
	CSP_ENSIGN=12,
	CSP_ROBE=13,
	CSP_BACK=15,
	CSP_SHINE_EYE=17,
	CSP_SASH=18,
};

enum PARTICLE_EMITTER_TYPE
{
	EMITTER_PLANE=1,
	EMITTER_SPHERE=2,
	EMITTER_SPLINE=3,
};

enum PARTICLE_EMITTER_BLENDING_TYPE
{
	EBT_OPACITY=0,			//glDisable(GL_BLEND); glDisable(GL_ALPHA_TEST);  
	EBT_ADD=1,				//glBlendFunc(GL_SRC_COLOR, GL_ONE);  
	EBT_ALPHA_BLEND=2,		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	EBT_ALPHA_TEST=3,		//glDisable(GL_BLEND); glEnable(GL_ALPHA_TEST);  
	EBT_ALPHA_ADD=4,		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
};

enum PARTICLE_EMITTER_FLAG
{
	PEF_NO_GRAVITY_TRANSFORM=(0x2),
	PEF_DIR_VERTICAL_TRANS=0x1,
	PEDTT_DIR_HORIZONTAL_TRANS=0x400,
	PEF_NO_TRAIL=(0x10),	
	PEF_ACITVE_ON_TIME=(0x400),
};

enum WMO_LIGHT_TYPE 
{
	WMOLT_OMNI_LGT,
	WMOLT_SPOT_LGT,
	WMOLT_DIRECT_LGT,
	WMOLT_AMBIENT_LGT,
};

enum WMO_GROUP_FLAG
{
	WMOGF_HAVE_BOUNDBOX=0x1,
	WMOGF_HAVE_VERTEX_COLOR=0x4,
	WMOGF_OUTDOOR=0x8,
	WMOGF_HAVE_LIGHT=0x200,
	WMOGF_HAVE_DOODADS=0x800,
	WMOGF_HAVE_WATER=0x1000,
	WMOGF_INDOOR=0x2000,
	WMOGF_SHOW_SKYBOX=0x40000,
	WMOGF_IS_NOT_OCEAN=0x80000,
};

enum WMO_TRIANGLE_MATERIAL_FLAG
{
	WMOTMF_NO_COLLISION=0x4,
};

enum WMO_MATERIAL_FLAG
{
	WMOMF_NO_CULL=0x04,
	WMOMF_EMISSIVE=0x10,
	WMOMF_TEXTURE_CLAMP=0x40,
	WMOMF_TEXTURE_WRAP=0x80,
};

enum WMO_BLENDING_MODE
{
	WMOBM_OPAQUE,
	WMOBM_ALPHA_TEST,
};

enum MCNK_FLAGS
{
	MCNK_FLAG_MCSH_AVAILABLE=0x01,
	MCNK_FLAG_IMPASSABLE=0x02,
	MCNK_FLAG_RIVER=0x04,
	MCNK_FLAG_OCEAN=0x08,
	MCNK_FLAG_MAGMA=0x10,
	MCNK_FLAG_SLIME=0x20,
	MCNK_FLAG_MCCV_AVAILABLE=0x40,
	MCNK_FLAG_UNKNOWN=0x8000,
};

enum MCLY_LAYER_INFO_FLAGS
{
	MLIF_ANI_45=0x01,
	MLIF_ANI_90=0x02,
	MLIF_ANI_180=0x04,
	MLIF_ANI_FAST=0x08,
	MLIF_ANI_FAST_2=0x10,
	MLIF_ANI_FAST_3=0x20,
	MLIF_ANI=0x40,
	MLIF_GLOW=0x80,
	MLIF_ALPHA_MAP=0x100,
	MLIF_COMPRESSED_ALPHA_MAP=0x200,
	MLIF_SHINY=0x400,
};



#pragma pack(push)
#pragma pack(1)

struct BLZ_DBC_HEADER
{
	DWORD	Tag;
	UINT32	RecordCount;
	UINT32	FieldCount;
	UINT32	RecordSize;
	UINT32	StringBlockSize;
};

struct DBC_CHAR_SECTION_RECORD
{
	UINT32	ID;
	UINT32	Race;
	UINT32	Sex;
	UINT32	GeneralType;
	UINT32	Texture1;
	UINT32	Texture2;
	UINT32	Texture3;
	UINT32	Flags;
	UINT32	Type;
	UINT32	Variation;
};

struct DBC_ANIMATION_DATA_RECORD
{
	UINT32	ID;
	UINT32	Name;
	UINT32	WeaponState;
	UINT32	Flags;
	UINT32	Unknown1;
	UINT32	Preceeding;
	UINT32	RealId;
	UINT32	Group;
};

struct DBC_CHAR_HAIR_GEOSET_RECORD
{
	UINT32	ID;
	UINT32	Race;
	UINT32	Sex;
	UINT32	HairType;
	UINT32	SubMeshID;
	UINT32	IsBald;
};

struct DBC_CHAR_WHISKER_TYPE_RECORD
{	
	UINT32	Race;
	UINT32	Sex;
	UINT32	WhiskerType;	
	UINT32	SubMeshID1;
	UINT32	SubMeshID3;
	UINT32	SubMeshID2;
	UINT32	Unknown1;
	UINT32	Unknown2;
};

struct DBC_CHAR_RACE_RECORD
{	
	UINT32	RaceID;
	UINT32	Flags;					//Integer  &1: Not playable; This can remove or show boots. Set to 12 to show them. Known to be checked: 4 together with CMD 0x80. (?)  
	UINT32	FactionID;				//iRefID  facton template id. The order in the creation screen depends on this.  
	UINT32	Exploration;			//iRefID  Played on exploring zones with SMSG_EXPLORATION_EXPERIENCE.  
	UINT32	MaleModel;				//iRefID  only used for the char creation/selection screen. Ingame the server sets the model.  
	UINT32	FemaleModel;			//iRefID  only used for the char creation/selection screen. Ingame the server sets the model.  
	UINT32	Abbrev;					//String  A short form of the name. Used for helmet models.  
	UINT32	Unknown1;				//Float  This is 1.0 for the most. Maybe some sort of size or speed.  
	UINT32	Faction;				//Integer  1 = Horde, 7 = Alliance & not playable  
	UINT32	CreatureType;			//iRefID  Always 7 (humanoid).  
	UINT32	Unknown2;				//Integer  Always 15007.  
	UINT32	Unknown3;				//Integer  1090 for dwarfs, 1096 for the others. Getting stored in CGUnit at CGUnit::PostInit.  
	UINT32	InternalName;			//String  Same as the one used in model-filepathes.  
	UINT32	Camera;					//iRefID  Used for the opening cinematic.  
	UINT32	RaceNameMale[BLZ_DBC_STR_LOCAL_NUM];		//Loc  A name to display.  
	UINT32	RaceNameFemale[BLZ_DBC_STR_LOCAL_NUM];		//Loc  always is NULL for zhCN.  
	UINT32	RaceNameNeutral[BLZ_DBC_STR_LOCAL_NUM];	//Loc  always is NULL for zhCN.  
	UINT32	Feature1;				//String  Internal names for the facial features.  
	UINT32	Feature2;				//String  The localized ones are in luas.  
	UINT32	Feature3;				//String   
	UINT32	Expansion;				//Integer  0 for classic and non-playables, 1 for Burning Crusade  

};

struct DBC_CREATURE_DISPLAY_INFO_RECORD
{
	UINT32	ID;							//Integer   
	UINT32	ModelID;					//iRefID  A model to be used.  
	UINT32	SoundID;					//iRefID  Not set for that much models. Can also be set in CreatureModelData.  
	UINT32	ExtraDisplayInformationID;	//iRefID  If this display-id is a NPC wearing things that are described in there.  
	FLOAT	Scale;						//Float  Default scale, if not set by server. 1 is the normal size.  
	UINT32	Opacity;					//Integer  0 (transparent) to 255 (opaque).  
	UINT32	Skin1;						//String  Skins that are used in the model.  
	UINT32	Skin2;						//String  See this for information when they are used.  
	UINT32	Skin3;						//String   
	UINT32	Icon;						//String  Holding an icon like INV_Misc_Food_59. Only on a few.  
	UINT32	BloodLevelID;				//iRefID  If 0, this is read from CreatureModelData. (CGUnit::RefreshDataPointers)  
	UINT32	BloodID;					//iRefID   
	UINT32	NPCSoundID;					//iRefID  Sounds used when interacting with the NPC.  
	UINT32	ParticleID;					//iRefID  Values are 0 and >281. Wherever they are used ..  
	UINT32	CreatureGeosetDataID;		//Integer  Only set for IronDwarfs. 0x1111 to 0x1213. (WotLK)  
	UINT32	ObjectEffectPackageID;		//iRefID  Set for gyrocopters, catapults, rocketmounts and siegevehicles. (WotLK)  
};

struct DBC_CREATURE_MODEL_INFO_RECORD
{
	UINT32	ID;							//Integer 	
	UINT32	Flags;						//Integer 	Known to be checked: 0x80. Known: 4: Has death corpse.
	UINT32	ModelPath;					//String 		*.MDX!
	UINT32	AltermateModel;				//String 		This is always 0. It would be used, if something was in here. Its pushed into M2Scene::AddNewModel(GetM2Cache(), Modelpath, AlternateModel, 0).
	UINT32	Unknown1;					//Integer* 	0 - 4 | 4 got mostly big models (ragnaros, nef.) but again, not all big models got 4 ...
	FLOAT	Scale;						//Float 		CMD.Scale * CDI.Scale is used in CUnit.
	UINT32	BloodLevelID;				//iRefID	
	UINT32	Unknown2;					//Integer* 	-1, 0, 1, 3 - 7 | 0 for "Creature\CrystalSpider\CrystalSpider.mdx" only
	FLOAT	Unknown3; 					//Float 		big dragons got 30.0, some mounts 24.0 etc | most time 18.0
	FLOAT	Unknown4; 					//Float 	        mostly 12, others are 0.0 - 20.0
	FLOAT	Unknown5; 					//Float 	        mostly 1.0, others are 0.0 - 5.0  
	UINT32	Unknown6;					//Integer* 	always 0.
	UINT32	Unknown7;					//Integer* 	ground shake? (Kunga) | 0 - 2, 10, 79, 82 | again bigger models got bigger number most time
	UINT32	Unknown8; 					//Integer* 	0, 10 - 12, 38 | 0 most of the time.
	UINT32	SoundDataID;				//iRefID		
	FLOAT	CollisionWidth; 			//Float 		Size of collision for the model. Has to be bigger than 0.41670012920929, else "collision width is too small.".
	FLOAT	CollisionHeight;			//Float 		ZEROSCALEUNIT when 0-CollisionHeight < 0
	FLOAT	Unknown9; 					//Float 		<.. other collision data?
	FLOAT	Unknown10; 					//Float 		 ..
	FLOAT	Unknown11; 					//Float 		 ..
	FLOAT	Unknown12; 					//Float 		 .. selection circle size ?
	FLOAT	Unknown13; 					//Float 		 ..
	FLOAT	Unknown14; 					//Float 		 .. 
	FLOAT	Unknown15;					//Float 	 	 other collision data? ..>
	FLOAT	Unknown16; 					//Float 		 mostly 1.0, others are 0.03 - 0.9
	FLOAT	Unknown17; 					//Float 		 mostly 1.0, others are 0.5 - 2.9
};

struct DBC_ITEM_CLASS
{
	UINT32	ClassID;
	UINT32	SubClassID;
	UINT32	IsWeapon;
	UINT32	Name[BLZ_DBC_STR_LOCAL_NUM];
};

struct DBC_ITEM_SUB_CLASS
{
	UINT32	ClassID;
	UINT32	SubClassID;
	UINT32	Unknow1;
	UINT32	Unknow2;
	UINT32	Unknow3;
	UINT32	Unknow4;
	UINT32	Unknow5;
	UINT32	Unknow6;
	UINT32	Unknow7;
	UINT32	HandsNumber;
	UINT32	Name[BLZ_DBC_STR_LOCAL_NUM];
	UINT32	UniquePlural[BLZ_DBC_STR_LOCAL_NUM];
};

struct DBC_ITEM_SUB_CLASS_MASK
{
	UINT32	ID;
	UINT32	Mask;
	UINT32	Name[BLZ_DBC_STR_LOCAL_NUM];
};

struct DBC_ITEM_DATA
{
	UINT32	ItemID;							//Integer   
	UINT32	ItemClass;						//iRefID  
	UINT32	ItemSubClass;					//iRefID  
	INT32	Unknown;						//Integer  -1 - 20, mostly -1, same as ItemCache.wdb column5  
	INT32	MaterialID;						//Integer  -1 - 8  
	UINT32	ItemDisplayInfo;				//iRefID  
	UINT32	InventorySlotID;				//Integer  These inventory slot ID's are used to determine how an item may be equipped, see below  
	UINT32	SheathID;						//Integer  0 -4, mostly 0  
};


struct DBC_ITEM_DISPLAY_INFO
{
	UINT32	ID;								//Integer   
	UINT32	LeftModel;						//String  For example at shoulders. Defineing the pairs.  
	UINT32	RightModel;						//String   
	UINT32	LeftModelTexture;				//String  And of course the textures, if not hardcoded.  
	UINT32	RightModelTexture;				//String   
	UINT32	Icon;							//  String  The icon displayed in the bags etc.  
	UINT32	Texture;						//  String  If another texture is needed.  
	UINT32	GloveGeosetFlags;				//  Integer  Maybe the gloves and bracers are swapped!  
	UINT32	PantsGeosetFlags;				//  Integer   
	UINT32	RobeGeosetFlags;				//  Integer   
	UINT32	BootsGeosetFlags;				//  Integer   
	UINT32	UnkownGeosetFlags;				//  Integer   
	UINT32	ItemGroupSounds;				//  iRefID   
	UINT32	HelmetGeosetVisData1;			//  iRefID   
	UINT32	HelmetGeosetVisData2;			//  iRefID   
	UINT32	UpperArmTexture;				//  String   
	UINT32	LowerArmTexture;				//  String   
	UINT32	HandsTexture;					//  String   
	UINT32	UpperTorsoTexture;				//  String   
	UINT32	LowerTorsoTexture;				//  String   
	UINT32	UpperLegTexture;				//  String   
	UINT32	LowerLegTexture;				//  String   
	UINT32	FootTexture;					//  String   
	UINT32	ItemVisuals;					//  iRefID  Static enchants.  
	UINT32	Unkown2;						//Integer  

};

struct DBC_HELMET_GEOSET_VISIBLE_INFO
{
	UINT32	ID;								//Integer   
	INT32	HairFlags;						//Integer  0 = show, anything else = don't show? eg: a value of 1020 won't hide night elf ears, but 999999 or -1 will.  
	INT32	Facial1Flags;					//Integer  (Beard, Tusks)  
	INT32	Facial2Flags;					//Integer  (Earrings)  
	INT32	Facial3Flags;					//Integer  See ChrRaces, column 24 to 26 for information on what is what.  
	INT32	EarsFlags;						//Integer  
	INT32	Unknow1;
	INT32	Unknow2;
};


struct DBC_MAP_INFO
{
	UINT32	ID;
	UINT32	InternalName;							//String reference to World\Map\ [...] \  
	UINT32	AreaType;								// Integer 0: none, 1: party, 2: raid, 3: pvp, 4: arena, >=5: none (official from "IsInInstance()")  
	UINT32	IsBattleground;							//  Boolean Simple flag for battleground maps  
	UINT32	Name[BLZ_DBC_STR_LOCAL_NUM];			// Loc Name (displayed on World Map for example)  
	UINT32	AreaTable;								// iRefID iRefID to AreaTable.dbc  
	UINT32	AllianceDesc[BLZ_DBC_STR_LOCAL_NUM];	// Loc Alliance  
	UINT32	HordeDesc[BLZ_DBC_STR_LOCAL_NUM];		// Loc Horde  
	UINT32	LoadingScreen;							// iRefID The LoadingScreen to Display  
	FLOAT	BattlefieldMapIconScale;				//Float  
	UINT32	Requirement[BLZ_DBC_STR_LOCAL_NUM];		//Loc Requirement to enter  
	UINT32	Heroic[BLZ_DBC_STR_LOCAL_NUM];			//Loc Heroic-Requirement to enter  
	UINT32	Unknown1[BLZ_DBC_STR_LOCAL_NUM];		//Loc Some other requirement to enter.  
	UINT32	ParentArea;								// iRefID Points to column 1, -1 if none  
	FLOAT	XCoord;									// Float The X-Coord of the instance entrance  
	FLOAT	YCoord;									// Float The Y-Coord of the instance entrance  
	UINT32	ResetTimeRaid;							//Integer Raid.  
	UINT32	ResetTimeHeroicRaid;					// Integer Heroic raid or instance?  
	UINT32	Unknown2;								//  Integer All the time 0.  
	UINT32	TimeOfDayOverride;						//  Integer Set to -1 for everything but Orgrimmar and Dalaran arena. For those, the time of day will change to this.  
	UINT32	ExpansionID;							//  Integer Vanilla: 0, BC: 1, WotLK: 2  
	UINT32	Unknown3;								//  Integer  Another resettime? Is the same as the heroic one for some entries.  

};

struct BLZ_M2_HEADER
{
	DWORD		Tag;
	DWORD		Version;
	UINT		ModelNameLength;
	UINT		ModelNameOffset;
	UINT		ModelType;
	UINT		GlobalSequencesCount;
	UINT		GlobalSequencesOffset;
	UINT		AnimationsCount;
	UINT		AnimationsOffset;
	UINT		AnimationLookupCount;
	UINT		AnimationLookupOffset;
	UINT		BonesCount;
	UINT		BonesOffset;
	UINT		KeyBoneLookupCount;
	UINT		KeyBoneLookupOffset;
	UINT		VerticesCount;
	UINT		VerticesOffset;	
	UINT		ViewsCount;	
	UINT		ColorsCount;
	UINT		ColorsOffset;
	UINT		TexturesCount;
	UINT		TexturesOffset;
	UINT		TransparencyCount;
	UINT		TransparencyOffset;
	UINT		TexAnimsCount;
	UINT		TexAnimsOffset;
	UINT		TexReplaceCount;
	UINT		TexReplaceOffset;
	UINT		RenderFlagsCount;
	UINT		RenderFlagsOffset;
	UINT		BoneLookupCount;
	UINT		BoneLookupOffset;
	UINT		TexLookupCount;
	UINT		TexLookupOffset;
	UINT		TexUnitsCount;
	UINT		TexUnitsOffset;
	UINT		TransLookupCount;
	UINT		TransLookupOffset;
	UINT		TexAnimLookupCount;
	UINT		TexAnimLookupOffset;
	UINT		UnknowFloats[14];
	UINT		BoundingTrianglesCount;
	UINT		BoundingTrianglesOffset;
	UINT		BoundingVerticesCount;
	UINT		BoundingVerticesOffsets;
	UINT		BoundingNormalsCount;
	UINT		BoundingNormalsOffset;
	UINT		AttachmentsCount;
	UINT		AttachmentsOffset;
	UINT		AttachLookupCount;
	UINT		AttachLookupOffset;
	UINT		Events2Count;
	UINT		EventsOffset;
	UINT		LightsCount;
	UINT		LightsOffset;
	UINT		CamerasCount;
	UINT		CamerasOffset;
	UINT		CameraLookupCount;
	UINT		CameraLookupOffset;
	UINT		RibbonEmittersCount;
	UINT		RibbonEmittersOffset;
	UINT		ParticleEmittersCount;
	UINT		ParticleEmittersOffset;
};

struct M2_MODEL_VERTEXT
{
	CD3DVector3		Pos;
	BYTE			BoneWeight[4];
	BYTE			BoneIndex[4];
	CD3DVector3		Normal;
	CD3DVector2		Tex;
	FLOAT			Unknow[2];
};

struct M2_MODEL_TEXTURE
{
	UINT32 Type;
	UINT16 Unknown;
	UINT16 Flags;
	UINT32 FileNameLength;
	UINT32 FileNameOffset;
};
struct M2_MODEL_ANIMATION_SEQUENCE
{
	UINT16		AnimationID;
	UINT16		SubAnimationID;
	UINT32		Length;
	FLOAT		MovingSpeed;
	UINT32		Flags;
	UINT32		Flags2;
	UINT32		Unknown1;
	UINT32		Unknown2;
	UINT32		PlaybackSpeed;
	CD3DVector3 BoundingBox[2];
	FLOAT		Radius;
	INT16		NextAnimation;
	UINT16		IndexId;
};

struct M2_MODEL_ANIMATION_BLOCK
{
	UINT16	InterpolationType;
	INT16	GlobalSequenceID;
	UINT32	TimestampsCount;
	UINT32	TimestampsOffset;
	UINT32	KeysCount;
	UINT32	KeysOffset;
};

struct M2_MODEL_FAKE_ANIMATION_BLOCK
{	
	UINT32	TimestampsCount;
	UINT32	TimestampsOffset;
	UINT32	KeysCount;
	UINT32	KeysOffset;
};

struct M2_MODEL_ANIMATION_PAIR
{
	UINT32	Count;
	UINT32	Offset;
};



struct M2_MODEL_BONE
{
	INT32						AnimationSeq;
	UINT32						Flags;
	INT16						ParentBone;
	UINT16						GeosetID;
	UINT32						Unknown;
	M2_MODEL_ANIMATION_BLOCK	Translation;
	M2_MODEL_ANIMATION_BLOCK	Rotation;
	M2_MODEL_ANIMATION_BLOCK	Scaling;
	CD3DVector3					PivotPoint;
};

struct M2_MODEL_RENDER_FLAG
{
	UINT16	Flags;
	UINT16	BlendingMode;
};

struct M2_MODEL_ATTACHMENT
{
	UINT32						ID;
	UINT32						Bone;
	CD3DVector3					Position;
	M2_MODEL_ANIMATION_BLOCK	Data;
};

struct M2_MODEL_ATTACHMENT2
{
	UINT32						Identifier;
	UINT32						ID;
	UINT32						Bone;
	CD3DVector3					Position;
	UINT16						InterpolationType;
	INT16						GlobalSequenceID;
	UINT32						TimestampsCount;
	UINT32						TimestampsOffset;
};

struct M2_COLOR_ANIMATION
{
	M2_MODEL_ANIMATION_BLOCK	ColorAni;
	M2_MODEL_ANIMATION_BLOCK	AlphaAni;
};

struct M2_TRANSPARENCY_ANIMATION
{
	M2_MODEL_ANIMATION_BLOCK	AlphaAni;
};


struct SKIN_HEADER
{
	UINT32	Tag;
	UINT32	IndicesCount;
	UINT32	IndicesOffset;
	UINT32	TrianglesCount;
	UINT32	TrianglesOffset;
	UINT32	PropertiesCount;
	UINT32	PropertiesOffset;
	UINT32	SubmeshesCount;
	UINT32	SubmeshesOffset;
	UINT32	TextureUnitsCount;
	UINT32	TextureUnitsOffset;
	UINT32	LOD;
};

struct SKIN_SUB_MESH
{
	UINT32		ID;
	UINT16		StartVertex;
	UINT16		VerticesCount;
	UINT16		StartTriangle;
	UINT16		TrianglesCount;
	UINT16		BonesCount;
	UINT16		StartBones;
	UINT16		Unknown;		
	UINT16		RootBone;
	CD3DVector3	BoundingBox[2];
	FLOAT		Radius;
};

struct SKIN_TEXTURE_UNIT
{
	UINT16	Flags;			//Usually 16 for static textures, and 0 for animated textures.
	UINT16	Shading;		//If set to 0x8000: shaders. Used in skyboxes to ditch the need for depth buffering. See below.
	UINT16	SubmeshIndex;	//A duplicate entry of a submesh from the list above.
	UINT16	SubmeshIndex2;	
	INT16	ColorIndex;		//A Color out of the Colors-Block or -1 if none.
	UINT16	RenderFlags;	//The renderflags used on this texture-unit.
	UINT16	TexUnitNumber;	//Index into the texture unit lookup table.
	UINT16	Mode;			//Always 1.
	UINT16	Texture;		//Index into Texture lookup table
	UINT16	TexUnitNumber2;	//Duplicate of TexUnitNumber.
	UINT16	Transparency;	//Index into transparency lookup table.
	UINT16	TextureAnim;	//Was a index into the texture animation lookup table. 
};

struct SKIN_VERTEX_PROPERTIES
{
	BYTE	BoneLookupIndex[4];
};


struct BLZ_SHORT_QUATERNION
{
	short x,y,z,w;
};


struct M2_PARTICLE_EMITTER
{
	UINT32							Unknow1;					//Unknown Always (as I have seen): -1.  
	UINT32							Flags;						//See Below  
	CD3DVector3						Position;					//The position. Relative to the following bone.  
	UINT16							Bone;						//The bone its attached to.  
	UINT16							Texture;					//And the texture that is used.  
	UINT32							ModelFileNameLength;		//The lenght of the ModelFilename. Zeroterminated String!  
	UINT32							ModelFileNameOffset;		//And the matching offset. This is used for spawning Models. *.mdx  
	UINT32							ParticleFileNameLength;		//The lenght of the ParticleFilename. Zeroterminated String!  
	UINT32							ParticleFileNameOffset;		//And the matching offset again.This is used for spawning models of a model. *.mdx!  
	UINT8							BlendingType;				//A blending type for the particle. See Below  
	UINT8							EmitterType;				//1 - Plane (rectangle), 2 - Sphere, 3 - Spline? (can't be bothered to find one)  
	UINT16							ParticleColorRef;			//This one is used for ParticleColor.dbc. See below.  
	UINT8							ParticleType;				//? Found below.  
	UINT8							HeadOrTail;					//0 - Head, 1 - Tail, 2 - Both  
	UINT16							TextureTileRotation;		// Rotation for the texture tile. (Values: -1,0,1)  
	UINT16							TextureRows;				// How many different frames are on that texture? People should learn what rows and cols are.  
	UINT16							TextureCols;				// Its different everywhere. I just took it random.  
	M2_MODEL_ANIMATION_BLOCK		EmissionSpeed;				// All of the following blocks should be floats.  
	M2_MODEL_ANIMATION_BLOCK		SpeedVariation;				// Variation in the flying-speed. (range: 0 to 1)  
	M2_MODEL_ANIMATION_BLOCK		VerticalRange;				// Drifting away vertically. (range: 0 to pi)  
	M2_MODEL_ANIMATION_BLOCK		HorizontalRange;			// They can do it horizontally too! (range: 0 to 2*pi)  
	M2_MODEL_ANIMATION_BLOCK		Gravity;					// Fall down, apple!  
	M2_MODEL_ANIMATION_BLOCK		LifeSpan;					// Everyone has to die.  
	UINT32							UnknownPadding1;			//  I don't know what these two values should do..  
	M2_MODEL_ANIMATION_BLOCK		EmissionRate;				// Stread your particles, emitter.  
	UINT32							UnknownPadding2;				//It could have been an array without them..  
	M2_MODEL_ANIMATION_BLOCK		EmissionAreaLength;			// Well, you can do that in this area.  
	M2_MODEL_ANIMATION_BLOCK		EmissionAreaWidth;			//  
	M2_MODEL_ANIMATION_BLOCK		Decelerate;					// ¼õËÙ¶È
	M2_MODEL_FAKE_ANIMATION_BLOCK	ParticleColor;				// (short, vec3f) This one points to 3 floats defining red, green and blue.  
	M2_MODEL_FAKE_ANIMATION_BLOCK	ParticleOpacity;			//? (short, short) Looks like opacity (short) --Igor; Most likely they all have 3 timestamps for {start, middle, end}.  
	M2_MODEL_FAKE_ANIMATION_BLOCK	ParticleSizes;				// (short, vec2f) It carries two floats per key. (x and y scale)  
	INT32							UnknownFields[2];			//No references. Padding?  
	M2_MODEL_FAKE_ANIMATION_BLOCK	Intensity;					//Some kind of intensity values seen: 0,16,17,32(if set to different it will have high intensity) (short, short)  
	M2_MODEL_FAKE_ANIMATION_BLOCK	UnknownReferences1;			//(short, short)  
	FLOAT							UnknownFloats1[3];			//They have something to do with the spread.  
	CD3DVector3						Scale;						// Wheey, its the scale!  
	FLOAT							Slowdown;					//Slowpoke is slow.  
	FLOAT							UnknownFloats2[2];			//More unknown fields.  
	FLOAT							Rotation;					// As a single value? Most likely only one axis then..  
	FLOAT							UnknownFloats3[2];			// More unknown fields.  
	FLOAT							Rotation1[3];				// Model Rotation 1  
	FLOAT							Rotation2[3];				// Model Rotation 2  
	CD3DVector3						Trans;						// Model Translation  
	FLOAT							UnknownFloats4[4];			// Unknown, unknown, unknown, unknown, unknown...  
	UINT32							UnknownReferenceCount;  
	UINT32							UnknownReferenceOffset;		// Vec3D array  
	M2_MODEL_ANIMATION_BLOCK		EnabledIn;					// (boolean) Has been in the earlier documentations. Enabled Anim Block.  
};


struct M2_RIBBON_EMITTER
{
	UINT32						Unknown1;						//  Always (as I have seen): -1.  
	UINT32						BoneID;							//  A bone to attach to.  
	CD3DVector3					Position;						// And a position, relative to that bone.  
	UINT32						TextureCount;					// Number of referenced textures.  
	UINT32						TextureOffset;					// Offset to the referenced textures.  
	UINT32						BlendRefCount;					//  Number of some referenced integers,which look like the blending for the texture  
	UINT32						BlendRefOffset;					// Offset to the blending-integers.  
	M2_MODEL_ANIMATION_BLOCK	Color;							// A color in three floats.  
	M2_MODEL_ANIMATION_BLOCK	Opacity;						// And an alpha value in a short, where: 0 - transparent, 0x7FFF - opaque.  
	M2_MODEL_ANIMATION_BLOCK	Above;							// The height above.  
	M2_MODEL_ANIMATION_BLOCK	Below;							// The height below. Do not set these to the same!  
	FLOAT						Resolution;						// This defines how smooth the ribbon is. A low value may produce a lot of edges.  
	FLOAT						Length;							//  The length aka Lifespan.  
	FLOAT						Emissionangle;					// use arcsin(val) to get the angle in degree  
	WORD						Renderflags[2];					// Perhaps the same as in renderflags  
	M2_MODEL_ANIMATION_BLOCK	UnknownABlock1;					// (short)  
	M2_MODEL_ANIMATION_BLOCK	UnknownABlock2;					// (boolean)  
	INT32						unknown2;						// This looks much like just some Padding to the fill up the 0x10 Bytes, always 0  
};

struct M2_TEXTURE_UV_ANIMATION
{
	M2_MODEL_ANIMATION_BLOCK	Translation;
	M2_MODEL_ANIMATION_BLOCK	Rotation;
	M2_MODEL_ANIMATION_BLOCK	Scaling;
};

struct BLZ_CHUNK_HEADER
{
	union
	{
		DWORD	ChunkID;
		char	ChunkName[4];
	};
	
	UINT32	ChunkSize;
};

struct BLZ_CHUNK_VERSION:public BLZ_CHUNK_HEADER
{
	UINT32	Version;
};


struct BLZ_CHUNK_MOHD:public BLZ_CHUNK_HEADER
{
	UINT32 		MaterialCount;							// - number of materials
	UINT32 		GroupCount;								// - number of WMO groups
	UINT32 		PortalCount;							// - number of portals
	UINT32 		LightCount;								// - number of lights
	UINT32 		ModelCount;								// - number of M2 models imported
	UINT32 		DoodadCount;							// - number of dedicated files (*see below this table!) 
	UINT32 		DoodadSetCount;							// - number of doodad sets
	UINT32 		AmbientColor;
	UINT32 		WMOID;									// (column 2 in WMOAreaTable.dbc)
	CD3DVector3 BoundingBox[2];
	UINT32 		LiquidType;								// related, see below in the MLIQ chunk.

};

struct BLZ_CHUNK_MOTX:public BLZ_CHUNK_HEADER
{
	char TextureFileNames[1];
};


struct WMOMaterial
{
	UINT32	Flags;		
	UINT32	SpecularMode;		
	UINT32	BlendMode;		// Blending: 0 for opaque, 1 for transparent
	INT32	Texture1;		// Start position for the first texture filename in the MOTX data block	
	UINT32	TextureColor1; 
	UINT32	TextureFlags1;
	INT32	Texture2;		// Start position for the second texture filename in the MOTX data block	
	UINT32	TextureColor2; 	
	UINT32	TextureFlags2;
	UINT32	Color3; 	
	UINT32	Unknow[4];
	void*	pTexture1;		// this is the first texture object. of course only in RAM. leave this alone. :D
	void*	pTexture2;		// this is the second texture object.
};

struct BLZ_CHUNK_MOMT:public BLZ_CHUNK_HEADER
{
	WMOMaterial		Materials[1];
};


struct BLZ_CHUNK_MOGN:public BLZ_CHUNK_HEADER
{
	char GroupFileNames[1];
};

struct WMOGroup
{
	UINT32			Flags;  		
	CD3DVector3		BoundingBox[2];  		
	INT32			NameOffset;
};

struct BLZ_CHUNK_MOGI:public BLZ_CHUNK_HEADER
{
	WMOGroup Group[1];
};

struct BLZ_CHUNK_MOPV:public BLZ_CHUNK_HEADER
{
	CD3DVector3 PortalVertices[1];
};

struct WMOPortalInfo
{
	UINT16 			VertexIndex;
	UINT16 			VertexCount;			// (?), always 4 (?)
	CD3DVector3 	Normal;					// vector maybe? haven't checked.
	FLOAT 			Unknown;				//  - if this is NAN, the three floats will be (0,0,1)
};

struct BLZ_CHUNK_MOPT:public BLZ_CHUNK_HEADER
{
	WMOPortalInfo PortalInfo[1];
};

struct WMOPortalRelationship
{
	UINT16 		PortalIndex;
	UINT16 		GroupIndex;
	INT16 		Unknow1;		//1 or -1
	UINT16 		Unknow2;		//always 0
};

struct BLZ_CHUNK_MOPR:public BLZ_CHUNK_HEADER
{
	WMOPortalRelationship PortalRelationship[1];
};

struct BLZ_CHUNK_MOVV:public BLZ_CHUNK_HEADER
{
	CD3DVector3 Vertices[1];
};

struct WMOVisibleBlockList
{
	UINT16	FirstVertex;
	UINT16	VertextCount;
};

struct BLZ_CHUNK_MOVB:public BLZ_CHUNK_HEADER
{
	WMOVisibleBlockList VisibleBlock[1];
};

struct WMOLightInfo
{
	UINT8		LightType; 	
	UINT8		Type;
	UINT8		UseAtten;
	UINT8		Pad;
	UINT32		Color;
	CD3DVector3 Position;
	float 		Intensity;
	float 		AttenStart;
	float 		AttenEnd;
	float 		Unknow1;
	float 		Unknow2;
	float 		Unknow3;
	float 		Unknow4;
};

struct BLZ_CHUNK_MOLT:public BLZ_CHUNK_HEADER
{
	WMOLightInfo Lights[1];
};

struct WMODoodadSet
{
	char   Name[20];
	UINT32 FirstInstanceIndex;
	UINT32 DoodadCount;
	UINT32 Nulls;
};

struct BLZ_CHUNK_MODS:public BLZ_CHUNK_HEADER
{
	WMODoodadSet DoodadSets[1];
};

struct BLZ_CHUNK_MODN:public BLZ_CHUNK_HEADER
{
	char DoodadFileNames[1];
};

struct SMODoodadInfo
{
	UINT32			NameIndex;
	CD3DVector3		Translation;
	CD3DQuaternion	Rotation;
	float			Scaling;
	UINT32			Color;
};

struct BLZ_CHUNK_MODD:public BLZ_CHUNK_HEADER
{
	SMODoodadInfo Doodads[1];
};

struct BLZ_CHUNK_MFOG:public BLZ_CHUNK_HEADER
{
	UINT32 		Flags;
	CD3DVector3	Position;
	FLOAT 		SmalleRadius;
	FLOAT 		LargeRadius;
	FLOAT 		FogEnd;
	FLOAT 		FogStart;						// multiplier (0..1)
	UINT32 		FogColor;						//The back buffer is also cleared to this colour 
	FLOAT 		Unknown1;						// (almost always 222.222)
	FLOAT 		Unknown2;						// (-1 or -0.5)
	UINT32 		Color2;
};

struct BLZ_CHUNK_MCVP:public BLZ_CHUNK_HEADER
{
	float ConvexVolumePlanes[1];
};


struct BLZ_CHUNK_MOGP:public BLZ_CHUNK_HEADER
{
	UINT32 		Name;							//Group name (offset into MOGN chunk)
	UINT32 		Descriptive;					// group name (offset into MOGN chunk)
	UINT32 		Flags;							//
	CD3DVector3 BoundingBox[2];
	UINT16 		PortalStart;					//Index into the MOPR chunk
	UINT16 		PortalCount;					//Number of items used from the MOPR chunk
	UINT16 		Batches[4];
	UINT8	 	Fogs[4];						//Up to four indices into the WMO fog list
	UINT32 		LiquidType;						// related, see below in the MLIQ chunk.
	UINT32 		GroupID;						//WMO group ID (column 4 in WMOAreaTable.dbc)
	UINT32 		Unknow1;
	UINT32 		Unknow2;
};

struct WMOTriangleMaterialInfo
{
	UINT8	Flags;
	INT8	MaterialIndex;
};

struct BLZ_CHUNK_MOPY:public BLZ_CHUNK_HEADER
{
	WMOTriangleMaterialInfo TriangleMaterialInfo[1];
};

struct BLZ_CHUNK_MOVI:public BLZ_CHUNK_HEADER
{
	UINT16 VertexIndices[1];
};

struct BLZ_CHUNK_MOVT:public BLZ_CHUNK_HEADER
{
	CD3DVector3 Vertices[1];
};

struct BLZ_CHUNK_MONR:public BLZ_CHUNK_HEADER
{
	CD3DVector3 Normals[1];
};

struct BLZ_CHUNK_MOTV:public BLZ_CHUNK_HEADER
{
	CD3DVector2 TextureCoordinates[1];
};

struct WMORenderBatch
{
	UINT32 		Color1;
	UINT32 		Color2;
	UINT32 		Color3;
	UINT32 		StartIndex;
	UINT16 		IndexCount;
	UINT16 		StartVertex;
	UINT16 		EndVertex;
	UINT8 		Unknow;
	UINT8 		Material;
};

struct BLZ_CHUNK_MOBA:public BLZ_CHUNK_HEADER
{
	WMORenderBatch RenderBatchs[1];
};

struct BLZ_CHUNK_MOLR:public BLZ_CHUNK_HEADER
{
	UINT16 LightIndices[1];
};

struct BLZ_CHUNK_MODR:public BLZ_CHUNK_HEADER
{
	UINT16 DoodadIndices[1];
};

struct WMOBSPNode
{	
	INT16	PlaneType;          // unsure
	INT16	RightChild;			// index of bsp child node(right in this array)   
	INT16	LeftChild;
	UINT16	FaceCount;			// num of triangle faces in  MOBR
	UINT16	FirstFace;			// index of the first triangle index(in  MOBR)
	INT16	Unknow;		        // 0
	float	Distance;    
};

struct BLZ_CHUNK_MOBN:public BLZ_CHUNK_HEADER
{
	WMOBSPNode BSPNodes[1];
};

struct BLZ_CHUNK_MOBR:public BLZ_CHUNK_HEADER
{
	UINT16 TriangleIndices[1];
};

struct BLZ_CHUNK_MOCV:public BLZ_CHUNK_HEADER
{
	UINT32 VertexColors[1];
};

struct WMOLiquidInfo
{
	UINT32 		XVertexCount;				//number of X vertices (xverts)
	UINT32 		YVertexCount;				//number of Y vertices (yverts)
	UINT32 		XTileCount;					//number of X tiles (xtiles = xverts-1)
	UINT32 		YTileCount;					//number of Y tiles (ytiles = yverts-1)
	CD3DVector3 BaseCoordinates;
	UINT16 		MaterialID;
};

struct BLZ_CHUNK_MLIQ:public BLZ_CHUNK_HEADER
{
	WMOLiquidInfo LiquidInfos[1];
};


struct BLZ_CHUNK_MHDR:public BLZ_CHUNK_HEADER
{
	UINT32 flags;		// &1: MFBO, &2: unknown. in some Northrend ones.
	UINT32 MCINChunkOffset;		
	UINT32 MTEXChunkOffset;		
	UINT32 MMDXChunkOffset;		
	UINT32 MMIDChunkOffset;		
	UINT32 MWMOChunkOffset;		
	UINT32 MWIDChunkOffset;		
	UINT32 MDDFChunkOffset;		
	UINT32 MODFChunkOffset;	
	UINT32 MFBOChunkOffset; 		// tbc, wotlk; only when flags&1
	UINT32 MH2OChunkOffset;		// wotlk
	UINT32 MTFXChunkOffset;		// wotlk
	UINT32 PAD4;		
	UINT32 PAD5;		
	UINT32 PAD6;		
	UINT32 PAD7;	
};

struct MCNKChunkInfo
{
	UINT32 ChunkOffset;				// absolute offset.
	UINT32 Size;					// the size of the MCNK chunk, this is refering to.
	UINT32 UnusedFlags;				// these two are always 0. only set in the client.
	UINT32 UnusedAsyncID;
};

struct BLZ_CHUNK_MCIN:public BLZ_CHUNK_HEADER
{
	MCNKChunkInfo MCNKChunkList[256];
};


struct BLZ_CHUNK_MTEX:public BLZ_CHUNK_HEADER
{
	char TextureFileNames[1];
};

struct BLZ_CHUNK_MMDX:public BLZ_CHUNK_HEADER
{
	char M2FileNames[1];
};

struct BLZ_CHUNK_MMID:public BLZ_CHUNK_HEADER
{
	UINT32 M2FileNameIndices[1];
};

struct BLZ_CHUNK_MWMO:public BLZ_CHUNK_HEADER
{
	char WMOFileNames[1];
};

struct BLZ_CHUNK_MWID:public BLZ_CHUNK_HEADER
{
	UINT32 WMOFileNameIndices[1];
};

struct ADT_M2_OBJECT_INFO
{
	UINT32 			Index;			//ID (index in the MMID list)
	UINT32 			ID;				//unique identifier for this instance
	CD3DVector3 	Position;		// (X,Y,Z)
	CD3DVector3 	Orientation;	//(A,B,C)
	UINT16 			Scale;			// factor * 1024 (it's scale / 1024 for the other way around)
	UINT16 			Flags;			//, known: &1 (sets the internal flags to 0x801 instead of 0x1. (WDOODADDEF.var0xC))
};

struct BLZ_CHUNK_MDDF:public BLZ_CHUNK_HEADER
{
	ADT_M2_OBJECT_INFO	M2Objects[1];
};

struct ADT_WMO_OBJECT_INFO
{
	UINT32 			Index;			//ID (index in the MWID list)
	UINT32 			ID;				//unique identifier for this instance
	CD3DVector3 	Position;		// (X,Y,Z)
	CD3DVector3 	Orientation;	// (A,B,C)
	CD3DVector3 	UpperExtents;
	CD3DVector3 	LowerExtents;
	UINT16 			Flags;
	UINT16 			DoodadSet;
	UINT16 			NameSet;
	UINT16 			Unknown;
};

struct BLZ_CHUNK_MODF:public BLZ_CHUNK_HEADER
{
	ADT_WMO_OBJECT_INFO	WMOObjects[1];
};

struct MH2OHeader
{
	UINT32	InformationOffset;			//An offset to the MH2O_Information struct(s) for this chunk.
	UINT32	LayerCount;					//0 if the chunk has no liquids. If > 1, the offsets will point to arrays.
	UINT32	RenderOffset;				//An offset to the 64 bit render mask.
};

struct MH2OInfo
{
	INT16	LiquidType;			//Points to LiquidType.dbc
	UINT16	Flags;			
	FLOAT	HeightLevel1;		//The global liquid-height of this chunk. Which is always in there twice. Blizzard knows why.
	FLOAT	HeightLevel2;		//(Actually these values are not always identical, I think they define the highest and lowest points in the heightmap)
	BYTE	XOffset;			//The X offset of the liquid square (0-7)
	BYTE	YOffset;			//The Y offset of the liquid square (0-7)
	BYTE	Width;				//The width of the liquid square (1-8)
	BYTE	Height;				//The height of the liquid square (1-8)
	UINT32	Mask2Offset;		//Offset to some data.
	UINT32	HeightMapOffset;	//Offset to MH2O_HeightmapData structure for this chunk.
};

struct BLZ_CHUNK_MH2O:public BLZ_CHUNK_HEADER
{
	MH2OHeader	Header[256];
};

struct BLZ_CHUNK_MFBO:public BLZ_CHUNK_HEADER
{
	short	Maximum[3][3];
	short	Minimum[3][3];
};

struct BLZ_CHUNK_MTFX:public BLZ_CHUNK_HEADER
{
	UINT32	Mode[1];
};

struct BLZ_CHUNK_MCNK:public BLZ_CHUNK_HEADER
{
	UINT32 		Flags;
	UINT32 		IndexX;
	UINT32 		IndexY;
	UINT32 		LayerCount;							// maximum 4
	UINT32 		DoodadRefCount;
	UINT32 		MCVTOffset;
	UINT32 		MCNROffset;
	UINT32 		MCLYOffset;
	UINT32 		MCRFOffset;
	UINT32 		MCALOffset;
	UINT32 		AlphaSize;
	UINT32 		MCSHOffset;							// only with flags&0x1
	UINT32 		ShadowSize;
	UINT32 		AreaID;
	UINT32 		MapObjRefCount;
	UINT32 		Holes;
	UINT16		ReallyLowQualityTextureingMap[8];	// the content is the layer being on top, I guess.
	UINT32 		PredTex;							// 03-29-2005 By ObscuR; TODO: Investigate
	UINT32 		NoEffectDoodad;						// 03-29-2005 By ObscuR; TODO: Investigate
	UINT32 		MCSEOffset;
	UINT32 		SndEmitterCount;					//will be set to 0 in the client if ofsSndEmitters doesn't point to MCSE!
	UINT32 		MCLQOffset;
	UINT32 		LiquidSize;  						// 8 when not used; only read if >8.
	CD3DVector3 Position;
	UINT32 		MCCVOffset; 						// only with flags&0x40, had UINT32 textureId; in ObscuR's structure.
	UINT32 		Unused1; 							// will most likely be ofsNewSubChunk in next expansion, currently 0.
	UINT32 		Unused2; 							// most likely nNewSubChunkEntries/sizeNewSubChunk in next Expansion or another offset --Tigurius
};



struct BLZ_CHUNK_MCVT:public BLZ_CHUNK_HEADER
{
	FLOAT	Heights[145];
};

struct MCCVColor
{
	BYTE	Red;
	BYTE	Green;
	BYTE	Blue;
	BYTE	Alpha;
};

struct BLZ_CHUNK_MCCV:public BLZ_CHUNK_HEADER
{
	MCCVColor	Colors[145];
};

struct BLZ_CHUNK_MCNR:public BLZ_CHUNK_HEADER
{
	char	Normals[145][3];
};

struct MCLYLayerInfo
{
	UINT32	TextureID; 
	UINT32	Flags;		
	UINT32	MCALOffset;
	INT16	EffectID;			// (actually int16 and padding)
	INT16	Padding;
};



struct BLZ_CHUNK_MCLY:public BLZ_CHUNK_HEADER
{
	MCLYLayerInfo	Layers[1];
};

struct BLZ_CHUNK_MCRF:public BLZ_CHUNK_HEADER
{
	UINT32	ObjectRefs[1];
};

struct BLZ_CHUNK_MCSH:public BLZ_CHUNK_HEADER
{
	UINT64	ShadowMap[64];
};

struct BLZ_CHUNK_MCAL:public BLZ_CHUNK_HEADER
{
	BYTE	AlphaMapData[1];
};

struct MCSESoundEmitter
{
	UINT32		SoundEntriesAdvancedId;
	CD3DVector3 Position;
	CD3DVector3 Size; 					// I'm not really sure with this. I'm far too lazy to analyze this. Seems like noone ever needed these anyway.
};

struct BLZ_CHUNK_MCSE:public BLZ_CHUNK_HEADER
{
	MCSESoundEmitter	SoundEmitters[1];
};

#pragma pack(pop)

inline CD3DQuaternion ShortQuaternionToLongQuaternion(BLZ_SHORT_QUATERNION& Quaternion)
{
	return CD3DQuaternion(
		float(Quaternion.x < 0? Quaternion.x + 32768 : Quaternion.x - 32767)/ 32767.0f, 
		float(Quaternion.y < 0? Quaternion.y + 32768 : Quaternion.y - 32767)/ 32767.0f,
		float(Quaternion.z < 0? Quaternion.z + 32768 : Quaternion.z - 32767)/ 32767.0f,
		float(Quaternion.w < 0? Quaternion.w + 32768 : Quaternion.w - 32767)/ 32767.0f);
}


inline FLOAT BLZShortToFloat(short Value)
{
	return FLOAT(Value < 0? Value + 32768 : Value - 32767)/ 32767.0f;
}

inline CD3DVector3 BLZTranslationToD3D(CD3DVector3 Value)
{
	//return CD3DVector3(Value.x, Value.z, -Value.y);
	return CD3DVector3(Value.y, Value.z,-Value.x);
}

inline CD3DVector3 BLZScalingToD3D(CD3DVector3 Value)
{
	//return CD3DVector3(Value.x, Value.z, Value.y);
	return CD3DVector3(Value.y, Value.z, Value.x);
}

inline CD3DQuaternion BLZRotationToD3D(CD3DQuaternion Value)
{
	//return CD3DQuaternion(-Value.x, -Value.z, Value.y, Value.w);
	return CD3DQuaternion(-Value.y, -Value.z, Value.x, Value.w);//.GetInverse();
}

}