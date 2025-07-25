#pragma once

namespace Engine
{
	enum class COLLIDER { AABB, OBB, SPHERE, END };
	enum class MODEL { NONANIM, ANIM };
	enum class DIM { LBUTTON, RBUTTON, WHEELBUTTON };
	enum class DIMM { X, Y, WHEEL };
	enum class D3DTS { VIEW, PROJ, END };
	
	enum class RENDERGROUP{ RG_PRIORITY, RG_SHADOW, RG_NONBLEND, RG_NONLIGHT, RG_BLEND, RG_UI, RG_END };

	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };

	enum EVENT { EVN_NONE, EVN_DEAD, EVN_OFF, EVN_END };
}