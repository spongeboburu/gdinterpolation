#include "interpolation.h"

using namespace godot;

void Interpolation::_register_methods()
{
    register_method((char *)"function_name", &Interpolation::function_name);
    register_method((char *)"operation_name", &Interpolation::operation_name);
    register_method((char *)"interpolation_factor", &Interpolation::interpolation_factor);
    register_method((char *)"interpolate", &Interpolation::interpolate);
    register_method((char *)"get_value", &Interpolation::get_value);
    register_method((char *)"get_value_for_timestep", &Interpolation::get_value_for_timestep);
    register_method((char *)"get_factor", &Interpolation::get_factor);
    register_method((char *)"get_factor_for_timestep", &Interpolation::get_factor_for_timestep);
    godot_property_usage_flags usage = (godot_property_usage_flags)(GODOT_PROPERTY_USAGE_STORAGE|GODOT_PROPERTY_USAGE_EDITOR);
    register_property((char *)"function", &Interpolation::_function, (int)0,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_ENUM, "None,Back,Bezier,Boolean,Bounce,Circ,Cubic,Elastic,Expo,Linear,Quad,Quart,Quint,Sine,Square");
    register_property((char *)"operation", &Interpolation::_operation, (int)0,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_ENUM, "In,Out,InOut");
    register_property((char *)"start", &Interpolation::_start, Variant((real_t)0.0),
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "Start value");
    register_property((char *)"end", &Interpolation::_end, Variant((real_t)1.0),
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "End value");
    register_property((char *)"duration", &Interpolation::_duration, (real_t)0.0,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "Duration");
    register_property((char *)"loop", &Interpolation::_loop, (bool)false,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "Loop");
    register_property((char *)"reverse", &Interpolation::_reverse, (bool)false,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "Reverse");
    register_property((char *)"timestep", &Interpolation::_timestep, (real_t)0.0,
		      GODOT_METHOD_RPC_MODE_DISABLED, usage,
		      GODOT_PROPERTY_HINT_NONE, "Current timestep");
}

const char * FUNCTION_NAMES[Interpolation::MAX_FUNCTION] = {
    "None",
    "Back",
    "Bezier",
    "Boolean",
    "Bounce",
    "Circ",
    "Cubic",
    "Elastic",
    "Expo",
    "Linear",
    "Quad",
    "Quart",
    "Quint",
    "Sine",
    "Square",
    "Custom",
};

const char * OPERATION_NAMES[Interpolation::MAX_OPERATION] = {
    "None",
    "In",
    "Out",
    "InOut",
};

const char * Interpolation::operation_name(int operation)
{
    if (operation >= OPERATION_NONE && operation < MAX_OPERATION)
	return OPERATION_NAMES[operation];
    return "";
}
    
const char * Interpolation::function_name(int function)
{
    if (function >= FUNCTION_NONE && function < MAX_FUNCTION)
	return FUNCTION_NAMES[function];
    return "";
}


real_t Interpolation::interpolation_factor(int function, int operation, real_t tau, bool loop, bool reverse) {
    if (operation < OPERATION_NONE || operation >= MAX_OPERATION
	|| function < FUNCTION_NONE || function >= MAX_FUNCTION)
	return 0.0;
    real_t factor;
    if (loop) {
	factor = fmod(tau, 1.0);
	if (factor < 0.0)
	    factor += 1.0;
    } else if (tau < 0.0)
	factor = 0.0;
    else if (tau > 1.0)
	factor = 1.0;
    else
	factor = tau;
    if (reverse)
	factor = 1.0 - factor;
    switch (operation) {
    case OPERATION_OUT:
	factor = 1.0 - factor;
	break;
    case OPERATION_INOUT:
	factor = (factor < 0.5 ? factor * 2.0 : 2.0 - factor * 2.0);
	break;	
    }
    switch (function) {
    case FUNCTION_BACK:
	// float s = 1.70158f;
	// float postFix = t/=d;
	// return c*(postFix)*t*((s+1)*t - s) + b;
	return factor * factor * (2.70158 * factor - 1.70158);
    case FUNCTION_BEZIER:
	break;
    case FUNCTION_BOOLEAN:
	if (factor < 0.5)
	    return 0.0;
	return 1.0;
    case FUNCTION_BOUNCE:
	if (factor < 1 / 2.75) {
	    return 7.5625 * factor * factor;
	} else if (factor < 2 / 2.75) {
	    real_t postFix = factor - 1.5 / 2.75;
	    return 7.5625 * postFix * postFix + 0.75;
	} else if (factor < 2.5 / 2.75) {
	    real_t postFix = factor - 2.25 / 2.75;
	    return 7.5625 * postFix * postFix + 0.9375;
	} else {
	    real_t postFix = factor - 2.625 / 2.75;
	    return 7.5625 * postFix * postFix + 0.984375;
	}
	break;
    case FUNCTION_CIRC:
	// return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
	return -(sqrt(1 - factor * factor) - 1);
    case FUNCTION_CUBIC:
	return factor * factor * factor;
    case FUNCTION_ELASTIC:	    
	// if (t==0) return b;  if ((t/=d)==1) return b+c;  
	// float p=d*.3f;
	// float a=c; 
	// float s=p/4;
	// float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
	// return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
	if (factor > 0 && factor < 1) {
	    real_t d = 1.0; // MISSING. Required?
	    real_t p = d * 0.3;
	    real_t a = 1.0; // Since we have normalized this function it can be simplified. TODO.
	    real_t s = p / 4;
	    real_t postFix = a * pow(2, 10 * (factor - 1));
	    return -(postFix * sin((factor * d - s) * 6.283185307179586 / p ));
	}
	break;
    case FUNCTION_EXPO:
	// return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
	return (factor == 0) ? 0.0 : pow(2, 10 * (factor - 1));
    case FUNCTION_LINEAR:
	return factor;
    case FUNCTION_QUAD:
    case FUNCTION_SQUARE:
	return factor * factor;
    case FUNCTION_QUART:
	// return c*(t/=d)*t*t*t + b;
	return factor * factor * factor * factor;
    case FUNCTION_QUINT:
	// return c*(t/=d)*t*t*t*t + b;
	return factor * factor * factor * factor * factor;
    case FUNCTION_SINE:
// 	// return -c * cos(t/d * (PI/2)) + c + b;
	return sin(factor * 1.5707963267948966);
    default:
	break;
    }
    return factor;
}

Variant Interpolation::interpolate(int function, int operation, real_t tau, Variant start, Variant end, bool loop, bool reverse) {
    Variant::Type t_start = start.get_type();
    Variant::Type t_end = end.get_type();
    real_t factor = interpolation_factor(function, operation, tau, loop, reverse);
    switch (t_start) {
    case Variant::REAL:
    case Variant::INT:
    	if (t_end == Variant::REAL || t_end == Variant::INT) {
    	    real_t vstart = start;
	    real_t vend = end;
    	    real_t ret = vstart + factor * (vend - vstart);
	    // This is the exception, we don't care that t_start is an integer, return a real.
    	    return Variant(ret);
    	}
    	break;
    case Variant::VECTOR2:
    	if (t_end == Variant::VECTOR2 || t_end == Variant::VECTOR3) {
    	    Vector2 vstart = start;
    	    Vector2 vend = end;
    	    Vector2 ret = vstart + factor * (vend - vstart);
    	    return Variant(ret);
    	}
    	break;
    case Variant::VECTOR3:
    	if (t_end == Variant::VECTOR3 || t_end == Variant::VECTOR2) {
    	    Vector3 vstart = start;
    	    Vector3 vend = end;
    	    Vector3 ret = vstart + factor * (vend - vstart);
    	    return Variant(ret);
    	}
    	break;
    case Variant::RECT2:
    	if (t_end == Variant::RECT2) {
    	    Rect2 vstart = start;
    	    Rect2 vend = end;
    	    Rect2 ret = Rect2(vstart.pos + factor * (vend.pos - vstart.pos), vstart.size + factor * (vend.size - vstart.size));
    	    return Variant(ret);
    	}
    	break;
    case Variant::COLOR:
    	if (t_end == t_start || t_end == Variant::VECTOR3) {
    	    Color vstart = start;
    	    Color vend;
    	    if (t_end == Variant::VECTOR3) {
    	    	Vector3 v = end;
    	    	vend = Color(v.x, v.y, v.z, 1.0);
    	    } else
    	    	vend = end;
    	    Color ret = Color(
		vstart.r + factor * (vend.r - vstart.r),
		vstart.g + factor * (vend.g - vstart.r),
		vstart.b + factor * (vend.b - vstart.r),
		vstart.a + factor * (vend.a - vstart.r)
		);
    	    return Variant(ret);
    	}
    	break;
    case Variant::QUAT:
    	if (t_end == t_start || t_end == Variant::BASIS || t_end == Variant::TRANSFORM) {
    	    Quat vstart = start;
    	    if (t_end == Variant::TRANSFORM) {
    		Transform t = end;
    		end = t.get_basis();
    	    }
    	    Quat vend = end;
    	    return Variant(vstart.slerp(vend, factor));
    	}
    	break;
    case Variant::BASIS:
    	if (t_end == t_start || t_end == Variant::QUAT || t_end == Variant::TRANSFORM) {
    	    Quat vstart = start;
    	    if (t_end == Variant::TRANSFORM) {
    		Transform t = end;
    		end = t.get_basis();
    	    }
    	    Quat vend = end;
    	    return Variant(Basis(vstart.slerp(vend, factor)));
    	}
    	break;
    case Variant::TRANSFORM:
    	if (t_end == t_start || t_end == Variant::QUAT || t_end == Variant::BASIS || t_end == Variant::VECTOR3) {
    	    Transform tstart = start;
    	    Quat qstart = tstart.get_basis();
    	    Vector3 ostart = tstart.get_origin();
    	    Quat qend;
    	    Vector3 oend;
    	    if (t_end == Variant::TRANSFORM) {
    		Transform t = end;
    		qend = t.get_basis();
    		oend = t.get_origin();
    	    } else if (t_end == Variant::VECTOR3) {
    		qend = qstart;
    		oend = end;
    	    } else {
    		qend = end;
    		oend = ostart;
    	    }
    	    Quat q = qstart.slerp(qend, factor);
    	    Vector3 v = ostart + (oend - ostart) * factor;
    	    return Variant(Transform(Basis(q), v));
    	}
    	break;
    case Variant::TRANSFORM2D:
    	if (t_end == t_start || t_end == Variant::VECTOR2 || t_end == Variant::REAL || t_end == Variant::INT) {
    	    Transform2D tstart = start;
    	    double rstart = tstart.get_rotation();
    	    Vector2 ostart = tstart.get_origin();
    	    double rend;
    	    Vector2 oend;
    	    if (t_end == Variant::TRANSFORM2D) {
    		Transform2D t = end;
    		rend = t.get_rotation();
    		oend = t.get_origin();
    	    } else if (t_end == Variant::VECTOR2) {
    		rend = rstart;
    		oend = end;
    	    } else {
    		rend = end;
    		oend = ostart;
    	    }
    	    return Variant(Transform2D(rstart + (rend - rstart) * factor, ostart + (oend - ostart) * factor));
    	}
    	break;
    default:
    	break;
    }
    return Variant(0);
}

Interpolation::Interpolation() {
    _function = FUNCTION_NONE;
    _operation = OPERATION_NONE;
    _start = Variant(0.0);
    _end = Variant(1.0);
    _duration = 1.0;
    _loop = false;
    _reverse = false;
    _timestep = 0;
    _custom = NULL;
}

Interpolation::~Interpolation()
{
}

Variant Interpolation::get_value(void) {
    return interpolate(_function, _operation, _timestep / _duration, _start, _end, _loop, _reverse);
}

Variant Interpolation::get_value_for_timestep(real_t timestep)
{
    return interpolate(_function, _operation, timestep / _duration, _start, _end, _loop, _reverse);
}

float Interpolation::get_factor(void) {
    return interpolation_factor(_function, _operation, _timestep / _duration, _loop, _reverse);
}


float Interpolation::get_factor_for_timestep(real_t timestep)
{
    return interpolation_factor(_function, _operation, timestep / _duration, _loop, _reverse);
}
