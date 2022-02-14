#pragma once

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class Buffer_Type
{
	Vertex,
	Index
};

enum class Shader_Type
{
	Graphic,
	Compute
};

enum class Buffer_Shader_Type
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class Component_Type
{
	SceneComponent,
	ObjectComponent
};

enum class Input_Type
{
	Direct,
	Window
};

enum Key_State
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class Image_Type
{
	Atlas,
	Frame,
	Array
};

enum class Sampler_Type
{
	Point,
	Linear,
	Anisotropic
};

enum class RenderState_Type
{
	Blend,
	Rasterizer,
	DepthStencil,
	Max
};

enum class CreateObject_Type
{
	GameObject,
	Player
};

enum class CreateComponent_Type
{
	Sprite,
	Static
};

enum Collision_Channel
{
	Object,
	Player,
	PlayerAttack,
	Monster,
	MonsterAttack,
	Channel1,
	Channel2,
	Channel3,
	Channel4,
	Channel5,
	Channel6,
	Channel7,
	Channel8,
	Channel9,
	Channel10,
	Channel11,
	Channel12,
	Channel13,
	Channel14,
	Channel15,
	Channel16,
	End
};

enum class Collision_Interaction
{
	Ignore,
	Collision
};

enum class Collider_Type
{
	Box_2D,
	Circle,
	Pixel
};

enum class Engine_Space
{
	Space2D,
	Space3D
};

// 충돌시작 시, 충돌 끝날 시 
enum class Collision_State
{
	Begin,
	End,
	Max
};

enum class Camera_Type {
	Camera2D,
	Camera3D,
	CameraUI
};

enum class PixelCollision_Type {
	Color_Ignore,
	Color_Confirm,
	Alpha_Ignore,
	Alpha_Confirm
};

enum Button_State {
	Normal,
	MouseOn,
	Click,
	Disable,
	Max
};

enum Button_SoundState {
	BtnHover,
	BtnClick,
	BtnMax
};

enum WidgetComponent_Space {
	Screen,
	World
};

enum class ProgressBar_Dir {
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

enum class Mouse_State {
	Normal,
	State1,
	State2,
	State3,
	State4,
	State5,
	State6,
	State7,
	Max
};

enum class Tile_Type {
	Normal,
	Wall,
	End
};

enum class Tile_Shape {
	Rect,
	Rhombus
};