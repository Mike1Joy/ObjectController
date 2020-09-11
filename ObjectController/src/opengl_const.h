#pragma once

//layers - distance to camera (larger number is behind)
#define BACKGROUND 99.0f
#define CELLS_1 5.5f
#define CELLS_2 5.0f
#define ARCS 4.0f
#define STAIRS_TOP 3.5f
#define NODES 3.0f
#define STAIRS_BOT 2.5f
#define BOUNDLINES 2.3f
#define OBJECT_prev 2.1f
#define OBJECT_0 1.001f
#define OBJECT_1 1.0f
#define OBJECT_2 0.999f
#define OBJECT_3 0.998f
#define UI_1 -1.0f
#define UI_2 -2.0f
#define UI_3 -3.0f
#define UI_4 -4.0f
#define TOP_LAYER -99.0f

// modes
#define GEOMETRY 0
#define OBJECT 1
#define SIMULATE 2
#define TCP_CONTROL -1

// edit / add
#define EDIT_NODE 1
#define EDIT_ARC 2
#define EDIT_BOUND 3
#define EDIT_STAIR 4

// right click menu
#define SHOW_HIDE_NODES 1
#define SHOW_HIDE_ARCS 2
#define SHOW_HIDE_BOUND 3
#define SHOW_HIDE_CELLS 4
#define SHOW_HIDE_CSPACE 5
#define SHOW_HIDE_LDCSPACE 6
#define SHOW_HIDE_SIM_DATA 7
#define SHOW_HIDE_VOS 8
#define FLIP_VIEW 9
#define SHOW_HIDE_ALL 10
#define SHOW_HIDE_PRE_CONFIGS 11
#define START_STOP_ONION_SKIN 12
#define SHOW_HIDE_POLY 13
#define SHOW_HIDE_AP 14
#define SHOW_HIDE_COR 15
#define SHOW_HIDE_SEL_OBJ 16
#define SHOW_ALL_OBJ 17

// rainbow
#define RAIN 0
#define RED 1
#define MIN_TIME 2
#define AVG_TIME 3
static constexpr float rainbow[258][3] =
{
	{ 0.0f, 0.20387f, 0.96251f },
	{ 0.0f, 0.21524f, 0.9514f },
	{ 0.0f, 0.22613f, 0.94031f },
	{ 0.0f, 0.23654f, 0.92923f },
	{ 0.0f, 0.24654f, 0.91817f },
	{ 0.0f, 0.2562f, 0.90712f },
	{ 0.0f, 0.26557f, 0.89608f },
	{ 0.0f, 0.27465f, 0.88506f },
	{ 0.0f, 0.28348f, 0.87405f },
	{ 0.0f, 0.29209f, 0.86305f },
	{ 0.0f, 0.30047f, 0.85206f },
	{ 0.0f, 0.3087f, 0.84109f },
	{ 0.0f, 0.31672f, 0.83013f },
	{ 0.0f, 0.32458f, 0.81917f },
	{ 0.0f, 0.33232f, 0.80823f },
	{ 0.0f, 0.3399f, 0.7973f },
	{ 0.0f, 0.34736f, 0.78638f },
	{ 0.0f, 0.3547f, 0.77546f },
	{ 0.0f, 0.36191f, 0.76456f },
	{ 0.0f, 0.36902f, 0.75367f },
	{ 0.0f, 0.37602f, 0.7428f },
	{ 0.0f, 0.38291f, 0.73193f },
	{ 0.0f, 0.38969f, 0.72108f },
	{ 0.0f, 0.39636f, 0.71026f },
	{ 0.0f, 0.40292f, 0.69946f },
	{ 0.0f, 0.40934f, 0.68872f },
	{ 0.0f, 0.41561f, 0.67802f },
	{ 0.0f, 0.42172f, 0.66738f },
	{ 0.0f, 0.42768f, 0.65684f },
	{ 0.0f, 0.43342f, 0.64639f },
	{ 0.0f, 0.43896f, 0.63605f },
	{ 0.0f, 0.44432f, 0.62583f },
	{ 0.0f, 0.44945f, 0.61575f },
	{ 0.0f, 0.45438f, 0.60579f },
	{ 0.0f, 0.45911f, 0.59597f },
	{ 0.0043377f, 0.46367f, 0.58627f },
	{ 0.029615f, 0.46807f, 0.57668f },
	{ 0.055795f, 0.47235f, 0.56717f },
	{ 0.077065f, 0.47652f, 0.55774f },
	{ 0.095292f, 0.48061f, 0.54837f },
	{ 0.11119f, 0.48465f, 0.53903f },
	{ 0.1253f, 0.48865f, 0.52971f },
	{ 0.13799f, 0.49262f, 0.5204f },
	{ 0.14937f, 0.49658f, 0.5111f },
	{ 0.15963f, 0.50055f, 0.50179f },
	{ 0.169f, 0.50452f, 0.49244f },
	{ 0.17747f, 0.50849f, 0.48309f },
	{ 0.18517f, 0.51246f, 0.4737f },
	{ 0.19217f, 0.51645f, 0.46429f },
	{ 0.19856f, 0.52046f, 0.45483f },
	{ 0.20443f, 0.52448f, 0.44531f },
	{ 0.20974f, 0.52851f, 0.43577f },
	{ 0.21461f, 0.53255f, 0.42616f },
	{ 0.21905f, 0.53661f, 0.41651f },
	{ 0.22309f, 0.54066f, 0.40679f },
	{ 0.22674f, 0.54474f, 0.397f },
	{ 0.23002f, 0.54883f, 0.38713f },
	{ 0.233f, 0.55292f, 0.3772f },
	{ 0.23568f, 0.55703f, 0.36716f },
	{ 0.23802f, 0.56114f, 0.35704f },
	{ 0.24006f, 0.56526f, 0.34678f },
	{ 0.24185f, 0.56939f, 0.3364f },
	{ 0.24334f, 0.57354f, 0.32588f },
	{ 0.24458f, 0.57769f, 0.31523f },
	{ 0.24556f, 0.58185f, 0.30439f },
	{ 0.2463f, 0.58603f, 0.29336f },
	{ 0.2468f, 0.59019f, 0.28214f },
	{ 0.24707f, 0.59438f, 0.27067f },
	{ 0.24714f, 0.59856f, 0.25896f },
	{ 0.24703f, 0.60275f, 0.24696f },
	{ 0.24679f, 0.60693f, 0.23473f },
	{ 0.24647f, 0.61109f, 0.22216f },
	{ 0.24615f, 0.61523f, 0.20936f },
	{ 0.24595f, 0.61936f, 0.19632f },
	{ 0.246f, 0.62342f, 0.18304f },
	{ 0.24644f, 0.62742f, 0.16969f },
	{ 0.24748f, 0.63135f, 0.1563f },
	{ 0.24925f, 0.63518f, 0.14299f },
	{ 0.25196f, 0.6389f, 0.13001f },
	{ 0.2557f, 0.64249f, 0.11741f },
	{ 0.26057f, 0.64594f, 0.10557f },
	{ 0.26659f, 0.64926f, 0.094696f },
	{ 0.27372f, 0.65242f, 0.084904f },
	{ 0.28182f, 0.65545f, 0.076489f },
	{ 0.29078f, 0.65835f, 0.069753f },
	{ 0.30043f, 0.66113f, 0.064513f },
	{ 0.31061f, 0.66383f, 0.060865f },
	{ 0.32112f, 0.66642f, 0.058721f },
	{ 0.33186f, 0.66896f, 0.057692f },
	{ 0.34272f, 0.67144f, 0.057693f },
	{ 0.35356f, 0.67388f, 0.058443f },
	{ 0.36439f, 0.67628f, 0.059738f },
	{ 0.37512f, 0.67866f, 0.061142f },
	{ 0.38575f, 0.68102f, 0.062974f },
	{ 0.39627f, 0.68337f, 0.064759f },
	{ 0.40666f, 0.68571f, 0.066664f },
	{ 0.41692f, 0.68803f, 0.068644f },
	{ 0.42707f, 0.69034f, 0.070512f },
	{ 0.43709f, 0.69266f, 0.072423f },
	{ 0.44701f, 0.69494f, 0.074359f },
	{ 0.45683f, 0.69723f, 0.076211f },
	{ 0.46657f, 0.6995f, 0.07809f },
	{ 0.47621f, 0.70177f, 0.079998f },
	{ 0.48577f, 0.70403f, 0.081943f },
	{ 0.49527f, 0.70629f, 0.083778f },
	{ 0.5047f, 0.70853f, 0.085565f },
	{ 0.51405f, 0.71076f, 0.087502f },
	{ 0.52335f, 0.71298f, 0.089316f },
	{ 0.53259f, 0.7152f, 0.091171f },
	{ 0.54176f, 0.7174f, 0.092931f },
	{ 0.5509f, 0.7196f, 0.094839f },
	{ 0.55999f, 0.72178f, 0.096566f },
	{ 0.56902f, 0.72396f, 0.098445f },
	{ 0.57802f, 0.72613f, 0.10023f },
	{ 0.58698f, 0.72828f, 0.10204f },
	{ 0.5959f, 0.73044f, 0.10385f },
	{ 0.60479f, 0.73258f, 0.10564f },
	{ 0.61363f, 0.73471f, 0.10744f },
	{ 0.62246f, 0.73683f, 0.10925f },
	{ 0.63125f, 0.73895f, 0.11102f },
	{ 0.64001f, 0.74104f, 0.11282f },
	{ 0.64874f, 0.74315f, 0.11452f },
	{ 0.65745f, 0.74523f, 0.11636f },
	{ 0.66613f, 0.74731f, 0.11813f },
	{ 0.67479f, 0.74937f, 0.11986f },
	{ 0.68343f, 0.75144f, 0.12161f },
	{ 0.69205f, 0.75348f, 0.12338f },
	{ 0.70065f, 0.75552f, 0.12517f },
	{ 0.70923f, 0.75755f, 0.12691f },
	{ 0.71779f, 0.75957f, 0.12868f },
	{ 0.72633f, 0.76158f, 0.13048f },
	{ 0.73487f, 0.76359f, 0.13221f },
	{ 0.74338f, 0.76559f, 0.13396f },
	{ 0.75188f, 0.76756f, 0.13568f },
	{ 0.76037f, 0.76954f, 0.13747f },
	{ 0.76884f, 0.77151f, 0.13917f },
	{ 0.77731f, 0.77346f, 0.14097f },
	{ 0.78576f, 0.77541f, 0.14269f },
	{ 0.7942f, 0.77736f, 0.14444f },
	{ 0.80262f, 0.77928f, 0.14617f },
	{ 0.81105f, 0.7812f, 0.14791f },
	{ 0.81945f, 0.78311f, 0.14967f },
	{ 0.82786f, 0.78502f, 0.15138f },
	{ 0.83626f, 0.78691f, 0.15311f },
	{ 0.84465f, 0.7888f, 0.15486f },
	{ 0.85304f, 0.79066f, 0.15662f },
	{ 0.86141f, 0.79251f, 0.15835f },
	{ 0.86978f, 0.79434f, 0.16002f },
	{ 0.87814f, 0.79612f, 0.16178f },
	{ 0.88647f, 0.79786f, 0.16346f },
	{ 0.89477f, 0.79952f, 0.16507f },
	{ 0.90301f, 0.80106f, 0.1667f },
	{ 0.91115f, 0.80245f, 0.16819f },
	{ 0.91917f, 0.80364f, 0.16964f },
	{ 0.92701f, 0.80456f, 0.1709f },
	{ 0.93459f, 0.80514f, 0.172f },
	{ 0.94185f, 0.80532f, 0.17289f },
	{ 0.94869f, 0.80504f, 0.17355f },
	{ 0.95506f, 0.80424f, 0.17392f },
	{ 0.96088f, 0.80289f, 0.17399f },
	{ 0.96609f, 0.80097f, 0.17375f },
	{ 0.97069f, 0.7985f, 0.17319f },
	{ 0.97465f, 0.79549f, 0.17234f },
	{ 0.97801f, 0.79201f, 0.17121f },
	{ 0.98082f, 0.7881f, 0.16986f },
	{ 0.98314f, 0.78384f, 0.16825f },
	{ 0.98504f, 0.77928f, 0.16652f },
	{ 0.9866f, 0.7745f, 0.16463f },
	{ 0.98789f, 0.76955f, 0.16265f },
	{ 0.98897f, 0.76449f, 0.16056f },
	{ 0.9899f, 0.75932f, 0.15848f },
	{ 0.99072f, 0.75411f, 0.15634f },
	{ 0.99146f, 0.74885f, 0.15414f },
	{ 0.99214f, 0.74356f, 0.15196f },
	{ 0.99279f, 0.73825f, 0.14981f },
	{ 0.9934f, 0.73293f, 0.1476f },
	{ 0.99398f, 0.72759f, 0.14543f },
	{ 0.99454f, 0.72224f, 0.1432f },
	{ 0.99509f, 0.71689f, 0.14103f },
	{ 0.99562f, 0.71152f, 0.1388f },
	{ 0.99613f, 0.70614f, 0.13659f },
	{ 0.99662f, 0.70075f, 0.13444f },
	{ 0.9971f, 0.69534f, 0.13223f },
	{ 0.99755f, 0.68993f, 0.13006f },
	{ 0.998f, 0.6845f, 0.12783f },
	{ 0.99842f, 0.67906f, 0.12564f },
	{ 0.99883f, 0.67361f, 0.1234f },
	{ 0.99922f, 0.66815f, 0.12119f },
	{ 0.99959f, 0.66267f, 0.11904f },
	{ 0.99994f, 0.65717f, 0.11682f },
	{ 1.0f, 0.65166f, 0.11458f },
	{ 1.0f, 0.64613f, 0.11244f },
	{ 1.0f, 0.64059f, 0.11024f },
	{ 1.0f, 0.63503f, 0.10797f },
	{ 1.0f, 0.62945f, 0.1058f },
	{ 1.0f, 0.62386f, 0.1036f },
	{ 1.0f, 0.61825f, 0.10135f },
	{ 1.0f, 0.61261f, 0.099135f },
	{ 1.0f, 0.60697f, 0.096882f },
	{ 1.0f, 0.6013f, 0.094743f },
	{ 1.0f, 0.59561f, 0.092465f },
	{ 1.0f, 0.58989f, 0.090257f },
	{ 1.0f, 0.58416f, 0.088032f },
	{ 1.0f, 0.5784f, 0.085726f },
	{ 1.0f, 0.57263f, 0.083542f },
	{ 1.0f, 0.56682f, 0.081316f },
	{ 1.0f, 0.56098f, 0.079004f },
	{ 1.0f, 0.55513f, 0.076745f },
	{ 1.0f, 0.54925f, 0.07453f },
	{ 1.0f, 0.54333f, 0.072245f },
	{ 1.0f, 0.53739f, 0.070004f },
	{ 1.0f, 0.53141f, 0.067732f },
	{ 1.0f, 0.52541f, 0.065424f },
	{ 1.0f, 0.51937f, 0.06318f },
	{ 1.0f, 0.5133f, 0.06081f },
	{ 1.0f, 0.50718f, 0.058502f },
	{ 1.0f, 0.50104f, 0.056232f },
	{ 1.0f, 0.49486f, 0.053826f },
	{ 1.0f, 0.48863f, 0.051494f },
	{ 1.0f, 0.48236f, 0.049242f },
	{ 1.0f, 0.47605f, 0.046828f },
	{ 1.0f, 0.46969f, 0.044447f },
	{ 1.0f, 0.46327f, 0.042093f },
	{ 1.0f, 0.45681f, 0.039648f },
	{ 1.0f, 0.45031f, 0.037261f },
	{ 1.0f, 0.44374f, 0.034882f },
	{ 1.0f, 0.43712f, 0.032495f },
	{ 1.0f, 0.43043f, 0.030303f },
	{ 1.0f, 0.42367f, 0.02818f },
	{ 1.0f, 0.41686f, 0.026121f },
	{ 1.0f, 0.40997f, 0.024126f },
	{ 1.0f, 0.40299f, 0.022194f },
	{ 1.0f, 0.39595f, 0.020325f },
	{ 1.0f, 0.38882f, 0.018517f },
	{ 0.99994f, 0.38159f, 0.016771f },
	{ 0.99961f, 0.37428f, 0.015085f },
	{ 0.99927f, 0.36685f, 0.013457f },
	{ 0.99892f, 0.35932f, 0.011916f },
	{ 0.99855f, 0.35167f, 0.010169f },
	{ 0.99817f, 0.3439f, 0.0087437f },
	{ 0.99778f, 0.336f, 0.0073541f },
	{ 0.99738f, 0.32796f, 0.0060199f },
	{ 0.99696f, 0.31976f, 0.0047429f },
	{ 0.99653f, 0.31138f, 0.0035217f },
	{ 0.99609f, 0.30282f, 0.0023557f },
	{ 0.99563f, 0.29407f, 0.0012445f },
	{ 0.99517f, 0.2851f, 0.00018742f },
	{ 0.99469f, 0.27591f, 0.0f },
	{ 0.9942f, 0.26642f, 0.0f },
	{ 0.99369f, 0.25664f, 0.0f },
	{ 0.99318f, 0.24652f, 0.0f },
	{ 0.99265f, 0.23605f, 0.0f },
	{ 0.99211f, 0.22511f, 0.0f },
	{ 0.99155f, 0.2137f, 0.0f },
	{ 0.99099f, 0.20169f, 0.0f },
	{ 0.99041f, 0.18903f, 0.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f }
};
static constexpr float red[258][3] =
{
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.996078431372549f, 0.996078431372549f },
	{ 1.0f, 0.992156862745098f, 0.992156862745098f },
	{ 1.0f, 0.988235294117647f, 0.988235294117647f },
	{ 1.0f, 0.984313725490196f, 0.984313725490196f },
	{ 1.0f, 0.980392156862745f, 0.980392156862745f },
	{ 1.0f, 0.976470588235294f, 0.976470588235294f },
	{ 1.0f, 0.972549019607843f, 0.972549019607843f },
	{ 1.0f, 0.968627450980392f, 0.968627450980392f },
	{ 1.0f, 0.964705882352941f, 0.964705882352941f },
	{ 1.0f, 0.96078431372549f, 0.96078431372549f },
	{ 1.0f, 0.956862745098039f, 0.956862745098039f },
	{ 1.0f, 0.952941176470588f, 0.952941176470588f },
	{ 1.0f, 0.949019607843137f, 0.949019607843137f },
	{ 1.0f, 0.945098039215686f, 0.945098039215686f },
	{ 1.0f, 0.941176470588235f, 0.941176470588235f },
	{ 1.0f, 0.937254901960784f, 0.937254901960784f },
	{ 1.0f, 0.933333333333333f, 0.933333333333333f },
	{ 1.0f, 0.929411764705882f, 0.929411764705882f },
	{ 1.0f, 0.925490196078431f, 0.925490196078431f },
	{ 1.0f, 0.92156862745098f, 0.92156862745098f },
	{ 1.0f, 0.917647058823529f, 0.917647058823529f },
	{ 1.0f, 0.913725490196078f, 0.913725490196078f },
	{ 1.0f, 0.909803921568627f, 0.909803921568627f },
	{ 1.0f, 0.905882352941176f, 0.905882352941176f },
	{ 1.0f, 0.901960784313726f, 0.901960784313726f },
	{ 1.0f, 0.898039215686275f, 0.898039215686275f },
	{ 1.0f, 0.894117647058824f, 0.894117647058824f },
	{ 1.0f, 0.890196078431372f, 0.890196078431372f },
	{ 1.0f, 0.886274509803922f, 0.886274509803922f },
	{ 1.0f, 0.882352941176471f, 0.882352941176471f },
	{ 1.0f, 0.87843137254902f, 0.87843137254902f },
	{ 1.0f, 0.874509803921569f, 0.874509803921569f },
	{ 1.0f, 0.870588235294118f, 0.870588235294118f },
	{ 1.0f, 0.866666666666667f, 0.866666666666667f },
	{ 1.0f, 0.862745098039216f, 0.862745098039216f },
	{ 1.0f, 0.858823529411765f, 0.858823529411765f },
	{ 1.0f, 0.854901960784314f, 0.854901960784314f },
	{ 1.0f, 0.850980392156863f, 0.850980392156863f },
	{ 1.0f, 0.847058823529412f, 0.847058823529412f },
	{ 1.0f, 0.843137254901961f, 0.843137254901961f },
	{ 1.0f, 0.83921568627451f, 0.83921568627451f },
	{ 1.0f, 0.835294117647059f, 0.835294117647059f },
	{ 1.0f, 0.831372549019608f, 0.831372549019608f },
	{ 1.0f, 0.827450980392157f, 0.827450980392157f },
	{ 1.0f, 0.823529411764706f, 0.823529411764706f },
	{ 1.0f, 0.819607843137255f, 0.819607843137255f },
	{ 1.0f, 0.815686274509804f, 0.815686274509804f },
	{ 1.0f, 0.811764705882353f, 0.811764705882353f },
	{ 1.0f, 0.807843137254902f, 0.807843137254902f },
	{ 1.0f, 0.803921568627451f, 0.803921568627451f },
	{ 1.0f, 0.8f, 0.8f },
	{ 1.0f, 0.796078431372549f, 0.796078431372549f },
	{ 1.0f, 0.792156862745098f, 0.792156862745098f },
	{ 1.0f, 0.788235294117647f, 0.788235294117647f },
	{ 1.0f, 0.784313725490196f, 0.784313725490196f },
	{ 1.0f, 0.780392156862745f, 0.780392156862745f },
	{ 1.0f, 0.776470588235294f, 0.776470588235294f },
	{ 1.0f, 0.772549019607843f, 0.772549019607843f },
	{ 1.0f, 0.768627450980392f, 0.768627450980392f },
	{ 1.0f, 0.764705882352941f, 0.764705882352941f },
	{ 1.0f, 0.76078431372549f, 0.76078431372549f },
	{ 1.0f, 0.756862745098039f, 0.756862745098039f },
	{ 1.0f, 0.752941176470588f, 0.752941176470588f },
	{ 1.0f, 0.749019607843137f, 0.749019607843137f },
	{ 1.0f, 0.745098039215686f, 0.745098039215686f },
	{ 1.0f, 0.741176470588235f, 0.741176470588235f },
	{ 1.0f, 0.737254901960784f, 0.737254901960784f },
	{ 1.0f, 0.733333333333333f, 0.733333333333333f },
	{ 1.0f, 0.729411764705882f, 0.729411764705882f },
	{ 1.0f, 0.725490196078431f, 0.725490196078431f },
	{ 1.0f, 0.72156862745098f, 0.72156862745098f },
	{ 1.0f, 0.717647058823529f, 0.717647058823529f },
	{ 1.0f, 0.713725490196078f, 0.713725490196078f },
	{ 1.0f, 0.709803921568627f, 0.709803921568627f },
	{ 1.0f, 0.705882352941177f, 0.705882352941177f },
	{ 1.0f, 0.701960784313725f, 0.701960784313725f },
	{ 1.0f, 0.698039215686274f, 0.698039215686274f },
	{ 1.0f, 0.694117647058824f, 0.694117647058824f },
	{ 1.0f, 0.690196078431373f, 0.690196078431373f },
	{ 1.0f, 0.686274509803922f, 0.686274509803922f },
	{ 1.0f, 0.682352941176471f, 0.682352941176471f },
	{ 1.0f, 0.67843137254902f, 0.67843137254902f },
	{ 1.0f, 0.674509803921569f, 0.674509803921569f },
	{ 1.0f, 0.670588235294118f, 0.670588235294118f },
	{ 1.0f, 0.666666666666667f, 0.666666666666667f },
	{ 1.0f, 0.662745098039216f, 0.662745098039216f },
	{ 1.0f, 0.658823529411765f, 0.658823529411765f },
	{ 1.0f, 0.654901960784314f, 0.654901960784314f },
	{ 1.0f, 0.650980392156863f, 0.650980392156863f },
	{ 1.0f, 0.647058823529412f, 0.647058823529412f },
	{ 1.0f, 0.643137254901961f, 0.643137254901961f },
	{ 1.0f, 0.63921568627451f, 0.63921568627451f },
	{ 1.0f, 0.635294117647059f, 0.635294117647059f },
	{ 1.0f, 0.631372549019608f, 0.631372549019608f },
	{ 1.0f, 0.627450980392157f, 0.627450980392157f },
	{ 1.0f, 0.623529411764706f, 0.623529411764706f },
	{ 1.0f, 0.619607843137255f, 0.619607843137255f },
	{ 1.0f, 0.615686274509804f, 0.615686274509804f },
	{ 1.0f, 0.611764705882353f, 0.611764705882353f },
	{ 1.0f, 0.607843137254902f, 0.607843137254902f },
	{ 1.0f, 0.603921568627451f, 0.603921568627451f },
	{ 1.0f, 0.6f, 0.6f },
	{ 1.0f, 0.596078431372549f, 0.596078431372549f },
	{ 1.0f, 0.592156862745098f, 0.592156862745098f },
	{ 1.0f, 0.588235294117647f, 0.588235294117647f },
	{ 1.0f, 0.584313725490196f, 0.584313725490196f },
	{ 1.0f, 0.580392156862745f, 0.580392156862745f },
	{ 1.0f, 0.576470588235294f, 0.576470588235294f },
	{ 1.0f, 0.572549019607843f, 0.572549019607843f },
	{ 1.0f, 0.568627450980392f, 0.568627450980392f },
	{ 1.0f, 0.564705882352941f, 0.564705882352941f },
	{ 1.0f, 0.56078431372549f, 0.56078431372549f },
	{ 1.0f, 0.556862745098039f, 0.556862745098039f },
	{ 1.0f, 0.552941176470588f, 0.552941176470588f },
	{ 1.0f, 0.549019607843137f, 0.549019607843137f },
	{ 1.0f, 0.545098039215686f, 0.545098039215686f },
	{ 1.0f, 0.541176470588235f, 0.541176470588235f },
	{ 1.0f, 0.537254901960784f, 0.537254901960784f },
	{ 1.0f, 0.533333333333333f, 0.533333333333333f },
	{ 1.0f, 0.529411764705882f, 0.529411764705882f },
	{ 1.0f, 0.525490196078431f, 0.525490196078431f },
	{ 1.0f, 0.52156862745098f, 0.52156862745098f },
	{ 1.0f, 0.517647058823529f, 0.517647058823529f },
	{ 1.0f, 0.513725490196078f, 0.513725490196078f },
	{ 1.0f, 0.509803921568627f, 0.509803921568627f },
	{ 1.0f, 0.505882352941176f, 0.505882352941176f },
	{ 1.0f, 0.501960784313725f, 0.501960784313725f },
	{ 1.0f, 0.498039215686275f, 0.498039215686275f },
	{ 1.0f, 0.494117647058824f, 0.494117647058824f },
	{ 1.0f, 0.490196078431373f, 0.490196078431373f },
	{ 1.0f, 0.486274509803922f, 0.486274509803922f },
	{ 1.0f, 0.482352941176471f, 0.482352941176471f },
	{ 1.0f, 0.47843137254902f, 0.47843137254902f },
	{ 1.0f, 0.474509803921569f, 0.474509803921569f },
	{ 1.0f, 0.470588235294118f, 0.470588235294118f },
	{ 1.0f, 0.466666666666667f, 0.466666666666667f },
	{ 1.0f, 0.462745098039216f, 0.462745098039216f },
	{ 1.0f, 0.458823529411765f, 0.458823529411765f },
	{ 1.0f, 0.454901960784314f, 0.454901960784314f },
	{ 1.0f, 0.450980392156863f, 0.450980392156863f },
	{ 1.0f, 0.447058823529412f, 0.447058823529412f },
	{ 1.0f, 0.443137254901961f, 0.443137254901961f },
	{ 1.0f, 0.43921568627451f, 0.43921568627451f },
	{ 1.0f, 0.435294117647059f, 0.435294117647059f },
	{ 1.0f, 0.431372549019608f, 0.431372549019608f },
	{ 1.0f, 0.427450980392157f, 0.427450980392157f },
	{ 1.0f, 0.423529411764706f, 0.423529411764706f },
	{ 1.0f, 0.419607843137255f, 0.419607843137255f },
	{ 1.0f, 0.415686274509804f, 0.415686274509804f },
	{ 1.0f, 0.411764705882353f, 0.411764705882353f },
	{ 1.0f, 0.407843137254902f, 0.407843137254902f },
	{ 1.0f, 0.403921568627451f, 0.403921568627451f },
	{ 1.0f, 0.4f, 0.4f },
	{ 1.0f, 0.396078431372549f, 0.396078431372549f },
	{ 1.0f, 0.392156862745098f, 0.392156862745098f },
	{ 1.0f, 0.388235294117647f, 0.388235294117647f },
	{ 1.0f, 0.384313725490196f, 0.384313725490196f },
	{ 1.0f, 0.380392156862745f, 0.380392156862745f },
	{ 1.0f, 0.376470588235294f, 0.376470588235294f },
	{ 1.0f, 0.372549019607843f, 0.372549019607843f },
	{ 1.0f, 0.368627450980392f, 0.368627450980392f },
	{ 1.0f, 0.364705882352941f, 0.364705882352941f },
	{ 1.0f, 0.36078431372549f, 0.36078431372549f },
	{ 1.0f, 0.356862745098039f, 0.356862745098039f },
	{ 1.0f, 0.352941176470588f, 0.352941176470588f },
	{ 1.0f, 0.349019607843137f, 0.349019607843137f },
	{ 1.0f, 0.345098039215686f, 0.345098039215686f },
	{ 1.0f, 0.341176470588235f, 0.341176470588235f },
	{ 1.0f, 0.337254901960784f, 0.337254901960784f },
	{ 1.0f, 0.333333333333333f, 0.333333333333333f },
	{ 1.0f, 0.329411764705882f, 0.329411764705882f },
	{ 1.0f, 0.325490196078431f, 0.325490196078431f },
	{ 1.0f, 0.32156862745098f, 0.32156862745098f },
	{ 1.0f, 0.317647058823529f, 0.317647058823529f },
	{ 1.0f, 0.313725490196078f, 0.313725490196078f },
	{ 1.0f, 0.309803921568627f, 0.309803921568627f },
	{ 1.0f, 0.305882352941176f, 0.305882352941176f },
	{ 1.0f, 0.301960784313725f, 0.301960784313725f },
	{ 1.0f, 0.298039215686275f, 0.298039215686275f },
	{ 1.0f, 0.294117647058824f, 0.294117647058824f },
	{ 1.0f, 0.290196078431373f, 0.290196078431373f },
	{ 1.0f, 0.286274509803922f, 0.286274509803922f },
	{ 1.0f, 0.282352941176471f, 0.282352941176471f },
	{ 1.0f, 0.27843137254902f, 0.27843137254902f },
	{ 1.0f, 0.274509803921569f, 0.274509803921569f },
	{ 1.0f, 0.270588235294118f, 0.270588235294118f },
	{ 1.0f, 0.266666666666667f, 0.266666666666667f },
	{ 1.0f, 0.262745098039216f, 0.262745098039216f },
	{ 1.0f, 0.258823529411765f, 0.258823529411765f },
	{ 1.0f, 0.254901960784314f, 0.254901960784314f },
	{ 1.0f, 0.250980392156863f, 0.250980392156863f },
	{ 1.0f, 0.247058823529412f, 0.247058823529412f },
	{ 1.0f, 0.243137254901961f, 0.243137254901961f },
	{ 1.0f, 0.23921568627451f, 0.23921568627451f },
	{ 1.0f, 0.235294117647059f, 0.235294117647059f },
	{ 1.0f, 0.231372549019608f, 0.231372549019608f },
	{ 1.0f, 0.227450980392157f, 0.227450980392157f },
	{ 1.0f, 0.223529411764706f, 0.223529411764706f },
	{ 1.0f, 0.219607843137255f, 0.219607843137255f },
	{ 1.0f, 0.215686274509804f, 0.215686274509804f },
	{ 1.0f, 0.211764705882353f, 0.211764705882353f },
	{ 1.0f, 0.207843137254902f, 0.207843137254902f },
	{ 1.0f, 0.203921568627451f, 0.203921568627451f },
	{ 1.0f, 0.2f, 0.2f },
	{ 1.0f, 0.196078431372549f, 0.196078431372549f },
	{ 1.0f, 0.192156862745098f, 0.192156862745098f },
	{ 1.0f, 0.188235294117647f, 0.188235294117647f },
	{ 1.0f, 0.184313725490196f, 0.184313725490196f },
	{ 1.0f, 0.180392156862745f, 0.180392156862745f },
	{ 1.0f, 0.176470588235294f, 0.176470588235294f },
	{ 1.0f, 0.172549019607843f, 0.172549019607843f },
	{ 1.0f, 0.168627450980392f, 0.168627450980392f },
	{ 1.0f, 0.164705882352941f, 0.164705882352941f },
	{ 1.0f, 0.16078431372549f, 0.16078431372549f },
	{ 1.0f, 0.156862745098039f, 0.156862745098039f },
	{ 1.0f, 0.152941176470588f, 0.152941176470588f },
	{ 1.0f, 0.149019607843137f, 0.149019607843137f },
	{ 1.0f, 0.145098039215686f, 0.145098039215686f },
	{ 1.0f, 0.141176470588235f, 0.141176470588235f },
	{ 1.0f, 0.137254901960784f, 0.137254901960784f },
	{ 1.0f, 0.133333333333333f, 0.133333333333333f },
	{ 1.0f, 0.129411764705882f, 0.129411764705882f },
	{ 1.0f, 0.125490196078431f, 0.125490196078431f },
	{ 1.0f, 0.12156862745098f, 0.12156862745098f },
	{ 1.0f, 0.117647058823529f, 0.117647058823529f },
	{ 1.0f, 0.113725490196078f, 0.113725490196078f },
	{ 1.0f, 0.109803921568627f, 0.109803921568627f },
	{ 1.0f, 0.105882352941176f, 0.105882352941176f },
	{ 1.0f, 0.101960784313725f, 0.101960784313725f },
	{ 1.0f, 0.0980392156862745f, 0.0980392156862745f },
	{ 1.0f, 0.0941176470588235f, 0.0941176470588235f },
	{ 1.0f, 0.0901960784313725f, 0.0901960784313725f },
	{ 1.0f, 0.0862745098039216f, 0.0862745098039216f },
	{ 1.0f, 0.0823529411764706f, 0.0823529411764706f },
	{ 1.0f, 0.0784313725490196f, 0.0784313725490196f },
	{ 1.0f, 0.0745098039215686f, 0.0745098039215686f },
	{ 1.0f, 0.0705882352941176f, 0.0705882352941176f },
	{ 1.0f, 0.0666666666666667f, 0.0666666666666667f },
	{ 1.0f, 0.0627450980392157f, 0.0627450980392157f },
	{ 1.0f, 0.0588235294117647f, 0.0588235294117647f },
	{ 1.0f, 0.0549019607843137f, 0.0549019607843137f },
	{ 1.0f, 0.0509803921568627f, 0.0509803921568627f },
	{ 1.0f, 0.0470588235294118f, 0.0470588235294118f },
	{ 1.0f, 0.0431372549019608f, 0.0431372549019608f },
	{ 1.0f, 0.0392156862745098f, 0.0392156862745098f },
	{ 1.0f, 0.0352941176470588f, 0.0352941176470588f },
	{ 1.0f, 0.0313725490196078f, 0.0313725490196078f },
	{ 1.0f, 0.0274509803921569f, 0.0274509803921569f },
	{ 1.0f, 0.0235294117647059f, 0.0235294117647059f },
	{ 1.0f, 0.0196078431372549f, 0.0196078431372549f },
	{ 1.0f, 0.0156862745098039f, 0.0156862745098039f },
	{ 1.0f, 0.0117647058823529f, 0.0117647058823529f },
	{ 1.0f, 0.00784313725490196f, 0.00784313725490196f },
	{ 1.0f, 0.00392156862745098f, 0.00392156862745098f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f }
};
